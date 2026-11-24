// Prüft das Kuscheln (kuscheln.ino, seit 24.11.2026): Touch ist keine Taste mehr, Evi antippen oder streicheln
// kuschelt sie. Dazu die kleine Uhrzeit, der trübe Himmel bei Regen und das Menü ohne Uhr, Name und Hilfe.
// Der Touch-Chip CST328 ist nachgebildet (ersatz/Wire.h: simFinger), die Tasten (MCP23017) auch.
#include "Arduino.h"
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
// Programm laufen lassen; liegt ein Finger auf, meldet der Touch-Chip alle 10 ms (wie die Unterbrechung am Board)
void laufen(uint32_t ms) {
  for (uint32_t t = 0; t < ms; t += 10) {
    simMillis += 10;
    if (simCst().unten) touchMeldet = true;
    loop();
  }
}
void tippen(int x, int y) { simFinger(true, x, y); laufen(60); simFinger(false); laufen(250); }
// Ein Tag mit dieser Tageslaune (ab dem 24.11.2026), mittags
ms_t tagMitLaune(int laune) {
  for (int tag = 0; tag < 60; tag++) {
    const ms_t t = ausOrtszeit(2026, 11, 24, 12, 0) + (ms_t)tag * TAG;
    pet = neuesTier(t - 4 * TAG);       // die Laune hängt auch von Evis Geburt ab (wie in vorbereiten)
    if (launeHeute(t) == laune) return t;
  }
  return ausOrtszeit(2026, 11, 24, 12, 0);
}
void vorbereiten(ms_t utc) {
  uhrSetzen(utc);
  pet = neuesTier(utc - 4 * TAG);
  pet.aktualisiert = utc;
  pet.satt = pet.laune = pet.sauber = 70;
  pet.muenzen = 5;
  himmel = himmelBei(utc);
  spiel = SpielZustand();
  momentAufraeumen(); geher.x = 0; geher.fuss = RAUM_HEIM_Y; geher.flip = false;
  moment.naechster = utc + 30 * MINUTE;
  moment.ruheBisTakt = 0; moment.letzterBesonderer = utc;
  menueOffen = false; ritual.aktiv = false;
  bildschirmAn = true; letzteBeruehrung = millis();
  evi.reaktion = R_KEINE; evi.reaktionBisTakt = 0;
  kuschel.seit = 0; kuschel.letzteWirkung = 0; kuschel.heim = false;
  sagen.bis = 0; meldungBis = 0;
  zeichneAlles();                       // damit Evis Lage für das Antippen bekannt ist
}
int eviMitteX() { return eviLinks + eviBreite / 2; }
int eviMitteY() { return eviOben + eviHoehe / 2; }
int herzenSichtbar() {
  int n = 0;
  for (int i = 0; i < 8; i++) if (kuschel.herzStart[i] && millis() - kuschel.herzStart[i] < HERZ_STEIGT) n++;
  return n;
}

int main() {
  bildschirmStarten(); bild.cp437(true);
  simCst().an = true;
  touchStarten();
  PRUEFE(touchChip == TOUCH_CST328, "Touch-Chip CST328 gefunden");
  PRUEFE(tastenStarten(), "Tasten bereit");
  lageStarten();

  // ---------- Menü ohne Uhr stellen, Name ändern, Hilfe ----------
  const ms_t verspielt = tagMitLaune(0), verschmust = tagMitLaune(LAUNE_VERSCHMUST);
  vorbereiten(verspielt);
  menueZeigen(true);
  bool alt = false;
  for (int i = 0; i < menueAnzahl; i++)
    if (strstr(menue[i].text, "Uhr") || strstr(menue[i].text, "Name") || strstr(menue[i].text, "Hilfe")) alt = true;
  PRUEFE(!alt && menueAnzahl > 0, "Menü ohne Uhr stellen, Name ändern und Hilfe");
  zeichneAlles(); foto("k-menue");
  menueZeigen(false);

  // ---------- Touch ist keine Taste mehr ----------
  vorbereiten(verspielt);
  tippen(20, 120);
  PRUEFE(!menueOffen && kuschel.seit == 0, "links neben Evi tippen: kein Menü, kein Kuscheln");
  tippen(300, 210);
  PRUEFE(!menueOffen && kuschel.seit == 0, "rechts unten tippen: nichts");
  // Ohne Tasten gelten die Drittel weiter (Notlösung)
  tastenDa = false; tastenLetzteSuche = millis();   // die nächsten 2 s nicht nach den Tasten suchen
  tippen(160, 120);
  PRUEFE(menueOffen && kuschel.seit == 0, "ohne Tasten: Mitte öffnet das Menü wie früher");
  tippen(300, 120);
  PRUEFE(!menueOffen, "ohne Tasten: rechts schließt");
  tastenDa = true;

  // ---------- Evi antippen: kuscheln ----------
  vorbereiten(verspielt);
  double vorher = pet.laune;
  simFinger(true, eviMitteX(), eviMitteY()); laufen(40);
  PRUEFE(evi.reaktion == R_KUSCHELN && kuschel.seit != 0, "auf Evi tippen kuschelt");
  PRUEFE(pet.laune > vorher + R::KUSCHEL_LAUNE - 0.5, "Stimmung +5");
  PRUEFE(millis() < sagen.bis && strlen(sagen.text) > 0, "Evi sagt etwas Liebes");
  printf("   sagt: %s · Zeile: %s\n", sagen.text, meldung);
  simFinger(false); laufen(400);
  zeichneAlles(); foto("k-kuscheln");
  PRUEFE(herzenSichtbar() >= 1, "Herzen steigen auf");
  // Gleich noch einmal: nur Herzen, keine Stimmung
  vorher = pet.laune;
  tippen(eviMitteX(), eviMitteY());
  PRUEFE(pet.laune <= vorher + 0.01, "innerhalb einer Minute: keine Stimmung mehr");
  laufen(5000);
  PRUEFE(kuschel.seit == 0 && evi.takt >= evi.reaktionBisTakt, "Kuscheln endet");
  simMillis += 61000; uhrSetzen(jetztMs());
  vorher = pet.laune; kuschelnBeginnen();
  PRUEFE(pet.laune > vorher + R::KUSCHEL_LAUNE - 0.5, "nach einer Minute wirkt es wieder");
  laufen(3000);

  // Streicheln: Finger bleibt auf Evi und bewegt sich, das Kuscheln geht weiter
  vorbereiten(verspielt);
  for (int i = 0; i < 40; i++) { simFinger(true, eviMitteX() - 20 + (i % 10) * 4, eviMitteY()); laufen(100); }
  PRUEFE(kuschelnLaeuft() && evi.reaktion == R_KUSCHELN && evi.takt < evi.reaktionBisTakt, "4 s streicheln: kuschelt noch");
  int hoechst = 0; for (int i = 0; i < 10; i++) { laufen(120); hoechst = max(hoechst, herzenSichtbar()); }
  PRUEFE(hoechst >= 3, "beim Streicheln steigen immer neue Herzen auf");
  simFinger(false); laufen(4500);
  PRUEFE(!kuschelnLaeuft(), "Finger weg: Kuscheln hört auf");

  // ---------- verschmust: mehr Stimmung ----------
  vorbereiten(verschmust);
  vorher = pet.laune; kuschelnBeginnen();
  PRUEFE(pet.laune > vorher + R::KUSCHEL_LAUNE_VERSCHMUST - 0.5, "verschmust: Stimmung +8");
  printf("   verschmust sagt: %s\n", sagen.text);
  laufen(3000);
  char zeile[100]; launeZeile(jetztMs(), zeile, sizeof(zeile));
  PRUEFE(strstr(zeile, "tipp mich an") != nullptr, "Zeile: verschmust · tipp mich an");

  // ---------- Kuschelwunsch ----------
  vorbereiten(verschmust + 3 * STUNDE);   // 15 Uhr: kein Mittagessen in der Blase
  pet.satt = pet.laune = pet.sauber = 70;
  { Wunsch w; if (aktuellerWunsch(pet, jetztMs(), w)) pet.wunschErfuellt = w.beginn; }   // kein Wunsch gerade
  // Seit 24.11.2026 (launen.ino): jede Laune hat ihren Wunsch, jeder dritte kleine Moment, nur wenn jemand da ist
  launen = {};
  int wuenscheNiemand = 0;
  for (int i = 0; i < 300; i++) {
    Gedanke g = GD_KEINER; moment.letzter = M_GEHEN;
    if (momentWaehlen(S_GLUECKLICH, false, LAUNE_VERSCHMUST, g) == M_KUSCHELN) wuenscheNiemand++;
  }
  PRUEFE(wuenscheNiemand == 0, "ohne Bedienung in den letzten 10 Minuten keine Launenwünsche");
  launen.letzteBedienung = millis() | 1;
  int wuensche = 0;
  for (int i = 0; i < 300; i++) {
    Gedanke g = GD_KEINER; moment.letzter = M_GEHEN;
    if (momentWaehlen(S_GLUECKLICH, false, LAUNE_VERSCHMUST, g) == M_KUSCHELN) wuensche++;
  }
  printf("   Launenwunsch unter 300 kleinen Momenten: %d\n", wuensche);
  PRUEFE(wuensche > 70 && wuensche < 130, "etwa jeder dritte kleine Moment ist ein Launenwunsch");
  {
    Tier merken = pet; pet.wunschErfuellt = 0;
    Wunsch w; int mitWunsch = 0;
    for (int h = 8; h < 22 && !mitWunsch; h++) {
      uhrSetzen(verschmust - 12 * STUNDE + h * STUNDE);
      if (!aktuellerWunsch(pet, jetztMs(), w)) continue;
      for (int i = 0; i < 100; i++) { Gedanke g = GD_KEINER; moment.letzter = M_GEHEN; if (momentWaehlen(S_GLUECKLICH, false, LAUNE_VERSCHMUST, g) == M_KUSCHELN) mitWunsch++; }
      mitWunsch = mitWunsch ? mitWunsch : -1;
    }
    PRUEFE(mitWunsch == -1, "solange ein Wunsch in der Blase steht, kein Kuschelwunsch");
    pet = merken; uhrSetzen(verschmust + 3 * STUNDE);
  }
  momentStarten(M_KUSCHELN, GD_KEINER);
  laufen(1500); foto("k-wunsch-frage");
  laufen(3000); foto("k-wunsch-denken");
  PRUEFE(moment.art == M_KUSCHELN && moment.gedanke == GD_HERZ, "Kuschelwunsch: Herz in der Gedankenwolke");
  vorher = pet.laune;
  const uint32_t muenzenVorher = pet.muenzen;
  tippen(eviMitteX(), eviMitteY());
  PRUEFE(moment.art == M_KEINER && moment.gedanke == GD_KEINER, "Antippen erfüllt den Kuschelwunsch, Moment endet");
  PRUEFE(pet.laune > vorher + R::KUSCHEL_LAUNE_WUNSCH - 0.5 && pet.muenzen == muenzenVorher, "Kuschelwunsch: Stimmung +10");
  printf("   sagt: %s\n", sagen.text);
  laufen(300); foto("k-wunsch-erfuellt");
  laufen(5000);
  // Unbeantwortet (launen.ino): -5 Stimmung, enttäuscht; beim zweiten Mal hintereinander beleidigt
  PRUEFE(!launenWunschOffen() && launen.verpasst == 0, "erfüllter Wunsch ist nicht mehr offen");
  vorher = pet.laune;
  momentStarten(M_KUSCHELN, GD_KEINER);
  for (int i = 0; i < 6000 && launenWunschOffen(); i++) laufen(10);
  PRUEFE(!launenWunschOffen() && launen.verpasst == 1 && !launenBeleidigt(), "nach 45 s ohne Antippen: Wunsch verpasst");
  PRUEFE(pet.laune < vorher - 4 && pet.laune > vorher - 6, "verpasster Wunsch: Stimmung -5");
  printf("   enttäuscht: %s\n", sagen.text);
  laufen(300); foto("k-wunsch-verpasst");
  for (int i = 0; i < 2000 && moment.art != M_KEINER; i++) laufen(10);
  PRUEFE(geher.x == 0, "danach zurück auf dem Teppich");
  momentStarten(M_KUSCHELN, GD_KEINER);
  for (int i = 0; i < 6000 && launenWunschOffen(); i++) laufen(10);
  PRUEFE(launenBeleidigt() && launen.verpasst == 0, "zweiter verpasster Wunsch: beleidigt");
  printf("   beleidigt: %s\n", sagen.text);
  for (int i = 0; i < 2000 && moment.art != M_KEINER; i++) laufen(10);
  laufen(4000); foto("k-beleidigt");
  aktionAusfuehren(E_BALL);
  PRUEFE(!spiel.aktiv, "beleidigt: kein Spiel");
  vorher = pet.laune;
  tippen(eviMitteX(), eviMitteY()); laufen(900);
  PRUEFE(launenBeleidigt() && pet.laune < vorher + 1, "beleidigt: Kuscheln wirkt nicht");
  tippen(eviMitteX(), eviMitteY()); laufen(900);
  tippen(eviMitteX(), eviMitteY()); laufen(300);
  PRUEFE(!launenBeleidigt(), "dreimal streicheln versöhnt sie");
  printf("   versöhnt: %s\n", sagen.text);
  laufen(5000);

  // ---------- ein Moment wird unterbrochen: Evi bleibt stehen, danach geht sie zurück ----------
  vorbereiten(verspielt);
  momentStarten(M_FENSTER, GD_KEINER);
  for (int i = 0; i < 2000 && moment.bild != MB_FENSTER; i++) laufen(10);
  laufen(200);
  const int amFenster = geher.x;
  PRUEFE(amFenster < -40, "Evi steht am Fenster");
  tippen(eviMitteX(), eviOben + 4);
  PRUEFE(moment.art == M_KEINER && geher.x == amFenster && evi.reaktion == R_KUSCHELN, "am Fenster gekuschelt: bleibt dort stehen");
  foto("k-fenster");
  laufen(14000);
  PRUEFE(geher.x == 0 && !geher.flip, "danach geht sie zurück zum Teppich");

  // ---------- Besuch: nur Herzen, der Schmetterling bleibt ----------
  vorbereiten(verspielt);
  momentStarten(M_FALTER, GD_KEINER);
  laufen(4000);
  kuschelnBeginnen();
  PRUEFE(moment.art == M_FALTER && kuschel.klein && evi.reaktion != R_KUSCHELN, "Schmetterling: Moment läuft weiter, nur Herzen");
  laufen(60000);

  // ---------- Schlaf, Ei, Grab ----------
  vorbereiten(ausOrtszeit(2026, 11, 25, 1, 0));
  PRUEFE(schlaeft(pet, jetztMs()), "nachts schläft Evi");
  vorher = pet.laune;
  zeichneAlles();
  tippen(eviMitteX(), eviMitteY());
  PRUEFE(schlaeft(pet, jetztMs()) && kuschel.klein && pet.laune <= vorher + 0.01 && strstr(meldung, "im Schlaf"),
         "schlafend: kuschelt sich an, wacht nicht auf, keine Wirkung");
  laufen(200); foto("k-schlaf");
  vorbereiten(verspielt);
  pet = neuesTier(jetztMs() - MINUTE); pet.aktualisiert = jetztMs();
  zeichneAlles();
  tippen(eviMitteX(), eviMitteY());
  PRUEFE(evi.wackelnBis > evi.takt && strstr(meldung, "Ei"), "Ei: wackelt, ist schön warm");
  vorbereiten(verspielt);
  pet.tot = true; pet.gestorben = jetztMs(); zeichneAlles();
  tippen(eviMitteX(), eviMitteY());
  PRUEFE(kuschel.seit == 0, "Grab: nichts");

  // ---------- Menü offen, Spiel: Antippen tut nichts ----------
  vorbereiten(verspielt);
  menueZeigen(true);
  tippen(eviMitteX() - 30, eviMitteY());
  PRUEFE(menueOffen && kuschel.seit == 0, "Menü offen: Antippen tut nichts");
  menueZeigen(false);
  spielStarten(SPIEL_BALL);
  tippen(160, 120);
  PRUEFE(spiel.aktiv && spiel.treffer == 0 && kuschel.seit == 0, "im Spiel: Touch fängt nicht und kuschelt nicht");
  spielEnde(true);

  // ---------- Bildschirm aus: der erste Tipper weckt nur ----------
  vorbereiten(verspielt);
  bildschirmAus();
  tippen(eviMitteX(), eviMitteY());
  PRUEFE(bildschirmAn && kuschel.seit == 0, "dunkel: erster Tipper macht nur den Bildschirm an (auch wenn der Finger kurz liegen bleibt)");
  tippen(eviMitteX(), eviMitteY());
  PRUEFE(kuschel.seit != 0, "danach kuschelt der nächste Tipper");

  // ---------- Uhrzeit und Regenhimmel ----------
  ms_t regen = 0;
  for (int tag = 0; tag < 365 && !regen; tag++) {
    const ms_t k = ausOrtszeit(2027, 3, 1, 11, 0) + (ms_t)tag * TAG;
    if (wetterBei(k) == WT_REGEN && himmelBei(k) == HIMMEL_TAG) regen = k;
  }
  vorbereiten(regen);
  zeichneAlles(); foto("k-regen-uhr");
  bild.fillScreen(1); fensterZeichnen(regen);
  int gepunktet = 0, weiss = 0;
  for (int y = GLAS_Y + 40; y < GLAS_Y + 44; y++)
    for (int x = GLAS_X + 2; x < GLAS_X + 26; x++) {
      if (bildDeckt(*FENSTER_JAHRESZEIT[jahreszeitBei(regen)], x - GLAS_X, y - GLAS_Y)) continue;
      const bool hell = bild.getBuffer()[y * 40 + x / 8] & (0x80 >> (x % 8));
      if (hell) weiss++; else gepunktet++;
    }
  PRUEFE(gepunktet > 10 && weiss > 10, "Regen am Tag: Himmel gepunktet wie in der Dämmerung");
  zeichneAlles();
  Ortszeit o = ortszeit(regen);
  char uhr[8]; snprintf(uhr, sizeof(uhr), "%02d:%02d", o.stunde, o.minute);
  printf("   Uhrzeit oben: %s\n", uhr);
  int schwarz = 0;
  for (int y = UHRZEIT_Y; y < UHRZEIT_Y + 7; y++)
    for (int x = UHRZEIT_X; x < UHRZEIT_X + 30; x++) if (!(bild.getBuffer()[y * 40 + x / 8] & (0x80 >> (x % 8)))) schwarz++;
  PRUEFE(schwarz > 20, "Uhrzeit steht oben an der Wand");
  // Sonne: Himmel bleibt weiß
  ms_t sonne = 0;
  for (int tag = 0; tag < 365 && !sonne; tag++) {
    const ms_t k = ausOrtszeit(2027, 3, 1, 11, 0) + (ms_t)tag * TAG;
    if (wetterBei(k) == WT_SONNE && himmelBei(k) == HIMMEL_TAG) sonne = k;
  }
  vorbereiten(sonne);
  zeichneAlles();
  bild.fillScreen(1); fensterZeichnen(sonne);
  gepunktet = 0;
  for (int y = GLAS_Y + 40; y < GLAS_Y + 44; y++)
    for (int x = GLAS_X + 2; x < GLAS_X + 26; x++) {
      if (bildDeckt(*FENSTER_JAHRESZEIT[jahreszeitBei(sonne)], x - GLAS_X, y - GLAS_Y)) continue;
      if (!(bild.getBuffer()[y * 40 + x / 8] & (0x80 >> (x % 8)))) gepunktet++;
    }
  PRUEFE(gepunktet < 5, "Sonne: Himmel bleibt hell");

  printf("Display: %ld Bilder, %ld falsche Punkte\n", simBilder, simFalschePunkte);
  if (simFalschePunkte) { printf("FEHLER: Display weicht ab\n"); fehler++; }
  printf("%s: %d Fehler\n", fehler ? "NICHT OK" : "OK", fehler);
  return fehler ? 1 : 0;
}
