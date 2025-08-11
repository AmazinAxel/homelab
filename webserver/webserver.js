import { createServer } from 'http';
import { Database } from 'bun:sqlite';
import { readdirSync } from 'fs';

const usbDrives = readdirSync('/media/', { withFileTypes: true })
  .filter(d => d.isDirectory());

if (usbDrives.length !== 1)
  throw new Error('Improper drive amount detected');

const db = new Database('/media/' + usbDrives[0].name + '/airQuality.db');

db.run(`
  CREATE TABLE IF NOT EXISTS airquality (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    pm25 REAL NOT NULL,
    pm10 REAL NOT NULL,
    timestamp INTEGER NOT NULL
  )
`);

function send(res, status, body) {
  res.writeHead(status, { 'Content-Type': 'text/html' });
  res.end(body);
}

function handlePost(req, res) {
  let body = '';
  req.on('data', chunk => body += chunk);
  req.on('end', () => {
    try {
      const data = JSON.parse(body);
      const { pm25, pm10 } = data;
      if (typeof pm25 !== 'number' || typeof pm10 !== 'number')
        return send(res, 400, 'Invalid data');

      const lastEntryStmt = db.prepare('SELECT timestamp FROM airquality ORDER BY id DESC LIMIT 1');
      const lastEntry = lastEntryStmt.get();

      const timestamp = Date.now();
      if ((lastEntry) && ((timestamp - lastEntry.timestamp) < 540000))
        return send(res, 429, 'Rate limited');

      const insertStmt = db.prepare('INSERT INTO airquality (pm25, pm10, timestamp) VALUES (?, ?, ?)');
      insertStmt.run(pm25, pm10, timestamp);

      send(res, 200, 'Success');
    } catch {
      send(res, 400, 'Invalid data');
    };
  });
};

function handleGet(res) {
  const selectStmt = db.prepare('SELECT pm25, pm10, timestamp FROM airquality ORDER BY id DESC LIMIT 1');
  const data = selectStmt.get();
  if (!data) return send(res, 200, 'No data yet');

  send(res, 200, `
    <p>PM2.5: ${data.pm25}</p>
    <p>PM10: ${data.pm10}</p>
    <p>At ${new Date(Number(data.timestamp)).toLocaleString()}</p>
  `);
}

createServer((req, res) => {
  (req.method === 'POST') ?
    handlePost(req, res) :
    handleGet(res);
}).listen(80);
