#pragma once
#include "Arduino.h"
struct I2S{I2S(int){} bool setBCLK(int){return true;} bool setDATA(int){return true;} bool setBitsPerSample(int){return true;} bool setBuffers(size_t,size_t){return true;} bool begin(long){return true;} int availableForWrite(){return 0;} size_t write16(int16_t,int16_t){return 4;}};
