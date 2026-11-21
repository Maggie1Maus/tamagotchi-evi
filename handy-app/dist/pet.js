// Pure game rules: no browser, display, storage, timer or hardware dependencies.
// The adapter supplies the current time in milliseconds and one action command.
// Local time of day (sleep, meals, nap, wishes) is read with the standard Date object.
const MINUTE = 60000, HOUR = 60 * MINUTE, DAY = 24 * HOUR;

export const RULES = Object.freeze({
  version: 3,
  // Life cycle, measured from the moment the egg hatches.
  eggDuration: 30 * MINUTE,
  // Durations after hatching; after the senior phase the pet dies of old age.
  stages: { baby: 1 * DAY, teen: 2 * DAY, adult: 5 * DAY, senior: 2 * DAY },
  // Hourly decay while awake. Chosen so a pet needs care every 1–2 h (baby),
  // 2–3 h (teen) or 3–4 h (adult, senior): the time a full value needs to reach "low" (30).
  decay: {
    baby: { satiety: 45, mood: 35, cleanliness: 30 },
    teen: { satiety: 28, mood: 22, cleanliness: 18 },
    adult: { satiety: 20, mood: 15, cleanliness: 12 },
    senior: { satiety: 18, mood: 15, cleanliness: 12 }
  },
  sleep: { from: 23, to: 7, needs: 0.15, moodDark: 0.1, moodLight: 0.6, lightBonus: 10 },
  // Evi's day (version 0.9): tired before bed and right after waking, an evening ritual,
  // meal times, snacks, a nap for the baby and a few wishes a day.
  tired: { evening: 22, morningMinutes: 30 },
  ritual: { from: 22, mood: 10 },
  // Woken at night, Evi stays up while someone plays with her and falls asleep again after 5 quiet minutes.
  wake: { awake: 5 * MINUTE },
  nap: { stage: 'baby', from: 12, to: 13 },
  meals: { windows: [['breakfast', 7, 9], ['lunch', 11, 14], ['dinner', 17, 20]], satiety: 10, mood: 5 },
  snack: { perDay: 3, tummyAche: 2 * HOUR },
  wishes: { slots: [[8, 11], [11, 14], [14, 18], [18, 22]], duration: HOUR, mood: 10, coins: 3,
    kinds: ['ball', 'guess', 'simon', 'snack', 'clean', 'feed'] },
  actions: {
    feed: { satiety: 40, mood: 3, cleanliness: -2 },
    snack: { satiety: 10, mood: 10, cleanliness: 0 },
    clean: { satiety: 0, mood: 3, cleanliness: 60 },
    medicine: { satiety: 0, mood: -5, cleanliness: 0 }
  },
  // A full game (+50 mood) carries a baby for about 1.5 hours, an adult for over 3.
  play: { mood: 15, moodPerHit: 7, satiety: -8, cleanliness: -5, coinsPerHit: 2, rounds: 5 },
  // Neglect: time with at least one need at zero.
  sickAfter: 4 * HOUR, sickMoodFactor: 1.5,
  deathAfterNeglect: 30 * HOUR, deathAfterSick: 24 * HOUR,
  low: 30
});

const stats = ['satiety', 'mood', 'cleanliness'];
const DEFAULT_NAME = 'Evi';
const clamp = value => Math.min(100, Math.max(0, value));
const hourOf = t => new Date(t).getHours();
export const isNight = t => { const h = hourOf(t); return h >= RULES.sleep.from || h < RULES.sleep.to; };
const dayKey = t => { const d = new Date(t); return `${d.getFullYear()}-${d.getMonth() + 1}-${d.getDate()}`; };
// One key per night (the evening's date), so the light bonus and the ritual count once per night.
const nightKey = t => dayKey(t - RULES.sleep.to * HOUR);

export function createPet(now, { name = DEFAULT_NAME, customName = false, coins = 0, generation = 1,
  eggDuration = RULES.eggDuration } = {}) {
  return { version: RULES.version, name, customName, generation, bornAt: now, hatchAt: now + eggDuration,
    updatedAt: now, satiety: 80, mood: 80, cleanliness: 100, careCount: 0, coins,
    sick: false, neglectMs: 0, sickMs: 0, lightOff: false, lightBonusNight: null,
    dead: false, diedAt: null, causeOfDeath: null, ...DAY_DEFAULTS };
}
// Fields added in version 0.9; older saves get these values.
const DAY_DEFAULTS = Object.freeze({ ritualNight: null, lastMeal: null, snackDay: null, snackCount: 0,
  tummyUntil: 0, wishDone: 0, awakeUntil: 0 });

const LIFESPAN = Object.values(RULES.stages).reduce((sum, ms) => sum + ms, 0);
/** Life stage at time t: egg, baby, teen, adult, senior or dead. */
export function stageAt(pet, t = pet.updatedAt) {
  if (pet.dead) return 'dead';
  if (t < pet.hatchAt) return 'egg';
  let age = t - pet.hatchAt;
  for (const stage of ['baby', 'teen', 'adult']) {
    if (age < RULES.stages[stage]) return stage;
    age -= RULES.stages[stage];
  }
  return 'senior';
}

/** Bedtime: 23–7, or from 22 o'clock after the evening ritual. */
const bedtime = (pet, t) => isNight(t) || (hourOf(t) >= RULES.ritual.from && pet.ritualNight === nightKey(t));
/** Woken up at bedtime and not yet asleep again. */
export const isWoken = (pet, t = pet.updatedAt) => bedtime(pet, t) && t < pet.awakeUntil;
/** Why Evi sleeps at time t: 'night' (bedtime, unless she was woken), 'nap' or null. */
export function sleepKind(pet, t = pet.updatedAt) {
  const stage = stageAt(pet, t);
  if (stage === 'egg' || stage === 'dead') return null;
  if (bedtime(pet, t)) return t < pet.awakeUntil ? null : 'night';
  const h = hourOf(t);
  if (stage === RULES.nap.stage && h >= RULES.nap.from && h < RULES.nap.to) return 'nap';
  return null;
}
/** Any key press while woken keeps Evi up for another 5 minutes. */
export function stayAwake(pet, now) {
  const next = advancePet(pet, now);
  if (isWoken(next, now)) next.awakeUntil = Math.max(next.awakeUntil, now + RULES.wake.awake);
  return next;
}
export const isAsleep = (pet, t = pet.updatedAt) => sleepKind(pet, t) !== null;
const awake = (pet, t) => !['egg', 'dead'].includes(stageAt(pet, t)) && !sleepKind(pet, t);
/** Woke up rested: the evening ritual was done the night before. */
const ritualLastNight = (pet, t) => pet.ritualNight !== null && pet.ritualNight === dayKey(t - 12 * HOUR);
const earlyMorning = t => { const d = new Date(t); return d.getHours() === RULES.sleep.to && d.getMinutes() < RULES.tired.morningMinutes; };
/** Tired: the hour before bedtime, when woken at night, and the first half hour of the morning unless she slept well. */
export function isTired(pet, t = pet.updatedAt) {
  if (!awake(pet, t)) return false;
  return isNight(t) || hourOf(t) === RULES.tired.evening || (earlyMorning(t) && !ritualLastNight(pet, t));
}
export const isRested = (pet, t = pet.updatedAt) => awake(pet, t) && earlyMorning(t) && ritualLastNight(pet, t);
export const hasTummyAche = (pet, t = pet.updatedAt) => !pet.dead && t < pet.tummyUntil;

/** The meal that fits time t ('breakfast', 'lunch', 'dinner') or null. */
export function mealAt(t) {
  const h = hourOf(t);
  return RULES.meals.windows.find(([, from, to]) => h >= from && h < to)?.[0] ?? null;
}
const mealKey = t => `${dayKey(t)}-${mealAt(t)}`;
/** The meal Evi can still have now, or null when there is none or she already ate it. */
export const mealDue = (pet, t = pet.updatedAt) => (mealAt(t) && pet.lastMeal !== mealKey(t) ? mealAt(t) : null);

// Wishes: up to one per time slot, at a pseudo-random but fixed minute, so a closed app and
// an open app agree. They depend on the day and on the pet, and can be fulfilled once.
function hash(text) {
  let h = 2166136261;
  for (let i = 0; i < text.length; i++) { h ^= text.charCodeAt(i); h = Math.imul(h, 16777619); }
  h ^= h >>> 15; h = Math.imul(h, 2246822507); h ^= h >>> 13;
  return h >>> 0;
}
/** Today's wishes: [{ kind, start }], 3 or 4 of them. */
export function wishesOn(pet, t) {
  const d = new Date(t), key = dayKey(t), seed = `${pet.bornAt}:${key}`;
  const drop = hash(`${seed}:drop`);
  const dropped = drop & 1 ? (drop >>> 1) % RULES.wishes.slots.length : -1;
  return RULES.wishes.slots.flatMap(([from, to], i) => {
    if (i === dropped) return [];
    const h = hash(`${seed}:${i}`);
    // Each wish ends within its slot, so two wishes never overlap.
    const latest = (to - from) * 60 - RULES.wishes.duration / MINUTE;
    const start = new Date(d.getFullYear(), d.getMonth(), d.getDate(), from, h % (latest + 1)).getTime();
    return [{ kind: RULES.wishes.kinds[(h >>> 12) % RULES.wishes.kinds.length], start }];
  });
}
/** The wish Evi has at time t, or null. A wish lasts an hour; it pauses while she sleeps or feels ill. */
export function currentWish(pet, t = pet.updatedAt) {
  if (!awake(pet, t) || pet.sick || hasTummyAche(pet, t)) return null;
  const wish = wishesOn(pet, t).find(w => t >= w.start && t < w.start + RULES.wishes.duration);
  return wish && wish.start !== pet.wishDone ? { ...wish, until: wish.start + RULES.wishes.duration } : null;
}

const finite = (v, min = 0) => Number.isFinite(v) && v >= min;
export function restorePet(value, now) {
  if (!value || typeof value.name !== 'string' || !value.name.trim() || value.name.length > 20 ||
      (value.customName !== undefined && typeof value.customName !== 'boolean') ||
      !finite(value.bornAt) || !finite(value.updatedAt) || value.updatedAt < value.bornAt ||
      !Number.isSafeInteger(value.careCount) || value.careCount < 0 ||
      stats.some(key => !Number.isFinite(value[key]) || value[key] < 0 || value[key] > 100)) {
    return null;
  }
  const name = value.name.trim();
  if (value.version === 1) {
    // Saves from version 0.4/0.5: the pet existed before the life cycle and counts as an adult.
    const customName = value.customName ?? (name !== 'Momo');
    const migrated = { ...createPet(value.bornAt), name: customName ? name : DEFAULT_NAME, customName,
      hatchAt: Math.min(value.bornAt, value.updatedAt - RULES.stages.baby - RULES.stages.teen),
      updatedAt: value.updatedAt, careCount: value.careCount,
      satiety: value.satiety, mood: value.mood, cleanliness: value.cleanliness };
    return advancePet(migrated, now);
  }
  if (![2, RULES.version].includes(value.version) || typeof value.customName !== 'boolean' ||
      !Number.isSafeInteger(value.generation) || value.generation < 1 || !finite(value.hatchAt) ||
      !Number.isSafeInteger(value.coins) || value.coins < 0 || typeof value.sick !== 'boolean' ||
      !finite(value.neglectMs) || !finite(value.sickMs) || typeof value.lightOff !== 'boolean' ||
      (value.lightBonusNight !== null && typeof value.lightBonusNight !== 'string') ||
      typeof value.dead !== 'boolean' || (value.diedAt !== null && !finite(value.diedAt)) ||
      ![undefined, null, 'neglect', 'sickness', 'age'].includes(value.causeOfDeath)) {
    return null;
  }
  // Version 2 (0.6–0.8) has no day fields yet: Evi keeps everything and starts her new day routine.
  // Fields added later within version 3 start with their default value.
  const day = Object.fromEntries(Object.entries(DAY_DEFAULTS).map(([key, fallback]) =>
    [key, value.version === 2 ? fallback : value[key] ?? fallback]));
  const keyOrNull = v => v === null || typeof v === 'string';
  if (!keyOrNull(day.ritualNight) || !keyOrNull(day.lastMeal) || !keyOrNull(day.snackDay) ||
      !Number.isSafeInteger(day.snackCount) || day.snackCount < 0 || !finite(day.tummyUntil) || !finite(day.wishDone) ||
      !finite(day.awakeUntil)) {
    return null;
  }
  const keys = Object.keys(createPet(0));
  const pet = Object.fromEntries(keys.map(key => [key, key in DAY_DEFAULTS ? day[key] : value[key]]));
  pet.version = RULES.version;
  pet.causeOfDeath ??= pet.dead ? 'neglect' : null;
  pet.name = name;
  return advancePet(pet, now);
}

/** Applies the real time that passed since the last update, minute by minute. */
export function advancePet(pet, now) {
  if (pet.dead || !Number.isFinite(now) || now <= pet.updatedAt) return { ...pet };
  const next = { ...pet };
  let t = pet.updatedAt;
  while (t < now) {
    // Steps end on whole minutes so that one long update and several short ones agree.
    const end = Math.min(now, Math.floor(t / MINUTE) * MINUTE + MINUTE);
    const dt = end - t, hours = dt / HOUR, stage = stageAt(next, t);
    if (stage !== 'egg' && t >= next.hatchAt + LIFESPAN) {
      // Old age: the senior falls asleep peacefully at the end of her life.
      const diedAt = next.hatchAt + LIFESPAN;
      Object.assign(next, { dead: true, diedAt, updatedAt: diedAt, lightOff: false, causeOfDeath: 'age' });
      return next;
    }
    if (stage !== 'egg') {
      const asleep = sleepKind(next, t);
      // The light comes on by itself in the morning; a woken Evi falls asleep again in the same light.
      if (!bedtime(next, t)) next.lightOff = false;
      const rate = RULES.decay[stage];
      const needs = asleep ? RULES.sleep.needs : 1;
      // A nap is as restful as a dark night.
      const moodSleep = !asleep ? 1 : asleep === 'nap' || next.lightOff ? RULES.sleep.moodDark : RULES.sleep.moodLight;
      const unwell = next.sick || t < next.tummyUntil ? RULES.sickMoodFactor : 1;
      next.satiety = clamp(next.satiety - rate.satiety * needs * hours);
      next.cleanliness = clamp(next.cleanliness - rate.cleanliness * needs * hours);
      next.mood = clamp(next.mood - rate.mood * moodSleep * unwell * hours);
      const neglected = stats.some(key => next[key] <= 0);
      next.neglectMs = neglected ? next.neglectMs + dt : Math.max(0, next.neglectMs - dt);
      if (next.sick) next.sickMs += dt;
      if (!next.sick && next.neglectMs >= RULES.sickAfter) next.sick = true;
      if (next.neglectMs >= RULES.deathAfterNeglect || next.sickMs >= RULES.deathAfterSick) {
        Object.assign(next, { dead: true, diedAt: end, updatedAt: end, lightOff: false,
          causeOfDeath: next.sickMs >= RULES.deathAfterSick ? 'sickness' : 'neglect' });
        return next;
      }
    }
    t = end;
  }
  next.updatedAt = now;
  return next;
}

/** Why an action is not possible right now, or null when it is. */
export function blockedReason(pet, action, now = pet.updatedAt) {
  const stage = stageAt(pet, now);
  if (stage === 'dead') return action === 'newEgg' ? null : 'dead';
  if (action === 'newEgg') return 'alive';
  if (stage === 'egg') return 'egg';
  const asleep = sleepKind(pet, now);
  if (action === 'light') return asleep === 'night' ? null : 'day';
  if (action === 'wake') return asleep === 'night' ? null : 'awake';
  if (asleep === 'night') return 'asleep';
  if (asleep === 'nap') return 'nap';
  if (action === 'ritual' && hourOf(now) !== RULES.ritual.from) return 'early';
  if (action === 'ritual' && pet.ritualNight === nightKey(now)) return 'done';
  if (action === 'play' && pet.sick) return 'sick';
  if (action === 'medicine' && !pet.sick && !hasTummyAche(pet, now)) return 'healthy';
  if (action === 'snack' && hasTummyAche(pet, now)) return 'tummy';
  // A wish can always be fulfilled, even when the need is already met.
  if (currentWish(pet, now)?.kind === action) return null;
  if (action === 'feed' && pet.satiety >= 99.5) return 'full';
  if (action === 'clean' && pet.cleanliness >= 99.5) return 'full';
  if (action === 'play' && pet.mood >= 99.5) return 'full';
  return null;
}

/** Fulfils the wish if the action matches it: +10 mood, +3 coins. */
function grantWish(pet, wish, action) {
  if (!wish || wish.kind !== action) return false;
  pet.wishDone = wish.start;
  pet.mood = clamp(pet.mood + RULES.wishes.mood);
  pet.coins += RULES.wishes.coins;
  return true;
}

function applyEffect(pet, effect) {
  for (const key of stats) pet[key] = clamp(pet[key] + (effect[key] ?? 0));
}

/** Care commands: feed, snack, clean, medicine, light, ritual, wake, newEgg. Playing goes through finishPlay. */
export function careForPet(pet, action, now) {
  const next = advancePet(pet, now);
  const known = ['feed', 'snack', 'clean', 'medicine', 'light', 'ritual', 'wake', 'newEgg'];
  if (!known.includes(action)) return { pet: next, changed: false, reason: 'unknown' };
  const reason = blockedReason(next, action, now);
  if (reason) return { pet: next, changed: false, reason };
  if (action === 'newEgg') {
    return { pet: createPet(now, { name: next.name, customName: next.customName, coins: next.coins,
      generation: next.generation + 1 }), changed: true };
  }
  const lightOut = () => {
    next.lightOff = true;
    if (next.lightBonusNight === nightKey(now)) return false;
    next.lightBonusNight = nightKey(now);
    next.mood = clamp(next.mood + RULES.sleep.lightBonus);
    return true;
  };
  if (action === 'light') {
    if (next.lightOff) { next.lightOff = false; return { pet: next, changed: true, bonus: false }; }
    return { pet: next, changed: true, bonus: lightOut() };
  }
  if (action === 'wake') {
    next.awakeUntil = now + RULES.wake.awake;
    return { pet: next, changed: true };
  }
  if (action === 'ritual') {
    // Brush teeth, a story, tucked in, light off: Evi falls asleep at once and wakes rested.
    next.ritualNight = nightKey(now);
    next.mood = clamp(next.mood + RULES.ritual.mood);
    next.careCount++;
    return { pet: next, changed: true, bonus: lightOut() };
  }
  const wish = currentWish(next, now);
  const result = { pet: next, changed: true };
  applyEffect(next, RULES.actions[action]);
  if (action === 'feed' && mealDue(next, now)) {
    result.meal = mealAt(now);
    next.lastMeal = mealKey(now);
    applyEffect(next, { satiety: RULES.meals.satiety, mood: RULES.meals.mood });
  }
  if (action === 'snack') {
    if (next.snackDay !== dayKey(now)) Object.assign(next, { snackDay: dayKey(now), snackCount: 0 });
    next.snackCount++;
    if (next.snackCount > RULES.snack.perDay) {
      next.tummyUntil = now + RULES.snack.tummyAche;
      result.tummy = true;
    }
  }
  if (action === 'medicine') {
    if (next.sick) Object.assign(next, { sick: false, sickMs: 0, neglectMs: 0 });
    next.tummyUntil = 0;
  }
  result.wish = grantWish(next, wish, action);
  next.careCount++;
  if (isWoken(next, now)) next.awakeUntil = Math.max(next.awakeUntil, now + RULES.wake.awake);
  return result;
}

/** Result of one game ('ball', 'guess' or 'simon'): hits out of RULES.play.rounds. */
export function finishPlay(pet, hits, now, game = 'ball') {
  const next = advancePet(pet, now);
  const reason = blockedReason(next, 'play', now);
  if (reason && reason !== 'full') return { pet: next, changed: false, reason, coins: 0 };
  const wish = currentWish(next, now);
  const caught = Math.max(0, Math.min(RULES.play.rounds, Math.floor(hits) || 0));
  applyEffect(next, { mood: RULES.play.mood + caught * RULES.play.moodPerHit,
    satiety: RULES.play.satiety, cleanliness: RULES.play.cleanliness });
  const coins = caught * RULES.play.coinsPerHit;
  next.coins += coins;
  next.careCount++;
  if (isWoken(next, now)) next.awakeUntil = Math.max(next.awakeUntil, now + RULES.wake.awake);
  return { pet: next, changed: true, coins, wish: grantWish(next, wish, game) };
}

export function renamePet(pet, name) {
  const trimmed = String(name).trim().slice(0, 20);
  return trimmed ? { ...pet, name: trimmed, customName: true } : { ...pet };
}

const STAGE_NAMES = { egg: 'Ei', baby: 'Baby', teen: 'Teenager', adult: 'Erwachsen', senior: 'Oma', dead: 'Gestorben' };
export const stageName = stage => STAGE_NAMES[stage];

/** Stage and the most pressing state: dead, egg, sleeping, sick, tummy, hungry, dirty, sad or happy. */
export function describePet(pet, now = pet.updatedAt) {
  const stage = stageAt(pet, now);
  if (stage === 'dead' || stage === 'egg') return { stage, mood: stage };
  if (sleepKind(pet, now)) return { stage, mood: 'sleeping' };
  if (pet.sick) return { stage, mood: 'sick' };
  if (hasTummyAche(pet, now)) return { stage, mood: 'tummy' };
  const lowest = Math.min(...stats.map(key => pet[key]));
  if (lowest <= RULES.low) {
    if (pet.satiety === lowest) return { stage, mood: 'hungry' };
    if (pet.cleanliness === lowest) return { stage, mood: 'dirty' };
    return { stage, mood: 'sad' };
  }
  return { stage, mood: 'happy' };
}
