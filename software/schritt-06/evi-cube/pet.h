#ifndef TAMAGOTCHI_PET_H
#define TAMAGOTCHI_PET_H

#include <stdint.h>

// Spielregeln fuer Evi, unabhaengig von Display und Eingabe.
// Die Werte entsprechen der erwachsenen Evi in der Handy-App
// (handy-app/dist/pet.js): Pflege etwa alle 3-4 Stunden.
class Pet {
 public:
  static const uint8_t START_SATIETY = 80;
  static const uint8_t FEED_AMOUNT = 40;
  static const uint8_t HUNGRY_AT = 30;
  // 20 Punkte pro Stunde: ein Punkt alle 3 Minuten.
  static const uint32_t MS_PER_POINT = 180000UL;
  // Im Schlaf (Licht aus) sinkt die Saettigung nur mit 15 % des Tempos.
  static const uint8_t SLEEP_PERCENT = 15;
  static const uint32_t REACTION_MS = 1500;

  // msPerPoint nur fuer Pruefungen veraendern.
  explicit Pet(uint32_t msPerPoint = MS_PER_POINT) : msPerPoint_(msPerPoint) {}

  void feed(uint32_t now) {
    update(now);
    const uint16_t nextSatiety = satiety_ + FEED_AMOUNT;
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

  // Vorher update(now) aufrufen, damit die Zeit davor im alten Tempo zaehlt.
  void setAsleep(bool asleep) { asleep_ = asleep; }

  void update(uint32_t now) {
    uint32_t elapsed = uint32_t(now - lastUpdateAt_);
    lastUpdateAt_ = now;
    // Fortschritt in Hundertsteln, damit auch das Schlaftempo genau bleibt.
    // In Stuecken von hoechstens einer Stunde, damit nichts ueberlaeuft.
    const uint32_t perPoint = msPerPoint_ * 100UL;
    while (elapsed > 0) {
      const uint32_t chunk = elapsed > 3600000UL ? 3600000UL : elapsed;
      elapsed -= chunk;
      progress_ += chunk * (asleep_ ? SLEEP_PERCENT : 100U);
      const uint32_t steps = progress_ / perPoint;
      progress_ -= steps * perPoint;
      satiety_ = steps >= satiety_ ? 0 : satiety_ - steps;
      // Leer bleibt leer; angesammelter Rest zaehlt nach dem Fuettern nicht.
      if (satiety_ == 0) progress_ = 0;
    }
    if (uint32_t(now - reactionAt_) >= REACTION_MS) {
      eating_ = false;
      greeting_ = false;
    }
  }

  uint8_t satiety() const { return satiety_; }
  bool isHungry() const { return satiety_ <= HUNGRY_AT; }
  bool isEating() const { return eating_; }
  bool isGreeting() const { return greeting_; }
  bool isAsleep() const { return asleep_; }

 private:
  uint32_t msPerPoint_;
  uint8_t satiety_ = START_SATIETY;
  bool eating_ = false;
  bool greeting_ = false;
  bool asleep_ = false;
  uint32_t lastUpdateAt_ = 0;
  uint32_t progress_ = 0;
  uint32_t reactionAt_ = 0;
};

#endif
