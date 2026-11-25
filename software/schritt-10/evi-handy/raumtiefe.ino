int raumSkala(int fuss, bool draussen) {
  const int hinten = draussen ? 139 : 151;
  const int t = max(0, min(1000, (fuss - hinten) * 1000 / (RAUM_HEIM_Y - hinten)));
  return 650 + t * 350 / 1000;
}
// Evi ist seit 25.11.2026 um 10 % größer (Wunsch der Nutzerin: „geht im Zimmer ein bisschen verloren“).
// Im Tierbett bleibt sie in der alten Größe, sonst passt die Oma nicht in den Korb (bett.ino).
const int EVI_GROESSE_PROZENT = 110;
int raumFigurHoehe(const Bild& b, Phase ph, int fuss, bool draussen, bool fundPose) {
  static const int HOEHE[6] = {34,54,64,68,64,38};
  static const int FUND_BASIS[5] = {1,70,82,86,82};
  if (ph > TOT) return 1;
  const int norm = ph == EI || ph == TOT ? b.h : fundPose ? FUND_BASIS[ph] : ART[ph][1]->h;
  return max(1, (int)b.h * HOEHE[ph] * (raumImBett ? 100 : EVI_GROESSE_PROZENT) / 100 * raumSkala(fuss,draussen) / (max(1,norm) * 1000));
}
bool raumMaske(const uint8_t* maske, int x, int y) {
  return x >= 0 && x < 320 && y >= 27 && y < 201 && (maske[(y-27)*40+x/8] & (0x80>>(x%8)));
}
bool raumVerdeckt(bool draussen, int fuss, int x, int y) {
  if (draussen) {
    if (fuss < 143 && raumMaske(TIEFE_GARTEN_BEET,x,y)) return true;
    if (fuss < 163 && raumMaske(TIEFE_GARTEN_BAUM,x,y)) return true;
    if (fuss < 178 && raumMaske(TIEFE_GARTEN_BUSCH,x,y)) return true;
  } else {
    return zimmerNeuVerdeckt(fuss,x,y);
  }
  return false;
}
void raumRasterZeichnen(const Bild& b, int x0, int y0, int w, int h, bool spiegeln, bool tiefentest, bool draussen, int fuss) {
  if (const uint8_t* g = grauDaten(b)) {   // Graustufen: Mittel über den Quellbereich jedes Zielpunkts (grau.h)
    for (int y=0;y<h;y++) for(int x=0;x<w;x++) {
      const int a0=spiegeln ? b.w-(x+1)*b.w/w : x*b.w/w, a1=spiegeln ? b.w-x*b.w/w : (x+1)*b.w/w;
      const int s0=y*b.h/h, s1=(y+1)*b.h/h;
      uint16_t wert;
      if (!grauMitteln(g,b.w,a0,max(a1,a0+1),s0,max(s1,s0+1),wert)) continue;
      const int px=x0+x, py=y0+y;
      if (tiefentest && raumVerdeckt(draussen,fuss,px,py)) continue;
      bild.drawPixel(px,py,wert);
    }
    return;
  }
  for (int y=0;y<h;y++) for(int x=0;x<w;x++) {
    const int sx=spiegeln ? b.w-1-x*b.w/w : x*b.w/w, sy=y*b.h/h;
    if (!bildDeckt(b,sx,sy)) continue;
    const int px=x0+x, py=y0+y;
    if (tiefentest && raumVerdeckt(draussen,fuss,px,py)) continue;
    const bool schwarz=b.linien[sy*((b.w+7)/8)+sx/8] & (0x80>>(sx%8));
    bild.drawPixel(px,py,schwarz?0:1);
  }
}
void raumFigurZeichnen(const Bild& b, Phase ph, int cx, int fuss, bool spiegeln, int lift, bool draussen, bool fundPose) {
  const int h=raumFigurHoehe(b,ph,fuss,draussen,fundPose), w=max(1,(int)b.w*h/b.h);
  const int x=max(1,min(cx-w/2,319-w)), y=fuss-h-lift;
  figurLage={&b,x,y,w,h,fuss,spiegeln,draussen};
  eviLinks=x; eviOben=y; eviBreite=w; eviHoehe=h; eviFuss=fuss;
  if (!raumVerdeckt(draussen,fuss,cx,fuss-1)) ellipseFuellen(cx,fuss-1,max(3,w/4),2,0);
  raumRasterZeichnen(b,x,y,w,h,spiegeln,true,draussen,fuss);
}
bool raumFigurTreffer(int x,int y) {
  if (raumVerdeckt(false,eviFuss,x,y)) return false;
  const int px=max(eviLinks,min(x,eviLinks+eviBreite-1)), py=max(eviOben,min(y,eviOben+eviHoehe-1));
  return !raumVerdeckt(false,eviFuss,px,py);
}
bool zimmerScheibe(int x,int y) {
  return x>=ZF_X && x<ZF_X+ZF_W && y>=ZF_Y && y<ZF_Y+ZF_H &&
         zimmerBit(ZIMMER_SCHEIBE,ZF_W,x-ZF_X,y-ZF_Y);
}
// Fensterbesuch behält seine eigenen Koordinaten, wird aber auf die neue Scheibe abgebildet.
void zimmerFensterPunkt(int x,int y,uint16_t farbe) {
  const int px=ZF_X+(x-GLAS_X)*ZF_W/GLAS_B, py=ZF_Y+(y-GLAS_Y)*ZF_H/GLAS_H;
  if (zimmerScheibe(px,py)) bild.drawPixel(px,py,farbe);
}
