// Bildschirm im Querformat 320 x 240 in Graustufen (seit 25.11.2026, Wunsch der Nutzerin; vorher strikt schwarz-weiß).
// Gezeichnet wird in ein 8-Bit-Bild im Speicher (Adafruit GFX, GFXcanvas8), angezeigt werden 16 Graustufen.
// Altes Zeichnen funktioniert unverändert: Farbe 0 = schwarz, Farbe 1 = weiß (wird 255). Graue Werte sind
// 0, 2 bis 254 (nie 1). zeigen() schickt die geänderten Stellen an den Bildschirm.
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

// Zeichenfläche: alle Zeichenbefehle der alten 1-Bit-Fläche gehen weiter (0 = schwarz, 1 = weiß).
class GrauCanvas : public GFXcanvas8 {
 public:
  GrauCanvas(uint16_t w, uint16_t h) : GFXcanvas8(w, h) {}
  static inline uint16_t wert(uint16_t c) { return c == 1 ? 255 : (c > 255 ? 255 : c); }
  void drawPixel(int16_t x, int16_t y, uint16_t c) override { geaendert = true; GFXcanvas8::drawPixel(x, y, wert(c)); }
  void fillScreen(uint16_t c) override { geaendert = true; GFXcanvas8::fillScreen(wert(c)); }
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t c) override { geaendert = true; GFXcanvas8::drawFastVLine(x, y, h, wert(c)); }
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t c) override { geaendert = true; GFXcanvas8::drawFastHLine(x, y, w, wert(c)); }
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c) override { geaendert = true; GFXcanvas8::fillRect(x, y, w, h, wert(c)); }
  void markieren() { geaendert = true; }   // nach direktem Schreiben in getBuffer()
  // Stufe 0–15 an dieser Stelle (so wird sie angezeigt)
  uint8_t stufe(int x, int y) { return (x < 0 || y < 0 || x >= BREITE || y >= HOEHE) ? 0 : (getBuffer()[y * BREITE + x] >> 4); }
  // Schwarz-weiß-Auszug (1 = hell, ab Stufe 8), 40 Byte je Zeile: für die Prüfung über USB und die PC-Prüfprogramme
  const uint8_t* bits() {
    if (geaendert) {
      const uint8_t* q = getBuffer();
      memset(auszug, 0, sizeof(auszug));
      for (int y = 0; y < HOEHE; y++)
        for (int x = 0; x < BREITE; x++)
          if (q[y * BREITE + x] >= 128) auszug[y * (BREITE / 8) + x / 8] |= 0x80 >> (x % 8);
      geaendert = false;
    }
    return auszug;
  }
 private:
  bool geaendert = true;
  uint8_t auszug[(BREITE / 8) * HOEHE];
};
GrauCanvas bild(BREITE, HOEHE);   // 0 = schwarz, 1 oder 255 = weiß, dazwischen Grau

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

// Was zuletzt an den Bildschirm geschickt wurde (Stufe 0–15 je Punkt). zeigen() vergleicht damit und sendet nur die
// geänderten Stellen: Das ganze Bild braucht etwa 35 ms, ein kleiner Streifen (z. B. der Ball)
// unter 1 ms. Darum läuft der Ball flüssig, und Menü und Momente reagieren schneller.
static uint8_t gesendet[BREITE * HOEHE];
static bool gesendetGueltig = false, gesendetUmgekehrt = false;

// Stufe 0–15 -> Farbe des Bildschirms (RGB565, grau): 0 = schwarz, 15 = weiß
static uint16_t grauFarbe[16];
static void grauFarbenRechnen() {
  for (int l = 0; l < 16; l++) { const int v = l * 17; grauFarbe[l] = (uint16_t)(((v >> 3) << 11) | ((v >> 2) << 5) | (v >> 3)); }
}

// Helligkeit 0-100 Prozent
void helligkeit(int prozent) {
  analogWrite(LCD_BL, prozent * 255 / 100);
}

// Schlafmodus des Bildschirm-Chips (seit 25.11.2026, schlaf.h). Bei ausgeschaltetem Bildschirm braucht der
// ST7789 sonst weiter Strom, auch ohne Licht. Das Bild in seinem Speicher bleibt im Schlaf erhalten.
// Laut Datenblatt: nach dem Aufwachen 5 ms bis zum nächsten Befehl, zwischen Einschlafen und Aufwachen
// (und umgekehrt) mindestens 120 ms.
const bool LCD_SCHLAF = true;   // false = Bildschirm-Chip bleibt wach (falls am Board etwas nicht stimmt)
bool lcdSchlaeft = false;
static uint32_t lcdWechsel = 0;

static void lcdWechselAbwarten() {
  const uint32_t seit = millis() - lcdWechsel;
  if (seit < 120) delay(120 - seit);
}

void lcdSchlafen() {
  if (!LCD_SCHLAF || lcdSchlaeft) return;
  lcdWechselAbwarten();
  lcdBefehl(0x28);   // Anzeige aus
  lcdBefehl(0x10);   // schlafen
  lcdSchlaeft = true;
  lcdWechsel = millis();
}

void lcdAufwachen() {
  if (!lcdSchlaeft) return;
  lcdWechselAbwarten();
  lcdBefehl(0x11);   // aufwachen
  delay(5);
  lcdBefehl(0x29);   // Anzeige an
  lcdSchlaeft = false;
  lcdWechsel = millis();
  gesendetGueltig = false;   // zur Sicherheit beim nächsten Mal das ganze Bild schicken
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
  lcdSchlaeft = false;
  lcdWechsel = millis();

  grauFarbenRechnen();
  bild.fillScreen(0);
}

// Stufe eines Punktes im Bild (0–15), bei „umgekehrt“ (Nacht) gespiegelt
static inline uint8_t punktStufe(const uint8_t* q, int i, bool umgekehrt) { const uint8_t l = q[i] >> 4; return umgekehrt ? 15 - l : l; }

// Schickt einen Ausschnitt (x0..x1, y0..y1, jeweils einschließlich) aus dem Bild an den Bildschirm.
static void bereichSenden(int x0, int y0, int x1, int y1, bool umgekehrt) {
  const uint8_t spalten[] = {(uint8_t)(x0 >> 8), (uint8_t)(x0 & 0xFF), (uint8_t)(x1 >> 8), (uint8_t)(x1 & 0xFF)};
  const uint8_t zeilen[]  = {(uint8_t)(y0 >> 8), (uint8_t)(y0 & 0xFF), (uint8_t)(y1 >> 8), (uint8_t)(y1 & 0xFF)};
  lcdBefehlMitDaten(0x2A, spalten, 4);
  lcdBefehlMitDaten(0x2B, zeilen, 4);
  lcdBefehl(0x2C);

  static uint8_t zeile[BREITE * 2];
  const uint8_t* quelle = bild.getBuffer();
  const int n = x1 - x0 + 1;

  digitalWrite(LCD_DC, HIGH);
  digitalWrite(LCD_CS, LOW);
  for (int y = y0; y <= y1; y++) {
    for (int i = 0; i < n; i++) {
      const int p = y * BREITE + x0 + i;
      const uint8_t l = punktStufe(quelle, p, umgekehrt);
      const uint16_t f = grauFarbe[l];
      zeile[i * 2] = (uint8_t)(f >> 8);
      zeile[i * 2 + 1] = (uint8_t)(f & 0xFF);
      gesendet[p] = l;
    }
    SPI1.transfer(zeile, n * 2);
  }
  digitalWrite(LCD_CS, HIGH);
}

// Schickt das Bild an den Bildschirm, aber nur, was sich seit dem letzten Mal geändert hat.
// umgekehrt = true zeigt Schwarz als Weiß und Weiß als Schwarz (Nacht).
void zeigen(bool umgekehrt = false) {
  const uint8_t* neu = bild.getBuffer();
  if (!gesendetGueltig || umgekehrt != gesendetUmgekehrt) {
    bereichSenden(0, 0, BREITE - 1, HOEHE - 1, umgekehrt);
    gesendetGueltig = true;
    gesendetUmgekehrt = umgekehrt;
  } else {
    // Zeilenweise vergleichen: Kurze Lücken (bis 4 unveränderte Zeilen) gehören zu einem Block.
    auto zeileAnders = [&](int y, int& von, int& bis) {
      von = BREITE; bis = -1;
      const int o = y * BREITE;
      for (int x = 0; x < BREITE; x++)
        if (punktStufe(neu, o + x, umgekehrt) != gesendet[o + x]) { if (x < von) von = x; bis = x; }
      return bis >= 0;
    };
    for (int y = 0; y < HOEHE;) {
      int von, bis;
      if (!zeileAnders(y, von, bis)) { y++; continue; }
      int y1 = y, luecke = 0, xMin = von, xMax = bis;
      for (int z = y + 1; z < HOEHE; z++) {
        int v, b;
        if (!zeileAnders(z, v, b)) { if (++luecke > 4) break; continue; }
        luecke = 0;
        y1 = z;
        if (v < xMin) xMin = v;
        if (b > xMax) xMax = b;
      }
      bereichSenden(xMin, y, xMax, y1, umgekehrt);
      y = y1 + 1;
    }
  }
#ifdef NACHBILDUNG
  nachbildungBildFertig();   // nur in der PC-Nachbildung (tools/pc-nachbildung)
#endif
}
