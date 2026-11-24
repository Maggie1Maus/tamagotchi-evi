// Anschlüsse des Waveshare RP2350-Touch-LCD-2.8.
// Abgelesen aus Waveshares Beispielprogramm (Demo V2 vom 14.10.2026,
// Dateien DEV_Config.h und bsp_*.h). Auf dem Board fest verdrahtet.
#pragma once

// Im Retro Gamer sitzt die USB-C-Buchse links. Bild und Touch gemeinsam drehen.
const bool BILD_DREHEN = true;

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

// Stromversorgung: Waveshare setzt diesen Pin beim Start auf HIGH
const int BAT_EN  = 26;
const int BAT_ADC = 27;
