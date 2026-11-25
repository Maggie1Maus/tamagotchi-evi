// Kleine, dauerhaft gespeicherte Fundstücke. Bilder liegen separat im Flash.
#pragma once
#include "pet.h"

// Bestehende IDs 0–3 bleiben unverändert, damit gespeicherte Funde ihre Bedeutung behalten.
enum Fundart : uint8_t { FUND_STEIN, FUND_BLATT, FUND_FEDER, FUND_ZAPFEN,
                         FUND_MICKY, FUND_MUSCHEL, FUND_DRACHE, FUND_TRIFORCE, FUND_FOSSIL, FUND_JUNIMO, FUNDARTEN };
enum Fundort : uint8_t { FUND_GASSI, FUND_GARTEN };
const uint8_t FUND_UNBEKANNT = 255;
const uint16_t SAMMLUNG_MAX = 256;
struct Fundstueck {
  uint8_t art, ort;
  uint16_t generation;
  int32_t tag;                       // echtes Ortsdatum, ohne Uhrzeit
  uint8_t wetter, jahreszeit, stelle, reserviert; // 255 = unbekannt (z. B. reales Wetter)
};
struct Sammlung {
  uint32_t ausfluege;
  int32_t letzterFundTag;
  uint16_t anzahl;
  Fundstueck fund[SAMMLUNG_MAX];
  int32_t gartenTag;
  uint8_t gartenVersucht; // drei Suchstellen, je einmal pro Tag
};
Sammlung sammlung = {};
const char* const FUND_NAME[] = {"Glatter Stein", "Kleines Blatt", "Weiche Feder", "Tannenzapfen",
                               "Micky-Knopf", "Arielles Muschel", "Kleiner Gartendrache", "Triforce-Anhänger", "Spiralfossil", "Junimo-Figur"};
static_assert(sizeof(FUND_NAME) / sizeof(FUND_NAME[0]) == FUNDARTEN, "Jede Fundart braucht einen Namen");

bool fundGueltig(const Fundstueck& f) {
  return f.art < FUNDARTEN && f.ort <= FUND_GARTEN && f.tag >= 20250101 && f.tag <= 20991231 &&
         (f.wetter < 5 || f.wetter == FUND_UNBEKANNT) &&
         (f.jahreszeit < 4 || f.jahreszeit == FUND_UNBEKANNT) &&
         (f.stelle < 3 || f.stelle == FUND_UNBEKANNT) &&
         (f.ort != FUND_GARTEN || (f.wetter < 5 && f.jahreszeit < 4 && f.stelle < 3));
}
bool sammlungGueltig(const Sammlung& s) {
  if (s.anzahl > SAMMLUNG_MAX || s.gartenVersucht > 7 ||
      (s.gartenTag && (s.gartenTag < 20250101 || s.gartenTag > 20991231))) return false;
  for (int i = 0; i < s.anzahl; i++) if (!fundGueltig(s.fund[i])) return false;
  return true;
}

bool fundAufnehmen(Fundart art, Fundort ort, int32_t tag, uint16_t generation,
                  uint8_t wetter = FUND_UNBEKANNT, uint8_t jahreszeit = FUND_UNBEKANNT, uint8_t stelle = FUND_UNBEKANNT) {
  Fundstueck f = {(uint8_t)art, (uint8_t)ort, generation, tag, wetter, jahreszeit, stelle, 0};
  if (sammlung.anzahl >= SAMMLUNG_MAX || !fundGueltig(f)) return false;
  sammlung.fund[sammlung.anzahl++] = f;
  if (ort == FUND_GASSI) sammlung.letzterFundTag = tag;
  return true;
}

const char* fundErinnerung(const Fundstueck& f) {
  if (f.ort == FUND_GASSI) return "Vom gemeinsamen Spaziergang mitgebracht.";
  if (f.wetter == 2) return "An einem Regentag im Garten entdeckt.";
  if (f.wetter == 4) return "Zwischen Schneeflocken im Garten entdeckt.";
  if (f.jahreszeit == 2) return "Zwischen den Herbstblättern gefunden.";
  if (f.stelle == 0) return "Unter unserem Gartenbaum gefunden.";
  if (f.stelle == 1) return "In unserem Gartenbusch entdeckt.";
  return "Am kleinen Blumenbeet gefunden.";
}

// Funde vom Gassigehen gibt es seit 25.11.2026 nicht mehr (Gassi entfernt); ältere bleiben in der Sammlung
// und zeigen weiter „Vom gemeinsamen Spaziergang mitgebracht.“. Neue Funde kommen aus dem Garten (garten.ino).

enum SideboardPhase : uint8_t { SB_ZU, SB_HIN, SB_OEFFNEN, SB_OFFEN, SB_HEIM };
struct SideboardZustand {
  SideboardPhase phase = SB_ZU;
  uint32_t seit = 0;
  int vonX = 167, x = 167;
  uint16_t auswahl = 0;
} sideboard;
