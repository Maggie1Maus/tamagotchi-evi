const figuren = {
  mohn: 'Mohn',
  flocke: 'Flocke',
};

const merkmale = {
  ohren: 'Ohren',
  augen: 'Augen',
  schnauze: 'Mundpartie',
  koerper: 'Körper',
  schwanz: 'Schwanz',
};

const altersstufen = {
  baby: { label: 'Baby', kopfY: 38, kopf: .83, koerper: .62, schwanz: .72 },
  erwachsen: { label: 'Erwachsen', kopfY: 30, kopf: 1, koerper: 1, schwanz: 1 },
};

const grau = {
  kontur: '#222',
  dunkel: '#555',
  schatten: '#888',
  mittel: '#aaa',
  hell: '#ccc',
  licht: '#eee',
  weiss: '#fff',
};

function form(ctx, pfad, farbe, rand = false) {
  const silhouette = new Path2D(pfad);
  ctx.fillStyle = farbe;
  ctx.fill(silhouette);
  if (rand) {
    ctx.strokeStyle = grau.kontur;
    ctx.lineWidth = 1.55;
    ctx.lineJoin = 'round';
    ctx.stroke(silhouette);
  }
}

function linie(ctx, pfad, farbe = grau.kontur, breite = 1.35) {
  ctx.strokeStyle = farbe;
  ctx.lineWidth = breite;
  ctx.lineCap = 'round';
  ctx.lineJoin = 'round';
  ctx.stroke(new Path2D(pfad));
}

function punkte(ctx, positionen, farbe = grau.schatten) {
  ctx.fillStyle = farbe;
  for (const [x, y, ecke = 1] of positionen) ctx.fillRect(x, y, ecke, ecke);
}

const schwanz = {
  mohn(ctx) {
    form(ctx, 'M-3 2 Q-4 -2 0 -5 L1 -7 L4 -7 L5 -9 L8 -8 Q12 -8 13 -4 Q16 -2 14 1 Q13 5 8 6 L1 5 Z', grau.licht, true);
    form(ctx, 'M-2 2 Q5 4 11 1 L14 -1 Q14 4 9 5 L2 5 Z', grau.hell);
    punkte(ctx, [[5,-4],[8,-6],[10,-3],[3,1]]);
  },
  flocke(ctx) {
    form(ctx, 'M-3 2 Q2 -1 3 -8 Q3 -16 9 -22 Q14 -26 19 -22 L21 -18 Q17 -19 16 -14 Q22 -9 19 -2 Q17 4 8 5 L-1 6 Z', grau.hell, true);
    form(ctx, 'M6 -12 Q8 -22 14 -23 Q19 -24 20 -19 Q16 -20 15 -14 L12 -15 L10 -12 L8 -14 Z', grau.licht);
    form(ctx, 'M0 3 Q11 3 16 -5 Q17 -10 15 -14 Q22 -7 18 -1 Q15 4 8 5 L0 5 Z', grau.schatten);
    punkte(ctx, [[8,-9],[10,-6],[12,-9],[12,-3],[15,-6],[7,-2]]);
  },
};

const koerper = {
  mohn: {
    ansatz: [10, -11],
    zeichnen(ctx) {
      form(ctx, 'M-9 -26 Q-15 -18 -12 -8 L-15 -4 Q-16 0 -11 0 L-6 0 L-4 -1 L5 -1 L7 0 L13 0 Q16 -1 14 -4 L12 -7 Q13 -15 8 -24 L4 -29 Z', grau.mittel, true);
      form(ctx, 'M-8 -19 L-5 -21 L-3 -19 L0 -21 L2 -19 L5 -20 L8 -17 Q10 -10 6 -6 Q0 -3 -6 -7 Q-10 -11 -8 -19 Z', grau.licht);
      form(ctx, 'M-12 -8 Q-11 -5 -8 -4 L-5 -4 L-6 -1 L-12 -1 Q-16 -2 -12 -5 Z M8 -7 L12 -7 L14 -3 L13 -1 L8 -1 L6 -3 Z', grau.schatten);
      linie(ctx, 'M-9 -16 Q-12 -11 -7 -8 L-4 -9 M8 -16 Q11 -12 7 -8 L4 -9');
      linie(ctx, 'M-12 -2 v1 M10 -2 v1', grau.dunkel, 1);
      punkte(ctx, [[-11,-18],[-12,-15],[10,-15],[11,-12],[7,-5],[-11,-6]], grau.dunkel);
    },
  },
  flocke: {
    ansatz: [13, -7],
    zeichnen(ctx) {
      form(ctx, 'M-10 -26 Q0 -28 8 -19 Q10 -14 14 -11 Q18 -8 17 -4 Q17 0 12 0 L-10 0 Q-15 0 -15 -4 L-13 -8 Q-16 -14 -13 -20 Z', grau.hell, true);
      form(ctx, 'M1 -17 Q11 -16 15 -9 Q18 -4 15 -2 L9 -1 Q12 -6 9 -8 Q5 -10 3 -8 L0 -10 Z', grau.schatten);
      form(ctx, 'M-12 -19 L-9 -21 L-7 -18 L-5 -19 L-3 -16 L0 -15 L-1 -12 Q-4 -9 -8 -9 L-9 -4 Q-9 -1 -12 -1 L-14 -3 L-12 -8 Q-15 -14 -12 -19 Z', grau.licht);
      form(ctx, 'M-2 -17 L1 -16 L2 -13 L1 -11 L-1 -11 L-3 -14 Z M8 -13 L10 -12 L11 -10 L9 -9 L7 -10 Z M12 -6 L14 -5 L13 -3 L11 -4 Z', grau.mittel);
      linie(ctx, 'M-5 -8 L-4 -4 L-5 -1 M9 -9 Q5 -10 4 -7 Q3 -5 5 -3 L4 -1 M-11 -3 v2 M-1 -3 v2', grau.dunkel, 1.15);
      punkte(ctx, [[6,-16],[8,-15],[11,-13],[13,-11],[15,-8],[12,-6]], grau.dunkel);
    },
  },
};

const ohren = {
  mohn(ctx) {
    form(ctx, 'M-13 -6 Q-18 -13 -18 -21 Q-18 -26 -15 -27 Q-11 -28 -9 -23 Q-6 -16 -6 -9 Z', grau.hell, true);
    form(ctx, 'M5 -10 Q6 -20 11 -25 Q14 -27 16 -25 Q20 -22 17 -15 L12 -6 Z', grau.hell, true);
    form(ctx, 'M-13 -9 Q-17 -18 -16 -23 Q-15 -25 -13 -23 Q-10 -20 -9 -10 Z M8 -9 Q9 -19 13 -23 Q15 -24 16 -21 Q16 -16 12 -10 Z', grau.schatten);
    linie(ctx, 'M-16 -19 L-14 -14 M16 -19 L13 -14', grau.mittel, 1);
  },
  flocke(ctx) {
    form(ctx, 'M-13 -9 Q-18 -12 -22 -6 Q-25 -2 -26 6 Q-26 12 -22 13 Q-17 14 -16 9 L-11 -5 Z', grau.mittel, true);
    form(ctx, 'M13 -9 Q18 -11 22 -6 Q25 -1 25 7 Q24 13 20 13 Q16 12 15 6 L11 -5 Z', grau.mittel, true);
    form(ctx, 'M-21 -6 Q-25 0 -25 6 Q-25 10 -22 12 Q-19 12 -18 9 L-16 3 Q-21 8 -22 5 Q-23 2 -21 -6 Z M16 1 L18 9 Q20 13 23 10 Q25 7 24 3 Q23 9 21 7 Z', grau.schatten);
    punkte(ctx, [[-22,0],[-23,3],[-21,5],[-20,8],[-19,-3],[20,-3],[21,0],[22,3],[19,5],[22,8]], grau.dunkel);
  },
};

// Genau diese Kontur und Schattierung werden für beide Figuren und alle Kinder verwendet.
function kopf(ctx) {
  form(ctx, 'M-17 0 Q-17 -7 -10 -10 L-6 -12 L-3 -11 L0 -14 L2 -11 L6 -12 L9 -10 Q17 -8 17 1 Q18 7 13 11 Q9 15 0 15 Q-9 15 -14 11 Q-18 7 -17 0 Z', grau.hell, true);
  form(ctx, 'M-15 -1 Q-12 -6 -8 -7 Q-4 -8 0 -5 Q5 -8 9 -6 Q14 -5 15 0 L16 5 Q14 10 10 11 Q0 14 -11 11 Q-16 9 -16 4 Z', grau.licht);
  form(ctx, 'M-16 3 Q-13 9 -8 10 Q1 14 10 10 Q14 9 16 5 Q15 10 11 12 Q2 16 -9 13 Q-15 11 -16 7 Z', grau.mittel);
  punkte(ctx, [[-13,-4],[-11,-7],[-10,-5],[10,-6],[13,-3],[-14,7],[13,7]], grau.mittel);
}

const augen = {
  mohn(ctx) {
    form(ctx, 'M-11 -1 Q-11 -5 -8 -5 Q-5 -5 -5 -1 Q-5 3 -8 3 Q-11 3 -11 -1 Z M5 -1 Q5 -5 8 -5 Q11 -5 11 -1 Q11 3 8 3 Q5 3 5 -1 Z', grau.kontur);
    punkte(ctx, [[-10,-4,2],[6,-4,2]], grau.weiss);
    punkte(ctx, [[-7,1],[9,1]], grau.mittel);
    punkte(ctx, [[-14,4],[-12,5],[11,5],[13,4]]);
  },
  flocke(ctx) {
    linie(ctx, 'M-12 1 Q-9 -4 -6 0 M6 0 Q9 -4 12 1', grau.kontur, 1.9);
    linie(ctx, 'M-12 1 l-1 -1 M12 1 l1 -1', grau.kontur, 1);
    punkte(ctx, [[-13,4],[-11,4],[10,4],[12,4]], grau.schatten);
  },
};

const schnauze = {
  mohn(ctx) {
    form(ctx, 'M-2 4 Q0 3 2 4 Q2 6 0 7 Q-2 6 -2 4 Z', grau.dunkel);
    linie(ctx, 'M0 7 v1 Q-2 10 -4 8 M0 8 Q2 10 4 8');
  },
  flocke(ctx) {
    form(ctx, 'M-5 6 Q-1 3 0 5 Q2 3 5 6 Q7 10 3 12 Q-2 13 -5 10 Q-7 8 -5 6 Z', grau.weiss);
    form(ctx, 'M-2 5 Q0 4 2 5 Q2 7 0 7.5 Q-2 7 -2 5 Z', grau.kontur);
    form(ctx, 'M-4 8 Q0 10 4 8 Q3 12 0 12 Q-3 12 -4 8 Z', grau.kontur);
    punkte(ctx, [[-1,10,2]], grau.mittel);
  },
};

function zeichneFigur(canvas, phase, auswahl) {
  const ctx = canvas.getContext('2d', { willReadFrequently: true });
  const alter = altersstufen[phase];
  const basis = koerper[auswahl.koerper];
  const x = 33;
  const boden = 62;
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  const [schwanzX, schwanzY] = basis.ansatz;
  ctx.save();
  ctx.translate(x + alter.koerper * schwanzX, boden + alter.koerper * schwanzY);
  ctx.scale(alter.schwanz, alter.schwanz);
  schwanz[auswahl.schwanz](ctx);
  ctx.restore();

  ctx.save();
  ctx.translate(x, boden);
  ctx.scale(alter.koerper, alter.koerper);
  basis.zeichnen(ctx);
  ctx.restore();

  ctx.save();
  ctx.translate(x, alter.kopfY);
  ctx.scale(alter.kopf, alter.kopf);
  ohren[auswahl.ohren](ctx);
  kopf(ctx);
  augen[auswahl.augen](ctx);
  schnauze[auswahl.schnauze](ctx);
  ctx.restore();

  // Auf 16 neutrale Graustufen und ganze sichtbare Pixel begrenzen.
  const bild = ctx.getImageData(0, 0, canvas.width, canvas.height);
  for (let i = 0; i < bild.data.length; i += 4) {
    const alpha = bild.data[i + 3] >= 128 ? 255 : 0;
    const ton = alpha ? Math.round(bild.data[i] / 17) * 17 : 0;
    bild.data.set([ton, ton, ton, alpha], i);
  }
  ctx.putImageData(bild, 0, 0);
}
