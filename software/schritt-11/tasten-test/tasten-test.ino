// Schritt 11: Tasten-Test für den Retro Gamer
//
// Liest die 8 Retro-Gamer-Tasten über den MCP23017 und zeigt sie auf dem Bildschirm.
//  - Oben: ob der MCP23017 gefunden wurde und welche I²C-Geräte am Bus hängen.
//  - Darunter die Tasten wie am Gerät: Steuerkreuz links, A und B rechts,
//    RESET und START unten. Gedrückt = weißes Kästchen, die Zahl zählt jeden Druck.
//  - Unten die Rohwerte der beiden Anschlussreihen (zum Nachsehen bei Fehlern).
//
// Verdrahtung (Beschluss 23.11.2026): jede Taste schaltet ihre Leitung auf Minus.
//   UP=A0, DOWN=A1, L=A2, R=A3, A=A4, B=A5, START=A6, RESET=B0, Minus=GND.
// Der MCP23017 hängt mit dem Kabel Adafruit 4401 am I²C-Stecker des Waveshare.
// Erwartet wird er am selben Bus wie Touch, Uhr und Lagesensor (Wire1, GP6/GP7).
//
// Board in der Arduino IDE: „Raspberry Pi Pico 2“. Bibliothek: „Adafruit GFX Library“.

#include <Wire.h>
#include "board.h"
#include "bildschirm.h"

// ---------- MCP23017 ----------
// Register bei Werkseinstellung (IOCON.BANK = 0)
const uint8_t MCP_IODIRA = 0x00;   // 1 = Eingang
const uint8_t MCP_IODIRB = 0x01;
const uint8_t MCP_GPPUA  = 0x0C;   // 1 = Pull-up an
const uint8_t MCP_GPPUB  = 0x0D;
const uint8_t MCP_GPIOA  = 0x12;   // Zustand der Anschlüsse, danach folgt GPIOB
const uint8_t MCP_OLATA  = 0x14;
const uint8_t MCP_OLATB  = 0x15;

uint8_t mcpAdresse = 0;            // 0 = nicht gefunden; normal 0x20

static bool i2cDa(uint8_t adresse) {
  Wire1.beginTransmission(adresse);
  return Wire1.endTransmission() == 0;
}

static bool mcpSchreiben(uint8_t reg, uint8_t wert) {
  Wire1.beginTransmission(mcpAdresse);
  Wire1.write(reg);
  Wire1.write(wert);
  return Wire1.endTransmission() == 0;
}

// Liest GPIOA und GPIOB auf einmal. Bit = 0 heißt: Leitung liegt auf Minus.
static bool mcpLesen(uint8_t& a, uint8_t& b) {
  Wire1.beginTransmission(mcpAdresse);
  Wire1.write(MCP_GPIOA);
  if (Wire1.endTransmission(false) != 0) return false;
  if (Wire1.requestFrom((int)mcpAdresse, 2, (int)true) != 2) return false;
  a = Wire1.read();
  b = Wire1.read();
  return true;
}

// A0–A6 und B0–B6 als Eingänge mit Pull-up. A7 und B7 sind laut neuem
// Datenblatt nur Ausgänge: als Ausgang auf LOW, dort ist nichts angeschlossen.
static bool mcpEinrichten() {
  return mcpSchreiben(MCP_OLATA, 0x00) && mcpSchreiben(MCP_OLATB, 0x00) &&
         mcpSchreiben(MCP_IODIRA, 0x7F) && mcpSchreiben(MCP_IODIRB, 0x7F) &&
         mcpSchreiben(MCP_GPPUA, 0x7F) && mcpSchreiben(MCP_GPPUB, 0x7F);
}

// ---------- Tasten ----------
struct Taste {
  const char* name;
  bool reiheB;      // false = Reihe A, true = Reihe B
  uint8_t bit;
  int x, y, b, h;   // Kästchen auf dem Bildschirm
  bool gedrueckt;
  int zaehler;
};

Taste tasten[] = {
  {"UP",     false, 0,  58,  70, 52, 40, false, 0},
  {"DOWN",   false, 1,  58, 160, 52, 40, false, 0},
  {"L",      false, 2,   4, 115, 52, 40, false, 0},
  {"R",      false, 3, 112, 115, 52, 40, false, 0},
  {"A",      false, 4, 180, 100, 62, 46, false, 0},
  {"B",      false, 5, 252, 100, 62, 46, false, 0},
  {"START",  false, 6, 230, 206, 84, 30, false, 0},
  {"RESET",  true,  0, 140, 206, 84, 30, false, 0},
};
const int ANZAHL = sizeof(tasten) / sizeof(tasten[0]);

uint8_t rohA = 0xFF, rohB = 0xFF;
char geraete[64] = "";

// Bekommt die Nummer der Taste, nicht die Taste selbst: Die Arduino IDE setzt
// Funktionsankündigungen an den Anfang der Datei, noch vor „struct Taste“.
void tasteZeichnen(int nummer) {
  const Taste& t = tasten[nummer];
  const uint16_t vorne = t.gedrueckt ? 0 : 1;
  bild.fillRect(t.x, t.y, t.b, t.h, t.gedrueckt ? 1 : 0);
  bild.drawRect(t.x, t.y, t.b, t.h, 1);
  bild.setTextColor(vorne);
  bild.setTextSize(t.h >= 40 ? 2 : 1);
  const int zeichenB = t.h >= 40 ? 12 : 6;
  bild.setCursor(t.x + (t.b - (int)strlen(t.name) * zeichenB) / 2, t.y + 5);
  bild.print(t.name);
  char zahl[8];
  snprintf(zahl, sizeof(zahl), "%d", t.zaehler);
  bild.setTextSize(1);
  bild.setCursor(t.x + (t.b - (int)strlen(zahl) * 6) / 2, t.y + t.h - 11);
  bild.print(zahl);
  bild.setTextColor(1);
}

void kopfZeichnen() {
  bild.fillRect(0, 0, BREITE, 62, 0);
  bild.setTextColor(1);
  bild.setTextSize(2);
  bild.setCursor(4, 4);
  bild.print("Tasten-Test");

  bild.setTextSize(1);
  bild.setCursor(4, 26);
  if (mcpAdresse) {
    char zeile[40];
    snprintf(zeile, sizeof(zeile), "MCP23017 gefunden (0x%02X)", mcpAdresse);
    bild.print(zeile);
  } else {
    bild.print("MCP23017 NICHT gefunden");
  }
  bild.setCursor(4, 38);
  bild.print("I2C: ");
  bild.print(geraete);
  bild.setCursor(4, 50);
  bild.print(mcpAdresse ? "Bitte jede Taste einmal dr\x81" "cken."
                        : "Kabel und L\x94tstellen pr\x81" "fen.");
}

void rohZeichnen() {
  bild.fillRect(0, 206, 136, 34, 0);
  if (!mcpAdresse) return;
  char zeile[32];
  bild.setTextSize(1);
  bild.setCursor(4, 212);
  snprintf(zeile, sizeof(zeile), "Reihe A: %02X", rohA);
  bild.print(zeile);
  bild.setCursor(4, 224);
  snprintf(zeile, sizeof(zeile), "Reihe B: %02X", rohB);
  bild.print(zeile);
}

void allesZeichnen() {
  bild.fillScreen(0);
  kopfZeichnen();
  for (int i = 0; i < ANZAHL; i++) tasteZeichnen(i);
  rohZeichnen();
  zeigen();
}

// Sucht alle Geräte am Bus und den MCP23017 (0x20–0x27, je nach Lötbrücken).
void busAbsuchen() {
  geraete[0] = 0;
  mcpAdresse = 0;
  for (uint8_t a = 0x08; a < 0x78; a++) {
    if (!i2cDa(a)) continue;
    char teil[6];
    snprintf(teil, sizeof(teil), "%02X ", a);
    if (strlen(geraete) + strlen(teil) < sizeof(geraete)) strcat(geraete, teil);
    if (a >= 0x20 && a <= 0x27 && mcpAdresse == 0) mcpAdresse = a;
  }
  if (geraete[0] == 0) strcpy(geraete, "keine");
  if (mcpAdresse && !mcpEinrichten()) mcpAdresse = 0;

  Serial.print("I2C-Geraete: ");
  Serial.println(geraete);
  Serial.println(mcpAdresse ? "MCP23017 gefunden" : "MCP23017 nicht gefunden");
}

void setup() {
  Serial.begin(115200);
  pinMode(BAT_EN, OUTPUT);
  digitalWrite(BAT_EN, HIGH);

  bildschirmStarten();
  bild.cp437(true);   // für ü und ö

  // Touch-Chip aus dem Reset holen, damit er den Bus nicht stört
  pinMode(TOUCH_RST, OUTPUT);
  digitalWrite(TOUCH_RST, HIGH);

  Wire1.setSDA(I2C_SDA);
  Wire1.setSCL(I2C_SCL);
  Wire1.setClock(100000);
  Wire1.begin();
  delay(50);

  busAbsuchen();
  allesZeichnen();
  helligkeit(80);
}

unsigned long letzteSuche = 0;

void loop() {
  // Ohne MCP23017 jede Sekunde neu suchen
  if (!mcpAdresse) {
    if (millis() - letzteSuche > 1000) {
      letzteSuche = millis();
      busAbsuchen();
      allesZeichnen();
    }
    return;
  }

  uint8_t a, b;
  if (!mcpLesen(a, b)) {
    // Verbindung verloren (z. B. Kabel gezogen)
    mcpAdresse = 0;
    allesZeichnen();
    return;
  }

  bool geaendert = (a != rohA) || (b != rohB);
  rohA = a;
  rohB = b;

  for (int i = 0; i < ANZAHL; i++) {
    Taste& t = tasten[i];
    const uint8_t reihe = t.reiheB ? b : a;
    const bool jetzt = !(reihe & (1 << t.bit));   // LOW = gedrückt
    if (jetzt != t.gedrueckt) {
      t.gedrueckt = jetzt;
      if (jetzt) {
        t.zaehler++;
        Serial.print(t.name);
        Serial.println(" gedrueckt");
      }
      geaendert = true;
    }
  }

  if (geaendert) {
    for (int i = 0; i < ANZAHL; i++) tasteZeichnen(i);
    rohZeichnen();
    zeigen();
  }
  delay(10);   // etwa 100-mal pro Sekunde nachsehen
}
