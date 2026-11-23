import test from 'node:test';
import assert from 'node:assert/strict';
import { MOMENTS, momentsFor, nextDelay, chooseMoment, specialsOn, dueSpecial, greeting } from '../dist/moments.js';

const MIN = 60000, H = 60 * MIN;
// Fixed local times, so the hours are the same in every time zone.
const at = (day, hour, minute = 0) => new Date(2026, 10, day, hour, minute).getTime();
const seed = at(20, 8);
const fixed = values => { let i = 0; return () => values[i++ % values.length]; };

test('Moments: for now only the grown-up Evi does things by herself', () => {
  assert.equal(momentsFor('adult'), true);
  for (const stage of ['egg', 'baby', 'teen', 'senior', 'dead']) assert.equal(momentsFor(stage), false);
});

test('Moments: a small one every 5 to 10 minutes', () => {
  assert.equal(nextDelay(0), 5 * MIN);
  assert.ok(nextDelay(0.999) < 10 * MIN && nextDelay(0.999) > 9.9 * MIN);
  for (let r = 0; r < 1; r += 0.05) assert.ok(nextDelay(r) >= 5 * MIN && nextDelay(r) < 10 * MIN);
});

test('Moments: a hungry, sad or dirty Evi thinks of what she misses; asleep or ill she does nothing', () => {
  assert.deepEqual(chooseMoment({ mood: 'hungry' }), { kind: 'think', icon: 'food' });
  assert.deepEqual(chooseMoment({ mood: 'sad' }), { kind: 'think', icon: 'ball' });
  assert.deepEqual(chooseMoment({ mood: 'dirty' }), { kind: 'think', icon: 'clean' });
  for (const mood of ['sleeping', 'sick', 'tummy', 'egg', 'dead']) assert.equal(chooseMoment({ mood }), null);
  assert.deepEqual(chooseMoment({ mood: 'happy', tired: true }), { kind: 'yawn' });
});

test('Moments: all kinds come up, never twice in a row, and reduced motion keeps her in place', () => {
  const seen = new Set();
  for (let r = 0; r < 1; r += 0.01) {
    for (const last of ['', ...MOMENTS.small]) {
      const choice = chooseMoment({ mood: 'happy', last }, fixed([r, (r * 7) % 1]));
      assert.ok(MOMENTS.small.includes(choice.kind));
      assert.notEqual(choice.kind, last);
      if (choice.kind === 'think') assert.ok(MOMENTS.thoughts.includes(choice.icon));
      seen.add(choice.kind);
      const still = chooseMoment({ mood: 'happy', still: true, last }, fixed([r, (r * 3) % 1]));
      assert.ok(['think', 'hum', 'yawn'].includes(still.kind));
    }
  }
  assert.deepEqual([...seen].sort(), [...MOMENTS.small].sort());
});

test('Special moments: once or twice a day between 9 and 21 o\'clock, the same for each day and Evi', () => {
  const counts = new Set();
  for (let day = 1; day <= 60; day++) {
    const t = new Date(2026, 10, day, 12).getTime();
    const list = specialsOn(seed, t);
    assert.deepEqual(list, specialsOn(seed, t + 3 * H));
    counts.add(list.length);
    for (const s of list) {
      const d = new Date(s.at);
      assert.equal(d.getDate(), new Date(t).getDate());
      assert.ok(d.getHours() >= 9 && d.getHours() < 21, d.toString());
      assert.ok(MOMENTS.specials.kinds.includes(s.kind));
    }
    if (list.length === 2) assert.ok(list[0].at < list[1].at && new Date(list[0].at).getHours() < 15);
  }
  assert.deepEqual([...counts].sort(), [1, 2]);
});

test('Special moments wait until the app is open, come once, and only until 22 o\'clock', () => {
  // A day with two special moments.
  let day = 1;
  while (specialsOn(seed, at(day, 12)).length !== 2) day++;
  const [first, second] = specialsOn(seed, at(day, 12));
  assert.equal(dueSpecial(seed, first.at - MIN, 0), null);
  assert.deepEqual(dueSpecial(seed, first.at, 0), first);
  assert.deepEqual(dueSpecial(seed, first.at + 30 * MIN, 0), first);              // it waited for you
  assert.equal(dueSpecial(seed, first.at + 31 * MIN, first.at + 30 * MIN), null);    // shown, so done
  assert.deepEqual(dueSpecial(seed, second.at + MIN, first.at + 30 * MIN), second);
  // Missed both: only one comes, the later one.
  assert.deepEqual(dueSpecial(seed, at(day, 21, 30), 0), second);
  assert.equal(dueSpecial(seed, at(day, 21, 31), at(day, 21, 30)), null);
  assert.equal(dueSpecial(seed, at(day, 22, 5), 0), null);
  // Yesterday's moment does not count for today.
  assert.equal(dueSpecial(seed, at(day + 1, 8, 59), second.at), null);
});

test('Greeting: good morning, good evening, and extra glad after a long time', () => {
  assert.deepEqual(greeting(at(21, 8), at(20, 22)), { text: 'Guten Morgen!', big: true });
  assert.deepEqual(greeting(at(21, 10, 30), at(21, 6, 50)), { text: 'Guten Morgen!', big: true });
  assert.deepEqual(greeting(at(21, 15), at(21, 10)), { text: 'Da bist du ja endlich!', big: true });
  assert.deepEqual(greeting(at(21, 19), at(21, 17, 30)), { text: 'Guten Abend!', big: false });
  for (const r of [0, 0.5, 0.99]) {
    const { text, big } = greeting(at(21, 15), at(21, 14, 50), r);
    assert.ok(['Da bist du ja!', 'Hallo!', 'Schön, dass du da bist!'].includes(text));
    assert.equal(big, false);
  }
  // Seen this morning already: no second "Guten Morgen".
  assert.notEqual(greeting(at(21, 9, 30), at(21, 9)).text, 'Guten Morgen!');
});
