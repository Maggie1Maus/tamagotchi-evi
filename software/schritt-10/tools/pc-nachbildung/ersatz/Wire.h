#pragma once
#include "Arduino.h"
// Ersatz für Wire mit einem nachgebildeten MCP23017 an Adresse 0x20 und einem Lagesensor an 0x6B (Register, Pull-ups,
// Merken von Änderungen in INTF/INTCAP). Alle anderen Adressen antworten nicht.
// simTaste(k, gedrueckt) legt den Anschluss von Knopf k auf Minus bzw. lässt ihn los
// (k wie in tasten.h: 0–6 = A0–A6, 7 = B0).
struct McpNachbildung {
  uint8_t reg[0x16];
  uint8_t pin[2] = {0xFF, 0xFF};   // Pegel an den Anschlüssen (1 = offen, durch Pull-up HIGH)
  McpNachbildung() { for (int i = 0; i < 0x16; i++) reg[i] = 0; reg[0x00] = reg[0x01] = 0xFF; }
  uint8_t lesen(uint8_t r) {
    if (r == 0x12 || r == 0x13) { const int p = r - 0x12; reg[0x0E + p] = 0; return (pin[p] & reg[0x00 + p]) | (reg[0x14 + p] & ~reg[0x00 + p]); }
    if (r == 0x10 || r == 0x11) { const int p = r - 0x10; uint8_t v = reg[r]; reg[0x0E + p] = 0; return v; }
    return r < 0x16 ? reg[r] : 0;
  }
  void schreiben(uint8_t r, uint8_t v) { if (r < 0x16 && r != 0x0E && r != 0x0F && r != 0x10 && r != 0x11) reg[r] = v; }
  void setzen(int k, bool gedrueckt) {
    const int p = k < 7 ? 0 : 1, bit = k < 7 ? k : 0;
    const uint8_t vorher = pin[p];
    if (gedrueckt) pin[p] &= ~(1 << bit); else pin[p] |= (1 << bit);
    if (pin[p] == vorher || !(reg[0x04 + p] & (1 << bit)) || (reg[0x08 + p] & (1 << bit))) return;
    if (reg[0x0E + p] == 0) reg[0x10 + p] = pin[p];   // INTCAP: Zustand beim ersten Ereignis
    reg[0x0E + p] |= 1 << bit;                         // INTF
  }
};
inline McpNachbildung& simMcp() { static McpNachbildung m; return m; }
inline void simTaste(int k, bool gedrueckt) { simMcp().setzen(k, gedrueckt); }
// Nachgebildeter Lagesensor QMI8658 an 0x6B: WHO_AM_I = 0x05, Beschleunigung ab 0x35 (±4 g, 8192 je g,
// niedriges Byte zuerst). simLage(x, y, z) stellt die Messung in g ein.
struct QmiNachbildung {
  float x = 0, y = 0, z = 1;
  uint8_t lesen(uint8_t r) {
    if (r == 0x00) return 0x05;
    if (r >= 0x35 && r <= 0x3A) {
      const float w[3] = {x, y, z};
      const int i = (r - 0x35) / 2;
      float g = w[i]; if (g > 3.99f) g = 3.99f; if (g < -3.99f) g = -3.99f;
      const int16_t v = (int16_t)(g * 8192);
      return (r - 0x35) % 2 ? (uint8_t)((uint16_t)v >> 8) : (uint8_t)(v & 0xFF);
    }
    return 0;
  }
};
inline QmiNachbildung& simQmi() { static QmiNachbildung q; return q; }
inline void simLage(float x, float y, float z) { simQmi().x = x; simQmi().y = y; simQmi().z = z; }
struct TwoWire {
  int ziel = 0, geschrieben = 0, leseN = 0, leseI = 0; uint8_t zeiger = 0, puffer[32];
  bool setSDA(int){return true;} bool setSCL(int){return true;} void setClock(unsigned long){} void begin(){}
  void beginTransmission(int a){ ziel = a; geschrieben = 0; }
  size_t write(uint8_t b){ if (ziel == 0x20) { if (geschrieben++ == 0) zeiger = b; else simMcp().schreiben(zeiger++, b); }
    else if (ziel == 0x6B) { if (geschrieben++ == 0) zeiger = b; }
    return 1; }
  uint8_t endTransmission(bool = true){ return (ziel == 0x20 || ziel == 0x6B) ? 0 : 2; }
  size_t requestFrom(int a, int n, int){ if (a != 0x20 && a != 0x6B) return 0; leseN = n; leseI = 0;
    for (int i = 0; i < n && i < 32; i++) puffer[i] = a == 0x20 ? simMcp().lesen(zeiger++) : simQmi().lesen(zeiger++);
    return n; }
  int available(){ return leseN - leseI; } int read(){ return leseI < leseN ? puffer[leseI++] : 0; }
};
extern TwoWire Wire1;
