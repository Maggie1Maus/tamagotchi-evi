const figuren = {
  mohn: 'Mohn',
  flocke: 'Flocke',
  komet: 'Komet',
  pippa: 'Pippa',
};

const merkmale = {
  ohren: 'Ohren / Hörner',
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
  komet(ctx) {
    form(ctx, 'M-3 1 Q6 2 10 -5 Q12 -8 13 -13 L11 -14 L18 -21 L20 -12 L17 -12 Q15 1 8 5 L-3 5 Z', grau.mittel, true);
    form(ctx, 'M13 -14 L18 -19 L19 -13 L16 -13 L13 -6 Q9 2 2 4 L0 2 Q9 0 13 -14 Z', grau.licht);
    linie(ctx, 'M8 0 l2 1 M11 -4 l2 1', grau.dunkel, 1);
  },
  pippa(ctx) {
    form(ctx, 'M-3 2 Q6 4 9 -4 Q12 -11 9 -15 Q7 -17 7 -19 Q8 -22 12 -21 Q20 -17 18 -9 Q18 4 7 6 L-3 6 Z', grau.dunkel, true);
    form(ctx, 'M8 -19 Q9 -21 12 -20 L15 -18 L12 -15 L10 -15 Z M12 -13 L17 -15 L18 -11 L13 -8 L11 -8 Z M10 -5 L17 -8 L16 -3 L9 0 Z', grau.hell);
    linie(ctx, 'M13 -2 Q10 3 6 4', grau.schatten, 1.1);
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
    ansatz: [12, -7],
    zeichnen(ctx) {
      form(ctx, 'M-8 -26 Q0 -30 8 -26 Q11 -21 10 -14 L14 -8 Q16 -2 13 0 L6 0 L4 -1 L-4 -1 L-6 0 L-13 0 Q-16 -2 -14 -8 L-10 -14 Q-11 -21 -8 -26 Z', grau.hell, true);
      form(ctx, 'M-14 -6 Q-12 -11 -9 -11 L-6 -5 L-6 -1 L-12 -1 Q-15 -1 -14 -6 Z M14 -6 Q12 -11 9 -11 L6 -5 L6 -1 L12 -1 Q15 -1 14 -6 Z', grau.schatten);
      form(ctx, 'M-7 -22 Q0 -24 7 -22 L8 -17 Q6 -12 4 -9 L-4 -9 Q-6 -12 -8 -17 Z', grau.licht);
      form(ctx, 'M-8 -16 Q-5 -17 -5 -13 L-3 -6 L-3 -2 Q-5 0 -9 -1 L-10 -3 Z M8 -16 Q5 -17 5 -13 L3 -6 L3 -2 Q5 0 9 -1 L10 -3 Z', grau.licht);
      linie(ctx, 'M-7 -14 Q-6 -8 -7 -5 Q-10 -3 -9 -1 M7 -14 Q6 -8 7 -5 Q10 -3 9 -1 M-3 -7 L-3 -2 M3 -7 L3 -2', grau.dunkel, 1.05);
      linie(ctx, 'M-8 -19 Q0 -16 8 -19', grau.dunkel, 1.75);
      form(ctx, 'M-2 -17 Q0 -19 2 -17 L3 -13 L-3 -13 Z', grau.weiss, true);
      punkte(ctx, [[-11,-9],[-12,-7],[10,-9],[11,-6],[0,-14]], grau.dunkel);
    },
  },
  komet: {
    ansatz: [11, -8],
    zeichnen(ctx) {
      form(ctx, 'M-8 -22 Q-14 -28 -21 -23 Q-17 -17 -19 -11 Q-14 -14 -13 -9 L-7 -13 Z M8 -22 Q14 -28 21 -23 Q17 -17 19 -11 Q14 -14 13 -9 L7 -13 Z', grau.schatten, true);
      form(ctx, 'M-19 -23 Q-14 -25 -9 -21 L-12 -12 Q-14 -16 -17 -14 Q-16 -19 -19 -23 Z M19 -23 Q14 -25 9 -21 L12 -12 Q14 -16 17 -14 Q16 -19 19 -23 Z', grau.hell);
      linie(ctx, 'M-19 -23 Q-13 -21 -13 -16 M19 -23 Q13 -21 13 -16', grau.dunkel, 1);
      form(ctx, 'M-8 -28 Q0 -30 8 -28 L9 -18 Q15 -9 14 -4 Q14 0 9 0 L5 0 L3 -2 L-3 -2 L-5 0 L-9 0 Q-14 0 -14 -4 Q-15 -9 -9 -18 Z', grau.schatten, true);
      form(ctx, 'M-5 -22 Q0 -24 5 -22 Q8 -14 7 -8 Q6 -4 0 -4 Q-6 -4 -7 -8 Q-8 -14 -5 -22 Z', grau.licht);
      linie(ctx, 'M-6 -15 Q0 -13 6 -15 M-6 -11 Q0 -9 6 -11 M-4 -7 Q0 -6 4 -7', grau.mittel, 1.2);
      linie(ctx, 'M-11 -8 l4 3 M11 -8 l-4 3 M-10 -3 v2 M10 -3 v2', grau.kontur, 1.1);
      punkte(ctx, [[-10,-13],[-12,-10],[9,-13],[11,-10]]);
    },
  },
  pippa: {
    ansatz: [11, -7],
    zeichnen(ctx) {
      form(ctx, 'M-8 -27 Q0 -29 8 -27 L10 -17 Q13 -11 13 -5 L15 -2 Q15 0 11 0 L4 0 L2 -1 L-2 -1 L-4 0 L-11 0 Q-15 0 -15 -2 L-13 -5 Q-13 -11 -10 -17 Z', grau.mittel, true);
      form(ctx, 'M-8 -21 L-6 -18 L-4 -19 L-2 -15 L0 -17 L2 -15 L4 -19 L6 -18 L8 -21 Q10 -15 5 -11 L0 -9 L-5 -11 Q-10 -15 -8 -21 Z', grau.weiss);
      form(ctx, 'M-12 -8 L-7 -6 L-6 -3 L-4 -2 L-5 -1 L-12 -1 Q-15 -1 -13 -4 Z M12 -8 L7 -6 L6 -3 L4 -2 L5 -1 L12 -1 Q15 -1 13 -4 Z', grau.dunkel);
      linie(ctx, 'M-5 -10 Q-1 -7 -3 -2 M5 -10 Q1 -7 3 -2', grau.dunkel, 1.25);
      punkte(ctx, [[-10,-14],[-11,-11],[10,-14],[11,-11],[-2,-6],[1,-6]], grau.schatten);
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
  komet(ctx) {
    form(ctx, 'M-13 -6 Q-19 -18 -15 -25 Q-14 -27 -13 -24 Q-12 -17 -5 -11 L-5 -7 Z M13 -6 Q19 -18 15 -25 Q14 -27 13 -24 Q12 -17 5 -11 L5 -7 Z', grau.licht, true);
    form(ctx, 'M-15 -10 L-23 -11 L-20 -6 L-24 -4 L-18 -2 L-20 2 L-13 0 Z M15 -10 L23 -11 L20 -6 L24 -4 L18 -2 L20 2 L13 0 Z', grau.schatten, true);
    linie(ctx, 'M-15 -10 l-1 -4 M15 -10 l1 -4 M-20 -5 l4 1 M20 -5 l-4 1', grau.mittel, 1.15);
  },
  pippa(ctx) {
    form(ctx, 'M-15 -4 Q-21 -6 -21 -12 Q-21 -18 -16 -19 Q-12 -19 -7 -11 L-9 -6 Z M15 -4 Q21 -6 21 -12 Q21 -18 16 -19 Q12 -19 7 -11 L9 -6 Z', grau.dunkel, true);
    form(ctx, 'M-16 -8 Q-20 -11 -18 -15 Q-16 -18 -11 -11 Z M16 -8 Q20 -11 18 -15 Q16 -18 11 -11 Z', grau.licht);
    linie(ctx, 'M-17 -13 l2 2 M17 -13 l-2 2', grau.mittel, 1);
  },
};

// Alle Figuren und Kinder verwenden dieselbe Kontur und Schattierung.
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
  komet(ctx) {
    form(ctx, 'M-12 -3 Q-9 -6 -4 -3 Q-5 3 -9 3 Q-12 3 -12 -3 Z M12 -3 Q9 -6 4 -3 Q5 3 9 3 Q12 3 12 -3 Z', grau.kontur);
    form(ctx, 'M-10 -3 Q-9 -4 -8 -2 L-8 1 Q-10 1 -10 -3 Z M10 -3 Q9 -4 8 -2 L8 1 Q10 1 10 -3 Z', grau.weiss);
    linie(ctx, 'M-12 -6 l6 1 M12 -6 l-6 1', grau.dunkel, 1);
  },
  pippa(ctx) {
    form(ctx, 'M-15 0 Q-11 -7 -5 -3 Q-6 2 -10 4 Q-13 4 -15 0 Z M15 0 Q11 -7 5 -3 Q6 2 10 4 Q13 4 15 0 Z', grau.mittel);
    form(ctx, 'M-12 -1 Q-9 -4 -6 -2 Q-7 2 -9 2 Q-11 2 -12 -1 Z M12 -1 Q9 -4 6 -2 Q7 2 9 2 Q11 2 12 -1 Z', grau.kontur);
    punkte(ctx, [[-10,-2],[9,-2],[-7,-1],[6,-1]], grau.weiss);
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
  komet(ctx) {
    form(ctx, 'M-7 7 Q-7 4 -4 4 L4 4 Q7 4 7 7 Q6 11 0 12 Q-6 11 -7 7 Z', grau.mittel, true);
    linie(ctx, 'M-4 6 h1 M3 6 h1 M-4 8 Q0 11 4 8', grau.kontur, 1.2);
    form(ctx, 'M4 9 l-2 3 l-2 -2 Z', grau.weiss);
  },
  pippa(ctx) {
    form(ctx, 'M-6 7 Q-5 4 0 6 Q5 4 6 7 Q6 10 0 12 Q-6 10 -6 7 Z', grau.weiss);
    form(ctx, 'M-2 6 L2 6 L0 8 Z', grau.kontur);
    linie(ctx, 'M-3 9 Q-1 11 0 9 Q1 11 3 9 M-7 7 l-4 -1 M7 7 l4 -1 M-8 9 h-3 M8 9 h3', grau.dunkel, 1);
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
  ctx.scale(alter.kopf * .86, alter.kopf);
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
