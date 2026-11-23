// Datum und Uhrzeit ohne Bibliothek: Millisekunden seit 1.1.1970 (UTC) und deutsche
// Ortszeit mit Sommerzeit (letzter Sonntag im März bis letzter Sonntag im Oktober,
// Wechsel jeweils um 1 Uhr UTC). Entspricht dem, was die Handy-App mit Date rechnet.
#pragma once
#include <stdint.h>

typedef int64_t ms_t;
const ms_t SEKUNDE = 1000, MINUTE = 60 * SEKUNDE, STUNDE = 60 * MINUTE, TAG = 24 * STUNDE;

static inline int64_t teileAb(int64_t a, int64_t b) { return a >= 0 ? a / b : -((-a + b - 1) / b); }

// Tage seit 1.1.1970 für ein Datum (Algorithmus von Howard Hinnant)
static inline int64_t tageAusDatum(int y, int m, int d) {
  y -= m <= 2;
  const int64_t era = (y >= 0 ? y : y - 399) / 400;
  const unsigned yoe = (unsigned)(y - era * 400);
  const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  return era * 146097 + (int64_t)doe - 719468;
}

static inline void datumAusTagen(int64_t z, int& y, int& m, int& d) {
  z += 719468;
  const int64_t era = (z >= 0 ? z : z - 146096) / 146097;
  const unsigned doe = (unsigned)(z - era * 146097);
  const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
  const int64_t yy = (int64_t)yoe + era * 400;
  const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
  const unsigned mp = (5 * doy + 2) / 153;
  d = doy - (153 * mp + 2) / 5 + 1;
  m = mp < 10 ? mp + 3 : mp - 9;
  y = (int)(yy + (m <= 2));
}

// 0 = Sonntag
static inline int wochentag(int64_t tage) { return (int)(((tage % 7) + 11) % 7); }

static inline int64_t letzterSonntag(int y, int m) {
  const int64_t letzter = tageAusDatum(m == 12 ? y + 1 : y, m == 12 ? 1 : m + 1, 1) - 1;
  return letzter - wochentag(letzter);
}

// Abstand der deutschen Ortszeit zu UTC zum Zeitpunkt t
static inline ms_t ortsAbstand(ms_t t) {
  int y, m, d;
  datumAusTagen(teileAb(t, TAG), y, m, d);
  const ms_t beginn = letzterSonntag(y, 3) * TAG + STUNDE;
  const ms_t ende = letzterSonntag(y, 10) * TAG + STUNDE;
  return (t >= beginn && t < ende) ? 2 * STUNDE : STUNDE;
}

struct Ortszeit { int jahr, monat, tag, stunde, minute, sekunde; };

static inline Ortszeit ortszeit(ms_t t) {
  const ms_t l = t + ortsAbstand(t);
  const int64_t tage = teileAb(l, TAG);
  Ortszeit o;
  datumAusTagen(tage, o.jahr, o.monat, o.tag);
  const ms_t imTag = l - tage * TAG;
  o.stunde = (int)(imTag / STUNDE);
  o.minute = (int)(imTag % STUNDE / MINUTE);
  o.sekunde = (int)(imTag % MINUTE / SEKUNDE);
  return o;
}

// Wie new Date(jahr, monat - 1, tag, stunde, minute) in der App. Minuten über 59 laufen weiter.
static inline ms_t ausOrtszeit(int jahr, int monat, int tag, int stunde, int minute) {
  const ms_t naiv = tageAusDatum(jahr, monat, tag) * TAG + stunde * STUNDE + minute * MINUTE;
  ms_t t = naiv - STUNDE;
  t = naiv - ortsAbstand(t);
  return naiv - ortsAbstand(t);
}

static inline int stundeVon(ms_t t) { return ortszeit(t).stunde; }

// Kennzahl eines Tages (Ortszeit), z. B. 20261123
static inline int32_t tagKennung(ms_t t) {
  const Ortszeit o = ortszeit(t);
  return o.jahr * 10000 + o.monat * 100 + o.tag;
}
