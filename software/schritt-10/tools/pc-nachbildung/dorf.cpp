// Das Dorf: Weg im Park, Auswahl, „nicht zu Hause“, Besuch im Haus (26.11.2026)
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
void nachbildungBildFertig() { simMillis += 5; }
#include "EEPROM.h"
EEPROMC EEPROM;
#include "evi-handy.ino"
int fehler = 0;
#define PRUEFE(b, t) do { if (!(b)) { printf("FEHLER: %s\n", t); fehler++; } else printf("ok: %s\n", t); } while (0)
const ms_t HEUTE = ausOrtszeit(2026, 6, 10, 12, 0);      // Sommer, kein Weihnachten
void laufen(uint32_t ms) { for (uint32_t t = 0; t < ms; t += 10) { simMillis += 10; loop(); } }
ms_t geburtFuer(int wesen) { for (int i = 0; i < 100000; i++) { const ms_t g = HEUTE - 3 * TAG - 1000 * (ms_t)i; if (wesenVon(g) == wesen) return g; } return 0; }
void foto(const char* name) { FILE* f = fopen(name, "wb"); fprintf(f, "P5\n320 240\n255\n"); for (int y = 0; y < 240; y++) for (int x = 0; x < 320; x++) fputc(bild.stufe(x, y) * (bild.stufe(x, y) <= 15 ? 17 : 1), f); fclose(f); }
int zeichen(const char* s) { int n = 0; for (; *s; s++) if ((*s & 0xC0) != 0x80) n++; return n; }
// Eine Zeit, zu der Figur f im Park steht (parkBesucher), und eine, zu der sie zu Hause ist
ms_t zeitMitBesucher(int f, bool imPark) {
  for (int t = 0; t < 4000; t++) { const ms_t z = HEUTE + t * 4 * STUNDE + 1; if ((parkBesucher(z) == f) == imPark) return z; }
  return 0;
}

int main() {
  bildschirmStarten(); bild.cp437(true);
  uhrSetzen(HEUTE);
  pet = neuesTier(HEUTE - 3 * TAG, ERSTES_EI);
  pet.geboren = geburtFuer(W_NASCH); pet.schluepft = HEUTE - 3 * TAG; pet.aktualisiert = HEUTE; pet.satt = pet.laune = pet.sauber = 90;
  himmel = himmelBei(HEUTE); moment.naechster = HEUTE + 10 * MINUTE; moment.letzterBesonderer = HEUTE;
  freundLaden(); parkFreundLaden(); stammLaden(); parkFreund.evi = pet.geboren;
  tastenStarten();
  bildschirmAn = true; bildschirmEin();

  // ---- Texte passen in die Zeile (mit dem längsten Namen) ----
  {
    bool gut = true;
    for (int i = 0; i < 8; i++) { char t[80]; snprintf(t, sizeof(t), NICHT_ZUHAUSE[i], "Glöckchen"); if (zeichen(t) > 50) { gut = false; printf("  zu lang: %s\n", t); } }
    for (int i = 0; i < 3; i++) if (zeichen(KENN_ICH_NICHT[i]) > 50) gut = false;
    for (int i = 0; i < 5; i++) if (zeichen(GRUSS_HAUS[i]) > 38) { gut = false; printf("  zu lang: %s\n", GRUSS_HAUS[i]); }
    PRUEFE(gut, "Dorf-Meldungen und Hausgrüße passen in die Zeile");
  }

  // ---- Weg zum Dorf ----
  parkFreund.treffer[8] = 2; parkFreund.treffer[13] = 7; parkFreund.treffer[4] = 3;   // Bekannte: Kiwi-Figur, Mampf, Ella
  parkOeffnen(HEUTE);
  PRUEFE(parkOffen() && park.phase == PARK_WAHL, "Park offen");
  zeichneAlles();
  foto("dorf-park.pgm");
  // Antippen der Häuser (Parkkoordinaten 150/70 → Bildschirm y + 27)
  beruehrt(160, 27 + 64);
  PRUEFE(park.phase == PARK_LAEUFT && park.ziel == PARK_ZIEL_DORF, "Antippen der Häuser: Tamagotchi läuft zum Dorf");
  laufen(6000);
  PRUEFE(park.phase == PARK_DORF, "angekommen: Auswahl im Dorf");
  zeichneAlles(); foto("dorf-auswahl.pgm");

  // ---- Unbekannte kann man nicht besuchen ----
  albumWahl = 5; knopf(K_A);
  PRUEFE(park.phase == PARK_DORF && park.meldung == 2, "Unbekannte: „kennst du noch nicht“");
  zeichneAlles();
  // ---- Wer im Park steht, ist nicht zu Hause ----
  const ms_t imPark = zeitMitBesucher(13, true), zuHause = zeitMitBesucher(13, false);
  PRUEFE(imPark && zuHause, "Testzeiten gefunden");
  uhrSetzen(imPark); pet.aktualisiert = imPark; himmel = himmelBei(imPark);
  albumWahl = 13; knopf(K_A);
  PRUEFE(park.phase == PARK_DORF && park.meldung == 1 && parkFreund.treffer[13] == 7, "Mampf steht im Park: nicht zu Hause, kein Treffen gezählt");
  zeichneAlles();
  // Alle 8 Meldungen kommen vor
  { bool vor[8] = {}; for (int i = 0; i < 200; i++) { park.meldung = 0; knopf(K_A); vor[park.meldungNr % 8] = true; }
    bool alle = true; for (int i = 0; i < 8; i++) if (!vor[i]) alle = false;
    PRUEFE(alle, "alle acht „nicht zu Hause“-Meldungen kommen vor"); }
  // ---- Besuch ----
  uhrSetzen(zuHause); pet.aktualisiert = zuHause; himmel = himmelBei(zuHause);
  parkFreund.heuteTag = 0;
  const int vor = parkFreund.treffer[13];
  albumWahl = 13; knopf(K_A);
  PRUEFE(park.phase == PARK_BESUCHT && park.besuch == 13, "Besuch im Haus");
  PRUEFE(parkFreund.treffer[13] == vor + 1, "der Besuch zählt als Treffen");
  zeichneAlles(); foto("dorf-haus.pgm");
  PRUEFE(!dorfHaus, "Weihnachtsdeko-Schalter ist nach dem Zeichnen wieder aus");
  // A: Begrüßung → zweiter Satz → (Geschenk) → plaudern
  knopf(K_A); const int z1 = park.zweiter;
  bool geschenkGesehen = false;
  if (park.geschenk) { knopf(K_A); geschenkGesehen = park.zweiter == 2; }
  knopf(K_A);
  PRUEFE(z1 == 1 && (!park.geschenk || geschenkGesehen) && park.zweiter == 3 && park.phase == PARK_BESUCHT, "A: Begrüßung, zweiter Satz, dann plaudern");
  { int unterschiedlich = 0; uint16_t letzter = 0xFFFF; for (int i = 0; i < 20; i++) { const uint16_t k = park.nachArt * 256 + park.nachSatz; if (k != letzter) unterschiedlich++; letzter = k; knopf(K_A); }
    PRUEFE(unterschiedlich == 20, "beim Plaudern nie zweimal derselbe Satz hintereinander"); }
  knopf(K_B);
  PRUEFE(park.phase == PARK_DORF, "B: zurück ins Dorf");
  // Mehrere Besuche bringen mehr Freundschaft; Geschenke höchstens einmal am Tag
  {
    const int v0 = parkFreund.treffer[13];
    int geschenke = 0;
    parkFreund.treffer[13] = 7; stamm.geschenkMaske[1] = 0; stamm.geschenkMaske[0] = 0; stamm.geschenkMaske[2] = 0;
    for (int i = 0; i < 40; i++) {
      albumWahl = 13; knopf(K_A);
      if (park.geschenk) geschenke++;
      knopf(K_B);
    }
    PRUEFE(parkFreund.treffer[13] == 47 && v0 > 0, "40 Besuche am selben Tag: 40 Treffen mehr (mehr Freundschaft)");
    PRUEFE(geschenke <= 1, "Beste Freunde schenken am selben Tag höchstens einmal");
    uhrSetzen(zuHause + 2 * TAG); pet.aktualisiert = zuHause + 2 * TAG;
    int geschenke2 = 0; for (int i = 0; i < 60 && !geschenke2; i++) { albumWahl = 13; if (!dorfZuHause(13, jetztMs())) { uhrSetzen(zuHause + 2 * TAG + 4 * STUNDE); pet.aktualisiert = jetztMs(); } knopf(K_A); if (park.geschenk) geschenke2++; if (park.phase == PARK_BESUCHT) knopf(K_B); stamm.geschenkMaske[1] = 0; stamm.geschenkMaske[0] = 0; stamm.geschenkMaske[2] = 0; }
    PRUEFE(geschenke2 > 0, "an einem anderen Tag gibt es wieder Geschenke");
    uhrSetzen(zuHause); pet.aktualisiert = zuHause;
  }
  // Geschenk wird gemeldet, wenn man das Haus vorher verlässt
  {
    bool ok = false;
    for (int i = 0; i < 120 && !ok; i++) {
      parkFreund.treffer[13] = 7; stamm.geschenkMaske[1] = 0; stamm.geschenkMaske[0] = 0; stamm.geschenkMaske[2] = 0;
      albumWahl = 13; park.phase = PARK_DORF; knopf(K_A);
      if (park.phase == PARK_BESUCHT && park.geschenk) { meldung[0] = 0; knopf(K_B); ok = meldung[0] != 0; }
      else if (park.phase == PARK_BESUCHT) knopf(K_B);
    }
    PRUEFE(ok, "Haus verlassen, bevor man das Geschenk sah: Meldung");
  }
  // ---- Keine Weihnachtsdeko im Haus ----
  {
    const ms_t advent = ausOrtszeit(2026, 12, 15, 12, 0);
    dorfHaus = false; const bool aussen = weihnachtsZeit(advent);
    dorfHaus = true; const bool innen = weihnachtsZeit(advent);
    dorfHaus = false;
    PRUEFE(aussen && !innen, "im Haus eines Freundes gibt es keine Weihnachtsdeko, sonst schon");
    uhrSetzen(advent); pet.aktualisiert = advent; himmel = himmelBei(advent);
    park.phase = PARK_DORF; parkFreund.treffer[4] = 3;
    const ms_t z = advent; (void)z;
    albumWahl = 4; for (int t = 0; t < 30 && !dorfZuHause(4, jetztMs()); t++) { uhrSetzen(jetztMs() + 4 * STUNDE); pet.aktualisiert = jetztMs(); }
    knopf(K_A); zeichneAlles(); foto("dorf-haus-advent.pgm");
    PRUEFE(park.phase == PARK_BESUCHT, "Besuch im Advent");
    knopf(K_B);
    uhrSetzen(zuHause); pet.aktualisiert = zuHause; himmel = himmelBei(zuHause);
  }
  // ---- Zurück und nach Hause ----
  knopf(K_B); PRUEFE(park.phase == PARK_WAHL, "B im Dorf: zurück zum Park");
  knopf(K_B); PRUEFE(!parkOffen(), "B im Park: nach Hause");
  // Touch im Dorf
  parkOeffnen(jetztMs()); beruehrt(160, 27 + 64); laufen(6000);
  PRUEFE(park.phase == PARK_DORF, "zweiter Weg zum Dorf");
  beruehrt(2 * 64 + 10, 45 + 39 + 10);   // Figur 7 (Spalte 2, Zeile 1)
  PRUEFE(albumWahl == 7, "Antippen wählt eine Figur");
  beruehrt(280, ZEILE_Y + 10);
  PRUEFE(park.phase == PARK_WAHL, "Tipper unten rechts: zurück");
  // Schlafen schließt alles
  bildschirmAus(); PRUEFE(!parkOffen(), "Bildschirm aus schließt das Dorf");
  printf(fehler ? "%d FEHLER\n" : "Alles in Ordnung.\n", fehler);
  return fehler ? 1 : 0;
}
