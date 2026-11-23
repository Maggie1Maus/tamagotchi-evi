// Zustand für die Spiele (Etappe 2) und Evis eigene Momente (Etappe 4).
// Nur Typen und Variablen: Die Arduino IDE setzt Funktionsankündigungen vor die erste Funktion,
// darum müssen Typen, die in Funktionsköpfen vorkommen, in einer Kopfdatei stehen.
// Die Abläufe selbst stehen in spiele.ino und momente.ino.
#pragma once
#include <stdint.h>
#include "zeit.h"

// ---------- Spiele (GAMES in app.js) ----------
enum SpielArt : uint8_t { SPIEL_BALL, SPIEL_RATEN, SPIEL_MELODIE };
// Tasten im Spiel wie in der App: links, Mitte, rechts; dazu „abbrechen“ (nur am Gerät)
enum SpielTaste : uint8_t { ST_LINKS, ST_MITTE, ST_RECHTS, ST_ABBRUCH };
// Was später passieren soll (statt setTimeout in der App)
enum SpielEreignis : uint8_t { SE_NICHTS, SE_ENDE, SE_BALL_WEITER, SE_RATEN_ZURUECK, SE_TON, SE_DU_BIST_DRAN, SE_MELODIE_WEITER };

struct SpielPlan { SpielEreignis was; int8_t wert; uint32_t um; };

struct SpielZustand {
  bool aktiv = false;
  SpielArt art = SPIEL_BALL;
  int runde = 0, treffer = 0;
  char notiz[64] = "";
  // Ball
  float x = 0, tempo = 0, pause = 0;
  int richtung = 1;
  uint32_t letzteZeit = 0;
  // Raten und Melodie
  bool beschaeftigt = false;
  int eviX = 0;                // Evi springt beim Raten 96 Punkte zur Seite
  uint8_t folge[8];
  int folgeLaenge = 0, schritt = 0;
  int leuchtet = -1;           // Feld 0–2, das gerade schwarz ist
  uint32_t leuchtetBis = 0;
  SpielPlan plan[12];
  int planAnzahl = 0;
};
SpielZustand spiel;

// ---------- Evis Momente (moments.js, SCRIPTS in app.js) ----------
const uint32_t RUHE_TAKTE = 120;   // QUIET_TICKS: etwa 15 s ohne neuen Moment nach Begrüßung und Pflege
enum MomentArt : uint8_t { M_GEHEN, M_FENSTER, M_DENKEN, M_SUMMEN, M_GAEHNEN, M_DREHEN, M_FALTER, M_VOGEL, M_STERN, M_HEIM, M_KEINER = 255 };
// Bilder der Momente: Nummer in MOMENT_BILD (momente.h)
enum MomentBild : int8_t { MB_KEINS = -1, MB_LAUFEN, MB_FENSTER, MB_DENKEN, MB_SINGEN, MB_GAEHNEN, MB_DREHEN, MB_HUEPFEN, MB_STAUNEN,
                           MB_GLUECKLICH = 100 };
// Gedanken: Nummer in GEDANKE_BILD (momente.h)
enum Gedanke : int8_t { GD_KEINER = -1, GD_HERZ, GD_KEKS, GD_BALL, GD_NOTE, GD_FUTTER, GD_PUTZEN, GD_SONNE, GD_MOND };
// Einzelne Schritte eines Moments (ein Moment ist eine Liste davon)
enum AktArt : uint8_t { A_MELDUNG, A_SAGEN, A_TON, A_WARTEN, A_GEHEN, A_HEIM, A_HUEPFEN, A_BILD, A_FLIP, A_DENKEN, A_SUMMEN, A_DREHEN,
                        A_FALTER_REIN, A_FALTER_KREISEN, A_FALTER_LANDEN, A_FALTER_SITZEN, A_FALTER_WEG,
                        A_VOGEL_REIN, A_VOGEL_SINGEN, A_VOGEL_WEG, A_STERN };
struct Akt { AktArt art; int16_t a, b; const char* text; };

struct MomentZustand {
  MomentArt art = M_KEINER;
  Akt plan[24];
  int anzahl = 0, pos = 0, zaehler = 0, warten = 0;
  int unter = 0;               // Hilfszähler (Hüpfen, Verfolgen)
  bool unterFertig = false;
  MomentBild bild = MB_KEINS, bildVorher = MB_KEINS;
  Gedanke gedanke = GD_KEINER;
  bool summt = false;
  ms_t naechster = 0;          // nächster kleiner Moment
  MomentArt letzter = M_KEINER;
  uint32_t ruheBisTakt = 0;    // etwa 15 s Ruhe nach Begrüßung und Pflege
  ms_t letzterBesonderer = 0;  // nur im Arbeitsspeicher (der Spielstand bleibt unverändert)
  int32_t launeTag = -1;       // Tag, an dem die Tageslaune schon gesagt wurde
  uint32_t letzterGruss = 0, launeSagenUm = 0;
  int8_t launeSagen = -1;
  MomentBild huepfVorher = MB_KEINS;
  float zielX = 0, zielY = 0;  // Ziel des Schmetterlings
  bool gehtNoch = false, fliegtNoch = false;
  bool grussGeplant = false;   // Begrüßung kurz nach dem Einschalten
  uint32_t grussUm = 0;
  ms_t grussSeit = 0;
};
MomentZustand moment;

// Wo Evi ist: x = Abstand vom Teppich, flip = gespiegelt, lift = angehoben (Schritt, Hüpfer)
struct { int x = 0, lift = 0; bool flip = false; } geher;
// Besuch: Schmetterling (frei im Zimmer), Vogel und Sternschnuppe (im Fenster)
// x/y: wo er hinfliegt; px/py: wo er gerade gezeichnet wird (mit Flattern); bild: Flügel bzw. Haltung
struct Besucher { bool sichtbar = false; float x = 0, y = 0, px = 0, py = 0; int t = 0; uint8_t bild = 0; };
Besucher falter, vogel, stern;
// Ein Gang durchs Zimmer (walkTo in app.js); es läuft immer nur einer
struct { int ziel = 0, schritte = 0; bool laeuft = false; MomentBild vorher = MB_KEINS; } gang;
// Wo Evis Bild zuletzt gezeichnet wurde (für den Schmetterling auf ihrem Kopf)
int eviOben = 100, eviHoehe = 80;
// Seit wann der Bildschirm dunkel ist (für die Begrüßung)
ms_t dunkelSeit = 0;
