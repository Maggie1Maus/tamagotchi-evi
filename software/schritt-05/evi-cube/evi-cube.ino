#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "pet.h"
#include "button_input.h"
#include "menu.h"
#include "melody.h"
#ifdef RUN_CHECKS
#include "checks.h"
#endif

// Pinbelegung des Funduino Cube laut Platinenbeschriftung (Loetanleitung V2).
const uint8_t BUTTON_PIN = 9;      // Taster SW1: gedrueckt = HIGH, R4 zieht nach GND
const uint8_t SPEAKER_PIN = 7;     // Lautsprecher BZ1
const uint8_t LED_RED_PIN = 12;    // LED 1
const uint8_t LED_YELLOW_PIN = 11; // LED 2
const uint8_t LED_GREEN_PIN = 10;  // LED 3
const uint8_t LIGHT_PIN = A7;      // Fotowiderstand: hell = hoher Wert

// Unter DARK_BELOW schlaeft Evi, ueber BRIGHT_ABOVE wacht sie auf.
// Der Abstand verhindert Flackern. Werte mit dem Cube-Test pruefen.
const uint16_t DARK_BELOW = 120;
const uint16_t BRIGHT_ABOVE = 220;
const uint32_t LIGHT_CHANGE_MS = 1500;

const uint32_t FRAME_MS = 80;

// Ein kleiner Seitenpuffer spart Arbeitsspeicher auf dem Nano.
U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);
Pet pet;
SingleButtonInput button;
Menu menu;

volatile bool buttonPressPending = false;
uint32_t lastFrameAt = 0;

bool dark = false;
uint32_t lightStableSince = 0;
bool hungerAnnounced = false;

const Note *melody = nullptr;
uint8_t melodyLength = 0;
uint8_t melodyIndex = 0;
uint32_t noteEndsAt = 0;

// D9 hat keinen eigenen Interrupt. Der Pin-Change-Interrupt meldet jede
// Aenderung an D8 bis D13; gemerkt wird nur, wenn der Taster gedrueckt ist.
// So gehen auch kurze Klicks waehrend der Displayuebertragung nicht verloren.
ISR(PCINT0_vect) {
  if (digitalRead(BUTTON_PIN) == HIGH) buttonPressPending = true;
}

void enableButtonInterrupt() {
  *digitalPinToPCMSK(BUTTON_PIN) |= bit(digitalPinToPCMSKbit(BUTTON_PIN));
  PCIFR |= bit(digitalPinToPCICRbit(BUTTON_PIN));
  PCICR |= bit(digitalPinToPCICRbit(BUTTON_PIN));
}

// Die OLEDs auf dem Cube melden sich laut Funduino unter 0x3C oder 0x3D.
uint8_t findDisplayAddress() {
  Wire.begin();
  for (uint8_t address = 0x3C; address <= 0x3D; ++address) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) return address;
  }
  return 0x3C;
}

void playMelody(const Note *notes, uint8_t length) {
  melody = notes;
  melodyLength = length;
  melodyIndex = 0;
  noteEndsAt = millis();
}

// Spielt Melodien ohne Warten ab, damit Anzeige und Taster weiterlaufen.
void updateSound(uint32_t now) {
  if (melody == nullptr || int32_t(now - noteEndsAt) < 0) return;
  if (melodyIndex >= melodyLength) {
    melody = nullptr;
    noTone(SPEAKER_PIN);
    return;
  }
  const uint16_t freq = pgm_read_word(&melody[melodyIndex].freq);
  const uint16_t ms = pgm_read_word(&melody[melodyIndex].ms);
  if (freq > 0) tone(SPEAKER_PIN, freq, ms);
  else noTone(SPEAKER_PIN);
  noteEndsAt = now + ms;
  ++melodyIndex;
}

void handleCommand(Command command, uint32_t now) {
  if (command == Command::None) return;
  const GameAction action = menu.handle(command);
  if (action == GameAction::Greet) {
    pet.greet(now);
    playMelody(MELODY(MELODY_GREET));
  } else if (action == GameAction::Feed) {
    pet.feed(now);
    playMelody(MELODY(MELODY_FEED));
  } else if (command == Command::Back) {
    playMelody(MELODY(MELODY_BACK));
  } else {
    playMelody(MELODY(MELODY_TICK));
  }
}

void readButton(uint32_t now) {
  // Kopieren und Loeschen duerfen keinen neuen Tastendruck verschlucken.
  noInterrupts();
  const bool pressed = buttonPressPending;
  buttonPressPending = false;
  interrupts();
  const bool down = digitalRead(BUTTON_PIN) == HIGH;
  const ButtonCommands commands = button.update(now, down, pressed);
  handleCommand(commands.first, now);
  handleCommand(commands.second, now);
}

// Licht aus: Evi schlaeft, das Display wird dunkler und die LEDs gehen aus.
void updateLight(uint32_t now) {
  const uint16_t level = analogRead(LIGHT_PIN);
  const bool wantsDark = dark ? level < BRIGHT_ABOVE : level < DARK_BELOW;
  if (wantsDark == dark) {
    lightStableSince = now;
    return;
  }
  if (uint32_t(now - lightStableSince) < LIGHT_CHANGE_MS) return;
  dark = wantsDark;
  lightStableSince = now;
  display.setContrast(dark ? 1 : 255);
  if (!dark) playMelody(MELODY(MELODY_WAKE));
}

// Ein Hinweiston, sobald Evi Hunger hat. Im Dunkeln erst nach dem Aufwachen.
void updateAttention() {
  if (!pet.isHungry()) {
    hungerAnnounced = false;
    return;
  }
  if (!hungerAnnounced && !dark) {
    playMelody(MELODY(MELODY_HUNGRY));
    hungerAnnounced = true;
  }
}

// Gruen = satt, gelb = bald hungrig, rot blinkend = Hunger. Nachts aus.
void updateLeds(uint32_t now) {
  const uint8_t satiety = pet.satiety();
  const bool blinkOn = ((now / 400UL) % 2) == 0;
  digitalWrite(LED_GREEN_PIN, !dark && satiety > 50 ? HIGH : LOW);
  digitalWrite(LED_YELLOW_PIN,
               !dark && satiety > 30 && satiety <= 50 ? HIGH : LOW);
  digitalWrite(LED_RED_PIN, !dark && pet.isHungry() && blinkOn ? HIGH : LOW);
}

void drawPet(bool eating, bool greeting, bool hungry, bool eyesClosed,
             uint8_t bounce, bool waveUp) {
  const uint8_t y = 23 - bounce;

  // Ohren, Koerper und Fuesse. Die Figur ist ein vorlaeufiger Platzhalter.
  display.drawTriangle(51, y + 5, 52, y - 7, 60, y + 1);
  display.drawTriangle(67, y + 1, 75, y - 7, 76, y + 5);
  display.drawRFrame(50, y, 28, 24, 7);
  display.drawLine(54, y + 23, 52, 48);
  display.drawLine(73, y + 23, 75, 48);

  if (eating || greeting || eyesClosed) {
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

void drawMenu(const Menu &view, bool holding, bool confirmed, bool waiting,
              uint32_t held) {
  if (view.screen() == Screen::Actions) {
    const uint8_t selected = view.selectedAction();
    display.drawBox(selected == 0 ? 3 : 62, 50, selected == 0 ? 51 : 63, 7);
    display.setDrawColor(selected == 0 ? 0 : 1);
    display.drawStr(13, 56, "Winken");
    display.setDrawColor(selected == 1 ? 0 : 1);
    display.drawStr(73, 56, "Fuettern");
    display.setDrawColor(1);
  } else {
    display.drawStr(9, 56, "Aktionen: kurz/halten");
  }

  display.setFont(u8g2_font_4x6_tr);
  if (confirmed) {
    display.drawStr(40, 63, "Loslassen");
  } else if (holding) {
    const uint8_t fill = held >= SingleButtonInput::LONG_PRESS_MS ? 82 :
                        (held * 82) / SingleButtonInput::LONG_PRESS_MS;
    display.drawStr(3, 63, "Halten");
    display.drawFrame(40, 58, 84, 6);
    if (fill > 0) display.drawBox(41, 59, fill, 4);
  } else if (waiting) {
    display.drawStr(18, 63, "2. Klick? -> Zurueck");
  } else {
    display.drawStr(18, 63, "1x >  2x <  halten OK");
  }
}

void drawScene(uint32_t now) {
  // Derselbe Zustand wird fuer alle Displayseiten verwendet.
  const bool eating = pet.isEating();
  const bool greeting = pet.isGreeting();
  const bool hungry = pet.isHungry();
  const bool sleeping = dark && !eating && !greeting;
  const uint8_t satiety = pet.satiety();
  const uint8_t barFill = (uint16_t(satiety) * 40) / 100;
  const bool blinking = (now % 4000UL) < 160;
  const bool waveUp = ((now / 160UL) % 2) == 0;
  const uint8_t bounce = (eating || greeting) ? (waveUp ? 2 : 0) : 0;
  const bool zzzHigh = ((now / 700UL) % 2) == 0;
  // Die Eingabe wird auch zwischen Displayseiten verarbeitet. Diese Kopien
  // halten das gerade gezeichnete Bild trotzdem ueber alle Seiten konsistent.
  const Menu view = menu;
  const bool holding = button.isHolding();
  const bool confirmed = button.isConfirmed();
  const bool waiting = button.isWaitingForSecondClick();
  const uint32_t held = button.heldFor(now);

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
    if (view.screen() == Screen::Home) display.drawStr(3, 20, "Zuhause");
    drawPet(eating, greeting, hungry, blinking || sleeping, bounce, waveUp);
    display.drawHLine(15, 48, 98);
    if (eating) {
      display.drawStr(90, 40, "Mjam!");
    } else if (greeting) {
      display.drawStr(3, 28, "Hallo!");
    } else if (sleeping) {
      display.drawStr(84, zzzHigh ? 20 : 24, "Zzz");
    } else if (hungry) {
      display.drawStr(3, 40, "Hunger!");
    }
    drawMenu(view, holding, confirmed, waiting, held);
    const uint32_t pageNow = millis();
    readButton(pageNow);
    updateSound(pageNow);
  } while (display.nextPage());
}

void setup() {
#ifdef RUN_CHECKS
  Serial.begin(115200);
  if (!runChecks()) {
    while (true) {}
  }
#endif
  pinMode(BUTTON_PIN, INPUT);  // externer Widerstand R4 auf der Platine
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  enableButtonInterrupt();

  display.setI2CAddress(findDisplayAddress() * 2);
  display.setBusClock(400000);
  display.begin();
  display.setContrast(255);
  lightStableSince = millis();
  playMelody(MELODY(MELODY_HELLO));
  drawScene(millis());
}

void loop() {
  const uint32_t now = millis();
  readButton(now);
  updateSound(now);
  pet.update(now);
  updateLight(now);
  updateAttention();
  updateLeds(now);

  if (uint32_t(now - lastFrameAt) >= FRAME_MS) {
    lastFrameAt = now;
    drawScene(now);
  }
}
