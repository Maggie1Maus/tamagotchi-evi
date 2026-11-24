// Kleine, dauerhaft gespeicherte Fundstücke. Bilder liegen separat im Flash.
#pragma once
#include "pet.h"

// Bestehende IDs 0–3 bleiben unverändert, damit gespeicherte Funde ihre Bedeutung behalten.
enum Fundart : uint8_t { FUND_STEIN, FUND_BLATT, FUND_FEDER, FUND_ZAPFEN,
                         FUND_MICKY, FUND_MUSCHEL, FUND_DRACHE, FUND_TRIFORCE, FUND_FOSSIL, FUND_JUNIMO, FUNDARTEN };
enum Fundort : uint8_t { FUND_GASSI };
const uint16_t SAMMLUNG_MAX = 256;
struct Fundstueck {
  uint8_t art, ort;
  uint16_t generation;
  int32_t tag;                       // echtes Ortsdatum, ohne Uhrzeit
};
struct Sammlung {
  uint32_t ausfluege;
  int32_t letzterFundTag;
  uint16_t anzahl;
  Fundstueck fund[SAMMLUNG_MAX];
};
Sammlung sammlung = {};
const char* const FUND_NAME[] = {"Glatter Stein", "Kleines Blatt", "Weiche Feder", "Tannenzapfen",
                               "Micky-Knopf", "Arielles Muschel", "Kleiner Gartendrache", "Triforce-Anhänger", "Spiralfossil", "Junimo-Figur"};
static_assert(sizeof(FUND_NAME) / sizeof(FUND_NAME[0]) == FUNDARTEN, "Jede Fundart braucht einen Namen");

bool sammlungGueltig(const Sammlung& s) {
  if (s.anzahl > SAMMLUNG_MAX) return false;
  for (int i = 0; i < s.anzahl; i++)
    if (s.fund[i].art >= FUNDARTEN || s.fund[i].ort != FUND_GASSI || s.fund[i].tag < 20250101 || s.fund[i].tag > 20991231) return false;
  return true;
}

bool fundAufnehmen(Fundart art, Fundort ort, int32_t tag, uint16_t generation) {
  if (art >= FUNDARTEN || sammlung.anzahl >= SAMMLUNG_MAX || ort != FUND_GASSI || tag < 20250101 || tag > 20991231) return false;
  sammlung.fund[sammlung.anzahl++] = {(uint8_t)art, (uint8_t)ort, generation, tag};
  sammlung.letzterFundTag = tag;
  return true;
}

// Ab 100 Schritten: beim ersten Fund sicher, danach 40 % je Spaziergang, höchstens ein Fund
// pro echtem Tag. Eine volle Sammlung wird niemals überschrieben. Park-Regeln bleiben offen.
int fundBeimGassi(uint32_t schritte, ms_t jetzt, uint16_t generation) {
  const int32_t tag = tagKennung(jetzt);
  if (schritte < 100 || sammlung.anzahl >= SAMMLUNG_MAX || sammlung.letzterFundTag == tag) return -1;
  sammlung.ausfluege++;
  char schluessel[64];
  snprintf(schluessel, sizeof(schluessel), "%ld:fund:%u:%lu", (long)tag, generation, (unsigned long)sammlung.ausfluege);
  const uint32_t zufall = streuwert(schluessel);
  if (sammlung.anzahl && zufall % 100 >= 40) return -1;
  const Fundart art = (Fundart)((zufall / 100) % FUNDARTEN);
  return fundAufnehmen(art, FUND_GASSI, tag, generation) ? (int)art : -1;
}

enum SideboardPhase : uint8_t { SB_ZU, SB_HIN, SB_OEFFNEN, SB_OFFEN, SB_HEIM };
struct SideboardZustand {
  SideboardPhase phase = SB_ZU;
  uint32_t seit = 0;
  int vonX = 167, x = 167;
  uint16_t auswahl = 0;
} sideboard;
