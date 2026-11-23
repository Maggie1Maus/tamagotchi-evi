#pragma once
#include "Arduino.h"
struct SPISettings{SPISettings(long,int,int){}};
extern void (*simNachBild)();
extern int simZeilen;
struct SPIClass{bool setRX(int){return true;} bool setSCK(int){return true;} bool setTX(int){return true;} void begin(){} void beginTransaction(SPISettings){}
 uint8_t transfer(uint8_t){return 0;}
 void transfer(void*,size_t){ if (++simZeilen >= 240) { simZeilen = 0; simMillis += 30; if (simNachBild) simNachBild(); } }};
extern SPIClass SPI1;
