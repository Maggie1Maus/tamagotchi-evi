#ifndef TAMAGOTCHI_CHECKS_H
#define TAMAGOTCHI_CHECKS_H

#include "button_input.h"
#include "menu.h"
#include "pet.h"
#include "save.h"

// Nur beim separaten Prueflauf mit RUN_CHECKS aktiv, nicht im normalen Spiel.
struct Checks {
  uint16_t passed = 0;
  uint16_t failed = 0;

  void expect(bool condition, const __FlashStringHelper *name) {
    if (condition) ++passed;
    else {
      ++failed;
      Serial.print(F("FAIL: "));
      Serial.println(name);
    }
  }

  bool only(ButtonCommands commands, Command expected) {
    return commands.first == expected && commands.second == Command::None;
  }
};

bool runChecks() {
  Checks checks;
  {
    SingleButtonInput input;
    input.update(100, true, true);
    input.update(150, false, false);
    input.update(180, false, false);
    checks.expect(checks.only(input.update(450, false, false), Command::None),
                  F("single waits for double-click window"));
    checks.expect(checks.only(input.update(451, false, false), Command::Next),
                  F("single becomes Next once"));
    checks.expect(checks.only(input.update(900, false, false), Command::None),
                  F("single is not repeated"));
  }
  {
    SingleButtonInput input;
    input.update(100, true, true);
    input.update(120, false, false);
    input.update(150, false, false);
    checks.expect(checks.only(input.update(220, true, true), Command::None),
                  F("second press does not emit Next"));
    input.update(240, false, false);
    checks.expect(checks.only(input.update(270, false, false), Command::Back),
                  F("double becomes Back only"));
    checks.expect(checks.only(input.update(700, false, false), Command::None),
                  F("no delayed Next after double"));
  }
  {
    SingleButtonInput input;
    input.update(100, true, true);
    checks.expect(checks.only(input.update(799, true, false), Command::None),
                  F("hold is not confirmed too early"));
    checks.expect(checks.only(input.update(800, true, false), Command::Confirm),
                  F("hold becomes Confirm"));
    checks.expect(checks.only(input.update(1900, true, false), Command::None),
                  F("hold is not repeated"));
    input.update(2000, false, false);
    checks.expect(checks.only(input.update(2030, false, false), Command::None),
                  F("release after hold does not navigate"));
  }
  {
    SingleButtonInput input;
    input.update(100, true, true);
    input.update(120, false, false);
    input.update(150, false, false);
    input.update(220, true, true);
    const ButtonCommands commands = input.update(920, true, false);
    checks.expect(commands.first == Command::Next &&
                  commands.second == Command::Confirm,
                  F("short then long means Next then Confirm"));
    input.update(1000, false, false);
    checks.expect(checks.only(input.update(1030, false, false), Command::None),
                  F("short then long has no extra Back"));
  }
  {
    SingleButtonInput input;
    input.update(100, true, true);
    input.update(120, false, false);
    input.update(150, false, false);
    checks.expect(checks.only(input.update(500, true, true), Command::Next),
                  F("late second click emits first Next"));
    input.update(550, false, false);
    input.update(580, false, false);
    checks.expect(checks.only(input.update(851, false, false), Command::Next),
                  F("late second click becomes another Next"));
  }
  {
    SingleButtonInput input;
    input.update(100, true, true);
    input.update(120, false, false);
    input.update(125, true, true);
    input.update(130, false, false);
    checks.expect(checks.only(input.update(160, false, false), Command::None),
                  F("release bounce is not a double click"));
    checks.expect(checks.only(input.update(431, false, false), Command::Next),
                  F("bounced click still produces one Next"));
  }
  {
    SingleButtonInput input;
    input.update(0xffffff80UL, true, true);
    input.update(0xffffff90UL, false, false);
    input.update(0xffffffb0UL, false, false);
    checks.expect(checks.only(input.update(189, false, false), Command::Next),
                  F("single survives clock rollover"));
  }
  {
    SingleButtonInput input;
    input.update(0xffffff00UL, true, true);
    checks.expect(checks.only(input.update(444, true, false), Command::Confirm),
                  F("hold survives clock rollover"));
  }
  {
    SingleButtonInput input;
    input.update(100, true, true);
    input.update(180, false, true);
    input.update(190, false, false);
    checks.expect(checks.only(input.update(195, true, true), Command::None),
                  F("fast second press does not emit Next"));
    input.update(359, false, true);
    checks.expect(checks.only(input.update(369, false, false), Command::Back),
                  F("15 ms release gap is a double click"));
  }
  {
    Menu menu;
    checks.expect(menu.handle(Command::Back) == GameAction::None &&
                  menu.screen() == Screen::Home, F("Back at home is harmless"));
    checks.expect(menu.handle(Command::Next) == GameAction::None &&
                  menu.screen() == Screen::Actions && menu.selectedAction() == 0,
                  F("Next opens action menu"));
    checks.expect(menu.handle(Command::Confirm) == GameAction::Feed,
                  F("first action is feeding"));
    menu.handle(Command::Next);
    checks.expect(menu.handle(Command::Confirm) == GameAction::Play,
                  F("second action is playing"));
    menu.handle(Command::Next);
    checks.expect(menu.handle(Command::Confirm) == GameAction::Clean,
                  F("third action is cleaning"));
    checks.expect(menu.handle(Command::Next) == GameAction::None &&
                  menu.selectedAction() == 0, F("selection wraps around"));
    checks.expect(menu.handle(Command::Back) == GameAction::None &&
                  menu.screen() == Screen::Home && menu.selectedAction() == 0,
                  F("Back closes menu and preserves selection"));
    checks.expect(menu.handle(Command::Confirm) == GameAction::None &&
                  menu.screen() == Screen::Actions,
                  F("Confirm at home opens without acting"));
  }
  {
    // Echtes Tempo, wie bei der erwachsenen Evi in der Handy-App.
    const uint32_t MIN = 60000UL, HOUR = 60UL * MIN;
    Pet pet;
    checks.expect(pet.satiety() == 80 && pet.mood() == 80 &&
                  pet.cleanliness() == 100, F("starts at 80/80/100"));
    checks.expect(pet.face() == Face::Happy, F("happy at start"));
    pet.update(3 * MIN - 1);
    checks.expect(pet.satiety() == 80, F("no point lost before 3 minutes"));
    pet.update(HOUR);
    checks.expect(pet.satiety() == 60 && pet.mood() == 65 &&
                  pet.cleanliness() == 88, F("per hour: -20 / -15 / -12"));
    pet.setAsleep(true);
    pet.update(9 * HOUR);
    checks.expect(pet.satiety() == 36 && pet.mood() == 53 &&
                  pet.cleanliness() == 74, F("8 h asleep: -24 / -12 / -14"));
    checks.expect(pet.face() == Face::Sleeping, F("sleeping face in the dark"));
    pet.feed(9 * HOUR);
    checks.expect(pet.satiety() == 76 && pet.mood() == 56 &&
                  pet.cleanliness() == 72, F("feed: +40 / +3 / -2"));
    checks.expect(pet.face() == Face::Happy && pet.reaction() == Reaction::Eating,
                  F("eating wakes the face for a moment"));
    pet.update(9 * HOUR + 1500);
    checks.expect(pet.reaction() == Reaction::None && pet.face() == Face::Sleeping,
                  F("reaction ends after 1.5 s"));
    pet.setAsleep(false);
    pet.play(9 * HOUR + 1500);
    checks.expect(pet.satiety() == 68 && pet.mood() == 86 &&
                  pet.cleanliness() == 67, F("play: -8 / +30 / -5"));
    pet.clean(9 * HOUR + 1500);
    checks.expect(pet.cleanliness() == 100 && pet.mood() == 89,
                  F("clean: +60 capped at 100, +3 mood"));
  }
  {
    // Welches Bild: der niedrigste Wert entscheidet, ab 30.
    Pet pet;
    pet.restore(PetState{30, 50, 60}, 0);
    checks.expect(pet.face() == Face::Hungry && pet.isHungry(), F("hungry face"));
    pet.restore(PetState{40, 20, 60}, 0);
    checks.expect(pet.face() == Face::Sad && pet.isSad(), F("sad face"));
    pet.restore(PetState{40, 50, 10}, 0);
    checks.expect(pet.face() == Face::Dirty && pet.isDirty(), F("dirty face"));
    pet.restore(PetState{31, 31, 31}, 0);
    checks.expect(pet.face() == Face::Happy && !pet.needsCare(),
                  F("31 is still fine"));
    pet.restore(PetState{120, 0, 5}, 0);
    checks.expect(pet.satiety() == 100 && pet.face() == Face::Sad,
                  F("restore clamps, lowest wins"));
  }
  {
    // Lange Pause und Ueberlauf der Uhr nach 49 Tagen.
    Pet pet;
    pet.update(0xffff0000UL);
    checks.expect(pet.lowest() == 0, F("long gap empties without overflow"));
    pet.feed(0xffff0000UL);
    pet.update(0x0000ffffUL + 180000UL);
    checks.expect(pet.satiety() == 39, F("hunger survives clock rollover"));
  }
  {
    struct FakeStorage {
      uint8_t mem[16 * 8];  // 16 Plaetze zu 8 Bytes
      uint16_t writes = 0;
      uint8_t read(uint16_t a) { return mem[a]; }
      void update(uint16_t a, uint8_t v) { if (mem[a] != v) { mem[a] = v; ++writes; } }
    } storage;
    for (uint16_t i = 0; i < sizeof(storage.mem); ++i) storage.mem[i] = 0xFF;
    SaveSlots<FakeStorage> slots(storage);
    PetState loaded = {0, 0, 0};
    checks.expect(!slots.load(loaded), F("empty EEPROM has no save"));
    checks.expect(slots.save(PetState{70, 60, 50}), F("first save writes"));
    checks.expect(!slots.save(PetState{70, 60, 50}), F("unchanged is not written"));
    SaveSlots<FakeStorage> reread(storage);
    checks.expect(reread.load(loaded) && loaded.satiety == 70 && loaded.mood == 60 &&
                  loaded.cleanliness == 50, F("save can be loaded"));
    // 300 Speicherungen: reihum, mit Ueberlauf der Folgenummer.
    for (uint16_t i = 0; i < 300; ++i) {
      reread.save(PetState{uint8_t(i % 101), uint8_t((i + 1) % 101), 7});
    }
    SaveSlots<FakeStorage> third(storage);
    checks.expect(third.load(loaded) && loaded.satiety == 299 % 101 &&
                  loaded.mood == 300 % 101, F("newest of many saves wins"));
    // Strom fehlt mitten im Schreiben: Pruefsumme passt nicht.
    third.save(PetState{1, 2, 3});
    SaveSlots<FakeStorage> fourth(storage);
    fourth.load(loaded);
    checks.expect(loaded.satiety == 1, F("latest save found before damage"));
    // Den eben geschriebenen Platz beschaedigen (Pruefsumme kippen).
    for (uint8_t i = 0; i < 16; ++i) {
      const uint16_t base = uint16_t(i) * 8;
      if (storage.mem[base + 3] == 1 && storage.mem[base + 4] == 2) storage.mem[base + 6] ^= 0xFF;
    }
    SaveSlots<FakeStorage> fifth(storage);
    checks.expect(fifth.load(loaded) && loaded.satiety == 299 % 101,
                  F("damaged save falls back to previous"));
    fifth.erase();
    SaveSlots<FakeStorage> sixth(storage);
    checks.expect(!sixth.load(loaded), F("erase starts a new Evi"));
  }
  Serial.print(F("Checks: "));
  Serial.print(checks.passed);
  Serial.print(F(" PASS, "));
  Serial.print(checks.failed);
  Serial.println(F(" FAIL"));
  return checks.failed == 0;
}

#endif
