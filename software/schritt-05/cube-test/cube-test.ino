#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

// Prueft die fuer Evi wichtigen Bauteile des Funduino Cube einzeln.
// Pinbelegung laut Platinenbeschriftung (Loetanleitung V2).
const uint8_t BUTTON_PIN = 9;   // gedrueckt = HIGH
const uint8_t SPEAKER_PIN = 7;
const uint8_t LED_PINS[3] = {12, 11, 10};
const char *const LED_NAMES[3] = {"rot (D12)", "gelb (D11)", "gruen (D10)"};
const uint8_t LIGHT_PIN = A7;

U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

uint8_t displayAddress = 0;
uint16_t presses = 0;
bool buttonDown = false;
uint32_t buttonChangedAt = 0;
uint8_t ledIndex = 0;
uint32_t ledChangedAt = 0;
uint32_t lastFrameAt = 0;
uint32_t lastReportAt = 0;
uint16_t lightLevel = 0;

// Sucht das Display unter beiden moeglichen Adressen.
uint8_t findDisplayAddress() {
  Wire.begin();
  for (uint8_t address = 0x3C; address <= 0x3D; ++address) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) return address;
  }
  return 0;
}

void updateButton(uint32_t now) {
  const bool down = digitalRead(BUTTON_PIN) == HIGH;
  // 30 ms Ruhezeit gegen Kontaktprellen.
  if (down == buttonDown || uint32_t(now - buttonChangedAt) < 30) return;
  buttonDown = down;
  buttonChangedAt = now;
  if (down) {
    ++presses;
    tone(SPEAKER_PIN, presses % 2 ? 880 : 1319, 80);
  }
}

void updateLeds(uint32_t now) {
  if (uint32_t(now - ledChangedAt) < 600) return;
  ledChangedAt = now;
  ledIndex = (ledIndex + 1) % 3;
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(LED_PINS[i], i == ledIndex ? HIGH : LOW);
  }
}

void drawScreen() {
  const uint8_t bar = uint32_t(lightLevel) * 60 / 1023;
  display.firstPage();
  do {
    display.setFont(u8g2_font_6x10_tr);
    display.drawStr(0, 9, "Cube-Test");
    display.setFont(u8g2_font_5x7_tr);
    display.setCursor(70, 8);
    display.print("OLED 0x");
    display.print(displayAddress, HEX);
    display.drawHLine(0, 12, 128);

    display.drawStr(0, 22, "Taster:");
    display.drawStr(40, 22, buttonDown ? "GEDRUECKT" : "los");
    display.setCursor(0, 31);
    display.print("Druecke: ");
    display.print(presses);

    display.setCursor(0, 41);
    display.print("Licht A7: ");
    display.print(lightLevel);
    display.drawFrame(66, 35, 62, 7);
    if (bar > 0) display.drawBox(67, 36, bar, 5);

    display.drawStr(0, 51, "LED an:");
    display.drawStr(40, 51, LED_NAMES[ledIndex]);
    display.drawStr(0, 62, "Druecken = Piep");
  } while (display.nextPage());
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);  // externer Widerstand R4 auf der Platine
  for (uint8_t i = 0; i < 3; ++i) pinMode(LED_PINS[i], OUTPUT);

  displayAddress = findDisplayAddress();
  if (displayAddress != 0) {
    display.setI2CAddress(displayAddress * 2);
    display.begin();
  }
  Serial.println(F("Cube-Test gestartet"));
  if (displayAddress == 0) {
    Serial.println(F("Kein Display unter 0x3C oder 0x3D gefunden."));
  }
  tone(SPEAKER_PIN, 1047, 150);
}

void loop() {
  const uint32_t now = millis();
  updateButton(now);
  updateLeds(now);
  lightLevel = analogRead(LIGHT_PIN);

  if (displayAddress != 0 && uint32_t(now - lastFrameAt) >= 100) {
    lastFrameAt = now;
    drawScreen();
  }
  if (uint32_t(now - lastReportAt) >= 500) {
    lastReportAt = now;
    Serial.print(F("Taster="));
    Serial.print(buttonDown ? F("gedrueckt") : F("los"));
    Serial.print(F(" Druecke="));
    Serial.print(presses);
    Serial.print(F(" Licht="));
    Serial.println(lightLevel);
  }
}
