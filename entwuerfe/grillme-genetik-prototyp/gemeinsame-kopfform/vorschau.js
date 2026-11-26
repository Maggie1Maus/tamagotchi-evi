const liste = document.getElementById('merkmale');
const tafel = document.getElementById('tafel');
const herkunft = document.getElementById('herkunft');
const figurenIds = Object.keys(figuren);
const merkmaleIds = Object.keys(merkmale);
const auswahl = {};
const kindBilder = {};
const start = ['flocke', 'mohn', 'flocke', 'mohn', 'flocke'];

function parent(figur) {
  return Object.fromEntries(merkmaleIds.map(merkmal => [merkmal, figur]));
}

for (const [merkmal, name] of Object.entries(merkmale)) {
  const feld = document.createElement('div');
  const label = document.createElement('label');
  label.htmlFor = merkmal;
  label.textContent = name;
  const eingabe = document.createElement('select');
  eingabe.id = merkmal;
  for (const [id, figur] of Object.entries(figuren)) eingabe.add(new Option(figur, id));
  feld.append(label, eingabe);
  liste.append(feld);
  eingabe.addEventListener('change', aktualisieren);
  auswahl[merkmal] = eingabe;
}

function beschriftung(text, klasse) {
  const feld = document.createElement('div');
  feld.textContent = text;
  feld.className = klasse;
  tafel.append(feld);
}

function bild(phase, name, istKind = false) {
  const feld = document.createElement('div');
  feld.className = istKind ? 'bild kind' : 'bild';
  const canvas = document.createElement('canvas');
  canvas.width = 72;
  canvas.height = 64;
  canvas.setAttribute('role', 'img');
  canvas.setAttribute('aria-label', `${name}, ${altersstufen[phase].label}`);
  feld.append(canvas);
  tafel.append(feld);
  return canvas;
}

for (const titel of ['', figuren.mohn, 'Kind', figuren.flocke]) beschriftung(titel, 'titel');
for (const [phase, alter] of Object.entries(altersstufen)) {
  beschriftung(alter.label, 'phase');
  zeichneFigur(bild(phase, figuren.mohn), phase, parent('mohn'));
  kindBilder[phase] = bild(phase, 'Kind', true);
  zeichneFigur(bild(phase, figuren.flocke), phase, parent('flocke'));
}

function aktualisieren() {
  const kombination = Object.fromEntries(merkmaleIds.map(id => [id, auswahl[id].value]));
  for (const [phase, canvas] of Object.entries(kindBilder)) zeichneFigur(canvas, phase, kombination);
  herkunft.textContent = merkmaleIds.map(id => `${merkmale[id]} von ${figuren[kombination[id]]}`).join(' · ');
}

function setzeKombination(varianten) {
  merkmaleIds.forEach((id, index) => { auswahl[id].value = varianten[index]; });
  aktualisieren();
}

document.getElementById('zufall').addEventListener('click', () => {
  const aktuell = merkmaleIds.map(id => auswahl[id].value).join(',');
  const kombinationen = [];
  for (let bits = 1; bits < 2 ** merkmaleIds.length - 1; bits++) {
    const variante = merkmaleIds.map((_, stelle) => figurenIds[(bits >> stelle) & 1]);
    if (variante.join(',') !== aktuell) kombinationen.push(variante);
  }
  setzeKombination(kombinationen[Math.floor(Math.random() * kombinationen.length)]);
});
document.getElementById('umkehren').addEventListener('click', () => {
  setzeKombination(merkmaleIds.map(id => figurenIds.find(figur => figur !== auswahl[id].value)));
});
setzeKombination(start);
