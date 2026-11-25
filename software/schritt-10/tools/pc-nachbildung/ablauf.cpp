#include "Arduino.h"
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
// Steht auf dem nachgebildeten Display dasselbe wie im Bild des Programms?
void simDisplayVergleichen() {
  const uint8_t* b = bild.bits();
  for (int y = 0; y < 240; y++) for (int x = 0; x < 320; x++)
    if (simDisplay[y][x] != bild.stufe(x, y)) simFalschePunkte++;
}
int fehler = 0;
#define PRUEFE(b, t) do { if (!(b)) { printf("FEHLER: %s\n", t); fehler++; } else printf("ok: %s\n", t); } while (0)
void laufen(uint32_t ms) { for (uint32_t t = 0; t < ms; t += 10) { simMillis += 10; loop(); } }
// Eine echte Taste über den nachgebildeten MCP23017 drücken und loslassen
void druecken(int k) { simTaste(k, true); laufen(60); simTaste(k, false); laufen(60); }
int main() {
  bildschirmStarten(); bild.cp437(true);
  const ms_t utc = ausOrtszeit(2026, 11, 23, 12, 0);
  uhrSetzen(utc); pet = neuesTier(utc - 4 * TAG); pet.aktualisiert = utc; pet.satt = pet.laune = pet.sauber = 90;
  himmel = himmelBei(utc); moment.naechster = utc + 10 * MINUTE;
  moment.letzterBesonderer = utc;           // heute schon gesehen
  PRUEFE(tastenStarten(), "Tasten (nachgebildeter MCP23017) bereit");
  bildschirmAn = true; letzteBeruehrung = millis();
  laufen(5 * 60000);
  PRUEFE(bildschirmAn, "Bildschirm bleibt an (auch nach 5 Minuten)");
  druecken(K_RESET);
  PRUEFE(!bildschirmAn, "RESET schaltet den Bildschirm aus");
  PRUEFE(dunkelSeit > 0, "dunkelSeit gesetzt");
  // Besuch wird fällig, während der Bildschirm dunkel ist
  MomentArt arten[2]; ms_t um[2]; int n = besuchHeute(jetztMs(), arten, um);
  ms_t erster = um[0]; for (int i = 0; i < n; i++) if (um[i] > jetztMs() && um[i] < erster) erster = um[i];
  if (erster <= jetztMs()) erster = um[n - 1];
  moment.letzterBesonderer = 0;
  const ms_t bis = erster + MINUTE;
  uhrSetzen(bis);   // springen: kurz nach dem Besuchstermin
  laufen(2000);
  PRUEFE(bildschirmAn, "Besuch schaltet den Bildschirm ein");
  PRUEFE(moment.art == M_FALTER || moment.art == M_VOGEL || moment.art == M_STERN, "Besuch läuft");
  bool immerAn = true;
  for (int i = 0; i < 900 && moment.art != M_KEINER; i++) { laufen(100); if (moment.art != M_KEINER && !bildschirmAn) immerAn = false; }
  PRUEFE(moment.art == M_KEINER, "Besuch vorbei");
  PRUEFE(immerAn, "Bildschirm blieb an, solange Besuch da war");
  laufen(70000);
  PRUEFE(bildschirmAn, "bleibt nach dem Besuch an");
  druecken(K_RESET);
  PRUEFE(!bildschirmAn, "RESET schaltet wieder aus");
  // nach 4 Minuten mit RESET wieder an: Begrüßung
  laufen(4 * 60000);
  evi.reaktion = R_KEINE;
  druecken(K_RESET);
  PRUEFE(bildschirmAn, "RESET schaltet wieder an");
  PRUEFE(evi.reaktion == R_GRUSS || evi.reaktion == R_JUBEL, "Begrüßung beim Aufwecken");
  printf("sagt: %s\n", sagen.text);
  // RESET mitten im Spiel: Spiel abgebrochen, Bildschirm aus; A macht ihn wieder an (ohne Spielzug)
  spielStarten(SPIEL_BALL); laufen(500);
  druecken(K_RESET);
  PRUEFE(!spiel.aktiv && !bildschirmAn, "RESET im Spiel: abgebrochen und aus");
  druecken(K_A);
  PRUEFE(bildschirmAn && !menueOffen, "erster Druck macht nur an");
  // kleiner Moment kommt bei hellem Bildschirm
  moment.naechster = jetztMs() + 5000; moment.ruheBisTakt = 0; evi.reaktionBisTakt = 0;
  for (int i = 0; i < 300 && moment.art == M_KEINER; i++) { laufen(100); letzteBeruehrung = millis(); }
  PRUEFE(moment.art != M_KEINER, "kleiner Moment startet");
  printf("Display: %ld Bilder verglichen, %ld falsche Punkte\n", simBilder, simFalschePunkte);
  if (simFalschePunkte) { printf("FEHLER: Display weicht ab\n"); fehler++; }
  printf("%s: %d Fehler\n", fehler ? "NICHT OK" : "OK", fehler);
  return fehler;
}
