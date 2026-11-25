// Anschlüsse des Waveshare RP2350-Touch-LCD-2.8.
// Abgelesen aus Waveshares Beispielprogramm (Demo V2 vom 14.10.2026,
// Dateien DEV_Config.h und bsp_*.h). Auf dem Board fest verdrahtet.
#pragma once

// Bildschirm ST7789T3, 240 x 320 Pixel, an SPI1
const int LCD_SCK  = 10;
const int LCD_MOSI = 11;
const int LCD_MISO = 12;   // wird nicht benutzt, SPI1 braucht aber einen Pin
const int LCD_CS   = 13;
const int LCD_DC   = 14;
const int LCD_RST  = 15;
const int LCD_BL   = 16;   // Hintergrundbeleuchtung (PWM = Helligkeit)

// Touch, Uhr (PCF85063) und Lagesensor (QMI8658) an I2C1 = Wire1
const int I2C_SDA   = 6;
const int I2C_SCL   = 7;
const int TOUCH_RST = 17;
const int TOUCH_INT = 18;

// Ton: I2S-Wandler, LRCK ist automatisch BCK + 1 = 3
const int I2S_BCK  = 2;
const int I2S_DIN  = 4;

// Stromversorgung (Schaltplan RP2350-Touch-LCD-2.8, von der Nutzerin am 25.11.2026 geschickt):
// BAT_EN hält den Akku am eigenen Akku-Anschluss J1 eingeschaltet (über den Transistor T1 und Q1/Q2).
// Evi bekommt ihren Strom aber über VBUS (Pololu am Stecker J4); solange VBUS da ist, sperrt Q2 den Akku-Weg
// ohnehin. HIGH kostet nur Strom: etwa 2,6 mA Basisstrom durch R15 (1 kΩ) und 0,3 mA über R8/D1/D2.
// Waveshare setzt den Pin beim Start auf HIGH. Läuft das Board einmal von einem Akku an J1, hier true eintragen.
const int BAT_EN  = 26;
const int BAT_ADC = 27;
const bool BAT_HALTEN = false;
// Leitungen mit 10-kΩ-Pull-up auf dem Board, die Evi nicht braucht: SD-Karte (GP19–24) und die Akku-Taste (GP25).
// Nach dem Start ist im Chip ein Pull-down an; gegen den Pull-up fließen sonst je etwa 55 µA.
const int OHNE_PULL[] = {19, 20, 21, 22, 23, 24, 25};
