const emotionen = {
  ruhe:'Neutral', gluecklich:'Glücklich', hungrig:'Hungrig', traurig:'Traurig',
  muede:'Müde', krank:'Krank', schmutzig:'Schmutzig', beleidigt:'Beleidigt',
};
const posen = {
  sitzen:{ label:'Ruhepose' },
  laufen:{ label:'Laufen', frames:8 },
  schnuppern:{ label:'Schnüffeln', frames:6, kopf:[-6,9,-.17] },
  schlafen:{ label:'Schlafen', frames:16, kopf:[-8,19,-.14], hoehe:.59, gesicht:'schlaf' },
  untersuchen:{ label:'Untersuchen', frames:8, kopf:[-5,7,-.14] },
  zeigen:{ label:'Fund zeigen', kopf:[0,-2,0] },
  fenster:{ label:'Aus dem Fenster schauen', kopf:[3,-2,.11] },
  denken:{ label:'Denken', kopf:[1,-1,.12] },
  singen:{ label:'Singen', frames:8, kopf:[0,-2,0], gesicht:'lied' },
  gaehnen:{ label:'Gähnen', frames:16, kopf:[0,-3,0], gesicht:'gaehnen' },
  drehen:{ label:'Drehen', frames:12 },
  huepfen:{ label:'Hüpfen', frames:12 },
  staunen:{ label:'Staunen', kopf:[0,-3,0] },
};
const bilderBreite = 112;
const bilderHoehe = 96;

const augenArt = {
  evi: { abstand:8.1, breit:2.9, hoch:3.3, dunkel:grau.rand },
  lotti:{ abstand:8.2, breit:3.7, hoch:3.4, dunkel:grau.dunkel },
  gloeckchen:{ abstand:7.7, breit:3.2, hoch:2.7, dunkel:grau.rand },
  mia:{ abstand:8.4, breit:3.6, hoch:2.6, dunkel:grau.rand },
};

function augenZeichnen(ctx, figur, emotion, ausdruck, alter, blick) {
  const a = augenArt[figur];
  spiegel(ctx, c => {
    c.save();
    c.translate(a.abstand,0);
    const b = a.breit;
    if (figur === 'mia') strich(c,'M2 -2.6 l2 -1.4 M2.8 -1 l1.7 -.5',grau.dunkel,.75);
    if (figur === 'lotti') {
      form(c,'M-4 -3 Q0 -7 4 -3 L3 2 Q0 4 -3 2 Z',grau.licht);
      punkte(c,[[-3,-5],[1,-5],[2,-4]],grau.weiss);
    }
    if (figur === 'gloeckchen') {
      form(c,'M-3 -4 Q1 -6 4 -3 L3 1 Q1 3 -2 2 Z',grau.fell);
    }

    if (ausdruck === 'schlaf' || ausdruck === 'gaehnen') {
      strich(c,`M${-b} -.1 Q0 2 ${b} 0`,a.dunkel,1.3);
    } else if (emotion === 'gluecklich' || ausdruck === 'lied') {
      strich(c,`M${-b} 0 Q0 ${-a.hoch*1.3} ${b} .1`,a.dunkel,1.45);
    } else {
      const hoch = ['muede','krank','beleidigt'].includes(emotion) ? a.hoch*.56 : a.hoch;
      form(c,`M${-b} 0 Q${-b} ${-hoch} -.3 ${-hoch-1} Q${b} ${-hoch} ${b} -1 Q${b} ${hoch-1} 0 ${hoch} Q${-b+1} ${hoch} ${-b} 0 Z`,a.dunkel);
      const rechts = c.getTransform().a >= 0;
      const sx = (rechts ? blick : -blick) * .8;
      punkte(c,[[-1.1 + sx,-hoch*.67,1.15],[1.1+sx,.9,.68]],grau.weiss);
      if (emotion === 'traurig') strich(c,'M-2.8 -4.8 Q-.3 -4 2.6 -3.7',grau.schatten,.95);
      if (emotion === 'beleidigt') strich(c,'M-3.4 -3.4 l5.5 -1.9',grau.rand,1.3);
      if (emotion === 'krank') strich(c,'M-2.8 -4 Q0 -6 2.8 -4',grau.schatten,.8);
      if (emotion === 'muede') strich(c,'M-3 -1.55 h5.3',grau.fell,.9);
    }

    if (alter.haube === 'senior') strich(c,'M3 2.7 l1.1 .6 M2.7 4 l1.2 .7',grau.schatten,.61);
    punkte(c,[[3,4.8],[4.6,5.8],[5.9,5.2]],grau.mittel);
    c.restore();
  });
}

const maulArt = {
  evi: { breit:4, nase:'M-2.2 4 Q0 3.4 2.2 4 L.9 5.8 L-.9 5.8 Z', weiss:grau.creme },
  lotti:{ breit:3.5, nase:'M-2.3 4 Q0 3 2.3 4 Q1.3 6 0 6 Q-1.3 6 -2.3 4 Z', weiss:grau.weiss },
  gloeckchen:{ breit:4.7, nase:'M-2.5 4 Q0 2.8 2.5 4 Q2 6 0 6 Q-2 6 -2.5 4 Z', weiss:grau.weiss },
  mia:{ breit:3.4, nase:'M-2 4 h4 L0 6.15 Z', weiss:grau.weiss },
};
function mundZeichnen(ctx, figur, emotion, ausdruck, pose, wellen) {
  const art = maulArt[figur];
  const b = art.breit;
  form(ctx,`M${-b-1} 7 Q${-b} 3.6 0 5 Q${b} 3.6 ${b+1} 7 Q${b} 11.5 0 12 Q${-b} 11.5 ${-b-1} 7 Z`,art.weiss);
  form(ctx,art.nase,grau.dunkel);
  punkte(ctx,[[-.9,4,.55]],grau.fell);
  if (figur === 'mia') {
    strich(ctx,'M-7 6.2 l-4 -1 M7 6.2 l4 -1 M-7.5 8 h-3 M7.5 8 h3',grau.schatten,.7);
  } else if (figur === 'evi' || figur === 'gloeckchen') {
    punkte(ctx,[[-b,6.5],[-b+.6,7.6],[b-.7,6.5],[b-1.2,7.7]],grau.schatten);
  }

  if (ausdruck === 'schlaf') {
    strich(ctx,`M${-b*.62} 8 Q0 9 ${b*.62} 8`,grau.dunkel,.9);
  } else if (ausdruck === 'gaehnen' || ausdruck === 'lied' || emotion === 'hungrig' || pose === 'staunen') {
    const tief = ausdruck === 'gaehnen' ? 11 + Math.max(0,wellen) : 10.7;
    form(ctx,`M${-b*.73} 8.8 Q0 5.9 ${b*.73} 8.8 Q${b*.5} ${tief} 0 ${tief+.7} Q${-b*.5} ${tief} ${-b*.73} 8.8 Z`,grau.rand);
    strich(ctx,`M-1 ${tief-.1} h2`,grau.fell,.8);
  } else if (emotion === 'gluecklich') {
    form(ctx,`M${-b} 7.2 Q0 8.8 ${b} 7.2 Q${b-1} 12 0 12 Q${-b+1} 12 ${-b} 7.2 Z`,grau.rand);
    form(ctx,'M-2 10 Q0 9.1 2 10 L1 11 L-1 11 Z',grau.fell);
  } else if (emotion === 'traurig' || emotion === 'beleidigt') {
    strich(ctx,`M${-b*.74} 9.7 Q0 6.8 ${b*.74} 9.7`,grau.dunkel,1.06);
  } else if (emotion === 'krank') {
    strich(ctx,'M-3 9 q1 -1 2 0 q1 1 2 0 q1 -1 2 0',grau.dunkel);
  } else {
    strich(ctx,`M0 6.4 v1.4 Q${-b*.66} 9.6 ${-b*.8} 8 M0 7.8 Q${b*.66} 9.6 ${b*.8} 8`,grau.dunkel,.95);
  }
}

function effekt(ctx, pose, emotion, dx, y, boden, welle) {
  const x = dx;
  if (pose === 'schnuppern') {
    strich(ctx,`M${x-20} ${y+10} l-3 -2 M${x-21} ${y+13} h-4`,grau.schatten,.88);
  } else if (pose === 'schlafen') {
    strich(ctx,`M${x+21} ${y-18+welle} h5 l-5 5 h5 M${x+28} ${y-22+welle} h3 l-3 3 h3`,grau.schatten,1);
  } else if (pose === 'singen') {
    strich(ctx,`M${x+25} ${y-13} v-8 l4 1 v6`,grau.dunkel,1);
    punkte(ctx,[[x+22,y-13,2],[x+26,y-14,2]],grau.dunkel);
  } else if (pose === 'untersuchen' || pose === 'zeigen') {
    const fy = pose === 'untersuchen' ? boden-2 : boden-13;
    const fx = pose === 'untersuchen' ? x-14 : x-10;
    form(ctx,`M${fx-4} ${fy} l1 -2 l3 -1 l2 2 l-1 2 l-4 1 Z`,grau.fell,true);
    strich(ctx,`M${fx-1} ${fy-1} l2 -.7`,grau.weiss,.8);
    if (pose === 'zeigen') strich(ctx,`M${fx-4} ${fy-4} l-2 -1 M${fx} ${fy-5} v-2`,grau.schatten,.8);
  } else if (pose === 'denken' || pose === 'staunen') {
    if (pose === 'denken') {
      punkte(ctx,[[x+24,y-11,1],[x+27,y-13,1.5],[x+30,y-16,2]],grau.schatten);
    } else {
      strich(ctx,`M${x+22} ${y-17} v4 M${x+22} ${y-11} v.2`,grau.dunkel,1.35);
    }
  }
  if (emotion === 'traurig' && pose !== 'schlafen') {
    form(ctx,`M${x+11} ${y+3} q3 3 2 5 q-2 2 -3 0 q-1 -2 1 -5 Z`,grau.mittel);
  }
}

function zeichneFigur(canvas, phase, wahl, emotion = 'ruhe', pose = 'sitzen', frame = 0, effekte = true) {
  const ctx = canvas.getContext('2d', { willReadFrequently:true });
  const alter = altersstufen[phase];
  const stance = posen[pose];
  const koerperId = wahl.koerper;
  const basis = koerper[koerperId];
  const zyklus = stance.frames ? frame / stance.frames * 2 * Math.PI : 0;
  const welle = Math.sin(zyklus);
  const laeuft = pose === 'laufen';
  const schlaeft = pose === 'schlafen';
  const springt = pose === 'huepfen';
  const dreht = pose === 'drehen';
  const neigung = dreht ? Math.abs(welle) * .38 : 0;
  const ruecken = dreht && Math.cos(zyklus) < -.48;
  const boden = 67 - (springt ? Math.max(0,welle)*7 : 0);
  const x = 42;
  const hoehe = alter.hoch * (stance.hoehe || 1);
  const ausmass = alter.breit * (1-neigung);
  const haltung = stance.kopf || [0,0,0];
  const kopfX = x + (laeuft ? -3 : 0) + basis.achse * ausmass + haltung[0];
  const kopfY = alter.y + boden - 67 + haltung[1] + (laeuft ? Math.abs(welle) : 0);
  const blick = pose === 'fenster' ? 1 : pose === 'denken' || pose === 'untersuchen' ? -1 : 0;
  ctx.clearRect(0,0,canvas.width,canvas.height);
  ctx.save();
  ctx.translate(5,6);
  ctx.scale(1.2,1.2);
  if (dreht && welle < -.15) {
    ctx.translate(x*2,0);
    ctx.scale(-1,1);
  }

  const [sx,sy] = basis.ansatz;
  ctx.save();
  ctx.translate(x + sx * ausmass,boden + sy * hoehe + (schlaeft ? 1 : 0));
  ctx.scale(alter.schwanz * (1-neigung),alter.schwanz * (schlaeft ? .57 : 1));
  ctx.rotate(laeuft ? welle*.12 : schlaeft ? -.13 : 0);
  schwanz[wahl.schwanz](ctx);
  ctx.restore();

  ctx.save();
  ctx.translate(x,boden);
  ctx.scale(ausmass,hoehe);
  if (schlaeft) {
    maleSchlafkoerper(ctx,koerperId);
  } else {
    const laenge = (koerperId === 'lotti' ? 11.6 : 9.4) * alter.bein;
    for (const index of [0,3,1,2]) {
      const tief = index === 0 || index === 3;
      const schritt = laeuft ? Math.sin(zyklus + index * Math.PI/2) * (koerperId === 'lotti' ? 4.4 : 3.6) : 0;
      const angehoben = pose === 'zeigen' && index === 1 ? -7 : pose === 'denken' && index === 2 ? -4 : 0;
      const lift = laeuft ? -Math.max(0,Math.cos(zyklus + index * Math.PI/2))*3 : angehoben;
      malePfote(ctx,koerperId,basis.pfoten[index],tief,lift,laenge,schritt,-1,laeuft);
    }
    form(ctx,basis.form,basis.ton,true);
    if (!ruecken) basis.details(ctx);
    else strich(ctx,'M-6 -19 Q0 -21 6 -19 M-2 -16 l3 -1 M-5 -11 l2 1 M4 -12 l2 -1',grau.licht,1);
  }
  if (alter.haube === 'senior') strich(ctx,'M-9 -11 l2 2 M-6 -12 l2 1 M7 -9 l2 1',grau.creme,.75);
  if (emotion === 'schmutzig') {
    form(ctx,'M-8 -11 l3 -2 l2 1 l2 3 l-4 2 l-3 -1 Z M8 -8 l2 -2 l2 2 l-1 2 l-3 -1 Z',grau.schatten);
    punkte(ctx,[[-7,-9],[6,-6],[2,-12],[8,-12]],grau.tief);
  }
  ctx.restore();

  ctx.save();
  ctx.translate(kopfX,kopfY);
  ctx.rotate(haltung[2] + (pose === 'schnuppern' ? welle*.045 : 0));
  ctx.scale(alter.kopf * alter.rund * (1-neigung),alter.kopf * (schlaeft ? .73 : 1));
  ctx.save();
  ctx.translate(0,-8);
  ctx.scale(1,alter.ohr * (schlaeft ? .59 : 1));
  ctx.translate(0,8);
  ohren[wahl.ohren](ctx);
  ctx.restore();
  kopf(ctx,alter,ruecken);
  if (!ruecken) {
    augenZeichnen(ctx,wahl.augen,emotion,stance.gesicht,alter,blick);
    mundZeichnen(ctx,wahl.schnauze,emotion,stance.gesicht,pose,welle);
  }
  if (emotion === 'schmutzig') punkte(ctx,[[-13,-1,1.6],[-11,-.6],[13,1.3,1.28],[10,3.5,.9]],grau.schatten);
  ctx.restore();

  if (effekte && !ruecken) effekt(ctx,pose,emotion,kopfX,kopfY,boden,welle);
  ctx.restore();

  // Pixelkanten und dieselben 16 Stufen wie auf der Parkanzeige.
  const bild = ctx.getImageData(0,0,canvas.width,canvas.height);
  for (let i = 0; i < bild.data.length; i += 4) {
    const alpha = bild.data[i+3] >= 128 ? 255 : 0;
    const ton = alpha ? Math.round(bild.data[i]/17)*17 : 0;
    bild.data[i] = bild.data[i+1] = bild.data[i+2] = ton;
    bild.data[i+3] = alpha;
  }
  ctx.putImageData(bild,0,0);
}
