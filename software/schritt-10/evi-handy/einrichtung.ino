// Graustufen-Ebenen des Waveshare-Zimmers und Einrichtungsmodus.
void zimmerEbene(const ZimmerBild& ebene, int x, int y) {
 for (int yy = 0; yy < ebene.h; yy++)
  for (int xx = 0; xx < ebene.w; xx++)
   if (ebene.deckt(xx, yy)) bild.drawPixel(x + xx, y + yy, ebene.wert(xx, yy));
}

void zimmerFensterNeu(Jahreszeit j, Himmel h, Wetter w, int phase) {
 if (j != JZ_WINTER && w == WT_SCHNEE) w = WT_BEWOELKT;
 const int f = phase % 16;
 zimmerEbene(*ZG_HIMMEL[h][w], ZF_X, ZF_Y);
 if (w == WT_SONNE || w == WT_SONNE_WOLKEN) zimmerEbene(*ZG_ZEICHEN[h], ZF_X, ZF_Y);
 zimmerEbene(*ZG_WOLKEN[h][w], ZF_X, ZF_Y);
 zimmerEbene(ZG_HUEGEL, ZF_X, ZF_Y);
 if (j == JZ_WINTER) zimmerEbene(ZG_SCHNEE_BODEN, ZF_X, ZF_Y);
 zimmerEbene(*ZG_BAEUME[h][j], ZF_X, ZF_Y);
 if (w == WT_SCHNEE) zimmerEbene(ZG_SCHNEE_AESTE, ZF_X, ZF_Y);
 if (j == JZ_HERBST) {
  zimmerEbene(ZG_LAUB_HERBST, ZF_X, ZF_Y);
  const int lx = 19 + (int)lround(sin(f * 2 * M_PI / 16) * 2);
  zimmerEbene(ZG_HERBSTBLATT, ZF_X + lx, ZF_Y + 28 + f * 18 / 16);
 }
 if (w == WT_REGEN) zimmerEbene(*ZG_REGEN[f], ZF_X, ZF_Y);
 if (w == WT_SCHNEE) zimmerEbene(*ZG_SCHNEE[f], ZF_X, ZF_Y);
 zimmerEbene(ZG_FENSTERRAHMEN, ZIMMER_RAHMEN_X, ZIMMER_RAHMEN_Y);
}

void fensterZeichnen(ms_t jetzt) {
 const Wetter w = wetterBei(jetzt);
 const int dauer = w == WT_REGEN ? 110 : w == WT_SCHNEE ? 190 : 220;
 zimmerFensterNeu(jahreszeitBei(jetzt), himmelBei(jetzt), w, (millis() / dauer) % 16);
}

// Die Zimmerlampe ändert die Innenbeleuchtung. Der Ausblick behält seine eigenen Helligkeiten.
static void zimmerAbdunkeln() {
 uint8_t* puffer = bild.getBuffer();
 for (int y = STATUS_H; y < ZEILE_Y; y++) {
  uint8_t* zeile = puffer + y * BREITE;
  for (int x = 0; x < BREITE; x++)
   if (!zimmerScheibe(x, y)) zeile[x] = zimmerDunkelwert(zeile[x]);
 }
 bild.markieren();
}
// Weihnachtsbaum: steht rechts auf dem Boden (Fuß bei y = 173) und reicht bis unter die Balkenleiste oben
// (Wunsch der Nutzerin vom 25.11.2026: „viel größer, bis zum oberen Rand“; vorher 100 Punkte hoch).
const int XBAUM_FUSS = 173, XBAUM_H = XBAUM_FUSS - 28, XBAUM_W = WEIHNACHTSBAUM.w * XBAUM_H / WEIHNACHTSBAUM.h;
const int XBAUM_X = 317 - XBAUM_W, XBAUM_Y = XBAUM_FUSS - XBAUM_H;
void zimmerMoebelZeichnen(int id){const auto&p=einrichtung.platz[id];if(p.aufgestellt)zimmerEbene(*Z_MOEBEL[id].grafik,p.x,p.fuss-Z_MOEBEL[id].grafik->h);}
void zimmerNeuZeichnen(ms_t jetzt){
 bild.fillScreen(1);zimmerEbene(ZG_WAND_BODEN,0,STATUS_H);
 fensterZeichnen(jetzt);
 zimmerMoebelZeichnen(Z_TEPPICH);
 const bool festlich=weihnachtsZeit(jetzt);
 if(!festlich)zimmerMoebelZeichnen(Z_REGAL);
 int a=Z_BETT,b=Z_SIDEBOARD;if(einrichtung.platz[a].fuss>einrichtung.platz[b].fuss){int t=a;a=b;b=t;}
 // Das Kerzentablett gehört zum Sideboard: direkt danach zeichnen, damit Bett und Weihnachtsbaum davor bleiben,
 // wenn sie weiter vorne stehen (am 25.11.2026 stand es sonst vor dem großen Baum).
 auto kerzen=[&](int id){
  const auto&p=einrichtung.platz[Z_SIDEBOARD];
  if(festlich&&id==Z_SIDEBOARD&&p.aufgestellt&&zimmerAblagePasst(Z_SIDEBOARD,0,zimmerAblageBreite(true),0))zeichneBild(KERZEN,p.x+(ZG_SIDEBOARD.w-KERZEN.w)/2,p.fuss-ZG_SIDEBOARD.h+6-KERZEN.h);
 };
 zimmerMoebelZeichnen(a);kerzen(a);zimmerMoebelZeichnen(b);kerzen(b);
 if(festlich){
  raumRasterZeichnen(WEIHNACHTSBAUM,XBAUM_X,XBAUM_Y,XBAUM_W,XBAUM_H,false,false,false,XBAUM_FUSS);
  const int sw=95,sh=FENSTERSTERNE.h*sw/FENSTERSTERNE.w;
  raumRasterZeichnen(FENSTERSTERNE,64,29,sw,sh,false,false,false,0);
 }
 if (pet.lichtAus) zimmerAbdunkeln();
}
bool zimmerNeuVerdeckt(int fuss,int x,int y){
 for(int i=0;i<Z_MOEBEL_ANZAHL;i++){const auto&a=Z_MOEBEL[i];const auto&p=einrichtung.platz[i];if(!p.aufgestellt||a.wand||a.bodenfrei||fuss>=p.fuss)continue;
  const int sx=x-p.x,sy=y-(p.fuss-a.grafik->h);if(sx>=0&&sy>=0&&sx<a.grafik->w&&sy<a.grafik->h&&a.grafik->deckt(sx,sy))return true;
 }
 if(weihnachtsZeit(jetztMs())&&fuss<XBAUM_FUSS){const int sx=x-XBAUM_X,sy=y-XBAUM_Y;if(sx>=0&&sy>=0&&sx<XBAUM_W&&sy<XBAUM_H&&bildDeckt(WEIHNACHTSBAUM,sx*WEIHNACHTSBAUM.w/XBAUM_W,sy*WEIHNACHTSBAUM.h/XBAUM_H))return true;}
 return false;
}
void zimmerLaden(){
 ZimmerSpeicher s;uint8_t*b=(uint8_t*)&s;for(size_t i=0;i<sizeof(s);i++)b[i]=EEPROM.read(ZIMMER_ADRESSE+i);
 einrichtung=zimmerStart();
 if(s.kennung==0x45565A31&&s.version==1&&s.groesse==sizeof(s)&&s.crc==pruefsumme(b,offsetof(ZimmerSpeicher,crc))&&zimmerGueltig(s.zimmer))einrichtung=s.zimmer;
}
void zimmerSpeichern(){
 ZimmerSpeicher s;memset(&s,0,sizeof(s));s.kennung=0x45565A31;s.version=1;s.groesse=sizeof(s);
 // Assign fields individually: padding stays zero, so an unchanged room never
 // triggers another flash write just because temporary struct padding differed.
 for(int i=0;i<Z_MOEBEL_ANZAHL;i++){s.zimmer.platz[i].x=einrichtung.platz[i].x;s.zimmer.platz[i].fuss=einrichtung.platz[i].fuss;s.zimmer.platz[i].aufgestellt=einrichtung.platz[i].aufgestellt;}
 s.crc=pruefsumme((const uint8_t*)&s,offsetof(ZimmerSpeicher,crc));
 const uint8_t*b=(const uint8_t*)&s;bool anders=false;for(size_t i=0;i<sizeof(s);i++)if(EEPROM.read(ZIMMER_ADRESSE+i)!=b[i]){EEPROM.write(ZIMMER_ADRESSE+i,b[i]);anders=true;}if(anders)EEPROM.commit();
}
void zimmerEinrichten(){
 momentStoppen(true);evi.reaktion=R_KEINE;sagen.bis=0;zimmerEditor=ZimmerEditor();zimmerEditor.offen=true;menueOffen=false;
}
void zimmerEditorPruefen(){ZimmerInventar probe=einrichtung;probe.platz[zimmerEditor.auswahl]=zimmerEditor.probe;zimmerEditor.gueltig=zimmerGueltig(probe);}
void zimmerEditorStart(){zimmerEditor.probe=einrichtung.platz[zimmerEditor.auswahl];zimmerEditor.probe.aufgestellt=1;zimmerEditor.verschieben=true;zimmerEditorPruefen();}
void zimmerEditorKnopf(Knopf k){
 if(k==K_B||k==K_START){if(zimmerEditor.verschieben)zimmerEditor.verschieben=false;else zimmerEditor.offen=false;return;}
 const int id=zimmerEditor.auswahl;
 if(!zimmerEditor.verschieben){
  if(k==K_UP||k==K_L)zimmerEditor.auswahl=(id+Z_MOEBEL_ANZAHL-1)%Z_MOEBEL_ANZAHL;
  if(k==K_DOWN)zimmerEditor.auswahl=(id+1)%Z_MOEBEL_ANZAHL;
  if(k==K_A)zimmerEditorStart();
  if(k==K_R){if(einrichtung.platz[id].aufgestellt){einrichtung.platz[id].aufgestellt=0;zimmerSpeichern();}else zimmerEditorStart();}
  return;
 }
 if(k==K_A){if(zimmerEditor.gueltig){einrichtung.platz[id]=zimmerEditor.probe;zimmerSpeichern();zimmerEditor.verschieben=false;}else zeigeMeldung("Hier bleibt zu wenig Platz.");return;}
 auto&p=zimmerEditor.probe;const auto&a=Z_MOEBEL[id];
 if(k==K_L)p.x-=ZIMMER_SCHRITT;
 if(k==K_R)p.x+=ZIMMER_SCHRITT;
 if(k==K_UP)p.fuss-=ZIMMER_SCHRITT;
 if(k==K_DOWN)p.fuss+=ZIMMER_SCHRITT;
 p.x=max(0,min((int)p.x,320-(int)a.grafik->w));p.fuss=max(a.wand?35:146,min((int)p.fuss,a.wand?132:199));zimmerEditorPruefen();
}
void zimmerEditorTaste(Taste t){zimmerEditorKnopf(t==LINKS?K_DOWN:t==MITTE?K_A:K_B);}
void zimmerEditorTouch(int x,int y){
 if(y>=201){zimmerEditorKnopf(x<110?K_R:x<220?K_A:K_B);return;}
 if(!zimmerEditor.verschieben){if(x>=166&&y>=66&&y<160)zimmerEditor.auswahl=min(Z_MOEBEL_ANZAHL-1,(y-66)/23);return;}
 auto&p=zimmerEditor.probe;const auto&a=Z_MOEBEL[zimmerEditor.auswahl];p.x=max(4,min(x-a.grafik->w/2,316-(int)a.grafik->w));p.fuss=max(a.wand?35:146,min(y,a.wand?132:199));zimmerEditorPruefen();
}
void zimmerEditorZeichnen(){
 const ZimmerInventar vorher=einrichtung;if(zimmerEditor.verschieben)einrichtung.platz[zimmerEditor.auswahl]=zimmerEditor.probe;
 zimmerNeuZeichnen(jetztMs());einrichtung=vorher;
 const auto&a=Z_MOEBEL[zimmerEditor.auswahl];const auto&p=zimmerEditor.probe;
 if(zimmerEditor.verschieben){
  bild.drawRect(p.x-2,p.fuss-a.grafik->h-2,a.grafik->w+4,a.grafik->h+4,0);
  bild.fillRect(0,201,320,39,1);schreibe(bild,7,205,a.name,0);
  const char* grund=zimmerEditor.gueltig?"A aufstellen · B zurück":!a.wand&&!a.bodenfrei&&p.fuss>=ZIMMER_VORNE_AB?"Vorne bleibt Platz zum Spielen.":"Fenster, Möbel oder Weg versperrt.";
  schreibe(bild,7,219,grund,0);
 }else{
  bild.fillRoundRect(166,40,150,135,5,1);bild.drawRoundRect(166,40,150,135,5,0);schreibe(bild,174,48,"Einrichten",0,1,true);
  for(int i=0;i<Z_MOEBEL_ANZAHL;i++){const int y=68+i*23;if(i==zimmerEditor.auswahl)bild.fillRect(169,y-2,143,21,0);const uint16_t f=i==zimmerEditor.auswahl?1:0;schreibe(bild,174,y,Z_MOEBEL[i].name,f);schreibe(bild,174,y+10,!einrichtung.platz[i].aufgestellt?"im Inventar":i==Z_REGAL&&weihnachtsZeit(jetztMs())?"Weihnachtspause":"im Zimmer",f);}
  bild.fillRect(0,201,320,39,1);schreibe(bild,6,205,"A verschieben · R ins Inventar",0);schreibe(bild,6,219,"Steuerkreuz wählen · B fertig",0);
 }
 zeichneStatus(phaseBei(pet,jetztMs()));zeigen();
}
