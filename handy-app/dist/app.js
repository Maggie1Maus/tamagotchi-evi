import { RULES, createPet, restorePet, advancePet, careForPet, finishPlay, renamePet, describePet,
  stageAt, stageName, blockedReason, isNight, isAsleep, sleepKind, isTired, isRested, mealAt, mealDue,
  currentWish, isWoken, stayAwake } from './pet.js';
import { drawEvi } from './evi-sprite.js';
import { MOMENTS, momentsFor, nextDelay, chooseMoment, dueSpecial, greeting } from './moments.js';
import { ART } from './art.js';

// Version 0.7 started the game anew in a new save slot. Since 0.9 a browser that never had
// 0.7 or 0.8 keeps its Evi from the earlier slot instead of starting with a new egg.
const SAVE_KEY = 'kleines-zuhause.pet.v2';
const OLD_SAVE_KEY = 'kleines-zuhause.pet.v1';
const RESTART_EGG_MS = 60000;             // one-off: this first egg hatches after one minute
const SETTINGS_KEY = 'kleines-zuhause.settings';
const $ = id => document.getElementById(id);
const scene = $('scene-wrap');
const gameMenu = $('game-menu');
const confirmButton = $('confirm-button');
const nameInput = $('name-input');
const TICK_MS = 120;
let selected = 0;
let pet;
let storageError = false;
let invalidSave = false;
let lastSaved = '';
let reactionMessage = '';
let reactionTimer;
let missedNote = '';
let lastSeen = 0;           // when the app was last open, for Evi's greeting

// ---------- Loading ----------

/** What happened while the app was closed, as one short sentence. */
function missedEvents(before, after) {
  const now = Date.now(), from = stageAt(before, before.updatedAt), to = stageAt(after, now);
  const name = after.name;
  if (to === 'dead' && from !== 'dead') return after.causeOfDeath === 'age'
    ? `${name} ist friedlich eingeschlafen, während du weg warst.` : `${name} ist gestorben, während du weg warst.`;
  if (from === 'egg' && to !== 'egg') return `${name} ist geschlüpft, während du weg warst.`;
  if (from !== to && to === 'teen') return `${name} ist jetzt ein Teenager!`;
  if (from !== to && to === 'adult') return `${name} ist erwachsen geworden!`;
  if (from !== to && to === 'senior') return `${name} ist jetzt eine Oma!`;
  if (!before.sick && after.sick) return `${name} ist krank geworden.`;
  return '';
}

try {
  const raw = localStorage.getItem(SAVE_KEY) ?? localStorage.getItem(OLD_SAVE_KEY);
  if (raw) {
    let parsed = null;
    try { parsed = JSON.parse(raw); pet = restorePet(parsed, Date.now()); } catch { pet = null; }
    invalidSave = !pet;
    if (pet && parsed?.version >= 2) missedNote = missedEvents(parsed, pet);
    if (pet && Number.isFinite(parsed?.updatedAt)) lastSeen = parsed.updatedAt;
  }
} catch { storageError = true; }
pet ??= createPet(Date.now(), { eggDuration: RESTART_EGG_MS });

let settings = { sound: true };
try { settings = { ...settings, ...JSON.parse(localStorage.getItem(SETTINGS_KEY) || '{}') }; } catch { /* defaults */ }

function save() {
  try {
    if (invalidSave) throw new Error('Unrecognized save; preserve original.');
    const serialized = JSON.stringify(pet);
    if (lastSaved !== serialized) localStorage.setItem(SAVE_KEY, serialized);
    lastSaved = serialized;
    storageError = false;
  } catch { storageError = true; }
  const note = $('save-state');
  note.hidden = !storageError;
  const message = invalidSave
    ? 'Alter Spielstand nicht lesbar. Diese Sitzung wird nicht gespeichert.'
    : 'Speichern nicht möglich. Bitte Browser-Speicher erlauben.';
  if (storageError && note.textContent !== message) note.textContent = message;
}
function saveSettings() {
  try { localStorage.setItem(SETTINGS_KEY, JSON.stringify(settings)); } catch { /* only this session */ }
}

// ---------- Sound (a short beep, like the handheld's buzzer) ----------

let audio;
function unlockAudio() {
  try {
    audio ??= new (window.AudioContext || window.webkitAudioContext)();
    if (audio.state === 'suspended') audio.resume();
  } catch { audio = null; }
}
const SOUNDS = {
  hatch: [[660, 110], [880, 110], [1320, 260]],
  evolve: [[523, 100], [659, 100], [784, 220]],
  alert: [[880, 90], [0, 60], [880, 90]],
  sad: [[330, 220], [262, 360]],
  coin: [[1320, 60], [1760, 90]],
  miss: [[196, 140]],
  crack: [[440, 40]],
  wish: [[784, 90], [1047, 140]],
  special: [[1047, 70], [1319, 70], [1568, 70], [2093, 200]]
};
function beep(name) {
  if (!settings.sound || !audio) return;
  let t = audio.currentTime;
  for (const [freq, ms] of SOUNDS[name]) {
    if (freq) {
      const osc = audio.createOscillator(), gain = audio.createGain();
      osc.type = 'square';
      osc.frequency.value = freq;
      gain.gain.value = 0.04;
      osc.connect(gain).connect(audio.destination);
      osc.start(t);
      osc.stop(t + ms / 1000);
    }
    t += ms / 1000 + 0.03;
  }
  if (!['coin', 'miss', 'crack', 'wish', 'special'].includes(name)) {
    try { navigator.vibrate?.(160); } catch { /* not supported */ }
  }
}
/** One tone, used for the pads in "Melodie merken". */
function tone(freq, ms) {
  if (!settings.sound || !audio) return;
  const osc = audio.createOscillator(), gain = audio.createGain();
  osc.type = 'square';
  osc.frequency.value = freq;
  gain.gain.value = 0.04;
  osc.connect(gain).connect(audio.destination);
  osc.start();
  osc.stop(audio.currentTime + ms / 1000);
}
document.addEventListener('pointerdown', unlockAudio);
document.addEventListener('keydown', unlockAudio);

// ---------- Evi's animation (display only; the game rules live in pet.js) ----------

const reducedMotion = matchMedia('(prefers-reduced-motion: reduce)');
const random = (min, max) => min + Math.floor(Math.random() * (max - min + 1));
const evi = {
  stage: 'egg', mood: 'egg', dirty: false, sick: false, eggSoon: false, tick: 0, lastKey: '',
  breath: 0, nextBreath: 11,
  blinkUntil: 0, nextBlink: random(20, 45),
  wagUntil: 0, nextWag: random(15, 40),
  twitchUntil: 0, twitchSide: 'L', nextTwitch: random(40, 90),
  wobbleUntil: 0, nextWobble: random(15, 40),
  hatchUntil: 0, hatchPending: false,
  reaction: '', reactionUntil: 0,
  artOverride: ''             // a moment can show another of her drawings for a while, e.g. 'muede' to yawn
};

function eviPose() {
  const t = evi.tick, still = reducedMotion.matches;
  if (evi.stage === 'dead') return { stage: 'dead' };
  if (evi.stage === 'egg' || t < evi.hatchUntil) {
    const hatching = t < evi.hatchUntil;
    const crack = hatching ? (evi.hatchUntil - t > 8 ? 1 : 2) : 0;
    const fast = hatching || evi.eggSoon;
    const wobbling = !still && (fast || t < evi.wobbleUntil);
    return { stage: 'egg', crack, wobble: wobbling ? [0, 1, 0, -1][Math.floor(t / (fast ? 1 : 2)) % 4] : 0 };
  }
  const pose = { stage: evi.stage, breath: still ? 0 : evi.breath, eyes: 'open', brows: false, mouth: 'smile',
    blush: false, tail: 0, earL: 0, earR: 0, dirty: evi.dirty, sick: false };
  if (evi.mood === 'sleeping') {
    Object.assign(pose, { eyes: 'closed', mouth: 'flat', earL: -0.12, earR: -0.12, tail: 0.12 });
    return pose;
  }
  if (evi.mood === 'happy') pose.blush = true;
  if (evi.mood === 'hungry') Object.assign(pose, { brows: true, mouth: 'open', tail: 0.18, earL: -0.12, earR: -0.12 });
  if (evi.mood === 'sad') Object.assign(pose, { brows: true, mouth: 'frown', tail: 0.3, earL: -0.3, earR: -0.3 });
  if (evi.mood === 'dirty') Object.assign(pose, { eyes: 'tired', mouth: 'flat' });
  if (evi.mood === 'sick' || evi.mood === 'tummy') Object.assign(pose, { eyes: 'tired', brows: true, mouth: 'frown', sick: true, tail: 0.25, earL: -0.25, earR: -0.25 });
  if (!still) {
    if (t < evi.blinkUntil) pose.eyes = 'closed';
    if (evi.mood === 'happy' && t < evi.wagUntil) pose.tail = [0, 0.08, 0, -0.08][Math.floor(t / 2) % 4];
    if (t < evi.twitchUntil) pose[`ear${evi.twitchSide}`] -= 0.14;
  }
  if (t < evi.reactionUntil) {
    Object.assign(pose, { eyes: 'happy', brows: false, blush: true, earL: 0, earR: 0, sick: false });
    pose.mouth = evi.reaction === 'feed' ? (Math.floor(t / 2) % 2 ? 'open' : 'smile') : 'smile';
    if (evi.reaction === 'medicine') Object.assign(pose, { eyes: 'closed', mouth: 'frown', blush: false });
    pose.tail = still ? 0 : [0, 0.1, 0, -0.1][t % 4];
    if (evi.reaction === 'clean') pose.dirty = false;
  }
  return pose;
}

// The owner's drawings (art.js, generated from entwuerfe/evi/) are shown exactly as drawn.
// All pictures of one stage share one scale, so a lying, sleeping Evi is lower, not squeezed.
// Scene units per picture pixel:
const ART_SCALE = { egg: 0.232, baby: 0.344, teen: 0.372, adult: 0.38, senior: 0.4 };
const FLOOR = { x: 167, y: 180 };
const MOOD_ART = { sleeping: 'schlafend', sick: 'krank', tummy: 'krank', hungry: 'hungrig', dirty: 'schmutzig', sad: 'traurig' };

/** Which drawing fits Evi right now, or null to use the pixel sprite. */
function artChoice(pose) {
  const stage = pose.stage === 'egg' ? 'egg' : evi.stage;
  const set = ART[stage];
  if (!set) return null;
  if (stage === 'egg') return [stage, set.base];
  const cheering = evi.tick < evi.reactionUntil && evi.reaction !== 'medicine';
  const glad = ['greet', 'cheer'].includes(evi.reaction) && set[MOMENT_ART.hop] ? MOMENT_ART.hop : 'gluecklich';
  const mood = cheering ? glad : evi.artOverride && set[evi.artOverride] ? evi.artOverride
    : MOOD_ART[evi.mood] ?? (evi.tired ? 'muede' : evi.allGood ? 'gluecklich' : 'base');
  return [stage, set[mood] ?? set.base ?? set.gluecklich];
}

function drawEviNow() {
  const pose = eviPose();
  const choice = artChoice(pose);
  const key = JSON.stringify(pose) + (choice ? choice[1][0] : '');
  if (key === evi.lastKey) return;
  evi.lastKey = key;
  const image = $('evi-image');
  if (choice) {
    const [stage, [src, w, h]] = choice, width = w * ART_SCALE[stage], height = h * ART_SCALE[stage];
    if (image.getAttribute('href') !== src) image.setAttribute('href', src);
    image.setAttribute('width', width.toFixed(1));
    image.setAttribute('height', height.toFixed(1));
    image.setAttribute('x', (FLOOR.x - width / 2).toFixed(1));
    image.setAttribute('y', (FLOOR.y - height - (pose.breath ? 2 : 0)).toFixed(1));
    if (pose.wobble) image.setAttribute('transform', `rotate(${pose.wobble * 5} ${FLOOR.x} ${FLOOR.y})`);
    else image.removeAttribute('transform');
    image.setAttribute('visibility', 'visible');
    $('evi-white').setAttribute('d', '');
    $('evi-black').setAttribute('d', '');
    document.querySelector('.pet-shadow').setAttribute('rx', Math.round(width * 0.32));
    return;
  }
  image.setAttribute('visibility', 'hidden');
  document.querySelector('.pet-shadow').setAttribute('rx', 34);
  const sprite = drawEvi(pose);
  $('evi-white').setAttribute('d', sprite.white);
  $('evi-black').setAttribute('d', sprite.black);
}

function eviTick() {
  if (document.visibilityState !== 'visible') return;
  const t = ++evi.tick;
  const slow = ['sad', 'hungry', 'sick', 'sleeping'].includes(evi.mood);
  if (t >= evi.nextBreath) { evi.breath ^= 1; evi.nextBreath = t + (evi.mood === 'sleeping' ? 20 : slow ? 15 : 11); }
  if (t >= evi.nextBlink) { evi.blinkUntil = t + 2; evi.nextBlink = t + random(25, 55); }
  if (t >= evi.nextWag) { evi.wagUntil = t + 16; evi.nextWag = t + random(35, 80); }
  if (t >= evi.nextWobble) { evi.wobbleUntil = t + 8; evi.nextWobble = t + random(30, 70); }
  if (t >= evi.nextTwitch) {
    evi.twitchSide = Math.random() < 0.5 ? 'L' : 'R';
    evi.twitchUntil = t + 3;
    evi.nextTwitch = t + random(50, 120);
  }
  if (evi.hatchPending && t === evi.hatchUntil - 8) beep('crack');
  if (evi.hatchPending && t >= evi.hatchUntil) {
    evi.hatchPending = false;
    react('hatch', 25);
    beep('hatch');
    flash(`${pet.name} ist geschlüpft! Willkommen!`, 6000);
  }
  if (evi.mood === 'sleeping' && t % 8 === 0) $('sleep-z-text').textContent = ['z', 'zZ', 'zZz'][(t / 8) % 3];
  runMoments();
  // Hatch exactly on time while someone is watching.
  if (evi.stage === 'egg' && Date.now() >= pet.hatchAt) tick();
  drawEviNow();
}

function react(action, ticks = Math.round(2400 / TICK_MS)) {
  evi.reaction = action;
  evi.reactionUntil = evi.tick + ticks;
  scene.classList.remove('reacting');
  scene.dataset.action = action;
  $('reaction-symbol').textContent = { feed: '♥', snack: '♥', play: '♪', clean: '✦', medicine: '+', hatch: '✦', evolve: '✦',
    greet: '♥', cheer: '♥' }[action] ?? '♥';
  void scene.getBoundingClientRect();
  scene.classList.add('reacting');
  setTimeout(() => scene.classList.remove('reacting'), 2400);
}

// ---------- Texts ----------

const MEAL_NAMES = { breakfast: 'Frühstück', lunch: 'Mittagessen', dinner: 'Abendessen' };
/** Icon and short text of a wish, for the speech bubble and the caption. */
function wishText(wish, now) {
  const meal = mealDue(pet, now);
  return {
    ball: ['i-ball', 'Ballfangen!'], guess: ['i-arrows', 'Links oder rechts?'], simon: ['i-note', 'Melodie merken!'],
    snack: ['i-cookie', 'Was Süßes!'], clean: ['i-clean', 'Baden!'], feed: ['i-food', meal ? `${MEAL_NAMES[meal]}!` : 'Hunger!']
  }[wish.kind];
}

function moodMessage(d) {
  const name = pet.name, now = Date.now();
  if (ritualShow.active) return 'Gute Nacht …';
  switch (d.mood) {
    case 'dead': return pet.causeOfDeath === 'age' ? `${name} ist friedlich eingeschlafen. Menü: neues Ei` : `${name} ist gestorben. Menü: neues Ei`;
    case 'egg': {
      const minutes = Math.max(1, Math.ceil((pet.hatchAt - Date.now()) / 60000));
      return `Schlüpft in ca. ${minutes} Min.`;
    }
    case 'sleeping':
      if (sleepKind(pet, now) === 'nap') return `Psst … Mittagsschlaf bis ${RULES.nap.to} Uhr`;
      return pet.lightOff ? 'Psst … schläft' : 'Schläft · Licht ausmachen?';
    case 'sick': return 'Krank · braucht Medizin';
    case 'tummy': return 'Bauchweh … zu viel genascht';
    case 'hungry': return 'Mein Bauch knurrt';
    case 'dirty': return 'Zeit für ein Bad';
    case 'sad': return 'Spielst du mit mir?';
    default: {
      if (isWoken(pet, now)) return `Gähn … wach für ${Math.max(1, Math.ceil((pet.awakeUntil - now) / 60000))} Min.`;
      const wish = currentWish(pet, now);
      if (wish) return 'Ein Wunsch!';
      if (isRested(pet, now)) return 'Ausgeschlafen! Guten Morgen';
      if (isTired(pet, now)) return new Date(now).getHours() < 12 ? 'Gähn … guten Morgen' : 'Gähn … Zeit für Gute Nacht?';
      return 'Mitte öffnet das Menü';
    }
  }
}

function reasonText(reason, action) {
  const name = pet.name;
  return {
    dead: `${name} lebt nicht mehr.`,
    egg: `Erst muss ${name} schlüpfen.`,
    day: 'Es ist noch hell. Schlafenszeit ist um 23 Uhr.',
    asleep: `${name} schläft gerade.`,
    nap: `Psst, ${name} macht Mittagsschlaf.`,
    early: `Gute Nacht gibt es ab ${RULES.ritual.from} Uhr.`,
    done: 'Heute gab es schon Gute Nacht.',
    awake: `${name} ist schon wach.`,
    tummy: `${name} hat Bauchweh. Erst mal nichts Süßes.`,
    sick: `${name} ist zu krank zum Spielen.`,
    healthy: `${name} ist gesund.`,
    full: { feed: 'Schon satt', clean: 'Schon sauber', play: 'Schon glücklich' }[action] ?? 'Gerade nicht nötig'
  }[reason] ?? 'Das geht gerade nicht.';
}

function flash(message, ms = 2400) {
  reactionMessage = message;
  clearTimeout(reactionTimer);
  reactionTimer = setTimeout(() => { reactionMessage = ''; renderCaption(); }, ms);
  renderCaption();
}

function renderCaption() {
  let message;
  if (game.active) {
    const status = `${game.round}/${RULES.play.rounds}`;
    message = game.kind === 'ball' ? game.note || `Ball ${status} · ${game.hits} gefangen · rechts beendet`
      : game.kind === 'guess' ? `${status} · ${game.note}` : `${status} · ${game.note}`;
  }
  else if (!gameMenu.hidden) message = 'Links weiter · Mitte OK · Rechts zurück';
  else message = reactionMessage || moodMessage(describePet(pet, Date.now()));
  const caption = $('pet-message');
  if (caption.textContent !== message) caption.textContent = message;
}

// ---------- Rendering ----------

function render() {
  const now = Date.now();
  const d = describePet(pet, now);
  // During the evening ritual Evi is shown awake and tired until the light goes out.
  if (ritualShow.active) d.mood = 'happy';
  $('pet-name').textContent = pet.name;
  document.title = `${pet.name}s kleines Zuhause`;
  $('room-title').textContent = `${pet.name} (${stageName(d.stage)}) im Zimmer`;
  $('stage-badge').textContent = stageName(d.stage);
  $('coins-value').textContent = pet.coins;
  scene.dataset.mood = d.mood;
  if (['egg', 'dead', 'sleeping', 'sick', 'tummy'].includes(d.mood) && (moment.gen || walker.x || walker.flip)) stopMoment(true);
  scene.classList.toggle('sleeping', d.mood === 'sleeping');
  scene.classList.toggle('lights-off', d.mood === 'sleeping' && pet.lightOff);
  evi.stage = d.stage;
  evi.mood = d.mood;
  evi.sick = pet.sick;
  evi.eggSoon = d.stage === 'egg' && pet.hatchAt - now < 2 * 60000;
  evi.dirty = !['egg', 'dead'].includes(d.stage) && Math.round(pet.cleanliness) <= RULES.low;
  // Tired: the hour before bedtime and just after waking. Glad: every need at 60 or more, or well rested.
  evi.tired = ritualShow.active || (d.mood === 'happy' && isTired(pet, now));
  evi.allGood = ['satiety', 'mood', 'cleanliness'].every(key => pet[key] >= 60) || isRested(pet, now);
  drawEviNow();
  for (const key of ['satiety', 'mood', 'cleanliness']) {
    const value = Math.round(pet[key]);
    $(`${key}-value`).textContent = `${value}%`;
    const meter = $(`${key}-meter`);
    meter.setAttribute('aria-valuenow', value);
    meter.firstElementChild.style.width = `${value}%`;
    meter.parentElement.classList.toggle('low', value <= RULES.low && !['egg', 'dead'].includes(d.stage));
  }
  $('window-sky').setAttribute('fill', isNight(now) || sleepKind(pet, now) === 'night' ? '#000000' : '#ffffff');
  renderWish(now);
  renderCaption();
}

// ---------- Wishes ----------

let shownWish = null;      // start time of the wish in the bubble, to notice a new one
// Something Evi says for a few seconds ("Danke!", a greeting). It covers a wish only for that long.
let saying = { text: '', icon: '', until: 0 };
let sayTimer;
function renderWish(now) {
  const bubble = $('wish-bubble');
  const wish = ritualShow.active || game.active ? null : currentWish(pet, now);
  if (wish && wish.start !== shownWish && shownWish !== null) beep('wish');
  shownWish = wish ? wish.start : 0;
  const talking = now < saying.until;
  bubble.hidden = !wish && !talking;
  if (bubble.hidden) return;
  const [icon, text] = talking ? [saying.icon, saying.text] : wishText(wish, now);
  bubble.querySelector('use').setAttribute('href', `#${icon}`);
  bubble.querySelector('span').textContent = text;
  bubble.setAttribute('aria-label', talking ? `${pet.name} sagt: ${text}` : `${pet.name} wünscht sich: ${text}`);
}
function say(text, icon = 'i-heart', ms = 3000) {
  saying = { text, icon, until: Date.now() + ms };
  renderWish(Date.now());
  clearTimeout(sayTimer);
  sayTimer = setTimeout(() => renderWish(Date.now()), ms + 50);
}
/** After a fulfilled wish: thanks in the bubble and a coin sound. */
function wishGranted() {
  say('Danke!', 'i-heart', 3000);
  beep('coin');
  setTimeout(() => render(), 3100);
}

/** Sounds and messages for things that happen while the app is open. */
function announce(before, after) {
  const now = Date.now();
  const from = stageAt(before, before.updatedAt), to = stageAt(after, now);
  const name = after.name;
  if (from !== to) {
    if (from === 'egg' && to !== 'dead') {
      evi.hatchUntil = evi.tick + 16;
      evi.hatchPending = true;
      flash('Das Ei bricht auf …', 2000);
    } else if (to === 'dead') {
      beep('sad');
      flash(after.causeOfDeath === 'age' ? `${name} ist friedlich eingeschlafen.` : `${name} ist gestorben.`, 8000);
    } else if (['teen', 'adult', 'senior'].includes(to)) {
      beep('evolve');
      react('evolve', 25);
      flash({ teen: `${name} ist jetzt ein Teenager!`, adult: `${name} ist erwachsen geworden!`,
        senior: `${name} ist jetzt eine Oma!` }[to], 6000);
    }
    return;
  }
  if (!before.sick && after.sick) { beep('alert'); flash(`${name} ist krank geworden!`, 5000); return; }
  if (isWoken(before, before.updatedAt) && sleepKind(after, now) === 'night') { flash(`${name} ist wieder eingeschlafen.`, 5000); return; }
  if (!isAsleep(after, now) && ['satiety', 'mood', 'cleanliness'].some(key => before[key] > RULES.low && after[key] <= RULES.low)) beep('alert');
}

function tick() {
  const before = pet;
  pet = advancePet(pet, Date.now());
  announce(before, pet);
  render();
  save();
}

// ---------- Menu ----------

function menuEntries() {
  const now = Date.now();
  const stage = stageAt(pet, now);
  if (stage === 'dead') return [['newEgg', 'i-egg', 'Neues Ei'], ['help', 'i-help', 'Hilfe']];
  const entries = [];
  const asleep = sleepKind(pet, now);
  if (stage !== 'egg') {
    if (asleep === 'night') entries.push(['wake', 'i-sun', 'Aufwecken'], ['light', 'i-bulb', pet.lightOff ? 'Licht an' : 'Licht aus']);
    else if (!asleep) {
      if (!blockedReason(pet, 'ritual', now)) entries.push(['ritual', 'i-moon', 'Gute Nacht']);
      if (!blockedReason(pet, 'medicine', now)) entries.push(['medicine', 'i-pill', 'Medizin']);
      entries.push(['feed', 'i-food', mealDue(pet, now) ? MEAL_NAMES[mealAt(now)] : 'Füttern'], ['snack', 'i-cookie', 'Naschen'],
        ['play', 'i-ball', 'Spielen'], ['clean', 'i-clean', 'Saubermachen']);
    }
  }
  entries.push(['rename', 'i-edit', 'Name ändern'], ['sound', 'i-sound', settings.sound ? 'Ton aus' : 'Ton an'], ['help', 'i-help', 'Hilfe']);
  return entries;
}

// The menu shows the main entries, or after "Spielen" the choice of games.
let menuMode = 'main';
function buildMenu() {
  const entries = menuMode === 'games' ? GAMES.map(([kind, icon, label]) => [`game:${kind}`, icon, label]) : menuEntries();
  gameMenu.setAttribute('aria-label', menuMode === 'games' ? 'Spiel auswählen' : 'Spielmenü');
  gameMenu.replaceChildren(...entries.map(([option, icon, label]) => {
    const item = document.createElement('li');
    item.dataset.option = option;
    item.innerHTML = `<svg aria-hidden="true"><use href="#${icon}"/></svg>`;
    item.append(label);
    return item;
  }));
  selected = 0;
}

function renderSelection() {
  const items = [...gameMenu.children];
  for (const [index, item] of items.entries()) item.setAttribute('aria-current', String(index === selected));
  const item = items[selected];
  item.scrollIntoView({ block: 'nearest', inline: 'nearest' });
  $('menu-selection').textContent = item.textContent.trim();
}

function showMenu(open, mode = 'main') {
  menuMode = mode;
  if (open) buildMenu();
  gameMenu.hidden = !open;
  confirmButton.setAttribute('aria-expanded', String(open));
  if (open) renderSelection();
  else $('menu-selection').textContent = '';
  renderCaption();
}

function openRename() {
  nameInput.value = pet.name;
  nameInput.setCustomValidity('');
  $('name-dialog').showModal();
  nameInput.focus();
  nameInput.select();
}

const careMessages = { feed: 'Mmh, lecker!', snack: 'Mmh, ein Keks!', clean: 'Blitzblank!', medicine: 'Bäh … aber es hilft!' };

function performAction(action) {
  if (action === 'rename') { openRename(); return; }
  if (action === 'help') { $('help-dialog').showModal(); return; }
  if (action === 'sound') {
    settings.sound = !settings.sound;
    saveSettings();
    unlockAudio();
    beep('coin');
    flash(settings.sound ? 'Ton an' : 'Ton aus');
    return;
  }
  const now = Date.now();
  if (action === 'play' || action.startsWith('game:')) {
    const current = advancePet(pet, now);
    const reason = blockedReason(current, 'play', now);
    if (reason && reason !== 'full') { flash(reasonText(reason, 'play')); return; }
    if (action === 'play') showMenu(true, 'games');
    else startGame(action.slice(5));
    return;
  }
  // Whatever Evi was doing by herself, now you are taking care of her.
  stopMoment();
  moment.quietUntil = evi.tick + QUIET_TICKS;
  const before = pet;
  const result = careForPet(pet, action, now);
  pet = result.pet;
  announce(before, pet);
  if (!result.changed) {
    flash(reasonText(result.reason, action));
  } else if (action === 'light') {
    flash(pet.lightOff ? (result.bonus ? 'Gute Nacht! ♥' : 'Licht aus') : 'Licht an');
  } else if (action === 'ritual') {
    startRitualShow();
  } else if (action === 'wake') {
    flash(`Gähn … ${pet.name} ist wach.`, 3000);
  } else if (action === 'newEgg') {
    flash('Ein neues Ei! Es schlüpft in 30 Minuten.', 5000);
  } else {
    react(action);
    let message = careMessages[action];
    if (result.meal) message = `Mmh, ${MEAL_NAMES[result.meal]}!`;
    if (action === 'snack' && pet.snackCount === RULES.snack.perDay) message = 'Mmh! Jetzt ist aber genug Süßes.';
    if (result.tummy) { message = 'Aua, Bauchweh! Zu viel genascht.'; beep('miss'); }
    flash(message);
  }
  if (result.wish) wishGranted();
  render();
  save();
}

// ---------- Evening ritual ----------
// Brush teeth, a bedtime story, tucked in, light off. The rules apply at once (pet.js);
// this only shows it. Any key skips to the end.

const STORIES = [
  'Ein kleiner Stern konnte nicht schlafen. Da summte ihm der Mond ein Lied, und der Stern funkelte ganz leise ein.',
  'Ein Igel suchte im Wald das weichste Blatt für sein Bett. Am Ende lag es direkt vor seiner Haustür.',
  'Ein Fuchs zählte abends die Sterne über seinem Bau. Bei hundertzwei fielen ihm die Augen zu.',
  'Die Maus Pina backte einen Käsekuchen für den Mond. Der Mond lächelte und schickte ihr dafür einen schönen Traum.',
  'Im Garten flüsterten die Blumen sich Gute-Nacht-Witze zu. Die Glühwürmchen lachten so sehr, dass sie leuchteten.',
  'Eine kleine Eule flog heute zum ersten Mal drei Bäume weit. Danach schlief sie stolz in ihrem Astloch ein.',
  'Ein Teddybär hält heimlich Wache, damit nur schöne Träume kommen. Dafür bekommt er morgen eine extra Umarmung.',
  'Ein Fuchs und ein Hase wetteten, wer schneller einschläft. Am Ende schnarchten beide gleichzeitig.',
  'Eine Schneeflocke landete auf {name}s Nase. Es war dort so gemütlich, dass sie gar nicht schmelzen wollte.',
  'Der Wind trug eine kleine Wolke über das Meer. Sie wurde ganz müde und kuschelte sich an einen Berg.'
];
const ritualShow = { active: false, timers: [] };
function startRitualShow() {
  const story = STORIES[random(0, STORIES.length - 1)].replaceAll('{name}', pet.name);
  const steps = [
    ['Zähne putzen … schrubb, schrubb!', 2600, () => react('clean')],
    [`Gute-Nacht-Geschichte: ${story}`, 8000],
    ['Zudecken … schön kuschelig.', 2600, () => react('feed')],
    [`Licht aus. Gute Nacht, ${pet.name}! ♥`, 2400]
  ];
  stopMoment(true);
  ritualShow.active = true;
  let at = 0;
  for (const [text, ms, effect] of steps) {
    ritualShow.timers.push(setTimeout(() => { $('story').textContent = text; $('story').hidden = false; effect?.(); }, at));
    at += ms;
  }
  ritualShow.timers.push(setTimeout(endRitualShow, at));
  render();
}
function endRitualShow() {
  if (!ritualShow.active) return;
  ritualShow.active = false;
  for (const timer of ritualShow.timers) clearTimeout(timer);
  ritualShow.timers = [];
  $('story').hidden = true;
  flash(`Psst … ${pet.name} schläft.`);
  render();
}

// ---------- Games ----------
// Three games share one frame: 5 rounds, each success counts as a hit (2 coins).
// While a game runs it fills the screen; the keys belong to the game.

const GAMES = [['ball', 'i-ball', 'Ballfangen'], ['guess', 'i-arrows', 'Links oder rechts?'], ['simon', 'i-note', 'Melodie merken']];
const game = { active: false, kind: '', round: 0, hits: 0, note: '', timers: [],
  x: 0, dir: 1, speed: 0, raf: 0, last: 0, pause: 0,     // ball
  busy: false,                                           // guess and simon: waiting for an animation
  sequence: [], step: 0 };                               // simon
const later = (fn, ms) => game.timers.push(setTimeout(fn, ms));

function startGame(kind) {
  showMenu(false);
  stopMoment(true);
  for (const timer of game.timers) clearTimeout(timer);
  Object.assign(game, { active: true, kind, round: 1, hits: 0, note: '', timers: [], busy: false });
  scene.classList.add('playing');
  scene.dataset.game = kind;
  if (kind === 'ball') startBall();
  if (kind === 'guess') game.note = 'Wohin springt Evi? Links oder rechts';
  if (kind === 'simon') { game.sequence = [random(0, 2), random(0, 2)]; playSequence(); }
  renderCaption();
}

function gameKey(key) {
  if (game.kind === 'ball') { if (key === 'right') endGame(true); else catchBall(); }
  if (game.kind === 'guess') { if (key === 'mid') endGame(true); else guessSide(key); }
  if (game.kind === 'simon') simonPress(key);
}

function nextRound(hit) {
  if (hit) { game.hits++; beep('coin'); evi.reaction = 'play'; evi.reactionUntil = evi.tick + 6; } else beep('miss');
  if (game.round >= RULES.play.rounds) { later(() => endGame(false), 900); return false; }
  return true;   // the caller starts the next round (and counts it) after showing the result
}

// Ball: press when the ball is inside the dotted zone.
const ZONE = { from: 146, to: 174 };            // the dotted catch zone in scene units
const TRACK = { from: 40, to: 266 };            // left edge of the 14-unit ball
function startBall() {
  Object.assign(game, { x: TRACK.from, dir: 1, speed: 125, pause: 0.4 });
  game.last = performance.now();
  cancelAnimationFrame(game.raf);
  game.raf = requestAnimationFrame(gameFrame);
}
function gameFrame(ts) {
  if (!game.active) return;
  // Real elapsed time, so the ball keeps its speed even when the phone drops frames.
  let dt = Math.min(1, (ts - game.last) / 1000);
  game.last = ts;
  if (game.pause > 0) { const used = Math.min(game.pause, dt); game.pause -= used; dt -= used; }
  let travel = game.speed * dt;
  while (travel > 0) {
    const room = game.dir > 0 ? TRACK.to - game.x : game.x - TRACK.from;
    const step = Math.min(room, travel);
    game.x += game.dir * step;
    travel -= step;
    if (travel > 0 || step === room) game.dir = -game.dir;
  }
  $('game-ball').setAttribute('transform', `translate(${Math.round(game.x / 2) * 2} 28)`);
  game.raf = requestAnimationFrame(gameFrame);
}
function catchBall() {
  if (!game.active || game.pause > 0) return;
  const center = game.x + 7;
  const hit = center >= ZONE.from && center <= ZONE.to;
  game.note = hit ? 'Gefangen!' : 'Daneben!';
  game.pause = 0.7;
  if (nextRound(hit)) { game.speed *= 1.2; later(() => { game.round++; game.note = ''; renderCaption(); }, 700); }
  renderCaption();
}

// Links oder rechts: guess the side, then Evi jumps to one side at random.
function guessSide(key) {
  if (game.busy) return;
  game.busy = true;
  const side = Math.random() < 0.5 ? 'left' : 'right';
  $('evi-pos').setAttribute('transform', `translate(${side === 'left' ? -96 : 96} 0)`);
  react('play', 8);
  const hit = side === key;
  game.note = `${hit ? 'Richtig!' : 'Leider nicht.'} Evi ist nach ${side === 'left' ? 'links' : 'rechts'} gesprungen.`;
  const more = nextRound(hit);
  renderCaption();
  later(() => {
    $('evi-pos').removeAttribute('transform');
    game.busy = false;
    if (more) { game.round++; game.note = 'Wohin springt Evi? Links oder rechts'; renderCaption(); }
  }, 1300);
}

// Melodie merken: Evi plays a sequence on the three pads; repeat it with the three keys.
const PADS = ['left', 'mid', 'right'], PAD_TONES = [523, 659, 784];
function lightPad(index, ms) {
  const pad = $(`pad-${PADS[index]}`);
  pad.classList.add('lit');
  tone(PAD_TONES[index], ms);
  later(() => pad.classList.remove('lit'), ms);
}
function playSequence() {
  game.busy = true;
  game.step = 0;
  game.note = `Hör gut zu … (${game.sequence.length} Töne)`;
  renderCaption();
  game.sequence.forEach((pad, i) => later(() => lightPad(pad, 380), 700 + i * 560));
  later(() => { game.busy = false; game.note = 'Jetzt du!'; renderCaption(); }, 700 + game.sequence.length * 560);
}
function simonPress(key) {
  if (game.busy) return;
  const index = PADS.indexOf(key);
  lightPad(index, 220);
  if (index !== game.sequence[game.step]) {
    // A wrong note ends the game; finished rounds still count.
    game.busy = true;
    game.note = 'Oh, falscher Ton!';
    beep('miss');
    renderCaption();
    later(() => endGame(false), 1000);
    return;
  }
  game.step++;
  if (game.step < game.sequence.length) return;
  game.busy = true;
  game.note = 'Super!';
  renderCaption();
  if (nextRound(true)) {
    game.sequence.push(random(0, 2));
    later(() => { game.round++; playSequence(); }, 800);
  }
}

function endGame(aborted) {
  if (!game.active) return;
  game.active = false;
  cancelAnimationFrame(game.raf);
  for (const timer of game.timers) clearTimeout(timer);
  game.timers = [];
  $('evi-pos').removeAttribute('transform');
  for (const pad of document.querySelectorAll('.pad')) pad.classList.remove('lit');
  scene.classList.remove('playing');
  if (aborted) { flash('Spiel abgebrochen'); return; }
  const result = finishPlay(pet, game.hits, Date.now(), game.kind);
  pet = result.pet;
  if (result.wish) wishGranted();
  if (result.changed) {
    react('play');
    const done = { ball: 'gefangen', guess: 'richtig geraten', simon: 'Melodien geschafft' }[game.kind];
    flash(`${game.hits} von ${RULES.play.rounds} ${done} · +${result.coins} Münzen`, 4000);
  } else flash(reasonText(result.reason, 'play'));
  render();
  save();
}

// ---------- Evi's own moments ----------
// moments.js decides what and when; this shows it. Each moment is a small script (a generator):
// it changes the picture, then yields how many ticks (120 ms) to wait before it goes on.
// Evi's drawings stay as they are: she is only moved, lifted or mirrored as a whole.

const MOMENTS_KEY = 'kleines-zuhause.moments';
const QUIET_TICKS = 120;            // about 15 s without a new moment after a greeting or care
const STEP = 2;                     // scene units per tick while walking
let momentMemory = { lastSpecial: 0 };
try { momentMemory = { ...momentMemory, ...JSON.parse(localStorage.getItem(MOMENTS_KEY) || '{}') }; } catch { /* defaults */ }
function saveMoments() {
  try { localStorage.setItem(MOMENTS_KEY, JSON.stringify(momentMemory)); } catch { /* only this session */ }
}

// The owner's drawings for the moments (entwuerfe/evi/momente/, see Bildauftraege.md). Each one is
// used as soon as it exists in art.js; until then Evi shows her usual drawing for her mood.
const MOMENT_ART = { walk: 'laufen', window: 'fenster', think: 'denken', hum: 'singen', yawn: 'gaehnen',
  spin: 'drehen', hop: 'huepfen', wonder: 'staunen' };
/** The first of these drawings that exists for Evi's stage, or '' for her usual one. */
const drawn = (...names) => names.find(name => ART[evi.stage]?.[name]) ?? '';

// Where Evi is: x is the distance from her rug, flip mirrors her picture, lift raises it (a step, a hop).
const walker = { x: 0, flip: false, lift: 0 };
const moment = { gen: null, kind: '', wait: 0, last: '', next: Date.now() + nextDelay(), quietUntil: 0 };

function placeEvi() {
  const pos = $('evi-pos'), flipper = $('evi-flip');
  if (walker.x) pos.setAttribute('transform', `translate(${walker.x} 0)`);
  else pos.removeAttribute('transform');
  const mirror = walker.flip ? ` translate(${2 * FLOOR.x} 0) scale(-1 1)` : '';
  if (walker.lift || mirror) flipper.setAttribute('transform', `translate(0 ${-walker.lift})${mirror}`);
  else flipper.removeAttribute('transform');
  // The speech bubble stays above her head, but inside the screen.
  $('wish-bubble').style.left = `${Math.min(58, Math.max(4, 58 + walker.x / 3.2)).toFixed(1)}%`;
}

function* walkTo(x) {
  if (walker.x === x) return;
  const before = evi.artOverride;
  evi.artOverride = drawn(MOMENT_ART.walk) || before;
  walker.flip = x > walker.x;           // drawn facing left, tail on the right: mirrored when she walks right
  for (let steps = 0; walker.x !== x; steps++) {
    const left = x - walker.x;
    walker.x += Math.sign(left) * Math.min(Math.abs(left), STEP);
    walker.lift = Math.floor(steps / 2) % 2 ? 2 : 0;
    placeEvi();
    yield 1;
  }
  walker.lift = 0;
  evi.artOverride = before;
  placeEvi();
}
/** Back to her rug, facing you again. */
function* walkHome() {
  yield* walkTo(0);
  walker.flip = false;
  placeEvi();
}
function* hop(times = 1, height = 8) {
  const before = evi.artOverride;
  for (let i = 0; i < times; i++) {
    evi.artOverride = drawn(MOMENT_ART.hop) || before;
    for (const lift of [height / 2, height, height, height / 2, 0]) { walker.lift = lift; placeEvi(); yield 1; }
    evi.artOverride = before;
    yield 2;
  }
}

const THOUGHTS = {
  cookie: ['i-cookie', 'Denkt an Kekse'], ball: ['i-ball', 'Denkt ans Spielen'], heart: ['i-heart', 'Denkt an dich ♥'],
  note: ['i-note', 'Denkt an ein Lied'], food: ['i-food', 'Denkt ans Essen'], clean: ['i-clean', 'Denkt ans Baden'],
  sun: ['i-sun', 'Schaut aus dem Fenster'], moon: ['i-moon', 'Schaut aus dem Fenster']
};
function showThought(kind) {
  const cloud = $('thought');
  if (kind) $('thought-icon').setAttribute('href', `#${THOUGHTS[kind][0]}`);
  cloud.setAttribute('visibility', kind ? 'visible' : 'hidden');
}

// The butterfly: two pixel pictures (wings open and closed), 15 × 12 pixels, drawn twice as large.
const pixelPath = rows => rows.flatMap((row, y) => [...row].map((c, x) => (c === '#' ? `M${x} ${y}h1v1h-1z` : ''))).join('');
const BUTTERFLY = [pixelPath([
  '...#.......#...',
  '....#.....#....',
  '.####.#.#.####.',
  '######.#.######',
  '#...##.#.##...#',
  '#...##.#.##...#',
  '######.#.######',
  '.####..#..####.',
  '...##..#..##...',
  '..####.#.####..',
  '.#####.#.#####.',
  '..##...#...##..'
]), pixelPath([
  '...#.......#...',
  '....#.....#....',
  '.....#.#.#.....',
  '....##.#.##....',
  '...###.#.###...',
  '...#.#.#.#.#...',
  '...###.#.###...',
  '....##.#.##....',
  '.....#.#.#.....',
  '....##.#.##....',
  '....##.#.##....',
  '.......#.......'
])];
const BUTTERFLY_SIZE = { w: 30, h: 24 };
function placeButterfly(x, y, open) {
  const fly = $('butterfly');
  fly.setAttribute('transform', `translate(${Math.round(x)} ${Math.round(y)})`);
  $('butterfly-wings').setAttribute('d', BUTTERFLY[open ? 0 : 1]);
  fly.setAttribute('visibility', 'visible');
}
const hideButterfly = () => $('butterfly').setAttribute('visibility', 'hidden');
/** Where the butterfly sits on Evi's head (its top left corner), wherever she is. */
function headSpot() {
  const image = $('evi-image'), top = Number(image.getAttribute('y')) || 100, height = Number(image.getAttribute('height')) || 80;
  return { x: FLOOR.x + walker.x - BUTTERFLY_SIZE.w / 2, y: top + height * 0.17 - BUTTERFLY_SIZE.h + 2 };
}
function* flutterTo(fly, x, y, speed = 2.4) {
  while (Math.hypot(x - fly.x, y - fly.y) > speed) {
    const angle = Math.atan2(y - fly.y, x - fly.x);
    fly.x += Math.cos(angle) * speed;
    fly.y += Math.sin(angle) * speed;
    fly.t++;
    placeButterfly(fly.x, fly.y + Math.sin(fly.t / 3) * 4, Math.floor(fly.t / 2) % 2 === 0);
    yield 1;
  }
  Object.assign(fly, { x, y });
}

const SCRIPTS = {
  *walk() {
    const side = Math.random() < 0.5 ? -1 : 1;
    flash('Ein kleiner Spaziergang …', 9000);
    yield* walkTo(side * random(30, side < 0 ? 66 : 56));    // stays in the room
    yield 10;
    yield* hop(1, 5);
    yield 12;
    yield* walkHome();
  },
  *window() {
    flash(THOUGHTS.sun[1], 12000);
    yield* walkTo(-66);
    walker.flip = false;
    evi.artOverride = drawn(MOMENT_ART.window);
    placeEvi();
    yield 6;
    showThought(new Date().getHours() < 17 ? 'sun' : 'moon');
    yield 45;
    showThought(null);
    evi.artOverride = '';
    yield* walkHome();
  },
  *think(icon) {
    flash(THOUGHTS[icon][1], 6000);
    evi.artOverride = drawn(MOMENT_ART.think);
    showThought(icon);
    yield 45;
    showThought(null);
    evi.artOverride = '';
  },
  *hum() {
    flash('Summt ein Lied ♪', 6000);
    evi.artOverride = drawn(MOMENT_ART.hum);
    scene.classList.add('humming');
    yield 45;
    scene.classList.remove('humming');
    evi.artOverride = '';
  },
  *yawn() {
    flash('Gäähn …', 3000);
    evi.artOverride = drawn(MOMENT_ART.yawn, 'muede');
    yield 22;
    evi.artOverride = '';
  },
  *spin() {
    flash('Dreht sich im Kreis', 4000);
    evi.artOverride = drawn(MOMENT_ART.spin);
    for (let i = 0; i < 10; i++) { walker.flip = !walker.flip; walker.lift = i % 2 ? 0 : 3; placeEvi(); yield 3; }
    Object.assign(walker, { flip: false, lift: 0 });
    evi.artOverride = '';
    placeEvi();
    say('Hihi!', 'i-heart', 2000);
    yield 16;
  },
  // Special: a butterfly flutters in through the window, circles Evi, rests on her head and
  // flies off to the right. She runs after it a little and hops.
  *butterfly() {
    beep('special');
    flash('Oh, ein Schmetterling!', 30000);
    const fly = { x: -20, y: 96, t: 0 };
    if (reducedMotion.matches) {
      const spot = headSpot();
      evi.artOverride = drawn(MOMENT_ART.wonder, 'gluecklich');
      placeButterfly(spot.x, spot.y, true);
      yield 60;
      hideButterfly();
      evi.artOverride = '';
      return;
    }
    let spot = headSpot();
    yield* flutterTo(fly, spot.x, spot.y - 24);
    evi.artOverride = drawn(MOMENT_ART.wonder, 'gluecklich');
    for (let i = 0; i < 48; i++, fly.t++) {
      spot = headSpot();
      placeButterfly(spot.x + Math.sin(i / 4) * 22, spot.y - 22 + Math.cos(i / 4) * 7, Math.floor(fly.t / 2) % 2 === 0);
      yield 1;
    }
    Object.assign(fly, { x: spot.x, y: spot.y - 22 });
    yield* flutterTo(fly, spot.x, spot.y, 1.5);
    for (let i = 0; i < 32; i++) { placeButterfly(spot.x, spot.y, Math.floor(i / 8) % 2 === 0); yield 1; }
    say('Hihi, das kitzelt!', 'i-heart', 2600);
    yield 10;
    evi.artOverride = 'gluecklich';
    // Off it goes, and Evi runs after it for a bit.
    const chase = walkTo(52);
    for (let running = true; fly.x < 340; fly.t++) {
      fly.x += 2.6;
      fly.y -= fly.y > 40 ? 1.2 : 0;
      placeButterfly(fly.x, fly.y + Math.sin(fly.t / 3) * 4, Math.floor(fly.t / 2) % 2 === 0);
      if (running && fly.t % 2) running = !chase.next().done;
      yield 1;
    }
    hideButterfly();
    yield* chase;
    yield* hop(2, 9);
    evi.artOverride = '';
    flash('Tschüss, Schmetterling!', 3000);
    yield 14;
    yield* walkHome();
  }
};

function startMoment(kind, arg) {
  Object.assign(moment, { kind, gen: SCRIPTS[kind](arg), wait: 0 });
}
/** Ends a moment's extras: thought, notes, butterfly, borrowed drawing. */
function clearMoment() {
  Object.assign(moment, { gen: null, kind: '', wait: 0 });
  showThought(null);
  scene.classList.remove('humming');
  hideButterfly();
  evi.artOverride = '';
  walker.lift = 0;
}
/** Stops a moment. Evi walks back to her rug, or is there at once (snap) for games, the ritual and sleep. */
function stopMoment(snap = false) {
  const away = walker.x !== 0 || walker.flip;
  clearMoment();
  if (snap) {
    Object.assign(walker, { x: 0, flip: false });
    placeEvi();
  } else if (away) Object.assign(moment, { kind: 'home', gen: walkHome() });
}

const momentAllowed = () => !game.active && !ritualShow.active && gameMenu.hidden && !document.querySelector('dialog[open]')
  && !evi.hatchPending && evi.tick >= evi.reactionUntil && evi.tick >= moment.quietUntil;
const RESTING = ['egg', 'dead', 'sleeping', 'sick', 'tummy'];

/** Called every tick: runs the current moment, or about once a second looks whether a new one is due. */
function runMoments() {
  if (moment.gen) {
    if (moment.wait > 0) { moment.wait--; return; }
    const step = moment.gen.next();
    if (step.done) { clearMoment(); placeEvi(); } else moment.wait = Math.max(0, (step.value ?? 1) - 1);
    return;
  }
  if (evi.tick % 8 || !momentAllowed()) return;
  const now = Date.now(), { stage, mood } = describePet(pet, now);
  if (!momentsFor(stage) || RESTING.includes(mood) || isWoken(pet, now)) return;
  const special = dueSpecial(pet.bornAt, now, momentMemory.lastSpecial);
  if (special) {
    momentMemory.lastSpecial = now;
    saveMoments();
    moment.next = Math.max(moment.next, now + MOMENTS.every[0]);
    startMoment(special.kind);
    return;
  }
  if (now < moment.next) return;
  moment.next = now + nextDelay();
  const choice = chooseMoment({ mood, tired: isTired(pet, now), still: reducedMotion.matches, last: moment.last });
  if (!choice) return;
  moment.last = choice.kind;
  startMoment(choice.kind, choice.icon);
}

/** Evi is glad you are here: she hops and says hello. Not while she sleeps or feels ill; for now only as an adult. */
function greet(since) {
  const now = Date.now(), { stage, mood } = describePet(pet, now);
  if (!momentsFor(stage) || RESTING.includes(mood) || isWoken(pet, now) || game.active || ritualShow.active) return;
  const { text, big } = greeting(now, since);
  stopMoment(true);
  react(big ? 'cheer' : 'greet', big ? 20 : 12);
  say(text, 'i-heart', 3500);
  moment.quietUntil = evi.tick + QUIET_TICKS;
}

// ---------- Keys ----------

/** Every key press counts as playing with Evi: woken at night, she stays up 5 more minutes. */
function touch() {
  const now = Date.now();
  if (!isWoken(pet, now)) return;
  pet = stayAwake(pet, now);
  save();
}

$('next-button').addEventListener('click', () => {
  touch();
  if (ritualShow.active) { endRitualShow(); return; }
  if (game.active) { gameKey('left'); return; }
  if (gameMenu.hidden) { showMenu(true); return; }
  selected = (selected + 1) % gameMenu.children.length;
  renderSelection();
});
confirmButton.addEventListener('click', () => {
  touch();
  if (ritualShow.active) { endRitualShow(); return; }
  if (game.active) { gameKey('mid'); return; }
  if (gameMenu.hidden) { showMenu(true); return; }
  const action = gameMenu.children[selected].dataset.option;
  showMenu(false);
  performAction(action);
});
$('back-button').addEventListener('click', () => {
  touch();
  if (ritualShow.active) { endRitualShow(); return; }
  if (game.active) { gameKey('right'); return; }
  // In the game choice, "back" returns to the main menu.
  if (!gameMenu.hidden && menuMode === 'games') { showMenu(true); return; }
  showMenu(false);
});

$('name-form').addEventListener('submit', event => {
  event.preventDefault();
  if (!nameInput.value.trim()) {
    nameInput.setCustomValidity('Bitte gib einen Namen ein.');
    nameInput.reportValidity();
    return;
  }
  pet = renamePet(pet, nameInput.value);
  $('name-dialog').close();
  render();
  save();
});
nameInput.addEventListener('input', () => nameInput.setCustomValidity(''));
for (const button of document.querySelectorAll('[data-close]')) button.addEventListener('click', () => button.closest('dialog').close());

document.addEventListener('visibilitychange', () => {
  if (document.visibilityState !== 'visible') {
    endGame(true);
    endRitualShow();
    stopMoment(true);
    lastSeen = Date.now();
    tick();
    return;
  }
  tick();
  if (lastSeen && Date.now() - lastSeen >= MOMENTS.greet.after) greet(lastSeen);
});
window.addEventListener('pagehide', tick);
window.addEventListener('pageshow', tick);
// Keep another tab in this browser in sync, without overwriting a newer state.
window.addEventListener('storage', event => {
  if (event.key !== SAVE_KEY || !event.newValue) return;
  try {
    const incoming = restorePet(JSON.parse(event.newValue), Date.now());
    if (incoming && JSON.parse(event.newValue).updatedAt >= pet.updatedAt) { pet = incoming; invalidSave = false; render(); }
  } catch { /* Leave the running session intact. */ }
});
setInterval(() => { if (document.visibilityState === 'visible') tick(); }, 10000);
setInterval(eviTick, TICK_MS);
render();
save();
if (missedNote) flash(missedNote, 8000);
if (lastSeen && document.visibilityState === 'visible') setTimeout(() => greet(lastSeen), 700);
if ('serviceWorker' in navigator) {
  navigator.serviceWorker.register('./sw.js').catch(() => {
    // Gameplay still works online; offline availability is not guaranteed.
  });
}
