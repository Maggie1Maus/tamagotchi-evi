const figuren = { evi: 'Evi-Entwurf', lotti: 'Lotti', gloeckchen: 'Glöckchen', mia: 'Mia' };
const merkmale = { ohren: 'Ohren', augen: 'Augen', schnauze: 'Mundpartie', koerper: 'Körper', schwanz: 'Schwanz' };
const altersstufen = {
  baby: { label: 'Baby', y: 37, kopf: .85, rund: 1.04, breit: .72, hoch: .74, bein: .69, ohr: .80, schwanz: .70, haube: 'baby' },
  teenager: { label: 'Teenager', y: 29, kopf: .91, rund: .95, breit: .80, hoch: .97, bein: 1.14, ohr: 1.03, schwanz: .88, haube: 'teenager' },
  erwachsen: { label: 'Erwachsen', y: 30, kopf: 1, rund: 1, breit: 1, hoch: 1, bein: 1, ohr: 1, schwanz: 1, haube: 'erwachsen' },
  senior: { label: 'Senior', y: 33, kopf: 1.01, rund: 1.01, breit: 1.04, hoch: .91, bein: .87, ohr: .91, schwanz: .98, haube: 'senior' },
};

const grau = {
  rand:'#222', dunkel:'#444', tief:'#555', schatten:'#777', mittel:'#999',
  fell:'#bbb', hell:'#ccc', licht:'#ddd', creme:'#eee', weiss:'#fff',
};

function form(ctx, pfad, farbe, rand = false) {
  const linie = new Path2D(pfad);
  ctx.fillStyle = farbe;
  ctx.fill(linie);
  if (rand) strich(ctx, linie, grau.rand, 1.18);
}
function strich(ctx, pfad, farbe = grau.rand, breite = 1.06) {
  ctx.strokeStyle = farbe;
  ctx.lineWidth = breite;
  ctx.lineCap = 'round';
  ctx.lineJoin = 'round';
  ctx.stroke(typeof pfad === 'string' ? new Path2D(pfad) : pfad);
}
function punkte(ctx, positionen, farbe = grau.mittel) {
  ctx.fillStyle = farbe;
  for (const [x,y,s = .7] of positionen) ctx.fillRect(x,y,s,s);
}
function spiegel(ctx, zeichnen) {
  for (const seite of [-1,1]) {
    ctx.save();
    ctx.scale(seite,1);
    zeichnen(ctx);
    ctx.restore();
  }
}

const ohren = {
  evi(ctx) {
    spiegel(ctx, c => {
      form(c, 'M6 -10 Q11 -18 18 -23 L24 -25 L23 -19 Q22 -10 17 -4 L13 -2 Z', grau.hell, true);
      form(c, 'M11 -10 Q15 -18 22 -23 Q21 -10 16 -6 L13 -4 Z', grau.dunkel);
      form(c, 'M12 -9 L16 -15 L19 -17 L20 -17 Q19 -10 16 -6 Z', grau.tief);
      strich(c, 'M8 -10 Q13 -18 21 -23', grau.creme, 1);
      punkte(c, [[18,-19],[17,-16],[16,-13],[22,-20],[20,-14]], grau.mittel);
    });
  },
  lotti(ctx) {
    spiegel(ctx, c => {
      form(c, 'M8 -10 Q15 -23 26 -19 Q27 -16 23 -11 Q19 -5 13 -5 L10 -6 Z', grau.fell, true);
      form(c, 'M12 -9 Q16 -18 24 -18 Q22 -11 15 -7 Z', grau.schatten);
      form(c, 'M15 -8 Q17 -13 22 -15 Q20 -10 15 -8 Z', grau.licht);
      strich(c, 'M14 -16 Q19 -21 24 -19', grau.creme, .8);
      punkte(c, [[19,-17],[21,-17],[16,-13],[18,-10]], grau.mittel);
    });
  },
  gloeckchen(ctx) {
    spiegel(ctx, c => {
      form(c, 'M11 -11 Q18 -13 21 -7 Q23 -3 25 3 L26 9 Q26 15 22 16 Q18 16 16 13 L13 6 L11 -4 Z', grau.fell, true);
      form(c, 'M17 -10 Q19 -8 21 -3 L24 5 Q25 12 22 14 Q19 14 17 10 Q21 11 21 7 L18 -3 Z', grau.schatten);
      strich(c, 'M13 -9 Q18 -8 19 -1 M20 15 l-1 -3', grau.licht, .9);
      punkte(c, [[19,-2,1],[21,0],[17,3],[22,4,1.2],[20,7],[23,9],[19,10]], grau.dunkel);
    });
  },
  mia(ctx) {
    spiegel(ctx, c => {
      form(c, 'M8 -10 L13 -21 Q14 -23 16 -20 Q21 -17 21 -7 L18 -4 L13 -7 Z', grau.hell, true);
      form(c, 'M12 -11 L15 -19 Q19 -17 19 -9 L16 -8 Z', grau.schatten);
      form(c, 'M14 -12 L15 -17 L18 -12 L17 -10 Z', grau.creme);
      strich(c, 'M15 -20 l-4 8', grau.creme, .8);
    });
  },
};

const kopfKontur = 'M-17 0 Q-17 -7 -10 -10 Q-7 -13 -3 -12 L0 -14 L3 -12 Q8 -13 11 -10 Q17 -8 17 0 Q18 6 14 10 Q10 15 0 15 Q-10 15 -14 11 Q-18 8 -17 0 Z';

function kopf(ctx, alter, ruecken) {
  form(ctx, kopfKontur, grau.hell, true);
  if (ruecken) {
    form(ctx,'M-16 2 Q-14 -10 -5 -10 Q2 -14 9 -10 Q14 -8 16 -2 L16 6 Q11 12 0 13 Q-9 13 -14 10 Z',grau.fell);
    strich(ctx,'M-8 -8 q7 -4 13 -2 M-5 -5 q4 -3 8 -2 M5 -5 l3 -1 M-1 7 q5 1 9 -1',grau.licht,1.1);
    punkte(ctx,[[-11,2],[-8,7],[4,6],[10,3],[10,-4],[5,1],[-5,-1]],grau.schatten);
    return;
  }
  form(ctx, 'M-16 -1 Q-14 -9 -6 -10 Q-1 -12 7 -11 Q15 -9 16 -2 L15 6 Q10 12 0 13 Q-12 13 -16 6 Z', grau.licht);
  form(ctx, 'M-15 0 Q-11 -5 -7 -4 Q-2 -4 0 -1 Q4 -5 9 -4 Q13 -4 16 0 Q16 7 12 9 Q6 13 0 13 Q-10 13 -14 9 Q-17 5 -15 0 Z', grau.creme);
  form(ctx, 'M-16 5 Q-11 11 -5 12 Q2 14 9 11 Q14 10 16 6 Q14 12 8 13 Q-1 16 -11 12 Q-15 10 -16 7 Z', grau.fell);
  strich(ctx, 'M-13 -5 Q-11 -8 -8 -8 M10 -8 l3 2', grau.creme, .85);
  punkte(ctx, [[-12,-6],[-10,-7],[-8,-9],[9,-8],[13,-5],[-15,5],[-13,7],[12,7],[14,5],[13,9]], grau.mittel);
  if (alter.haube === 'baby') {
    form(ctx, 'M-5 -10 Q-2 -13 0 -13 Q4 -13 6 -9 Q1 -10 -1 -8 Q-3 -9 -5 -10 Z', grau.creme);
  } else if (alter.haube === 'teenager') {
    strich(ctx, 'M-6 -9 l2 -2 l1 1 M-2 -9 l3 -3 l2 2 M4 -9 l2 -1', grau.schatten, .9);
  } else if (alter.haube === 'erwachsen') {
    strich(ctx, 'M-4 -9 l1 -2 M0 -8 v-3 M3 -9 l1 -1', grau.mittel, .85);
  } else {
    form(ctx, 'M-15 5 Q-11 8 -9 8 Q-6 8 -4 11 Q-9 13 -13 10 Z M15 5 Q11 8 9 8 Q6 8 4 11 Q9 13 13 10 Z', grau.weiss);
    strich(ctx, 'M-6 -9 q2 -3 4 -2 M-2 -8 q2 -4 5 -3 M2 -8 q2 -2 4 -2 M-14 4 l2 2 M14 4 l-2 2', grau.schatten, .85);
  }
}

const schwanz = {
  evi(ctx) {
    form(ctx,'M-4 3 Q2 3 4 -3 Q3 -13 7 -19 Q9 -24 17 -25 Q20 -25 22 -27 Q22 -21 21 -17 Q25 -13 23 -8 Q24 -2 19 2 Q16 6 10 7 L1 7 Z', grau.fell, true);
    form(ctx,'M4 -9 Q4 -21 13 -24 L22 -26 L21 -18 L22 -13 L19 -14 L16 -12 L14 -14 L10 -12 L8 -15 Z', grau.creme);
    strich(ctx,'M5 -7 Q6 -18 11 -21 M6 5 Q17 4 21 -5',grau.licht,1.1);
    strich(ctx,'M8 -15 l2 3 l4 -2 l2 2 l3 -2 l3 1',grau.mittel,.9);
    punkte(ctx, [[8,-8],[12,-10],[15,-8],[19,-9],[11,-4],[16,-5],[20,-4],[9,0],[14,1],[17,-1],[12,4]], grau.schatten);
  },
  lotti(ctx) {
    form(ctx,'M-3 1 Q1 -2 0 -6 Q2 -11 9 -12 L11 -15 Q15 -12 15 -8 Q15 -1 8 2 L1 4 Z', grau.schatten, true);
    form(ctx,'M1 -4 Q0 -9 8 -11 L11 -14 Q14 -11 14 -8 L10 -6 L7 -6 L6 -4 Z', grau.creme);
    strich(ctx,'M3 -3 Q8 -3 12 -6',grau.fell,1);
    punkte(ctx, [[4,-8],[8,-9],[8,-1],[4,0]], grau.mittel);
  },
  gloeckchen(ctx) {
    form(ctx,'M-4 1 Q2 1 4 -7 Q6 -11 9 -8 Q11 -6 10 -3 Q15 -5 15 -10 Q17 -12 19 -9 Q21 -1 14 3 Q10 6 1 5 Z', grau.hell, true);
    form(ctx,'M0 2 Q9 3 14 0 Q18 -3 19 -7 Q20 -2 15 2 Q10 5 2 5 Z', grau.schatten);
    strich(ctx,'M4 -3 q2 -5 4 -4 M12 -2 q4 -2 5 -6',grau.creme,1);
    punkte(ctx, [[8,-2],[6,0],[11,1],[15,-1]], grau.mittel);
  },
  mia(ctx) {
    form(ctx,'M-4 2 Q4 4 7 -1 Q8 -5 6 -11 Q3 -17 7 -22 Q11 -27 18 -24 Q23 -22 22 -18 L20 -16 Q18 -20 15 -18 Q12 -16 14 -13 L18 -7 Q20 1 14 5 Q8 9 -2 7 Z', grau.hell, true);
    form(ctx,'M4 5 Q10 6 14 2 Q17 -1 16 -5 L12 -12 Q10 -16 12 -18 Q15 -21 18 -19 Q15 -19 14 -16 Q14 -14 17 -9 Q20 -3 17 2 Q13 8 3 7 Z', grau.schatten);
    form(ctx,'M6 -17 Q6 -23 12 -24 Q18 -25 21 -21 L21 -18 Q17 -23 12 -20 L10 -16 Z', grau.creme);
    strich(ctx,'M8 -11 l2 5 M10 2 q3 -2 3 -5',grau.licht,1);
    punkte(ctx, [[8,-14],[10,-10],[14,-9],[12,-4],[15,-4],[11,1],[14,1],[6,4]], grau.mittel);
  },
};

const koerper = {
  evi: {
    achse: 0, ansatz: [10,-11], form: 'M-8 -28 Q0 -31 8 -28 L9 -19 Q15 -15 14 -8 Q10 -4 4 -5 L-6 -5 Q-14 -4 -14 -10 Q-13 -18 -9 -22 Z',
    pfoten: [-11,-6,6,11], ton: grau.fell,
    details(ctx) {
      form(ctx,'M-10 -23 Q0 -25 10 -23 L13 -18 L11 -16 L12 -13 L8 -13 L7 -10 L3 -12 L0 -9 L-4 -12 L-8 -10 L-8 -13 L-12 -13 L-11 -16 L-13 -18 Z',grau.creme,true);
      strich(ctx,'M-10 -19 l2 3 M-6 -16 l2 2 M6 -17 l-2 3 M10 -19 l-1 2',grau.fell,.85);
      punkte(ctx, [[-10,-10],[-8,-8],[8,-9],[10,-11],[11,-7],[-6,-7],[4,-7]],grau.schatten);
    },
  },
  lotti: {
    achse: -6, ansatz: [16,-12], form: 'M-11 -29 Q-6 -29 -5 -23 Q-4 -18 1 -18 Q10 -21 16 -15 Q20 -11 15 -7 Q8 -6 1 -8 L-6 -8 Q-12 -9 -12 -16 L-14 -24 Z',
    pfoten: [-10,-6,9,15], ton: grau.mittel,
    details(ctx) {
      form(ctx,'M-12 -24 Q-11 -17 -6 -14 L-4 -9 Q-11 -9 -11 -15 L-13 -20 Z',grau.creme);
      form(ctx,'M-5 -10 Q-2 -13 2 -11 Q7 -8 15 -11 L15 -8 Q10 -6 4 -8 L-6 -8 Z',grau.schatten);
      punkte(ctx, [[0,-16,1.4],[4,-17,1.3],[8,-17,1.5],[12,-15,1.6],[15,-13,1.3],[2,-13,1.2],[6,-14,1.2],[10,-13,1.4],[13,-11]], grau.creme);
      strich(ctx,'M-9 -22 q0 5 3 7 M5 -18 q5 -1 8 2',grau.hell,.9);
    },
  },
  gloeckchen: {
    achse: 0, ansatz: [12,-8], form: 'M-8 -26 Q0 -30 8 -26 L10 -18 L10 -14 Q15 -14 16 -9 Q16 -5 9 -5 L-9 -5 Q-16 -5 -16 -9 Q-15 -14 -10 -14 L-10 -18 Z',
    pfoten: [-12,-6,6,12], ton: grau.hell,
    details(ctx) {
      form(ctx,'M-8 -23 Q0 -25 8 -23 L7 -16 Q6 -10 0 -7 Q-6 -10 -7 -16 Z',grau.creme);
      form(ctx,'M-14 -10 Q-12 -13 -10 -12 L-8 -9 L-9 -6 L-12 -6 Z M14 -10 Q12 -13 10 -12 L8 -9 L9 -6 L12 -6 Z',grau.schatten);
      strich(ctx,'M-8 -20 Q0 -17 8 -20',grau.tief,1.75);
      form(ctx,'M0 -18 Q2 -19 2.5 -16.5 L3 -15 L-3 -15 L-2.5 -16.5 Q-2 -19 0 -18 Z',grau.licht,true);
      strich(ctx,'M-.6 -14 h1.2',grau.dunkel,.85);
      punkte(ctx,[[-12,-9],[-10,-8],[11,-9],[12,-7],[-8,-16],[7,-15]],grau.mittel);
    },
  },
  mia: {
    achse: -2, ansatz: [12,-9], form: 'M-9 -27 Q-2 -31 7 -27 L9 -21 Q9 -15 14 -12 Q18 -7 13 -5 L3 -4 L-9 -5 Q-15 -6 -14 -11 L-11 -18 Z',
    pfoten: [-10,-5,6,12], ton: grau.fell,
    details(ctx) {
      form(ctx,'M-9 -23 L-6 -22 L-4 -18 L-1 -20 L1 -18 L4 -20 L7 -19 Q6 -12 1 -9 L-2 -7 L-7 -10 Q-10 -14 -9 -23 Z',grau.creme);
      form(ctx,'M1 -21 L6 -23 L8 -22 L8 -17 L4 -18 L1 -19 L-2 -17 L-5 -17 L-5 -22 L-3 -23 Z',grau.hell,true);
      form(ctx,'M-1 -21 L2 -21 L2 -18 L-1 -18 Z',grau.schatten,true);
      strich(ctx,'M8 -13 Q15 -11 13 -7 M-11 -10 l3 3 M4 -7 l2 -1',grau.licht,1);
      punkte(ctx, [[7,-12],[9,-10],[11,-10],[12,-7],[8,-7],[-11,-12],[-10,-9]],grau.schatten);
    },
  },
};

function malePfote(ctx, figur, x, tiefe, boden, laenge, schu, oben, laeuft) {
  ctx.save();
  ctx.translate(x + schu,boden);
  const y = -laenge + oben;
  if (figur === 'lotti') {
    form(ctx,`M${-2-schu} ${y} L${1-schu} ${y-1} Q1 -6 1 -3 L2 0 L-2 0 L-2 -3 Z`,tiefe ? grau.schatten : grau.fell,true);
    form(ctx,'M-2 -3 L1.5 -3 L2 0 L-2 0 Z',grau.dunkel);
    strich(ctx,'M-1 0 v-1',grau.mittel,.65);
  } else {
    const breit = laeuft ? 3.3 : 4.1;
    form(ctx,`M${-3-schu} ${y} L${2.5-schu} ${y} Q3 -4 ${breit} -2 Q${breit+1} .6 1 .3 L-2 .3 Q${-breit-1} 0 ${-breit} -2 Z`,tiefe ? grau.mittel : grau.hell,true);
    form(ctx,`M${-breit+1} -2 Q-1 -3 2 -2 L3 -1 L-3 -1 Z`,tiefe ? grau.fell : grau.creme);
    strich(ctx,'M-1 .1 v-1.4 M1 .1 v-1.4',grau.schatten,.7);
  }
  ctx.restore();
}

function maleSchlafkoerper(ctx, figur) {
  const basis = koerper[figur];
  const formen = {
    evi:'M-15 -7 Q-16 -15 -10 -20 Q-3 -28 6 -24 Q15 -22 17 -14 Q20 -8 14 -3 Q6 0 -9 -1 Q-14 -1 -15 -7 Z',
    lotti:'M-18 -8 Q-19 -13 -14 -17 Q-10 -22 -5 -17 Q-1 -16 3 -20 Q14 -26 19 -16 Q23 -8 17 -4 Q12 0 -3 -1 L-12 -1 Q-18 -2 -18 -8 Z',
    gloeckchen:'M-17 -7 Q-18 -12 -14 -17 Q-5 -28 8 -24 Q18 -23 18 -14 Q21 -6 14 -3 Q3 0 -10 -1 Q-16 -1 -17 -7 Z',
    mia:'M-15 -7 Q-16 -13 -10 -19 Q-5 -28 7 -27 Q17 -25 18 -17 Q22 -9 16 -4 Q8 0 -8 -1 Q-14 -1 -15 -7 Z',
  };
  form(ctx,formen[figur],basis.ton,true);
  basis.details(ctx);
  form(ctx,'M5 -5 Q7 -10 12 -9 Q16 -8 14 -4 Q12 -1 5 -2 Q2 -3 5 -5 Z',grau.hell,true);
  strich(ctx,'M8 -3 l-1 -2 M10 -3 l-1 -2 M-2 -10 Q0 -13 4 -14',grau.schatten,.9);
}
