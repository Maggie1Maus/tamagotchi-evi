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
  meldungBis=0;sagen.bis=0;
}
void fotoGrau(const char* name, int x0, int y0, int w, int h) {
  char pfad[100]; snprintf(pfad, sizeof(pfad), "bilder/%s.pgm", name);
  FILE* f = fopen(pfad, "wb");
  if (!f) { PRUEFE(false, "Graustufenfoto lässt sich schreiben"); return; }
  fprintf(f, "P5\n%d %d\n255\n", w, h);
  for (int y = y0; y < y0 + h; y++)
    fwrite(bild.getBuffer() + y * BREITE + x0, 1, w, f);
  fclose(f);
}

void fensterPruefen() {
  bool glasPasst = ZF_X == 78 && ZF_Y == 45 && ZF_W == 66 && ZF_H == 53;
  for (int y = -1; y <= ZF_H; y++) for (int x = -1; x <= ZF_W; x++) {
    const bool scheibe = x >= 0 && x < ZF_W && y >= 0 && y < ZF_H &&
                         (x < 31 || x >= 35) && (y < 26 || y >= 30);
    glasPasst &= zimmerScheibe(ZF_X + x, ZF_Y + y) == scheibe;
  }
  PRUEFE(glasPasst, "Fensterscheiben schließen am vorgesehenen Platz lückenlos mit Rahmen und Kreuz ab");

  int kombinationen = 0;
  bool animationPasst = true;
  for (int j = 0; j < 4; j++) for (int h = 0; h < 3; h++) for (int w = 0; w < 5; w++) {
    if (w == WT_SCHNEE && j != JZ_WINTER) continue;
    const bool bewegt = w == WT_REGEN || w == WT_SCHNEE || j == JZ_HERBST;
    uint8_t vorher[(ZG_FENSTERRAHMEN.w + 2) * (ZG_FENSTERRAHMEN.h + 2)];
    bool geaendert = false;
    for (int f = 0; f < (bewegt ? 16 : 1); f++) {
      bild.fillScreen(1);
      zimmerFensterNeu((Jahreszeit)j, (Himmel)h, (Wetter)w, f);
      char name[64]; snprintf(name, sizeof(name), "grau-fenster-%d-%d-%d-%d", j, h, w, f);
      const int x0 = ZIMMER_RAHMEN_X - 1, y0 = ZIMMER_RAHMEN_Y - 1;
      const int breite = ZG_FENSTERRAHMEN.w + 2, hoehe = ZG_FENSTERRAHMEN.h + 2;
      fotoGrau(name, x0, y0, breite, hoehe);
      for (int y = 0; y < hoehe; y++) for (int x = 0; x < breite; x++) {
        const int px = x0 + x, py = y0 + y, i = y * breite + x;
        const uint8_t wert = bild.getBuffer()[py * BREITE + px];
        if (f == 0) vorher[i] = wert;
        else if (vorher[i] != wert) {
          geaendert = true;
          animationPasst &= zimmerScheibe(px, py);
        }
      }
    }
    if (bewegt) animationPasst &= geaendert;
    kombinationen++;
  }
  PRUEFE(kombinationen == 51, "Alle 51 Fensterkombinationen exportiert");
  PRUEFE(animationPasst, "Regen, Schnee und Herbstblatt bewegen nur die Fensterscheiben");
}

void fensterHelligkeitPruefen() {
  bool sonnigHell = true, baeumeHell = true;
  for (int j = 0; j < 4; j++) {
    zimmerFensterNeu((Jahreszeit)j, HIMMEL_TAG, WT_SONNE, 0);
    sonnigHell &= bild.stufe(ZF_X + 29, ZF_Y + 24) >= 14;
    const ZimmerBild& b = *ZG_BAEUME[HIMMEL_TAG][j];
    long summe = 0, n = 0;
    for (int y = 0; y < b.h; y++) for (int x = 0; x < b.w; x++)
      if (b.deckt(x, y)) { summe += b.wert(x, y); n++; }
    baeumeHell &= n > 0 && summe / n >= 153;
  }
  PRUEFE(sonnigHell, "Sonniger Tageshimmel ist in allen Jahreszeiten fast weiß");
  PRUEFE(baeumeHell, "Alle vier Tagesbäume sind freundlich hell statt dunkler Silhouetten");
  zimmerFensterNeu(JZ_WINTER, HIMMEL_TAG, WT_SONNE, 0);
  long schnee = 0;
  for (int y = 48; y < 52; y++) for (int x = 2; x < 29; x++)
    schnee += bild.getBuffer()[(ZF_Y + y) * BREITE + ZF_X + x];
  PRUEFE(schnee / (4 * 27) >= 238, "Winterboden bleibt auch bei Sonne hell und freundlich");
  const int klar = ZG_HIMMEL[HIMMEL_TAG][WT_SONNE]->wert(0, 0);
  const int regen = ZG_HIMMEL[HIMMEL_TAG][WT_REGEN]->wert(0, 0);
  const int abend = ZG_HIMMEL[HIMMEL_DAEMMERUNG][WT_REGEN]->wert(0, 0);
  const int nacht = ZG_HIMMEL[HIMMEL_NACHT][WT_REGEN]->wert(0, 0);
  PRUEFE(klar > regen && regen > abend && abend > nacht,
         "Sonne, Regen, Dämmerung und Nacht haben schlüssige Helligkeiten");
  bool schwarz = true;
  for (int w = 0; w < 5; w++) for (int y = 0; y < 53; y++) for (int x = 0; x < 66; x++)
    schwarz &= ZG_HIMMEL[HIMMEL_NACHT][w]->wert(x, y) == 0;
  PRUEFE(schwarz, "Freier Nachthimmel bleibt bei allen Wetterlagen vollständig schwarz");
}

void zimmerLichtPruefen() {
  const ms_t mittag = ausOrtszeit(2027, 7, 12, 12, 0);
  const ms_t zeiten[] = {mittag, sonnenzeiten(mittag).untergang + MINUTE,
                         ausOrtszeit(2027, 7, 12, 1, 0)};
  const char* fotos[][2] = {{"licht-tag-an", "licht-tag-aus"},
                            {"licht-abend-an", "licht-abend-aus"},
                            {"licht-nacht-an", "licht-nacht-aus"}};
  static uint8_t tag[BREITE * HOEHE], hell[BREITE * HOEHE];
  bool innenGleich = true, aussenWechselt = false, schalterPasst = true;
  for (int i = 0; i < 3; i++) {
    bereit(zeiten[i]);
    schalterPasst &= himmelBei(zeiten[i]) == (Himmel)i;
    pet.lichtAus = false;
    zimmerNeuZeichnen(zeiten[i]);
    memcpy(hell, bild.getBuffer(), sizeof(hell));
    if (i == 0) memcpy(tag, hell, sizeof(tag));
    for (int y = STATUS_H; y < ZEILE_Y; y++) for (int x = 0; x < BREITE; x++) {
      const int p = y * BREITE + x;
      if (zimmerScheibe(x, y)) aussenWechselt |= tag[p] != hell[p];
      else innenGleich &= tag[p] == hell[p];
    }
    pet.lichtAus = true;
    zimmerNeuZeichnen(zeiten[i]);
    bool aussenGleich = true, innenDunkler = true;
    for (int y = STATUS_H; y < ZEILE_Y; y++) for (int x = 0; x < BREITE; x++) {
      const int p = y * BREITE + x;
      const uint8_t dunkel = bild.getBuffer()[p];
      if (zimmerScheibe(x, y)) aussenGleich &= dunkel == hell[p];
      else innenDunkler &= hell[p] <= 17 ? dunkel == hell[p] : dunkel < hell[p];
    }
    schalterPasst &= aussenGleich && innenDunkler;
    pet.lichtAus = false;
    zeichneAlles();
    fotoGrau(fotos[i][0], 0, 0, BREITE, HOEHE);
    pet.lichtAus = true;
    zeichneAlles();
    fotoGrau(fotos[i][1], 0, 0, BREITE, HOEHE);
  }
  PRUEFE(innenGleich && aussenWechselt, "Bei Licht an bleibt das Zimmer tagsüber, abends und nachts gleich hell");
  PRUEFE(schalterPasst, "Licht aus dunkelt das Zimmer ab und lässt die Fensterscheiben hell wie zuvor");

  bild.fillScreen(204);
  zeichneUhrzeit(mittag);
  bool nebenDerUhrFrei = true;
  for (int y = STATUS_H + 1; y < STATUS_H + 27; y++)
    for (int x = UHRZEIT_X + textBreite("12:00") + 4; x < UHRZEIT_X + textBreite("12:00") + 33; x++)
      nebenDerUhrFrei &= bild.getBuffer()[y * BREITE + x] == 204;
  PRUEFE(nebenDerUhrFrei, "Neben der Uhr wird kein Wettersymbol gezeichnet");
}

void parkPruefen() {
  const ms_t ersterTag = ausOrtszeit(2027, 7, 12, 12, 0);
  bereit(ersterTag);
  parkHintergrund(JZ_SOMMER, HIMMEL_TAG, WT_SONNE_WOLKEN);
  const int sommerwiese = bild.stufe(280, STATUS_H + 150);
  parkHintergrund(JZ_WINTER, HIMMEL_TAG, WT_SONNE_WOLKEN);
  const int winterwiese = bild.stufe(280, STATUS_H + 150);
  parkHintergrund(JZ_WINTER, HIMMEL_NACHT, WT_SONNE_WOLKEN);
  PRUEFE(winterwiese > sommerwiese && bild.stufe(280, STATUS_H + 150) < winterwiese,
         "Der Park zeigt Winter und Nacht mit eigener Helligkeit");
  bool bewegtSich = true;
  static uint8_t vorher[BREITE * HOEHE];
  for (Wetter wetter : {WT_REGEN, WT_SCHNEE}) {
    bild.fillScreen(1);
    parkHintergrund(JZ_WINTER, HIMMEL_TAG, wetter);
    parkNiederschlag(wetter, HIMMEL_TAG, 100);
    memcpy(vorher, bild.getBuffer(), sizeof(vorher));
    parkHintergrund(JZ_WINTER, HIMMEL_TAG, wetter);
    parkNiederschlag(wetter, HIMMEL_TAG, 300);
    bewegtSich &= memcmp(vorher + STATUS_H * BREITE, bild.getBuffer() + STATUS_H * BREITE,
                          (ZEILE_Y - STATUS_H) * BREITE) != 0;
    bewegtSich &= !memcmp(vorher, bild.getBuffer(), STATUS_H * BREITE);
    bewegtSich &= !memcmp(vorher + ZEILE_Y * BREITE, bild.getBuffer() + ZEILE_Y * BREITE,
                           (HOEHE - ZEILE_Y) * BREITE);
  }
  PRUEFE(bewegtSich, "Regen und Schnee bewegen sich innerhalb der Parkszene");
  sammlung = {};
  PRUEFE(fundAufnehmen(FUND_FEDER, FUND_GASSI, 20260701, pet.generation), "Alter Fund vor Parkbesuch vorhanden");
  menueZeigen(true);
  int eintrag = -1;
  for (int i = 0; i < menueAnzahl; i++) if (menue[i].e == E_PARK) eintrag = i;
  PRUEFE(eintrag >= 0, "In den Park ist im normalen Menü erreichbar");
  if (eintrag < 0) return;
  for (int i = 0; i < eintrag; i++) knopf(K_DOWN);
  knopf(K_A);
  PRUEFE(park.phase == PARK_WAHL && park.fuesse.x == 62, "Evi kommt am linken Kiesweg an");
  zeichneAlles();
  fotoGrau("park-eingang-spiel", 0, 0, BREITE, HOEHE);

  knopf(K_UP);
  knopf(K_A);
  warten(350);
  PRUEFE(park.phase == PARK_LAEUFT && park.fuesse.x > 62, "Evi läuft zum gewählten Ziel selbstständig los");
  warten(1800);
  PRUEFE(park.phase == PARK_GRUSS && park.ziel == PARK_ZIEL_BESUCH, "Steuerkreuz und A führen zur Begrüßung bei Lotti");

  beruehrt(179, STATUS_H + 149);
  warten(2300);
  PRUEFE(park.phase == PARK_HILFE && sammlung.anzahl == 1, "Antippen führt zur Fundstelle und Evi wartet auf Hilfe");
  beruehrt(179, STATUS_H + 149);
  warten(700);
  PRUEFE(park.phase == PARK_FUND && park.fund == 1 && sammlung.fund[1].art == FUND_STEIN,
         "Mit Hilfe beim Nachsehen findet Evi ihren ersten Stein");
  zeichneAlles();
  fotoGrau("park-erster-fund", 0, 0, BREITE, HOEHE);
  knopf(K_B);
  PRUEFE(!parkOffen(), "B führt aus dem Park ins Zimmer");
  Tier geladen;
  bool ton;
  PRUEFE(spielstandLaden(geladen, ton) && sammlung.anzahl == 2 &&
         sammlung.fund[0].ort == FUND_GASSI && sammlung.fund[1].ort == FUND_PARK &&
         sammlung.fund[1].tag == tagKennung(ersterTag) && sammlung.fund[1].stelle == PARK_KIESWEG,
         "Alter Fund und neuer Parkfund überstehen Neustart mit richtiger Herkunft und Datum");
  PRUEFE(parkFundstelle(ersterTag) == -1, "Neustart gibt am selben Tag keinen weiteren Parkfund");
  aktionAusfuehren(E_SIDEBOARD);
  warten(3500);
  PRUEFE(sideboard.phase == SB_OFFEN && sideboard.auswahl == 1, "Der Parkfund ist direkt im Sideboard zu sehen");
  sideboardSofortZu();

  const ms_t naechsterTag = ersterTag + TAG;
  bereit(naechsterTag);
  aktionAusfuehren(E_PARK);
  const int stelle = parkFundstelle(naechsterTag);
  PRUEFE(stelle == PARK_KIESWEG || stelle == PARK_WIESE, "Am nächsten Tag gibt es erneut eine Fundstelle");
  if (stelle == PARK_WIESE) knopf(K_DOWN);
  knopf(K_A);
  warten(2300);
  knopf(K_A);
  warten(700);
  PRUEFE(park.phase == PARK_FUND && sammlung.anzahl == 3 && sammlung.fund[2].art != FUND_STEIN &&
         sammlung.fund[2].tag == tagKennung(naechsterTag), "Der nächste Parkfund ist ein anderer Gegenstand");
  knopf(K_RESET);
  PRUEFE(!parkOffen() && !bildschirmAn, "RESET schließt auch den Park");
}

int main(){
 bildschirmStarten();bild.cp437(true);bereit(ausOrtszeit(2027,7,12,12,0));momentStoppen(true);
 fensterPruefen();
 fensterHelligkeitPruefen();
 zimmerLichtPruefen();
 bereit(ausOrtszeit(2027,7,12,12,0));momentStoppen(true);
 PRUEFE(zimmerGueltig(einrichtung),"Starterzimmer mit freien Interaktionswegen gültig");
 PRUEFE(zimmerZone(151)==Z_HINTEN&&zimmerZone(164)==Z_MITTE&&zimmerZone(194)==Z_VORNE,"Drei unsichtbare Tiefenzonen");
 ZimmerInventar probe=einrichtung;probe.platz[Z_BETT].fuss=181;
 PRUEFE(!zimmerGueltig(probe),"Möbel blockieren die vordere Spielfläche nicht");
 probe=einrichtung;probe.platz[Z_REGAL]={202,114,1};
 PRUEFE(!zimmerGueltig(probe),"Hinteres Sideboard blockiert belegte Wandfläche");
 probe.platz[Z_SIDEBOARD]={160,164,1};
 PRUEFE(zimmerGueltig(probe),"Möbel in der Mitte dürfen Wandmöbel optisch verdecken");
 PRUEFE(zimmerAblagePasst(Z_SIDEBOARD,0,2,0)&&!zimmerAblagePasst(Z_SIDEBOARD,1,2,0)&&!zimmerAblagePasst(Z_SIDEBOARD,0,1,3),"Kerzentablett belegt beide Sideboardplätze");
 PRUEFE(zimmerAblagePasst(Z_SIDEBOARD,0,1,0)&&zimmerAblagePasst(Z_SIDEBOARD,1,1,1),"Zwei kleine Gegenstände passen nebeneinander");
 menueBauen();bool imMenue=false;for(int i=0;i<menueAnzahl;i++)imMenue|=menue[i].e==E_EINRICHTEN;
 PRUEFE(imMenue,"Einrichtungsmodus ist im Menü erreichbar");
 zimmerEinrichten();knopf(K_A);knopf(K_R);const int altX=einrichtung.platz[Z_BETT].x;
 PRUEFE(zimmerEditor.probe.x==altX+4&&einrichtung.platz[Z_BETT].x==altX,"Verschieben ist zunächst nur eine Vorschau");
 knopf(K_B);PRUEFE(einrichtung.platz[Z_BETT].x==altX&&!zimmerEditor.verschieben,"Abbrechen erhält die vorige Position");
 knopf(K_A);knopf(K_L);knopf(K_A);
 PRUEFE(einrichtung.platz[Z_BETT].x==altX-4&&!zimmerEditor.verschieben,"A stellt Möbel an gültiger Stelle auf");
 knopf(K_R);PRUEFE(!einrichtung.platz[Z_BETT].aufgestellt,"Möbel werden im Inventar aufbewahrt");
 zimmerSpeichern();einrichtung=zimmerStart();zimmerLaden();PRUEFE(!einrichtung.platz[Z_BETT].aufgestellt&&einrichtung.platz[Z_BETT].x==altX-4,"Inventar und Position überstehen Neustart");
 knopf(K_A);knopf(K_A);PRUEFE(einrichtung.platz[Z_BETT].aufgestellt,"Inventarobjekt kann wieder aufgestellt werden");
 zimmerEditor.auswahl=Z_BETT;zimmerEditorStart();zimmerEditor.probe.fuss=190;zimmerEditorPruefen();knopf(K_A);
 PRUEFE(!zimmerEditor.gueltig&&einrichtung.platz[Z_BETT].fuss==167,"Ungültiges Aufstellen verändert weder Zimmer noch Speicher");
 knopf(K_RESET);PRUEFE(!zimmerEditor.offen&&!bildschirmAn,"RESET verwirft die Vorschau und schaltet aus");
 bildschirmAn=true;einrichtung=zimmerStart();
 // Existing pet and friendship/birthday bytes must survive the separate room write.
 pet.muenzen=123;spielstandSpeichern(pet,false);uint8_t vor[4096];memcpy(vor,EEPROM.daten,4096);
 einrichtung.platz[Z_BETT].x=24;zimmerSpeichern();bool getrennt=true;
 for(int i=0;i<4096;i++)if((i<ZIMMER_ADRESSE||i>=ZIMMER_ADRESSE+(int)sizeof(ZimmerSpeicher))&&vor[i]!=EEPROM.daten[i])getrennt=false;
 Tier geladen;bool ton=true;PRUEFE(getrennt&&spielstandLaden(geladen,ton)&&geladen.muenzen==123&&!ton,"Möbel überschreiben weder Evi noch Freundschaft und Geburtstag");
 EEPROM.write(ZIMMER_ADRESSE+12,EEPROM.read(ZIMMER_ADRESSE+12)^1);zimmerLaden();
 PRUEFE(einrichtung.platz[Z_BETT].x==20,"Beschädigter Möbelstand fällt auf Starterzimmer zurück");
 bereit(ausOrtszeit(2027,7,12,12,0));momentStoppen(true);gangBeginnenTief(111-BODEN_X,151);int n=0;bool frei=true;
 while(!gangSchritt()&&n++<500)frei&=zimmerBodenFrei(einrichtung,BODEN_X+geher.x,geher.fuss,false);
 PRUEFE(n<500&&frei&&geher.x==111-BODEN_X&&geher.fuss==151,"Kontinuierlicher Gang folgt einem kollisionsfreien Weg zum Fenster");
 einrichtung.platz[Z_SIDEBOARD].aufgestellt=0;sideboardOeffnen();PRUEFE(sideboard.phase==SB_OFFEN,"Sammlung bleibt bei eingelagertem Sideboard erreichbar");sideboardSofortZu();einrichtung=zimmerStart();
 bereit(ausOrtszeit(2027,7,12,12,0));momentStoppen(true);zeichneAlles();foto("v2-zimmer-sommer");fotoGrau("grau-zimmer-sommer",0,0,BREITE,HOEHE);
 bereit(ausOrtszeit(2026,12,12,12,0));momentStoppen(true);zeichneAlles();foto("v2-zimmer-weihnachten");fotoGrau("grau-zimmer-weihnachten",0,0,BREITE,HOEHE);
 zimmerEinrichten();zeichneAlles();foto("v2-einrichten-auswahl");knopf(K_A);zeichneAlles();foto("v2-einrichten-verschieben");
 zimmerEditor.offen=false;
 // Weihnachtsbaum reicht bis unter die Balkenleiste (Wunsch der Nutzerin vom 25.11.2026)
 PRUEFE(XBAUM_Y<=28&&XBAUM_FUSS==173&&XBAUM_X+XBAUM_W<=318,"Weihnachtsbaum vom Boden bis an den oberen Rand des Zimmers");
 // Evi schläft im Tierbett (bett.ino): hinlaufen, liegen, zurück; dunkel gleich ins Bett
 einrichtung=zimmerStart();bereit(ausOrtszeit(2026,11,25,23,30));pet.lichtAus=true;pet.wachBis=0;
 for(int i=0;i<400&&bettZustand.phase!=BETT_LIEGT;i++)warten(120);
 PRUEFE(bettZustand.phase==BETT_LIEGT,"schläft sie ein, läuft sie ins Tierbett");
 zeichneAlles();foto("bett-schlaeft");
 const auto&bp=einrichtung.platz[Z_BETT];
 PRUEFE(eviLinks>=bp.x&&eviLinks+eviBreite<=bp.x+(int)ZG_TIERBETT.w&&eviOben<bp.fuss,"Evi liegt im Bett, nicht daneben");
 // Die sichtbaren Pixel des vorderen Korbrands behalten ihre Graustufe vor Evi.
 bool randDavor = true;
 const auto& rand = ZG_TIERBETT_VORDERRAND;
 for (int y = 0; y < rand.h; y++) for (int x = 0; x < rand.w; x++) {
   if (!rand.deckt(x, y)) continue;
   const int px = bp.x + x, py = bp.fuss - rand.h + y;
   if (!raumVerdeckt(false, bp.fuss, px, py)) {
     const uint8_t wert = pet.lichtAus ? zimmerDunkelwert(rand.wert(x, y)) : rand.wert(x, y);
     randDavor &= bild.getBuffer()[py * BREITE + px] == wert;
   }
 }
 PRUEFE(randDavor,"vorderer Rand des Korbs liegt vor Evi");
 pet.wachBis=jetztMs()+30*MINUTE;   // aufgeweckt
 for(int i=0;i<400&&bettZustand.phase!=BETT_NEIN;i++)warten(120);
 PRUEFE(bettZustand.phase==BETT_NEIN&&geher.x==0&&geher.fuss==RAUM_HEIM_Y,"aufgewacht: zurück auf den Teppich");
 bereit(ausOrtszeit(2026,11,26,0,30));pet.lichtAus=true;bettZustand.phase=BETT_NEIN;geher.x=0;geher.fuss=RAUM_HEIM_Y;bildschirmAus();warten(300);
 PRUEFE(bettZustand.phase==BETT_LIEGT,"bei dunklem Bildschirm liegt sie gleich im Bett");
 bildschirmEin();einrichtung.platz[Z_BETT].aufgestellt=false;warten(1000);
 PRUEFE(bettZustand.phase!=BETT_LIEGT,"Bett im Inventar: sie schläft nicht in der Luft");
 einrichtung=zimmerStart();
 // Evi und Zimmer werden gemeinsam in Graustufen gezeichnet.
 {bereit(ausOrtszeit(2026,7,12,12,0));momentStoppen(true);zeichneAlles();
  bool stufen[16]={};for(int y=eviOben;y<eviOben+eviHoehe;y++)for(int x=eviLinks;x<eviLinks+eviBreite;x++)stufen[bild.stufe(x,y)]=true;
  int n=0;for(bool b:stufen)n+=b;
  PRUEFE(n>=6,"Evi wird in mehreren Graustufen gezeichnet");
  int mitGrau=0;for(int i=0;i<GRAU_ANZAHL;i++){const Bild&b=*GRAU_TABELLE[i].alt;if(b.w>0&&b.h>0&&grauDaten(b)==GRAU_TABELLE[i].daten)mitGrau++;}
  PRUEFE(mitGrau==GRAU_ANZAHL,"alle Evi-Graustufen-Bilder gehören zu einem vorhandenen Bild");
  bool zimmerGrau=false;for(int y=30;y<60;y++)for(int x=10;x<60;x++){const int l=bild.stufe(x,y);if(l!=0&&l!=15)zimmerGrau=true;}
  PRUEFE(zimmerGrau,"Zimmer zeigt Graustufen");
  PRUEFE(bild.stufe(0,239)==15&&GrauCanvas::wert(1)==255&&GrauCanvas::wert(0)==0,"altes Zeichnen: Farbe 1 ist weiß, 0 schwarz");}
 parkPruefen();
 PRUEFE(simFalschePunkte==0,"Display bei Zimmer, Park und Einrichtung pixelgenau");
 printf("%s: %d Fehler\n",fehler?"FEHLER":"OK",fehler);return fehler?1:0;
}
