// Prüft, was Evi bei Bewegung tut (lage.h, bewegung.ino), „Wie war dein Tag?“ und die Spiele
// Kekse fangen und Schnick, Schnack, Schnuck. Der Lagesensor ist nachgebildet (ersatz/Wire.h: simLage).
// Gerät hier: Bildschirm-Achse z, Oberkante y, rechte Kante x (das Programm lernt das selbst).
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

void foto(const std::string& name) {
  std::string p = "bilder/" + name + ".pbm";
  FILE* f = fopen(p.c_str(), "wb");
  fprintf(f, "P4\n320 240\n");
  const uint8_t* b = bild.getBuffer();
  for (int i = 0; i < 40 * 240; i++) { uint8_t v = ~b[i]; fwrite(&v, 1, 1, f); }
  fclose(f);
}
// Lage: Funktion der Zeit seit Beginn (ms) -> x, y, z in g
using Bewegung = std::function<void(uint32_t, float&, float&, float&)>;
void laufen(uint32_t ms, Bewegung b = nullptr) {
  for (uint32_t t = 0; t < ms; t += 10) {
    if (b) { float x, y, z; b(t, x, y, z); simLage(x, y, z); }
    simMillis += 10;
    loop();
  }
}
// In der Hand, schräg (45°), ruhig
void inDerHand(uint32_t, float& x, float& y, float& z) { x = 0; y = 0.71f; z = 0.71f; }
void druecken(int k) { simTaste(k, true); laufen(60); simTaste(k, false); laufen(60); }
void vorbereiten(int stunde, int minute) {
  const ms_t utc = ausOrtszeit(2026, 11, 24, stunde, minute);
  uhrSetzen(utc);
  pet = neuesTier(utc - 4 * TAG);
  pet.aktualisiert = utc;
  pet.satt = pet.laune = pet.sauber = 80;
  pet.muenzen = 5;
  himmel = himmelBei(utc);
  spiel = SpielZustand();
  momentAufraeumen(); geher.x = 0; geher.flip = false;
  moment.naechster = utc + 30 * MINUTE;
  moment.ruheBisTakt = 0; moment.letzterBesonderer = utc;
  menueOffen = false; ritual.aktiv = false;
  bildschirmAn = true; letzteBeruehrung = millis();
  evi.reaktion = R_KEINE; evi.reaktionBisTakt = 0;
}

int main() {
  bildschirmStarten(); bild.cp437(true);
  PRUEFE(tastenStarten(), "Tasten bereit");
  PRUEFE(lageStarten(), "Lagesensor gefunden");
  PRUEFE(!lage.grossEndian, "Reihenfolge der Bytes richtig erkannt (niedriges Byte zuerst)");
  vorbereiten(12, 0);
  laufen(500, inDerHand);
  PRUEFE(lage.nAchse < 0 && lageKippen() == 0, "ohne Bedienen: Achsen noch unbekannt, kein Kippen");
  // Bedienen lehrt die Achsen: dreimal B (schließt nur, ändert nichts)
  for (int i = 0; i < 3; i++) { simTaste(K_B, true); laufen(60, inDerHand); simTaste(K_B, false); laufen(300, inDerHand); }
  PRUEFE(lage.nAchse == 2 && lage.nVz == 1, "Bildschirm-Achse gelernt (z nach oben)");
  PRUEFE(lage.uAchse == 1 && lage.uVz == 1, "Oberkante gelernt (y)");
  printf("   Neigung in der Hand: %.2f\n", lageKippen());
  PRUEFE(fabsf(lageKippen()) < 0.05f, "gerade gehalten: keine Neigung");

  // ---------- Gassi gehen ----------
  // Schritte mit 1,8 pro Sekunde: die Kraft schwingt um 1 g (±0,35 g)
  const uint32_t munzenVorher = pet.muenzen;
  const double launeVorher = pet.laune, sattVorher = pet.satt;
  auto gehen = [](uint32_t t, float& x, float& y, float& z) {
    const float w = 1 + 0.35f * sinf(t / 1000.0f * 2 * 3.14159f * 1.8f);
    x = 0; y = 0.71f * w; z = 0.71f * w;
  };
  laufen(7000, gehen);
  PRUEFE(lage.gassi, "Gehen erkannt");
  PRUEFE(moment.art == M_GASSI, "Evi trabt mit");
  laufen(2000, gehen);
  char zeile[100];
  stimmungsText(beschreiben(pet, jetztMs()), jetztMs(), zeile, sizeof(zeile));
  printf("   Zeile: %s\n", zeile);
  PRUEFE(strstr(zeile, "Gassi gehen") != nullptr, "Zeile zeigt die Schritte");
  zeichneAlles(); foto("gassi");
  laufen(240000, gehen);   // 4 Minuten Spaziergang
  printf("   Schritte bisher: %lu (erwartet etwa %d)\n", (unsigned long)lage.gassiSchritte, (int)(249 * 1.8));
  PRUEFE(lage.gassiSchritte > 420 && lage.gassiSchritte < 460, "Schritte richtig gezählt");
  laufen(5000, inDerHand);
  PRUEFE(moment.art != M_GASSI, "nach dem Gehen trabt sie nicht mehr");
  laufen(60000, inDerHand);
  printf("   Laune %.1f -> %.1f, Münzen %lu -> %lu, Satt %.1f -> %.1f\n", launeVorher, pet.laune,
         (unsigned long)munzenVorher, (unsigned long)pet.muenzen, sattVorher, pet.satt);
  printf("   Meldung: %s\n", meldung);
  PRUEFE(!lage.gassi, "Spaziergang ist zu Ende");
  PRUEFE(pet.muenzen == munzenVorher + bewegung.schritteHeute / 500, "1 Münze je 500 Schritte");
  PRUEFE(pet.laune > launeVorher + 5, "Evi freut sich nach dem Spaziergang");
  PRUEFE(strstr(meldung, "Spaziergang") != nullptr, "Meldung zum Spaziergang");
  PRUEFE(bewegung.schritteHeute > 400, "Schritte von heute gemerkt");
  // Ruhig liegend (kleines Zittern) zählt nichts
  laufen(60000, [](uint32_t t, float& x, float& y, float& z) { x = 0.01f * sinf(t * 0.37f); y = 0.71f; z = 0.71f + 0.02f * sinf(t * 0.11f); });
  PRUEFE(!lage.gassi, "Zittern ist kein Gehen");

  // ---------- Was am Board zu viel auslöste (24.11.2026) ----------
  // In der Hand stillhalten: die Hand zittert und bewegt sich etwas (zufällig bis ±0,12 g, kleine Ruckler)
  vorbereiten(12, 10);
  srand(7);
  auto hand = [](uint32_t t, float& x, float& y, float& z) {
    auto z1 = []() { return (rand() % 2001 - 1000) / 1000.0f; };
    const float ruck = (t % 1700 < 60) ? 0.25f : 0;
    x = 0.06f * z1() + 0.05f * sinf(t * 0.004f); y = 0.71f + 0.08f * z1() + ruck; z = 0.71f + 0.08f * z1() - ruck;
  };
  const uint32_t schritteVorher = bewegung.schritteHeute;
  laufen(120000, hand);
  PRUEFE(!lage.gassi && bewegung.schritteHeute == schritteVorher, "in der Hand stillgehalten: keine Schritte");
  PRUEFE(moment.art == M_KEINER, "in der Hand stillgehalten: Evi tut nichts Besonderes");
  // Nach links und rechts kippen, immer wieder (je 1,5 s)
  laufen(60000, [](uint32_t t, float& x, float& y, float& z) {
    const float w = 0.6f * sinf(t / 1500.0f * 3.14159f);
    x = sinf(w) * 0.71f; y = 0.71f; z = cosf(w) * 0.71f; });
  PRUEFE(!lage.gassi && bewegung.schritteHeute == schritteVorher, "hin und her gekippt: keine Schritte");
  laufen(3000, inDerHand);
  momentStoppen(true);
  // Auf den Tisch legen: hinunter bewegen, kurz aufsetzen (Stoß mit Nachschwingen), dann still flach
  auto ablegen = [](uint32_t t, float& x, float& y, float& z) {
    if (t < 800) { const float a = t / 800.0f; x = 0; y = 0.71f * (1 - a); z = 0.71f + 0.29f * a; z += 0.3f * sinf(a * 3.14159f); return; }
    const float s = (t - 800) / 1000.0f;
    x = 0; y = 0; z = 1 + 2.5f * expf(-s / 0.02f) * sinf(s * 2 * 3.14159f * 40);
  };
  for (int i = 0; i < 5; i++) { laufen(4000, ablegen); laufen(2000, inDerHand); }
  PRUEFE(moment.art == M_KEINER, "fünfmal auf den Tisch gelegt: Evi tut nichts Besonderes");
  PRUEFE(!lage.gassi && bewegung.schritteHeute == schritteVorher, "auf den Tisch gelegt: keine Schritte");
  // Wackeln und Schütteln: seit 24.11.2026 ohne Reaktion (Wunsch der Nutzerin), und es zählt nicht als Gehen
  laufen(2000, inDerHand);
  const char* meldungVorher = meldung;
  laufen(2000, [](uint32_t t, float& x, float& y, float& z) { x = 1.0f * sinf(t / 1000.0f * 2 * 3.14159f * 3); y = 0.71f; z = 0.71f; });
  laufen(1200, [](uint32_t t, float& x, float& y, float& z) { x = 2.6f * sinf(t / 1000.0f * 2 * 3.14159f * 4); y = 0.71f; z = 0.71f; });
  PRUEFE(moment.art == M_KEINER && !lage.gassi && geher.x == 0 && !strstr(meldung, "dreht"), "gewackelt und geschüttelt: Evi tut nichts");
  (void)meldungVorher;
  laufen(10000, inDerHand);
  momentStoppen(true);
  // Sensor misst im doppelten Maßstab (anders eingestellt als erwartet): nach kurzer Ruhe stimmt es wieder
  laufen(8000, [](uint32_t, float& x, float& y, float& z) { x = 0; y = 1.42f; z = 1.42f; });
  printf("   gelernter Maßstab: 1 g = %.2f\n", lage.eins);
  PRUEFE(fabsf(lage.eins - 2) < 0.1f, "anderer Maßstab: beim Stillhalten gelernt");
  laufen(120000, [&hand](uint32_t t, float& x, float& y, float& z) { hand(t, x, y, z); x *= 2; y *= 2; z *= 2; });
  PRUEFE(!lage.gassi, "anderer Maßstab: Stillhalten gibt keine Schritte");
  laufen(30000, [](uint32_t t, float& x, float& y, float& z) {
    const float w = 1 + 0.35f * sinf(t / 1000.0f * 2 * 3.14159f * 1.8f); x = 0; y = 1.42f * w; z = 1.42f * w; });
  PRUEFE(lage.gassi, "anderer Maßstab: Gehen wird erkannt");
  laufen(70000, [](uint32_t, float& x, float& y, float& z) { x = 0; y = 1.42f; z = 1.42f; });
  laufen(3000, inDerHand);

  // ---------- Kippen ----------
  vorbereiten(13, 0);
  laufen(3000, inDerHand);
  // rechte Kante tiefer (etwa 30°): die Kraft bekommt einen Anteil nach links (-x)
  auto rechtsTief = [](uint32_t, float& x, float& y, float& z) { x = -0.5f; y = 0.61f; z = 0.61f; };
  laufen(1500, rechtsTief);
  PRUEFE(moment.art == M_ROLLEN, "gekippt: der Ball rollt");
  laufen(1000, rechtsTief);
  zeichneAlles(); foto("rollen");
  laufen(500, rechtsTief);
  printf("   Ball bei x=%.0f, Evi bei %d\n", rollBall.x, BODEN_X + geher.x);
  PRUEFE(rollBall.sichtbar && rollBall.x > BODEN_X + 60, "Ball rollt nach rechts (zur tieferen Seite)");
  laufen(3000, rechtsTief);
  PRUEFE(geher.x > 40, "Evi rennt hinterher");
  auto linksTief = [](uint32_t, float& x, float& y, float& z) { x = 0.5f; y = 0.61f; z = 0.61f; };
  laufen(5000, linksTief);
  printf("   Ball bei x=%.0f, Evi bei %d\n", rollBall.x, BODEN_X + geher.x);
  PRUEFE(rollBall.x < BODEN_X - 40, "andersherum gekippt: Ball rollt nach links");
  laufen(25000, inDerHand);
  PRUEFE(moment.art == M_KEINER && !rollBall.sichtbar && geher.x == 0, "gerade gelegt: Evi holt den Ball und geht zurück");

  // ---------- Aufs Gesicht ----------
  vorbereiten(14, 0);
  laufen(2000, inDerHand);
  auto gesichtUnten = [](uint32_t, float& x, float& y, float& z) { x = 0; y = 0; z = -1; };
  laufen(2500, gesichtUnten);
  PRUEFE(!bildschirmAn && bewegung.ausDurchLage, "aufs Gesicht gelegt: Bildschirm aus");
  laufen(30000, gesichtUnten);
  laufen(800, inDerHand);
  PRUEFE(bildschirmAn, "umgedreht: Bildschirm wieder an");
  PRUEFE(moment.art == M_NICKERCHEN, "nach 30 s: Evi gähnt nach dem Nickerchen");
  laufen(5000, inDerHand);
  // Nachts macht Hinlegen das Licht aus
  vorbereiten(23, 30);
  pet.lichtAus = false;
  laufen(2000, inDerHand);
  const double launeNacht = pet.laune;
  laufen(2500, gesichtUnten);
  PRUEFE(!bildschirmAn && pet.lichtAus, "nachts hingelegt: Licht aus");
  PRUEFE(pet.laune > launeNacht + 5, "mit Lichtbonus");
  laufen(5 * 60000, gesichtUnten);
  laufen(800, inDerHand);
  PRUEFE(bildschirmAn && pet.lichtAus, "umgedreht: Bildschirm an, Licht bleibt aus");
  // Mit RESET ausgeschaltet: Umdrehen macht ihn nicht an
  vorbereiten(14, 30);
  laufen(2000, inDerHand);
  druecken(K_RESET);
  laufen(2500, gesichtUnten);
  laufen(800, inDerHand);
  PRUEFE(!bildschirmAn, "mit RESET aus: Umdrehen lässt ihn aus");
  druecken(K_A);

  // ---------- Wie war dein Tag? ----------
  vorbereiten(19, 0);
  laufen(500, inDerHand);
  bewegung.tagGefragt = bewegung.tagBeantwortet = -1;
  moment.naechster = jetztMs() + SEKUNDE;
  laufen(3000, inDerHand);
  PRUEFE(menueOffen && tagAuswahl, "abends fragt Evi von selbst: Wie war dein Tag?");
  zeichneAlles(); foto("tag-frage");
  druecken(K_B);
  PRUEFE(!menueOffen && bewegung.tagBeantwortet == -1, "ohne Antwort geschlossen");
  PRUEFE(strstr(sagen.text, "später") != nullptr, "Evi: Erzählst du es mir später?");
  laufen(4000, inDerHand);
  druecken(K_A);   // Menü auf
  bool imMenue = false;
  for (int i = 0; i < menueAnzahl; i++) if (menue[i].e == E_TAG) { imMenue = true; gewaehlt = i; }
  PRUEFE(imMenue, "Frage steht im Menü");
  druecken(K_A);
  PRUEFE(menueOffen && tagAuswahl, "aus dem Menü gefragt");
  druecken(K_DOWN); druecken(K_DOWN); druecken(K_A);   // „Nicht so gut“
  PRUEFE(moment.art == M_TROESTEN && bewegung.tagBeantwortet == tagKennung(jetztMs()), "Nicht so gut: Evi tröstet dich");
  laufen(5000, inDerHand);
  zeichneAlles(); foto("troesten");
  laufen(20000, inDerHand);
  PRUEFE(moment.art == M_KEINER, "Trösten vorbei");
  druecken(K_A);
  imMenue = false;
  for (int i = 0; i < menueAnzahl; i++) if (menue[i].e == E_TAG) imMenue = true;
  PRUEFE(!imMenue, "nach der Antwort nicht mehr im Menü");
  druecken(K_B);
  vorbereiten(12, 0);
  druecken(K_A);
  imMenue = false;
  for (int i = 0; i < menueAnzahl; i++) if (menue[i].e == E_TAG) imMenue = true;
  PRUEFE(!imMenue, "mittags nicht im Menü");
  druecken(K_B);

  // ---------- Kekse fangen ----------
  vorbereiten(15, 0);
  laufen(1000, inDerHand);
  menueZeigen(true, true);
  zeichneAlles(); foto("spiele-menue");
  menueZeigen(false);
  srand(3);                 // fester Zufall: die Kekse fallen immer gleich
  spielStarten(SPIEL_KEKSE);
  laufen(1200, inDerHand);
  zeichneAlles(); foto("kekse");
  // Mit Kippen zum Keks laufen: rechts tief, wenn der Keks rechts ist
  int runden = 0;
  for (int i = 0; i < 6000 && spiel.aktiv; i++) {
    const float ziel = spiel.kx - BODEN_X - spiel.eviGenau;
    const float neigung = ziel > 6 ? 0.8f : ziel < -6 ? -0.8f : 0;
    laufen(10, [neigung](uint32_t, float& x, float& y, float& z) { x = -neigung; y = 0.42f; z = 0.42f; });
    runden = spiel.runde;
  }
  printf("   Meldung: %s\n", meldung);
  PRUEFE(!spiel.aktiv && (strstr(meldung, "5 von 5 Kekse") || strstr(meldung, "4 von 5 Kekse")), "Kekse fangen mit Kippen: mindestens 4 von 5 gefangen");
  vorbereiten(15, 10);
  spielStarten(SPIEL_KEKSE);
  laufen(30000, inDerHand);
  printf("   Meldung ohne Laufen: %s\n", meldung);
  PRUEFE(!spiel.aktiv && strstr(meldung, "Kekse gefangen") != nullptr, "Kekse fangen ohne Laufen endet nach 5 Keksen");
  vorbereiten(15, 20);
  spielStarten(SPIEL_KEKSE);
  const int vorX = spiel.eviX;
  druecken(K_R); druecken(K_R);
  PRUEFE(spiel.eviX == vorX + 52 && geher.flip, "▶ läuft nach rechts (gespiegelt)");
  druecken(K_B);
  PRUEFE(!spiel.aktiv && !geher.flip, "B beendet, Evi schaut wieder normal");

  // ---------- Schnick, Schnack, Schnuck ----------
  vorbereiten(16, 0);
  spielStarten(SPIEL_SCHNICK);
  laufen(300, inDerHand);
  zeichneAlles(); foto("schnick-wahl");
  druecken(K_L);   // Schere
  laufen(1400, inDerHand);
  zeichneAlles(); foto("schnick-zeigen");
  printf("   %s\n", spiel.notiz);
  PRUEFE(spiel.evis >= 0, "Evi zeigt ihre Wahl");
  for (int i = 0; i < 40 && spiel.aktiv; i++) { laufen(2200, inDerHand); if (spiel.aktiv) druecken(i % 3 == 0 ? K_L : i % 3 == 1 ? K_A : K_R); }
  laufen(3000, inDerHand);
  printf("   Meldung: %s\n", meldung);
  PRUEFE(!spiel.aktiv && strstr(meldung, "gewonnen") != nullptr, "Schnick, Schnack, Schnuck endet nach 5 Runden");
  // Alle drei Zeichen in allen Größen ansehen
  bild.fillScreen(1);
  for (int i = 0; i < 5; i++) {
    zeichneZeichen(i, 30 + i * 60, 40, 13, 0, 1);
    bild.fillRect(14 + i * 60, 80, 34, 34, 0);
    zeichneZeichen(i, 30 + i * 60, 97, 13, 1, 0);
    zeichneZeichen(i, 30 + i * 60, 140, 6, 0, 1);
    bild.fillRect(22 + i * 60, 160, 16, 16, 0);
    zeichneZeichen(i, 30 + i * 60, 168, 6, 1, 0);
  }
  foto("zeichen");
  printf("Display: %ld Bilder verglichen, %ld falsche Punkte\n", simBilder, simFalschePunkte);
  PRUEFE(simFalschePunkte == 0, "Display zeigt genau das Bild");
  printf("%s: %d Fehler\n", fehler ? "FEHLER" : "OK", fehler);
  return fehler ? 1 : 0;
}
