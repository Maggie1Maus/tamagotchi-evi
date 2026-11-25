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
  const ms_t jetzt=ausOrtszeit(2027,4,12,12,0);
  bereit(jetzt); momentStoppen(true);
  PRUEFE(raumSkala(151,false)==650 && raumSkala(194,false)==1000, "Größe wird an der Bodenposition begrenzt");
  int alt=0; bool stetig=true;
  for(int y=130;y<210;y++){const int neu=raumSkala(y,false);stetig &= neu>=alt && (y==130 || neu-alt<=9);alt=neu;}
  PRUEFE(stetig,"Skalierung wächst kontinuierlich statt in drei Sprüngen");
  int phase=1;
  for(ms_t alter : {3*STUNDE,36*STUNDE,4*TAG,9*TAG}) {
    bereit(jetzt,alter); momentStoppen(true); int hoeheVorher=0;
    for(int y : {151,172,194}) {
      geher.fuss=y; geher.x=y==151?-84:0; moment.bild=MB_KEINS;
      zeichneAlles();
      PRUEFE(eviHoehe>hoeheVorher && eviFuss==y && eviOben+eviHoehe<=y+1,"Figur wächst in jeder Altersstufe vom selben Bodenanker aus");
      hoeheVorher=eviHoehe;
      char name[64];snprintf(name,sizeof(name),"tiefe-zimmer-phase-%d-y-%d",phase,y);foto(name);
    }
    phase++;
  }
  bereit(jetzt); momentStoppen(true);
  // Unter der Verdeckung bleiben exakt die Originalpixel des möblierten Zimmers stehen.
  zimmerZeichnen(jetzt); uint8_t original[9600]; memcpy(original,bild.getBuffer(),sizeof(original));
  geher.x=185-BODEN_X; geher.fuss=141;
  raumFigurZeichnen(ART_ERWACHSEN_GLUECKLICH,ERWACHSEN,185,141,false,0,false,false);
  int sichtbar=0,verdecktGeaendert=0;
  for(int y=27;y<201;y++)for(int x=0;x<320;x++) {
    const int i=y*40+x/8; if(!((original[i]^bild.getBuffer()[i])&(0x80>>(x%8))))continue;
    if(raumVerdeckt(false,141,x,y))verdecktGeaendert++;else sichtbar++;
  }
  PRUEFE(sichtbar>5 && verdecktGeaendert==0,"Hinter dem Sideboard bleibt dessen Originalbild unverändert vor Evi");
  bool sichtbarerTreffer=false;
  for(int y=0;y<eviHoehe && !sichtbarerTreffer;y++)for(int x=0;x<eviBreite && !sichtbarerTreffer;x++) {
    const int px=eviLinks+x,py=eviOben+y;
    if(bildDeckt(*figurLage.bild,x*figurLage.bild->w/eviBreite,y*figurLage.bild->h/eviHoehe) && !raumVerdeckt(false,eviFuss,px,py)) sichtbarerTreffer=aufEvi(px,py);
  }
  PRUEFE(!aufEvi(200,130) && sichtbarerTreffer,"Touch ignoriert verdeckte Figurteile und trifft den sichtbaren Kopf");
  zeichneAlles();foto("tiefe-hinter-sideboard");
  zimmerZeichnen(jetzt); memcpy(original,bild.getBuffer(),sizeof(original));
  raumFigurZeichnen(ART_ERWACHSEN_GLUECKLICH,ERWACHSEN,185,194,false,0,false,false);
  int davor=0;
  for(int y=100;y<148;y++)for(int x=195;x<298;x++)if((original[y*40+x/8]^bild.getBuffer()[y*40+x/8])&(0x80>>(x%8)))davor++;
  PRUEFE(davor>5,"Vor dem Sideboard wird Evi vor dessen Konturen gezeichnet");foto("tiefe-vor-sideboard");
  bereit(jetzt); momentStoppen(true); gangBeginnenTief(111-BODEN_X,151);
  int schritte=0; bool durchMoebel=false,diagonal=false;
  while(!gangSchritt() && schritte++<500){
    const int x=BODEN_X+geher.x;
    durchMoebel |= !zimmerBodenFrei(einrichtung,x,geher.fuss,false);
    diagonal |= geher.fuss<RAUM_HEIM_Y;
  }
  PRUEFE(schritte<500 && !durchMoebel && diagonal && geher.x==111-BODEN_X && geher.fuss==151,"Weg zum Fenster hält Abstand zu den aktuellen Möbeln");
  moment.bild=MB_FENSTER;zeichneAlles();foto("tiefe-am-fenster");
  gangBeginnen(0);schritte=0;while(!gangSchritt() && schritte++<500){}
  PRUEFE(geher.x==0 && geher.fuss==RAUM_HEIM_Y,"Rückweg endet am vorderen Teppich");
  // Garten: erst auf der freien Wiese entlang, dann kleiner ins Beet; danach nach vorn kommen.
  bereit(jetzt);momentStoppen(true);sammlung=Sammlung();gartenOeffnen();garten.stelle=2;gartenWeiter();
  warten(450);const int vorne=raumSkala(garten.y,true);
  PRUEFE(garten.y==RAUM_HEIM_Y,"Seitlicher Gartenweg bleibt zunächst vor den Pflanzen");
  warten(750);PRUEFE(garten.y==139 && raumSkala(garten.y,true)<vorne,"Evi geht zum Beet in die Tiefe und wird kleiner");
  zeichneAlles();foto("tiefe-garten-im-beet");
  PRUEFE(raumVerdeckt(true,139,205,136),"Vordere Beetkante liegt vor Evis Pfoten");
  warten(1000);gartenWeiter();warten(1100);
  PRUEFE(garten.phase==GA_HERAUS,"Nach dem Untersuchen kommt Evi aus dem Beet hervor");
  warten(800);PRUEFE(garten.y==184 && (garten.phase==GA_ZEIGEN || garten.phase==GA_LEER),"Ergebnis wird weiter vorn und größer gezeigt");
  zeichneAlles();foto("tiefe-garten-wieder-vorne");
  gartenWeiter();warten(800);PRUEFE(garten.phase==GA_WAHL && garten.y==RAUM_HEIM_Y,"A führt vollständig zurück auf die freie Wiese");
  knopf(K_RESET);PRUEFE(!bildschirmAn && garten.phase==GA_ZU && geher.fuss==RAUM_HEIM_Y,"RESET räumt alle Tiefenpositionen auf");
  // Tatsächlich gerendertes Fenster: Wetter bewegt ausschließlich die vier Scheiben.
  for(Wetter w : {WT_REGEN,WT_SCHNEE}) for(int stunde : {12,20}) {
    ms_t t=0;
    for(int d=0;d<40 && !t;d++){const ms_t k=ausOrtszeit(2027,1,1,stunde,0)+d*TAG;if(wetterBei(k)==w)t=k;}
    bereit(t);momentStoppen(true);zimmerZeichnen(t);memcpy(original,bild.getBuffer(),sizeof(original));
    simMillis+=220;zimmerZeichnen(t);int bewegt=0,ausserhalb=0;
    for(int y=0;y<240;y++)for(int x=0;x<320;x++)if((original[y*40+x/8]^bild.getBuffer()[y*40+x/8])&(0x80>>(x%8))){bewegt++;if(!zimmerScheibe(x,y))ausserhalb++;}
    PRUEFE(t && bewegt>0 && ausserhalb==0,"Neuer Fensterausblick animiert Regen/Schnee tags/nachts nur hinter dem Rahmen");
    char n[64];snprintf(n,sizeof(n),"tiefe-fenster-wetter-%d-%d",w,stunde);zeichneAlles();foto(n);
  }
  bereit(jetzt);momentStoppen(true);spiel.aktiv=true;zeichneEvi(jetzt,S_GLUECKLICH,ERWACHSEN);
  PRUEFE(eviHoehe==ART_ERWACHSEN_GLUECKLICH.h && eviFuss==BODEN_Y,"Minispiele behalten ihre eigenen Größen und Bodenkoordinaten");
  spiel.aktiv=false;
  for(int weihnachten=0;weihnachten<2;weihnachten++) {
    const ms_t t=ausOrtszeit(2026,weihnachten?12:7,12,12,0);bereit(t);momentStoppen(true);zeichneAlles();foto(weihnachten?"tiefe-zimmer-weihnachten":"tiefe-zimmer-sommer");
  }
  PRUEFE(simFalschePunkte==0,"Display stimmt bei Raumtiefe und Szenenwechseln");
  printf("%s: %d Fehler\n",fehler?"FEHLER":"OK",fehler);return fehler?1:0;
}
