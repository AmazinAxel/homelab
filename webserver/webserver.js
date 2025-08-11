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
    pm25 INTEGER NOT NULL,
    pm10 INTEGER NOT NULL,
    timestamp INTEGER PRIMARY KEY
  );
  CREATE INDEX IF NOT EXISTS idx_timestamp ON airquality(timestamp DESC);
`);

function send(res, status, body) {
  res.writeHead(status, { 'Content-Type': 'text/html' });
  res.end(body);
}

const insert = db.prepare('INSERT INTO airquality (pm25, pm10, timestamp) VALUES (?, ?, ?)');
const latest = db.prepare('SELECT pm25, pm10, timestamp FROM airquality ORDER BY timestamp DESC LIMIT 1');

function handlePost(req, res) {
  let body = '';
  req.on('data', chunk => body += chunk);
  req.on('end', () => {
    try {
      const { pm25, pm10 } = JSON.parse(body);
      if (typeof pm25 !== 'number' || typeof pm10 !== 'number')
        return send(res, 400, 'Invalid data');

      const now = Date.now();
      const last = latest.get();
      if (last && now - last.timestamp < 540_000)
        return send(res, 429, 'Rate limited');

      insert.run(pm25, pm10, now);
      send(res, 200, 'Success');
    } catch {
      send(res, 400, 'Invalid data');
    };
  });
};

function handleGet(res) {
  const data = latest.get();
  if (!data) return send(res, 200, 'No data yet');

  send(res, 200, `
    <p>PM2.5: ${data.pm25}</p>
    <p>PM10: ${data.pm10}</p>
    <p>At ${new Date(data.timestamp).toLocaleString()}</p>
  `);
}

createServer((req, res) => {
  (req.method === 'POST') ?
    handlePost(req, res) :
    handleGet(res);
}).listen(80);
