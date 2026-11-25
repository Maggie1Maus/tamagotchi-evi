// Touch: erkennt selbst, welcher Chip verbaut ist.
//  - CST3530 (Adresse 0x58): Boards ab Oktober 2026 („V2“). Ablauf wie in Waveshares CST3530.cpp.
//  - CST328  (Adresse 0x1A): ältere Boards („V1“). Das Board der Nutzerin hat diesen Chip; am 23.11.2026 geprüft.
// Liefert die Fingerposition im Querformat 320 x 240.
#pragma once
#include <Wire.h>
#include "board.h"

// Liegt der Punkt gespiegelt neben dem Finger? Dann hier 1 eintragen.
#define TOUCH_SPIEGELN_X 0
#define TOUCH_SPIEGELN_Y 0

enum TouchChip { TOUCH_KEINER, TOUCH_CST3530, TOUCH_CST328 };
TouchChip touchChip = TOUCH_KEINER;

static volatile bool touchMeldet = false;
static void touchUnterbrechung() { touchMeldet = true; }

static bool i2cDa(uint8_t adresse) {
  Wire1.beginTransmission(adresse);
  return Wire1.endTransmission() == 0;
}

// Register mit 4 Byte (CST3530) oder 2 Byte (CST328) Adresse lesen
static void touchLesen(uint8_t adresse, uint32_t reg, int regBytes, uint8_t* ziel, int anzahl) {
  Wire1.beginTransmission(adresse);
  for (int i = regBytes - 1; i >= 0; i--) Wire1.write((uint8_t)(reg >> (8 * i)));
  Wire1.endTransmission(false);
  Wire1.requestFrom((int)adresse, anzahl, (int)true);
  for (int i = 0; i < anzahl; i++) ziel[i] = Wire1.available() ? Wire1.read() : 0;
}

static void touchSchreiben(uint8_t adresse, uint32_t reg, int regBytes, const uint8_t* daten, int anzahl) {
  Wire1.beginTransmission(adresse);
  for (int i = regBytes - 1; i >= 0; i--) Wire1.write((uint8_t)(reg >> (8 * i)));
  for (int i = 0; i < anzahl; i++) Wire1.write(daten[i]);
  Wire1.endTransmission();
}

void touchStarten() {
  Wire1.setSDA(I2C_SDA);
  Wire1.setSCL(I2C_SCL);
  Wire1.setClock(400000);
  Wire1.begin();

  pinMode(TOUCH_RST, OUTPUT);
  pinMode(TOUCH_INT, INPUT_PULLUP);
  digitalWrite(TOUCH_RST, LOW);
  delay(10);
  digitalWrite(TOUCH_RST, HIGH);
  delay(100);

  if (i2cDa(0x58))      touchChip = TOUCH_CST3530;
  else if (i2cDa(0x1A)) touchChip = TOUCH_CST328;

  attachInterrupt(digitalPinToInterrupt(TOUCH_INT), touchUnterbrechung, FALLING);
}

// Im Dunkeln ist Touch aus (Wunsch der Nutzerin vom 25.11.2026, schlaf.h): Der Touch-Chip wird im Reset
// gehalten und braucht so kaum Strom. Wecken geht dann nur mit den Tasten. Nach dem Loslassen braucht der Chip
// etwa 100 ms, bis er wieder misst (wie in touchStarten); so lange werden Meldungen nicht beachtet.
bool touchAus = false;
static uint32_t touchBereitAb = 0;

void touchSchlafen() {
  if (touchAus) return;
  digitalWrite(TOUCH_RST, LOW);
  touchAus = true;
  touchMeldet = false;
}

void touchAufwachen() {
  if (!touchAus) return;
  digitalWrite(TOUCH_RST, HIGH);
  touchAus = false;
  touchBereitAb = millis() + 150;
  touchMeldet = false;
}

const char* touchName() {
  switch (touchChip) {
    case TOUCH_CST3530: return "CST3530 (V2)";
    case TOUCH_CST328:  return "CST328 (V1)";
    default:            return "nicht gefunden";
  }
}

// Rohwerte des Chips (hochkant, 240 x 320), zum Nachsehen auf dem Bildschirm
int touchRohX = -1, touchRohY = -1;

// true, wenn gerade ein Finger erkannt wurde; x/y im Querformat
bool touchLesen(int& x, int& y) {
  if (!touchMeldet) return false;
  touchMeldet = false;
  if (touchAus || (int32_t)(millis() - touchBereitAb) < 0) return false;   // im Reset oder startet noch

  int rx, ry;
  if (touchChip == TOUCH_CST3530) {
    uint8_t buf[9] = {0};
    touchLesen(0x58, 0xD0070000, 4, buf, 9);
    touchSchreiben(0x58, 0xD00002AB, 4, nullptr, 0);   // „fertig gelesen“
    if ((buf[3] & 0x0F) == 0 || (buf[8] & 0xF0) == 0) return false;
    rx = ((buf[7] & 0x0F) << 8) + buf[4];
    ry = ((buf[7] & 0xF0) << 4) + buf[5];
  } else if (touchChip == TOUCH_CST328) {
    uint8_t anzahl = 0, buf[5] = {0}, null = 0;
    touchLesen(0x1A, 0xD005, 2, &anzahl, 1);
    if ((anzahl & 0x0F) == 0) { touchSchreiben(0x1A, 0xD005, 2, &null, 1); return false; }
    touchLesen(0x1A, 0xD000, 2, buf, 5);
    touchSchreiben(0x1A, 0xD005, 2, &null, 1);
    rx = (buf[1] << 4) | (buf[3] >> 4);
    ry = (buf[2] << 4) | (buf[3] & 0x0F);
  } else {
    return false;
  }

  touchRohX = rx;
  touchRohY = ry;

  // hochkant (240 x 320) -> quer (320 x 240), wie Waveshares „rotation 1“
  x = ry;
  y = 239 - rx;
  if (TOUCH_SPIEGELN_X) x = 319 - x;
  if (TOUCH_SPIEGELN_Y) y = 239 - y;
  x = constrain(x, 0, 319);
  y = constrain(y, 0, 239);
  return true;
}
