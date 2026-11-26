// Parkfreunde, zweiter Satz, Evis Wesen und Album (26.11.2026)
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
// Zeichen (nicht Bytes) zählen
int zeichen(const char* s) { int n = 0; for (; *s; s++) if ((*s & 0xC0) != 0x80) n++; return n; }
void langePruefen(const char* name, const char* s, int& zuLang) { if (zeichen(s) > 38) { printf("  zu lang (%d): %s [%s]\n", zeichen(s), s, name); zuLang++; } }
int main() {
  bildschirmStarten(); bild.cp437(true);
  const ms_t heute = ausOrtszeit(2026, 11, 26, 12, 0);
  uhrSetzen(heute);
  pet = neuesTier(heute - 5 * TAG, ERSTES_EI);
  pet.schluepft = heute - 4 * TAG;                       // Erwachsen
  pet.aktualisiert = heute; pet.satt = pet.laune = pet.sauber = 90;
  himmel = himmelBei(heute);
  freundLaden(); parkFreundLaden();

  // --- Sätze nicht zu lang ---
  int zuLang = 0;
  for (int w = 0; w < WESEN_ZAHL; w++) for (int i = 0; i < 3; i++) {
    langePruefen("passt", NACH_PASST[w][i], zuLang); langePruefen("eigen", NACH_EIGEN[w][i], zuLang);
    langePruefen("plaudern", WESEN_PLAUDERN[w][i], zuLang);
  }
  for (int a = 0; a < 10; a++) for (int i = 0; i < 3; i++) langePruefen("wetter", NACH_WETTER[a][i], zuLang);
  for (int i = 0; i < 3; i++) { langePruefen("fund", NACH_FUND_DA[i], zuLang); langePruefen("fund", NACH_FUND_NOCH[i], zuLang); }
  for (int s = 0; s < 4; s++) for (int i = 0; i < 5; i++) langePruefen("gruss", GRUSS_STUFE[s][i], zuLang);
  PRUEFE(zuLang == 0, "alle neuen Sätze höchstens 38 Zeichen");
  int wesenZaehler[WESEN_ZAHL] = {};
  for (int i = 0; i < BESUCHER; i++) wesenZaehler[FIGUR_WESEN[i]]++;
  bool jedes = true; for (int w = 0; w < WESEN_ZAHL; w++) if (!wesenZaehler[w]) jedes = false;
  PRUEFE(jedes, "jedes Wesen hat mindestens eine Figur");

  // --- Wesen ist fest und verschieden je Evi ---
  const int w0 = eviWesen();
  PRUEFE(w0 >= 0 && w0 < WESEN_ZAHL && eviWesen() == w0, "Evis Wesen ist fest");
  bool verschieden = false;
  const ms_t geb = pet.geboren;
  for (int i = 1; i < 40; i++) { pet.geboren = geb + i * 1000; if (eviWesen() != w0) verschieden = true; }
  pet.geboren = geb;
  PRUEFE(verschieden, "andere Evis haben ein anderes Wesen");

  // --- Treffen zählen: einmal pro Tag und Figur, Stufen ---
  freund.punkte = 10; freundTagPruefen(heute);
  park = ParkZustand(); park.phase = PARK_GRUSS; park.besuch = 5;
  parkFreundBegegnung(heute);
  PRUEFE(park.stufe == 0 && parkFreund.treffer[5] == 1, "erstes Treffen: fremd, 1 Treffen");
  parkFreundBegegnung(heute);
  PRUEFE(parkFreund.treffer[5] == 1 && park.stufe == 1, "am selben Tag zählt es nicht doppelt");
  for (int tag = 1; tag <= 2; tag++) { parkFreundBegegnung(heute + tag * TAG); }
  PRUEFE(parkFreund.treffer[5] == 3 && parkFreundStufe(3) == 2, "nach 3 Treffen vertraut");
  PRUEFE(parkFreundStufe(0) == 0 && parkFreundStufe(2) == 1 && parkFreundStufe(7) == 3 && parkFreundStufe(255) == 3, "Stufengrenzen");
  PRUEFE(parkFreundGetroffen() == 1, "Album zählt getroffene Figuren");
  PRUEFE(freund.punkte > 10, "Treffen erhöhen die Freundschaft ein wenig");

  // --- Begrüßung und zweiter Satz ---
  int fremdGruss = 0, andereGruss = 0, jederNachsatz[4] = {};
  for (int i = 0; i < 300; i++) {
    park = ParkZustand(); park.phase = PARK_GRUSS; park.besuch = i % BESUCHER;
    parkFreund.treffer[park.besuch] = 0; parkFreund.heuteMaske = 0;
    parkGrussWaehlen();
    char t[80]; parkGrussText(t, sizeof(t));
    if (park.stufe == 0 && park.grussArt == 1) fremdGruss++; else andereGruss++;
    park.zweiter = true;
    parkGrussText(t, sizeof(t));
    if (!t[0] || strstr(t, "(null)")) { printf("  leerer Satz\n"); fehler++; }
    jederNachsatz[park.nachArt]++;
  }
  PRUEFE(fremdGruss == 300 && andereGruss == 0, "Fremde werden immer mit einem Kennenlern-Satz begrüßt");
  PRUEFE(jederNachsatz[1] > 0 && jederNachsatz[2] > 0, "zweiter Satz: über sich selbst und Wetter kommen vor");
  // Passt das Wesen, kommt „passt zu Evi“ oft vor
  int passt = 0, anzahl = 0;
  for (int i = 0; i < BESUCHER; i++) {
    if (FIGUR_WESEN[i] != eviWesen()) continue;
    for (int k = 0; k < 60; k++) { park = ParkZustand(); park.besuch = i; parkFreund.heuteMaske = 0; park.phase = PARK_GRUSS; parkGrussWaehlen(); anzahl++; if (park.passt && park.nachArt == 0) passt++; }
  }
  PRUEFE(anzahl == 0 || passt > anzahl / 4, "gleiches Wesen: oft ein Satz, der zu Evi passt");
  // Auswahl nur mit Wesen-Treffer: nie ein Satz „passt“ bei anderem Wesen
  bool zuFalsch = false;
  for (int k = 0; k < 200; k++) { park = ParkZustand(); park.besuch = k % BESUCHER; parkFreund.heuteMaske = 0; park.phase = PARK_GRUSS; parkGrussWaehlen(); if (park.nachArt == 0 && !park.passt) zuFalsch = true; }
  PRUEFE(!zuFalsch, "„passt“-Sätze nur bei gleichem Wesen");

  // --- Speichern und Laden ---
  parkFreund.treffer[0] = 9; parkFreundGeaendert = true;
  parkFreundSpeichern();
  memset(&parkFreund, 0, sizeof(parkFreund));
  parkFreundLaden();
  PRUEFE(parkFreund.treffer[0] == 9, "Treffen bleiben nach Neustart erhalten");
  EEPROM.write(PARKFREUND_ADRESSE + 5, EEPROM.read(PARKFREUND_ADRESSE + 5) ^ 0x55);
  parkFreundLaden();
  PRUEFE(parkFreund.treffer[0] == 0 && parkFreundGetroffen() == 0, "kaputter Speicher wird zurückgesetzt");
  PRUEFE(EEPROM.read(EEPROM_GROESSE - 1) == EEPROM.read(GEB_NEU_ADRESSE), "Geburtstags-Merker unberührt");

  // --- Besucher bevorzugt passendes Wesen (ca. jedes 3. Mal von den 19) ---
  int passendBesucher = 0, lotti = 0, gesamt = 0;
  for (int d = 0; d < 300; d++) for (int s = 0; s < 6; s++) {
    const int b = parkBesucher(heute + d * TAG + s * 4 * STUNDE + 1);
    gesamt++; if (b == 0) lotti++; else if (FIGUR_WESEN[b] == eviWesen()) passendBesucher++;
    if (b < 0 || b >= BESUCHER) { printf("  Besucher außerhalb\n"); fehler++; }
  }
  printf("  Lotti %d, passend %d von %d\n", lotti, passendBesucher, gesamt);
  PRUEFE(lotti > gesamt / 4 && lotti < gesamt * 2 / 5, "Lotti kommt weiter etwa jedes dritte Mal");
  PRUEFE(passendBesucher > (gesamt - lotti) / 4, "Besucher mit gleichem Wesen kommen häufiger");

  // --- Folgen des Wesens für die Freundschaft ---
  freund.plusTag = -1; freund.punkte = 10; freundTagPruefen(heute); freund.plusHeute = 0;
  const int w = eviWesen();
  float vorher = freund.punkte; freundGespielt(); const float spiel = freund.punkte - vorher;
  freund.plusHeute = 0; vorher = freund.punkte; freundGekuschelt(); const float kuschel = freund.punkte - vorher;
  freund.plusHeute = 0; vorher = freund.punkte; freundGepflegt(PUTZEN); const float putzen = freund.punkte - vorher;
  freund.plusHeute = 0; vorher = freund.punkte; freundGepflegt(NASCHEN); const float naschen = freund.punkte - vorher;
  PRUEFE(spiel == (w == W_SPORT ? 2.0f : 1.0f), "Spielen: Sportlerin doppelt");
  PRUEFE(kuschel == (w == W_SCHMUSE ? 1.0f : 0.5f), "Kuscheln: Schmusekatze doppelt");
  PRUEFE(putzen == (w == W_WASSER ? 0.0f : 0.5f), "Saubermachen: wasserscheu gibt nichts");
  PRUEFE(naschen == (w == W_NASCH ? 2.0f : 0.5f), "Naschen: Naschkatze mehr");
  // Alle Wesen durchprobieren
  bool alleWesen[WESEN_ZAHL] = {};
  for (int i = 0; i < 400; i++) { pet.geboren = geb + i * 777; alleWesen[eviWesen()] = true; }
  pet.geboren = geb;
  bool alle = true; for (int k = 0; k < WESEN_ZAHL; k++) if (!alleWesen[k]) alle = false;
  PRUEFE(alle, "alle sechs Wesen kommen vor");

  // --- Neue Evi: Treffen von vorne ---
  parkFreund.treffer[4] = 6; parkFreund.heuteMaske = 5; parkFreund.heuteTag = tagKennung(heute); parkFreundGeaendert = true;
  parkFreund.evi = pet.geboren;
  parkFreundSpeichern();
  parkFreundLaden();
  PRUEFE(parkFreund.treffer[4] == 6, "dieselbe Evi: Treffen bleiben");
  const ms_t alteGeburt = pet.geboren;
  pet.geboren = alteGeburt + 5 * TAG;     // neue Evi
  freundRunde(heute);
  PRUEFE(parkFreund.treffer[4] == 0 && parkFreund.heuteMaske == 0 && parkFreundGetroffen() == 0, "neue Evi: alle Treffen wieder bei null");
  parkFreundSpeichern(); parkFreundLaden();
  PRUEFE(parkFreund.treffer[4] == 0 && parkFreund.evi == pet.geboren, "neue Evi: bleibt zurückgesetzt nach Neustart");
  pet.geboren = alteGeburt;
  parkFreund.treffer[4] = 6; parkFreund.evi = alteGeburt; parkFreundGeaendert = true; parkFreundSpeichern();
  pet.geboren = alteGeburt + 9 * TAG; parkFreundLaden();     // Neustart mit neuer Evi
  PRUEFE(parkFreundGetroffen() == 0, "Neustart mit neuer Evi: von vorne");
  pet.geboren = alteGeburt;
  parkFreundLaden(); parkFreund.treffer[1] = 1; parkFreund.treffer[2] = 4; parkFreund.treffer[3] = 12; parkFreundGeaendert = true;

  // --- Wasserscheu: nicht in den Park bei Regen ---
  {
    ms_t regen = 0, trocken = 0;
    for (int t = 0; t < 400 && !(regen && trocken); t++) {
      const ms_t z = heute + t * 7 * STUNDE;
      if (wetterBei(z) == WT_REGEN && !regen && himmelBei(z) == HIMMEL_TAG) regen = z;
      if (wetterBei(z) != WT_REGEN && !trocken && himmelBei(z) == HIMMEL_TAG) trocken = z;
    }
    PRUEFE(regen && trocken, "Testtage mit und ohne Regen gefunden");
    const ms_t geb0 = pet.geboren;
    int wasserscheu = 0, andere = 0;
    for (int i = 0; i < 60; i++) {
      pet.geboren = geb0 + i * 1111;
      const bool ws = eviWesen() == W_WASSER;
      pet.schluepft = regen - 3 * TAG; pet.aktualisiert = regen;
      uhrSetzen(regen);
      park = ParkZustand();
      parkOeffnen(regen);
      if (ws) { wasserscheu++; if (parkOffen()) { printf("  wasserscheu geht bei Regen in den Park\n"); fehler++; } }
      else { andere++; if (!parkOffen()) { printf("  andere Evi darf bei Regen nicht\n"); fehler++; } }
      parkSchliessen();
      uhrSetzen(trocken); pet.schluepft = trocken - 3 * TAG; pet.aktualisiert = trocken;
      parkOeffnen(trocken);
      if (!parkOffen()) { printf("  bei trockenem Wetter geht der Park nicht\n"); fehler++; }
      parkSchliessen();
    }
    PRUEFE(wasserscheu > 0 && andere > 0, "Wasserscheue und andere Evis getestet");
    // Regen beginnt, während Evi im Park ist
    for (int i = 0; i < 60; i++) {
      pet.geboren = geb0 + i * 1111;
      if (eviWesen() != W_WASSER) continue;
      uhrSetzen(trocken); pet.schluepft = trocken - 3 * TAG; pet.aktualisiert = trocken;
      parkOeffnen(trocken);
      uhrSetzen(regen); pet.schluepft = regen - 3 * TAG; pet.aktualisiert = regen;
      parkTakt(regen, millis());
      PRUEFE(!parkOffen(), "Regen im Park: wasserscheue Evi geht heim");
      break;
    }
    pet.geboren = geb0;
  }
  uhrSetzen(heute); pet.schluepft = heute - 4 * TAG; pet.aktualisiert = heute;

  // --- Album ---
  parkFreund.treffer[1] = 1; parkFreund.treffer[2] = 4; parkFreund.treffer[3] = 12;
  uhrSetzen(heute); pet.aktualisiert = heute;
  bildschirmAn = true; bildschirmEin();
  menueBauen();
  bool hatAlbum = false; for (int i = 0; i < menueAnzahl; i++) if (menue[i].e == E_ALBUM) hatAlbum = true;
  PRUEFE(hatAlbum, "Menü hat „Parkfreunde“");
  albumOeffnen(heute);
  PRUEFE(parkOffen() && park.phase == PARK_ALBUM, "Album geht auf (Park-Phase)");
  zeichneAlles();
  int dunkel = 0; for (int y = 0; y < 240; y++) for (int x = 0; x < 320; x++) if (bild.stufe(x, y) < 8) dunkel++;
  PRUEFE(dunkel > 300, "Album zeichnet etwas");
  const int vorWahl = albumWahl;
  knopf(K_R); PRUEFE(albumWahl == (vorWahl + 1) % BESUCHER, "Rechts: nächste Figur");
  knopf(K_DOWN); PRUEFE(albumWahl == (vorWahl + 6) % BESUCHER, "Runter: eine Zeile weiter");
  albumWahl = 0; knopf(K_L); PRUEFE(albumWahl == BESUCHER - 1, "Links vom ersten: letzte Figur");
  albumWahl = 2; knopf(K_UP); PRUEFE(albumWahl == BESUCHER - 3, "Hoch vom ersten Rand: umbrechen");
  zeichneAlles();
  beruehrt(70, 50);   // Touch: zweite Spalte, erste Zeile
  PRUEFE(albumWahl == 1, "Antippen wählt die Figur");
  beruehrt(10, ZEILE_Y + 10);
  PRUEFE(albumWahl == 2 && parkOffen(), "Tipper unten links: weiter");
  beruehrt(280, ZEILE_Y + 10);
  PRUEFE(!parkOffen(), "Tipper unten rechts schließt");
  albumOeffnen(heute); knopf(K_B); PRUEFE(!parkOffen(), "B schließt das Album");
  albumOeffnen(heute); bildschirmAus(); PRUEFE(!parkOffen(), "Bildschirm aus schließt das Album");
  // Alle Figuren zeichnen, ohne dass etwas abstürzt (alle Lebensphasen)
  for (int phase = 0; phase < 4; phase++) {
    const Phase soll[4] = {BABY, TEEN, ERWACHSEN, OMA};
    const ms_t beginn[4] = {0, R::BABY, R::BABY + R::TEEN, R::BABY + R::TEEN + R::ERWACHSEN};
    pet.schluepft = heute - beginn[phase] - 3 * STUNDE; pet.geboren = pet.schluepft - 1000;
    pet.aktualisiert = heute;
    if (phaseBei(pet, heute) != soll[phase]) { printf("  Phase passt nicht\n"); fehler++; continue; }
    albumOeffnen(heute); albumWahl = 7; zeichneAlles(); parkSchliessen();
  }
  PRUEFE(true, "Album in allen vier Lebensphasen gezeichnet");
  printf(fehler ? "%d FEHLER\n" : "Alles in Ordnung.\n", fehler);
  return fehler ? 1 : 0;
}
