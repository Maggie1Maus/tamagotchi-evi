// Pure game rules: no browser, display, storage, timer or hardware dependencies.
// The adapter supplies the current time in milliseconds and one action command.
// Local time of day (sleep from 23 to 7 o'clock) is read with the standard Date object.
const MINUTE = 60000, HOUR = 60 * MINUTE, DAY = 24 * HOUR;

export const RULES = Object.freeze({
  version: 2,
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
  actions: {
    feed: { satiety: 40, mood: 3, cleanliness: -2 },
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
// One key per night (the evening's date), so the light bonus is given once per night.
const nightKey = t => { const d = new Date(t - RULES.sleep.to * HOUR); return `${d.getFullYear()}-${d.getMonth() + 1}-${d.getDate()}`; };

export function createPet(now, { name = DEFAULT_NAME, customName = false, coins = 0, generation = 1,
  eggDuration = RULES.eggDuration } = {}) {
  return { version: RULES.version, name, customName, generation, bornAt: now, hatchAt: now + eggDuration,
    updatedAt: now, satiety: 80, mood: 80, cleanliness: 100, careCount: 0, coins,
    sick: false, neglectMs: 0, sickMs: 0, lightOff: false, lightBonusNight: null,
    dead: false, diedAt: null, causeOfDeath: null };
}

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
export const isAsleep = (pet, t = pet.updatedAt) => !['egg', 'dead'].includes(stageAt(pet, t)) && isNight(t);

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
  if (value.version !== RULES.version || typeof value.customName !== 'boolean' ||
      !Number.isSafeInteger(value.generation) || value.generation < 1 || !finite(value.hatchAt) ||
      !Number.isSafeInteger(value.coins) || value.coins < 0 || typeof value.sick !== 'boolean' ||
      !finite(value.neglectMs) || !finite(value.sickMs) || typeof value.lightOff !== 'boolean' ||
      (value.lightBonusNight !== null && typeof value.lightBonusNight !== 'string') ||
      typeof value.dead !== 'boolean' || (value.diedAt !== null && !finite(value.diedAt)) ||
      ![undefined, null, 'neglect', 'sickness', 'age'].includes(value.causeOfDeath)) {
    return null;
  }
  const keys = Object.keys(createPet(0));
  const pet = Object.fromEntries(keys.map(key => [key, value[key]]));
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
      const asleep = isNight(t);
      if (!asleep) next.lightOff = false;
      const rate = RULES.decay[stage];
      const needs = asleep ? RULES.sleep.needs : 1;
      const moodSleep = asleep ? (next.lightOff ? RULES.sleep.moodDark : RULES.sleep.moodLight) : 1;
      next.satiety = clamp(next.satiety - rate.satiety * needs * hours);
      next.cleanliness = clamp(next.cleanliness - rate.cleanliness * needs * hours);
      next.mood = clamp(next.mood - rate.mood * moodSleep * (next.sick ? RULES.sickMoodFactor : 1) * hours);
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
  if (action === 'light') return isNight(now) ? null : 'day';
  if (isNight(now)) return 'asleep';
  if (action === 'play' && pet.sick) return 'sick';
  if (action === 'medicine' && !pet.sick) return 'healthy';
  if (action === 'feed' && pet.satiety >= 99.5) return 'full';
  if (action === 'clean' && pet.cleanliness >= 99.5) return 'full';
  if (action === 'play' && pet.mood >= 99.5) return 'full';
  return null;
}

function applyEffect(pet, effect) {
  for (const key of stats) pet[key] = clamp(pet[key] + (effect[key] ?? 0));
}

/** Care commands: feed, clean, medicine, light, newEgg. Playing goes through finishPlay. */
export function careForPet(pet, action, now) {
  const next = advancePet(pet, now);
  const known = ['feed', 'clean', 'medicine', 'light', 'newEgg'];
  if (!known.includes(action)) return { pet: next, changed: false, reason: 'unknown' };
  const reason = blockedReason(next, action, now);
  if (reason) return { pet: next, changed: false, reason };
  if (action === 'newEgg') {
    return { pet: createPet(now, { name: next.name, customName: next.customName, coins: next.coins,
      generation: next.generation + 1 }), changed: true };
  }
  if (action === 'light') {
    next.lightOff = !next.lightOff;
    let bonus = false;
    if (next.lightOff && next.lightBonusNight !== nightKey(now)) {
      next.lightBonusNight = nightKey(now);
      next.mood = clamp(next.mood + RULES.sleep.lightBonus);
      bonus = true;
    }
    return { pet: next, changed: true, bonus };
  }
  applyEffect(next, RULES.actions[action]);
  if (action === 'medicine') Object.assign(next, { sick: false, sickMs: 0, neglectMs: 0 });
  next.careCount++;
  return { pet: next, changed: true };
}

/** Result of one round of the ball game: hits out of RULES.play.rounds. */
export function finishPlay(pet, hits, now) {
  const next = advancePet(pet, now);
  const reason = blockedReason(next, 'play', now);
  if (reason && reason !== 'full') return { pet: next, changed: false, reason, coins: 0 };
  const caught = Math.max(0, Math.min(RULES.play.rounds, Math.floor(hits) || 0));
  applyEffect(next, { mood: RULES.play.mood + caught * RULES.play.moodPerHit,
    satiety: RULES.play.satiety, cleanliness: RULES.play.cleanliness });
  const coins = caught * RULES.play.coinsPerHit;
  next.coins += coins;
  next.careCount++;
  return { pet: next, changed: true, coins };
}

export function renamePet(pet, name) {
  const trimmed = String(name).trim().slice(0, 20);
  return trimmed ? { ...pet, name: trimmed, customName: true } : { ...pet };
}

const STAGE_NAMES = { egg: 'Ei', baby: 'Baby', teen: 'Teenager', adult: 'Erwachsen', senior: 'Oma', dead: 'Gestorben' };
export const stageName = stage => STAGE_NAMES[stage];

/** Stage and the most pressing state: dead, egg, sleeping, sick, hungry, dirty, sad or happy. */
export function describePet(pet, now = pet.updatedAt) {
  const stage = stageAt(pet, now);
  if (stage === 'dead' || stage === 'egg') return { stage, mood: stage };
  if (isNight(now)) return { stage, mood: 'sleeping' };
  if (pet.sick) return { stage, mood: 'sick' };
  const lowest = Math.min(...stats.map(key => pet[key]));
  if (lowest <= RULES.low) {
    if (pet.satiety === lowest) return { stage, mood: 'hungry' };
    if (pet.cleanliness === lowest) return { stage, mood: 'dirty' };
    return { stage, mood: 'sad' };
  }
  return { stage, mood: 'happy' };
}
