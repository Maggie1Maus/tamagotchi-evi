#ifndef TAMAGOTCHI_PET_H
#define TAMAGOTCHI_PET_H

#include <stdint.h>

// Vorlaeufige Testregeln, unabhaengig von Display und Eingabe.
class Pet {
 public:
  void feed(uint32_t now) {
    update(now);
    const uint16_t nextSatiety = satiety_ + 25;
    satiety_ = nextSatiety > 100 ? 100 : nextSatiety;
    eating_ = true;
    greeting_ = false;
    reactionAt_ = now;
  }

  void greet(uint32_t now) {
    update(now);
    greeting_ = true;
    eating_ = false;
    reactionAt_ = now;
  }

  void update(uint32_t now) {
    // Testtempo: ein Saettigungspunkt pro simulierter Sekunde.
    const uint32_t steps = uint32_t(now - lastHungerAt_) / 1000UL;
    if (steps > 0) {
      lastHungerAt_ += steps * 1000UL;
      satiety_ = steps >= satiety_ ? 0 : satiety_ - steps;
    }
    if (uint32_t(now - reactionAt_) >= 1500UL) {
      eating_ = false;
      greeting_ = false;
    }
  }

  uint8_t satiety() const { return satiety_; }
  bool isHungry() const { return satiety_ <= 30; }
  bool isEating() const { return eating_; }
  bool isGreeting() const { return greeting_; }

 private:
  uint8_t satiety_ = 60;
  bool eating_ = false;
  bool greeting_ = false;
  uint32_t lastHungerAt_ = 0;
  uint32_t reactionAt_ = 0;
};

#endif
