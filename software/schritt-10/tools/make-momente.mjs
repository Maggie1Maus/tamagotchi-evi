// Erzeugt evi-handy/momente.h: was das Board für Evis eigene Momente zusätzlich braucht.
//
//  - Evis Zeichnungen der Momente für Baby, Teenager, Erwachsen und Oma
//    (handy-app/dist/img/<phase>-*.png: laufen, fenster, denken, singen, gaehnen, drehen, huepfen,
//    staunen) im Maßstab der App (ART_SCALE in app.js: baby 0.344, teen 0.372, adult 0.38, senior 0.4),
//    nur verkleinert und in Schwarz/Weiß umgerechnet, genau wie make-assets.ps1 es mit den
//    übrigen Zeichnungen macht (Linien: deckend und dunkel; Maske: deckend). Nichts wird neu gezeichnet.
//  - Die Besuchsbilder der Nutzerin (Schmetterling 32 × 26, Vogel 23 × 20) aus handy-app/dist/besuch/
//    (aufbereitet mit handy-app/tools/prepare_besuch.py), unverändert übernommen.
//  - Die Symbole in der Gedankenwolke (22 px, Strich 1,8 wie #thought-icon in style.css)
//    und die Summ-Noten ♪ (17 px) und ♫ (14 px).
//
// Läuft in der Cloud-Sitzung (Linux, Node mit Playwright/Chromium). Auf dem Haupt-PC wird es
// nicht gebraucht: momente.h liegt fertig im Repository.
//   node software/schritt-10/tools/make-momente.mjs
import { createRequire } from 'node:module';
import { readFileSync, writeFileSync } from 'node:fs';
import { dirname, join } from 'node:path';
import { fileURLToPath } from 'node:url';

// Playwright ist in der Cloud global installiert; require() findet es, import nicht.
const { chromium } = createRequire(import.meta.url)('playwright');
const here = dirname(fileURLToPath(import.meta.url));
const root = join(here, '..', '..', '..');
const dist = join(root, 'handy-app', 'dist');
const out = join(here, '..', 'evi-handy', 'momente.h');

const MOMENTE = ['laufen', 'fenster', 'denken', 'singen', 'gaehnen', 'drehen', 'huepfen', 'staunen'];
// Phasen wie in pet.h (EI, BABY, TEEN, ERWACHSEN, OMA); das Ei hat keine Momente
const PHASEN = [['baby', 'BABY', 0.344], ['teen', 'TEEN', 0.372], ['adult', 'ERWACHSEN', 0.38], ['senior', 'OMA', 0.4]];
const ICONS = [['HERZ', 'i-heart'], ['KEKS', 'i-cookie'], ['BALL', 'i-ball'], ['NOTE', 'i-note'],
  ['FUTTER', 'i-food'], ['PUTZEN', 'i-clean'], ['SONNE', 'i-sun'], ['MOND', 'i-moon']];
const NOTEN = [['NOTE_KLEIN', '&#x266A;', 17], ['NOTEN', '&#x266B;', 14]];

const index = readFileSync(join(dist, 'index.html'), 'utf8');
const libStart = index.indexOf('<svg class="svg-library"');
const library = index.slice(libStart, index.indexOf('</svg>', libStart) + 6);

const lines = [
  '// Erzeugt von software/schritt-10/tools/make-momente.mjs - nicht von Hand ändern.',
  '// Evis Zeichnungen der Momente (Baby, Teenager, Erwachsen, Oma) und die Symbole der Gedankenwolke, aus der Handy-App.',
  '// 1 Bit pro Punkt, Zeile für Zeile, linkes Pixel im höchsten Bit. 1 = schwarz. Bild steht in assets.h.',
  '#pragma once',
  '#include "assets.h"',
  ''
];
function addArray(name, bytes) {
  lines.push(`static const uint8_t ${name}[] = {`);
  for (let i = 0; i < bytes.length; i += 20) lines.push('  ' + bytes.slice(i, i + 20).map(b => '0x' + b.toString(16).padStart(2, '0')).join(',') + ',');
  lines.push('};');
}
// Bits einer Fläche: Zeile für Zeile, links im höchsten Bit.
function bits(w, h, test) {
  const bytes = [];
  for (let y = 0; y < h; y++) for (let bx = 0; bx < w; bx += 8) {
    let b = 0;
    for (let bit = 0; bit < 8; bit++) if (bx + bit < w && test(bx + bit, y)) b |= 0x80 >> bit;
    bytes.push(b);
  }
  return bytes;
}
const lum = (d, i) => 0.299 * d[i] + 0.587 * d[i + 1] + 0.114 * d[i + 2];

const browser = await chromium.launch();
const page = await browser.newPage({ deviceScaleFactor: 1 });
await page.setContent('<!doctype html><html><body style="margin:0;background:#fff"><canvas id="c"></canvas></body></html>');

// ---------- Evis Zeichnungen ----------
const zeichnungen = {};
for (const [phase, phaseC, SCALE] of PHASEN) {
 zeichnungen[phaseC] = [];
 for (const name of MOMENTE) {
  const png = readFileSync(join(dist, 'img', `${phase}-${name}.png`)).toString('base64');
  const res = await page.evaluate(async ({ png, scale }) => {
    const img = new Image();
    img.src = `data:image/png;base64,${png}`;
    await img.decode();
    const w = Math.round(img.naturalWidth * scale), h = Math.round(img.naturalHeight * scale);
    const c = document.createElement('canvas');
    c.width = w; c.height = h;
    const g = c.getContext('2d');
    g.imageSmoothingEnabled = true;
    g.imageSmoothingQuality = 'high';
    g.drawImage(img, 0, 0, w, h);
    return { w, h, data: Array.from(g.getImageData(0, 0, w, h).data) };
  }, { png, scale: SCALE });
  const { w, h, data } = res;
  const at = (x, y) => (y * w + x) * 4;
  const cname = `ART_${phaseC}_${name.toUpperCase()}`;
  addArray(`${cname}_L`, bits(w, h, (x, y) => data[at(x, y) + 3] > 127 && lum(data, at(x, y)) < 140));
  addArray(`${cname}_M`, bits(w, h, (x, y) => data[at(x, y) + 3] > 127));
  lines.push(`static const Bild ${cname} = {${w}, ${h}, ${-Math.round(w / 2)}, ${-h}, ${cname}_L, ${cname}_M};`);
  zeichnungen[phaseC].push(cname);
 }
}
lines.push('');

// ---------- Besuch: Schmetterling und Vogel (Bilder der Nutzerin) ----------
for (const [datei, cname] of [['schmetterling', 'BESUCH_FALTER'], ['vogel', 'BESUCH_VOGEL']]) {
  const png = readFileSync(join(dist, 'besuch', `${datei}.png`)).toString('base64');
  const { w, h, data } = await page.evaluate(async png => {
    const img = new Image();
    img.src = `data:image/png;base64,${png}`;
    await img.decode();
    const c = document.createElement('canvas');
    c.width = img.naturalWidth; c.height = img.naturalHeight;
    const g = c.getContext('2d');
    g.drawImage(img, 0, 0);
    return { w: c.width, h: c.height, data: Array.from(g.getImageData(0, 0, c.width, c.height).data) };
  }, png);
  const at = (x, y) => (y * w + x) * 4;
  addArray(`${cname}_L`, bits(w, h, (x, y) => data[at(x, y) + 3] > 127 && lum(data, at(x, y)) < 140));
  addArray(`${cname}_M`, bits(w, h, (x, y) => data[at(x, y) + 3] > 127));
  lines.push(`static const Bild ${cname} = {${w}, ${h}, 0, 0, ${cname}_L, ${cname}_M};`);
}
lines.push('');

// ---------- Symbole der Gedankenwolke und Noten ----------
const cell = 32;
const teile = [
  ...ICONS.map(([, id], i) => `<svg style="position:absolute;left:${i * cell}px;top:0;width:22px;height:22px;fill:none;stroke:#000;stroke-width:1.8px;stroke-linecap:round;stroke-linejoin:round"><use href="#${id}"/></svg>`),
  ...NOTEN.map(([, zeichen, px], i) => `<div style="position:absolute;left:${i * cell}px;top:${cell}px;width:${cell}px;height:${cell}px;font:${px}px/1 system-ui,sans-serif;color:#000">${zeichen}</div>`)
];
await page.setViewportSize({ width: ICONS.length * cell, height: 2 * cell });
await page.setContent(`<!doctype html><html><head><style>body{margin:0;background:#fff}.svg-library{display:none}</style></head><body>${library}${teile.join('')}</body></html>`);
const shot = await page.screenshot({ type: 'png' });
const atlas = await page.evaluate(async ({ b64, w, h }) => {
  const img = new Image();
  img.src = `data:image/png;base64,${b64}`;
  await img.decode();
  const c = document.createElement('canvas');
  c.width = w; c.height = h;
  const g = c.getContext('2d');
  g.drawImage(img, 0, 0);
  return Array.from(g.getImageData(0, 0, w, h).data);
}, { b64: shot.toString('base64'), w: ICONS.length * cell, h: 2 * cell });
const atlasW = ICONS.length * cell;
function symbol(name, cx, cy) {
  const dark = (x, y) => lum(atlas, ((cy + y) * atlasW + cx + x) * 4) < 150;
  let minX = cell, minY = cell, maxX = -1, maxY = -1;
  for (let y = 0; y < cell; y++) for (let x = 0; x < cell; x++) if (dark(x, y)) {
    minX = Math.min(minX, x); maxX = Math.max(maxX, x); minY = Math.min(minY, y); maxY = Math.max(maxY, y);
  }
  if (maxX < 0) throw new Error(`Symbol leer: ${name}`);
  const w = maxX - minX + 1, h = maxY - minY + 1;
  addArray(`SYM_${name}_BITS`, bits(w, h, (x, y) => dark(x + minX, y + minY)));
  lines.push(`static const Bild SYM_${name} = {${w}, ${h}, 0, 0, SYM_${name}_BITS, nullptr};`);
}
ICONS.forEach(([name], i) => symbol(`G_${name}`, i * cell, 0));
NOTEN.forEach(([name], i) => symbol(name, i * cell, cell));
await browser.close();

lines.push('');
lines.push('// [Phase][Bild]: Phase wie in pet.h (EI hat keine), Bild wie MOMENT_ART in app.js:');
lines.push('// laufen, fenster, denken, singen, gaehnen, drehen, huepfen, staunen');
lines.push(`static const Bild* const MOMENT_BILD[5][${MOMENTE.length}] = {`);
lines.push(`  {${MOMENTE.map(() => 'nullptr').join(', ')}},  // Ei`);
for (const [phase, phaseC] of PHASEN) lines.push(`  {${zeichnungen[phaseC].map(n => '&' + n).join(', ')}},  // ${phase}`);
lines.push('};');
lines.push('// Gedankenwolke: herz, keks, ball, note, futter, putzen, sonne, mond');
lines.push(`static const Bild* const GEDANKE_BILD[${ICONS.length}] = {${ICONS.map(([n]) => '&SYM_G_' + n).join(', ')}};`);
writeFileSync(out, lines.join('\n') + '\n');
console.log(`momente.h geschrieben: ${Math.round(readFileSync(out).length / 1024)} KB Text`);
