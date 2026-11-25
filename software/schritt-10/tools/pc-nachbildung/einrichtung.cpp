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
  meldungBis=0;sagen.bis=0;
}
// Graustufen-Fenster (zimmergrau.h, 25.11.2026): mittlere Helligkeit der oberen Scheibenzeilen und helle Punkte
int fensterMittel(int y0,int y1){long s=0;int n=0;for(int y=y0;y<y1;y++)for(int x=0;x<66;x++)if(zimmerScheibe(ZF_X+x,ZF_Y+y)){s+=bild.stufe(ZF_X+x,ZF_Y+y);n++;}return n?(int)(s/n):-1;}
int fensterHelle(){int n=0;for(int y=0;y<53;y++)for(int x=0;x<66;x++)if(zimmerScheibe(ZF_X+x,ZF_Y+y)&&bild.stufe(ZF_X+x,ZF_Y+y)>=12)n++;return n;}
void nachthimmelPruefen(Jahreszeit j,Wetter w){
 bild.fillScreen(1);zimmerFensterNeu(j,HIMMEL_TAG,w,0);const int tag=fensterMittel(0,10);
 bild.fillScreen(1);zimmerFensterNeu(j,HIMMEL_NACHT,w,0);const int nacht=fensterMittel(0,10);
 PRUEFE(tag>=0&&nacht>=0&&nacht<tag,"Nachthimmel ist dunkler als der Tageshimmel");
 if(w==WT_SONNE){PRUEFE(fensterHelle()>5,"klare Nacht zeigt Mond und Sterne");}
}
void fensterPalettenPruefen(){
 // Glas und Rahmen schließen lückenlos aneinander: jeder Punkt im Fensterbereich gehört zur Scheibe ODER zum Rahmen
 int luecken=0,doppelt=0,glas=0;
 for(int y=0;y<53;y++)for(int x=0;x<66;x++){
  const bool scheibe=zimmerBit(ZIMMER_SCHEIBE,66,x,y);
  const bool rahmen=grauStufe(ZG_M_EVI_FENSTER_RAHMEN,81,x+7,y+7)!=15;
  glas+=scheibe;luecken+=!scheibe&&!rahmen;doppelt+=scheibe&&rahmen;
 }
 PRUEFE(glas>2900&&luecken==0&&doppelt==0,"Fensterscheibe und neuer Rahmen schließen ohne Lücke aneinander");
 PRUEFE(ZF_X==71+7&&ZF_Y==38+7,"Ausblick sitzt bei (7, 7) im Fensterrahmen");
 bild.fillScreen(1);zimmerFensterNeu(JZ_SOMMER,HIMMEL_DAEMMERUNG,WT_SONNE,0);int daemm=fensterMittel(0,10);
 PRUEFE(daemm>0,"Dämmerung wird gezeichnet");
 // The legacy renderer used h != DAY and accidentally drew a moon at twilight.
 ms_t zeit=0;for(int tag=0;tag<50&&!zeit;tag++){const ms_t t=sonnenzeiten(ausOrtszeit(2027,1,7,12,0)+tag*TAG).untergang+MINUTE;if(himmelBei(t)==HIMMEL_DAEMMERUNG&&wetterBei(t)==WT_SONNE)zeit=t;}
 bool keinMond=true;int himmelPunkte=0;
 if(zeit){bild.fillScreen(1);fensterZeichnen(zeit);const Bild& baum=*FENSTER_JAHRESZEIT[jahreszeitBei(zeit)];for(int y=55;y<76;y++)for(int x=68;x<89;x++)if(!bildDeckt(baum,x-GLAS_X,y-GLAS_Y)){const bool hell=bild.bits()[y*40+x/8]&(128>>(x%8));keinMond&=hell==bool((x+y)&1);himmelPunkte++;}}
 PRUEFE(zeit&&himmelPunkte>10&&keinMond,"Auch der ältere Fensterzeichner blendet den Mond erst nachts ein");
 for(int h=0;h<3;h++){bild.fillScreen(1);zimmerFensterNeu(JZ_SOMMER,(Himmel)h,WT_SONNE,0);zimmerEbene(EVI_FENSTER_RAHMEN,71,38);char name[40];snprintf(name,sizeof(name),"v2-palette-%d",h);foto(name);}
}
int main(){
 bildschirmStarten();bild.cp437(true);bereit(ausOrtszeit(2027,7,12,12,0));momentStoppen(true);
 fensterPalettenPruefen();
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
 // Export every real firmware window; a Python check compares its glass pixels
 // with the independently produced approved PNGs, not with this renderer's logic.
 int kombinationen=0;
 for(int j=0;j<4;j++)for(int h=0;h<3;h++)for(int w=0;w<5;w++){
  if(w==WT_SCHNEE&&j!=JZ_WINTER)continue;
  bild.fillScreen(1);zimmerFensterNeu((Jahreszeit)j,(Himmel)h,(Wetter)w,0);
  char name[50];snprintf(name,sizeof(name),"v2-window-%d-%d-%d",j,h,w);foto(name);kombinationen++;
  if(h==HIMMEL_NACHT)nachthimmelPruefen((Jahreszeit)j,(Wetter)w);
  if(w==WT_REGEN||w==WT_SCHNEE){uint8_t vorher[9600];memcpy(vorher,bild.bits(),9600);zimmerFensterNeu((Jahreszeit)j,(Himmel)h,(Wetter)w,4);int bewegt=0,ausserhalb=0;for(int y=0;y<240;y++)for(int x=0;x<320;x++)if((vorher[y*40+x/8]^bild.bits()[y*40+x/8])&(0x80>>(x%8))){bewegt++;if(!zimmerScheibe(x,y))ausserhalb++;}PRUEFE(bewegt>0&&ausserhalb==0,"Niederschlag bewegt nur die tatsächlichen Fensterscheiben");}
 }
 PRUEFE(kombinationen==51,"Alle gültigen Jahreszeit-Wetter-Tageszeit-Kombinationen exportiert");
 bereit(ausOrtszeit(2027,7,12,12,0));momentStoppen(true);zeichneAlles();foto("v2-zimmer-sommer");
 bereit(ausOrtszeit(2026,12,12,12,0));momentStoppen(true);zeichneAlles();foto("v2-zimmer-weihnachten");
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
 PRUEFE(eviLinks>=bp.x&&eviLinks+eviBreite<=bp.x+(int)EVI_TIERBETT.w&&eviOben<bp.fuss,"Evi liegt im Bett, nicht daneben");
 // Der vordere Rand liegt vor Evi, ihr Fell ist deckend: im Bereich über dem Rand keine Bettlinien in Evi
 bool randDavor=true;{const int oben=bp.fuss-EVI_TIERBETT.h;for(int x=0;x<(int)EVI_TIERBETT.w;x++){const int y=EVI_TIERBETT.h-3;const bool l=zimmerBit(EVI_TIERBETT.linien,EVI_TIERBETT.w,x,y);if(l&&(bild.bits()[(oben+y)*40+(bp.x+x)/8]&(0x80>>((bp.x+x)%8))))randDavor=false;}}
 PRUEFE(randDavor,"vorderer Rand des Korbs liegt vor Evi");
 pet.wachBis=jetztMs()+30*MINUTE;   // aufgeweckt
 for(int i=0;i<400&&bettZustand.phase!=BETT_NEIN;i++)warten(120);
 PRUEFE(bettZustand.phase==BETT_NEIN&&geher.x==0&&geher.fuss==RAUM_HEIM_Y,"aufgewacht: zurück auf den Teppich");
 bereit(ausOrtszeit(2026,11,26,0,30));pet.lichtAus=true;bettZustand.phase=BETT_NEIN;geher.x=0;geher.fuss=RAUM_HEIM_Y;bildschirmAus();warten(300);
 PRUEFE(bettZustand.phase==BETT_LIEGT,"bei dunklem Bildschirm liegt sie gleich im Bett");
 bildschirmEin();einrichtung.platz[Z_BETT].aufgestellt=false;warten(1000);
 PRUEFE(bettZustand.phase!=BETT_LIEGT,"Bett im Inventar: sie schläft nicht in der Luft");
 einrichtung=zimmerStart();
 // Graustufen (seit 25.11.2026): Evi wird mit Zwischenstufen gezeichnet, Zimmer ebenfalls
 {bereit(ausOrtszeit(2026,7,12,12,0));momentStoppen(true);zeichneAlles();
  bool stufen[16]={};for(int y=eviOben;y<eviOben+eviHoehe;y++)for(int x=eviLinks;x<eviLinks+eviBreite;x++)stufen[bild.stufe(x,y)]=true;
  int n=0;for(bool b:stufen)n+=b;
  PRUEFE(n>=6,"Evi wird in mehreren Graustufen gezeichnet");
  int mitGrau=0;for(int i=0;i<GRAU_ANZAHL;i++){const Bild&b=*GRAU_TABELLE[i].alt;if(b.w>0&&b.h>0&&grauDaten(b)==GRAU_TABELLE[i].daten)mitGrau++;}
  PRUEFE(GRAU_ANZAHL==70&&mitGrau==70,"alle 70 Graustufen-Bilder gehören zu einem vorhandenen Bild");
  bool raumStufen[16]={};for(int y=30;y<200;y++)for(int x=0;x<320;x++)raumStufen[bild.stufe(x,y)]=true;
  int rn=0;for(bool b:raumStufen)rn+=b;
  PRUEFE(rn>=8,"Zimmer wird in Graustufen gezeichnet");
  PRUEFE(bild.stufe(0,239)==15&&GrauCanvas::wert(1)==255&&GrauCanvas::wert(0)==0,"altes Zeichnen: Farbe 1 ist weiß, 0 schwarz");}
 PRUEFE(simFalschePunkte==0,"Display bei neuem Zimmer und Einrichtung pixelgenau");
 printf("%s: %d Fehler\n",fehler?"FEHLER":"OK",fehler);return fehler?1:0;
}
