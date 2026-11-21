import test from 'node:test';
import assert from 'node:assert/strict';
import { RULES, createPet, restorePet, advancePet, careForPet, finishPlay, renamePet, describePet, stageAt,
  isNight as isNightAt, sleepKind, isTired, isRested, hasTummyAche, mealAt, mealDue, wishesOn, currentWish
} from '../dist/pet.js';

const MIN = 60000, H = 60 * MIN, DAY = 24 * H;
// Fixed local times, so day and night are predictable in every time zone.
const at = (day, hour, minute = 0) => new Date(2026, 10, day, hour, minute).getTime();
const morning = at(20, 9);
const adultAt = (now, values = {}) => {
  const pet = createPet(now - 5 * DAY);
  return { ...pet, updatedAt: now, satiety: 80, mood: 80, cleanliness: 80, ...values };
};
const close = (a, b) => Math.abs(a - b) < 1e-9;

test('Life cycle: egg 30 min, baby 1 day, teen 2 days, adult 5 days, grandma 2 days', () => {
  const pet = createPet(morning);
  const hatch = morning + 30 * MIN;
  assert.equal(stageAt(pet, morning + 29 * MIN), 'egg');
  assert.equal(stageAt(pet, hatch + MIN), 'baby');
  assert.equal(stageAt(pet, hatch + DAY + MIN), 'teen');
  assert.equal(stageAt(pet, hatch + 3 * DAY + MIN), 'adult');
  assert.equal(stageAt(pet, hatch + 8 * DAY + MIN), 'senior');
});

test('Grandma falls asleep for good after her two days, even when well cared for', () => {
  const pet = { ...createPet(morning), satiety: 100, mood: 100, cleanliness: 100 };
  const hatch = pet.hatchAt;
  // Keep her fed and clean every hour until shortly before the end of her life.
  let cared = pet;
  for (let t = hatch + H; t < hatch + 10 * DAY - H; t += H) {
    cared = advancePet(cared, t);
    if (!isNightAt(t)) {
      for (const action of ['feed', 'feed', 'clean']) cared = careForPet(cared, action, t).pet;
      if (((t - hatch) / H) % 2 === 0) cared = finishPlay(cared, 5, t).pet;
    } else if (!cared.lightOff) cared = careForPet(cared, 'light', t).pet;
  }
  assert.equal(cared.dead, false);
  assert.equal(stageAt(cared, cared.updatedAt), 'senior');
  const end = advancePet(cared, hatch + 10 * DAY + H);
  assert.equal(end.dead, true);
  assert.equal(end.causeOfDeath, 'age');
  assert.equal(end.diedAt, hatch + 10 * DAY);
});

test('A one-off short egg can hatch after one minute', () => {
  const pet = createPet(morning, { eggDuration: MIN });
  assert.equal(stageAt(pet, morning + 59 * 1000), 'egg');
  assert.equal(stageAt(pet, morning + MIN), 'baby');
});

test('The egg hatches without the player and needs nothing while inside', () => {
  const pet = createPet(morning);
  const inEgg = advancePet(pet, morning + 29 * MIN);
  assert.equal(inEgg.satiety, pet.satiety);
  const hatched = advancePet(pet, morning + 2 * H);
  assert.equal(stageAt(hatched, hatched.updatedAt), 'baby');
  assert.ok(hatched.satiety < pet.satiety);
});

test('Care rhythm: baby about every 1.5 h, teen 2.5 h, adult 3.5 h', () => {
  for (const [stage, expected] of [['baby', 1.56], ['teen', 2.5], ['adult', 3.5], ['senior', 3.89]]) {
    const hours = (100 - RULES.low) / RULES.decay[stage].satiety;
    assert.ok(Math.abs(hours - expected) < 0.05, `${stage}: ${hours}`);
  }
});

test('One long update equals many short ones', () => {
  const pet = { ...createPet(morning), satiety: 100, mood: 100, cleanliness: 100 };
  const start = pet.hatchAt;
  const long = advancePet(pet, start + 5 * H);
  const short = [1, 2, 3, 4, 5].reduce((p, i) => advancePet(p, start + i * H), pet);
  for (const key of ['satiety', 'mood', 'cleanliness']) assert.ok(close(long[key], short[key]), key);
});

test('Sleep from 23 to 7: needs barely drop, light off gives a bonus once per night', () => {
  const night = at(20, 23, 30);
  const pet = adultAt(night, { mood: 50 });
  assert.equal(describePet(pet, night).mood, 'sleeping');
  assert.equal(careForPet(pet, 'feed', night).reason, 'asleep');
  const off = careForPet(pet, 'light', night);
  assert.equal(off.bonus, true);
  assert.equal(off.pet.mood, 50 + RULES.sleep.lightBonus);
  const onAgain = careForPet(off.pet, 'light', night + MIN);
  assert.equal(careForPet(onAgain.pet, 'light', night + 2 * MIN).bonus, false);
  const woke = advancePet(off.pet, at(21, 7, 1));
  assert.equal(woke.lightOff, false);
  // 7.5 h of sleep cost an adult about 23 points instead of 150 while awake.
  assert.ok(off.pet.satiety - woke.satiety < 25);
  assert.equal(careForPet(woke, 'light', at(21, 12)).reason, 'day');
});

test('Neglect makes the pet sick after 4 h; medicine helps; long neglect is fatal', () => {
  const pet = adultAt(morning, { satiety: 0, mood: 0, cleanliness: 0 });
  assert.equal(advancePet(pet, morning + 3 * H).sick, false);
  const sick = advancePet(pet, morning + 4 * H + MIN);
  assert.equal(sick.sick, true);
  assert.equal(describePet(sick, sick.updatedAt).mood, 'sick');
  assert.equal(finishPlay(sick, 3, sick.updatedAt).reason, 'sick');
  const cured = careForPet(sick, 'medicine', sick.updatedAt);
  assert.equal(cured.pet.sick, false);
  assert.equal(careForPet(cured.pet, 'medicine', cured.pet.updatedAt).reason, 'healthy');
  const dead = advancePet(pet, morning + 2 * DAY);
  assert.equal(dead.dead, true);
  assert.equal(dead.causeOfDeath, 'sickness');
  assert.equal(stageAt(dead), 'dead');
  assert.equal(careForPet(dead, 'feed', dead.updatedAt + H).reason, 'dead');
});

test('A new egg after death keeps coins and counts the generation', () => {
  const dead = { ...adultAt(morning, { coins: 14 }), dead: true, diedAt: morning };
  const next = careForPet(dead, 'newEgg', morning + MIN);
  assert.equal(next.changed, true);
  assert.equal(stageAt(next.pet, next.pet.updatedAt), 'egg');
  assert.equal(next.pet.coins, 14);
  assert.equal(next.pet.generation, 2);
  assert.equal(careForPet(adultAt(morning), 'newEgg', morning).reason, 'alive');
});

test('The ball game pays 2 coins per catch and lifts the mood', () => {
  const pet = adultAt(at(21, 12), { mood: 40 });
  const result = finishPlay(pet, 4, pet.updatedAt);
  assert.equal(result.coins, 8);
  assert.equal(result.pet.coins, 8);
  assert.equal(result.pet.mood, 40 + RULES.play.mood + 4 * RULES.play.moodPerHit);
  assert.equal(finishPlay(pet, 99, pet.updatedAt).coins, RULES.play.rounds * RULES.play.coinsPerHit);
});

test('Actions respect bounds and report why they are not possible', () => {
  let pet = adultAt(at(21, 12));
  for (let i = 0; i < 10; i++) pet = careForPet(pet, 'feed', pet.updatedAt).pet;
  assert.equal(pet.satiety, 100);
  assert.equal(careForPet(pet, 'feed', pet.updatedAt).reason, 'full');
  assert.equal(careForPet(createPet(morning), 'feed', morning).reason, 'egg');
  assert.equal(careForPet(pet, 'toString', pet.updatedAt).changed, false);
});

test('A clock moving backwards cannot restore needs', () => {
  const pet = adultAt(morning);
  assert.deepEqual(advancePet(pet, morning - H), pet);
  assert.deepEqual(advancePet(pet, NaN), pet);
});

test('Saves survive a round trip; invalid saves are rejected', () => {
  const pet = finishPlay(adultAt(at(21, 12)), 2, at(21, 12)).pet;
  assert.deepEqual(restorePet(JSON.parse(JSON.stringify(pet)), pet.updatedAt), pet);
  for (const bad of [null, {}, { ...pet, coins: -1 }, { ...pet, version: 99 }, { ...pet, satiety: NaN },
    { ...pet, updatedAt: pet.bornAt - 1 }, { ...pet, sick: 'yes' }]) {
    assert.equal(restorePet(bad, pet.updatedAt), null);
  }
});

test('Saves from version 0.4/0.5 become an adult with the same name and needs', () => {
  const oldSave = { version: 1, name: 'Momo', bornAt: morning - DAY, updatedAt: morning,
    satiety: 42, mood: 61, cleanliness: 57, careCount: 12 };
  const migrated = restorePet(oldSave, morning);
  assert.equal(migrated.name, 'Evi');
  assert.equal(stageAt(migrated, morning), 'adult');
  assert.equal(migrated.satiety, 42);
  assert.equal(migrated.careCount, 12);
  assert.equal(restorePet({ ...oldSave, name: 'Pünktchen' }, morning).name, 'Pünktchen');
});

test('Names persist, trim whitespace and stay bounded', () => {
  const renamed = renamePet(createPet(morning), '  Pünktchen  ');
  assert.equal(restorePet(renamed, morning).name, 'Pünktchen');
  assert.equal(renamePet(renamed, ' ').name, 'Pünktchen');
  assert.equal(renamePet(renamed, 'x'.repeat(100)).name.length, 20);
});

// ---------- Evi's day (version 0.9) ----------

test('Tired in the hour before bed and for half an hour after waking at 7', () => {
  const pet = adultAt(at(21, 21));
  assert.equal(isTired(pet, at(21, 21, 59)), false);
  assert.equal(isTired(pet, at(21, 22, 15)), true);
  assert.equal(isTired(pet, at(22, 7, 0)), true);
  assert.equal(isTired(pet, at(22, 7, 29)), true);
  assert.equal(isTired(pet, at(22, 7, 30)), false);
  assert.equal(isTired(pet, at(22, 6, 50)), false, 'still asleep');
  assert.equal(describePet(pet, at(22, 7, 10)).mood, 'happy');
});

test('Evening ritual from 22 o\'clock: Evi falls asleep at once and wakes up rested', () => {
  const pet = adultAt(at(21, 21, 50), { mood: 50 });
  assert.equal(careForPet(pet, 'ritual', at(21, 21, 50)).reason, 'early');
  const night = careForPet(pet, 'ritual', at(21, 22, 10));
  assert.equal(night.changed, true);
  assert.equal(night.bonus, true);
  assert.ok(Math.abs(night.pet.mood - (50 - 20 * 15 / 60 + RULES.ritual.mood + RULES.sleep.lightBonus)) < 1e-9);
  assert.equal(night.pet.lightOff, true);
  assert.equal(sleepKind(night.pet, at(21, 22, 20)), 'night');
  assert.equal(describePet(night.pet, at(21, 22, 20)).mood, 'sleeping');
  assert.equal(careForPet(night.pet, 'feed', at(21, 22, 20)).reason, 'asleep');
  assert.equal(careForPet(night.pet, 'ritual', at(21, 22, 20)).reason, 'asleep');
  // Sleeping from 22 o'clock saves needs compared with staying up.
  const slept = advancePet(night.pet, at(21, 23));
  const awake = advancePet(advancePet(pet, at(21, 22, 10)), at(21, 23));
  assert.ok(slept.satiety > awake.satiety + 10);
  // The light bonus is not given twice in the same night.
  const on = careForPet(slept, 'light', at(21, 23, 30)).pet;
  assert.equal(careForPet(on, 'light', at(21, 23, 31)).bonus, false);
  const morning7 = advancePet(night.pet, at(22, 7, 5));
  assert.equal(isRested(morning7, at(22, 7, 5)), true);
  assert.equal(isTired(morning7, at(22, 7, 5)), false);
  // Without the ritual she is tired; the ritual only counts for the following morning.
  assert.equal(isTired(advancePet(pet, at(22, 7, 5)), at(22, 7, 5)), true);
  assert.equal(isTired(advancePet(night.pet, at(23, 7, 5)), at(23, 7, 5)), true);
});

test('Meals: breakfast, lunch and dinner fill more than feeding in between', () => {
  assert.deepEqual([at(21, 6, 59), at(21, 7), at(21, 9), at(21, 11), at(21, 13, 59), at(21, 17), at(21, 20)].map(mealAt),
    [null, 'breakfast', null, 'lunch', 'lunch', 'dinner', null]);
  const pet = adultAt(at(21, 8), { satiety: 20, mood: 50 });
  const breakfast = careForPet(pet, 'feed', at(21, 8));
  assert.equal(breakfast.meal, 'breakfast');
  assert.equal(breakfast.pet.satiety, 20 + 40 + RULES.meals.satiety);
  assert.equal(breakfast.pet.mood, 50 + 3 + RULES.meals.mood);
  assert.equal(mealDue(breakfast.pet, at(21, 8, 30)), null);
  const second = careForPet({ ...breakfast.pet, satiety: 20 }, 'feed', at(21, 8));
  assert.equal(second.meal, undefined);
  assert.equal(second.pet.satiety, 60);
  assert.equal(careForPet(adultAt(at(21, 10), { satiety: 20 }), 'feed', at(21, 10)).pet.satiety, 60, 'no meal at 10');
  assert.equal(mealDue(breakfast.pet, at(21, 12)), 'lunch');
});

test('Snacks: three a day are fine, the fourth gives a two-hour tummy ache', () => {
  let pet = adultAt(at(21, 10), { mood: 40, satiety: 40 });
  for (let i = 0; i < 3; i++) {
    const r = careForPet(pet, 'snack', at(21, 10));
    assert.equal(r.tummy, undefined);
    pet = r.pet;
  }
  assert.equal(pet.mood, 70);
  assert.equal(pet.satiety, 70);
  const fourth = careForPet(pet, 'snack', at(21, 10));
  assert.equal(fourth.tummy, true);
  assert.equal(describePet(fourth.pet, at(21, 11)).mood, 'tummy');
  assert.equal(careForPet(fourth.pet, 'snack', at(21, 11)).reason, 'tummy');
  // The mood drops faster with a tummy ache, but she does not get sick or die of it.
  const ache = advancePet(fourth.pet, at(21, 11));
  const fine = advancePet({ ...fourth.pet, tummyUntil: 0 }, at(21, 11));
  assert.ok(ache.mood < fine.mood - 5);
  const later = advancePet(fourth.pet, at(21, 12, 1));
  assert.equal(hasTummyAche(later, later.updatedAt), false);
  assert.equal(later.sick, false);
  // Medicine helps at once; it is not needed when she is fine.
  const cured = careForPet(fourth.pet, 'medicine', at(21, 10, 30));
  assert.equal(cured.changed, true);
  assert.equal(hasTummyAche(cured.pet, at(21, 10, 30)), false);
  assert.equal(careForPet(cured.pet, 'medicine', at(21, 10, 31)).reason, 'healthy');
  // Another snack the same day is again too much; the next day counts anew.
  assert.equal(careForPet(cured.pet, 'snack', at(21, 15)).tummy, true);
  assert.equal(careForPet(advancePet(cured.pet, at(22, 10)), 'snack', at(22, 10)).tummy, undefined);
});

test('The baby naps from 12 to 13; older Evis do not', () => {
  const baby = { ...createPet(at(21, 10)), satiety: 80, mood: 80, cleanliness: 80 };
  assert.equal(stageAt(baby, at(21, 12, 30)), 'baby');
  assert.equal(sleepKind(baby, at(21, 11, 59)), null);
  assert.equal(sleepKind(baby, at(21, 12, 30)), 'nap');
  assert.equal(sleepKind(baby, at(21, 13)), null);
  const napping = advancePet(baby, at(21, 12, 30));
  assert.equal(describePet(napping, at(21, 12, 30)).mood, 'sleeping');
  assert.equal(careForPet(napping, 'feed', at(21, 12, 30)).reason, 'nap');
  assert.equal(careForPet(napping, 'light', at(21, 12, 30)).reason, 'day');
  // One hour of napping costs far less than one hour awake.
  const fed = { ...baby, satiety: 100 };
  const napStart = advancePet(fed, at(21, 12)), awakeStart = advancePet(fed, at(21, 11));
  const napCost = napStart.satiety - advancePet(napStart, at(21, 13)).satiety;
  const awakeCost = awakeStart.satiety - advancePet(awakeStart, at(21, 12)).satiety;
  assert.ok(Math.abs(napCost - RULES.decay.baby.satiety * RULES.sleep.needs) < 1e-9, String(napCost));
  assert.ok(Math.abs(awakeCost - RULES.decay.baby.satiety) < 1e-9, String(awakeCost));
  assert.equal(sleepKind(adultAt(at(21, 12, 30)), at(21, 12, 30)), null);
});

test('Wishes: three or four a day, fixed in advance and within waking hours', () => {
  const pet = adultAt(at(1, 9));
  for (let day = 1; day <= 30; day++) {
    const wishes = wishesOn(pet, at(day, 9));
    assert.ok(wishes.length === 3 || wishes.length === 4, `day ${day}: ${wishes.length}`);
    for (const w of wishes) {
      const h = new Date(w.start).getHours();
      assert.ok(h >= 8 && w.start + RULES.wishes.duration <= at(day, 22), `day ${day}: ${h}`);
      assert.ok(RULES.wishes.kinds.includes(w.kind));
    }
    assert.deepEqual(wishesOn(pet, at(day, 20)), wishes);
  }
  // Another Evi has other wishes.
  const other = { ...pet, bornAt: pet.bornAt + 1 };
  assert.notDeepEqual([1, 2, 3, 4, 5].map(d => wishesOn(other, at(d, 9))), [1, 2, 3, 4, 5].map(d => wishesOn(pet, at(d, 9))));
});

test('A fulfilled wish gives +10 mood and 3 coins; a missed wish costs nothing', () => {
  const kindsSeen = new Set();
  for (let day = 1; day <= 12; day++) {
    const base = adultAt(at(day, 7));
    for (const w of wishesOn(base, at(day, 9))) {
      const t = w.start + 5 * MIN;
      const pet = { ...base, updatedAt: t, mood: 30, satiety: 50, cleanliness: 50 };
      const wish = currentWish(pet, t);
      assert.equal(wish.kind, w.kind);
      assert.equal(currentWish(pet, w.start + RULES.wishes.duration), null, 'lasts one hour');
      const used = { ...pet, wishDone: w.start };
      const play = p => (['ball', 'guess', 'simon'].includes(w.kind) ? finishPlay(p, 0, t, w.kind) : careForPet(p, w.kind, t));
      const granted = play(pet), plain = play(used);
      assert.equal(granted.wish, true);
      assert.equal(plain.wish, false);
      assert.ok(Math.abs(granted.pet.mood - plain.pet.mood - RULES.wishes.mood) < 1e-9);
      assert.equal(granted.pet.coins - plain.pet.coins, RULES.wishes.coins);
      assert.equal(currentWish(granted.pet, t + MIN), null, 'fulfilled only once');
      kindsSeen.add(w.kind);
      // A different action does not fulfil it.
      const other = w.kind === 'clean' ? 'snack' : 'clean';
      assert.equal(careForPet(pet, other, t).wish, false);
      // Missing it changes nothing.
      const missed = advancePet(pet, w.start + 2 * H), ignored = advancePet(used, w.start + 2 * H);
      assert.equal(missed.mood, ignored.mood);
    }
  }
  assert.equal(kindsSeen.size, RULES.wishes.kinds.length, [...kindsSeen].join());
});

test('A wish can be fulfilled even when the need is already met, but never while ill or asleep', () => {
  for (let day = 1; day <= 40; day++) {
    const base = adultAt(at(day, 7));
    const w = wishesOn(base, at(day, 9)).find(x => x.kind === 'clean');
    if (!w) continue;
    const t = w.start + MIN;
    const clean = { ...base, updatedAt: t, cleanliness: 100 };
    assert.equal(careForPet(clean, 'clean', t).wish, true);
    assert.equal(currentWish({ ...clean, sick: true }, t), null);
    assert.equal(currentWish({ ...clean, tummyUntil: t + H }, t), null);
    return;
  }
  assert.fail('no clean wish found');
});

test('Saves from version 0.8 keep Evi and add the new day fields', () => {
  const t = at(21, 12);
  const { ritualNight, lastMeal, snackDay, snackCount, tummyUntil, wishDone, ...v8 } = adultAt(t, { coins: 23, mood: 44 });
  const oldSave = JSON.parse(JSON.stringify({ ...v8, version: 2, name: 'Evi' }));
  const restored = restorePet(oldSave, t);
  assert.equal(restored.version, RULES.version);
  assert.equal(restored.coins, 23);
  assert.equal(restored.mood, 44);
  assert.equal(restored.hatchAt, oldSave.hatchAt);
  assert.equal(restored.snackCount, 0);
  assert.equal(restored.ritualNight, null);
  assert.deepEqual(restorePet(JSON.parse(JSON.stringify(restored)), t), restored);
  assert.equal(restorePet({ ...restored, snackCount: -1 }, t), null);
  assert.equal(restorePet({ ...restored, ritualNight: 5 }, t), null);
});
