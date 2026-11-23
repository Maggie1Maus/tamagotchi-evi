#pragma once
#include "Arduino.h"
// Nachgebildetes Display (ST7789, 320 x 240, 16 Bit je Punkt): versteht Spalten (0x2A), Zeilen (0x2B)
// und Schreiben (0x2C). So lässt sich prüfen, ob zeigen() wirklich alles Geänderte schickt.
struct SPISettings{SPISettings(long,int,int){}};
extern unsigned long simGesendet;
extern uint8_t simDisplay[240][320];   // 1 = weiß
struct SPIClass {
  int befehl = 0, n = 0; uint8_t d[4] = {0}; int x0 = 0, x1 = 319, y0 = 0, y1 = 239, x = 0, y = 0, halb = 0;
  bool setRX(int){return true;} bool setSCK(int){return true;} bool setTX(int){return true;} void begin(){} void beginTransaction(SPISettings){}
  uint8_t transfer(uint8_t b) {
    if (!simDC) { befehl = b; n = 0; if (b == 0x2C) { x = x0; y = y0; halb = 0; } return 0; }
    if ((befehl == 0x2A || befehl == 0x2B) && n < 4) { d[n++] = b; if (n == 4) { int a = d[0] << 8 | d[1], e = d[2] << 8 | d[3]; if (befehl == 0x2A) { x0 = a; x1 = e; } else { y0 = a; y1 = e; } } }
    return 0;
  }
  void transfer(void* p, size_t len) {
    simGesendet += len;
    if (!simDC || befehl != 0x2C) return;
    const uint8_t* b = (const uint8_t*)p;
    for (size_t i = 0; i < len; i++) {
      if (halb++ % 2) continue;                    // zwei Bytes je Punkt; das erste genügt (0xFF oder 0x00)
      if (y <= y1 && x >= 0 && x < 320 && y >= 0 && y < 240) simDisplay[y][x] = b[i] ? 1 : 0;
      if (++x > x1) { x = x0; y++; }
    }
  }
};
extern SPIClass SPI1;
