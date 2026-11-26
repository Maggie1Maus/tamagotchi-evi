#pragma once

enum ParkPhase : uint8_t { PARK_ZU, PARK_WAHL, PARK_LAEUFT, PARK_SCHNUPPERT, PARK_HILFE, PARK_SUCHT, PARK_GRUSS, PARK_FUND, PARK_ALBUM };
enum ParkZiel : uint8_t { PARK_KIESWEG, PARK_WIESE, PARK_ZIEL_BESUCH, PARK_ZIELE };

struct ParkPunkt { int x, y; };
// Lage des Besuchs auf dem Bildschirm (siehe parkBesuchRahmen in park.ino).
struct ParkRahmen { int ursprungX, ursprungY, links, rechts, oben; };
struct ParkZustand {
  ParkPhase phase = PARK_ZU;
  ParkZiel ziel = PARK_KIESWEG;
  ParkPunkt fuesse = {62, 163};
  ParkPunkt von = fuesse, nach = fuesse;
  bool amWegpunkt = false, rechts = true;
  uint32_t seit = 0, dauer = 0;
  int fund = -1;
  uint8_t besuch = 0;   // Wer heute im Park ist (BESUCH_NAME), fest für den ganzen Besuch
  uint8_t gruss = 0;    // Welcher Begrüßungssatz gerade gilt
  uint8_t grussArt = 0; // 0 allgemeiner Satz (PARK_GRUSS_TEXT), sonst Satz passend zur Stufe (parkfreunde.ino)
  uint8_t stufe = 0;    // Wie gut Evi den Besuch kannte: 0 fremd, 1 bekannt, 2 vertraut, 3 beste Freunde
  bool passt = false;   // Besuch hat dasselbe Wesen wie Evi
  bool zweiter = false; // zweiter Satz (nach A) wird gezeigt
  uint8_t nachArt = 0, nachSatz = 0;
};

ParkZustand park;
