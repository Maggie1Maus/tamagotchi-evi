// Bildschirm im Querformat 320 x 240, strikt schwarz-weiß.
// Gezeichnet wird in ein 1-Bit-Bild im Speicher (Adafruit GFX, GFXcanvas1),
// zeigen() schickt es als Ganzes an den Bildschirm.
#pragma once
#include <SPI.h>
#include <Adafruit_GFX.h>
#include "board.h"

const int BREITE = 320;
const int HOEHE  = 240;

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

// Schickt das ganze Bild an den Bildschirm (etwa 30 ms).
void zeigen() {
  const uint8_t spalten[] = {0, 0, (BREITE - 1) >> 8, (BREITE - 1) & 0xFF};
  const uint8_t zeilen[]  = {0, 0, (HOEHE - 1) >> 8, (HOEHE - 1) & 0xFF};
  lcdBefehlMitDaten(0x2A, spalten, 4);
  lcdBefehlMitDaten(0x2B, zeilen, 4);
  lcdBefehl(0x2C);

  static uint8_t zeile[BREITE * 2];
  const uint8_t* quelle = bild.getBuffer();
  const int bytesProZeile = (BREITE + 7) / 8;

  digitalWrite(LCD_DC, HIGH);
  digitalWrite(LCD_CS, LOW);
  for (int y = 0; y < HOEHE; y++) {
    const uint8_t* q = quelle + y * bytesProZeile;
    for (int x = 0; x < BREITE; x++) {
      const bool weiss = q[x >> 3] & (0x80 >> (x & 7));
      zeile[x * 2]     = weiss ? 0xFF : 0x00;
      zeile[x * 2 + 1] = weiss ? 0xFF : 0x00;
    }
    SPI1.transfer(zeile, sizeof(zeile));
  }
  digitalWrite(LCD_CS, HIGH);
}
