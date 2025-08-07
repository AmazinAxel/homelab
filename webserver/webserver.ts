import { createServer, IncomingMessage, ServerResponse } from 'http';
import Database from 'better-sqlite3';
import { readdirSync } from 'fs';

const usbDrives = readdirSync('/media/', { withFileTypes: true })
  .filter(d => d.isDirectory());

if (usbDrives.length !== 1)
  throw new Error('Improper drive amount detected');

const db = new Database('/media/' + usbDrives[0].name + '/airQuality.db');

// Init sqlite database
db.exec(`
  CREATE TABLE IF NOT EXISTS airquality (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    pm25 REAL NOT NULL,
    pm10 REAL NOT NULL,
    timestamp INTEGER NOT NULL
  )
`);

function send(res: ServerResponse, status: number, body: string) {
  res.writeHead(status, { 'Content-Type': 'text/html' });
  res.end(body);
}

function handlePost(req: IncomingMessage, res: ServerResponse) {
  let body = '';
  req.on('data', chunk => body += chunk);
  req.on('end', () => {
    try {
      const data = JSON.parse(body);
      const { pm25, pm10 } = data;
      if (typeof pm25 !== 'number' || typeof pm10 !== 'number')
        return send(res, 400, 'Invalid data');

      const timestamp = Date.now();
      db.prepare('INSERT INTO airquality (pm25, pm10, timestamp) VALUES (?, ?, ?)')
        .run(pm25, pm10, timestamp);

      send(res, 200, 'Success');
    } catch {
      send(res, 400, 'Invalid data');
    };
  });
};

type AirQualityData = {
  pm25: number;
  pm10: number;
  timestamp: number;
};

function handleGet(res: ServerResponse) {
  const data = db.prepare('SELECT pm25, pm10, timestamp FROM airquality ORDER BY id DESC LIMIT 1').get() as AirQualityData;
  if (!data) return send(res, 200, 'No data yet');

  send(res, 200, `
    <p><strong>PM2.5:</strong> ${data.pm25}</p>
    <p><strong>PM10:</strong> ${data.pm10}</p>
    <p>At ${new Date(Number(data.timestamp)).toLocaleString()}</p>
  `);
}

createServer((req, res) => {
  (req.method === 'POST') ?
    handlePost(req, res) :
    handleGet(res);
}).listen(80);
