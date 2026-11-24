#include "Arduino.h"
#include <stdexcept>
#include <string>
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
  const uint8_t* b = bild.getBuffer();
  for (int y = 0; y < 240; y++) for (int x = 0; x < 320; x++)
    if (simDisplay[y][x] != ((b[y * 40 + x / 8] >> (7 - x % 8)) & 1)) simFalschePunkte++;
}

static int fehler = 0;
#define PRUEFE(b, text) do { if (!(b)) { printf("FEHLER: %s\n", text); fehler++; } } while (0)

void foto(const std::string& name) {
  std::string p = "bilder/" + name + ".pbm";
  FILE* f = fopen(p.c_str(), "wb");
  fprintf(f, "P4\n320 240\n");
  const uint8_t* b = bild.getBuffer();
  for (int i = 0; i < 40 * 240; i++) { uint8_t v = ~b[i]; fwrite(&v, 1, 1, f); }
  fclose(f);
}
// Programm laufen lassen: ms Millisekunden in Schritten von 10 ms
void laufen(uint32_t ms) { for (uint32_t t = 0; t < ms; t += 10) { simMillis += 10; loop(); } }
void zeichnenJetzt() { zeichneAlles(); }

void vorbereiten(int stunde, int minute, ms_t seitGeburt = 4 * TAG) {
  simNachBild = nullptr;
  bild.cp437(true);
  const ms_t utc = ausOrtszeit(2026, 11, 23, stunde, minute);
  uhrSetzen(utc);
  pet = neuesTier(utc - seitGeburt, R::EI_DAUER);   // Standard: seit gut 3 Tagen geschlüpft, erwachsen
  pet.aktualisiert = utc;
  pet.satt = 90; pet.laune = 90; pet.sauber = 90;
  pet.muenzen = 12;
  himmel = himmelBei(utc);
  spiel = SpielZustand();
  momentAufraeumen(); geher.x = 0; geher.flip = false;
  moment.naechster = utc + 10 * MINUTE;
  moment.ruheBisTakt = 0; moment.letzterBesonderer = utc;   // kein Besuch von selbst
  menueOffen = false; ritual.aktiv = false;
  bildschirmAn = true; letzteBeruehrung = millis();
  evi.reaktion = R_KEINE; evi.reaktionBisTakt = 0;
  tastenDa = true;
}

int main() {
  bildschirmStarten();
  // Durch den Körper des laufenden Babys darf der dunkle Boden nicht durchscheinen,
  // weder nach links noch gespiegelt nach rechts. Außerhalb der Figur bleibt der Grund sichtbar.
  auto hell = [](int x, int y) { return (bild.getBuffer()[y * 40 + x / 8] & (0x80 >> (x % 8))) != 0; };
  for (bool spiegeln : {false, true}) {
    bild.fillScreen(0);
    if (spiegeln) zeichneBildGespiegelt(ART_BABY_LAUFEN, 10, 10);
    else zeichneBild(ART_BABY_LAUFEN, 10, 10);
    for (auto p : {std::pair<int, int>{55, 60}, {65, 65}, {50, 75}, {60, 70}, {73, 67}})
      PRUEFE(hell(10 + (spiegeln ? ART_BABY_LAUFEN.w - 1 - p.first : p.first), 10 + p.second), "Baby-Bauch deckend weiß");
    PRUEFE(!hell(10 + (spiegeln ? ART_BABY_LAUFEN.w - 1 : 0), 99), "außerhalb des Babys transparent");
  }
  // ---------- Zimmer und Menü ----------
  vorbereiten(14, 0);
  PRUEFE(phaseBei(pet, jetztMs()) == ERWACHSEN, "Evi ist nicht erwachsen");
  laufen(300); foto("01-zimmer");
  menueZeigen(true); zeichnenJetzt(); foto("02-menue");
  menueZeigen(false);

  // ---------- Ballfangen ----------
  vorbereiten(14, 0);
  spielStarten(SPIEL_BALL);
  laufen(900); foto("10-ball");
  {
    // Wie viel geht pro Bild an den Bildschirm, während der Ball läuft? (ganzes Bild: 153600 Bytes)
    const unsigned long vorher = simGesendet; int bilder = 0;
    simNachBild = [] {};
    for (int i = 0; i < 50; i++) { simMillis += 13; loop(); }
    bilder = 50;
    printf("Ball: im Mittel %lu Bytes pro Bild (ganzes Bild 153600)\n", (simGesendet - vorher) / bilder);
    PRUEFE((simGesendet - vorher) / bilder < 20000, "Ball sendet nur kleine Ausschnitte");
    simNachBild = nullptr;
  }
  int versuche = 0;
  while (spiel.aktiv && versuche < 50) {
    laufen(200);
    if (spiel.pause <= 0) { const float m = spiel.x + 7; if (m >= 150 && m <= 170) { spielTaste(ST_LINKS); versuche++; if (versuche == 1) { zeichnenJetzt(); foto("11-ball-gefangen"); } } }
  }
  laufen(1500);
  PRUEFE(!spiel.aktiv, "Ballspiel endet nicht");
  printf("Ball: %d Versuche, Münzen jetzt %u, Meldung: %s\n", versuche, pet.muenzen, meldung);
  PRUEFE(strstr(meldung, "5 von 5") != nullptr, "höchstens 5 Treffer");
  zeichnenJetzt(); foto("12-ball-ende");

  // ---------- Links oder rechts ----------
  vorbereiten(14, 0);
  spielStarten(SPIEL_RATEN);
  laufen(300); foto("20-raten");
  spielKnopf(K_L); laufen(300); foto("21-raten-sprung");
  for (int i = 0; i < 10 && spiel.aktiv; i++) { laufen(1500); spielKnopf(i % 2 ? K_L : K_R); }
  laufen(2500);
  PRUEFE(!spiel.aktiv, "Raten endet nicht");
  printf("Raten: Meldung: %s\n", meldung);

  // ---------- Melodie merken ----------
  vorbereiten(14, 0);
  spielStarten(SPIEL_MELODIE);
  laufen(800); foto("30-melodie-vorspielen");
  for (int runde = 0; runde < 5 && spiel.aktiv; runde++) {
    for (int w = 0; w < 100 && spiel.beschaeftigt; w++) laufen(100);
    if (runde == 0) { zeichnenJetzt(); foto("31-melodie-du"); }
    const int n = spiel.folgeLaenge;
    for (int i = 0; i < n && spiel.aktiv; i++) { spielTaste(spiel.folge[i]); laufen(250); }
    laufen(300);
  }
  laufen(2000);
  PRUEFE(!spiel.aktiv, "Melodie endet nicht");
  printf("Melodie: Meldung: %s\n", meldung);
  vorbereiten(14, 0);
  spielStarten(SPIEL_MELODIE);
  for (int w = 0; w < 100 && spiel.beschaeftigt; w++) laufen(100);
  spielTaste((spiel.folge[0] + 1) % 3); laufen(100); zeichnenJetzt(); foto("32-melodie-falsch");
  laufen(1500);
  PRUEFE(!spiel.aktiv, "falscher Ton beendet nicht");
  vorbereiten(14, 0);
  spielStarten(SPIEL_BALL); laufen(500); spielKnopf(K_B); laufen(200);
  PRUEFE(!spiel.aktiv && strstr(meldung, "abgebrochen"), "B bricht Spiel nicht ab");

  // ---------- Momente ----------
  struct { MomentArt art; Gedanke g; int h; const char* name; } momente[] = {
    {M_GEHEN, GD_KEINER, 14, "gehen"}, {M_FENSTER, GD_KEINER, 14, "fenster"}, {M_DENKEN, GD_KEKS, 14, "denken"},
    {M_SUMMEN, GD_KEINER, 14, "summen"}, {M_GAEHNEN, GD_KEINER, 14, "gaehnen"}, {M_DREHEN, GD_KEINER, 14, "drehen"},
    {M_FALTER, GD_KEINER, 14, "falter"}, {M_VOGEL, GD_KEINER, 14, "vogel"}, {M_STERN, GD_KEINER, 21, "stern"}};
  for (auto& mo : momente) {
    vorbereiten(mo.h, 0);
    momentStarten(mo.art, mo.g);
    int takte = 0, bilder = 0;
    while (moment.art != M_KEINER && takte < 2000) {
      simMillis += TAKT_MS; letzterTakt = 0; loop(); takte++;
      if (takte % (mo.art >= M_FALTER ? 25 : 12) == 0 && bilder < 12) { char n[64]; snprintf(n, sizeof n, "40-%s-%02d", mo.name, bilder++); foto(n); }
    }
    printf("Moment %-8s: %4d Takte, Evi bei x=%d flip=%d\n", mo.name, takte, geher.x, geher.flip);
    PRUEFE(moment.art == M_KEINER, "Moment endet nicht");
    PRUEFE(geher.x == 0 && !geher.flip, "Evi nicht zurück auf dem Teppich");
  }

  // ---------- Momente als Baby, Teenager und Oma (seit 24.11.2026) ----------
  struct { ms_t alter; Phase ph; MomentArt art; const char* name; } phasen[] = {
    {3 * STUNDE, BABY, M_GEHEN, "baby-gehen"}, {3 * STUNDE, BABY, M_FENSTER, "baby-fenster"},
    {36 * STUNDE, TEEN, M_FALTER, "teen-falter"}, {36 * STUNDE, TEEN, M_SUMMEN, "teen-summen"},
    {8 * TAG + 12 * STUNDE, OMA, M_DREHEN, "oma-drehen"}, {8 * TAG + 12 * STUNDE, OMA, M_VOGEL, "oma-vogel"}};
  for (auto& p : phasen) {
    vorbereiten(14, 0, p.alter);
    PRUEFE(phaseBei(pet, jetztMs()) == p.ph, "Phase stimmt");
    momentStarten(p.art, GD_KEINER);
    int takte = 0, bilder = 0, mitBild = 0;
    while (moment.art != M_KEINER && takte < 2000) {
      simMillis += TAKT_MS; letzterTakt = 0; loop(); takte++;
      if (momentBild(p.ph)) mitBild++;
      if (takte % 25 == 0 && bilder < 4) { char n[64]; snprintf(n, sizeof n, "45-%s-%02d", p.name, bilder++); foto(n); }
    }
    printf("Moment %-12s: %4d Takte, %d Takte mit eigener Zeichnung\n", p.name, takte, mitBild);
    PRUEFE(moment.art == M_KEINER && geher.x == 0, "Moment endet, Evi zurück");
    PRUEFE(mitBild > 0, "Moment zeigt eine Zeichnung der Phase");
  }
  vorbereiten(9, 0, 3 * STUNDE);
  begruessen(jetztMs() - 10 * STUNDE);
  PRUEFE(evi.reaktion == R_GRUSS || evi.reaktion == R_JUBEL, "Baby begrüßt");
  laufen(500); foto("46-baby-gruss");

  // ---------- Begrüßung und Tageslaune ----------
  vorbereiten(9, 0);
  moment.launeTag = -1;
  begruessen(jetztMs() - 10 * STUNDE);
  laufen(600); foto("50-gruss");
  laufen(3500); foto("51-laune");
  printf("Laune heute: %d\n", launeHeute(jetztMs()));
  // Pflege während eines Moments: Evi geht nach Hause
  vorbereiten(14, 0);
  momentStarten(M_FENSTER, GD_KEINER);
  for (int i = 0; i < 40; i++) { simMillis += TAKT_MS; letzterTakt = 0; loop(); }
  aktionAusfuehren(E_PUTZEN);
  for (int i = 0; i < 80 && moment.art != M_KEINER; i++) { simMillis += TAKT_MS; letzterTakt = 0; loop(); }
  PRUEFE(geher.x == 0, "nach Pflege nicht nach Hause");

  // ---------- Besuch fällig? ----------
  vorbereiten(14, 0);
  moment.letzterBesonderer = 0;
  MomentArt besuch; bool f = besuchFaellig(jetztMs(), besuch);
  MomentArt arten[2]; ms_t um[2]; int n = besuchHeute(jetztMs(), arten, um);
  for (int i = 0; i < n; i++) { Ortszeit o = ortszeit(um[i]); printf("Besuch %d: %s um %02d:%02d\n", i, arten[i] == M_FALTER ? "Schmetterling" : "Vogel", o.stunde, o.minute); }
  printf("um 14 Uhr fällig: %d\n", f);

  printf("Display: %ld Bilder verglichen, %ld falsche Punkte\n", simBilder, simFalschePunkte);
  if (simFalschePunkte) { printf("FEHLER: Display weicht ab\n"); fehler++; }
  printf("%s: %d Fehler\n", fehler ? "NICHT OK" : "OK", fehler);
  return fehler ? 1 : 0;
}
