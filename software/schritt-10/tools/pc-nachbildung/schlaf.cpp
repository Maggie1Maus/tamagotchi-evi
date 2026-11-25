// Prüft den Schlafmodus (schlaf.h, seit 25.11.2026): Helligkeit 50 %, Chip schläft zwischen den Abfragen
// (dunkel bis zum nächsten Spielschritt, 120 ms), langsamer Takt im Dunkeln, Bildschirm-Chip schläft, Ton-Chip nur
// bei Tönen an, Touch im Dunkeln aus (Tasten wecken), Umdrehen weckt weiter.
// Anders als in den anderen Prüfprogrammen vergeht hier beim Schlafen auch die Zeit (wie am Board).
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

// Lage: Funktion der Zeit seit Beginn (ms) -> x, y, z in g
using Bewegung = std::function<void(uint32_t, float&, float&, float&)>;
void inDerHand(uint32_t, float& x, float& y, float& z) { x = 0; y = 0.71f; z = 0.71f; }

// Wie am Board: Jede Runde braucht 1 ms, und was der Chip schläft, vergeht als Zeit.
// Liegt ein Finger auf, meldet der Touch-Chip wie die Unterbrechung am Board.
struct Statistik { uint32_t runden = 0, messungen = 0, geschlafen = 0, laengster = 0, taktGesenktRunden = 0; };
Statistik laufen(uint32_t ms, Bewegung b = nullptr) {
  Statistik s;
  const uint32_t start = simMillis;
  uint32_t letzteMessung = lage.letzteMessung;
  while (simMillis - start < ms) {
    if (b) { float x, y, z; b(simMillis - start, x, y, z); simLage(x, y, z); }
    if (simCst().unten) touchMeldet = true;
    const uint32_t vorher = schlafMs;
    simMillis += 1;
    loop();
    const uint32_t d = schlafMs - vorher;
    simMillis += d;
    s.runden++;
    s.geschlafen += d;
    if (d > s.laengster) s.laengster = d;
    if (taktGesenkt) s.taktGesenktRunden++;
    if (lage.letzteMessung != letzteMessung) { s.messungen++; letzteMessung = lage.letzteMessung; }
  }
  return s;
}
void druecken(int k) { simTaste(k, true); laufen(60); simTaste(k, false); laufen(60); }
void vorbereiten(int stunde, int minute) {
  const ms_t utc = ausOrtszeit(2026, 11, 24, stunde, minute);
  uhrSetzen(utc);
  pet = neuesTier(utc - 4 * TAG);
  pet.aktualisiert = utc;
  pet.satt = pet.laune = pet.sauber = 80;
  himmel = himmelBei(utc);
  spiel = SpielZustand();
  momentAufraeumen(); geher.x = 0; geher.fuss = RAUM_HEIM_Y; geher.flip = false;
  moment.naechster = utc + 30 * MINUTE;
  moment.ruheBisTakt = 0; moment.letzterBesonderer = utc;
  menueOffen = false; ritual.aktiv = false;
  gespraech.gebDialog = false;   // Geburtstag nicht gerade abfragen (freunde.ino)
  bildschirmAn = true; letzteBeruehrung = millis();
}

int main() {
  bildschirmStarten(); bild.cp437(true);
  simCst().an = true;
  touchStarten();
  PRUEFE(touchChip == TOUCH_CST328, "Touch-Chip bereit");
  PRUEFE(tastenStarten(), "Tasten bereit");
  PRUEFE(lageStarten(), "Lagesensor gefunden");
  tonStarten();
  PRUEFE(!i2sLaeuft, "Ton-Chip startet aus (bekommt erst bei einem Ton Takt)");
  vorbereiten(12, 0);
  bildschirmEin();
  PRUEFE(simLicht == 127, "Helligkeit 50 % (127 von 255)");

  // ---------- Hell ----------
  Statistik s = laufen(3000, inDerHand);
  printf("   hell: %u Runden in 3 s, geschlafen %u ms, längste Pause %u ms, Messungen %u\n", s.runden, s.geschlafen, s.laengster, s.messungen);
  PRUEFE(s.laengster <= SCHLAF_HELL_MS, "hell: kurze Pausen von höchstens 2 ms");
  PRUEFE(s.geschlafen > 1500, "hell: der Chip schläft mehr als die Hälfte der Zeit");
  PRUEFE(s.messungen >= 140, "hell: Lagesensor weiter etwa 50-mal pro Sekunde");
  PRUEFE(!taktGesenkt, "hell: voller Takt");
  PRUEFE(simFalschePunkte == 0, "hell: Bildschirm zeigt genau das Bild");

  // Finger liegt auf Evi: nicht schlafen (Streicheln bleibt flüssig)
  laufen(100);
  simFinger(true, eviLinks + eviBreite / 2, BODEN_Y - 10);
  laufen(30);
  s = laufen(500);
  PRUEFE(s.geschlafen == 0, "Finger liegt auf: keine Pausen");
  simFinger(false);
  laufen(400);

  // Im Spiel: keine Pausen
  spielStarten(SPIEL_BALL);
  PRUEFE(spiel.aktiv, "Spiel läuft");
  s = laufen(1000);
  PRUEFE(s.geschlafen == 0, "im Spiel: keine Pausen");
  druecken(K_RESET);
  PRUEFE(!spiel.aktiv && !bildschirmAn, "RESET bricht das Spiel ab und schaltet aus");

  // ---------- Dunkel ----------
  PRUEFE(simLicht == 0, "dunkel: Licht aus");
  PRUEFE(lcdSchlaeft && SPI1.schlaeft && !SPI1.anzeigeAn, "dunkel: Bildschirm-Chip schläft (0x28, 0x10)");
  PRUEFE(touchAus && simTouchRst == 0, "dunkel: Touch-Chip im Reset");
  const uint32_t taktVorher = evi.takt;
  s = laufen(10000, inDerHand);
  printf("   dunkel: %u Runden in 10 s, geschlafen %u ms, längste Pause %u ms, Messungen %u, Spielschritte %u\n", s.runden, s.geschlafen,
         s.laengster, s.messungen, evi.takt - taktVorher);
  PRUEFE(s.laengster <= SCHLAF_DUNKEL_MS, "dunkel: Pausen höchstens bis zum nächsten Spielschritt (120 ms)");
  PRUEFE(s.geschlafen > 9800, "dunkel: der Chip schläft über 98 % der Zeit");
  PRUEFE(s.runden < 100, "dunkel: nur etwa 8 Runden pro Sekunde");
  PRUEFE(s.messungen >= 70 && s.messungen <= 90, "dunkel: Lagesensor misst etwa 8-mal pro Sekunde");
  PRUEFE(evi.takt - taktVorher >= 75, "dunkel: Evis Spielschritte laufen weiter (etwa alle 120 ms)");
  PRUEFE(taktGesenkt, "dunkel: nach 3 s langsamer Takt");
  PRUEFE(s.taktGesenktRunden > s.runden / 2, "dunkel: die meiste Zeit langsamer Takt");
  PRUEFE(!bildschirmAn && lcdSchlaeft, "bleibt dunkel");

  // Ein Ton im Dunkeln: voller Takt, Ton-Chip an; danach wieder langsam, Ton-Chip nach dem Nachlauf aus
  const int startsVorher = i2s.starts;
  piep(TON(TON_WUNSCH));
  PRUEFE(!taktGesenkt && i2sLaeuft && i2s.starts == startsVorher + 1, "Ton: voller Takt, Ton-Chip an");
  bool taktBeiTon = true;
  uint32_t tonPause = 0;
  for (int i = 0; i < 400 && tonSpielt(); i++) {
    const bool spieltVorher = tonSpielt();
    const Statistik t = laufen(1);
    if (tonSpielt() && taktGesenkt) taktBeiTon = false;
    if (spieltVorher && tonSpielt() && t.laengster > tonPause) tonPause = t.laengster;   // Pause mitten im Ton
  }
  PRUEFE(!tonSpielt(), "Ton zu Ende gespielt");
  PRUEFE(taktBeiTon, "während des Tons voller Takt");
  PRUEFE(tonPause <= SCHLAF_TON_MS, "während des Tons höchstens 20 ms Pause (der Ton-Puffer reicht)");
  laufen(500);
  PRUEFE(i2sLaeuft, "Ton-Chip läuft kurz nach");
  laufen(2000);
  PRUEFE(!i2sLaeuft, "Ton-Chip nach dem Nachlauf wieder aus");
  PRUEFE(taktGesenkt, "danach wieder langsamer Takt");

  // Aufwachen mit einer Taste
  druecken(K_A);
  PRUEFE(bildschirmAn, "Taste weckt den Bildschirm");
  PRUEFE(!taktGesenkt, "wach: voller Takt");
  PRUEFE(!lcdSchlaeft && !SPI1.schlaeft && SPI1.anzeigeAn, "wach: Bildschirm-Chip wach (0x11, 0x29)");
  PRUEFE(!touchAus && simTouchRst == 1, "wach: Touch wieder an");
  PRUEFE(simLicht == 127, "wach: Helligkeit wieder 50 %");
  laufen(1000);
  PRUEFE(simFalschePunkte == 0, "wach: Bildschirm zeigt genau das Bild");

  // Zweimal RESET kurz hintereinander: der Bildschirm-Chip braucht 120 ms zwischen Schlafen und Aufwachen
  druecken(K_RESET);
  const uint32_t aus = lcdWechsel;
  simTaste(K_RESET, true); laufen(10); simTaste(K_RESET, false);
  PRUEFE(bildschirmAn && !lcdSchlaeft && SPI1.anzeigeAn, "schnell wieder an: Bildschirm-Chip wach");
  PRUEFE(lcdWechsel - aus >= 120, "zwischen Schlafen und Aufwachen mindestens 120 ms");
  laufen(1000);
  PRUEFE(simFalschePunkte == 0, "danach zeigt der Bildschirm genau das Bild");

  // Dunkel weckt Antippen nicht (Touch aus, Wunsch der Nutzerin vom 25.11.2026)
  druecken(K_RESET);
  laufen(5000);
  PRUEFE(taktGesenkt && lcdSchlaeft && touchAus, "wieder dunkel, langsam, Touch aus");
  simFinger(true, 160, 120);
  laufen(500);
  simFinger(false);
  laufen(500);
  PRUEFE(!bildschirmAn && taktGesenkt, "dunkel: Antippen weckt nicht");
  druecken(K_B);
  PRUEFE(bildschirmAn && !taktGesenkt && !lcdSchlaeft && !touchAus, "Taste weckt Bildschirm, Takt, Bildschirm-Chip und Touch");
  laufen(1000);

  // Aufs Gesicht legen und wieder umdrehen: geht auch mit dem seltenen Messen im Dunkeln
  for (int i = 0; i < 3; i++) { laufen(300, inDerHand); druecken(K_B); }   // Achsen lernen wie beim Bedienen
  PRUEFE(lage.nAchse >= 0, "Bildschirm-Achse gelernt");
  laufen(2000, inDerHand);
  auto gesichtUnten = [](uint32_t, float& x, float& y, float& z) { x = 0; y = 0; z = -1; };
  laufen(2500, gesichtUnten);
  PRUEFE(!bildschirmAn && bewegung.ausDurchLage, "aufs Gesicht gelegt: Bildschirm aus");
  s = laufen(20000, gesichtUnten);
  PRUEFE(s.geschlafen > 19600 && taktGesenkt, "liegt auf dem Gesicht: der Chip schläft");
  const uint32_t umgedrehtUm = simMillis;
  uint32_t anNach = 0;
  for (int i = 0; i < 300 && !bildschirmAn; i++) { laufen(10, inDerHand); anNach = simMillis - umgedrehtUm; }
  printf("   umgedreht: Bildschirm an nach %u ms\n", anNach);
  PRUEFE(bildschirmAn && anNach <= 1000, "umgedreht: Bildschirm geht innerhalb 1 s an");
  laufen(1000, inDerHand);
  PRUEFE(simFalschePunkte == 0, "danach zeigt der Bildschirm genau das Bild");

  printf("Display: %ld Bilder, %ld falsche Punkte\n", simBilder, simFalschePunkte);
  if (fehler) { printf("NICHT OK: %d Fehler\n", fehler); return 1; }
  printf("OK: 0 Fehler\n");
  return 0;
}
