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
  const uint8_t* b = bild.getBuffer();
  for (int y = 0; y < 240; y++) for (int x = 0; x < 320; x++)
    if (simDisplay[y][x] != ((b[y * 40 + x / 8] >> (7 - x % 8)) & 1)) simFalschePunkte++;
}
int fehler = 0;
#define PRUEFE(b, t) do { if (!(b)) { printf("FEHLER: %s\n", t); fehler++; } else printf("ok: %s\n", t); } while (0)
void foto(const char* name) {
  char p[80]; snprintf(p, sizeof(p), "bilder/%s.pbm", name);
  FILE* f = fopen(p, "wb"); fprintf(f, "P4\n320 240\n");
  const uint8_t* b = bild.getBuffer();
  for (int i = 0; i < 40 * 240; i++) { uint8_t v = ~b[i]; fwrite(&v, 1, 1, f); }
  fclose(f);
}
void warten(uint32_t ms) { for (uint32_t t = 0; t < ms; t += 10) { simMillis += 10; loop(); } }
void bereit(ms_t jetzt, ms_t alter = 4 * TAG) {
  gartenSchliessen(); sideboardSofortZu(); uhrSetzen(jetzt);
  pet = neuesTier(jetzt - alter); pet.aktualisiert = jetzt;
  pet.satt = pet.laune = pet.sauber = 90;
  moment.naechster = jetzt + STUNDE; moment.letzterBesonderer = jetzt;
  spiel = SpielZustand(); ritual.aktiv = false; menueOffen = false;
  bildschirmAn = true; himmel = himmelBei(jetzt);
}
int main() {
  bildschirmStarten(); bild.cp437(true);
  const ms_t jetzt = ausOrtszeit(2026, 11, 24, 14, 0);
  bereit(jetzt); sammlung = Sammlung();
  menueBauen(); bool da = false;
  for (int i = 0; i < menueAnzahl; i++) da |= menue[i].e == E_GARTEN;
  PRUEFE(da, "Garten vom Zimmermenü erreichbar");
  aktionAusfuehren(E_GARTEN);
  PRUEFE(garten.phase == GA_WAHL && !menueOffen, "Garten öffnet mit Suchstellenwahl");
  knopf(K_L); PRUEFE(garten.stelle == 1, "L blättert rückwärts zum Busch");
  knopf(K_A); warten(1200); PRUEFE(garten.phase == GA_SCHNUPPERN, "Evi läuft selbst zur gewählten Stelle und schnuppert");
  warten(1000); PRUEFE(garten.phase == GA_WARTEN, "Fragezeichen wartet auf unsere Hilfe");
  zeichneAlles(); foto("garten-warten");
  const uint16_t vorher = sammlung.anzahl;
  warten(15000); PRUEFE(garten.phase == GA_WARTEN && sammlung.anzahl == vorher, "keine schnelle Reaktion nötig und kein automatischer Fund");
  beruehrt(0, 150); PRUEFE(garten.phase == GA_WARTEN, "Tippen fern von der Suchstelle löst nichts aus");
  beruehrt(250, 160); PRUEFE(garten.phase == GA_UNTERSUCHEN, "Antippen hilft bei der Untersuchung");
  zeichneAlles(); foto("garten-untersuchen"); warten(2000);
  PRUEFE(garten.phase == GA_ZEIGEN || garten.phase == GA_LEER, "Suche hat ein Ergebnis");
  knopf(K_A); warten(800);
  // Alle drei Stellen: mindestens ein Fund, danach pro Stelle heute kein weiterer Versuch.
  for (int i = 0; i < 3; i++) { garten.stelle = i; gartenErgebnis(jetztMs()); }
  PRUEFE(sammlung.anzahl >= 1 && sammlung.gartenVersucht == 7, "mindestens eine ergiebige Stelle am Tag");
  const uint16_t gefunden = sammlung.anzahl;
  for (int i = 0; i < 3; i++) { garten.stelle = i; gartenErgebnis(jetztMs()); }
  PRUEFE(sammlung.anzahl == gefunden && garten.schon, "jede Stelle nur einmal am Tag");
  const Fundstueck f = sammlung.fund[0];
  PRUEFE(f.ort == FUND_GARTEN && f.tag == 20261124 && f.wetter == wetterBei(jetztMs()) && f.jahreszeit == JZ_WINTER && f.stelle < 3,
         "Fund speichert Art, Datum, Stelle, Jahreszeit und damaliges Wetter");
  const std::string erinnerung = fundErinnerung(f);
  uhrSetzen(jetzt + 10 * TAG);
  PRUEFE(erinnerung == fundErinnerung(f), "Erinnerung hängt nicht vom heutigen Wetter ab");
  uhrSetzen(jetzt);
  PRUEFE(fundBeimGassi(100, jetzt, pet.generation) >= 0, "Garten blockiert den ersten echten Spaziergang nicht");
  PRUEFE(sammlung.fund[sammlung.anzahl-1].wetter == FUND_UNBEKANNT && sammlung.fund[sammlung.anzahl-1].ort == FUND_GASSI,
         "echter Spaziergang behauptet kein gemessenes Wetter");
  Tier geladen; bool ton = true;
  spielstandSpeichern(pet, ton); sammlung = Sammlung();
  PRUEFE(spielstandLaden(geladen, ton) && sammlung.gartenVersucht == 7 && sammlung.fund[0].wetter == f.wetter && sammlung.fund[0].stelle == f.stelle,
         "Version 3 bewahrt Garten-Metadaten und besuchte Stellen");
  garten.stelle = 0; gartenErgebnis(jetzt + TAG);
  PRUEFE(sammlung.gartenTag == 20261125 && sammlung.gartenVersucht == 1, "am nächsten Tag kann neu gesucht werden");
  knopf(K_B); PRUEFE(garten.phase == GA_ZU, "B führt direkt ins Zimmer");
  // Alle neuen Posen jeder Lebensphase rendern; Gegenstände liegen separat auf den Pfoten.
  int phase = 1;
  for (ms_t alter : {3 * STUNDE, 36 * STUNDE, 4 * TAG, 9 * TAG}) {
    bereit(jetzt, alter); gartenOeffnen();
    PRUEFE(phaseBei(pet, jetzt) == phase, "richtige Lebensphase für die Gartenbilder");
    for (int pose = 0; pose < 3; pose++) {
      const Bild& b = *GARTEN_EVI[phase][pose];
      PRUEFE(b.w && b.h && b.maske, "neue Pose mit eigener Deckmaske vorhanden");
      garten.phase = pose == 0 ? GA_SCHNUPPERN : pose == 1 ? GA_UNTERSUCHEN : GA_ZEIGEN;
      garten.fund = 0; zeichneAlles();
      char name[40]; snprintf(name, sizeof(name), "garten-phase-%d-pose-%d", phase, pose); foto(name);
    }
    knopf(K_RESET); PRUEFE(garten.phase == GA_ZU && !bildschirmAn, "RESET beendet Garten in jeder Phase");
    phase++;
  }
  bereit(ausOrtszeit(2026,11,24,1,0)); gartenOeffnen();
  PRUEFE(garten.phase == GA_ZU && schlaeft(pet, jetztMs()), "schlafende Evi bleibt ungestört");
  bereit(jetzt); pet = neuesTier(jetzt); gartenOeffnen(); PRUEFE(garten.phase == GA_ZU, "Ei geht noch nicht in den Garten");
  bereit(jetzt); gartenOeffnen(); uhrSetzen(ausOrtszeit(2026,11,24,23,30)); gartenTakt(jetztMs());
  PRUEFE(garten.phase == GA_ZU, "zur Schlafenszeit geht es automatisch heim");
  // Version-2-Datei genauso aufbauen wie vor der Erweiterung.
  bereit(jetzt); SpielstandV2 v2 = {};
  v2.kennung = SPEICHER_KENNUNG; v2.version = 2; v2.groesse = sizeof(v2); v2.tier = pet;
  strcpy(v2.tier.name, "Alte Evi"); v2.tier.muenzen = 321; v2.tonAn = false;
  v2.sammlung.anzahl = 2; v2.sammlung.ausfluege = 5; v2.sammlung.letzterFundTag = 20261123;
  v2.sammlung.fund[0] = {FUND_FEDER, FUND_GASSI, 1, 20261122};
  v2.sammlung.fund[1] = {FUND_JUNIMO, FUND_GASSI, 2, 20261123};
  v2.pruefsumme = pruefsumme((uint8_t*)&v2, offsetof(SpielstandV2, pruefsumme));
  for (size_t i = 0; i < sizeof(v2); i++) EEPROM.write(i, ((uint8_t*)&v2)[i]);
  PRUEFE(spielstandLaden(geladen, ton) && sammlung.anzahl == 2 && geladen.muenzen == 321 && !strcmp(geladen.name,"Alte Evi") && !ton,
         "Version 2 wird mit Tier, Sammlung und Einstellungen übernommen");
  PRUEFE(sammlung.fund[0].tag == 20261122 && sammlung.fund[1].art == FUND_JUNIMO && sammlung.fund[1].generation == 2 && sammlung.fund[0].wetter == FUND_UNBEKANNT && sammlung.ausfluege == 5,
         "alte Funde behalten Datum, Art und Generation; unbekanntes Wetter bleibt unbekannt");
  EEPROM.write(offsetof(SpielstandV2, pruefsumme), EEPROM.read(offsetof(SpielstandV2, pruefsumme)) ^ 1);
  PRUEFE(!spielstandLaden(geladen, ton), "beschädigte Version 2 wird abgelehnt");
  // Voller Speicher: keine Daten überschreiben.
  for (int i = 0; i < SAMMLUNG_MAX; i++) fundAufnehmen(FUND_STEIN, FUND_GASSI, 20261124, 1);
  const Fundstueck erster = sammlung.fund[0]; garten.stelle = 0; gartenErgebnis(jetzt);
  PRUEFE(garten.voll && !memcmp(&erster,&sammlung.fund[0],sizeof(erster)), "volle Sammlung überschreibt nichts");
  sammlung = Sammlung(); fundAufnehmen(FUND_BLATT,FUND_GARTEN,20261124,1,WT_REGEN,JZ_HERBST,0);
  bereit(jetzt); sideboardOeffnen(); sideboard.phase = SB_OFFEN; zeichneAlles(); foto("garten-erinnerung"); sideboardSofortZu();
  // Neue Gesamtebenen: durchsichtiger Himmel, vollständig deckender Garten, Datumswechsel.
  for (int j = 0; j < 4; j++) {
    const Bild& b = *GARTEN_LANDSCHAFT[j];
    PRUEFE(b.w == 320 && b.h == 174 && !bildDeckt(b,200,0), "Gesamtszene passt zum Display und hat freien Himmel");
    bool bodenDeckend = true;
    for (int y = 72; y < 174; y++) for (int x = 0; x < 320; x++) bodenDeckend &= bildDeckt(b,x,y);
    PRUEFE(bodenDeckend, "Rasen, Schnee, Zaun und Beete lassen keinen Himmel durch");
  }
  PRUEFE(&gartenLandschaftBei(ausOrtszeit(2026,11,17,12,0)) == &LANDSCHAFT_WINTER &&
         &gartenLandschaftBei(ausOrtszeit(2026,11,18,12,0)) == &LANDSCHAFT_WEIHNACHTEN &&
         &gartenLandschaftBei(ausOrtszeit(2027,1,5,12,0)) == &LANDSCHAFT_WEIHNACHTEN &&
         &gartenLandschaftBei(ausOrtszeit(2027,1,6,12,0)) == &LANDSCHAFT_WINTER,
         "Weihnachts-Gesamtszene erscheint am 18. November und geht am 6. Januar");
  ms_t sonnig = 0;
  for (int d = 0; d < 31 && !sonnig; d++) {
    const ms_t k = ausOrtszeit(2026,7,1,12,0) + d * TAG;
    if (wetterBei(k) == WT_SONNE) sonnig = k;
  }
  gartenSzeneZeichnen(sonnig); bool himmelRuhig = true;
  for (int y = 30; y < 60; y++) for (int x = 190; x < 300; x++)
    himmelRuhig &= (bild.getBuffer()[y*40+x/8] & (0x80 >> (x%8))) != 0;
  PRUEFE(sonnig && himmelRuhig, "Sonnentag hat ruhigen Himmel ohne aufgeklebte Wettersymbole");
  // Vollständige Ansichten aller Jahreszeiten; separater Nacht- und Weihnachtstest.
  const int monate[] = {4,7,10,1,12,12};
  const int tage[] = {12,12,12,15,12,12};
  for (int i = 0; i < 6; i++) {
    const ms_t t = ausOrtszeit(2026,monate[i],tage[i], i == 4 ? 20 : 12,0);
    bereit(t); garten = GartenZustand(); garten.phase = GA_WAHL;
    zeichneAlles(); char name[40]; snprintf(name,sizeof(name),"garten-ansicht-%d",i); foto(name);
    gartenSzeneZeichnen(t); snprintf(name,sizeof(name),"garten-hintergrund-%d",i); foto(name);
  }
  for (Wetter w : {WT_REGEN, WT_SCHNEE}) for (int stunde : {12,20}) {
    ms_t t = 0;
    for (int d = 0; d < 31 && !t; d++) { const ms_t k = ausOrtszeit(2027,1,1,stunde,0) + d*TAG; if (wetterBei(k) == w) t = k; }
    PRUEFE(t != 0, "Datum für Regen oder Schnee gefunden");
    bereit(t); garten.phase = GA_WAHL; zeichneAlles();
    uint8_t vor[9600]; memcpy(vor,bild.getBuffer(),sizeof(vor));
    simMillis += 180; zeichneAlles();
    int bewegt = 0, draussen = 0;
    for (int y = 0; y < 240; y++) for (int x = 0; x < 40; x++) if (vor[y*40+x] != bild.getBuffer()[y*40+x]) { bewegt++; if (y < 27 || y >= 201) draussen++; }
    PRUEFE(bewegt > 0 && draussen == 0, "Niederschlag tags und nachts bewegt sich nur in der Gartenszene");
    char name[40]; snprintf(name,sizeof(name),"garten-wetter-%d-%d",w,stunde); foto(name);
  }
  gartenSchliessen(); bildschirmAus(); const long bilderVorher = simBilder; warten(1500);
  PRUEFE(simBilder == bilderVorher, "ausgeschalteter Bildschirm rendert keinen Garten");
  PRUEFE(simFalschePunkte == 0, "Display stimmt in Garten und Übergängen");
  printf("Spielstand: %zu Bytes, Sammlung: %zu Bytes\n",sizeof(Spielstand),sizeof(Sammlung));
  printf("%s: %d Fehler\n",fehler ? "FEHLER":"OK",fehler); return fehler ? 1 : 0;
}
