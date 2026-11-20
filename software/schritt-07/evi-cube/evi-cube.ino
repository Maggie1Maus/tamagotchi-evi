#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <U8g2lib.h>
#include "pet.h"
#include "button_input.h"
#include "menu.h"
#include "melody.h"
#include "save.h"
#include "evi_art.h"
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
const uint16_t DARK_BELOW = 120;
const uint16_t BRIGHT_ABOVE = 220;
const uint32_t LIGHT_CHANGE_MS = 1500;

const uint32_t FRAME_MS = 80;
// Spielstand alle 10 Minuten sichern (nur wenn sich etwas geaendert hat),
// ausserdem nach jeder Pflege und beim Einschlafen und Aufwachen.
const uint32_t SAVE_INTERVAL_MS = 600000UL;

// Bildschirm: links Evi, rechts ab PANEL_X die Werte oder das Menue.
const uint8_t PANEL_X = 76;
const uint8_t PANEL_W = 52;

// Ein kleiner Seitenpuffer spart Arbeitsspeicher auf dem Nano.
U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);
Pet pet;
SingleButtonInput button;
Menu menu;

struct EepromStorage {
  uint8_t read(uint16_t address) { return EEPROM.read(address); }
  void update(uint16_t address, uint8_t value) { EEPROM.update(address, value); }
};
EepromStorage eepromStorage;
SaveSlots<EepromStorage> saves(eepromStorage);
uint32_t lastSaveAt = 0;

volatile bool buttonPressPending = false;
uint32_t lastFrameAt = 0;

bool dark = false;
uint32_t lightStableSince = 0;
uint8_t announcedNeeds = 0;  // je Beduerfnis ein Bit: Hinweiston schon gespielt

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

void saveNow(uint32_t now) {
  saves.save(pet.state());
  lastSaveAt = now;
}

void handleCommand(Command command, uint32_t now) {
  if (command == Command::None) return;
  const GameAction action = menu.handle(command);
  if (action == GameAction::Feed) {
    pet.feed(now);
    playMelody(MELODY(MELODY_FEED));
  } else if (action == GameAction::Play) {
    pet.play(now);
    playMelody(MELODY(MELODY_PLAY));
  } else if (action == GameAction::Clean) {
    pet.clean(now);
    playMelody(MELODY(MELODY_CLEAN));
  } else if (command == Command::Back) {
    playMelody(MELODY(MELODY_BACK));
  } else {
    playMelody(MELODY(MELODY_TICK));
  }
  if (action != GameAction::None) saveNow(now);
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

// Licht aus: Evi schlaeft, wird langsamer hungrig, das Display wird dunkler
// und die LEDs gehen aus.
void updateLight(uint32_t now) {
  const uint16_t level = analogRead(LIGHT_PIN);
  const bool wantsDark = dark ? level < BRIGHT_ABOVE : level < DARK_BELOW;
  if (wantsDark == dark) {
    lightStableSince = now;
    return;
  }
  if (uint32_t(now - lightStableSince) < LIGHT_CHANGE_MS) return;
  // Bis jetzt im alten Tempo rechnen, ab jetzt im neuen.
  pet.update(now);
  dark = wantsDark;
  pet.setAsleep(dark);
  lightStableSince = now;
  display.setContrast(dark ? 1 : 255);
  if (!dark) playMelody(MELODY(MELODY_WAKE));
  saveNow(now);
}

// Ein Hinweiston, sobald ein Wert auf 30 % faellt. Im Dunkeln erst nach dem
// Aufwachen.
void updateAttention() {
  const PetState s = pet.state();
  const uint8_t values[Pet::NEEDS] = {s.satiety, s.mood, s.cleanliness};
  bool newNeed = false;
  for (uint8_t i = 0; i < Pet::NEEDS; ++i) {
    const uint8_t flag = 1 << i;
    if (values[i] > Pet::CARE_AT) {
      announcedNeeds &= ~flag;
    } else if (!(announcedNeeds & flag) && !dark) {
      announcedNeeds |= flag;
      newNeed = true;
    }
  }
  if (newNeed) playMelody(MELODY(MELODY_ATTENTION));
}

// Gruen = alles gut, gelb = bald braucht Evi etwas, rot blinkend = jetzt.
// Es zaehlt der niedrigste Wert. Nachts sind die LEDs aus.
void updateLeds(uint32_t now) {
  const uint8_t lowest = pet.lowest();
  const bool blinkOn = ((now / 400UL) % 2) == 0;
  digitalWrite(LED_GREEN_PIN, !dark && lowest > 50 ? HIGH : LOW);
  digitalWrite(LED_YELLOW_PIN, !dark && lowest > Pet::CARE_AT && lowest <= 50 ? HIGH : LOW);
  digitalWrite(LED_RED_PIN, !dark && lowest <= Pet::CARE_AT && blinkOn ? HIGH : LOW);
}

void drawEvi(Face face, Reaction reaction, uint32_t now) {
  const uint8_t *bits = ART_GLUECKLICH;
  uint8_t w = ART_GLUECKLICH_W, h = ART_GLUECKLICH_H;
  if (face == Face::Hungry) {
    bits = ART_HUNGRIG; w = ART_HUNGRIG_W; h = ART_HUNGRIG_H;
  } else if (face == Face::Sad) {
    bits = ART_TRAURIG; w = ART_TRAURIG_W; h = ART_TRAURIG_H;
  } else if (face == Face::Dirty) {
    bits = ART_SCHMUTZIG; w = ART_SCHMUTZIG_W; h = ART_SCHMUTZIG_H;
  } else if (face == Face::Sleeping) {
    bits = ART_SCHLAFEND; w = ART_SCHLAFEND_W; h = ART_SCHLAFEND_H;
  }
  // Beim Spielen tanzt Evi hin und her, beim Putzen schuettelt sie sich.
  int8_t dx = 0;
  if (reaction == Reaction::Playing) dx = ((now / 200UL) % 2) ? 3 : -3;
  else if (reaction == Reaction::Cleaning) dx = ((now / 70UL) % 2) ? 1 : -1;
  const uint8_t x = uint8_t(int16_t((PANEL_X - w) / 2) + dx);
  const uint8_t y = 64 - h;  // Fuesse unten am Rand
  display.drawXBMP(x, y, w, h, bits);
  if (face == Face::Sleeping) {
    display.setFont(u8g2_font_5x7_tr);
    display.drawStr(x + w - 18, ((now / 700UL) % 2) ? 12 : 16, "Zzz");
  }
}

void drawBar(uint8_t y, const char *label, uint8_t value) {
  display.drawStr(PANEL_X, y, label);
  if (value <= Pet::CARE_AT) display.drawStr(PANEL_X + PANEL_W - 5, y, "!");
  display.drawFrame(PANEL_X, y + 2, PANEL_W, 6);
  const uint8_t fill = (uint16_t(value) * (PANEL_W - 4)) / 100;
  if (fill > 0) display.drawBox(PANEL_X + 2, y + 4, fill, 2);
}

void drawPanel(const Menu &view, const PetState &state, Reaction reaction,
               bool holding, bool confirmed, bool waiting, uint32_t held) {
  display.setFont(u8g2_font_5x7_tr);
  if (view.screen() == Screen::Actions) {
    static const char *const NAMES[Menu::ACTIONS] = {"Fuettern", "Spielen", "Putzen"};
    for (uint8_t i = 0; i < Menu::ACTIONS; ++i) {
      const uint8_t y = 9 + i * 13;
      const bool selected = view.selectedAction() == i;
      if (selected) display.drawBox(PANEL_X, y - 8, PANEL_W, 10);
      display.setDrawColor(selected ? 0 : 1);
      display.drawStr(PANEL_X + 3, y, NAMES[i]);
      display.setDrawColor(1);
    }
  } else {
    drawBar(7, "Satt", state.satiety);
    drawBar(23, "Laune", state.mood);
    drawBar(39, "Sauber", state.cleanliness);
  }

  // Unterer Bereich: Reaktion, Halten-Balken oder Bedienhinweis.
  if (reaction == Reaction::Eating) {
    display.drawStr(PANEL_X + 4, 61, "Mjam!");
    return;
  }
  if (reaction == Reaction::Playing) {
    display.drawStr(PANEL_X + 4, 61, "Juhu!");
    return;
  }
  if (reaction == Reaction::Cleaning) {
    display.drawStr(PANEL_X + 4, 61, "Sauber!");
    return;
  }
  display.setFont(u8g2_font_4x6_tr);
  if (confirmed) {
    display.drawStr(PANEL_X, 63, "Loslassen");
  } else if (holding) {
    const uint8_t fill = held >= SingleButtonInput::LONG_PRESS_MS ? PANEL_W - 2 :
                         (held * (PANEL_W - 2)) / SingleButtonInput::LONG_PRESS_MS;
    display.drawStr(PANEL_X, 55, "Halten");
    display.drawFrame(PANEL_X, 58, PANEL_W, 6);
    if (fill > 0) display.drawBox(PANEL_X + 1, 59, fill, 4);
  } else if (waiting) {
    display.drawStr(PANEL_X, 63, "2x = zurueck");
  } else if (view.screen() == Screen::Actions) {
    display.drawStr(PANEL_X, 56, "1x = weiter");
    display.drawStr(PANEL_X, 63, "halten = OK");
  } else {
    display.drawStr(PANEL_X, 63, "Taste: Menue");
  }
}

void drawScene(uint32_t now) {
  // Derselbe Zustand wird fuer alle Displayseiten verwendet.
  const Face face = pet.face();
  const Reaction reaction = pet.reaction();
  const PetState state = pet.state();
  // Die Eingabe wird auch zwischen Displayseiten verarbeitet. Diese Kopien
  // halten das gerade gezeichnete Bild trotzdem ueber alle Seiten konsistent.
  const Menu view = menu;
  const bool holding = button.isHolding();
  const bool confirmed = button.isConfirmed();
  const bool waiting = button.isWaitingForSecondClick();
  const uint32_t held = button.heldFor(now);

  display.firstPage();
  do {
    drawEvi(face, reaction, now);
    drawPanel(view, state, reaction, holding, confirmed, waiting, held);
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

  // Taster beim Einstecken gedrueckt halten = neue Evi.
  const uint32_t now = millis();
  PetState saved;
  if (digitalRead(BUTTON_PIN) == HIGH) {
    saves.erase();
    pet.restore(Pet::startState(), now);
    saveNow(now);
    playMelody(MELODY(MELODY_NEW));
  } else {
    if (saves.load(saved)) pet.restore(saved, now);
    else saveNow(now);
    playMelody(MELODY(MELODY_HELLO));
  }
  lastSaveAt = now;
  enableButtonInterrupt();

  display.setI2CAddress(findDisplayAddress() * 2);
  display.setBusClock(400000);
  display.begin();
  display.setContrast(255);
  lightStableSince = now;
  drawScene(millis());
}

void loop() {
  const uint32_t now = millis();
  readButton(now);
  updateSound(now);
  updateLight(now);
  pet.update(now);
  updateAttention();
  updateLeds(now);
  if (uint32_t(now - lastSaveAt) >= SAVE_INTERVAL_MS) saveNow(now);

  if (uint32_t(now - lastFrameAt) >= FRAME_MS) {
    lastFrameAt = now;
    drawScene(now);
  }
}
