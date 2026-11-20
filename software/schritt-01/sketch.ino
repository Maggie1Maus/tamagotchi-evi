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
uint16_t greetingCount = 0;
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
    ++greetingCount;
    pet.greet(now);
  }

  // Erst nach ruhigem Loslassen erneut freigeben. So zaehlen weder
  // Kontaktprellen noch langes Gedrueckthalten als weitere Begruessung.
  if (pressed || digitalRead(BUTTON_PIN) == LOW) {
    lastButtonActivityAt = now;
  } else if (uint32_t(now - lastButtonActivityAt) >= DEBOUNCE_MS) {
    buttonLocked = false;
  }
}

void drawPet(bool greeting, bool blinking, uint8_t bounce) {
  const uint8_t y = 23 - bounce;

  // Ohren, Koerper und Fuesse. Die Figur ist ein vorlaeufiger Platzhalter.
  display.drawTriangle(51, y + 5, 52, y - 7, 60, y + 1);
  display.drawTriangle(67, y + 1, 75, y - 7, 76, y + 5);
  display.drawRFrame(50, y, 28, 24, 7);
  display.drawLine(54, y + 23, 52, 48);
  display.drawLine(73, y + 23, 75, 48);

  if (greeting || blinking) {
    display.drawLine(56, y + 9, 60, y + 9);
    display.drawLine(68, y + 9, 72, y + 9);
  } else {
    display.drawBox(57, y + 7, 2, 4);
    display.drawBox(69, y + 7, 2, 4);
  }

  display.drawLine(60, y + 15, 63, y + 17);
  display.drawLine(63, y + 17, 67, y + 15);
  display.drawLine(50, y + 13, 45, y + 16);
  if (greeting) {
    display.drawLine(77, y + 12, 84, y + 4);
    display.drawLine(84, y + 4, 84, y);
  } else {
    display.drawLine(77, y + 13, 82, y + 16);
  }
}

void drawScene(uint32_t now) {
  // Derselbe Zustand wird fuer alle Displayseiten verwendet.
  const bool greeting = pet.isGreeting();
  const bool blinking = (now % 4000UL) < 160;
  const uint8_t bounce = greeting ? ((now / 160UL) % 2) * 2 : 0;

  display.firstPage();
  do {
    display.setFont(u8g2_font_5x7_tr);
    display.drawStr(3, 8, greeting ? "Hallo zurueck!" : "Dein erstes Tier");
    display.drawHLine(0, 11, 128);
    drawPet(greeting, blinking, bounce);
    display.drawHLine(15, 49, 98);
    display.setCursor(3, 62);
    display.print("Hallo: ");
    display.print(greetingCount);
    display.drawStr(67, 62, "Klick/Space");
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
