// Bildschirm im Querformat 320 x 240, strikt schwarz-weiß.
// Gezeichnet wird in ein 1-Bit-Bild im Speicher (Adafruit GFX, GFXcanvas1),
// zeigen() schickt die geänderten Stellen an den Bildschirm.
#pragma once
#include <SPI.h>
#include <Adafruit_GFX.h>
#include "board.h"

const int BREITE = 320;
const int HOEHE  = 240;

// Die ersten Spalten links sind am Gerät nicht zu sehen (Foto vom 23.11.2026:
// Schrift ab x = 4 war bis einschließlich x = 7 verdeckt). Text und Balken beginnen deshalb
// erst rechts davon. Das Zimmer darf darunter weiterlaufen, es ist nur Hintergrund.
const int RAND_LINKS = 8;

// Steht das Bild auf dem Kopf? Dann hier 1 statt 0 eintragen.
#define BILD_DREHEN 0

GFXcanvas1 bild(BREITE, HOEHE);   // 1 = weiß, 0 = schwarz

static void lcdBefehl(uint8_t b) {
  digitalWrite(LCD_DC, LOW);
  digitalWrite(LCD_CS, LOW);
  SPI1.transfer(b);
  digitalWrite(LCD_CS, HIGH);
}

static void lcdDaten(uint8_t b) {
  digitalWrite(LCD_DC, HIGH);
  digitalWrite(LCD_CS, LOW);
  SPI1.transfer(b);
  digitalWrite(LCD_CS, HIGH);
}

static void lcdBefehlMitDaten(uint8_t befehl, const uint8_t* daten, int anzahl) {
  lcdBefehl(befehl);
  for (int i = 0; i < anzahl; i++) lcdDaten(daten[i]);
}

// Helligkeit 0-100 Prozent
void helligkeit(int prozent) {
  analogWrite(LCD_BL, prozent * 255 / 100);
}

void bildschirmStarten() {
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_DC, OUTPUT);
  pinMode(LCD_RST, OUTPUT);
  digitalWrite(LCD_CS, HIGH);

  SPI1.setRX(LCD_MISO);
  SPI1.setSCK(LCD_SCK);
  SPI1.setTX(LCD_MOSI);
  SPI1.begin();
  SPI1.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));

  // Reset wie bei Waveshare
  digitalWrite(LCD_RST, HIGH); delay(100);
  digitalWrite(LCD_RST, LOW);  delay(100);
  digitalWrite(LCD_RST, HIGH); delay(100);

  lcdBefehl(0x11);  // aufwachen
  delay(120);

  // Querformat: Zeilen und Spalten tauschen (MV) und spiegeln
  const uint8_t richtung = BILD_DREHEN ? 0xA0 : 0x60;
  lcdBefehlMitDaten(0x36, &richtung, 1);

  // Einstellungen 1:1 aus Waveshares LCD_2in8.cpp
  const uint8_t farbformat = 0x05;  // 16 Bit pro Pixel
  lcdBefehlMitDaten(0x3A, &farbformat, 1);
  const uint8_t b2[] = {0x0C, 0x0C, 0x00, 0x33, 0x33}; lcdBefehlMitDaten(0xB2, b2, 5);
  const uint8_t b7[] = {0x75};                         lcdBefehlMitDaten(0xB7, b7, 1);
  const uint8_t bb[] = {0x1A};                         lcdBefehlMitDaten(0xBB, bb, 1);
  const uint8_t c0[] = {0x2C};                         lcdBefehlMitDaten(0xC0, c0, 1);
  const uint8_t c2[] = {0x01, 0xFF};                   lcdBefehlMitDaten(0xC2, c2, 2);
  const uint8_t c3[] = {0x13};                         lcdBefehlMitDaten(0xC3, c3, 1);
  const uint8_t c4[] = {0x20};                         lcdBefehlMitDaten(0xC4, c4, 1);
  const uint8_t c6[] = {0x0F};                         lcdBefehlMitDaten(0xC6, c6, 1);
  const uint8_t d0[] = {0xA4, 0xA1};                   lcdBefehlMitDaten(0xD0, d0, 2);
  const uint8_t d6[] = {0xA1};                         lcdBefehlMitDaten(0xD6, d6, 1);
  const uint8_t e0[] = {0xD0, 0x0D, 0x14, 0x0D, 0x0D, 0x09, 0x38,
                        0x44, 0x4E, 0x3A, 0x17, 0x18, 0x2F, 0x30};
  lcdBefehlMitDaten(0xE0, e0, 14);
  const uint8_t e1[] = {0xD0, 0x09, 0x0F, 0x08, 0x07, 0x14, 0x37,
                        0x44, 0x4D, 0x38, 0x15, 0x16, 0x2C, 0x2E};
  lcdBefehlMitDaten(0xE1, e1, 14);
  lcdBefehl(0x21);  // Farben invertieren, braucht dieses Panel
  lcdBefehl(0x29);  // Anzeige an

  bild.fillScreen(0);
}

// Was zuletzt an den Bildschirm geschickt wurde. zeigen() vergleicht damit und sendet nur die
// geänderten Stellen: Das ganze Bild braucht etwa 35 ms, ein kleiner Streifen (z. B. der Ball)
// unter 1 ms. Darum läuft der Ball flüssig, und Menü und Momente reagieren schneller.
static uint8_t gesendet[(BREITE / 8) * HOEHE];
static bool gesendetGueltig = false, gesendetUmgekehrt = false;

// Schickt einen Ausschnitt (x0..x1, y0..y1, jeweils einschließlich) aus dem Bild an den Bildschirm.
static void bereichSenden(int x0, int y0, int x1, int y1, bool umgekehrt) {
  const uint8_t spalten[] = {(uint8_t)(x0 >> 8), (uint8_t)(x0 & 0xFF), (uint8_t)(x1 >> 8), (uint8_t)(x1 & 0xFF)};
  const uint8_t zeilen[]  = {(uint8_t)(y0 >> 8), (uint8_t)(y0 & 0xFF), (uint8_t)(y1 >> 8), (uint8_t)(y1 & 0xFF)};
  lcdBefehlMitDaten(0x2A, spalten, 4);
  lcdBefehlMitDaten(0x2B, zeilen, 4);
  lcdBefehl(0x2C);

  static uint8_t zeile[BREITE * 2];
  const uint8_t* quelle = bild.getBuffer();
  const int bytesProZeile = (BREITE + 7) / 8, n = x1 - x0 + 1;

  digitalWrite(LCD_DC, HIGH);
  digitalWrite(LCD_CS, LOW);
  for (int y = y0; y <= y1; y++) {
    const uint8_t* q = quelle + y * bytesProZeile;
    for (int i = 0; i < n; i++) {
      const int x = x0 + i;
      const bool weiss = ((q[x >> 3] & (0x80 >> (x & 7))) != 0) != umgekehrt;
      zeile[i * 2]     = weiss ? 0xFF : 0x00;
      zeile[i * 2 + 1] = weiss ? 0xFF : 0x00;
    }
    SPI1.transfer(zeile, n * 2);
  }
  digitalWrite(LCD_CS, HIGH);
}

// Schickt das Bild an den Bildschirm, aber nur, was sich seit dem letzten Mal geändert hat.
// umgekehrt = true zeigt Schwarz als Weiß und Weiß als Schwarz (Nacht).
void zeigen(bool umgekehrt = false) {
  const uint8_t* neu = bild.getBuffer();
  const int bpz = BREITE / 8;
  if (!gesendetGueltig || umgekehrt != gesendetUmgekehrt) {
    bereichSenden(0, 0, BREITE - 1, HOEHE - 1, umgekehrt);
    memcpy(gesendet, neu, sizeof(gesendet));
    gesendetGueltig = true;
    gesendetUmgekehrt = umgekehrt;
  } else {
    for (int y = 0; y < HOEHE;) {
      if (memcmp(neu + y * bpz, gesendet + y * bpz, bpz) == 0) { y++; continue; }
      // Ein Block aus geänderten Zeilen; kurze Lücken (bis 4 Zeilen) gehören mit dazu.
      int y1 = y, luecke = 0, bMin = bpz, bMax = -1;
      for (int z = y; z < HOEHE; z++) {
        const uint8_t* a = neu + z * bpz;
        const uint8_t* b = gesendet + z * bpz;
        if (memcmp(a, b, bpz) == 0) { if (++luecke > 4) break; continue; }
        luecke = 0;
        y1 = z;
        for (int i = 0; i < bpz; i++) {
          if (a[i] != b[i]) { if (i < bMin) bMin = i; if (i > bMax) bMax = i; }
        }
      }
      bereichSenden(bMin * 8, y, bMax * 8 + 7, y1, umgekehrt);
      for (int z = y; z <= y1; z++) memcpy(gesendet + z * bpz + bMin, neu + z * bpz + bMin, bMax - bMin + 1);
      y = y1 + 1;
    }
  }
#ifdef NACHBILDUNG
  nachbildungBildFertig();   // nur in der PC-Nachbildung (tools/pc-nachbildung)
#endif
}
