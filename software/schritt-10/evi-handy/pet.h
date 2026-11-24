// Spielregeln der Handy-App (handy-app/dist/pet.js, Version 0.11), übertragen nach C++.
// Keine Abhängigkeit von Bildschirm, Touch oder Speicher. Zahlen wie in RULES dort.
// Abweichung: Tageskennungen sind Zahlen (20261123) statt Texte ("2026-11-23").
#pragma once
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "zeit.h"

namespace R {
const ms_t EI_DAUER = 30 * MINUTE;
const ms_t BABY = 1 * TAG, TEEN = 2 * TAG, ERWACHSEN = 5 * TAG, OMA = 2 * TAG;
const ms_t LEBENSDAUER = BABY + TEEN + ERWACHSEN + OMA;
// Abnahme pro Stunde wach: satt, Laune, sauber
const double ABNAHME[4][3] = {{45, 35, 30}, {28, 22, 18}, {20, 15, 12}, {18, 15, 12}};
const int SCHLAF_VON = 23, SCHLAF_BIS = 7;
const double SCHLAF_BEDARF = 0.15, LAUNE_DUNKEL = 0.1, LAUNE_HELL = 0.6, LICHT_BONUS = 10;
const int MUEDE_ABENDS = 22, MUEDE_MORGENS_MIN = 30;
const int RITUAL_AB = 22; const double RITUAL_LAUNE = 10;
const ms_t WACH_BLEIBEN = 5 * MINUTE;
const int MITTAG_VON = 12, MITTAG_BIS = 13;   // nur Baby
const int MAHLZEIT[3][2] = {{7, 9}, {11, 14}, {17, 20}};
const double MAHLZEIT_SATT = 10, MAHLZEIT_LAUNE = 5;
const int NASCHEN_PRO_TAG = 3; const ms_t BAUCHWEH = 2 * STUNDE;
const int WUNSCH_FENSTER[4][2] = {{8, 11}, {11, 14}, {14, 18}, {18, 22}};
const ms_t WUNSCH_DAUER = STUNDE; const double WUNSCH_LAUNE = 10; const int WUNSCH_MUENZEN = 3;
const double SPIEL_LAUNE = 15, SPIEL_LAUNE_TREFFER = 7, SPIEL_SATT = -8, SPIEL_SAUBER = -5;
const int SPIEL_MUENZEN_TREFFER = 2, SPIEL_RUNDEN = 5;
const ms_t KRANK_NACH = 4 * STUNDE; const double KRANK_LAUNE = 1.5;
const ms_t TOD_VERNACHLAESSIGT = 30 * STUNDE, TOD_KRANK = 24 * STUNDE;
const double NIEDRIG = 30;
// Gassi gehen (24.11.2026, nur auf dem Board): ab 100 Schritten +10 Stimmung, je volle 1000 Schritte +5 mehr
// (höchstens +25), Spazierengehen macht etwas hungrig und die Pfoten schmutzig (je -5), 1 Münze je 500 Schritte (höchstens 10).
const uint32_t GASSI_MIN_SCHRITTE = 100, GASSI_SCHRITTE_JE_MUENZE = 500;
const double GASSI_LAUNE = 10, GASSI_LAUNE_JE_1000 = 5, GASSI_LAUNE_MAX = 25, GASSI_SATT = -5, GASSI_SAUBER = -5;
const int GASSI_MUENZEN_MAX = 10;
// Kuscheln (24.11.2026, nur auf dem Board): Evi antippen oder streicheln. +5 Stimmung, an verschmusten Tagen +8,
// wenn sie selbst kuscheln wollte +10. Wirkt höchstens einmal pro Minute, öfter gibt es nur Herzen.
const double KUSCHEL_LAUNE = 5, KUSCHEL_LAUNE_VERSCHMUST = 8, KUSCHEL_LAUNE_WUNSCH = 10;
const ms_t KUSCHEL_PAUSE = MINUTE;
// Launenwünsche (24.11.2026, nur auf dem Board, launen.ino): erfüllt +10 Stimmung (KUSCHEL_LAUNE_WUNSCH),
// nicht erfüllt -5. Zwei verpasste Wünsche hintereinander: Evi ist 15 Minuten beleidigt.
const double WUNSCH_VERPASST_LAUNE = -5;
const ms_t BELEIDIGT_DAUER = 15 * MINUTE;
}

enum Phase : uint8_t { EI, BABY, TEEN, ERWACHSEN, OMA, TOT };
enum Schlaf : uint8_t { WACH, NACHTS, MITTAGSSCHLAF };
enum Ursache : uint8_t { KEINE_URSACHE, VERNACHLAESSIGT, KRANKHEIT, ALTER };
enum Aktion : uint8_t { FUETTERN, NASCHEN, PUTZEN, MEDIZIN, LICHT, RITUAL, AUFWECKEN, NEUES_EI, SPIELEN };
enum Grund : uint8_t { OK, G_TOT, G_LEBT, G_EI, G_TAG, G_WACH, G_SCHLAEFT, G_MITTAG, G_FRUEH, G_SCHON, G_KRANK, G_GESUND, G_BAUCH, G_VOLL, G_UNBEKANNT };
enum Wunschart : uint8_t { W_BALL, W_RATEN, W_MELODIE, W_NASCHEN, W_PUTZEN, W_FUETTERN };
enum Stimmung : uint8_t { S_TOT, S_EI, S_SCHLAEFT, S_KRANK, S_BAUCH, S_HUNGRIG, S_SCHMUTZIG, S_TRAURIG, S_GLUECKLICH };

struct Tier {
  char name[21];
  bool eigenerName;
  uint16_t generation;
  ms_t geboren, schluepft, aktualisiert;
  double satt, laune, sauber;
  uint32_t pflege, muenzen;
  bool krank;
  ms_t vernachlaessigt, krankSeit;
  bool lichtAus;
  int32_t lichtBonusNacht;          // -1 = noch nie
  bool tot;
  ms_t gestorben;
  uint8_t ursache;
  int32_t ritualNacht, letzteMahlzeit, naschTag;   // -1 = noch nie
  uint16_t naschZahl;
  ms_t bauchwehBis, wunschErfuellt, wachBis;
};

struct Wunsch { Wunschart art; ms_t beginn; };

static inline double begrenzt(double v) { return v < 0 ? 0 : v > 100 ? 100 : v; }
static inline bool istNacht(ms_t t) { const int h = stundeVon(t); return h >= R::SCHLAF_VON || h < R::SCHLAF_BIS; }
// Eine Kennung pro Nacht (Datum des Abends)
static inline int32_t nachtKennung(ms_t t) { return tagKennung(t - R::SCHLAF_BIS * STUNDE); }

static inline Tier neuesTier(ms_t jetzt, ms_t eiDauer = R::EI_DAUER, const char* name = "Evi", bool eigener = false,
                             uint32_t muenzen = 0, uint16_t generation = 1) {
  Tier t;
  memset(&t, 0, sizeof(t));
  strncpy(t.name, name, 20);
  t.eigenerName = eigener;
  t.generation = generation;
  t.geboren = jetzt;
  t.schluepft = jetzt + eiDauer;
  t.aktualisiert = jetzt;
  t.satt = 80; t.laune = 80; t.sauber = 100;
  t.muenzen = muenzen;
  t.lichtBonusNacht = -1;
  t.ritualNacht = -1; t.letzteMahlzeit = -1; t.naschTag = -1;
  return t;
}

static inline Phase phaseBei(const Tier& p, ms_t t) {
  if (p.tot) return TOT;
  if (t < p.schluepft) return EI;
  ms_t alter = t - p.schluepft;
  if (alter < R::BABY) return BABY;
  alter -= R::BABY;
  if (alter < R::TEEN) return TEEN;
  alter -= R::TEEN;
  if (alter < R::ERWACHSEN) return ERWACHSEN;
  return OMA;
}

static inline bool schlafenszeit(const Tier& p, ms_t t) {
  return istNacht(t) || (stundeVon(t) >= R::RITUAL_AB && p.ritualNacht == nachtKennung(t));
}
static inline bool geweckt(const Tier& p, ms_t t) { return schlafenszeit(p, t) && t < p.wachBis; }

static inline Schlaf schlafArt(const Tier& p, ms_t t) {
  const Phase ph = phaseBei(p, t);
  if (ph == EI || ph == TOT) return WACH;
  if (schlafenszeit(p, t)) return t < p.wachBis ? WACH : NACHTS;
  const int h = stundeVon(t);
  if (ph == BABY && h >= R::MITTAG_VON && h < R::MITTAG_BIS) return MITTAGSSCHLAF;
  return WACH;
}
static inline bool schlaeft(const Tier& p, ms_t t) { return schlafArt(p, t) != WACH; }
static inline bool wach(const Tier& p, ms_t t) { const Phase ph = phaseBei(p, t); return ph != EI && ph != TOT && !schlaeft(p, t); }

static inline bool ritualLetzteNacht(const Tier& p, ms_t t) { return p.ritualNacht != -1 && p.ritualNacht == tagKennung(t - 12 * STUNDE); }
static inline bool fruehMorgens(ms_t t) { const Ortszeit o = ortszeit(t); return o.stunde == R::SCHLAF_BIS && o.minute < R::MUEDE_MORGENS_MIN; }
static inline bool muede(const Tier& p, ms_t t) {
  if (!wach(p, t)) return false;
  return istNacht(t) || stundeVon(t) == R::MUEDE_ABENDS || (fruehMorgens(t) && !ritualLetzteNacht(p, t));
}
static inline bool ausgeschlafen(const Tier& p, ms_t t) { return wach(p, t) && fruehMorgens(t) && ritualLetzteNacht(p, t); }
static inline bool bauchweh(const Tier& p, ms_t t) { return !p.tot && t < p.bauchwehBis; }

// Mahlzeit zur Zeit t: 0 Frühstück, 1 Mittagessen, 2 Abendessen, -1 keine
static inline int mahlzeitBei(ms_t t) {
  const int h = stundeVon(t);
  for (int i = 0; i < 3; i++) if (h >= R::MAHLZEIT[i][0] && h < R::MAHLZEIT[i][1]) return i;
  return -1;
}
static inline int32_t mahlzeitKennung(ms_t t) { return tagKennung(t) * 10 + mahlzeitBei(t); }
static inline int mahlzeitFaellig(const Tier& p, ms_t t) {
  const int m = mahlzeitBei(t);
  return (m >= 0 && p.letzteMahlzeit != mahlzeitKennung(t)) ? m : -1;
}

// Wie hash() in pet.js (32 Bit)
static inline uint32_t streuwert(const char* text) {
  uint32_t h = 2166136261u;
  for (const char* c = text; *c; c++) { h ^= (uint8_t)*c; h *= 16777619u; }
  h ^= h >> 15; h *= 2246822507u; h ^= h >> 13;
  return h;
}

// Die Wünsche eines Tages (3 oder 4). Gibt die Anzahl zurück.
static inline int wuenscheAm(const Tier& p, ms_t t, Wunsch aus[4]) {
  const Ortszeit o = ortszeit(t);
  char saat[48], text[56];
  snprintf(saat, sizeof(saat), "%lld:%d-%d-%d", (long long)p.geboren, o.jahr, o.monat, o.tag);
  snprintf(text, sizeof(text), "%s:drop", saat);
  const uint32_t weg = streuwert(text);
  const int entfaellt = (weg & 1) ? (int)((weg >> 1) % 4) : -1;
  int n = 0;
  for (int i = 0; i < 4; i++) {
    if (i == entfaellt) continue;
    snprintf(text, sizeof(text), "%s:%d", saat, i);
    const uint32_t h = streuwert(text);
    const int spaetestens = (R::WUNSCH_FENSTER[i][1] - R::WUNSCH_FENSTER[i][0]) * 60 - (int)(R::WUNSCH_DAUER / MINUTE);
    aus[n].beginn = ausOrtszeit(o.jahr, o.monat, o.tag, R::WUNSCH_FENSTER[i][0], (int)(h % (uint32_t)(spaetestens + 1)));
    aus[n].art = (Wunschart)((h >> 12) % 6);
    n++;
  }
  return n;
}

// Der Wunsch, den Evi gerade hat. Pausiert im Schlaf, bei Krankheit und Bauchweh.
static inline bool aktuellerWunsch(const Tier& p, ms_t t, Wunsch& w) {
  if (!wach(p, t) || p.krank || bauchweh(p, t)) return false;
  Wunsch alle[4];
  const int n = wuenscheAm(p, t, alle);
  for (int i = 0; i < n; i++) {
    if (t >= alle[i].beginn && t < alle[i].beginn + R::WUNSCH_DAUER) {
      if (alle[i].beginn == p.wunschErfuellt) return false;
      w = alle[i];
      return true;
    }
  }
  return false;
}

// Rechnet die vergangene Zeit nach, Minute für Minute.
static inline void weiterrechnen(Tier& p, ms_t jetzt) {
  if (p.tot || jetzt <= p.aktualisiert) return;
  ms_t t = p.aktualisiert;
  while (t < jetzt) {
    ms_t ende = teileAb(t, MINUTE) * MINUTE + MINUTE;
    if (ende > jetzt) ende = jetzt;
    const ms_t dt = ende - t;
    const double stunden = (double)dt / STUNDE;
    const Phase ph = phaseBei(p, t);
    if (ph != EI && t >= p.schluepft + R::LEBENSDAUER) {
      // Altersschwäche: die Oma schläft am Ende ihres Lebens friedlich ein.
      p.tot = true; p.gestorben = p.schluepft + R::LEBENSDAUER; p.aktualisiert = p.gestorben;
      p.lichtAus = false; p.ursache = ALTER;
      return;
    }
    if (ph != EI) {
      const Schlaf s = schlafArt(p, t);
      if (!schlafenszeit(p, t)) p.lichtAus = false;
      const double* rate = R::ABNAHME[ph - BABY];
      const double bedarf = s != WACH ? R::SCHLAF_BEDARF : 1;
      const double launeSchlaf = s == WACH ? 1 : (s == MITTAGSSCHLAF || p.lichtAus) ? R::LAUNE_DUNKEL : R::LAUNE_HELL;
      const double unwohl = (p.krank || t < p.bauchwehBis) ? R::KRANK_LAUNE : 1;
      p.satt = begrenzt(p.satt - rate[0] * bedarf * stunden);
      p.sauber = begrenzt(p.sauber - rate[2] * bedarf * stunden);
      p.laune = begrenzt(p.laune - rate[1] * launeSchlaf * unwohl * stunden);
      const bool vernachlaessigt = p.satt <= 0 || p.laune <= 0 || p.sauber <= 0;
      p.vernachlaessigt = vernachlaessigt ? p.vernachlaessigt + dt : (p.vernachlaessigt > dt ? p.vernachlaessigt - dt : 0);
      if (p.krank) p.krankSeit += dt;
      if (!p.krank && p.vernachlaessigt >= R::KRANK_NACH) p.krank = true;
      if (p.vernachlaessigt >= R::TOD_VERNACHLAESSIGT || p.krankSeit >= R::TOD_KRANK) {
        p.tot = true; p.gestorben = ende; p.aktualisiert = ende; p.lichtAus = false;
        p.ursache = p.krankSeit >= R::TOD_KRANK ? KRANKHEIT : VERNACHLAESSIGT;
        return;
      }
    }
    t = ende;
  }
  p.aktualisiert = jetzt;
}

// Jeder Tastendruck hält eine geweckte Evi weitere 5 Minuten wach.
static inline void wachHalten(Tier& p, ms_t jetzt) {
  weiterrechnen(p, jetzt);
  if (geweckt(p, jetzt) && p.wachBis < jetzt + R::WACH_BLEIBEN) p.wachBis = jetzt + R::WACH_BLEIBEN;
}

// Warum eine Aktion gerade nicht geht, oder OK.
static inline Grund gesperrt(const Tier& p, Aktion a, ms_t jetzt) {
  const Phase ph = phaseBei(p, jetzt);
  if (ph == TOT) return a == NEUES_EI ? OK : G_TOT;
  if (a == NEUES_EI) return G_LEBT;
  if (ph == EI) return G_EI;
  const Schlaf s = schlafArt(p, jetzt);
  // Nachts geweckt: Licht aus schickt sie sofort wieder schlafen (Wunsch der Nutzerin vom 24.11.2026)
  if (a == LICHT) return s == NACHTS || geweckt(p, jetzt) ? OK : G_TAG;
  if (a == AUFWECKEN) return s == NACHTS ? OK : G_WACH;
  if (s == NACHTS) return G_SCHLAEFT;
  if (s == MITTAGSSCHLAF) return G_MITTAG;
  if (a == RITUAL && stundeVon(jetzt) != R::RITUAL_AB) return G_FRUEH;
  if (a == RITUAL && p.ritualNacht == nachtKennung(jetzt)) return G_SCHON;
  if (a == SPIELEN && p.krank) return G_KRANK;
  if (a == MEDIZIN && !p.krank && !bauchweh(p, jetzt)) return G_GESUND;
  if (a == NASCHEN && bauchweh(p, jetzt)) return G_BAUCH;
  // Ein Wunsch lässt sich immer erfüllen, auch wenn der Wert schon voll ist.
  Wunsch w;
  if (aktuellerWunsch(p, jetzt, w)) {
    if ((a == FUETTERN && w.art == W_FUETTERN) || (a == NASCHEN && w.art == W_NASCHEN) || (a == PUTZEN && w.art == W_PUTZEN)) return OK;
  }
  if (a == FUETTERN && p.satt >= 99.5) return G_VOLL;
  if (a == PUTZEN && p.sauber >= 99.5) return G_VOLL;
  if (a == SPIELEN && p.laune >= 99.5) return G_VOLL;
  return OK;
}

struct Ergebnis {
  bool geaendert = false;
  Grund grund = OK;
  bool bonus = false;     // Licht aus: +10 Laune (einmal pro Nacht)
  int mahlzeit = -1;
  bool bauchweh = false;
  bool wunsch = false;    // Wunsch erfüllt
  int muenzen = 0;
};

static inline bool wunschErfuellen(Tier& p, bool hatte, const Wunsch& w, Wunschart passend) {
  if (!hatte || w.art != passend) return false;
  p.wunschErfuellt = w.beginn;
  p.laune = begrenzt(p.laune + R::WUNSCH_LAUNE);
  p.muenzen += R::WUNSCH_MUENZEN;
  return true;
}

// Pflege: Füttern, Naschen, Putzen, Medizin, Licht, Gute Nacht, Aufwecken, neues Ei.
static inline Ergebnis pflegen(Tier& p, Aktion a, ms_t jetzt) {
  Ergebnis e;
  weiterrechnen(p, jetzt);
  e.grund = gesperrt(p, a, jetzt);
  if (e.grund != OK) return e;
  e.geaendert = true;
  if (a == NEUES_EI) {
    p = neuesTier(jetzt, R::EI_DAUER, p.name, p.eigenerName, p.muenzen, p.generation + 1);
    return e;
  }
  auto lichtAusmachen = [&]() {
    p.lichtAus = true;
    if (p.lichtBonusNacht == nachtKennung(jetzt)) return false;
    p.lichtBonusNacht = nachtKennung(jetzt);
    p.laune = begrenzt(p.laune + R::LICHT_BONUS);
    return true;
  };
  if (a == LICHT) {
    // Geweckt: Licht aus, und sie schläft gleich wieder ein, ohne die 5 Minuten abzuwarten.
    if (geweckt(p, jetzt)) { p.wachBis = jetzt; e.bonus = lichtAusmachen(); return e; }
    if (p.lichtAus) { p.lichtAus = false; return e; }
    e.bonus = lichtAusmachen();
    return e;
  }
  if (a == AUFWECKEN) { p.wachBis = jetzt + R::WACH_BLEIBEN; return e; }
  if (a == RITUAL) {
    p.ritualNacht = nachtKennung(jetzt);
    p.laune = begrenzt(p.laune + R::RITUAL_LAUNE);
    p.pflege++;
    e.bonus = lichtAusmachen();
    return e;
  }
  Wunsch w;
  const bool hatteWunsch = aktuellerWunsch(p, jetzt, w);
  if (a == FUETTERN) { p.satt = begrenzt(p.satt + 40); p.laune = begrenzt(p.laune + 3); p.sauber = begrenzt(p.sauber - 2); }
  if (a == NASCHEN) { p.satt = begrenzt(p.satt + 10); p.laune = begrenzt(p.laune + 10); }
  if (a == PUTZEN) { p.laune = begrenzt(p.laune + 3); p.sauber = begrenzt(p.sauber + 60); }
  if (a == MEDIZIN) { p.laune = begrenzt(p.laune - 5); }
  if (a == FUETTERN && mahlzeitFaellig(p, jetzt) >= 0) {
    e.mahlzeit = mahlzeitBei(jetzt);
    p.letzteMahlzeit = mahlzeitKennung(jetzt);
    p.satt = begrenzt(p.satt + R::MAHLZEIT_SATT);
    p.laune = begrenzt(p.laune + R::MAHLZEIT_LAUNE);
  }
  if (a == NASCHEN) {
    if (p.naschTag != tagKennung(jetzt)) { p.naschTag = tagKennung(jetzt); p.naschZahl = 0; }
    p.naschZahl++;
    if (p.naschZahl > R::NASCHEN_PRO_TAG) { p.bauchwehBis = jetzt + R::BAUCHWEH; e.bauchweh = true; }
  }
  if (a == MEDIZIN) {
    if (p.krank) { p.krank = false; p.krankSeit = 0; p.vernachlaessigt = 0; }
    p.bauchwehBis = 0;
  }
  const Wunschart passend = a == FUETTERN ? W_FUETTERN : a == NASCHEN ? W_NASCHEN : a == PUTZEN ? W_PUTZEN : (Wunschart)255;
  e.wunsch = wunschErfuellen(p, hatteWunsch, w, passend);
  p.pflege++;
  if (geweckt(p, jetzt) && p.wachBis < jetzt + R::WACH_BLEIBEN) p.wachBis = jetzt + R::WACH_BLEIBEN;
  return e;
}

// Ergebnis eines Spiels (0 Ball, 1 Raten, 2 Melodie): Treffer von 5 Runden.
static inline Ergebnis spielBeenden(Tier& p, int treffer, ms_t jetzt, Wunschart spiel) {
  Ergebnis e;
  weiterrechnen(p, jetzt);
  e.grund = gesperrt(p, SPIELEN, jetzt);
  if (e.grund != OK && e.grund != G_VOLL) return e;
  e.grund = OK;
  e.geaendert = true;
  Wunsch w;
  const bool hatteWunsch = aktuellerWunsch(p, jetzt, w);
  if (treffer < 0) treffer = 0;
  if (treffer > R::SPIEL_RUNDEN) treffer = R::SPIEL_RUNDEN;
  p.laune = begrenzt(p.laune + R::SPIEL_LAUNE + treffer * R::SPIEL_LAUNE_TREFFER);
  p.satt = begrenzt(p.satt + R::SPIEL_SATT);
  p.sauber = begrenzt(p.sauber + R::SPIEL_SAUBER);
  e.muenzen = treffer * R::SPIEL_MUENZEN_TREFFER;
  p.muenzen += e.muenzen;
  p.pflege++;
  if (geweckt(p, jetzt) && p.wachBis < jetzt + R::WACH_BLEIBEN) p.wachBis = jetzt + R::WACH_BLEIBEN;
  e.wunsch = wunschErfuellen(p, hatteWunsch, w, spiel);
  return e;
}

// Ein Spaziergang ist zu Ende (Gassi gehen, nur auf dem Board). Zählt nur, wenn Evi wach und gesund ist.
static inline Ergebnis gassiBeenden(Tier& p, uint32_t schritte, ms_t jetzt) {
  Ergebnis e;
  weiterrechnen(p, jetzt);
  if (schritte < R::GASSI_MIN_SCHRITTE) { e.grund = G_UNBEKANNT; return e; }
  if (!wach(p, jetzt)) { e.grund = G_SCHLAEFT; return e; }
  if (p.krank) { e.grund = G_KRANK; return e; }
  e.geaendert = true;
  double laune = R::GASSI_LAUNE + (schritte / 1000) * R::GASSI_LAUNE_JE_1000;
  if (laune > R::GASSI_LAUNE_MAX) laune = R::GASSI_LAUNE_MAX;
  p.laune = begrenzt(p.laune + laune);
  p.satt = begrenzt(p.satt + R::GASSI_SATT);
  p.sauber = begrenzt(p.sauber + R::GASSI_SAUBER);
  int muenzen = (int)(schritte / R::GASSI_SCHRITTE_JE_MUENZE);
  if (muenzen > R::GASSI_MUENZEN_MAX) muenzen = R::GASSI_MUENZEN_MAX;
  e.muenzen = muenzen;
  p.muenzen += muenzen;
  p.pflege++;
  return e;
}

// Evi wird gekuschelt (nur auf dem Board). Wirkt nur, wenn sie wach ist; die Pause (KUSCHEL_PAUSE) achtet kuscheln.ino.
static inline Ergebnis kuscheln(Tier& p, double laune, ms_t jetzt) {
  Ergebnis e;
  weiterrechnen(p, jetzt);
  if (!wach(p, jetzt)) { e.grund = G_SCHLAEFT; return e; }
  e.geaendert = true;
  p.laune = begrenzt(p.laune + laune);
  p.pflege++;
  if (geweckt(p, jetzt) && p.wachBis < jetzt + R::WACH_BLEIBEN) p.wachBis = jetzt + R::WACH_BLEIBEN;
  return e;
}

// Stimmung ändern, ohne dass es als Pflege zählt (Launenwünsche, nur auf dem Board). Nur, wenn sie wach ist.
static inline bool launeAendern(Tier& p, double um, ms_t jetzt) {
  weiterrechnen(p, jetzt);
  if (!wach(p, jetzt)) return false;
  p.laune = begrenzt(p.laune + um);
  return true;
}

// Phase und das Wichtigste: tot, Ei, schläft, krank, Bauchweh, hungrig, schmutzig, traurig, glücklich
static inline Stimmung beschreiben(const Tier& p, ms_t t) {
  const Phase ph = phaseBei(p, t);
  if (ph == TOT) return S_TOT;
  if (ph == EI) return S_EI;
  if (schlaeft(p, t)) return S_SCHLAEFT;
  if (p.krank) return S_KRANK;
  if (bauchweh(p, t)) return S_BAUCH;
  double niedrigster = p.satt;
  if (p.laune < niedrigster) niedrigster = p.laune;
  if (p.sauber < niedrigster) niedrigster = p.sauber;
  if (niedrigster <= R::NIEDRIG) {
    if (p.satt == niedrigster) return S_HUNGRIG;
    if (p.sauber == niedrigster) return S_SCHMUTZIG;
    return S_TRAURIG;
  }
  return S_GLUECKLICH;
}

static inline const char* phasenName(Phase ph) {
  static const char* namen[] = {"Ei", "Baby", "Teenager", "Erwachsen", "Oma", "Gestorben"};
  return namen[ph];
}
