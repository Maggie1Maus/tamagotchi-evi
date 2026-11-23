// Der Himmel im Fenster folgt der echten Sonne in Deutschland, wie sky.js in der Handy-App.
#pragma once
#include <math.h>
#include "zeit.h"

enum Himmel : uint8_t { HIMMEL_TAG, HIMMEL_DAEMMERUNG, HIMMEL_NACHT };

namespace SKY {
const double BREITE = 51.16, LAENGE = 10.45;   // Mitte Deutschlands
const double SONNE_WEG = -0.833, DAEMMERUNG_WEG = -6;
}

struct Sonnenzeiten { ms_t morgengrauen, aufgang, untergang, abenddaemmerung; };

static inline Sonnenzeiten sonnenzeiten(ms_t t) {
  const double rad = M_PI / 180;
  const double mittag = (double)(teileAb(t, TAG) * TAG + TAG / 2);
  const double n = round(mittag / TAG + 2440587.5 - 2451545.0);
  const double mittel = n - SKY::LAENGE / 360;
  const double M = fmod(357.5291 + 0.98560028 * mittel, 360);
  const double C = 1.9148 * sin(M * rad) + 0.02 * sin(2 * M * rad) + 0.0003 * sin(3 * M * rad);
  const double L = fmod(M + C + 180 + 102.9372, 360);
  const double durchgang = 2451545.0 + mittel + 0.0053 * sin(M * rad) - 0.0069 * sin(2 * L * rad);
  const double dekl = asin(sin(L * rad) * sin(23.4397 * rad));
  auto bei = [&](double hoehe, ms_t& vor, ms_t& nach) {
    double c = (sin(hoehe * rad) - sin(SKY::BREITE * rad) * sin(dekl)) / (cos(SKY::BREITE * rad) * cos(dekl));
    if (c > 1) c = 1;
    if (c < -1) c = -1;
    const double stunden = acos(c) / rad / 360;
    vor = (ms_t)llround((durchgang - stunden - 2440587.5) * TAG);
    nach = (ms_t)llround((durchgang + stunden - 2440587.5) * TAG);
  };
  Sonnenzeiten s;
  bei(SKY::SONNE_WEG, s.aufgang, s.untergang);
  bei(SKY::DAEMMERUNG_WEG, s.morgengrauen, s.abenddaemmerung);
  return s;
}

static inline Himmel himmelBei(ms_t t) {
  Himmel h = HIMMEL_NACHT;
  for (int i = -1; i <= 1; i++) {
    const Sonnenzeiten s = sonnenzeiten(t + i * TAG);
    if (t >= s.aufgang && t < s.untergang) return HIMMEL_TAG;
    if (t >= s.morgengrauen && t < s.abenddaemmerung) h = HIMMEL_DAEMMERUNG;
  }
  return h;
}
