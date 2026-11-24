// Spielstand im Flash (EEPROM-Bibliothek des Board-Pakets): Evi und die Toneinstellung.
// Eine Prüfsumme erkennt einen beschädigten Stand; dann beginnt ein neues Ei.
// Flash hält etwa 100.000 Löschvorgänge, deshalb wird selten gespeichert (siehe .ino).
#pragma once
#include <EEPROM.h>
#include "pet.h"
#include "sammlung.h"

const uint32_t SPEICHER_KENNUNG = 0x45564931;   // "EVI1"
const uint16_t SPEICHER_VERSION = 2;

// Version 1 zum verlustfreien Übernehmen vorhandener Spielstände aufheben.
struct SpielstandV1 {
  uint32_t kennung;
  uint16_t version, groesse;
  Tier tier;
  bool tonAn;
  uint32_t pruefsumme;
};

struct Spielstand {
  uint32_t kennung;
  uint16_t version, groesse;
  Tier tier;
  bool tonAn;
  Sammlung sammlung;
  uint32_t pruefsumme;
};

static uint32_t pruefsumme(const uint8_t* d, size_t n) {
  uint32_t c = 0xFFFFFFFFu;
  for (size_t i = 0; i < n; i++) {
    c ^= d[i];
    for (int k = 0; k < 8; k++) c = (c >> 1) ^ (0xEDB88320u & (0u - (c & 1)));
  }
  return ~c;
}

void speicherStarten() { EEPROM.begin(sizeof(Spielstand) + 16); }

bool spielstandLaden(Tier& tier, bool& ton) {
  sammlung = Sammlung();
  SpielstandV1 alt;
  uint8_t* ab = (uint8_t*)&alt;
  for (size_t i = 0; i < sizeof(alt); i++) ab[i] = EEPROM.read(i);
  if (alt.kennung == SPEICHER_KENNUNG && alt.version == 1 && alt.groesse == sizeof(alt)) {
    if (alt.pruefsumme != pruefsumme(ab, offsetof(SpielstandV1, pruefsumme))) return false;
    if (!(alt.tier.satt >= 0 && alt.tier.satt <= 100 && alt.tier.laune >= 0 && alt.tier.laune <= 100 &&
          alt.tier.sauber >= 0 && alt.tier.sauber <= 100)) return false;
    alt.tier.name[20] = 0;
    tier = alt.tier; ton = alt.tonAn;
    return true;
  }
  Spielstand s;
  uint8_t* b = (uint8_t*)&s;
  for (size_t i = 0; i < sizeof(s); i++) b[i] = EEPROM.read(i);
  if (s.kennung != SPEICHER_KENNUNG || s.version != SPEICHER_VERSION || s.groesse != sizeof(Spielstand)) return false;
  if (s.pruefsumme != pruefsumme(b, offsetof(Spielstand, pruefsumme))) return false;
  if (!sammlungGueltig(s.sammlung)) return false;
  if (!(s.tier.satt >= 0 && s.tier.satt <= 100 && s.tier.laune >= 0 && s.tier.laune <= 100 &&
        s.tier.sauber >= 0 && s.tier.sauber <= 100)) return false;
  s.tier.name[20] = 0;
  tier = s.tier;
  ton = s.tonAn;
  sammlung = s.sammlung;
  return true;
}

void spielstandSpeichern(const Tier& tier, bool ton) {
  Spielstand s;
  memset(&s, 0, sizeof(s));
  s.kennung = SPEICHER_KENNUNG;
  s.version = SPEICHER_VERSION;
  s.groesse = sizeof(Spielstand);
  s.tier = tier;
  s.tonAn = ton;
  s.sammlung = sammlung;
  s.pruefsumme = pruefsumme((const uint8_t*)&s, offsetof(Spielstand, pruefsumme));
  const uint8_t* b = (const uint8_t*)&s;
  bool anders = false;
  for (size_t i = 0; i < sizeof(s); i++) {
    if (EEPROM.read(i) != b[i]) { EEPROM.write(i, b[i]); anders = true; }
  }
  if (anders) EEPROM.commit();
}
