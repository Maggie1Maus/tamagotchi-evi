#pragma once
#include "Arduino.h"
#include "font.h"
// Einfache Nachbildung von Adafruit GFX für den PC (nur zum Ansehen der Bilder)
struct Adafruit_GFX {
  int16_t W, H;
  Adafruit_GFX(int w, int h): W(w), H(h) {}
  virtual void drawPixel(int16_t x, int16_t y, uint16_t c) = 0;
  void setTextSize(uint8_t){} void setTextWrap(bool){} void setTextColor(uint16_t){} void setCursor(int16_t,int16_t){} void print(const char*){} void cp437(bool=true){}
  void fillRect(int16_t x,int16_t y,int16_t w,int16_t h,uint16_t c){for(int j=y;j<y+h;j++)for(int i=x;i<x+w;i++)drawPixel(i,j,c);}
  void fillScreen(uint16_t c){fillRect(0,0,W,H,c);}
  void drawFastHLine(int16_t x,int16_t y,int16_t w,uint16_t c){fillRect(x,y,w,1,c);}
  void drawFastVLine(int16_t x,int16_t y,int16_t h,uint16_t c){fillRect(x,y,1,h,c);}
  void drawRect(int16_t x,int16_t y,int16_t w,int16_t h,uint16_t c){drawFastHLine(x,y,w,c);drawFastHLine(x,y+h-1,w,c);drawFastVLine(x,y,h,c);drawFastVLine(x+w-1,y,h,c);}
  void drawRoundRect(int16_t x,int16_t y,int16_t w,int16_t h,int16_t r,uint16_t c){drawFastHLine(x+r,y,w-2*r,c);drawFastHLine(x+r,y+h-1,w-2*r,c);drawFastVLine(x,y+r,h-2*r,c);drawFastVLine(x+w-1,y+r,h-2*r,c);
    for(int k=0;k<r;k++){int d=(int)(r-sqrtf((float)r*r-(float)(r-k)*(r-k)));drawPixel(x+d,y+k,c);drawPixel(x+w-1-d,y+k,c);drawPixel(x+d,y+h-1-k,c);drawPixel(x+w-1-d,y+h-1-k,c);}}
  void fillRoundRect(int16_t x,int16_t y,int16_t w,int16_t h,int16_t r,uint16_t c){for(int j=0;j<h;j++){int k=j<r?r-j:(j>=h-r?j-(h-r-1):0);int d=k?(int)(r-sqrtf((float)r*r-(float)k*k)):0;drawFastHLine(x+d,y+j,w-2*d,c);}}
  // fillCircle wie in Adafruit GFX (fillCircleHelper mit Mittelpunktverfahren)
  void fillCircle(int16_t x0,int16_t y0,int16_t r,uint16_t c){drawFastVLine(x0,y0-r,2*r+1,c);int16_t f=1-r,ddx=1,ddy=-2*r,x=0,y=r,px=x,py=y;
    while(x<y){if(f>=0){y--;ddy+=2;f+=ddy;}x++;ddx+=2;f+=ddx;if(x<(y+1)){drawFastVLine(x0+x,y0-y,2*y+1,c);drawFastVLine(x0-x,y0-y,2*y+1,c);}
      if(y!=py){drawFastVLine(x0+py,y0-px,2*px+1,c);drawFastVLine(x0-py,y0-px,2*px+1,c);py=y;}px=x;}}
  void drawLine(int16_t x0,int16_t y0,int16_t x1,int16_t y1,uint16_t c){int dx=abs(x1-x0),sx=x0<x1?1:-1,dy=-abs(y1-y0),sy=y0<y1?1:-1,e=dx+dy;for(;;){drawPixel(x0,y0,c);if(x0==x1&&y0==y1)break;int e2=2*e;if(e2>=dy){e+=dy;x0+=sx;}if(e2<=dx){e+=dx;y0+=sy;}}}
  void fillTriangle(int16_t x0,int16_t y0,int16_t x1,int16_t y1,int16_t x2,int16_t y2,uint16_t c){int miny=std::min({y0,y1,y2}),maxy=std::max({y0,y1,y2}),minx=std::min({x0,x1,x2}),maxx=std::max({x0,x1,x2});
    auto s=[](float ax,float ay,float bx,float by,float px,float py){return (bx-ax)*(py-ay)-(by-ay)*(px-ax);};
    for(int y=miny;y<=maxy;y++)for(int x=minx;x<=maxx;x++){float a=s(x0,y0,x1,y1,x,y),b=s(x1,y1,x2,y2,x,y),d=s(x2,y2,x0,y0,x,y);if((a>=0&&b>=0&&d>=0)||(a<=0&&b<=0&&d<=0))drawPixel(x,y,c);}}
  void drawBitmap(int16_t x,int16_t y,const uint8_t* b,int16_t w,int16_t h,uint16_t c){int bw=(w+7)/8;for(int j=0;j<h;j++)for(int i=0;i<w;i++)if(b[j*bw+i/8]&(0x80>>(i&7)))drawPixel(x+i,y+j,c);}
  void drawChar(int16_t x,int16_t y,unsigned char ch,uint16_t c,uint16_t,uint8_t s){for(int i=0;i<5;i++){uint8_t l=SIMFONT[ch][i];for(int j=0;j<8;j++,l>>=1)if(l&1)fillRect(x+i*s,y+j*s,s,s,c);}}
};
struct GFXcanvas1: Adafruit_GFX {
  uint8_t* buf;
  GFXcanvas1(int w,int h):Adafruit_GFX(w,h){buf=(uint8_t*)calloc((w+7)/8*h,1);}
  void drawPixel(int16_t x,int16_t y,uint16_t c) override {if(x<0||y<0||x>=W||y>=H)return;uint8_t*p=&buf[y*((W+7)/8)+x/8];if(c)*p|=0x80>>(x&7);else *p&=~(0x80>>(x&7));}
  uint8_t* getBuffer(){return buf;}
};
