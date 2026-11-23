// Uhr PCF85063 auf dem Board (I2C-Adresse 0x51, an Wire1 wie der Touch).
// Gespeichert wird UTC. Ohne Pufferbatterie bleibt die Uhr beim Ausstecken stehen;
// dann ist das OS-Bit gesetzt und die Uhrzeit muss neu eingestellt werden.
#pragma once
#include <Wire.h>
#include "zeit.h"

const uint8_t UHR_ADRESSE = 0x51;

static inline uint8_t ausBcd(uint8_t b) { return (b >> 4) * 10 + (b & 0x0F); }
static inline uint8_t inBcd(int v) { return (uint8_t)(((v / 10) << 4) | (v % 10)); }

// Liest die Uhr. false = Uhr stand (OS-Bit) oder antwortet nicht.
bool uhrLesen(ms_t& utc) {
  Wire1.beginTransmission(UHR_ADRESSE);
  Wire1.write(0x04);
  if (Wire1.endTransmission(false) != 0) return false;
  if (Wire1.requestFrom((int)UHR_ADRESSE, 7, (int)true) != 7) return false;
  uint8_t r[7];
  for (int i = 0; i < 7; i++) r[i] = Wire1.read();
  if (r[0] & 0x80) return false;   // Oszillator stand: Zeit ungültig
  const int sek = ausBcd(r[0] & 0x7F), min = ausBcd(r[1] & 0x7F), std = ausBcd(r[2] & 0x3F);
  const int tag = ausBcd(r[3] & 0x3F), mon = ausBcd(r[5] & 0x1F), jahr = 2000 + ausBcd(r[6]);
  if (jahr < 2025 || mon < 1 || mon > 12 || tag < 1 || tag > 31) return false;
  utc = tageAusDatum(jahr, mon, tag) * TAG + std * STUNDE + min * MINUTE + sek * SEKUNDE;
  return true;
}

void uhrStellen(ms_t utc) {
  const int64_t tage = teileAb(utc, TAG);
  int y, m, d;
  datumAusTagen(tage, y, m, d);
  const ms_t imTag = utc - tage * TAG;
  Wire1.beginTransmission(UHR_ADRESSE);
  Wire1.write(0x04);
  Wire1.write(inBcd((int)(imTag % MINUTE / SEKUNDE)));   // löscht auch das OS-Bit
  Wire1.write(inBcd((int)(imTag % STUNDE / MINUTE)));
  Wire1.write(inBcd((int)(imTag / STUNDE)));
  Wire1.write(inBcd(d));
  Wire1.write(wochentag(tage));
  Wire1.write(inBcd(m));
  Wire1.write(inBcd(y - 2000));
  Wire1.endTransmission();
}

// Laufende Zeit: einmal von der Uhr gelesen, danach mit millis() weitergezählt.
static ms_t uhrBasis = 0;
static uint64_t millisGesamt = 0;
static uint32_t letzteMillis = 0;

void uhrSetzen(ms_t utc) {
  uhrBasis = utc;
  letzteMillis = millis();
  millisGesamt = 0;
}

// Jetzt in Millisekunden seit 1970 (UTC). Übersteht den Überlauf von millis().
ms_t jetztMs() {
  const uint32_t m = millis();
  millisGesamt += (uint32_t)(m - letzteMillis);
  letzteMillis = m;
  return uhrBasis + (ms_t)millisGesamt;
}
