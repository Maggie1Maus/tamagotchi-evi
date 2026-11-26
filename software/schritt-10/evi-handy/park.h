#pragma once

enum ParkPhase : uint8_t { PARK_ZU, PARK_WAHL, PARK_LAEUFT, PARK_SCHNUPPERT, PARK_HILFE, PARK_SUCHT, PARK_GRUSS, PARK_FUND, PARK_ALBUM, PARK_STAMM, PARK_NAME, PARK_DORF, PARK_BESUCHT, PARK_LIEBE };
enum ParkZiel : uint8_t { PARK_KIESWEG, PARK_WIESE, PARK_ZIEL_DORF, PARK_ZIEL_BESUCH, PARK_ZIELE };   // Besuch zuletzt: „zurück“ vom Kiesweg landet bei Lotti wie bisher

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
  uint8_t zweiter = 0;  // 0 Begrüßung, 1 zweiter Satz (nach A), 2 Geschenk (nach A)
  uint8_t geschenk = 0, geschenkZahl = 0;   // Geschenk des Besuchs: 0 keins, sonst 1 + Wesen (parkfreunde.ino)
  uint8_t nachArt = 0, nachSatz = 0;
  uint32_t meldungBis = 0;   // Dorf: bis dahin steht eine Meldung („nicht zu Hause“)
  uint8_t meldung = 0;       // Dorf: welche Meldung: 0 keine, 1 nicht zu Hause, 2 kenne ich nicht
  uint8_t meldungNr = 0;
  uint8_t liebeArt = 0, liebeKandidat = 0, liebeWahl = 0;   // Frage „Partner?“ (0 Freundschaft, 1 Klopfen) oder „Begraben?“ (2)
  ParkPhase liebeZurueck = PARK_ZU;                          // wohin es nach der Antwort zurückgeht
};

ParkZustand park;

// Im Haus eines Freundes (Dorf): das Zimmer ohne Weihnachtsdeko, solange die Szene gezeichnet wird
bool dorfHaus = false;
