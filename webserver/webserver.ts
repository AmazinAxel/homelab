import { createServer, IncomingMessage, ServerResponse } from 'http';
import Database from 'better-sqlite3';
import { readdirSync } from 'fs';

const usbDrives = readdirSync('/media', { withFileTypes: true })
  .filter(d => d.isDirectory());

if (usbDrives.length == 1)
  throw new Error('Improper drive amount detected');

const db = new Database('/media/' + usbDrives[0].name + '/airQuality.db');

// Init sqlite database
db.exec(`
  CREATE TABLE IF NOT EXISTS airquality (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    pm25 REAL NOT NULL,
    pm10 REAL NOT NULL,
    timestamp TEXT NOT NULL
  )
`);

function send(res: ServerResponse, status: number, body: string, contentType = 'text/html') {
  res.writeHead(status, { 'Content-Type': contentType });
  res.end(body);
}

function handlePost(req: IncomingMessage, res: ServerResponse) {
  let body = '';
  req.on('data', chunk => body += chunk);
  req.on('end', () => {
    try {
      const data = JSON.parse(body);
      const { pm25, pm10 } = data;
      if (typeof pm25 !== 'number' || typeof pm10 !== 'number') {
        return send(res, 400, 'Invalid data', 'text/plain');
      }

      const timestamp = new Date().toISOString();
      db.prepare('INSERT INTO airquality (pm25, pm10, timestamp) VALUES (?, ?, ?)')
        .run(pm25, pm10, timestamp);

      send(res, 200, JSON.stringify({ status: 'success', timestamp }), 'application/json');
    } catch {
      send(res, 400, 'Invalid data', 'text/plain');
    }
  });
}

type AirQualityRes = {
  pm25: number;
  pm10: number;
  timestamp: string;
};

function handleGet(res: ServerResponse) {
  const data = db.prepare('SELECT pm25, pm10, timestamp FROM airquality ORDER BY id DESC LIMIT 1').get() as AirQualityRes;
  if (!data) return send(res, 200, 'No data yet');

  const html = `
    <p><strong>PM2.5:</strong> ${data.pm25}</p>
    <p><strong>PM10:</strong> ${data.pm10}</p>
    <p>At ${data.timestamp}</p>
  `;
  send(res, 200, html);
}

createServer((req, res) => {
  const url = new URL(req.url || '/', `http://${req.headers.host}`);
  const pathname = url.pathname;

  if (req.method === 'POST' && pathname === '/submit') {
    handlePost(req, res);
  } else if (req.method === 'GET' && pathname === '/') {
    handleGet(res);
  } else {
    send(res, 404, 'Not found', 'text/plain');
  }
}).listen(80);
