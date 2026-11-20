#ifndef TAMAGOTCHI_MENU_H
#define TAMAGOTCHI_MENU_H

#include "commands.h"

enum class Screen : uint8_t { Home, Actions };
enum class GameAction : uint8_t { None, Feed, Play, Clean };

// Das Menue kennt nur Befehle, weder Taster noch Display oder Tierzustand.
class Menu {
 public:
  static const uint8_t ACTIONS = 3;  // Fuettern, Spielen, Putzen

  GameAction handle(Command command) {
    if (command == Command::Back) {
      screen_ = Screen::Home;
    } else if (command != Command::None && screen_ == Screen::Home) {
      screen_ = Screen::Actions;
    } else if (command == Command::Next) {
      selected_ = (selected_ + 1) % ACTIONS;
    } else if (command == Command::Confirm) {
      return selected_ == 0 ? GameAction::Feed :
             selected_ == 1 ? GameAction::Play : GameAction::Clean;
    }
    return GameAction::None;
  }

  Screen screen() const { return screen_; }
  uint8_t selectedAction() const { return selected_; }

 private:
  Screen screen_ = Screen::Home;
  uint8_t selected_ = 0;
};

#endif
