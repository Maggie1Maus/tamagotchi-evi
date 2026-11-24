// Evis Zimmer „Gemütlich & schlicht“ mit Jahreszeiten, Wetter und Weihnachtsdeko (Wunsch der Nutzerin vom
// 24.11.2026, nur auf dem Board). Die Bilder stehen in umgebung.h (tools/make-umgebung.py aus entwuerfe/umgebung/).
//
// Jahreszeit (Baum vor dem Fenster), nach deutschem Datum:
//   Frühling ab 1. Februar, Sommer ab 1. Juni, Herbst ab 21. September, Winter ab 1. November.
// Weihnachten vom 18. November bis einschließlich 5. Januar: Der Weihnachtsbaum steht statt Wandregal
//   und Pflanze auf dem Tisch, Sterne hängen am Fensterrahmen, ein Kerzentablett steht auf dem Sideboard.
// Wetter: wechselt alle 3 Stunden, zufällig je nach Jahreszeit, aber für jeden Tag und jede Uhrzeit fest
//   (nach einem Neustart dasselbe). Schnee fällt nur im Winter. Niederschlag bewegt sich auch nachts,
//   dann mit hellen Punkten vor dem dunklen Himmel. Baum, Hügel und Fensterkreuz bleiben davor.


// Wie oft welches Wetter je Jahreszeit (Sonne, Sonne mit Wolken, Regen, bewölkt, Schnee)
const uint8_t WETTER_GEWICHT[4][5] = {
  {3, 3, 3, 2, 0},   // Frühling
  {6, 3, 2, 1, 0},   // Sommer
  {2, 3, 4, 4, 0},   // Herbst
  {2, 2, 2, 3, 4},   // Winter: nur jetzt Schnee
};
const char* const WETTER_NAME[5] = {"Sonne", "Sonne und Wolken", "Regen", "Bewölkt", "Schnee"};

Jahreszeit jahreszeitBei(ms_t t) {
  const Ortszeit o = ortszeit(t);
  const int md = o.monat * 100 + o.tag;
  if (md >= 1101 || md < 201) return JZ_WINTER;
  if (md < 601) return JZ_FRUEHLING;
  if (md < 921) return JZ_SOMMER;
  return JZ_HERBST;
}

bool weihnachtsZeit(ms_t t) {
  const Ortszeit o = ortszeit(t);
  return (o.monat == 11 && o.tag >= 18) || o.monat == 12 || (o.monat == 1 && o.tag <= 5);
}

// Das Wetter zur Zeit t: fest für jeden 3-Stunden-Abschnitt eines Tages
Wetter wetterBei(ms_t t) {
  const Ortszeit o = ortszeit(t);
  char schluessel[40];
  snprintf(schluessel, sizeof(schluessel), "%d-%d-%d:wetter:%d", o.jahr, o.monat, o.tag, o.stunde / 3);
  const uint8_t* g = WETTER_GEWICHT[jahreszeitBei(t)];
  int summe = 0;
  for (int i = 0; i < 5; i++) summe += g[i];
  int r = (int)(streuwert(schluessel) % (uint32_t)summe);
  for (int i = 0; i < 5; i++) { r -= g[i]; if (r < 0) return (Wetter)i; }
  return WT_SONNE;
}

// Deckt ein Bild an dieser Stelle (Maske)? x/y relativ zum Bild
bool bildDeckt(const Bild& b, int x, int y) {
  if (x < 0 || y < 0 || x >= b.w || y >= b.h || !b.maske) return false;
  return b.maske[y * ((b.w + 7) / 8) + x / 8] & (0x80 >> (x % 8));
}

void zeichneAnStelle(const Bild& b) { zeichneBild(b, b.ox, b.oy); }

// Ein Schneestern (5 × 5) wie im Entwurf
void schneeflocke(int x, int y, const Bild& vorne, uint16_t farbe) {
  static const int8_t P[9][2] = {{0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2}, {-2, 0}, {-1, 0}, {1, 0}, {2, 0}};
  static const int8_t D[4][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
  auto punkt = [&](int px, int py) {
    if (px >= GLAS_X && px < GLAS_X + GLAS_B && py >= GLAS_Y && py < GLAS_Y + GLAS_H && !bildDeckt(vorne, px - GLAS_X, py - GLAS_Y))
      bild.drawPixel(px, py, farbe);
  };
  for (auto& p : P) punkt(x + p[0], y + p[1]);
  for (auto& d : D) punkt(x + d[0], y + d[1]);
}

// Was durchs Fenster zu sehen ist: Himmel, Wetter, Baum und Hügel der Jahreszeit, das Fensterkreuz
void fensterZeichnen(ms_t jetzt) {
  const Himmel h = himmelBei(jetzt);
  // Himmel
  for (int y = GLAS_Y; y < GLAS_Y + GLAS_H; y++)
    for (int x = GLAS_X; x < GLAS_X + GLAS_B; x++)
      bild.drawPixel(x, y, h == HIMMEL_TAG ? 1 : h == HIMMEL_NACHT ? 0 : ((x + y) & 1));
  const Bild& vorne = *FENSTER_JAHRESZEIT[jahreszeitBei(jetzt)];
  const Wetter w = wetterBei(jetzt);
  if (h == HIMMEL_NACHT) {
    // Sterne wie bisher (night-stars in der App) und der Mond
    static const int8_t STERNE[9][4] = {{38, 28, 1, 3}, {37, 29, 3, 1}, {52, 36, 1, 1}, {44, 47, 1, 1}, {57, 29, 1, 1},
                                        {91, 29, 1, 3}, {90, 30, 3, 1}, {70, 53, 1, 1}, {89, 50, 1, 1}};
    for (auto& s : STERNE) bild.fillRect(s[0], s[1] + 23, s[2], s[3], 1);
  }
  if (h != HIMMEL_TAG) {
    bild.fillCircle(78, 65, 10, 0);
    bild.fillCircle(78, 65, 8, 1);
  } else if (w == WT_SONNE || w == WT_SONNE_WOLKEN) {
    const Bild& s = w == WT_SONNE ? WETTER_SONNE : WETTER_SONNE_WOLKE;
    zeichneBild(s, GLAS_X + (28 - s.w) / 2, GLAS_Y + 2);
  }
  if (h != HIMMEL_NACHT) {
    if (w == WT_REGEN) {
      zeichneBild(WETTER_REGENWOLKE, GLAS_X + 1, GLAS_Y + 2);   // eine Regenwolke, wie im Entwurf
    } else if (w == WT_BEWOELKT || w == WT_SCHNEE) {
      zeichneBild(WETTER_WOLKE, GLAS_X + 1, GLAS_Y + 3);
      if (w == WT_BEWOELKT) zeichneBild(WETTER_WOLKE, 68, GLAS_Y + 1);
    } else if (w == WT_SONNE_WOLKEN && h == HIMMEL_DAEMMERUNG) {
      zeichneBild(WETTER_WOLKE, GLAS_X + 1, GLAS_Y + 3);
    }
  }
  // Baum und Hügel der Jahreszeit. Nachts sind die Umrisse zum Himmel hin hell (im Mondlicht), sonst
  // verschwänden die schwarzen Äste im schwarzen Himmel.
  zeichneBild(vorne, GLAS_X, GLAS_Y);
  if (h == HIMMEL_NACHT) {
    const int zeile = (vorne.w + 7) / 8;
    for (int y = 0; y < vorne.h; y++)
      for (int x = 0; x < vorne.w; x++) {
        if (!(vorne.linien[y * zeile + x / 8] & (0x80 >> (x % 8)))) continue;
        // Himmel daneben? (das Fensterkreuz zählt nicht als Himmel)
        auto himmelDa = [&](int hx, int hy) {
          const int gx = GLAS_X + hx, gy = GLAS_Y + hy;
          if ((gx >= 61 && gx < 65) || (gy >= 81 && gy < 85)) return false;
          return !bildDeckt(vorne, hx, hy);
        };
        const bool amHimmel = himmelDa(x - 1, y) || himmelDa(x + 1, y) || himmelDa(x, y - 1) || himmelDa(x, y + 1);
        if (amHimmel && x > 0 && y > 0 && x < vorne.w - 1 && y < vorne.h - 1) bild.drawPixel(GLAS_X + x, GLAS_Y + y, 1);
      }
  }
  // Regen und Schnee fallen vor dem Himmel (nicht über Baum und Hügel)
  if (w == WT_REGEN || w == WT_SCHNEE) {
    const uint32_t t = millis();
    const uint16_t farbe = h == HIMMEL_NACHT ? 1 : 0;
    for (int i = 0; i < (w == WT_REGEN ? 10 : 11); i++) {
      const int bx = GLAS_X + 3 + (i * 37) % (GLAS_B - 6);
      if (w == WT_REGEN) {
        // schräge Striche (5 Punkte lang, 2 breit), die unter der Wolke beginnen und nach links unten fallen
        const int lauf = (int)((t / 25 + i * 23) % (GLAS_H - 16));
        for (int k = 0; k < 5; k++) {
          for (int d = 0; d < 2; d++) {
            int px = bx - lauf / 4 - k / 2 + d;
            if (px < GLAS_X) px += GLAS_B;
            const int py = GLAS_Y + 16 + lauf + k;
            if (py < GLAS_Y + GLAS_H && px < GLAS_X + GLAS_B && !bildDeckt(vorne, px - GLAS_X, py - GLAS_Y)) bild.drawPixel(px, py, farbe);
          }
        }
      } else {
        const int y = GLAS_Y + 16 + (int)((t / 90 + i * 29) % (GLAS_H - 16));
        const int x = bx + (int)lroundf(2 * sinf((t % 6000) * (6.2831853f / 3000) + i * 1.7f));
        schneeflocke(x, y, vorne, farbe);
      }
    }
  }
  // Fensterkreuz davor
  bild.fillRect(61, 47, 4, 75, 0);
  bild.fillRect(31, 81, 65, 4, 0);
}

// Das ganze Zimmer: Grund, Möbel je nach Zeit, Fenster.
// Zu Weihnachten steht der Baum auf dem Boden; Tisch, Regal und Pflanze sind dann weg.
void zimmerZeichnen(ms_t jetzt) {
  uint8_t* puffer = bild.getBuffer();
  for (int i = 0; i < 40 * 224; i++) puffer[i] = ~ZIMMER_GRUND[i];
  fensterZeichnen(jetzt);
  if (weihnachtsZeit(jetzt)) {
    zeichneAnStelle(WEIHNACHTSBAUM);
    zeichneAnStelle(KERZEN);
    zeichneAnStelle(FENSTERSTERNE);   // Sternengirlande der Nutzerin über dem Fenster
  } else {
    zeichneAnStelle(TISCH);
    zeichneAnStelle(REGAL);
    zeichneAnStelle(PFLANZE);
  }
}

// Für den Moment „aus dem Fenster schauen“
const char* fensterText(ms_t jetzt) {
  if (himmelBei(jetzt) == HIMMEL_NACHT) return "Schaut aus dem Fenster";
  switch (wetterBei(jetzt)) {
    case WT_REGEN: return "Schaut dem Regen zu";
    case WT_SCHNEE: return "Oh, es schneit!";
    case WT_BEWOELKT: return "Schaut den Wolken nach";
    default: return "Die Sonne scheint!";
  }
}
