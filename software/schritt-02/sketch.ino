#include <Arduino.h>
#include <U8g2lib.h>
#include "pet.h"

// Vorlaeufige Anschluesse fuer die Simulation, noch keine Cube-Pinbelegung.
const uint8_t BUTTON_PIN = 2;
const uint32_t DEBOUNCE_MS = 30;
const uint32_t FRAME_MS = 80;

// Ein kleiner Seitenpuffer spart Arbeitsspeicher auf dem Nano.
U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);
Pet pet;

volatile bool buttonPressPending = false;
bool buttonLocked = false;
uint32_t lastButtonActivityAt = 0;
uint32_t lastFrameAt = 0;

// Merkt auch kurze Klicks waehrend der Displayuebertragung vor.
// Nur das Ereignis erfassen; Zeichnen und Spiellogik bleiben in loop().
void captureButtonPress() {
  buttonPressPending = true;
}

void readButton(uint32_t now) {
  // Kopieren und Loeschen duerfen keinen neuen Tastendruck verschlucken.
  noInterrupts();
  const bool pressed = buttonPressPending;
  buttonPressPending = false;
  interrupts();

  if (pressed && !buttonLocked) {
    buttonLocked = true;
    pet.feed(now);
  }

  // Erst nach ruhigem Loslassen erneut freigeben. So zaehlen weder
  // Kontaktprellen noch langes Gedrueckthalten als weiteres Fuettern.
  if (pressed || digitalRead(BUTTON_PIN) == LOW) {
    lastButtonActivityAt = now;
  } else if (uint32_t(now - lastButtonActivityAt) >= DEBOUNCE_MS) {
    buttonLocked = false;
  }
}

void drawPet(bool eating, bool hungry, bool blinking, uint8_t bounce) {
  const uint8_t y = 23 - bounce;

  // Ohren, Koerper und Fuesse. Die Figur ist ein vorlaeufiger Platzhalter.
  display.drawTriangle(51, y + 5, 52, y - 7, 60, y + 1);
  display.drawTriangle(67, y + 1, 75, y - 7, 76, y + 5);
  display.drawRFrame(50, y, 28, 24, 7);
  display.drawLine(54, y + 23, 52, 48);
  display.drawLine(73, y + 23, 75, 48);

  if (eating || blinking) {
    display.drawLine(56, y + 9, 60, y + 9);
    display.drawLine(68, y + 9, 72, y + 9);
  } else {
    display.drawBox(57, y + 7, 2, 4);
    display.drawBox(69, y + 7, 2, 4);
  }

  if (hungry && !eating) {
    display.drawLine(60, y + 17, 63, y + 15);
    display.drawLine(63, y + 15, 67, y + 17);
  } else {
    display.drawLine(60, y + 15, 63, y + 17);
    display.drawLine(63, y + 17, 67, y + 15);
  }
  display.drawLine(50, y + 13, 45, y + 16);
  if (eating) {
    display.drawLine(77, y + 12, 84, y + 4);
    display.drawLine(84, y + 4, 84, y);
    // Ein kleines Stueck Futter neben der angehobenen Pfote.
    display.drawDisc(87, y + 1, 2);
  } else {
    display.drawLine(77, y + 13, 82, y + 16);
  }
}

void drawScene(uint32_t now) {
  // Derselbe Zustand wird fuer alle Displayseiten verwendet.
  const bool eating = pet.isEating();
  const bool hungry = pet.isHungry();
  const uint8_t satiety = pet.satiety();
  const uint8_t barFill = (uint16_t(satiety) * 40) / 100;
  const bool blinking = (now % 4000UL) < 160;
  const uint8_t bounce = eating ? ((now / 160UL) % 2) * 2 : 0;

  display.firstPage();
  do {
    display.setFont(u8g2_font_5x7_tr);
    display.setCursor(3, 8);
    display.print("Satt: ");
    display.print(satiety);
    display.print('%');
    display.drawFrame(56, 1, 44, 8);
    if (barFill > 0) {
      display.drawBox(58, 3, barFill, 4);
    }
    display.drawStr(106, 8, "TEST");
    display.drawHLine(0, 11, 128);
    drawPet(eating, hungry, blinking, bounce);
    display.drawHLine(15, 49, 98);
    if (eating) {
      display.drawStr(37, 62, "Mjam! Danke.");
    } else if (hungry) {
      display.drawStr(4, 62, "Hunger! Taste: Fuettern");
    } else {
      display.drawStr(12, 62, "Klick/Space: Fuettern");
    }
  } while (display.nextPage());
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), captureButtonPress, FALLING);
  display.begin();
  drawScene(millis());
}

void loop() {
  const uint32_t now = millis();
  readButton(now);
  pet.update(now);

  if (uint32_t(now - lastFrameAt) >= FRAME_MS) {
    lastFrameAt = now;
    drawScene(now);
  }
}
