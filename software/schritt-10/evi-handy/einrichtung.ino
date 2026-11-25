// Approved v2 room layers and the first usable furniture editor (Waveshare only).
bool zimmerBit(const uint8_t* p,int w,int x,int y){return p[y*((w+7)/8)+x/8]&(0x80>>(x%8));}
void zimmerEbene(const EviGrafik& a,int x,int y){
 for(int yy=0;yy<a.h;yy++)for(int xx=0;xx<a.w;xx++)if(!a.maske||zimmerBit(a.maske,a.w,xx,yy))bild.drawPixel(x+xx,y+yy,zimmerBit(a.linien,a.w,xx,yy)?0:1);
}
bool zimmerPunkt(const EviGrafik& a,int x,int y,bool vorher){
 if(x<0||y<0||x>=a.w||y>=a.h||(a.maske&&!zimmerBit(a.maske,a.w,x,y)))return vorher;
 return zimmerBit(a.linien,a.w,x,y);
}
// Exactly the 51 native combinations in the approved archive, clipped before the frame.
void zimmerFensterNeu(Jahreszeit j,Himmel h,Wetter w,int phase){
 if(j!=JZ_WINTER&&w==WT_SCHNEE)w=WT_BEWOELKT;
 const int f=phase%16;
 const int lx=19+(int)lround(sin(f*2*M_PI/16)*2),ly=28+f*18/16;
 const EviGrafik& sky=*Z_HIMMEL[h][w],&zeichen=*Z_ZEICHEN[h][w],&wolken=*Z_WOLKEN[h][w],&baum=*Z_BAEUME[j];
 for(int y=0;y<53;y++)for(int x=0;x<66;x++){
  if(!zimmerBit(EVI_FENSTER_SCHEIBENMASKE.linien,66,x,y))continue;
  bool schwarz=zimmerPunkt(sky,x,y,false);
  schwarz=zimmerPunkt(zeichen,x,y,schwarz);schwarz=zimmerPunkt(wolken,x,y,schwarz);
  schwarz=zimmerPunkt(EVI_LANDSCHAFT_HUEGEL_BODEN,x,y,schwarz);schwarz=zimmerPunkt(baum,x,y,schwarz);
  if(j==JZ_HERBST){schwarz=zimmerPunkt(EVI_HERBSTLAUB_BODEN,x,y,schwarz);schwarz=zimmerPunkt(EVI_HERBSTBLATT,x-lx,y-ly,schwarz);}
  if(w==WT_REGEN)schwarz=zimmerPunkt(*Z_REGEN[f],x,y,schwarz);
  if(w==WT_SCHNEE)schwarz=zimmerPunkt(*Z_SCHNEE[f],x,y,schwarz);
  bild.drawPixel(79+x,46+y,schwarz?0:1);
 }
}
void zimmerMoebelZeichnen(int id){const auto&p=einrichtung.platz[id];if(p.aufgestellt)zimmerEbene(*Z_MOEBEL[id].grafik,p.x,p.fuss-Z_MOEBEL[id].grafik->h);}
void zimmerNeuZeichnen(ms_t jetzt){
 bild.fillScreen(1);zimmerEbene(EVI_RAUMSTRUKTUR,0,27);
 const Wetter w=wetterBei(jetzt);const int dauer=w==WT_REGEN?110:w==WT_SCHNEE?190:220;
 zimmerFensterNeu(jahreszeitBei(jetzt),himmelBei(jetzt),w,(millis()/dauer)%16);
 zimmerEbene(EVI_FENSTER_RAHMEN,71,38);
 zimmerMoebelZeichnen(Z_TEPPICH);
 const bool festlich=weihnachtsZeit(jetzt);
 if(!festlich)zimmerMoebelZeichnen(Z_REGAL);
 int a=Z_BETT,b=Z_SIDEBOARD;if(einrichtung.platz[a].fuss>einrichtung.platz[b].fuss){int t=a;a=b;b=t;}
 zimmerMoebelZeichnen(a);zimmerMoebelZeichnen(b);
 if(festlich){
  const int h=100,w=WEIHNACHTSBAUM.w*h/WEIHNACHTSBAUM.h;
  raumRasterZeichnen(WEIHNACHTSBAUM,317-w,173-h,w,h,false,false,false,173);
  const int sw=95,sh=FENSTERSTERNE.h*sw/FENSTERSTERNE.w;
  raumRasterZeichnen(FENSTERSTERNE,64,29,sw,sh,false,false,false,0);
  const auto&p=einrichtung.platz[Z_SIDEBOARD];
  if(p.aufgestellt&&zimmerAblagePasst(Z_SIDEBOARD,0,zimmerAblageBreite(true),0))zeichneBild(KERZEN,p.x+(EVI_SIDEBOARD.w-KERZEN.w)/2,p.fuss-EVI_SIDEBOARD.h+6-KERZEN.h);
 }
}
bool zimmerNeuVerdeckt(int fuss,int x,int y){
 for(int i=0;i<Z_MOEBEL_ANZAHL;i++){const auto&a=Z_MOEBEL[i];const auto&p=einrichtung.platz[i];if(!p.aufgestellt||a.wand||a.bodenfrei||fuss>=p.fuss)continue;
  const int sx=x-p.x,sy=y-(p.fuss-a.grafik->h);if(sx>=0&&sy>=0&&sx<a.grafik->w&&sy<a.grafik->h&&(!a.grafik->maske||zimmerBit(a.grafik->maske,a.grafik->w,sx,sy)))return true;
 }
 if(weihnachtsZeit(jetztMs())&&fuss<173){const int h=100,w=WEIHNACHTSBAUM.w*h/WEIHNACHTSBAUM.h,sx=x-(317-w),sy=y-73;if(sx>=0&&sy>=0&&sx<w&&sy<h&&bildDeckt(WEIHNACHTSBAUM,sx*WEIHNACHTSBAUM.w/w,sy*WEIHNACHTSBAUM.h/h))return true;}
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
