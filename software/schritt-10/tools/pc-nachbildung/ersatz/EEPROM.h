#pragma once
#include "Arduino.h"
struct EEPROMC{void begin(size_t){} uint8_t read(int){return 0;} void write(int,uint8_t){} bool commit(){return true;}};
extern EEPROMC EEPROM;
