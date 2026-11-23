// Die echten Tasten des Retro Gamers über den MCP23017 (Schritt 11, am Board geprüft 23.11.2026).
//
// Verdrahtung: jede Taste schaltet ihre Leitung auf Minus (gedrückt = LOW).
//   UP=A0, DOWN=A1, L=A2, R=A3, A=A4, B=A5, START=A6, RESET=B0.
// Der MCP23017 hängt mit dem Kabel Adafruit 4401 am I²C-Stecker des Waveshare,
// also an Wire1 (GP6/GP7) wie Touch und Uhr. touchStarten() muss vorher laufen.
// Ohne MCP23017 (z. B. Board außerhalb des Gehäuses) läuft alles nur mit Touch,
// und es wird alle 2 Sekunden neu gesucht.
#pragma once
#include <Wire.h>

enum Knopf : uint8_t { K_UP, K_DOWN, K_L, K_R, K_A, K_B, K_START, K_RESET, K_ANZAHL };

bool tastenDa = false;           // MCP23017 gefunden?
static uint8_t mcpAdr = 0;
static uint16_t tastenStabil = 0, tastenLetzteRoh = 0, tastenEreignisse = 0;
static uint32_t tastenLetztesLesen = 0, tastenLetzteSuche = 0;
static uint32_t tastenWiederholung[K_ANZAHL];
static uint8_t tastenFehler = 0;

const uint32_t TASTEN_WDH_START = 400;   // Festhalten: erste Wiederholung nach 0,4 s
const uint32_t TASTEN_WDH_TAKT = 120;    // danach alle 0,12 s (nur Steuerkreuz)

static bool mcpReg(uint8_t reg, uint8_t wert) {
  Wire1.beginTransmission(mcpAdr);
  Wire1.write(reg);
  Wire1.write(wert);
  return Wire1.endTransmission() == 0;
}

// Sucht den MCP23017 (0x20–0x27) und stellt A0–A6, B0–B6 auf Eingang mit Pull-up.
// A7/B7 sind laut neuem Datenblatt nur Ausgänge: als Ausgang auf LOW (nicht angeschlossen).
bool tastenStarten() {
  tastenDa = false;
  for (uint8_t a = 0x20; a <= 0x27; a++) {
    Wire1.beginTransmission(a);
    if (Wire1.endTransmission() != 0) continue;
    mcpAdr = a;
    tastenDa = mcpReg(0x14, 0x00) && mcpReg(0x15, 0x00) &&   // OLATA, OLATB
               mcpReg(0x00, 0x7F) && mcpReg(0x01, 0x7F) &&   // IODIRA, IODIRB
               mcpReg(0x0C, 0x7F) && mcpReg(0x0D, 0x7F);     // GPPUA, GPPUB
    break;
  }
  tastenStabil = tastenLetzteRoh = tastenEreignisse = 0;
  tastenFehler = 0;
  return tastenDa;
}

// Liest GPIOA und GPIOB; Ergebnis: ein Bit je Knopf, 1 = gedrückt.
static bool tastenRoh(uint16_t& gedrueckt) {
  Wire1.beginTransmission(mcpAdr);
  Wire1.write(0x12);   // GPIOA, danach GPIOB
  if (Wire1.endTransmission(false) != 0) return false;
  if (Wire1.requestFrom((int)mcpAdr, 2, (int)true) != 2) return false;
  const uint8_t a = (uint8_t)~Wire1.read();
  const uint8_t b = (uint8_t)~Wire1.read();
  gedrueckt = (a & 0x7F) | ((b & 0x01) << K_RESET);   // A0–A6 = UP … START, B0 = RESET
  return true;
}

// Oft aufrufen. Entprellt (zweimal gleich im Abstand von 8 ms) und merkt sich neue Drücke.
void tastenLesen() {
  const uint32_t m = millis();
  if (!tastenDa) {
    if (m - tastenLetzteSuche > 2000) { tastenLetzteSuche = m; tastenStarten(); }
    return;
  }
  if (m - tastenLetztesLesen < 8) return;
  tastenLetztesLesen = m;
  uint16_t roh;
  if (!tastenRoh(roh)) {
    if (++tastenFehler > 20) tastenDa = false;   // Kabel ab? Dann später neu suchen
    return;
  }
  tastenFehler = 0;
  if (roh == tastenLetzteRoh && roh != tastenStabil) {
    const uint16_t neu = roh & ~tastenStabil;
    for (int k = 0; k < K_ANZAHL; k++) {
      if (neu & (1 << k)) { tastenEreignisse |= 1 << k; tastenWiederholung[k] = m + TASTEN_WDH_START; }
    }
    tastenStabil = roh;
  }
  tastenLetzteRoh = roh;
  // Festhalten wiederholt das Steuerkreuz (Menü blättern, Uhr stellen)
  for (int k = K_UP; k <= K_R; k++) {
    if ((tastenStabil & (1 << k)) && (int32_t)(m - tastenWiederholung[k]) >= 0) {
      tastenEreignisse |= 1 << k;
      tastenWiederholung[k] = m + TASTEN_WDH_TAKT;
    }
  }
}

// Nächster neuer Tastendruck oder -1.
int naechsteTaste() {
  tastenLesen();
  for (int k = 0; k < K_ANZAHL; k++) {
    if (tastenEreignisse & (1 << k)) { tastenEreignisse &= ~(1 << k); return k; }
  }
  return -1;
}

// Vergisst alle noch nicht abgeholten Drücke.
void tastenVergessen() { tastenEreignisse = 0; }
