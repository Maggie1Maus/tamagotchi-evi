// Prüft den Schlafmodus (schlaf.h, seit 25.11.2026): Helligkeit 50 %, Chip schläft zwischen den Abfragen,
// langsamer Takt im Dunkeln, Bildschirm-Chip schläft, Ton-Chip nur bei Tönen an, Gassi zählt im Dunkeln weiter.
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
  const uint8_t* b = bild.getBuffer();
  for (int y = 0; y < 240; y++) for (int x = 0; x < 320; x++)
    if (simDisplay[y][x] != ((b[y * 40 + x / 8] >> (7 - x % 8)) & 1)) simFalschePunkte++;
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
  s = laufen(10000, inDerHand);
  printf("   dunkel: %u Runden in 10 s, geschlafen %u ms, längste Pause %u ms, Messungen %u\n", s.runden, s.geschlafen, s.laengster,
         s.messungen);
  PRUEFE(s.laengster <= L::ABTAST_MS, "dunkel: Pausen höchstens bis zur nächsten Messung (20 ms)");
  PRUEFE(s.geschlafen > 9000, "dunkel: der Chip schläft über 90 % der Zeit");
  PRUEFE(s.runden < 600, "dunkel: nur etwa 50 Runden pro Sekunde");
  PRUEFE(s.messungen >= 450, "dunkel: Lagesensor misst weiter etwa 50-mal pro Sekunde");
  PRUEFE(taktGesenkt, "dunkel: nach 3 s langsamer Takt");
  PRUEFE(s.taktGesenktRunden > s.runden / 2, "dunkel: die meiste Zeit langsamer Takt");
  PRUEFE(!bildschirmAn && lcdSchlaeft, "bleibt dunkel");

  // Ein Ton im Dunkeln: voller Takt, Ton-Chip an; danach wieder langsam, Ton-Chip nach dem Nachlauf aus
  const int startsVorher = i2s.starts;
  piep(TON(TON_WUNSCH));
  PRUEFE(!taktGesenkt && i2sLaeuft && i2s.starts == startsVorher + 1, "Ton: voller Takt, Ton-Chip an");
  bool taktBeiTon = true;
  for (int i = 0; i < 400 && tonSpielt(); i++) { laufen(1); if (tonSpielt() && taktGesenkt) taktBeiTon = false; }
  PRUEFE(!tonSpielt(), "Ton zu Ende gespielt");
  PRUEFE(taktBeiTon, "während des Tons voller Takt");
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

  // Aufwachen durch Antippen
  druecken(K_RESET);
  laufen(5000);
  PRUEFE(taktGesenkt && lcdSchlaeft, "wieder dunkel und langsam");
  simFinger(true, 160, 120);
  laufen(30);
  simFinger(false);
  PRUEFE(bildschirmAn && !taktGesenkt && !lcdSchlaeft, "Antippen weckt Bildschirm, Takt und Bildschirm-Chip");
  laufen(1000);

  // Gassi im Dunkeln: Schritte zählen weiter
  druecken(K_RESET);
  const uint32_t schritteVorher = bewegung.schritteHeute;
  uint32_t phase = 0;
  auto gehen = [&](uint32_t t, float& x, float& y, float& z) {
    const float w = 1 + 0.35f * sinf((t + phase) / 1000.0f * 2 * 3.14159f * 1.8f);
    x = 0; y = 0.71f * w; z = 0.71f * w;
  };
  s = laufen(20000, gehen);
  printf("   Gassi im Dunkeln: %u Schritte, geschlafen %u von 20000 ms, Bildschirm %s, Moment %d\n", bewegung.schritteHeute - schritteVorher,
         s.geschlafen, bildschirmAn ? "an" : "aus", (int)moment.art);
  PRUEFE(bewegung.schritteHeute - schritteVorher >= 25, "Gassi im Dunkeln: Schritte werden gezählt");
  PRUEFE(s.geschlafen > 15000, "Gassi im Dunkeln: der Chip schläft trotzdem meistens");

  printf("Display: %ld Bilder, %ld falsche Punkte\n", simBilder, simFalschePunkte);
  if (fehler) { printf("NICHT OK: %d Fehler\n", fehler); return 1; }
  printf("OK: 0 Fehler\n");
  return 0;
}
