// Spielstand im Flash (EEPROM-Bibliothek des Board-Pakets): Evi und die Toneinstellung.
// Eine Prüfsumme erkennt einen beschädigten Stand; dann beginnt ein neues Ei.
// Flash hält etwa 100.000 Löschvorgänge, deshalb wird selten gespeichert (siehe .ino).
#pragma once
#include <EEPROM.h>
#include "pet.h"
#include "sammlung.h"

const uint32_t SPEICHER_KENNUNG = 0x45564931;   // "EVI1"
const uint16_t SPEICHER_VERSION = 3;

// Version 1 zum verlustfreien Übernehmen vorhandener Spielstände aufheben.
struct SpielstandV1 {
  uint32_t kennung;
  uint16_t version, groesse;
  Tier tier;
  bool tonAn;
  uint32_t pruefsumme;
};

// Exakte Version-2-Strukturen erhalten: neue Felder dürfen alte Einträge nicht verschieben.
struct FundstueckV2 { uint8_t art, ort; uint16_t generation; int32_t tag; };
struct SammlungV2 { uint32_t ausfluege; int32_t letzterFundTag; uint16_t anzahl; FundstueckV2 fund[SAMMLUNG_MAX]; };
struct SpielstandV2 {
  uint32_t kennung; uint16_t version, groesse; Tier tier; bool tonAn;
  SammlungV2 sammlung; uint32_t pruefsumme;
};

struct Spielstand {
  uint32_t kennung;
  uint16_t version, groesse;
  Tier tier;
  bool tonAn;
  Sammlung sammlung;
  uint32_t pruefsumme;
};

static_assert(sizeof(Fundstueck) == 12, "Fundformat muss stabil bleiben");
static_assert(sizeof(Spielstand) + 16 <= 4096, "EEPROM-Bereich ist zu klein");

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
  if (alt.kennung == SPEICHER_KENNUNG && alt.version == 2 && alt.groesse == sizeof(SpielstandV2)) {
    SpielstandV2 v2;
    uint8_t* b2 = (uint8_t*)&v2;
    for (size_t i = 0; i < sizeof(v2); i++) b2[i] = EEPROM.read(i);
    if (v2.pruefsumme != pruefsumme(b2, offsetof(SpielstandV2, pruefsumme)) || v2.sammlung.anzahl > SAMMLUNG_MAX) return false;
    if (!(v2.tier.satt >= 0 && v2.tier.satt <= 100 && v2.tier.laune >= 0 && v2.tier.laune <= 100 &&
          v2.tier.sauber >= 0 && v2.tier.sauber <= 100)) return false;
    for (int i = 0; i < v2.sammlung.anzahl; i++) {
      const FundstueckV2& f = v2.sammlung.fund[i];
      if (f.ort != FUND_GASSI || !fundAufnehmen((Fundart)f.art, FUND_GASSI, f.tag, f.generation)) { sammlung = Sammlung(); return false; }
    }
    sammlung.ausfluege = v2.sammlung.ausfluege;
    sammlung.letzterFundTag = v2.sammlung.letzterFundTag;
    v2.tier.name[20] = 0; tier = v2.tier; ton = v2.tonAn;
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
