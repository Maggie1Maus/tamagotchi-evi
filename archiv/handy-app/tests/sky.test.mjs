import test from 'node:test';
import assert from 'node:assert/strict';
import { sunTimes, skyAt } from '../dist/sky.js';

const MIN = 60000;
// Clock times in Germany written as moments (CET = UTC+1, CEST = UTC+2), so every time zone agrees.
const de = (day, time, summer = false) => Date.parse(`${day}T${time}:00${summer ? '+02:00' : '+01:00'}`);
const near = (actual, expected, text) => assert.ok(Math.abs(actual - expected) <= 3 * MIN,
  `${text}: ${new Date(actual).toISOString()} statt ${new Date(expected).toISOString()}`);

test('Sun: sunrise and sunset in German cities within 3 minutes of the published times', () => {
  const cities = [
    ['Berlin', 52.52, 13.405, '2026-12-21', '08:15', '15:54', false],
    ['Berlin', 52.52, 13.405, '2026-06-21', '04:43', '21:33', true],
    ['Frankfurt', 50.11, 8.68, '2026-12-21', '08:23', '16:24', false],
    ['Frankfurt', 50.11, 8.68, '2026-06-21', '05:15', '21:39', true],
    ['München', 48.14, 11.58, '2026-12-21', '08:01', '16:22', false],
    ['Hamburg', 53.55, 9.99, '2026-06-21', '04:52', '21:52', true]
  ];
  for (const [city, lat, lon, day, rise, set, summer] of cities) {
    const sun = sunTimes(de(day, '12:00', summer), lat, lon);
    near(sun.rise, de(day, rise, summer), `${city} ${day} Aufgang`);
    near(sun.set, de(day, set, summer), `${city} ${day} Untergang`);
    assert.ok(sun.dawn < sun.rise && sun.set < sun.dusk);
  }
});

test('Sky: light by day, dotted at dusk, dark at night, in winter and in summer', () => {
  // 23 November 2026, middle of Germany: sunrise 7:47, sunset 16:21, dark from about 17:00.
  assert.equal(skyAt(de('2026-11-23', '07:00')), 'night');
  assert.equal(skyAt(de('2026-11-23', '07:30')), 'dusk');
  assert.equal(skyAt(de('2026-11-23', '12:00')), 'day');
  assert.equal(skyAt(de('2026-11-23', '16:40')), 'dusk');
  assert.equal(skyAt(de('2026-11-23', '17:30')), 'night');
  assert.equal(skyAt(de('2026-11-23', '23:59')), 'night');
  // Midsummer: still light at 21 o'clock, dusk at 22, dark only around midnight.
  assert.equal(skyAt(de('2026-06-21', '21:00', true)), 'day');
  assert.equal(skyAt(de('2026-06-21', '22:00', true)), 'dusk');
  assert.equal(skyAt(de('2026-06-22', '00:30', true)), 'night');
  assert.equal(skyAt(de('2026-06-22', '04:30', true)), 'dusk');
});

test('Sky: works every day of the year, sunset moves smoothly', () => {
  let previous = null;
  for (let d = 0; d < 366; d++) {
    const noon = Date.parse('2026-01-01T11:00:00Z') + d * 86400000;
    const { rise, set } = sunTimes(noon);
    assert.ok(rise < noon && noon < set);
    assert.ok(set - rise > 7.5 * 60 * MIN && set - rise < 17 * 60 * MIN);
    if (previous) assert.ok(Math.abs((set - noon) - previous) < 4 * MIN);
    previous = set - noon;
    assert.equal(skyAt(noon), 'day');
  }
});
