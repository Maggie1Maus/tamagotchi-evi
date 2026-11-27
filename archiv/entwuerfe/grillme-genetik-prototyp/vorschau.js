const namen = { evi: 'Evi', lotti: 'Lotti', gloeckchen: 'Glöckchen', mia: 'Mia' };
const alter = { baby: 'Baby', teenager: 'Teenager', erwachsen: 'Erwachsen', senior: 'Senior' };
const breite = 72;
const hoehe = 64;
const grundform = document.getElementById('body');
const ohrspender = document.getElementById('ears');
const wechseln = document.getElementById('random');
const statusText = document.getElementById('status');
const karten = document.getElementById('cards');
const sprites = {};

function leinwand() {
  const canvas = document.createElement('canvas');
  canvas.width = breite;
  canvas.height = hoehe;
  return canvas;
}

function quelle(name, phase) {
  return name === 'evi' ? `evi-${phase}.png`
    : `../neue-tamagotchis/graustufen/${name}/${phase}-gluecklich.png`;
}

async function ladeSprite(name, phase) {
  const bild = new Image();
  bild.src = quelle(name, phase);
  try {
    await bild.decode();
  } catch {
    throw new Error(`Grafik fehlt: ${bild.getAttribute('src')}`);
  }

  const canvas = leinwand();
  const ctx = canvas.getContext('2d');
  ctx.imageSmoothingEnabled = false;
  // Parkfiguren liegen schon im Zielraster. Evis Original wird proportional verkleinert.
  if (name === 'evi') {
    const faktor = Math.min(breite / bild.width, 60 / bild.height);
    const w = Math.round(bild.width * faktor);
    const h = Math.round(bild.height * faktor);
    ctx.drawImage(bild, Math.floor((breite - w) / 2), 62 - h, w, h);
  } else {
    ctx.drawImage(bild, 0, 0);
  }
  return canvas;
}

// Polygon als ganze Pixel zeichnen. So entstehen auch am Schnittrand keine
// halb transparenten oder geglätteten Pixel.
function zeichneMaske(ctx, punkte) {
  for (let y = 0; y < hoehe; y++) {
    const schnitte = [];
    for (let i = 0; i < punkte.length; i++) {
      const [ax, ay] = punkte[i];
      const [bx, by] = punkte[(i + 1) % punkte.length];
      if ((ay > y + 0.5) === (by > y + 0.5)) continue;
      schnitte.push(ax + (y + 0.5 - ay) * (bx - ax) / (by - ay));
    }
    schnitte.sort((a, b) => a - b);
    for (let i = 0; i < schnitte.length; i += 2) {
      const links = Math.ceil(schnitte[i] - 0.5);
      const rechts = Math.ceil(schnitte[i + 1] - 0.5);
      ctx.fillRect(links, y, rechts - links, 1);
    }
  }
}

function freistellen(sprite, ohr) {
  const canvas = leinwand();
  const ctx = canvas.getContext('2d');
  zeichneMaske(ctx, ohr.kontur);
  ctx.globalCompositeOperation = 'source-in';
  ctx.drawImage(sprite, 0, 0);
  return canvas;
}

function zusammensetzen(canvas, empfaenger, spender, phase) {
  const ctx = canvas.getContext('2d');
  ctx.imageSmoothingEnabled = false;
  ctx.clearRect(0, 0, breite, hoehe);
  const original = sprites[empfaenger][phase];
  if (empfaenger === spender) {
    ctx.drawImage(original, 0, 0);
    return;
  }

  const koerper = leinwand();
  const koerperCtx = koerper.getContext('2d');
  koerperCtx.drawImage(original, 0, 0);
  koerperCtx.globalCompositeOperation = 'destination-out';
  const empfaengerOhren = ohren[empfaenger][phase];
  const spenderOhren = ohren[spender][phase];

  for (const [seite, empfaengerOhr] of empfaengerOhren.entries()) {
    zeichneMaske(koerperCtx, empfaengerOhr.kontur);
    const spenderOhr = spenderOhren[seite];
    const [faktor, dx, dy] = eviOhren[empfaenger][phase][seite];
    const [sx, sy] = spenderOhr.ansatz;
    const [ex, ey] = empfaengerOhr.ansatz;
    const teil = freistellen(sprites[spender][phase], spenderOhr);
    ctx.drawImage(teil,
      Math.round(ex - sx * faktor + dx), Math.round(ey - sy * faktor + dy),
      Math.round(breite * faktor), Math.round(hoehe * faktor));
  }
  ctx.drawImage(koerper, 0, 0);
}

function zeile(titel) {
  const ueberschrift = document.createElement('div');
  ueberschrift.className = 'row-title';
  ueberschrift.textContent = titel;
  karten.append(ueberschrift);
  return Object.fromEntries(Object.entries(alter).map(([phase, label]) => {
    const canvas = leinwand();
    canvas.setAttribute('aria-label', `${titel}, ${label}`);
    karten.append(canvas);
    return [phase, canvas];
  }));
}

for (const label of Object.values(alter)) {
  const ueberschrift = document.createElement('div');
  ueberschrift.className = 'age';
  ueberschrift.textContent = label;
  karten.append(ueberschrift);
}
const kreuzung = zeile('Vorschau');
const original = zeile('Original der Grundform');
const spender = zeile('Original des Ohrspenders');

function aktualisieren() {
  const basis = grundform.value;
  const ohrenVon = ohrspender.value;
  for (const phase of Object.keys(alter)) {
    zusammensetzen(kreuzung[phase], basis, ohrenVon, phase);
    zusammensetzen(original[phase], basis, basis, phase);
    zusammensetzen(spender[phase], ohrenVon, ohrenVon, phase);
  }
  statusText.textContent = basis === ohrenVon ? `Original von ${namen[basis]}.`
    : `${namen[basis]} mit Ohren von ${namen[ohrenVon]}. Gesicht und Schwanz gehören zur Grundform.`;
}

function setzeGrundform(basis, ohrenVon = 'evi') {
  grundform.value = basis;
  ohrspender.replaceChildren(new Option(namen[basis], basis));
  if (eviOhren[basis]) ohrspender.add(new Option(namen.evi, 'evi'));
  ohrspender.value = eviOhren[basis] ? ohrenVon : basis;
  ohrspender.disabled = ohrspender.options.length < 2;
  aktualisieren();
}

async function start() {
  await Promise.all(Object.keys(namen).map(async name => {
    const phasen = await Promise.all(Object.keys(alter).map(async phase =>
      [phase, await ladeSprite(name, phase)]));
    sprites[name] = Object.fromEntries(phasen);
  }));
  for (const [name, label] of Object.entries(namen)) grundform.add(new Option(label, name));
  grundform.addEventListener('change', () => setzeGrundform(grundform.value));
  ohrspender.addEventListener('change', aktualisieren);
  wechseln.addEventListener('click', () => {
    const alternativen = Object.keys(eviOhren).filter(basis => basis !== grundform.value);
    setzeGrundform(alternativen[Math.floor(Math.random() * alternativen.length)]);
  });
  setzeGrundform('mia');
  grundform.disabled = false;
  wechseln.disabled = false;
}

start().catch(fehler => {
  statusText.textContent = fehler.message;
});
