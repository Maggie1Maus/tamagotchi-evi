#ifndef TAMAGOTCHI_PET_H
#define TAMAGOTCHI_PET_H

#include <stdint.h>

// Tierzustand ohne Abhaengigkeit von Display oder Taster.
class Pet {
 public:
  void greet(uint32_t now) {
    greeting_ = true;
    greetedAt_ = now;
  }

  void update(uint32_t now) {
    if (greeting_ && uint32_t(now - greetedAt_) >= 1500UL) {
      greeting_ = false;
    }
  }

  bool isGreeting() const { return greeting_; }

 private:
  bool greeting_ = false;
  uint32_t greetedAt_ = 0;
};

#endif
