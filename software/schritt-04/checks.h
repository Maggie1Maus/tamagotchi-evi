#ifndef TAMAGOTCHI_CHECKS_H
#define TAMAGOTCHI_CHECKS_H

#include "button_input.h"
#include "menu.h"
#include "pet.h"

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
    checks.expect(menu.handle(Command::Next) == GameAction::None &&
                  menu.selectedAction() == 1, F("Next selects food without feeding"));
    checks.expect(menu.handle(Command::Confirm) == GameAction::Feed,
                  F("Confirm chooses food"));
    checks.expect(menu.handle(Command::Back) == GameAction::None &&
                  menu.screen() == Screen::Home && menu.selectedAction() == 1,
                  F("Back closes menu and preserves selection"));
    checks.expect(menu.handle(Command::Confirm) == GameAction::None &&
                  menu.screen() == Screen::Actions,
                  F("Confirm at home opens without feeding"));
    menu.handle(Command::Next);
    checks.expect(menu.handle(Command::Confirm) == GameAction::Greet,
                  F("same commands select greeting independently of buttons"));
  }
  {
    Pet pet;
    pet.update(1000);
    pet.feed(1000);
    checks.expect(pet.satiety() == 84 && pet.isEating(), F("feeding adds 25"));
    pet.greet(1000);
    checks.expect(pet.satiety() == 84 && pet.isGreeting() && !pet.isEating(),
                  F("greeting does not add food"));
    pet.update(2500);
    checks.expect(pet.satiety() == 83 && !pet.isGreeting(),
                  F("reaction expires while hunger continues"));
    pet.feed(2500);
    checks.expect(pet.satiety() == 100, F("food is capped at 100"));
    pet.update(200000);
    checks.expect(pet.satiety() == 0, F("hunger is capped at zero"));
  }
  Serial.print(F("Checks: "));
  Serial.print(checks.passed);
  Serial.print(F(" PASS, "));
  Serial.print(checks.failed);
  Serial.println(F(" FAIL"));
  return checks.failed == 0;
}

#endif
