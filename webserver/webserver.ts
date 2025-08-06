import { createServer, IncomingMessage, ServerResponse } from 'http';
import Database from 'better-sqlite3';

const db = new Database('air_quality.db');

// Init sqlite database
db.exec(`
  CREATE TABLE IF NOT EXISTS air_quality (
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
      db.prepare('INSERT INTO air_quality (pm25, pm10, timestamp) VALUES (?, ?, ?)')
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
  const row = db.prepare('SELECT pm25, pm10, timestamp FROM air_quality ORDER BY id DESC LIMIT 1').get() as AirQualityRes;
  if (!row) return send(res, 200, 'No data yet');

  const html = `
    <p><strong>PM2.5:</strong> ${row.pm25}</p>
    <p><strong>PM10:</strong> ${row.pm10}</p>
    <p>At ${row.timestamp}</p>
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
