// The sky in Evi's window follows the real sun in Germany (version 0.11). Sunrise and sunset are
// worked out for every day from the date alone, with the usual sunrise equation: no internet,
// no table to keep up to date, summer time included (the times are moments, not clock times).
// No browser needed, so this also works later on the handheld with its clock.

const DAY = 86400000;
const rad = Math.PI / 180;

export const SKY = Object.freeze({
  // The middle of Germany (near Niederdorla, Thuringia). From Aachen to Görlitz the sun sets
  // up to about 20 minutes earlier or later than here.
  lat: 51.16, lon: 10.45,
  // The sun is "down" at 0.833° below the horizon; dusk lasts until 6° below (civil twilight).
  sunDown: -0.833, duskDown: -6
});

/**
 * Sun times of one day in Germany (the UTC date of t), as milliseconds:
 * { dawn, rise, set, dusk }. Accurate to about a minute.
 */
export function sunTimes(t, lat = SKY.lat, lon = SKY.lon) {
  const noon = Math.floor(t / DAY) * DAY + DAY / 2;
  const n = Math.round(noon / DAY + 2440587.5 - 2451545.0);       // days since 1 January 2000, noon
  const meanNoon = n - lon / 360;
  const M = (357.5291 + 0.98560028 * meanNoon) % 360;             // mean anomaly
  const C = 1.9148 * Math.sin(M * rad) + 0.02 * Math.sin(2 * M * rad) + 0.0003 * Math.sin(3 * M * rad);
  const L = (M + C + 180 + 102.9372) % 360;                       // ecliptic longitude
  const transit = 2451545.0 + meanNoon + 0.0053 * Math.sin(M * rad) - 0.0069 * Math.sin(2 * L * rad);
  const decl = Math.asin(Math.sin(L * rad) * Math.sin(23.4397 * rad));
  const at = altitude => {
    const cos = (Math.sin(altitude * rad) - Math.sin(lat * rad) * Math.sin(decl)) / (Math.cos(lat * rad) * Math.cos(decl));
    const hours = Math.acos(Math.min(1, Math.max(-1, cos))) / rad / 360;
    return [transit - hours, transit + hours].map(jd => Math.round((jd - 2440587.5) * DAY));
  };
  const [rise, set] = at(SKY.sunDown), [dawn, dusk] = at(SKY.duskDown);
  return { dawn, rise, set, dusk };
}

/** The sky at time t: 'day' (sun up), 'dusk' (twilight, morning or evening) or 'night'. */
export function skyAt(t) {
  // Look at yesterday, today and tomorrow, so the answer is right in every time zone of the phone.
  let sky = 'night';
  for (const offset of [-DAY, 0, DAY]) {
    const { dawn, rise, set, dusk } = sunTimes(t + offset);
    if (t >= rise && t < set) return 'day';
    if (t >= dawn && t < dusk) sky = 'dusk';
  }
  return sky;
}
