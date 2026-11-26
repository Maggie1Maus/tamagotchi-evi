const liste = document.getElementById('merkmale');
const tafel = document.getElementById('tafel');
const galerie = document.getElementById('figuren');
const herkunft = document.getElementById('herkunft');
const emotion = document.getElementById('emotion');
const pose = document.getElementById('pose');
const animation = document.getElementById('animation');
const eltern = [document.getElementById('eltern-a'), document.getElementById('eltern-b')];
const merkmaleIds = Object.keys(merkmale);
const auswahl = {};
const reihen = {};
const rollen = ['a', 'kind', 'b'];
let frame = 0;
let kreuzung;

function elternMerkmale(figur) {
  return Object.fromEntries(merkmaleIds.map(merkmal => [merkmal, figur]));
}

function caption(text, klasse) {
  const feld = document.createElement('div');
  feld.textContent = text;
  feld.className = klasse;
  return feld;
}

function bild(istKind = false) {
  const feld = document.createElement('div');
  feld.className = istKind ? 'bild kind' : 'bild';
  const canvas = document.createElement('canvas');
  canvas.width = bilderBreite;
  canvas.height = bilderHoehe;
  canvas.setAttribute('role', 'img');
  feld.append(canvas);
  return { feld, canvas };
}

function bezeichne(canvas, name, phase) {
  canvas.setAttribute('aria-label', `${name}, ${altersstufen[phase].label}`);
}

for (const [id, figur] of Object.entries(figuren)) {
  const { feld, canvas } = bild();
  bezeichne(canvas, figur, 'erwachsen');
  feld.append(caption(figur, 'titel'));
  galerie.append(feld);
  zeichneFigur(canvas, 'erwachsen', elternMerkmale(id), 'gluecklich');
}

for (const [id,label] of Object.entries(emotionen)) emotion.add(new Option(label,id));
for (const [id,haltung] of Object.entries(posen)) pose.add(new Option(haltung.label,id));
animation.checked = !matchMedia('(prefers-reduced-motion: reduce)').matches;
animation.disabled = true;
for (const select of [emotion,pose]) {
  select.addEventListener('change', () => {
    frame = 0;
    animation.disabled = !posen[pose.value].frames;
    zeichneVergleich();
  });
}

for (const [merkmal, name] of Object.entries(merkmale)) {
  const feld = document.createElement('div');
  const label = document.createElement('label');
  label.htmlFor = merkmal;
  label.textContent = name;
  const eingabe = document.createElement('select');
  eingabe.id = merkmal;
  feld.append(label, eingabe);
  liste.append(feld);
  eingabe.addEventListener('change', aktualisiereKind);
  auswahl[merkmal] = eingabe;
}

const titel = Object.fromEntries(rollen.map(rolle => [rolle, caption(rolle === 'kind' ? 'Kind' : '', 'titel')]));
tafel.append(caption('', 'titel'), ...rollen.map(rolle => titel[rolle]));
for (const [phase, alter] of Object.entries(altersstufen)) {
  tafel.append(caption(alter.label, 'phase'));
  reihen[phase] = Object.fromEntries(rollen.map(rolle => {
    const { feld, canvas } = bild(rolle === 'kind');
    bezeichne(canvas, 'Kind', phase);
    tafel.append(feld);
    return [rolle, canvas];
  }));
}

function variante() {
  return merkmaleIds.map(id => auswahl[id].selectedIndex);
}

function setzeKombination(merkmalsWahl) {
  merkmaleIds.forEach((id, index) => { auswahl[id].selectedIndex = merkmalsWahl[index]; });
  aktualisiereKind();
}

function aktualisiereKind() {
  kreuzung = Object.fromEntries(merkmaleIds.map(id => [id, auswahl[id].value]));
  for (const [phase,reihe] of Object.entries(reihen)) male(reihe.kind,phase,kreuzung);
  herkunft.textContent = merkmaleIds.map(id => `${merkmale[id]} von ${figuren[kreuzung[id]]}`).join(' · ');
}

function male(canvas, phase, merkmale) {
  zeichneFigur(canvas,phase,merkmale,emotion.value,pose.value,frame);
}

function zeichneVergleich() {
  const [a,b] = eltern.map(select => elternMerkmale(select.value));
  for (const [phase,reihe] of Object.entries(reihen)) {
    male(reihe.a,phase,a);
    male(reihe.kind,phase,kreuzung);
    male(reihe.b,phase,b);
  }
}

function aktualisiereEltern() {
  const paar = eltern.map(select => select.value);
  const vorher = variante();
  eltern.forEach((select, index) => {
    for (const option of select.options) option.disabled = option.value === paar[1 - index];
  });
  for (const select of Object.values(auswahl)) {
    select.replaceChildren(...paar.map(id => new Option(figuren[id], id)));
  }
  for (const [index, rolle] of ['a', 'b'].entries()) {
    const id = paar[index];
    titel[rolle].textContent = figuren[id];
    for (const [phase, reihe] of Object.entries(reihen)) {
      bezeichne(reihe[rolle], figuren[id], phase);
      male(reihe[rolle], phase, elternMerkmale(id));
    }
  }
  setzeKombination(vorher[0] < 0 ? [1, 0, 1, 0, 1] : vorher);
}

for (const select of eltern) {
  for (const [id, figur] of Object.entries(figuren)) select.add(new Option(figur, id));
  select.addEventListener('change', aktualisiereEltern);
}
eltern[0].value = 'evi';
eltern[1].value = 'lotti';
aktualisiereEltern();

document.getElementById('zufall').addEventListener('click', () => {
  const aktuell = variante().join(',');
  const gemischt = [];
  for (let bits = 1; bits < 2 ** merkmaleIds.length - 1; bits++) {
    const vorschlag = merkmaleIds.map((_, stelle) => (bits >> stelle) & 1);
    if (vorschlag.join(',') !== aktuell) gemischt.push(vorschlag);
  }
  setzeKombination(gemischt[Math.floor(Math.random() * gemischt.length)]);
});
document.getElementById('umkehren').addEventListener('click', () => {
  setzeKombination(variante().map(index => 1 - index));
});
setInterval(() => {
  const frames = posen[pose.value].frames;
  if (document.hidden || !frames || !animation.checked) return;
  frame = (frame + 1) % frames;
  zeichneVergleich();
}, 155);
