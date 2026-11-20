import test from 'node:test';
import assert from 'node:assert/strict';
import { RULES, createPet, restorePet, advancePet, careForPet, finishPlay, renamePet, describePet, stageAt,
  isNight as isNightAt } from '../dist/pet.js';

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
