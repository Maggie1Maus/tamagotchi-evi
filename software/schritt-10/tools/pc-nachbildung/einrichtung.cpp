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
  gartenSchliessen(); sideboardSofortZu(); uhrSetzen(jetzt);
  pet = neuesTier(jetzt - alter); pet.aktualisiert = jetzt;
  pet.satt = pet.laune = pet.sauber = 90;
  moment.naechster = jetzt + STUNDE; moment.letzterBesonderer = jetzt;
  spiel = SpielZustand(); ritual.aktiv = false; menueOffen = false;
  bildschirmAn = true; himmel = himmelBei(jetzt);
  meldungBis=0;sagen.bis=0;
}
bool fensterEbeneDeckt(const EviGrafik& a,int x,int y){
 return x>=0&&y>=0&&x<a.w&&y<a.h&&(!a.maske||zimmerBit(a.maske,a.w,x,y));
}
bool fensterHell(int x,int y){return bild.bits()[(46+y)*40+(79+x)/8]&(128>>((79+x)%8));}
void nachthimmelPruefen(Jahreszeit j,Wetter w){
 int himmel=0;bool schwarz=true;
 for(int y=0;y<40;y++)for(int x=0;x<66;x++){
  if(!zimmerScheibe(79+x,46+y)||fensterEbeneDeckt(*Z_BAEUME_NACHT[j],x,y)||fensterEbeneDeckt(*Z_ZEICHEN[HIMMEL_NACHT][w],x,y)||fensterEbeneDeckt(*Z_WOLKEN[HIMMEL_NACHT][w],x,y)||fensterEbeneDeckt(EVI_LANDSCHAFT_HUEGEL_BODEN,x,y))continue;
  if(j==JZ_HERBST&&(fensterEbeneDeckt(EVI_HERBSTLAUB_BODEN,x,y)||fensterEbeneDeckt(EVI_HERBSTBLATT,x-19,y-28)))continue;
  if((w==WT_REGEN&&fensterEbeneDeckt(*Z_REGEN[0],x,y))||(w==WT_SCHNEE&&fensterEbeneDeckt(*Z_SCHNEE[0],x,y)))continue;
  himmel++;schwarz&=!fensterHell(x,y);
 }
 PRUEFE(himmel>200&&schwarz,"Nachthimmel ist bei jeder Jahreszeit und jedem Wetter durchgehend schwarz");
 if(w==WT_REGEN||w==WT_SCHNEE){
  bool weiss=true;int punkte=0;
  for(int f=0;f<16;f++){
   zimmerFensterNeu(j,HIMMEL_NACHT,w,f);const EviGrafik& a=*(w==WT_REGEN?Z_REGEN[f]:Z_SCHNEE[f]);
   for(int y=0;y<53;y++)for(int x=0;x<66;x++)if(zimmerScheibe(79+x,46+y)&&zimmerBit(a.linien,66,x,y)){punkte++;weiss&=fensterHell(x,y);}
  }
  PRUEFE(punkte>100&&weiss,"Regen und Schnee bleiben nachts in allen 16 Animationsphasen weiß sichtbar");
  zimmerFensterNeu(j,HIMMEL_NACHT,w,0);
 }
}
void fensterPalettenPruefen(){
 uint8_t abend[9600];
 for(int j=0;j<4;j++){
  bild.fillScreen(1);zimmerFensterNeu((Jahreszeit)j,HIMMEL_DAEMMERUNG,WT_SONNE,0);memcpy(abend,bild.bits(),sizeof(abend));
  zimmerFensterNeu((Jahreszeit)j,HIMMEL_NACHT,WT_SONNE,0);
  const EviGrafik& original=*Z_BAEUME[j];int koerper=0,rand=0;bool weiss=true,schwarz=true,hellerRand=true;
  for(int y=1;y<45;y++)for(int x=1;x<65;x++){
   if(!zimmerScheibe(79+x,46+y)||!zimmerBit(original.maske,66,x,y))continue;
   const int i=(46+y)*40+(79+x)/8,b=128>>((79+x)%8);const bool vor=abend[i]&b,nach=bild.bits()[i]&b;
   if(zimmerBit(original.linien,66,x,y)){koerper++;weiss&=vor;schwarz&=!nach;}
   bool aussen=false;for(int dy=-1;dy<=1;dy++)for(int dx=-1;dx<=1;dx++)aussen|=!zimmerBit(original.maske,66,x+dx,y+dy);
   if(aussen&&!zimmerBit(original.linien,66,x,y)){rand++;hellerRand&=nach&&!vor;}
  }
  PRUEFE(koerper>20&&weiss&&schwarz,"Baumkörper aller Jahreszeiten ist abends weiß und nachts schwarz");
  PRUEFE(rand>10&&hellerRand,"Baumrand aller Jahreszeiten ist abends dunkel und nachts weiß");
 }
 bild.fillScreen(1);zimmerFensterNeu(JZ_SOMMER,HIMMEL_DAEMMERUNG,WT_SONNE,0);bool leererHimmel=true;
 for(int y=1;y<38;y++)for(int x=1;x<29;x++)if(zimmerScheibe(79+x,46+y)){
  const bool hell=bild.bits()[(46+y)*40+(79+x)/8]&(128>>((79+x)%8));
  leererHimmel&=hell!=zimmerBit(Z_HIMMEL[HIMMEL_DAEMMERUNG][WT_SONNE]->linien,66,x,y);
 }
 PRUEFE(leererHimmel,"Dämmerung zeigt keinen Mond oder mondähnlichen Himmelskörper");
 bild.fillScreen(1);zimmerFensterNeu(JZ_SOMMER,HIMMEL_NACHT,WT_SONNE,0);int mond=0;bool weisserMond=true;
 for(int y=1;y<23;y++)for(int x=1;x<25;x++)if(zimmerScheibe(79+x,46+y)){
  const bool hell=bild.bits()[(46+y)*40+(79+x)/8]&(128>>((79+x)%8));
  if(zimmerBit(Z_ZEICHEN[HIMMEL_NACHT][WT_SONNE]->linien,66,x,y)){mond++;weisserMond&=hell;}
 }
 PRUEFE(mond>10&&weisserMond,"Der Mond ist erst nachts weiß im schwarzen Himmel sichtbar");
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
 // Graustufen (seit 25.11.2026): Evi wird mit Zwischenstufen gezeichnet, das Zimmer bleibt Schwarz-Weiß
 {bereit(ausOrtszeit(2026,7,12,12,0));momentStoppen(true);zeichneAlles();
  bool stufen[16]={};for(int y=eviOben;y<eviOben+eviHoehe;y++)for(int x=eviLinks;x<eviLinks+eviBreite;x++)stufen[bild.stufe(x,y)]=true;
  int n=0;for(bool b:stufen)n+=b;
  PRUEFE(n>=6,"Evi wird in mehreren Graustufen gezeichnet");
  int mitGrau=0;for(int i=0;i<GRAU_ANZAHL;i++){const Bild&b=*GRAU_TABELLE[i].alt;if(b.w>0&&b.h>0&&grauDaten(b)==GRAU_TABELLE[i].daten)mitGrau++;}
  PRUEFE(GRAU_ANZAHL==82&&mitGrau==82,"alle 82 Graustufen-Bilder gehören zu einem vorhandenen Bild");
  bool zimmerGrau=false;for(int y=30;y<60;y++)for(int x=10;x<60;x++){const int l=bild.stufe(x,y);if(l!=0&&l!=15)zimmerGrau=true;}
  PRUEFE(!zimmerGrau,"Zimmer bleibt vorerst Schwarz-Weiß");
  PRUEFE(bild.stufe(0,239)==15&&GrauCanvas::wert(1)==255&&GrauCanvas::wert(0)==0,"altes Zeichnen: Farbe 1 ist weiß, 0 schwarz");}
 PRUEFE(simFalschePunkte==0,"Display bei neuem Zimmer und Einrichtung pixelgenau");
 printf("%s: %d Fehler\n",fehler?"FEHLER":"OK",fehler);return fehler?1:0;
}
