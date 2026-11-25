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
void warten(uint32_t ms) { for (uint32_t t = 0; t < ms; t += 10) { simMillis += 10; loop(); } }
void bereit(ms_t jetzt, ms_t alter = 4 * TAG) {
  sideboardSofortZu(); uhrSetzen(jetzt);
  pet = neuesTier(jetzt - alter); pet.aktualisiert = jetzt;
  pet.satt = pet.laune = pet.sauber = 90;
  moment.naechster = jetzt + STUNDE; moment.letzterBesonderer = jetzt;
  spiel = SpielZustand(); ritual.aktiv = false; menueOffen = false;
  bildschirmAn = true; himmel = himmelBei(jetzt);
}
bool warteAuf(SideboardPhase phase,uint32_t grenze=3500){for(uint32_t t=0;t<grenze&&sideboard.phase!=phase;t+=10)warten(10);return sideboard.phase==phase;}
int main() {
  bildschirmStarten(); bild.cp437(true);
  const ms_t jetzt = ausOrtszeit(2026, 11, 24, 14, 0);
  bereit(jetzt); sammlung = Sammlung();
  menueBauen(); bool imMenue = false;
  for (int i = 0; i < menueAnzahl; i++) if (menue[i].e == E_SIDEBOARD) imMenue = true;
  PRUEFE(imMenue, "Sideboard ist im Menü erreichbar");
  aktionAusfuehren(E_SIDEBOARD); warten(500);
  PRUEFE(sideboard.phase == SB_HIN && sideboard.x > 167 && sideboard.x <= 246, "Evi läuft zum Sideboard");
  zeichneAlles(); foto("sideboard-hinweg");
  PRUEFE(warteAuf(SB_OEFFNEN), "Evi steht kurz von hinten vor dem Sideboard");
  zeichneAlles(); foto("sideboard-rueckansicht");
  warten(700); PRUEFE(sideboard.phase == SB_OFFEN, "eigene Sammlungsansicht öffnet sich");
  zeichneAlles(); foto("sideboard-leer");
  PRUEFE(sammlung.anzahl == 0, "Öffnen erzeugt keine erfundenen Funde");
  // Funde vom Gassigehen gibt es seit 25.11.2026 nicht mehr; ältere bleiben in der Sammlung (hier nachgestellt)
  PRUEFE(fundAufnehmen(FUND_STEIN, FUND_GASSI, 20261124, 1), "älterer Fund vom Spaziergang wird aufgenommen");
  PRUEFE(sammlung.anzahl == 1 && sammlung.fund[0].tag == 20261124 && sammlung.fund[0].generation == 1, "Fund merkt echtes Datum und Generation");
  PRUEFE(fundAufnehmen(FUND_BLATT, FUND_GASSI, 20261123, 1), "weiterer Fund kann aufgenommen werden");
  sideboard.auswahl = 0; zeichneAlles(); foto("sideboard-fund");
  knopf(K_R); PRUEFE(sideboard.auswahl == 1, "rechts blättert vor");
  taste(RECHTS); PRUEFE(sideboard.auswahl == 0, "Touch-Notlösung (ohne Tasten): rechts blättert und läuft am Ende um");
  taste(LINKS); PRUEFE(sideboard.auswahl == 1, "Touch-Notlösung (ohne Tasten): links blättert zurück");
  knopf(K_B); PRUEFE(sideboard.phase == SB_HEIM, "B schließt die Sammlung");
  PRUEFE(warteAuf(SB_ZU) && geher.x == 0, "Evi kehrt ins Zimmer zurück");
  zeichneAlles(); foto("sideboard-zurueck");
  // Alle vier Lebensphasen verwenden ihre vorhandenen Lauf- und Rückansichten.
  for (ms_t alter : {3 * STUNDE, 36 * STUNDE, 4 * TAG, 9 * TAG}) {
    bereit(jetzt, alter); sideboardOeffnen(); warteAuf(SB_OEFFNEN);
    PRUEFE(sideboard.phase == SB_OEFFNEN, "Rückansicht in jeder Lebensphase");
    warten(700); knopf(K_RESET);
    PRUEFE(sideboard.phase == SB_ZU && !bildschirmAn, "RESET schließt Sideboard und schaltet aus");
  }
  bereit(ausOrtszeit(2026, 11, 24, 1, 0));
  sideboardOeffnen(); PRUEFE(sideboard.phase == SB_OFFEN, "nachts Sammlung ansehen, ohne Evi aufzuwecken");
  taste(MITTE); PRUEFE(sideboard.phase == SB_ZU && schlaeft(pet, jetztMs()), "Schließen lässt Evi schlafen");
  // Alten Spielstand mit Münzen, Name und Einstellungen verlustfrei übernehmen.
  bereit(jetzt);
  SpielstandV1 alt; memset(&alt, 0, sizeof(alt));
  alt.kennung = SPEICHER_KENNUNG; alt.version = 1; alt.groesse = sizeof(alt);
  alt.tier = pet; strcpy(alt.tier.name, "Meine Evi"); alt.tier.muenzen = 123; alt.tonAn = false;
  alt.pruefsumme = pruefsumme((uint8_t*)&alt, offsetof(SpielstandV1, pruefsumme));
  for (size_t i = 0; i < sizeof(alt); i++) EEPROM.write(i, ((uint8_t*)&alt)[i]);
  Tier geladen; bool ton = true;
  PRUEFE(spielstandLaden(geladen, ton), "Spielstand Version 1 wird übernommen");
  PRUEFE(geladen.muenzen == 123 && !strcmp(geladen.name, "Meine Evi") && !ton && sammlung.anzahl == 0, "Übernahme erhält Bedürfnisse, Namen, Münzen und Ton");
  pet = geladen;
  fundAufnehmen(FUND_FEDER, FUND_GASSI, 20261124, pet.generation);
  const uint16_t generation = pet.generation;
  spielstandSpeichern(pet, ton); sammlung = Sammlung();
  PRUEFE(spielstandLaden(geladen, ton) && sammlung.anzahl == 1 && sammlung.fund[0].art == FUND_FEDER, "Sammlung übersteht Neustart");
  pet.tot = true; aktionAusfuehren(E_NEUES_EI);
  PRUEFE(pet.generation == generation + 1 && sammlung.anzahl == 1 && sammlung.fund[0].generation == generation, "nächstes Ei behält Erbstücke und ihre Herkunft");
  sideboardOeffnen(); zeichneAlles(); foto("sideboard-erbstueck");
  // Neue Arten erweitern den Katalog; die IDs alter Spielstände bleiben unverändert.
  PRUEFE(FUND_STEIN == 0 && FUND_BLATT == 1 && FUND_FEDER == 2 && FUND_ZAPFEN == 3, "bestehende Fund-IDs bleiben gleich");
  {
    const Sammlung vorher = sammlung;
    sammlung = Sammlung();
    for (int art = 0; art < FUNDARTEN; art++) {
      PRUEFE(FUND_NAME[art][0] && FUND_BILD[art] && FUND_BILD[art]->w <= 60 && FUND_BILD[art]->h <= 60,
             "jede Fundart hat Namen und kleines Displaybild");
      PRUEFE(fundAufnehmen((Fundart)art, FUND_GASSI, 20261124, pet.generation), "neue Fundart kann aufgenommen werden");
      sideboard.auswahl = art;
      zeichneAlles();
      char name[40]; snprintf(name, sizeof(name), "fundart-%02d", art); foto(name);
    }
    spielstandSpeichern(pet, ton);
    sammlung = Sammlung();
    PRUEFE(spielstandLaden(geladen, ton) && sammlung.anzahl == FUNDARTEN, "alle Fundarten überstehen einen Neustart");
    for (int art = 0; art < FUNDARTEN; art++)
      PRUEFE(sammlung.fund[art].art == art && sammlung.fund[art].tag == 20261124, "Bildzuordnung und Fundtag bleiben erhalten");
    sammlung = Sammlung();
    // Seit Gassi entfernt ist (25.11.2026), kommen neue Funde nur aus dem Garten: jede Art muss dort vorkommen
    bool gesehen[FUNDARTEN] = {};
    for (int tag = 0; tag < 365; tag++) for (uint8_t st = 0; st < 3; st++) {
      sammlung = Sammlung(); garten.stelle = st; garten.fund = -1;
      gartenErgebnis(jetzt + (ms_t)tag * TAG);
      if (garten.fund >= 0) gesehen[sammlung.fund[garten.fund].art] = true;
    }
    garten = GartenZustand();
    for (int art = 0; art < FUNDARTEN; art++) PRUEFE(gesehen[art], "jede Art ist im Garten erreichbar");
    sammlung = vorher;
  }
  for (int i = sammlung.anzahl; i < SAMMLUNG_MAX; i++) fundAufnehmen(FUND_STEIN, FUND_GASSI, 20261124, 1);
  const Fundstueck erster = sammlung.fund[0];
  PRUEFE(!fundAufnehmen(FUND_BLATT, FUND_GASSI, 20261124, 2) && !memcmp(&erster, &sammlung.fund[0], sizeof(erster)), "volle Sammlung überschreibt keine Erbstücke");
  PRUEFE(sizeof(Spielstand) + 16 <= 4096, "Spielstand und Sammlung passen in den Speicherbereich");
  printf("Display: %ld Bilder, %ld falsche Punkte\n", simBilder, simFalschePunkte);
  PRUEFE(simFalschePunkte == 0, "Display stimmt bei Sammlung und Übergängen");
  printf("%s: %d Fehler\n", fehler ? "FEHLER" : "OK", fehler);
  return fehler ? 1 : 0;
}
