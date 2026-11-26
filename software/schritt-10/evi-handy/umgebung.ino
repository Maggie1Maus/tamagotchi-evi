// Evis Zimmer „Gemütlich & schlicht“ mit Jahreszeiten, Wetter und Weihnachtsdeko (Wunsch der Nutzerin vom
// 24.11.2026, nur auf dem Board). Zimmer, Fensterausblick und Wetter zeichnet einrichtung.ino.
//
// Jahreszeit (Baum vor dem Fenster), nach deutschem Datum:
//   Frühling ab 1. Februar, Sommer ab 1. Juni, Herbst ab 21. September, Winter ab 1. November.
// Weihnachten vom 18. November bis einschließlich 5. Januar. Der Weihnachtsbaum steht auf dem Boden,
//   Sterne hängen am Fensterrahmen und ein Kerzentablett steht auf dem Sideboard.
// Wetter: wechselt alle 3 Stunden, zufällig je nach Jahreszeit, aber für jeden Tag und jede Uhrzeit fest
//   (nach einem Neustart dasselbe). Schnee fällt nur im Winter. Niederschlag bewegt sich auch nachts,
//   dann mit hellen Punkten vor dem dunklen Himmel. Der Fensterrahmen begrenzt den Ausblick.


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
  if (dorfHaus) return false;      // im Haus eines Freundes keine Weihnachtsdeko (dorf.ino)
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

// Das ganze Zimmer: Grund, Möbel je nach Zeit, Fenster.
// Zu Weihnachten steht der Baum auf dem Boden; Tisch, Regal und Pflanze sind dann weg.
void zimmerZeichnen(ms_t jetzt) { zimmerNeuZeichnen(jetzt); }

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
