// Three depth zones, placement, surface capacity and saved starter furniture.
// Pixel movement is deliberately configurable; a final furniture slot grid is still open.
#pragma once
enum ZimmerZone : uint8_t { Z_HINTEN, Z_MITTE, Z_VORNE };
enum ZimmerMoebel : uint8_t { Z_BETT, Z_SIDEBOARD, Z_REGAL, Z_TEPPICH, Z_MOEBEL_ANZAHL };
struct ZimmerBild {
 uint16_t w, h;
 const uint8_t *grau, *maske;
 // Aufrufer begrenzen die Koordinaten auf das Bild.
 bool deckt(int x, int y) const {
  const int i = y * w + x;
  return !maske || (maske[i / 8] & (128 >> (i % 8)));
 }
 uint8_t wert(int x, int y) const { return grauStufe(grau, w, x, y) * 17; }
};
#include "zimmergrau.h"

constexpr int ZIMMER_RAHMEN_X = 71, ZIMMER_RAHMEN_Y = 38;
constexpr int ZIMMER_GLAS_DX = 7, ZIMMER_GLAS_DY = 7;
constexpr int ZF_X = ZIMMER_RAHMEN_X + ZIMMER_GLAS_DX, ZF_Y = ZIMMER_RAHMEN_Y + ZIMMER_GLAS_DY;
constexpr int ZF_W = ZG_HUEGEL.w, ZF_H = ZG_HUEGEL.h;
constexpr int BETT_RAND = ZG_TIERBETT.h - ZG_TIERBETT_VORDERRAND.h;

struct ZimmerStellung { int16_t x, fuss; uint8_t aufgestellt; };
struct ZimmerInventar { ZimmerStellung platz[Z_MOEBEL_ANZAHL]; };
struct ZimmerMoebelArt { const char* name; const ZimmerBild* grafik; uint8_t breiteSlots,hoeheSlots,ablagen,tiefe; bool wand,bodenfrei; };
const ZimmerMoebelArt Z_MOEBEL[Z_MOEBEL_ANZAHL]={
 {"Tierbett",&ZG_TIERBETT,2,1,0,18,false,false},
 {"Sideboard",&ZG_SIDEBOARD,2,2,2,13,false,false},
 {"Regal",&ZG_REGAL,2,1,2,0,true,false},
 {"Teppich",&ZG_TEPPICH,3,1,0,0,false,true}
};
inline ZimmerInventar zimmerStart(){return {{{20,167,1},{195,148,1},{195,77,1},{99,194,1}}};}
ZimmerInventar einrichtung=zimmerStart();
const int ZIMMER_VORNE_AB=174, ZIMMER_SCHRITT=4;
inline ZimmerZone zimmerZone(int fuss){return fuss<156?Z_HINTEN:fuss<ZIMMER_VORNE_AB?Z_MITTE:Z_VORNE;}
inline bool zimmerUeberlappt(int x,int y,int w,int h,int a,int b,int c,int d){return x<a+c&&x+w>a&&y<b+d&&y+h>b;}
inline bool zimmerBodenFrei(const ZimmerInventar& z,int x,int y,bool festlich){
 if(x<6||x>314||y<146||y>196)return false;
 for(int i=0;i<Z_MOEBEL_ANZAHL;i++){
  const auto& a=Z_MOEBEL[i];const auto& p=z.platz[i];
  if(!p.aufgestellt||a.wand||a.bodenfrei)continue;
  if(x>=p.x-6&&x<=p.x+a.grafik->w+6&&y>=p.fuss-a.tiefe-2&&y<=p.fuss+2)return false;
 }
 // Fixed seasonal tree in the middle-right corner, never in the front play area.
 if(festlich&&x>=267&&x<=313&&y>=158&&y<=173)return false;
 return true;
}
// A small four-pixel navigation mesh tests actual footprints, not sprite rectangles.
// Only direction changes are returned; movement itself stays continuous.
inline int zimmerPfad(const ZimmerInventar& z,int sx,int sy,int tx,int ty,bool festlich,int* wx,int* wy,int maxWeg){
 const int NX=78,NY=13,N=NX*NY;
 // Reused scratch space keeps the search off the microcontroller's small stack.
 static int16_t vorher[N],queue[N];for(int i=0;i<N;i++)vorher[i]=-2;
 auto naechster=[&](int x,int y){int best=-1,dist=1000000;for(int iy=0;iy<NY;iy++)for(int ix=0;ix<NX;ix++){const int px=6+ix*4,py=146+iy*4;if(!zimmerBodenFrei(z,px,py,festlich))continue;const int d=(px-x)*(px-x)+(py-y)*(py-y);if(d<dist){dist=d;best=iy*NX+ix;}}return best;};
 const int start=naechster(sx,sy),ziel=naechster(tx,ty);if(start<0||ziel<0)return -1;
 int kopf=0,ende=0;queue[ende++]=start;vorher[start]=-1;
 while(kopf<ende&&vorher[ziel]==-2){int at=queue[kopf++],ix=at%NX,iy=at/NX;for(int d=0;d<4;d++){const int nx=ix+(d==0?1:d==1?-1:0),ny=iy+(d==2?1:d==3?-1:0);if(nx<0||nx>=NX||ny<0||ny>=NY)continue;const int n=ny*NX+nx;if(vorher[n]!=-2||!zimmerBodenFrei(z,6+nx*4,146+ny*4,festlich))continue;vorher[n]=at;queue[ende++]=n;}}
 if(vorher[ziel]==-2)return -1;
 int n=0;for(int at=ziel;at>=0;at=vorher[at])queue[n++]=at;
 int anzahl=0,letzteRichtung=-999;
 for(int i=n-1;i>0;i--){const int richtung=queue[i-1]-queue[i];if(richtung!=letzteRichtung){if(anzahl>=maxWeg)return -1;wx[anzahl]=6+(queue[i]%NX)*4;wy[anzahl++]=146+(queue[i]/NX)*4;letzteRichtung=richtung;}}
 if(anzahl>=maxWeg)return -1;
 wx[anzahl]=6+(ziel%NX)*4;wy[anzahl++]=146+(ziel/NX)*4;
 // Retain exact requested endpoints whenever their feet fit the free floor.
 if(zimmerBodenFrei(z,tx,ty,festlich)){if(anzahl>=maxWeg)return -1;wx[anzahl]=tx;wy[anzahl++]=ty;}
 return anzahl;
}
inline bool zimmerGueltig(const ZimmerInventar& z){
 for(int i=0;i<Z_MOEBEL_ANZAHL;i++){
  const auto&a=Z_MOEBEL[i];const auto&p=z.platz[i];if(p.aufgestellt>1)return false;if(!p.aufgestellt)continue;
  const int w=a.grafik->w,h=a.grafik->h;
  if(p.x<4||p.x+w>316)return false;
  if(a.wand){if(p.fuss-h<35||p.fuss>132)return false;}
  else if(a.bodenfrei){if(p.fuss<174||p.fuss>199)return false;}
  else if(p.fuss<146||p.fuss>=ZIMMER_VORNE_AB)return false;
  if(!a.wand&&!a.bodenfrei&&zimmerUeberlappt(p.x,p.fuss-a.tiefe,w,a.tiefe,267,158,46,16))return false;
  const bool wand=a.wand||(!a.bodenfrei&&zimmerZone(p.fuss)==Z_HINTEN);
  if(wand&&zimmerUeberlappt(p.x,p.fuss-h,w,h,ZIMMER_RAHMEN_X,ZIMMER_RAHMEN_Y,ZG_FENSTERRAHMEN.w,ZG_FENSTERRAHMEN.h))return false;
  for(int j=0;j<i;j++){
   const auto&b=Z_MOEBEL[j];const auto&q=z.platz[j];if(!q.aufgestellt||a.bodenfrei||b.bodenfrei)continue;
   const bool andereWand=b.wand||zimmerZone(q.fuss)==Z_HINTEN;
   if(wand&&andereWand&&zimmerUeberlappt(p.x,p.fuss-h,w,h,q.x,q.fuss-b.grafik->h,b.grafik->w,b.grafik->h))return false;
   if(!a.wand&&!b.wand&&zimmerUeberlappt(p.x-4,p.fuss-a.tiefe,w+8,a.tiefe+3,q.x-4,q.fuss-b.tiefe,b.grafik->w+8,b.tiefe+3))return false;
  }
 }
 int wx[24],wy[24];
 for(int saison=0;saison<2;saison++){const bool festlich=saison!=0;
  if(!zimmerBodenFrei(z,167,194,festlich)||!zimmerBodenFrei(z,111,151,festlich))return false;
  if(zimmerPfad(z,167,194,111,151,festlich,wx,wy,24)<0)return false;
  for(int id=Z_BETT;id<=Z_SIDEBOARD;id++)if(z.platz[id].aufgestellt){const auto&p=z.platz[id];const int x=p.x+Z_MOEBEL[id].grafik->w/2,y=p.fuss+5;if(!zimmerBodenFrei(z,x,y,festlich)||zimmerPfad(z,167,194,x,y,festlich,wx,wy,24)<0)return false;}
 }
 return true;
}
inline uint8_t zimmerAblageBreite(bool kerzentablett){return kerzentablett?2:1;}
inline bool zimmerAblagePasst(uint8_t moebel,uint8_t start,uint8_t breite,uint8_t belegt){return moebel<Z_MOEBEL_ANZAHL&&breite>0&&start+breite<=Z_MOEBEL[moebel].ablagen&&!(belegt&(((1u<<breite)-1)<<start));}
// Keep furniture separate from both the v1/v2/v3 pet save and friendship/birthday.
const int ZIMMER_ADRESSE=3328;
struct ZimmerSpeicher {uint32_t kennung;uint16_t version,groesse;ZimmerInventar zimmer;uint32_t crc;};
static_assert(sizeof(Spielstand)+16<=ZIMMER_ADRESSE,"Room overlaps existing pet save");
static_assert(ZIMMER_ADRESSE+sizeof(ZimmerSpeicher)<=FREUND_ADRESSE,"Room overlaps friendship save");
struct ZimmerEditor {bool offen=false,verschieben=false,gueltig=true;uint8_t auswahl=0;ZimmerStellung probe={0,0,0};} zimmerEditor;
