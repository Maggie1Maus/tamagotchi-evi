// Prüft Jahreszeiten, Weihnachtszeit und Wetter (umgebung.ino) und fotografiert das Zimmer.
#include "Arduino.h"
#include <string>
#include <functional>
uint32_t simMillis = 1000;
SerialC Serial;
#include "Wire.h"
TwoWire Wire1;
#include "SPI.h"
SPIClass SPI1;
void (*simNachBild)() = nullptr;
unsigned long simGesendet = 0;
int simDC = 1;
uint8_t simDisplay[240][320];
long simFalschePunkte = 0, simBilder = 0;
void simDisplayVergleichen();
void nachbildungBildFertig() { simMillis += 5; simBilder++; simDisplayVergleichen(); if (simNachBild) simNachBild(); }
#include "EEPROM.h"
EEPROMC EEPROM;
#include "evi-handy.ino"
void simDisplayVergleichen() {
  const uint8_t* b = bild.bits();
  for (int y = 0; y < 240; y++) for (int x = 0; x < 320; x++)
    if (simDisplay[y][x] != bild.stufe(x, y)) simFalschePunkte++;
}
int fehler = 0;
#define PRUEFE(b, t) do { if (!(b)) { printf("FEHLER: %s\n", t); fehler++; } else printf("ok: %s\n", t); } while (0)
void foto(const char* name) {
  char p[80]; snprintf(p, sizeof(p), "bilder/%s.pbm", name);
  FILE* f = fopen(p, "wb"); fprintf(f, "P4\n320 240\n");
  const uint8_t* b = bild.bits();
  for (int i = 0; i < 40 * 240; i++) { uint8_t v = ~b[i]; fwrite(&v, 1, 1, f); }
  fclose(f);
}
ms_t um(int j, int m, int t, int h = 12, int mi = 0) { return ausOrtszeit(j, m, t, h, mi); }
// Ein Bild des Zimmers zu dieser Zeit (Evi erwachsen und zufrieden)
void zimmerFoto(const char* name, ms_t t, int extraMs = 0) {
  uhrSetzen(t);
  pet = neuesTier(t - 4 * TAG); pet.aktualisiert = t; pet.satt = pet.laune = pet.sauber = 90;
  himmel = himmelBei(t);
  moment.letzterBesonderer = t; moment.naechster = t + STUNDE;
  simMillis += extraMs;
  zeichneAlles();
  foto(name);
}

void niederschlagPruefen(Wetter wetter, int stunde) {
  ms_t t = 0;
  for (int tag = 0; tag < 365 && !t; tag++) {
    const ms_t kandidat = um(2027, 1, 1, stunde) + (ms_t)tag * TAG;
    if (jahreszeitBei(kandidat) == JZ_WINTER && wetterBei(kandidat) == wetter) t = kandidat;
  }
  PRUEFE(t != 0, "Tag für Niederschlag gefunden");
  if (!t) return;
  uint8_t vorher[BREITE * HOEHE];
  bild.fillScreen(1); fensterZeichnen(t);
  memcpy(vorher, bild.getBuffer(), sizeof(vorher));
  simMillis += wetter == WT_SCHNEE ? 190 : 110;
  fensterZeichnen(t);
  int bewegt = 0, ausserhalb = 0;
  for (int y = 0; y < HOEHE; y++) for (int x = 0; x < BREITE; x++) {
    const int i = y * BREITE + x;
    if (vorher[i] == bild.getBuffer()[i]) continue;
    bewegt++;
    if (!zimmerScheibe(x, y)) ausserhalb++;
  }
  PRUEFE(bewegt > 0, "Regen/Schnee bewegt sich am Tag und in der Nacht");
  PRUEFE(ausserhalb == 0, "Niederschlag bleibt innerhalb der vier Fensterscheiben");
  char name[50]; snprintf(name, sizeof(name), "animation-%s-%s", wetter == WT_REGEN ? "regen" : "schnee", stunde == 1 ? "nacht" : "tag");
  zimmerFoto(name, t);

  // Der echte Hauptablauf muss vor dem nächsten 120-ms-Spielschritt ein neues Wetterbild senden.
  bildschirmAn = true; menueOffen = false; spiel.aktiv = false;
  letzterTakt = letzteRunde = letztesBild = millis();
  const long bilderVorher = simBilder;
  simMillis += 45; loop();
  PRUEFE(simBilder > bilderVorher, "Wetter wird zwischen den Spielschritten neu gezeichnet");
  bildschirmAn = false;
  const long bilderDunkel = simBilder;
  simMillis += 45; loop();
  PRUEFE(simBilder == bilderDunkel, "Wetter zeichnet bei ausgeschaltetem Bildschirm nicht weiter");
  bildschirmAn = true;
}
int main() {
  bildschirmStarten(); bild.cp437(true);
  PRUEFE(jahreszeitBei(um(2027, 1, 31)) == JZ_WINTER, "31. Januar: Winter");
  PRUEFE(jahreszeitBei(um(2027, 2, 1, 0, 1)) == JZ_FRUEHLING, "1. Februar: Frühling");
  PRUEFE(jahreszeitBei(um(2027, 5, 31, 23, 59)) == JZ_FRUEHLING, "31. Mai: Frühling");
  PRUEFE(jahreszeitBei(um(2027, 6, 1, 0, 1)) == JZ_SOMMER, "1. Juni: Sommer");
  PRUEFE(jahreszeitBei(um(2027, 9, 20, 23, 59)) == JZ_SOMMER, "20. September: Sommer");
  PRUEFE(jahreszeitBei(um(2027, 9, 21, 0, 1)) == JZ_HERBST, "21. September: Herbst");
  PRUEFE(jahreszeitBei(um(2026, 10, 31, 23, 59)) == JZ_HERBST, "31. Oktober: Herbst");
  PRUEFE(jahreszeitBei(um(2026, 11, 1, 0, 1)) == JZ_WINTER, "1. November: Winter");
  PRUEFE(!weihnachtsZeit(um(2026, 11, 17, 23, 59)), "17. November: noch keine Weihnachtsdeko");
  PRUEFE(weihnachtsZeit(um(2026, 11, 18, 0, 1)), "18. November: Weihnachtsdeko kommt");
  PRUEFE(weihnachtsZeit(um(2026, 12, 31)), "Silvester: Weihnachtsdeko");
  PRUEFE(weihnachtsZeit(um(2027, 1, 5, 23, 59)), "5. Januar: noch da");
  PRUEFE(!weihnachtsZeit(um(2027, 1, 6, 0, 1)), "6. Januar: Weihnachtsdeko weg");
  // Wetter über ein ganzes Jahr
  int anzahl[4][5] = {};
  bool schneeAusserWinter = false;
  int wechsel = 0;
  Wetter vorher = wetterBei(um(2026, 1, 1, 0));
  for (ms_t t = um(2026, 1, 1, 0); t < um(2027, 1, 1, 0); t += STUNDE) {
    const Wetter w = wetterBei(t);
    const Jahreszeit j = jahreszeitBei(t);
    anzahl[j][w]++;
    if (w == WT_SCHNEE && j != JZ_WINTER) schneeAusserWinter = true;
    if (w != vorher && stundeVon(t) % 3 != 0) wechsel++;
    vorher = w;
  }
  static const char* JZ[4] = {"Frühling", "Sommer", "Herbst", "Winter"};
  for (int j = 0; j < 4; j++) printf("   %-9s Sonne %4d  Sonne+Wolken %4d  Regen %4d  bewölkt %4d  Schnee %4d\n", JZ[j],
                                    anzahl[j][0], anzahl[j][1], anzahl[j][2], anzahl[j][3], anzahl[j][4]);
  PRUEFE(!schneeAusserWinter, "Schnee nur im Winter");
  PRUEFE(anzahl[JZ_WINTER][WT_SCHNEE] > 100, "im Winter schneit es oft");
  PRUEFE(wechsel == 0, "Wetter wechselt nur alle 3 Stunden");
  PRUEFE(wetterBei(um(2026, 12, 3, 10, 5)) == wetterBei(um(2026, 12, 3, 11, 55)), "dasselbe Wetter im selben Abschnitt");
  for (Wetter w : {WT_REGEN, WT_SCHNEE}) for (int stunde : {12, 1}) niederschlagPruefen(w, stunde);
  // Bilder: jede Jahreszeit mit jedem Wetter (am Tag), dazu Nacht und Dämmerung
  for (int j = 0; j < 4; j++) {
    const int monate[4] = {4, 7, 10, 2};   // Frühling, Sommer, Herbst, Winter (Februar: Frühling! darum unten anders)
    for (int w = 0; w < 5; w++) {
      // einen Tag und Abschnitt am Mittag suchen, an dem genau dieses Wetter ist
      ms_t gefunden = 0;
      const int jahr = 2027;
      for (int tag = 1; tag < 200 && !gefunden; tag++) {
        ms_t t = um(jahr, j == 3 ? 1 : monate[j], 1, 12) + (ms_t)tag * TAG;
        if (j == 3) t = um(2027, 1, 7, 12) + (ms_t)(tag % 24) * TAG;   // Winter ohne Weihnachten: 7.–30. Januar
        if (jahreszeitBei(t) == (Jahreszeit)j && wetterBei(t) == (Wetter)w && !weihnachtsZeit(t)) gefunden = t;
      }
      char name[40];
      snprintf(name, sizeof(name), "zimmer-%d-%d", j, w);
      if (gefunden) zimmerFoto(name, gefunden, 0);
      else if (w != WT_SCHNEE || j == JZ_WINTER) { printf("FEHLER: kein Tag für %s/%d\n", JZ[j], w); fehler++; }
    }
  }
  zimmerFoto("weihnachten-tag", um(2026, 12, 10, 12));
  zimmerFoto("weihnachten-nacht", um(2026, 12, 10, 1));
  zimmerFoto("winter-daemmerung", um(2027, 1, 20, 16, 50));
  static const int NACHTMONAT[4] = {4, 7, 10, 1};
  for (int j = 0; j < 4; j++) { char n[32]; snprintf(n, sizeof(n), "nacht-%d", j); zimmerFoto(n, um(2027, NACHTMONAT[j], 20, 1)); }
  printf("Display: %ld Bilder, %ld falsche Punkte\n", simBilder, simFalschePunkte);
  printf("%s: %d Fehler\n", fehler ? "FEHLER" : "OK", fehler);
  return fehler ? 1 : 0;
}
