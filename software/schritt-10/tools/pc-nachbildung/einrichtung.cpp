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
  meldungBis=0;sagen.bis=0;
}
int main(){
 bildschirmStarten();bild.cp437(true);bereit(ausOrtszeit(2027,7,12,12,0));momentStoppen(true);
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
  if(w==WT_REGEN||w==WT_SCHNEE){uint8_t vorher[9600];memcpy(vorher,bild.getBuffer(),9600);zimmerFensterNeu((Jahreszeit)j,(Himmel)h,(Wetter)w,4);int bewegt=0,ausserhalb=0;for(int y=0;y<240;y++)for(int x=0;x<320;x++)if((vorher[y*40+x/8]^bild.getBuffer()[y*40+x/8])&(0x80>>(x%8))){bewegt++;if(!zimmerScheibe(x,y))ausserhalb++;}PRUEFE(bewegt>0&&ausserhalb==0,"Niederschlag bewegt nur die tatsächlichen Fensterscheiben");}
 }
 PRUEFE(kombinationen==51,"Alle gültigen Jahreszeit-Wetter-Tageszeit-Kombinationen exportiert");
 bereit(ausOrtszeit(2027,7,12,12,0));momentStoppen(true);zeichneAlles();foto("v2-zimmer-sommer");
 bereit(ausOrtszeit(2026,12,12,12,0));momentStoppen(true);zeichneAlles();foto("v2-zimmer-weihnachten");
 zimmerEinrichten();zeichneAlles();foto("v2-einrichten-auswahl");knopf(K_A);zeichneAlles();foto("v2-einrichten-verschieben");
 PRUEFE(simFalschePunkte==0,"Display bei neuem Zimmer und Einrichtung pixelgenau");
 printf("%s: %d Fehler\n",fehler?"FEHLER":"OK",fehler);return fehler?1:0;
}
