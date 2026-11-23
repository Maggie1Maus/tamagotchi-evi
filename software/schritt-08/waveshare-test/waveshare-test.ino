// Schritt 8: Erster Test für das Waveshare RP2350-Touch-LCD-2.8
//
// Prüft Bildschirm (quer, 320 x 240, schwarz-weiß), Touch und Ton.
//  - Beim Start: „Hallo Evi!“ und eine kleine Melodie.
//  - Mit dem Finger im großen Feld malen.
//  - TON spielt einen Piepton, NEU leert das Malfeld.
//
// Board in der Arduino IDE: „Raspberry Pi Pico 2“ (Paket „Raspberry Pi Pico/RP2040/RP2350“).
// Bibliothek: „Adafruit GFX Library“.

#include "board.h"
#include "bildschirm.h"
#include "touch.h"
#include "ton.h"

// Malfeld und Knöpfe
const int FELD_X = 4,   FELD_Y = 52,  FELD_B = 312, FELD_H = 138;
const int TON_X  = 10,  KNOPF_Y = 198, KNOPF_B = 140, KNOPF_H = 36;
const int NEU_X  = 170;

bool imRechteck(int x, int y, int rx, int ry, int rb, int rh) {
  return x >= rx && x < rx + rb && y >= ry && y < ry + rh;
}

void knopf(int x, const char* text) {
  bild.drawRoundRect(x, KNOPF_Y, KNOPF_B, KNOPF_H, 6, 1);
  bild.setTextSize(2);
  bild.setCursor(x + (KNOPF_B - (int)strlen(text) * 12) / 2, KNOPF_Y + 11);
  bild.print(text);
}

void malfeldLeeren() {
  bild.fillRect(FELD_X + 1, FELD_Y + 1, FELD_B - 2, FELD_H - 2, 0);
  bild.setTextSize(1);
  bild.setCursor(FELD_X + 8, FELD_Y + 8);
  bild.print("Hier mit dem Finger malen");
}

void zeileUnten(const char* text) {
  bild.fillRect(0, 40, BREITE, 10, 0);
  bild.setTextSize(1);
  bild.setCursor(4, 40);
  bild.print(text);
}

void grundbild() {
  bild.fillScreen(0);
  bild.setTextColor(1);
  bild.setTextSize(3);
  bild.setCursor(4, 4);
  bild.print("Hallo Evi!");

  bild.setTextSize(1);
  bild.setCursor(196, 6);
  bild.print("Touch:");
  bild.setCursor(196, 18);
  bild.print(touchName());

  bild.drawRect(FELD_X, FELD_Y, FELD_B, FELD_H, 1);
  malfeldLeeren();
  knopf(TON_X, "TON");
  knopf(NEU_X, "NEU");
  zeileUnten("Tippe auf das Feld oder einen Knopf.");
}

void setup() {
  Serial.begin(115200);
  pinMode(BAT_EN, OUTPUT);
  digitalWrite(BAT_EN, HIGH);

  bildschirmStarten();
  touchStarten();
  tonStarten();

  grundbild();
  zeigen();
  helligkeit(80);
  melodieHallo();
}

unsigned long letzterKnopf = 0;
unsigned long letztesZeigen = 0;
bool geaendert = false;

void loop() {
  int x, y;
  if (touchLesen(x, y)) {
    char zeile[64];
    snprintf(zeile, sizeof(zeile), "Finger bei x=%d y=%d   (roh %d / %d)", x, y, touchRohX, touchRohY);
    zeileUnten(zeile);
    geaendert = true;

    if (imRechteck(x, y, FELD_X + 3, FELD_Y + 3, FELD_B - 6, FELD_H - 6)) {
      bild.fillCircle(x, y, 3, 1);
    } else if (millis() - letzterKnopf > 400) {
      if (imRechteck(x, y, TON_X, KNOPF_Y, KNOPF_B, KNOPF_H)) {
        letzterKnopf = millis();
        bild.fillRoundRect(TON_X, KNOPF_Y, KNOPF_B, KNOPF_H, 6, 1);
        zeigen();
        melodiePiep();
        bild.fillRoundRect(TON_X, KNOPF_Y, KNOPF_B, KNOPF_H, 6, 0);
        knopf(TON_X, "TON");
      } else if (imRechteck(x, y, NEU_X, KNOPF_Y, KNOPF_B, KNOPF_H)) {
        letzterKnopf = millis();
        malfeldLeeren();
      }
    }
  }

  // höchstens etwa 30-mal pro Sekunde neu zeichnen
  if (geaendert && millis() - letztesZeigen > 30) {
    zeigen();
    geaendert = false;
    letztesZeigen = millis();
  }
}
