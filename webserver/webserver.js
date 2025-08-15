import { serve } from "bun";
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

const insert = db.prepare('INSERT INTO airquality (pm25, pm10, timestamp) VALUES (?, ?, ?)');
const getData = db.prepare(`
  SELECT pm25, pm10, timestamp FROM airquality
  WHERE timestamp >= ?
  ORDER BY timestamp ASC
`);

function send(status, message) {
  return new Response(message, {
    status,
    headers: { "Content-Type": "text/plain" }
  });
}

async function handlePost(req) {
  try {
    const { pm25, pm10 } = await req.json();

    if (typeof pm25 !== "number" || typeof pm10 !== "number")
      return send(400, "Invalid data");

    const now = Date.now();
    const last = getData.get();

    if (last && now - last.timestamp < 540_000)
      return send(429, "Rate limited");

    insert.run(pm25, pm10, now);
    return send(200, "Success");
  } catch {
    return send(400, "Invalid data");
  };
};

const page = await Bun.file("/home/alec/homelab/webserver/page.html").text();
const pageWithToken = page.replaceAll("AIRNOW_TOKEN", process.env.AIRNOW_TOKEN);

serve({
  port: 80,
  fetch(req) {
    const url = new URL(req.url);
    const pathname = url.pathname;

    if (req.method === "POST")
      return handlePost(req);

    else if (pathname == "/favicon.ico") {
      const file = Bun.file("favicon.ico");
      return new Response(file, {
        headers: { "Content-Type": "image/x-icon" }
      });
    } else if (pathname === "/getdata") {
      const data = getData.all(Date.now() - 86400000); // Get past 24 hours

      return new Response(JSON.stringify(data), {
        headers: { "Content-Type": "application/json" }
      });
    };

    return new Response(pageWithToken, { headers: { "Content-Type": "text/html" }});
  }
});