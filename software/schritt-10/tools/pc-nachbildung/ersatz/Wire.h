#pragma once
#include "Arduino.h"
struct TwoWire{bool setSDA(int){return true;} bool setSCL(int){return true;} void setClock(unsigned long){} void begin(){}
 void beginTransmission(int){} uint8_t endTransmission(bool=true){return 2;}
 size_t write(uint8_t){return 1;} size_t requestFrom(int,int q,int){return 0;}
 int available(){return 0;} int read(){return 0;}};
extern TwoWire Wire1;
