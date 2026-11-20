// Evi as two-colour pixel art. Pure drawing code: a pose goes in, pixel paths come out.
// No DOM, timers or game rules here, so the same shapes can later be exported as
// bitmaps for the handheld device.
//
// Design coordinates: Evi faces the viewer, her centre line is x = 32, her feet end at y = 60.
// Brown fur is shown as white with a sparse dot texture, cream fur (collar, tail tip) as pure white.
// The life stages follow the owner's drawings in entwuerfe/evi/ (egg, baby, teenager, grandma).

const OX = 6;                 // design x = -6 is grid column 0 (room for drooping ears)
const OY = 2;                 // design y = -2 is grid row 0 (room for the ear tips while breathing)
export const SPRITE = Object.freeze({ width: 82, height: 64, centerX: 32 + OX, footY: 63 });
const W = SPRITE.width, H = SPRITE.height;
const BLACK = 1, WHITE = 2, FUR = 3;

const ellipse = (cx, cy, rx, ry) => (x, y) => ((x - cx) / rx) ** 2 + ((y - cy) / ry) ** 2 <= 1;
const union = (...masks) => (x, y) => masks.some(mask => mask(x, y));
function polygon(points) {
  return (x, y) => {
    let inside = false;
    for (let i = 0, j = points.length - 1; i < points.length; j = i++) {
      const [xi, yi] = points[i], [xj, yj] = points[j];
      if ((yi > y) !== (yj > y) && x < (xj - xi) * (y - yi) / (yj - yi) + xi) inside = !inside;
    }
    return inside;
  };
}
const rotate = (points, [cx, cy], a) => points.map(([x, y]) => {
  const dx = x - cx, dy = y - cy;
  return [cx + dx * Math.cos(a) - dy * Math.sin(a), cy + dx * Math.sin(a) + dy * Math.cos(a)];
});
const mirror = points => points.map(([x, y]) => [64 - x, y]);
const mirrorPx = pixels => pixels.map(([x, y]) => [63 - x, y]);
const row = (y, ...xs) => xs.map(x => [x, y]);
// Sparse staggered dots (1 in 16 pixels) suggest the darker fur without looking busy.
const furDot = (x, y) => ((x & 3) === 1 && (y & 7) === 1) || ((x & 3) === 3 && (y & 7) === 5);

function paint(grid, mask, fill, { outline = true } = {}) {
  const inside = (gx, gy) => mask(gx - OX + 0.5, gy - OY + 0.5);
  for (let gy = 0; gy < H; gy++) {
    for (let gx = 0; gx < W; gx++) {
      if (!inside(gx, gy)) continue;
      const edge = outline && (!inside(gx - 1, gy) || !inside(gx + 1, gy) || !inside(gx, gy - 1) || !inside(gx, gy + 1));
      grid[gy * W + gx] = edge ? BLACK : fill === FUR ? (furDot(gx - OX, gy - OY) ? BLACK : WHITE) : fill;
    }
  }
}
function dots(grid, pixels, value, dy = 0) {
  for (const [x, y] of pixels) {
    const gx = x + OX, gy = y + OY + dy;
    if (gx >= 0 && gx < W && gy >= 0 && gy < H) grid[gy * W + gx] = value;
  }
}
const edgeAt = (mask, x, y) => !mask(x - 0.5, y + 0.5) || !mask(x + 1.5, y + 0.5) || !mask(x + 0.5, y - 0.5) || !mask(x + 0.5, y + 1.5);

// ---------- Life stages ----------
// Each stage scales the body parts and picks its own details. The feet always stay on the floor.
const STAGES = {
  baby: { body: 0.62, head: 0.84, ears: 0.74, tail: 0.6, earAngle: -0.28, tuft: 'single', eyeScale: 1.25,
    eyeSpread: 6.5, eyeDy: -1, mouthDy: -1, legs: false, ruff: 'small' },
  teen: { body: 0.86, head: 0.93, ears: 0.9, tail: 0.86, earAngle: -0.05, tuft: 'spiky', eyeScale: 1.05,
    eyeSpread: 6, eyeDy: 0, mouthDy: 0, legs: true, ruff: 'small' },
  adult: { body: 1, head: 1, ears: 1, tail: 1, earAngle: 0, tuft: 'adult', eyeScale: 1,
    eyeSpread: 6, eyeDy: 0, mouthDy: 0, legs: false, ruff: 'fluffy' },
  senior: { body: 1, head: 1, ears: 0.95, tail: 1, earAngle: -0.55, tuft: 'curl', eyeScale: 0.8,
    eyeSpread: 6, eyeDy: -2, mouthDy: 0, legs: false, ruff: 'shaggy', glasses: true, wrinkles: true }
};
const FOOT = [32, 60], HEAD_CENTER = [32, 26];
const scaled = (mask, [ax, ay], s, dy = 0) =>
  s === 1 && !dy ? mask : (x, y) => mask(ax + (x - ax) / s, ay + (y - dy - ay) / s);
const scaledPx = (pixels, [ax, ay], s, dy = 0) =>
  pixels.map(([x, y]) => [Math.round(ax + (x + 0.5 - ax) * s - 0.5), Math.round(ay + (y + 0.5 - ay) * s - 0.5 + dy)]);

// ---------- Body parts ----------

function tailMasks(angle) {
  const base = [42, 54];
  const [c, e] = rotate([[68, 47], [53, 20]], base, angle);
  const circles = [];
  for (let t = 0; t <= 1.0001; t += 0.03) {
    const u = 1 - t;
    circles.push({
      x: u * u * base[0] + 2 * u * t * c[0] + t * t * e[0],
      y: u * u * base[1] + 2 * u * t * c[1] + t * t * e[1],
      // Plume: thin at the root, widest high up, then a soft point that curls towards the head.
      r: 2.2 + 6 * Math.sin(Math.PI * Math.min(1, t * 1.05) * 0.88) * (1 - 0.35 * t), t
    });
  }
  const hit = (x, y, minT) => circles.some(k => k.t >= minT && (x - k.x) ** 2 + (y - k.y) ** 2 <= k.r * k.r);
  // The light tip has a jagged edge, as in the drawings.
  return { tail: (x, y) => hit(x, y, 0), tip: (x, y) => hit(x, y, (Math.floor(x) >> 1) & 1 ? 0.84 : 0.77) };
}

const EAR = [[29, 18], [22, 9], [13, 3], [5, 0.5], [5.5, 8], [9.5, 16], [19, 26]];
const EAR_PIVOT = [21, 20];
function earMasks(angle, right) {
  let outer = rotate(EAR, EAR_PIVOT, angle);
  let inner = outer.map(([x, y]) => [19 + (x - 19) * 0.58, 16.5 + (y - 16.5) * 0.58]);
  if (right) { outer = mirror(outer); inner = mirror(inner); }
  return { outer: polygon(outer), inner: polygon(inner) };
}

const tufts = list => list.map(([x, y, r]) => ellipse(x, y, r, r + 1.2));
const RUFFS = {
  // Fluffy collar: a core under the chin plus a row of tufts hanging down over the chest.
  fluffy: { mask: union(ellipse(32, 39, 14.5, 6.5), ...tufts([[18.5, 41.5, 3.6], [24.5, 45, 3.9], [32, 47, 4.2], [39.5, 45, 3.9], [45.5, 41.5, 3.6]])),
    strokes: [[21, 44], [21, 45], [27, 47], [27, 48], [36, 47], [36, 48], [42, 44], [42, 45]] },
  small: { mask: union(ellipse(32, 39, 12.5, 6), ...tufts([[21.5, 42, 3.2], [26.5, 44.5, 3.2], [32, 45.5, 3.4], [37.5, 44.5, 3.2], [42.5, 42, 3.2]])),
    strokes: [[24, 44], [24, 45], [29, 46], [34, 46], [39, 44], [39, 45]] },
  // Grandma's collar: longer, shaggier tufts.
  shaggy: { mask: union(ellipse(32, 39, 15.5, 6.5), ...tufts([[16.5, 42, 3], [21, 45, 3.2], [26.5, 47.5, 3.3], [32, 48.5, 3.4], [37.5, 47.5, 3.3], [43, 45, 3.2], [47.5, 42, 3]])),
    strokes: [[18, 44], [18, 45], [23, 47], [23, 48], [29, 49], [29, 50], [34, 49], [34, 50], [40, 47], [40, 48], [45, 44], [45, 45]] }
};

const HEAD_BASE = ellipse(32, 26, 15.5, 12);
const TUFTS = {
  adult: polygon([[25, 19], [28.5, 11.5], [30.6, 14.6], [32, 9], [33.4, 14.6], [35.5, 11.5], [39, 19]]),
  single: polygon([[28, 18], [32, 8.5], [36, 18]]),
  spiky: polygon([[24, 19], [26.5, 9.5], [29.2, 14.5], [32, 7], [34.8, 14.5], [37.5, 9.5], [40, 19]]),
  curl: null
};
// Grandma's hair curl: a white lock on top of the head with curved strands.
const CURL = union(ellipse(30, 13, 5, 4.2), ellipse(35.5, 14, 3.6, 3.4));
const CURL_STRANDS = [...row(11, 27, 28), ...row(10, 29, 30, 31), [32, 11], ...row(13, 29, 30), [31, 12], [32, 12], [33, 13],
  ...row(13, 35, 36), [37, 14]];

// ---------- Face ----------

function eyeCenters(stage) {
  return [[32 - stage.eyeSpread, 26 + stage.eyeDy], [32 + stage.eyeSpread, 26 + stage.eyeDy]];
}
function drawEyes(grid, eyes, stage, dy) {
  const s = stage.eyeScale, rx = 3.3 * s, ry = 4.4 * s;
  for (const [cx, cy] of eyeCenters(stage)) {
    const a = Math.floor(cx - rx - 0.6), b = Math.ceil(cx + rx + 0.6) - 1, mid = Math.floor(cy);
    if (eyes === 'closed') {
      dots(grid, [[a, mid], [b, mid], ...Array.from({ length: b - a - 1 }, (_, i) => [a + 1 + i, mid + 1])], BLACK, dy);
      continue;
    }
    if (eyes === 'happy') {
      const arc = [];
      for (let x = a; x <= b; x++) {
        const u = (x + 0.5 - cx) / ((b + 1 - a) / 2);
        const y = mid - 1 + Math.round(2.2 * u * u);
        arc.push([x, y]);
        if (x > a && x < b) arc.push([x, y + 1]);
      }
      dots(grid, arc, BLACK, dy);
      continue;
    }
    paint(grid, scaled(ellipse(cx, cy, rx + 1.2, ry + 1.2), [0, 0], 1, dy), WHITE, { outline: false });
    const eye = scaled(ellipse(cx, cy, rx, ry), [0, 0], 1, dy);
    paint(grid, eye, BLACK, { outline: false });
    if (eyes === 'tired') {
      // Heavy lids: the upper half of the eye is covered.
      const lid = Math.floor(cy - ry * 0.2);
      paint(grid, (x, y) => eye(x, y) && y - dy < lid, WHITE, { outline: false });
      dots(grid, Array.from({ length: b - a - 1 }, (_, i) => [a + 1 + i, lid]), BLACK, dy);
      dots(grid, [[Math.floor(cx - rx * 0.5), lid + 2], [Math.floor(cx - rx * 0.5) + 1, lid + 2]], WHITE, dy);
    } else {
      // A large highlight top left and a small one bottom right.
      const big = s > 1.15 ? 3 : 2, hx = Math.round(cx - rx * 0.62), hy = Math.round(cy - ry * 0.62);
      const sx = Math.round(cx + rx * 0.3), sy = Math.round(cy + ry * 0.42), small = s > 1.15 ? 2 : 1;
      const highlight = [];
      for (let i = 0; i < big; i++) for (let j = 0; j < big; j++) highlight.push([hx + i, hy + j]);
      for (let i = 0; i < small; i++) for (let j = 0; j < small; j++) highlight.push([sx + i, sy + j]);
      dots(grid, highlight, WHITE, dy);
    }
  }
}

function drawGlasses(grid, stage, dy) {
  const pixels = [];
  for (const [cx, cy] of eyeCenters(stage)) {
    // Round reading glasses sit low, over the lower half of the eyes.
    const gy = cy + 3.5;
    for (let y = Math.floor(gy - 6); y <= gy + 6; y++) {
      for (let x = Math.floor(cx - 6); x <= cx + 6; x++) {
        const d = Math.hypot(x + 0.5 - cx, y + 0.5 - gy);
        if (d >= 4 && d < 5) pixels.push([x, y]);
      }
    }
  }
  dots(grid, [...pixels, ...row(Math.floor(eyeCenters(stage)[0][1] + 3), 31, 32)], BLACK, dy);
}
// Laughter lines at the outer corners of the eyes.
const WRINKLES = [[17, 22], [18, 23], [17, 25], [18, 25], [17, 28], [18, 27]];

const NOSE = [[30, 30], [31, 30], [32, 30], [33, 30], [31, 31], [32, 31]];
// Mouths are drawn as black pixels; `tongue` pixels are white again inside an open mouth.
const MOUTHS = {
  // Open laughing mouth with raised corners and a little tongue.
  smile: { black: [...row(32, 28, 35), ...row(33, 29, 30, 31, 32, 33, 34), ...row(34, 30, 31, 32, 33), ...row(35, 31, 32)],
    tongue: row(34, 31, 32) },
  frown: { black: [...row(33, 30, 31, 32, 33), ...row(34, 29, 34)] },
  flat: { black: row(33, 30, 31, 32, 33) },
  open: { black: [...row(32, 30, 31, 32, 33), ...row(33, 29, 30, 31, 32, 33, 34), ...row(34, 29, 30, 31, 32, 33, 34),
    ...row(35, 30, 31, 32, 33)] }
};
const BROWS = [[23, 20], [24, 20], [25, 19], [26, 19]];
const BLUSH = [[20, 33], [21, 32], [22, 33], [23, 32]];
const DIRT = { body: [[17, 53], [18, 53], [17, 54], [45, 55], [46, 55], [30, 57], [31, 57]], head: [[37, 17], [38, 17], [38, 18], [20, 30], [24, 41], [25, 41], [25, 42]] };
const STINK = [[9, 36], [8, 37], [8, 38], [9, 39], [10, 40], [10, 41], [9, 42], [4, 41], [3, 42], [3, 43], [4, 44], [5, 45], [5, 46], [4, 47]];
// A small thermometer in the mouth when Evi is ill.
const THERMOMETER = [[36, 33], [37, 33], [38, 32], [39, 32], [40, 31], [41, 31], [42, 30], [43, 30], [44, 29], [44, 30], [45, 29], [45, 30]];
// Toe lines on the front paws.
const TOES = [[25, 59], [25, 60], [28, 59], [28, 60], [35, 59], [35, 60], [38, 59], [38, 60]];

// ---------- Pose ----------

export const DEFAULT_POSE = Object.freeze({
  stage: 'adult',     // egg | baby | teen | adult | senior | dead
  breath: 0,          // 0 or 1: head and collar rise by one pixel
  eyes: 'open',       // open | closed | happy | tired
  brows: false,       // worried brows
  mouth: 'smile',     // smile | frown | flat | open
  blush: false,
  tail: 0,            // tail rotation in radians; positive lets it droop
  earL: 0, earR: 0,   // ear rotation; negative lets the ear fall outward
  dirty: false,
  sick: false,
  wobble: 0,          // egg only: -1, 0 or 1
  crack: 0            // egg only: 0, 1 or 2
});

// The egg follows the drawing: a pointed top, a double zigzag band low down and rows of dots.
function buildEgg(grid, p) {
  const cy = 44, ry = 16, rx = 12.5;
  const lean = y => p.wobble * 1.5 * (60 - y) / 32;
  const shell = (x, y) => {
    const v = (y - cy) / ry;
    if (Math.abs(v) > 1) return false;
    const half = rx * Math.sqrt(1 - v * v) * (v < 0 ? 1 + 0.16 * v : 1);   // narrower towards the top
    return Math.abs(x - lean(y) - 32) <= half;
  };
  paint(grid, shell, WHITE);
  const inside = ([x, y]) => shell(x + 0.5, y + 0.5) && !edgeAt(shell, x, y);
  const place = pixels => pixels.map(([x, y]) => [x + Math.round(lean(y)), y]).filter(inside);
  const band = [];
  for (let x = 18; x <= 45; x++) {
    const tri = Math.abs(((x + 1) % 8) - 4);            // 0..4: three points up across the egg
    const y = 46 + Math.round(tri * 0.75);
    band.push([x, y], [x, y + 1]);
  }
  dots(grid, place(band), BLACK);
  dots(grid, place([[31, 32], [28, 36], [35, 36], [31, 37], [25, 40], [38, 40],
    [24, 53], [28, 53], [35, 53], [39, 53], [26, 56], [31, 57], [37, 56]]), BLACK);
  const zigzag = (from, to) => {
    const pixels = [];
    for (let x = from; x <= to; x++) { const y = 38 + ((x >> 1) & 1) * 2; pixels.push([x, y], [x, y + 1]); }
    return pixels;
  };
  dots(grid, place([[], zigzag(27, 36), zigzag(19, 44)][p.crack] ?? []), BLACK);
}

function buildGrave(grid) {
  paint(grid, ellipse(32, 61, 17, 3.5), FUR);
  paint(grid, union(polygon([[23, 60], [23, 40], [41, 40], [41, 60]]), ellipse(32, 40, 9, 8)), WHITE);
  dots(grid, [[31, 38], [32, 38], ...[39, 40, 41, 42, 43, 44, 45, 46, 47, 48].flatMap(y => [[31, y], [32, y]]),
    ...row(42, 28, 29, 30, 33, 34, 35), ...row(43, 28, 29, 30, 33, 34, 35)], BLACK);
  // Two little flowers in front of the stone.
  for (const fx of [19, 45]) {
    dots(grid, [[fx, 57], [fx, 58], [fx, 59], [fx - 1, 55], [fx + 1, 55], [fx, 54], [fx, 56]], BLACK);
    dots(grid, [[fx, 55]], WHITE);
  }
}

function buildGrid(pose) {
  const p = { ...DEFAULT_POSE, ...pose };
  const grid = new Uint8Array(W * H);
  if (p.stage === 'egg') { buildEgg(grid, p); return grid; }
  if (p.stage === 'dead') { buildGrave(grid); return grid; }

  const stage = STAGES[p.stage] ?? STAGES.adult;
  const sb = stage.body;
  const headDy = (FOOT[1] - HEAD_CENTER[1]) * (1 - sb);       // the head sits on the smaller body
  const up = Math.round(headDy) - p.breath;                    // pixel offset for face details
  const body = mask => scaled(mask, FOOT, sb);
  const head = (mask, s) => scaled(mask, HEAD_CENTER, s, headDy - p.breath);

  // Tail: scaled around its root, and the root moves with the smaller body.
  const tail = tailMasks(p.tail);
  const tailMask = scaled(tail.tail, [42, 54], stage.tail), tailTip = scaled(tail.tip, [42, 54], stage.tail);
  const shift = mask => (x, y) => mask(x - (42 - FOOT[0]) * (sb - 1), y - (54 - FOOT[1]) * (sb - 1));
  paint(grid, shift(tailMask), FUR);
  paint(grid, shift((x, y) => tailTip(x, y) && tailMask(x, y)), WHITE);

  paint(grid, body(ellipse(21.5, 53, 6.5, 5.5)), FUR);
  paint(grid, body(ellipse(42.5, 53, 6.5, 5.5)), FUR);
  paint(grid, body(ellipse(32, 49, 12.5, 10.5)), FUR);
  if (stage.legs) {
    paint(grid, body(polygon([[26.5, 42], [31.6, 42], [31.6, 58], [26.5, 58]])), FUR);
    paint(grid, body(polygon([[32.4, 42], [37.5, 42], [37.5, 58], [32.4, 58]])), FUR);
  }
  const ruff = RUFFS[stage.ruff];
  paint(grid, scaled(ruff.mask, FOOT, sb, -p.breath), WHITE);
  dots(grid, scaledPx(ruff.strokes, FOOT, sb, -p.breath), BLACK);
  paint(grid, body(ellipse(27, 57.6, 3.8, 2.6)), FUR);
  paint(grid, body(ellipse(37, 57.6, 3.8, 2.6)), FUR);
  dots(grid, scaledPx(TOES, FOOT, sb).filter(([x, y]) => sb > 0.7 || x % 2 === 0), BLACK);

  for (const [angle, right] of [[p.earL + stage.earAngle, false], [p.earR + stage.earAngle, true]]) {
    const ear = earMasks(angle, right);
    paint(grid, head(ear.outer, stage.ears), FUR);
    paint(grid, head(ear.inner, stage.ears), BLACK, { outline: false });
  }
  const tuft = TUFTS[stage.tuft];
  const headMask = head(tuft ? union(HEAD_BASE, tuft) : HEAD_BASE, stage.head);
  const face = head(ellipse(32, 28, 11.5, 8.5), stage.head);
  paint(grid, headMask, FUR);
  // The face stays free of fur dots so eyes and mouth read clearly on a small screen.
  paint(grid, (x, y) => face(x, y) && headMask(x, y) && !edgeAt(headMask, Math.floor(x), Math.floor(y)), WHITE, { outline: false });
  if (stage.tuft === 'curl') {
    paint(grid, head(CURL, stage.head), WHITE);
    dots(grid, scaledPx(CURL_STRANDS, HEAD_CENTER, stage.head, headDy - p.breath), BLACK);
  }

  drawEyes(grid, p.eyes, stage, up);
  if (stage.glasses) drawGlasses(grid, stage, up);
  if (stage.wrinkles && p.eyes !== 'closed') dots(grid, [...WRINKLES, ...mirrorPx(WRINKLES)], BLACK, up);
  const mouthUp = up + stage.mouthDy;
  if (p.brows) dots(grid, [...BROWS, ...mirrorPx(BROWS)], BLACK, up + stage.eyeDy);
  if (p.blush && !stage.glasses) dots(grid, [...BLUSH, ...mirrorPx(BLUSH)], BLACK, up);
  dots(grid, NOSE, BLACK, mouthUp);
  const mouth = MOUTHS[p.mouth] ?? MOUTHS.smile;
  dots(grid, mouth.black, BLACK, mouthUp);
  if (mouth.tongue) dots(grid, mouth.tongue, WHITE, mouthUp);
  if (p.sick) dots(grid, THERMOMETER, BLACK, mouthUp);
  if (p.dirty) {
    dots(grid, scaledPx(DIRT.body, FOOT, sb), BLACK);
    dots(grid, DIRT.head, BLACK, up);
    dots(grid, scaledPx(STINK, FOOT, sb), BLACK);
  }
  return grid;
}

function toPath(grid, value) {
  let d = '';
  for (let y = 0; y < H; y++) {
    for (let x = 0; x < W;) {
      if (grid[y * W + x] !== value) { x++; continue; }
      const start = x;
      while (x < W && grid[y * W + x] === value) x++;
      d += `M${start} ${y}h${x - start}v1h-${x - start}z`;
    }
  }
  return d;
}

const cache = new Map();
/** Returns SVG path data for the white and the black pixels of one pose. */
export function drawEvi(pose = {}) {
  const key = JSON.stringify({ ...DEFAULT_POSE, ...pose });
  let result = cache.get(key);
  if (!result) {
    const grid = buildGrid(pose);
    result = { white: toPath(grid, WHITE), black: toPath(grid, BLACK) };
    if (cache.size > 200) cache.clear();
    cache.set(key, result);
  }
  return result;
}
