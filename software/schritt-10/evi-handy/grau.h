// Graustufen-Bilder von Evi zeichnen (seit 25.11.2026). Die Bilddaten stehen in evigrau.h (4 Bit je Punkt,
// 15 = durchsichtig). Zu jedem vorhandenen Schwarz-Weiß-Bild gibt es höchstens ein Graustufen-Bild gleicher Größe;
// das Schwarz-Weiß-Bild bleibt für Trefferprüfung und Verdeckung zuständig. Bilder ohne Graustufen-Gegenstück
// (Zimmer, Symbole, Funde …) werden wie bisher schwarz-weiß gezeichnet.
#pragma once
#include "evigrau.h"

// Graustufen-Daten zu einem Bild oder nullptr
static const uint8_t* grauDaten(const Bild& b) {
  static const Bild* letzte = nullptr;
  static const uint8_t* letzteDaten = nullptr;
  if (&b == letzte) return letzteDaten;
  letzte = &b; letzteDaten = nullptr;
  for (int i = 0; i < GRAU_ANZAHL; i++) if (GRAU_TABELLE[i].alt == &b) { letzteDaten = GRAU_TABELLE[i].daten; break; }
  return letzteDaten;
}

// Halbbyte (0–15) an Stelle x, y eines Bildes der Breite w
static inline uint8_t grauStufe(const uint8_t* d, int w, int x, int y) {
  const int i = y * w + x;
  const uint8_t v = d[i >> 1];
  return (i & 1) ? (v & 15) : (v >> 4);
}

// Zeichenwert (0–255, nie 1) zu einem Halbbyte 0–14
static inline uint16_t grauWert(int stufe) { const int v = stufe * 255 / 14; return v == 1 ? 0 : v; }

// Ein Zielpunkt eines verkleinerten Bildes: Mittel über den Quellbereich [x0,x1) × [y0,y1). false = durchsichtig
// (weniger als die Hälfte der Quellpunkte deckt).
static bool grauMitteln(const uint8_t* d, int bw, int x0, int x1, int y0, int y1, uint16_t& wert) {
  int deckt = 0, alle = 0, summe = 0;
  for (int y = y0; y < y1; y++)
    for (int x = x0; x < x1; x++) {
      alle++;
      const uint8_t s = grauStufe(d, bw, x, y);
      if (s != 15) { deckt++; summe += s; }
    }
  if (deckt * 2 < alle || deckt == 0) return false;
  wert = grauWert((summe * 2 + deckt) / (2 * deckt));
  return true;
}
