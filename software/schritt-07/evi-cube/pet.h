#ifndef TAMAGOTCHI_PET_H
#define TAMAGOTCHI_PET_H

#include <stdint.h>

enum class Face : uint8_t { Happy, Hungry, Sad, Dirty, Sleeping };
enum class Reaction : uint8_t { None, Eating, Playing, Cleaning };

// Die drei Werte, die gespeichert werden.
struct PetState {
  uint8_t satiety;
  uint8_t mood;
  uint8_t cleanliness;
};

// Spielregeln fuer Evi, unabhaengig von Display, Taster und Speicher.
// Die Werte entsprechen der erwachsenen Evi in der Handy-App
// (handy-app/dist/pet.js): Pflege etwa alle 3-4 Stunden.
class Pet {
 public:
  static const uint8_t NEEDS = 3;  // Saettigung, Stimmung, Sauberkeit
  static const uint8_t CARE_AT = 30;  // ab hier zeigt Evi das Beduerfnis
  static const uint32_t REACTION_MS = 1500;

  static PetState startState() { return PetState{80, 80, 100}; }

  Pet() { restore(startState(), 0); }

  // Fuettern: +40 satt, +3 Laune, -2 sauber.
  void feed(uint32_t now) { act(now, 40, 3, -2, Reaction::Eating); }
  // Spielen: +30 Laune, -8 satt, -5 sauber.
  void play(uint32_t now) { act(now, -8, 30, -5, Reaction::Playing); }
  // Putzen: +60 sauber, +3 Laune.
  void clean(uint32_t now) { act(now, 0, 3, 60, Reaction::Cleaning); }

  // Vorher update(now) aufrufen, damit die Zeit davor im alten Tempo zaehlt.
  void setAsleep(bool asleep) { asleep_ = asleep; }

  void update(uint32_t now) {
    uint32_t elapsed = uint32_t(now - lastUpdateAt_);
    lastUpdateAt_ = now;
    // In Stuecken von hoechstens einer Stunde, damit nichts ueberlaeuft.
    while (elapsed > 0) {
      const uint32_t chunk = elapsed > 3600000UL ? 3600000UL : elapsed;
      elapsed -= chunk;
      for (uint8_t i = 0; i < NEEDS; ++i) decay(i, chunk);
    }
    if (uint32_t(now - reactionAt_) >= REACTION_MS) reaction_ = Reaction::None;
  }

  PetState state() const { return PetState{value_[0], value_[1], value_[2]}; }

  // Setzt gespeicherte Werte. Die Zeit ohne Strom zaehlt als Pause.
  void restore(const PetState &state, uint32_t now) {
    value_[0] = clamp(state.satiety);
    value_[1] = clamp(state.mood);
    value_[2] = clamp(state.cleanliness);
    for (uint8_t i = 0; i < NEEDS; ++i) progress_[i] = 0;
    lastUpdateAt_ = now;
    reaction_ = Reaction::None;
  }

  uint8_t satiety() const { return value_[0]; }
  uint8_t mood() const { return value_[1]; }
  uint8_t cleanliness() const { return value_[2]; }
  uint8_t lowest() const {
    uint8_t low = value_[0];
    for (uint8_t i = 1; i < NEEDS; ++i) if (value_[i] < low) low = value_[i];
    return low;
  }
  bool isHungry() const { return value_[0] <= CARE_AT; }
  bool isSad() const { return value_[1] <= CARE_AT; }
  bool isDirty() const { return value_[2] <= CARE_AT; }
  bool needsCare() const { return lowest() <= CARE_AT; }
  bool isAsleep() const { return asleep_; }
  Reaction reaction() const { return reaction_; }

  // Welches Bild passt gerade? Wie in der Handy-App: der niedrigste Wert
  // entscheidet, wenn er hoechstens 30 ist.
  Face face() const {
    if (asleep_ && reaction_ == Reaction::None) return Face::Sleeping;
    if (reaction_ != Reaction::None || !needsCare()) return Face::Happy;
    const uint8_t low = lowest();
    if (value_[0] == low) return Face::Hungry;
    if (value_[1] == low) return Face::Sad;
    return Face::Dirty;
  }

 private:
  // Punkte pro Stunde wach: satt 20, Laune 15, sauber 12.
  static uint32_t msPerPoint(uint8_t need) {
    return need == 0 ? 180000UL : need == 1 ? 240000UL : 300000UL;
  }
  // Im Schlaf (Licht aus): satt und sauber 15 %, Laune 10 % des Tempos.
  static uint8_t sleepPercent(uint8_t need) { return need == 1 ? 10 : 15; }

  static uint8_t clamp(int16_t v) { return v < 0 ? 0 : v > 100 ? 100 : v; }

  void decay(uint8_t need, uint32_t chunk) {
    // Fortschritt in Hundertsteln, damit auch das Schlaftempo genau bleibt.
    const uint32_t perPoint = msPerPoint(need) * 100UL;
    progress_[need] += chunk * (asleep_ ? sleepPercent(need) : 100U);
    const uint32_t steps = progress_[need] / perPoint;
    progress_[need] -= steps * perPoint;
    value_[need] = steps >= value_[need] ? 0 : value_[need] - steps;
    // Leer bleibt leer; angesammelter Rest zaehlt nach der Pflege nicht.
    if (value_[need] == 0) progress_[need] = 0;
  }

  void act(uint32_t now, int8_t satiety, int8_t mood, int8_t cleanliness,
           Reaction reaction) {
    update(now);
    value_[0] = clamp(int16_t(value_[0]) + satiety);
    value_[1] = clamp(int16_t(value_[1]) + mood);
    value_[2] = clamp(int16_t(value_[2]) + cleanliness);
    reaction_ = reaction;
    reactionAt_ = now;
  }

  uint8_t value_[NEEDS];
  uint32_t progress_[NEEDS];
  bool asleep_ = false;
  Reaction reaction_ = Reaction::None;
  uint32_t lastUpdateAt_ = 0;
  uint32_t reactionAt_ = 0;
};

#endif
