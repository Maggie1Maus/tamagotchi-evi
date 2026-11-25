#pragma once
#include "Arduino.h"
// Ton-Ausgabe: läuft nur zwischen begin() und end(). Der Puffer (6 x 256 Proben, je 4 Bytes) leert sich mit der
// nachgebildeten Zeit, 22050 Proben pro Sekunde; so endet eine Melodie wie am Board.
struct I2S{
  bool an = false; long frei = 0; uint32_t zuletzt = 0; int starts = 0;
  I2S(int){} bool setBCLK(int){return true;} bool setDATA(int){return true;} bool setBitsPerSample(int){return true;} bool setBuffers(size_t,size_t){return true;}
  bool begin(long){ an = true; frei = 6 * 256 * 4; zuletzt = simMillis; starts++; return true; }
  bool end(){ an = false; return true; }
  int availableForWrite(){
    if (!an) return 0;
    frei += (long)(simMillis - zuletzt) * 22050 / 1000 * 4; zuletzt = simMillis;
    if (frei > 6 * 256 * 4) frei = 6 * 256 * 4;
    return (int)frei;
  }
  size_t write16(int16_t,int16_t){ frei -= 4; return 4; }
};
