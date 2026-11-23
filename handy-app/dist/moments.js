// Evi's own moments (version 0.10): little things she does by herself while the app is open.
// Only what and when, no browser and no drawing: app.js shows them. Moments never change
// the needs, the coins or anything else in the game rules (pet.js).
import { hash, dayKey } from './pet.js';

const MINUTE = 60000, HOUR = 60 * MINUTE;
const hourOf = t => new Date(t).getHours();

export const MOMENTS = Object.freeze({
  // Since version 0.12 Evi does things by herself in every life stage, with her own drawings
  // for baby, teenager, grown-up and grandma (wish of the owner, 24 November 2026). Not as an egg.
  stages: ['baby', 'teen', 'adult', 'senior'],
  // A small moment every 5 to 10 minutes while the app is open and Evi is awake.
  every: [5 * MINUTE, 10 * MINUTE],
  small: ['walk', 'window', 'think', 'hum', 'yawn', 'spin'],
  // With "reduce motion" in the system settings: nothing that walks or turns.
  still: ['think', 'hum', 'yawn'],
  thoughts: ['cookie', 'ball', 'heart', 'note', 'food'],
  // Evi greets you when you open the app, or come back after at least 3 minutes.
  greet: { after: 3 * MINUTE, longAway: 4 * HOUR, morningUntil: 11, eveningFrom: 18 },
  // Something special once or twice a day, between 9 and 21 o'clock, with a sound. It waits
  // for you until 22 o'clock: it starts as soon as the app is open after its time.
  // A butterfly or a bird by day; when it is dark outside (sky.js), a shooting star instead.
  specials: { from: 9, to: 21, until: 22, kinds: ['butterfly', 'bird'], dark: 'star' },
  // Evi's mood of the day (version 0.11), the same all day for each Evi. It makes some moments
  // and thoughts more likely (weights; everything else counts 1).
  tempers: {
    verspielt: { text: 'Heute bin ich verspielt!', icon: 'ball', kinds: { spin: 3, walk: 2 }, thoughts: { ball: 3 } },
    verschmust: { text: 'Heute bin ich verschmust!', icon: 'heart', kinds: { think: 2 }, thoughts: { heart: 4 } },
    verfressen: { text: 'Heute bin ich verfressen!', icon: 'cookie', kinds: { think: 3 }, thoughts: { cookie: 3, food: 3 } },
    vertraeumt: { text: 'Heute bin ich verträumt!', icon: 'note', kinds: { window: 3, think: 2, hum: 2 }, thoughts: { note: 2 } }
  }
});

/** Whether Evi does things by herself (and greets you) in this life stage. */
export const momentsFor = stage => MOMENTS.stages.includes(stage);

/** Milliseconds until the next small moment; rand is a number from 0 to below 1. */
export function nextDelay(rand = Math.random()) {
  const [min, max] = MOMENTS.every;
  return Math.round(min + rand * (max - min));
}

/** Evi's mood of the day: 'verspielt', 'verschmust', 'verfressen' or 'vertraeumt'. */
export function temperOn(seed, t) {
  const names = Object.keys(MOMENTS.tempers);
  return names[hash(`${seed}:${dayKey(t)}:temper`) % names.length];
}

/**
 * The small moment that fits now, or null. mood comes from describePet; tired: the hour before
 * bed or early morning; still: reduced motion; last: the previous kind, so it does not repeat;
 * temper: the mood of the day. A hungry, sad or dirty Evi only thinks about what she is missing.
 */
export function chooseMoment({ mood, tired = false, still = false, last = '', temper = '' }, rand = Math.random) {
  const weights = MOMENTS.tempers[temper] ?? {};
  // Picks one entry; an entry with weight 3 is three times as likely as one with weight 1.
  const pick = (list, weight = {}) => {
    const total = list.reduce((sum, k) => sum + (weight[k] ?? 1), 0);
    let r = Math.floor(rand() * total) % total;
    return list.find(k => (r -= weight[k] ?? 1) < 0);
  };
  const worry = { hungry: 'food', sad: 'ball', dirty: 'clean' }[mood];
  if (worry) return { kind: 'think', icon: worry };
  if (mood !== 'happy') return null;
  if (tired) return { kind: 'yawn' };
  const kinds = (still ? MOMENTS.still : MOMENTS.small).filter(k => k !== last);
  const kind = pick(kinds, weights.kinds);
  return kind === 'think' ? { kind, icon: pick(MOMENTS.thoughts, weights.thoughts) } : { kind };
}

/** Which special moment to show: the planned visitor by day, a shooting star in the dark. */
export const specialFor = (special, sky) => (sky === 'night' ? MOMENTS.specials.dark : special.kind);

/** Today's special moments: [{ kind, at }], one or two, fixed for each day and each Evi. */
export function specialsOn(seed, t) {
  const d = new Date(t), key = `${seed}:${dayKey(t)}:special`;
  const { from, to, kinds } = MOMENTS.specials;
  // Two a day: one before and one after the middle of the time span.
  const middle = Math.round((from + to) / 2);
  const spans = hash(key) & 1 ? [[from, middle], [middle, to]] : [[from, to]];
  return spans.map(([start, end], i) => {
    const h = hash(`${key}:${i}`);
    const at = new Date(d.getFullYear(), d.getMonth(), d.getDate(), start, h % ((end - start) * 60)).getTime();
    return { kind: kinds[(h >>> 12) % kinds.length], at };
  });
}

/**
 * The special moment that is due at time t and has not been shown yet, or null. last is the
 * time the previous special moment was shown. If both of today's were missed, only one comes.
 */
export function dueSpecial(seed, t, last = 0) {
  if (hourOf(t) >= MOMENTS.specials.until) return null;
  const due = specialsOn(seed, t).filter(s => s.at <= t && s.at > last);
  return due.at(-1) ?? null;
}

/** What Evi says when you come back: { text, big }. big means extra happy (more hops). */
export function greeting(now, lastSeen, rand = Math.random()) {
  const { longAway, morningUntil, eveningFrom } = MOMENTS.greet;
  const h = hourOf(now), away = now - lastSeen;
  // The first visit since she got up this morning.
  const firstToday = dayKey(lastSeen) !== dayKey(now) || hourOf(lastSeen) < 7;
  if (firstToday && h >= 7 && h < morningUntil) return { text: 'Guten Morgen!', big: true };
  if (away >= longAway) return { text: 'Da bist du ja endlich!', big: true };
  if (h >= eveningFrom && away >= HOUR) return { text: 'Guten Abend!', big: false };
  const texts = ['Da bist du ja!', 'Hallo!', 'Schön, dass du da bist!'];
  return { text: texts[Math.floor(rand * texts.length) % texts.length], big: false };
}
