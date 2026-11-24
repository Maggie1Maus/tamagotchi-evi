#pragma once
#include "Arduino.h"
struct EEPROMC {
  uint8_t daten[4096] = {};
  void begin(size_t) {}
  uint8_t read(int i) { return i >= 0 && i < 4096 ? daten[i] : 0; }
  void write(int i, uint8_t v) { if (i >= 0 && i < 4096) daten[i] = v; }
  bool commit() { return true; }
};
extern EEPROMC EEPROM;
