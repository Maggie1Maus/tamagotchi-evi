#pragma once

enum ParkPhase : uint8_t { PARK_ZU, PARK_WAHL, PARK_LAEUFT, PARK_SCHNUPPERT, PARK_HILFE, PARK_SUCHT, PARK_GRUSS, PARK_FUND };
enum ParkZiel : uint8_t { PARK_KIESWEG, PARK_WIESE, PARK_ZIEL_LOTTI, PARK_ZIELE };

struct ParkPunkt { int x, y; };
struct ParkZustand {
  ParkPhase phase = PARK_ZU;
  ParkZiel ziel = PARK_KIESWEG;
  ParkPunkt fuesse = {62, 163};
  ParkPunkt von = fuesse, nach = fuesse;
  bool amWegpunkt = false, rechts = true;
  uint32_t seit = 0, dauer = 0;
  int fund = -1;
};

ParkZustand park;
