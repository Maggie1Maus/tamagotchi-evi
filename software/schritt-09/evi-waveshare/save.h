#ifndef TAMAGOTCHI_SAVE_H
#define TAMAGOTCHI_SAVE_H

#include <stdint.h>
#include "pet.h"

// Spielstand im EEPROM des Nano. Jede Speicherzelle haelt etwa 100.000
// Schreibvorgaenge. Damit nicht immer dieselbe Zelle abgenutzt wird, wird
// reihum in 16 Plaetze geschrieben; der neueste gueltige Platz gilt.
// Storage braucht read(adresse) und update(adresse, wert).
template <typename Storage>
class SaveSlots {
 public:
  static const uint8_t SLOTS = 16;
  static const uint8_t SLOT_SIZE = 8;
  static const uint8_t MAGIC = 'E';
  static const uint8_t VERSION = 1;

  explicit SaveSlots(Storage &storage) : storage_(storage) {}

  // Liest den neuesten gueltigen Spielstand. false = keiner vorhanden.
  bool load(PetState &out) {
    int8_t newest = -1;
    uint8_t newestSeq = 0;
    for (uint8_t i = 0; i < SLOTS; ++i) {
      uint8_t seq;
      PetState state;
      if (!readSlot(i, seq, state)) continue;
      bool olderThanAnother = false;
      for (uint8_t j = 0; j < SLOTS && !olderThanAnother; ++j) {
        uint8_t otherSeq;
        PetState other;
        if (j == i || !readSlot(j, otherSeq, other)) continue;
        const uint8_t ahead = otherSeq - seq;
        olderThanAnother = ahead >= 1 && ahead <= 127;
      }
      if (!olderThanAnother) {
        newest = i;
        newestSeq = seq;
        out = state;
      }
    }
    if (newest < 0) {
      next_ = 0;
      seq_ = 0;
      return false;
    }
    next_ = (newest + 1) % SLOTS;
    seq_ = newestSeq + 1;
    last_ = out;
    hasLast_ = true;
    return true;
  }

  // Schreibt nur, wenn sich etwas geaendert hat. true = geschrieben.
  bool save(const PetState &state) {
    if (hasLast_ && same(state, last_)) return false;
    const uint16_t base = uint16_t(next_) * SLOT_SIZE;
    storage_.update(base + 0, MAGIC);
    storage_.update(base + 1, VERSION);
    storage_.update(base + 2, seq_);
    storage_.update(base + 3, state.satiety);
    storage_.update(base + 4, state.mood);
    storage_.update(base + 5, state.cleanliness);
    // Pruefsumme zuletzt: Faellt mittendrin der Strom aus, ist der Platz
    // ungueltig und der vorige Spielstand gilt weiter.
    storage_.update(base + 6, checksum(seq_, state));
    next_ = (next_ + 1) % SLOTS;
    ++seq_;
    last_ = state;
    hasLast_ = true;
    return true;
  }

  // Macht alle Plaetze ungueltig (fuer eine neue Evi).
  void erase() {
    for (uint8_t i = 0; i < SLOTS; ++i) storage_.update(uint16_t(i) * SLOT_SIZE, 0xFF);
    next_ = 0;
    seq_ = 0;
    hasLast_ = false;
  }

 private:
  static bool same(const PetState &a, const PetState &b) {
    return a.satiety == b.satiety && a.mood == b.mood &&
           a.cleanliness == b.cleanliness;
  }

  static uint8_t checksum(uint8_t seq, const PetState &s) {
    return uint8_t(MAGIC + VERSION + seq + s.satiety + s.mood + s.cleanliness) ^ 0x5A;
  }

  bool readSlot(uint8_t slot, uint8_t &seq, PetState &state) {
    const uint16_t base = uint16_t(slot) * SLOT_SIZE;
    if (storage_.read(base + 0) != MAGIC || storage_.read(base + 1) != VERSION) {
      return false;
    }
    seq = storage_.read(base + 2);
    state.satiety = storage_.read(base + 3);
    state.mood = storage_.read(base + 4);
    state.cleanliness = storage_.read(base + 5);
    if (state.satiety > 100 || state.mood > 100 || state.cleanliness > 100) return false;
    return storage_.read(base + 6) == checksum(seq, state);
  }

  Storage &storage_;
  uint8_t next_ = 0;
  uint8_t seq_ = 0;
  PetState last_ = {0, 0, 0};
  bool hasLast_ = false;
};

#endif
