// Text mit der eingebauten 5x7-Schrift von Adafruit GFX (Codepage 437, daher mit Umlauten).
// Texte im Programm sind UTF-8 und werden hier umgesetzt: ä ö ü Ä Ö Ü ß · … ♥ ♪ ✦ ▲ ▼ ◀ ▶ „ “
#pragma once
#include <Adafruit_GFX.h>

// UTF-8 -> Zeichen der Schrift. Gibt die Anzahl gelesener Bytes zurück.
static int zeichenLesen(const char* s, uint8_t& z, bool& dreiPunkte) {
  const uint8_t a = (uint8_t)s[0];
  dreiPunkte = false;
  if (a < 0x80) { z = a; return 1; }
  const uint8_t b = (uint8_t)s[1];
  if (a == 0xC3) {
    switch (b) {
      case 0xA4: z = 0x84; break;  // ä
      case 0xB6: z = 0x94; break;  // ö
      case 0xBC: z = 0x81; break;  // ü
      case 0x84: z = 0x8E; break;  // Ä
      case 0x96: z = 0x99; break;  // Ö
      case 0x9C: z = 0x9A; break;  // Ü
      case 0x9F: z = 0xE1; break;  // ß
      default: z = '?';
    }
    return 2;
  }
  if (a == 0xC2) { z = b == 0xB7 ? 0xFA : '?'; return 2; }   // ·
  if (a == 0xE2) {
    const uint8_t c = (uint8_t)s[2];
    if (b == 0x80 && c == 0xA6) { dreiPunkte = true; z = '.'; }   // …
    else if (b == 0x80 && (c == 0x93 || c == 0x94)) z = '-';      // – —
    else if (b == 0x99 && c == 0xA5) z = 0x03;                     // ♥
    else if (b == 0x99 && (c == 0xAA || c == 0xAB)) z = 0x0D;      // ♪
    else if (b == 0x9C && c == 0xA6) z = 0x04;                     // ✦
    else if (b == 0x96 && c == 0xB2) z = 0x1E;                     // ▲
    else if (b == 0x96 && c == 0xBC) z = 0x1F;                     // ▼
    else if (b == 0x97 && c == 0x80) z = 0x11;                     // ◀
    else if (b == 0x96 && c == 0xB6) z = 0x10;                     // ▶
    else if (b == 0x80 && (c == 0x9E || c == 0x9C)) z = '"';      // „ “
    else z = '?';
    return 3;
  }
  z = '?';
  return a >= 0xF0 ? 4 : a >= 0xE0 ? 3 : 2;
}

// Anzahl der Zeichen auf dem Bildschirm
static int textLaenge(const char* s) {
  int n = 0;
  uint8_t z;
  bool drei;
  while (*s) { s += zeichenLesen(s, z, drei); n += drei ? 3 : 1; }
  return n;
}

static int textBreite(const char* s, int groesse = 1) { return textLaenge(s) * 6 * groesse; }

// Schreibt Text oben links ab x/y. fett = zweimal um 1 Pixel versetzt.
static void schreibe(Adafruit_GFX& g, int x, int y, const char* s, uint16_t farbe, int groesse = 1, bool fett = false) {
  g.setTextSize(groesse);
  g.setTextWrap(false);
  for (int durchgang = 0; durchgang < (fett ? 2 : 1); durchgang++) {
    int cx = x + durchgang;
    const char* p = s;
    uint8_t z;
    bool drei;
    while (*p) {
      p += zeichenLesen(p, z, drei);
      for (int i = 0; i < (drei ? 3 : 1); i++) {
        g.drawChar(cx, y, z, farbe, farbe, groesse);
        cx += 6 * groesse;
      }
    }
  }
}

// Zerlegt Text in Zeilen mit höchstens maxZeichen Zeichen (an Leerzeichen).
// Ruft zeile(text) für jede Zeile auf. Gibt die Zeilenzahl zurück.
template <typename F>
static int umbrechen(const char* s, int maxZeichen, F zeile) {
  char puffer[96];
  int zeilen = 0;
  while (*s) {
    while (*s == ' ') s++;
    if (!*s) break;
    // So viele Wörter wie passen
    const char* ende = s;
    const char* letzteLuecke = nullptr;
    int n = 0;
    while (*ende) {
      uint8_t z;
      bool drei;
      const int len = zeichenLesen(ende, z, drei);
      const int breite = drei ? 3 : 1;
      if (n + breite > maxZeichen) break;
      if (*ende == ' ') letzteLuecke = ende;
      n += breite;
      ende += len;
    }
    if (*ende && letzteLuecke) ende = letzteLuecke;
    int laenge = (int)(ende - s);
    if (laenge > (int)sizeof(puffer) - 1) laenge = sizeof(puffer) - 1;
    memcpy(puffer, s, laenge);
    puffer[laenge] = 0;
    zeile(puffer);
    zeilen++;
    s = ende;
  }
  return zeilen;
}
