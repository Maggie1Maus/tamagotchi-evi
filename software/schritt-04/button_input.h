#ifndef TAMAGOTCHI_BUTTON_INPUT_H
#define TAMAGOTCHI_BUTTON_INPUT_H

#include "commands.h"

struct ButtonCommands {
  Command first = Command::None;
  Command second = Command::None;

  void add(Command command) {
    if (first == Command::None) first = command;
    else second = command;
  }
};

// Keine Arduino-Abhaengigkeit: Zeit und Tasterzustand kommen von aussen.
class SingleButtonInput {
 public:
  static const uint32_t DOUBLE_CLICK_MS = 300;
  static const uint32_t LONG_PRESS_MS = 700;
  static const uint32_t RELEASE_DEBOUNCE_MS = 10;

  ButtonCommands update(uint32_t now, bool down, bool pressEdge) {
    ButtonCommands commands;

    // Ein abgelaufener Einzelklick wird vor einem neuen Druck ausgegeben.
    if (!active_ && waiting_ &&
        uint32_t(now - firstReleaseAt_) > DOUBLE_CLICK_MS) {
      commands.add(Command::Next);
      waiting_ = false;
    }

    if (pressEdge && !active_) {
      active_ = true;
      longSent_ = false;
      releasing_ = false;
      secondPress_ = waiting_;
      startedAt_ = now;
    }

    if (active_) {
      if (down) {
        releasing_ = false;
        if (!longSent_ && uint32_t(now - startedAt_) >= LONG_PRESS_MS) {
          // Kurz + lang bedeutet Weiter, dann Bestaetigen, keinen Zurueck-Befehl.
          if (waiting_) commands.add(Command::Next);
          waiting_ = false;
          commands.add(Command::Confirm);
          longSent_ = true;
          secondPress_ = false;
        }
      } else {
        if (!releasing_) {
          releasing_ = true;
          releaseAt_ = now;
        }
        if (uint32_t(now - releaseAt_) >= RELEASE_DEBOUNCE_MS) {
          if (!longSent_) {
            if (secondPress_ && waiting_) {
              commands.add(Command::Back);
              waiting_ = false;
            } else {
              waiting_ = true;
              firstReleaseAt_ = releaseAt_;
            }
          }
          active_ = false;
          releasing_ = false;
          secondPress_ = false;
        }
      }
    }

    if (!active_ && waiting_ &&
        uint32_t(now - firstReleaseAt_) > DOUBLE_CLICK_MS) {
      commands.add(Command::Next);
      waiting_ = false;
    }
    return commands;
  }

  bool isHolding() const { return active_ && !releasing_ && !longSent_; }
  bool isConfirmed() const { return active_ && longSent_; }
  bool isWaitingForSecondClick() const { return waiting_; }
  uint32_t heldFor(uint32_t now) const { return uint32_t(now - startedAt_); }

 private:
  bool active_ = false;
  bool releasing_ = false;
  bool longSent_ = false;
  bool waiting_ = false;
  bool secondPress_ = false;
  uint32_t startedAt_ = 0;
  uint32_t releaseAt_ = 0;
  uint32_t firstReleaseAt_ = 0;
};

#endif
