// Die echten Tasten des Retro Gamers über den MCP23017 (Schritt 11, am Board geprüft 23.11.2026).
//
// Verdrahtung: jede Taste schaltet ihre Leitung auf Minus (gedrückt = LOW).
//   UP=A0, DOWN=A1, L=A2, R=A3, A=A4, B=A5, START=A6, RESET=B0.
// Der MCP23017 hängt mit dem Kabel Adafruit 4401 am I²C-Stecker des Waveshare,
// also an Wire1 (GP6/GP7) wie Touch und Uhr. touchStarten() muss vorher laufen.
// Ohne MCP23017 (z. B. Board außerhalb des Gehäuses) läuft alles nur mit Touch,
// und es wird alle 2 Sekunden neu gesucht.
//
// Seit 24.11.2026 (Steuerkreuz fühlte sich schwerfällig an):
//  - Ein Druck zählt sofort beim ersten Lesen. Nur das Loslassen wird entprellt (20 ms),
//    damit ein flackernder Kohlekontakt nicht als zweiter Druck zählt.
//  - Der MCP23017 merkt sich jede Änderung selbst (Interrupt bei Änderung, INTF/INTCAP).
//    So geht auch ein ganz kurzes Antippen zwischen zwei Abfragen nicht verloren.
//    Der INT-Ausgang muss dafür nicht angeschlossen sein; die Register werden abgefragt.
//    Grenze: INTCAP hält nur die erste Änderung bis zum nächsten Lesen fest. Werden zwei
//    verschiedene Tasten innerhalb weniger ms angetippt und beide vor der nächsten Abfrage
//    wieder losgelassen, zählt nur die erste (im Alltag praktisch nie).
//  - Abgefragt wird etwa alle 3 ms statt alle 8 ms.
#pragma once
#include <Wire.h>

enum Knopf : uint8_t { K_UP, K_DOWN, K_L, K_R, K_A, K_B, K_START, K_RESET, K_ANZAHL };

bool tastenDa = false;           // MCP23017 gefunden?
static uint8_t mcpAdr = 0;
static uint16_t tastenStabil = 0, tastenEreignisse = 0;
static uint32_t tastenLetztesLesen = 0, tastenLetzteSuche = 0;
static uint32_t tastenWiederholung[K_ANZAHL], tastenZuletztGedrueckt[K_ANZAHL];
static uint8_t tastenFehler = 0;

const uint32_t TASTEN_ABFRAGE = 3;       // ms zwischen zwei Abfragen
const uint32_t TASTEN_LOS = 20;          // so lange muss eine Taste los sein, bis sie als losgelassen gilt
const uint32_t TASTEN_WDH_START = 400;   // Festhalten: erste Wiederholung nach 0,4 s
const uint32_t TASTEN_WDH_TAKT = 120;    // danach alle 0,12 s (nur Steuerkreuz)

// Register des MCP23017 bei Werkseinstellung (IOCON.BANK = 0)
const uint8_t MCP_IODIRA = 0x00, MCP_IODIRB = 0x01, MCP_GPINTENA = 0x04, MCP_GPINTENB = 0x05;
const uint8_t MCP_INTCONA = 0x08, MCP_INTCONB = 0x09, MCP_GPPUA = 0x0C, MCP_GPPUB = 0x0D;
const uint8_t MCP_INTFA = 0x0E, MCP_OLATA = 0x14, MCP_OLATB = 0x15;

static bool mcpReg(uint8_t reg, uint8_t wert) {
  Wire1.beginTransmission(mcpAdr);
  Wire1.write(reg);
  Wire1.write(wert);
  return Wire1.endTransmission() == 0;
}

// Aus den beiden Anschlussreihen ein Bit je Knopf machen (Bit = 1: Leitung liegt auf Minus)
static inline uint16_t tastenBits(uint8_t a, uint8_t b) {
  return (uint16_t)((~a & 0x7F) | ((~b & 0x01) << K_RESET));   // A0–A6 = UP … START, B0 = RESET
}

// Sucht den MCP23017 (0x20–0x27) und stellt A0–A6, B0–B6 auf Eingang mit Pull-up.
// A7/B7 sind laut neuem Datenblatt nur Ausgänge: als Ausgang auf LOW (nicht angeschlossen).
// Für die benutzten Tasten meldet er jede Änderung (GPINTEN, INTCON = 0: gegen den vorigen Wert).
bool tastenStarten() {
  tastenDa = false;
  for (uint8_t a = 0x20; a <= 0x27; a++) {
    Wire1.beginTransmission(a);
    if (Wire1.endTransmission() != 0) continue;
    mcpAdr = a;
    tastenDa = mcpReg(MCP_OLATA, 0x00) && mcpReg(MCP_OLATB, 0x00) &&
               mcpReg(MCP_IODIRA, 0x7F) && mcpReg(MCP_IODIRB, 0x7F) &&
               mcpReg(MCP_GPPUA, 0x7F) && mcpReg(MCP_GPPUB, 0x7F) &&
               mcpReg(MCP_INTCONA, 0x00) && mcpReg(MCP_INTCONB, 0x00) &&
               mcpReg(MCP_GPINTENA, 0x7F) && mcpReg(MCP_GPINTENB, 0x01);
    break;
  }
  tastenStabil = tastenEreignisse = 0;
  tastenFehler = 0;
  return tastenDa;
}

// Liest auf einmal INTFA, INTFB, INTCAPA, INTCAPB, GPIOA, GPIOB (0x0E–0x13).
// jetzt: was gerade gedrückt ist. gemerkt: was gedrückt war, als sich seit dem letzten Lesen
// etwas geändert hat (INTCAP). Das Lesen löscht die gemerkte Änderung im Chip.
static bool tastenRoh(uint16_t& jetzt, uint16_t& gemerkt) {
  Wire1.beginTransmission(mcpAdr);
  Wire1.write(MCP_INTFA);
  if (Wire1.endTransmission(false) != 0) return false;
  if (Wire1.requestFrom((int)mcpAdr, 6, (int)true) != 6) return false;
  uint8_t r[6];
  for (int i = 0; i < 6; i++) r[i] = (uint8_t)Wire1.read();
  jetzt = tastenBits(r[4], r[5]);
  // INTCAP gilt nur für eine Reihe, in der sich wirklich etwas geändert hat (INTF ≠ 0)
  gemerkt = tastenBits(r[0] ? r[2] : 0xFF, r[1] ? r[3] : 0xFF);
  return true;
}

// Oft aufrufen. Ein Druck zählt sofort; losgelassen ist eine Taste erst nach 20 ms ohne Druck.
void tastenLesen() {
  const uint32_t m = millis();
  if (!tastenDa) {
    if (m - tastenLetzteSuche > 2000) { tastenLetzteSuche = m; tastenStarten(); }
    return;
  }
  if (m - tastenLetztesLesen < TASTEN_ABFRAGE) return;
  tastenLetztesLesen = m;
  uint16_t jetzt, gemerkt;
  if (!tastenRoh(jetzt, gemerkt)) {
    if (++tastenFehler > 20) tastenDa = false;   // Kabel ab? Dann später neu suchen
    return;
  }
  tastenFehler = 0;
  const uint16_t gedrueckt = jetzt | gemerkt;
  for (int k = 0; k < K_ANZAHL; k++) {
    const uint16_t bit = 1 << k;
    if (gedrueckt & bit) {
      tastenZuletztGedrueckt[k] = m;
      if (!(tastenStabil & bit)) {               // neu gedrückt: sofort zählen
        tastenStabil |= bit;
        tastenEreignisse |= bit;
        tastenWiederholung[k] = m + TASTEN_WDH_START;
      }
    } else if ((tastenStabil & bit) && m - tastenZuletztGedrueckt[k] >= TASTEN_LOS) {
      tastenStabil &= ~bit;                      // losgelassen
    }
  }
  // Festhalten wiederholt das Steuerkreuz (Menü blättern, Uhr stellen)
  for (int k = K_UP; k <= K_R; k++) {
    if ((tastenStabil & (1 << k)) && (jetzt & (1 << k)) && (int32_t)(m - tastenWiederholung[k]) >= 0) {
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
