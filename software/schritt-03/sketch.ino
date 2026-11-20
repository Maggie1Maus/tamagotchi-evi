#include <Arduino.h>
#include <U8g2lib.h>
#include "pet.h"

// Vorlaeufige Anschluesse fuer die Simulation, noch keine Cube-Pinbelegung.
const uint8_t BUTTON_PIN = 2;
const uint32_t DEBOUNCE_MS = 30;
const uint32_t FRAME_MS = 80;
const uint32_t LONG_PRESS_MS = 700;

// Ein kleiner Seitenpuffer spart Arbeitsspeicher auf dem Nano.
U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);
Pet pet;

volatile bool buttonPressPending = false;
bool buttonLocked = false;
bool buttonDown = false;
bool actionDone = false;
uint8_t selectedAction = 0;  // 0: Winken, 1: Fuettern
uint32_t pressStartedAt = 0;
uint32_t lastButtonActivityAt = 0;
uint32_t lastFrameAt = 0;

// Merkt auch kurze Klicks waehrend der Displayuebertragung vor.
// Nur das Ereignis erfassen; Zeichnen und Spiellogik bleiben in loop().
void captureButtonPress() {
  buttonPressPending = true;
}

void executeAction(uint32_t now) {
  if (selectedAction == 0) {
    pet.greet(now);
  } else {
    pet.feed(now);
  }
}

void readButton(uint32_t now) {
  // Kopieren und Loeschen duerfen keinen neuen Tastendruck verschlucken.
  noInterrupts();
  const bool pressed = buttonPressPending;
  buttonPressPending = false;
  interrupts();
  buttonDown = digitalRead(BUTTON_PIN) == LOW;

  if (pressed && !buttonLocked) {
    buttonLocked = true;
    actionDone = false;
    pressStartedAt = now;
  }

  // Bestaetigen, sobald die Taste lange genug gehalten wurde.
  if (buttonLocked && !actionDone && buttonDown &&
      uint32_t(now - pressStartedAt) >= LONG_PRESS_MS) {
    executeAction(now);
    actionDone = true;
  }

  // Erst nach ruhigem Loslassen erneut freigeben. So zaehlen weder
  // Kontaktprellen noch langes Gedrueckthalten als weitere Aktion.
  if (pressed || buttonDown) {
    lastButtonActivityAt = now;
  } else if (buttonLocked &&
             uint32_t(now - lastButtonActivityAt) >= DEBOUNCE_MS) {
    if (!actionDone) {
      selectedAction = (selectedAction + 1) % 2;
    }
    buttonLocked = false;
  }
}

void drawPet(bool eating, bool greeting, bool hungry, bool blinking,
             uint8_t bounce, bool waveUp) {
  const uint8_t y = 23 - bounce;

  // Ohren, Koerper und Fuesse. Die Figur ist ein vorlaeufiger Platzhalter.
  display.drawTriangle(51, y + 5, 52, y - 7, 60, y + 1);
  display.drawTriangle(67, y + 1, 75, y - 7, 76, y + 5);
  display.drawRFrame(50, y, 28, 24, 7);
  display.drawLine(54, y + 23, 52, 48);
  display.drawLine(73, y + 23, 75, 48);

  if (eating || greeting || blinking) {
    display.drawLine(56, y + 9, 60, y + 9);
    display.drawLine(68, y + 9, 72, y + 9);
  } else {
    display.drawBox(57, y + 7, 2, 4);
    display.drawBox(69, y + 7, 2, 4);
  }

  if (hungry && !eating && !greeting) {
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
  } else if (greeting) {
    const uint8_t handY = waveUp ? y : y + 7;
    display.drawLine(77, y + 12, 84, handY + 4);
    display.drawLine(84, handY + 4, 84, handY);
  } else {
    display.drawLine(77, y + 13, 82, y + 16);
  }
}

void drawMenu(uint32_t now) {
  // Die Auswahl bleibt auch waehrend der Reaktion sichtbar.
  display.drawBox(selectedAction == 0 ? 3 : 62, 50,
                  selectedAction == 0 ? 51 : 63, 7);
  display.setDrawColor(selectedAction == 0 ? 0 : 1);
  display.drawStr(13, 56, "Winken");
  display.setDrawColor(selectedAction == 1 ? 0 : 1);
  display.drawStr(73, 56, "Fuettern");
  display.setDrawColor(1);

  if (buttonLocked && actionDone) {
    display.drawStr(40, 63, "Loslassen");
  } else if (buttonLocked && buttonDown) {
    const uint32_t held = uint32_t(now - pressStartedAt);
    const uint8_t fill = held >= LONG_PRESS_MS ? 82 :
                        (held * 82) / LONG_PRESS_MS;
    display.drawStr(3, 63, "Halten");
    display.drawFrame(40, 58, 84, 6);
    if (fill > 0) display.drawBox(41, 59, fill, 4);
  } else {
    display.drawStr(19, 63, "kurz: >  lang: OK");
  }
}

void drawScene(uint32_t now) {
  // Derselbe Zustand wird fuer alle Displayseiten verwendet.
  const bool eating = pet.isEating();
  const bool greeting = pet.isGreeting();
  const bool hungry = pet.isHungry();
  const uint8_t satiety = pet.satiety();
  const uint8_t barFill = (uint16_t(satiety) * 40) / 100;
  const bool blinking = (now % 4000UL) < 160;
  const bool waveUp = ((now / 160UL) % 2) == 0;
  const uint8_t bounce = (eating || greeting) ? (waveUp ? 2 : 0) : 0;

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
    drawPet(eating, greeting, hungry, blinking, bounce, waveUp);
    display.drawHLine(15, 48, 98);
    if (eating) {
      display.drawStr(90, 40, "Mjam!");
    } else if (greeting) {
      display.drawStr(3, 28, "Hallo!");
    } else if (hungry) {
      display.drawStr(3, 40, "Hunger!");
    }
    drawMenu(now);
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
