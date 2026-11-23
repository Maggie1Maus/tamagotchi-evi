// Schritt 10, Etappe 4: Evis eigene Momente, Begrüßung, Tageslaune und Besuch
// (moments.js und der Abschnitt „Evi's own moments“ in app.js).
//
// Nur die erwachsene Evi tut selbst etwas (Wunsch der Nutzerin vom 23.11.2026). Momente ändern
// nie die Spielwerte. Ein Moment ist eine Liste von Schritten (Akt); jeder Takt (120 ms) führt
// den aktuellen Schritt weiter, so wie die Generatoren in app.js mit „yield“.
//
// Anders als in der App, wegen des Geräts:
//  - Kleine Momente gibt es nur bei hellem Bildschirm. Besuch (Schmetterling, Vogel, Sternschnuppe)
//    schaltet den Bildschirm ein, mit Ton. Während eines Moments bleibt der Bildschirm an.
//  - Begrüßt wird nach dem Einschalten und wenn man den Bildschirm nach mindestens 3 Minuten weckt.
//  - Wann zuletzt Besuch da war und ob die Tageslaune schon gesagt wurde, steht nur im Arbeitsspeicher.

const int SCHRITT_WEITE = 2;                  // STEP: Punkte pro Takt beim Gehen
const ms_t MOMENT_MIN = 5 * MINUTE, MOMENT_MAX = 10 * MINUTE;
const ms_t GRUSS_NACH = 3 * MINUTE, LANGE_WEG = 4 * STUNDE;

// ---------- Tageslaune (MOMENTS.tempers) ----------
// Gewichte der kleinen Momente: gehen, fenster, denken, summen, gähnen, drehen
// Gewichte der Gedanken: keks, ball, herz, note, futter
struct LauneInfo { const char* text; const char* zeile; const Bild* symbol; uint8_t art[6]; uint8_t gedanke[5]; };
const LauneInfo LAUNEN[4] = {
  {"Heute bin ich verspielt!", "Heute bin ich verspielt", &SYM_W_BALL, {2, 1, 1, 1, 1, 3}, {1, 3, 1, 1, 1}},
  {"Heute bin ich verschmust!", "Heute bin ich verschmust", &SYM_W_HERZ, {1, 1, 2, 1, 1, 1}, {1, 1, 4, 1, 1}},
  {"Heute bin ich verfressen!", "Heute bin ich verfressen", &SYM_W_KEKS, {1, 1, 3, 1, 1, 1}, {3, 1, 1, 1, 3}},
  {"Heute bin ich verträumt!", "Heute bin ich verträumt", &SYM_W_NOTE, {1, 3, 2, 2, 1, 1}, {1, 1, 1, 2, 1}},
};
const Gedanke GEDANKEN_WAHL[5] = {GD_KEKS, GD_BALL, GD_HERZ, GD_NOTE, GD_FUTTER};
const int LAUNE_VERSCHMUST = 1;

const char* gedankenText(int g) {
  switch (g) {
    case GD_KEKS: return "Denkt an Kekse";
    case GD_BALL: return "Denkt ans Spielen";
    case GD_HERZ: return "Denkt an dich ♥";
    case GD_NOTE: return "Denkt an ein Lied";
    case GD_FUTTER: return "Denkt ans Essen";
    case GD_PUTZEN: return "Denkt ans Baden";
    default: return "Schaut aus dem Fenster";
  }
}

// Evis Tageslaune: 0 verspielt, 1 verschmust, 2 verfressen, 3 verträumt (temperOn)
int launeHeute(ms_t t) {
  const Ortszeit o = ortszeit(t);
  char s[64];
  snprintf(s, sizeof(s), "%lld:%d-%d-%d:temper", (long long)pet.geboren, o.jahr, o.monat, o.tag);
  return (int)(streuwert(s) % 4);
}

// Für die Zeile unten: die Laune ohne Ausrufezeichen
void launeZeile(ms_t jetzt, char* aus, size_t n) { snprintf(aus, n, "%s", LAUNEN[launeHeute(jetzt)].zeile); }

// ---------- Wann was passiert ----------
ms_t naechsterAbstand() { return MOMENT_MIN + (ms_t)random(0, (long)(MOMENT_MAX - MOMENT_MIN)); }

bool ruhend(Stimmung s) { return s == S_TOT || s == S_EI || s == S_SCHLAEFT || s == S_KRANK || s == S_BAUCH; }

// Der Besuch des Tages (specialsOn): ein oder zwei, für jede Evi und jeden Tag fest.
int besuchHeute(ms_t t, MomentArt art[2], ms_t um[2]) {
  const Ortszeit o = ortszeit(t);
  char schluessel[64], text[72];
  snprintf(schluessel, sizeof(schluessel), "%lld:%d-%d-%d:special", (long long)pet.geboren, o.jahr, o.monat, o.tag);
  // Zwei am Tag: einer vor und einer nach der Mitte (15 Uhr), sonst einer zwischen 9 und 21 Uhr.
  int spannen[2][2] = {{9, 21}, {0, 0}};
  int n = 1;
  if (streuwert(schluessel) & 1) { spannen[0][1] = 15; spannen[1][0] = 15; spannen[1][1] = 21; n = 2; }
  for (int i = 0; i < n; i++) {
    snprintf(text, sizeof(text), "%s:%d", schluessel, i);
    const uint32_t h = streuwert(text);
    um[i] = ausOrtszeit(o.jahr, o.monat, o.tag, spannen[i][0], (int)(h % (uint32_t)((spannen[i][1] - spannen[i][0]) * 60)));
    art[i] = ((h >> 12) % 2) == 0 ? M_FALTER : M_VOGEL;
  }
  return n;
}

// Besuch, der fällig ist und noch nicht gezeigt wurde (dueSpecial). Er wartet bis 22 Uhr.
bool besuchFaellig(ms_t t, MomentArt& art) {
  if (stundeVon(t) >= 22) return false;
  MomentArt arten[2];
  ms_t um[2];
  const int n = besuchHeute(t, arten, um);
  bool faellig = false;
  for (int i = 0; i < n; i++) {
    if (um[i] <= t && um[i] > moment.letzterBesonderer) { art = arten[i]; faellig = true; }
  }
  return faellig;
}

// Der kleine Moment, der gerade passt (chooseMoment). Wer Hunger hat, traurig oder schmutzig ist,
// denkt nur an das, was fehlt.
MomentArt momentWaehlen(Stimmung s, bool istMuede, int laune, Gedanke& gedanke) {
  if (s == S_HUNGRIG) { gedanke = GD_FUTTER; return M_DENKEN; }
  if (s == S_TRAURIG) { gedanke = GD_BALL; return M_DENKEN; }
  if (s == S_SCHMUTZIG) { gedanke = GD_PUTZEN; return M_DENKEN; }
  if (s != S_GLUECKLICH) return M_KEINER;
  if (istMuede) return M_GAEHNEN;
  const LauneInfo& l = LAUNEN[laune];
  int summe = 0;
  for (int k = 0; k < 6; k++) if (k != moment.letzter) summe += l.art[k];
  int r = random(0, summe);
  MomentArt art = M_GEHEN;
  for (int k = 0; k < 6; k++) {
    if (k == moment.letzter) continue;
    r -= l.art[k];
    if (r < 0) { art = (MomentArt)k; break; }
  }
  if (art == M_DENKEN) {
    summe = 0;
    for (int k = 0; k < 5; k++) summe += l.gedanke[k];
    r = random(0, summe);
    gedanke = GEDANKEN_WAHL[0];
    for (int k = 0; k < 5; k++) { r -= l.gedanke[k]; if (r < 0) { gedanke = GEDANKEN_WAHL[k]; break; } }
  }
  return art;
}

bool momentErlaubt() {
  return !spiel.aktiv && !ritual.aktiv && !menueOffen && !evi.schluepfen &&
         evi.takt >= evi.reaktionBisTakt && evi.takt >= moment.ruheBisTakt;
}

// ---------- Ablauf ----------
void akt(AktArt art, int a, int b, const char* text) {
  if (moment.anzahl >= 24) return;
  moment.plan[moment.anzahl].art = art;
  moment.plan[moment.anzahl].a = (int16_t)a;
  moment.plan[moment.anzahl].b = (int16_t)b;
  moment.plan[moment.anzahl].text = text;
  moment.anzahl++;
}

void momentStarten(MomentArt art, Gedanke gedanke) {
  moment.art = art;
  moment.anzahl = moment.pos = moment.zaehler = moment.warten = moment.unter = 0;
  switch (art) {
    case M_GEHEN: {
      const int seite = random(0, 2) ? 1 : -1;
      akt(A_MELDUNG, 9000, 0, "Ein kleiner Spaziergang …");
      akt(A_GEHEN, seite * (int)random(30, (seite < 0 ? 66 : 56) + 1), 0, nullptr);   // bleibt im Zimmer
      akt(A_WARTEN, 10, 0, nullptr);
      akt(A_HUEPFEN, 1, 5, nullptr);
      akt(A_WARTEN, 12, 0, nullptr);
      akt(A_HEIM, 0, 0, nullptr);
      break;
    }
    case M_FENSTER:
      akt(A_MELDUNG, 12000, 0, "Schaut aus dem Fenster");
      akt(A_GEHEN, -66, 0, nullptr);
      akt(A_FLIP, 0, 0, nullptr);
      akt(A_BILD, MB_FENSTER, 0, nullptr);
      akt(A_WARTEN, 6, 0, nullptr);
      akt(A_DENKEN, himmelBei(jetztMs()) == HIMMEL_TAG ? GD_SONNE : GD_MOND, 0, nullptr);
      akt(A_WARTEN, 45, 0, nullptr);
      akt(A_DENKEN, GD_KEINER, 0, nullptr);
      akt(A_BILD, MB_KEINS, 0, nullptr);
      akt(A_HEIM, 0, 0, nullptr);
      break;
    case M_DENKEN:
      akt(A_MELDUNG, 6000, 0, gedankenText(gedanke));
      akt(A_BILD, MB_DENKEN, 0, nullptr);
      akt(A_DENKEN, gedanke, 0, nullptr);
      akt(A_WARTEN, 45, 0, nullptr);
      akt(A_DENKEN, GD_KEINER, 0, nullptr);
      akt(A_BILD, MB_KEINS, 0, nullptr);
      break;
    case M_SUMMEN:
      akt(A_MELDUNG, 6000, 0, "Summt ein Lied ♪");
      akt(A_BILD, MB_SINGEN, 0, nullptr);
      akt(A_SUMMEN, 1, 0, nullptr);
      akt(A_WARTEN, 45, 0, nullptr);
      akt(A_SUMMEN, 0, 0, nullptr);
      akt(A_BILD, MB_KEINS, 0, nullptr);
      break;
    case M_GAEHNEN:
      akt(A_MELDUNG, 3000, 0, "Gäähn …");
      akt(A_BILD, MB_GAEHNEN, 0, nullptr);
      akt(A_WARTEN, 22, 0, nullptr);
      akt(A_BILD, MB_KEINS, 0, nullptr);
      break;
    case M_DREHEN:
      akt(A_MELDUNG, 4000, 0, "Dreht sich im Kreis");
      akt(A_BILD, MB_DREHEN, 0, nullptr);
      akt(A_DREHEN, 10, 3, nullptr);
      akt(A_FLIP, 0, 0, nullptr);
      akt(A_BILD, MB_KEINS, 0, nullptr);
      akt(A_SAGEN, 2000, 0, "Hihi!");
      akt(A_WARTEN, 16, 0, nullptr);
      break;
    case M_FALTER:
      // Ein Schmetterling flattert durchs Fenster herein, kreist um Evi, setzt sich auf ihren Kopf
      // und fliegt nach rechts davon. Sie läuft ihm ein Stück nach und hüpft.
      akt(A_TON, 0, 0, nullptr);
      akt(A_MELDUNG, 30000, 0, "Oh, ein Schmetterling!");
      akt(A_FALTER_REIN, 0, 0, nullptr);
      akt(A_BILD, MB_STAUNEN, 0, nullptr);
      akt(A_FALTER_KREISEN, 0, 0, nullptr);
      akt(A_FALTER_LANDEN, 0, 0, nullptr);
      akt(A_FALTER_SITZEN, 0, 0, nullptr);
      akt(A_SAGEN, 2600, 0, "Hihi, das kitzelt!");
      akt(A_WARTEN, 10, 0, nullptr);
      akt(A_BILD, MB_GLUECKLICH, 0, nullptr);
      akt(A_FALTER_WEG, 0, 0, nullptr);
      akt(A_HUEPFEN, 2, 9, nullptr);
      akt(A_BILD, MB_KEINS, 0, nullptr);
      akt(A_MELDUNG, 3000, 0, "Tschüss, Schmetterling!");
      akt(A_WARTEN, 14, 0, nullptr);
      akt(A_HEIM, 0, 0, nullptr);
      break;
    case M_VOGEL:
      // Ein Vogel landet auf dem Fensterkreuz. Evi kommt näher, staunt und hüpft, wenn er singt.
      akt(A_TON, 1, 0, nullptr);
      akt(A_MELDUNG, 30000, 0, "Ein Vogel am Fenster!");
      akt(A_VOGEL_REIN, 0, 0, nullptr);
      akt(A_BILD, MB_STAUNEN, 0, nullptr);
      akt(A_WARTEN, 20, 0, nullptr);
      akt(A_TON, 1, 0, nullptr);
      akt(A_MELDUNG, 4000, 0, "Piep, piep!");
      akt(A_VOGEL_SINGEN, 0, 0, nullptr);
      akt(A_HUEPFEN, 2, 8, nullptr);
      akt(A_WARTEN, 16, 0, nullptr);
      akt(A_VOGEL_WEG, 0, 0, nullptr);
      akt(A_BILD, MB_KEINS, 0, nullptr);
      akt(A_SAGEN, 2600, 0, "Tschüss, Vögelchen!");
      akt(A_WARTEN, 20, 0, nullptr);
      akt(A_HEIM, 0, 0, nullptr);
      break;
    case M_STERN:
      // Im Dunkeln: eine Sternschnuppe. Evi schaut zu, wünscht sich etwas und sieht noch eine.
      akt(A_TON, 0, 0, nullptr);
      akt(A_MELDUNG, 30000, 0, "Eine Sternschnuppe!");
      akt(A_GEHEN, -30, 0, nullptr);
      akt(A_FLIP, 0, 0, nullptr);
      akt(A_BILD, MB_STAUNEN, 0, nullptr);
      akt(A_WARTEN, 6, 0, nullptr);
      akt(A_STERN, 100, 8, nullptr);
      akt(A_MELDUNG, 5000, 0, "Ich wünsch mir was …");
      akt(A_BILD, MB_DENKEN, 0, nullptr);
      akt(A_DENKEN, GD_HERZ, 0, nullptr);
      akt(A_WARTEN, 40, 0, nullptr);
      akt(A_DENKEN, GD_KEINER, 0, nullptr);
      akt(A_BILD, MB_STAUNEN, 0, nullptr);
      akt(A_STERN, 100, 44, nullptr);     // diese fällt hinter die Hügel
      akt(A_SAGEN, 2400, 0, "Noch eine!");
      akt(A_HUEPFEN, 1, 8, nullptr);
      akt(A_WARTEN, 14, 0, nullptr);
      akt(A_BILD, MB_KEINS, 0, nullptr);
      akt(A_HEIM, 0, 0, nullptr);
      break;
    case M_HEIM:
      akt(A_HEIM, 0, 0, nullptr);
      break;
    default:
      moment.art = M_KEINER;
      break;
  }
}

// Räumt alles weg, was ein Moment zeigt: Gedanke, Noten, Besuch, geliehenes Bild (clearMoment).
void momentAufraeumen() {
  moment.art = M_KEINER;
  moment.anzahl = moment.pos = moment.warten = moment.zaehler = moment.unter = 0;
  moment.gedanke = GD_KEINER;
  moment.summt = false;
  moment.bild = MB_KEINS;
  falter.sichtbar = vogel.sichtbar = stern.sichtbar = false;
  gang.laeuft = false;
  geher.lift = 0;
}

// Beendet einen Moment. Evi läuft zu ihrem Teppich zurück, oder ist sofort dort
// (für Spiele, Gute Nacht und Schlaf).
void momentStoppen(bool sofort) {
  const bool weg = geher.x != 0 || geher.flip;
  momentAufraeumen();
  if (sofort) {
    geher.x = 0;
    geher.flip = false;
  } else if (weg) {
    momentStarten(M_HEIM, GD_KEINER);
  }
}

// ---------- Gehen, Hüpfen, Flattern ----------
void gangBeginnen(int ziel) {
  gang.ziel = ziel;
  gang.schritte = 0;
  gang.laeuft = geher.x != ziel;
  if (!gang.laeuft) return;
  gang.vorher = moment.bild;
  moment.bild = MB_LAUFEN;
  geher.flip = ziel > geher.x;    // gezeichnet nach links schauend: nach rechts gespiegelt
}

// Ein Schritt; true = angekommen (dann geht es ohne Warten weiter)
bool gangSchritt() {
  if (!gang.laeuft) return true;
  if (geher.x == gang.ziel) {
    geher.lift = 0;
    moment.bild = gang.vorher;
    gang.laeuft = false;
    return true;
  }
  const int rest = gang.ziel - geher.x;
  const int weite = abs(rest) < SCHRITT_WEITE ? abs(rest) : SCHRITT_WEITE;
  geher.x += rest > 0 ? weite : -weite;
  geher.lift = (gang.schritte / 2) % 2 ? 2 : 0;
  gang.schritte++;
  return false;
}

// Hüpfen: je Hüpfer 5 Takte hoch und runter, dann 2 Takte Pause. Gibt Wartetakte zurück, 0 = fertig.
int huepfen(int mal, int hoehe) {
  if (moment.unter >= mal) return 0;
  const int stufen[5] = {(hoehe + 1) / 2, hoehe, hoehe, (hoehe + 1) / 2, 0};
  if (moment.zaehler == 0) { moment.huepfVorher = moment.bild; moment.bild = MB_HUEPFEN; }
  if (moment.zaehler < 5) { geher.lift = stufen[moment.zaehler]; moment.zaehler++; return 1; }
  moment.bild = moment.huepfVorher;
  moment.zaehler = 0;
  moment.unter++;
  return 2;
}

// Wo der Schmetterling auf Evis Kopf sitzt (headSpot), wo sie auch ist
void kopfStelle(float& x, float& y) {
  x = BODEN_X + geher.x - 15;
  y = eviOben + eviHoehe * 0.17f - 24 + 2;
}

void falterZeigen(float x, float y, bool offen) {
  falter.px = x;
  falter.py = y;
  falter.bild = offen ? 0 : 1;
  falter.sichtbar = true;
}

// Ein Flatterschritt auf ein Ziel zu; true = angekommen
bool flattern(float zx, float zy, float tempo) {
  const float dx = zx - falter.x, dy = zy - falter.y;
  if (sqrtf(dx * dx + dy * dy) > tempo) {
    const float w = atan2f(dy, dx);
    falter.x += cosf(w) * tempo;
    falter.y += sinf(w) * tempo;
    falter.t++;
    falterZeigen(falter.x, falter.y + sinf(falter.t / 3.0f) * 4, (falter.t / 2) % 2 == 0);
    return false;
  }
  falter.x = zx;
  falter.y = zy;
  return true;
}

// Vogel: Bild 0 sitzt, 1 Flügel oben, 2 Flügel unten. Koordinaten wie im Fenster der App.
void vogelZeigen(float x, float y, int haltung) {
  vogel.px = x;
  vogel.py = y;
  vogel.bild = haltung;
  vogel.sichtbar = true;
}

bool vogelFliegen(float zx, float zy, float tempo) {
  const float dx = zx - vogel.x, dy = zy - vogel.y;
  if (sqrtf(dx * dx + dy * dy) > tempo) {
    const float w = atan2f(dy, dx);
    vogel.x += cosf(w) * tempo;
    vogel.y += sinf(w) * tempo;
    vogel.t++;
    vogelZeigen(vogel.x, vogel.y, (vogel.t / 2) % 2 ? 1 : 2);
    return false;
  }
  vogel.x = zx;
  vogel.y = zy;
  return true;
}

const float VOGEL_SITZ_X = 37, VOGEL_SITZ_Y = 43;   // BIRD_PERCH: auf dem Fensterkreuz, oben links

// Führt den aktuellen Schritt aus. 0 = fertig (gleich weiter), n > 0 = n Takte warten.
int aktSchritt(int nummer) {
  const Akt& a = moment.plan[nummer];
  switch (a.art) {
    case A_MELDUNG: zeigeMeldung(a.text, a.a); return 0;
    case A_SAGEN: sage(a.text, &SYM_W_HERZ, a.a); return 0;
    case A_TON: if (a.a == 0) piep(TON(TON_SPEZIAL)); else piep(TON(TON_ZWITSCHERN)); return 0;
    case A_WARTEN: return moment.zaehler++ == 0 ? a.a : 0;
    case A_GEHEN:
      if (moment.zaehler++ == 0) gangBeginnen(a.a);
      return gangSchritt() ? 0 : 1;
    case A_HEIM:
      if (moment.zaehler++ == 0) gangBeginnen(0);
      if (!gangSchritt()) return 1;
      geher.flip = false;
      return 0;
    case A_HUEPFEN: return huepfen(a.a, a.b);
    case A_BILD: moment.bild = (MomentBild)a.a; return 0;
    case A_FLIP: geher.flip = a.a != 0; geher.lift = 0; return 0;
    case A_DENKEN: moment.gedanke = (Gedanke)a.a; return 0;
    case A_SUMMEN: moment.summt = a.a != 0; return 0;
    case A_DREHEN:
      if (moment.zaehler >= a.a) return 0;
      geher.flip = !geher.flip;
      geher.lift = moment.zaehler % 2 ? 0 : 3;
      moment.zaehler++;
      return a.b;
    case A_FALTER_REIN:
      if (moment.zaehler++ == 0) {
        falter.x = -20; falter.y = 96; falter.t = 0;
        kopfStelle(moment.zielX, moment.zielY);
        moment.zielY -= 24;
      }
      return flattern(moment.zielX, moment.zielY, 2.4f) ? 0 : 1;
    case A_FALTER_KREISEN: {
      float kx, ky;
      kopfStelle(kx, ky);
      if (moment.zaehler < 48) {
        const int i = moment.zaehler++;
        falterZeigen(kx + sinf(i / 4.0f) * 22, ky - 22 + cosf(i / 4.0f) * 7, (falter.t / 2) % 2 == 0);
        falter.t++;
        return 1;
      }
      falter.x = kx; falter.y = ky - 22;
      moment.zielX = kx; moment.zielY = ky;
      return 0;
    }
    case A_FALTER_LANDEN:
      return flattern(moment.zielX, moment.zielY, 1.5f) ? 0 : 1;
    case A_FALTER_SITZEN:
      if (moment.zaehler < 32) {
        falterZeigen(moment.zielX, moment.zielY, (moment.zaehler / 8) % 2 == 0);
        moment.zaehler++;
        return 1;
      }
      return 0;
    case A_FALTER_WEG:
      // Weg fliegt er, und Evi läuft ihm ein Stück hinterher (jeden zweiten Takt ein Schritt).
      // moment.unter = 1, sobald der Gang begonnen hat
      if (moment.zaehler++ == 0) { moment.gehtNoch = true; moment.unter = 0; }
      if (falter.x < 340) {
        falter.x += 2.6f;
        falter.y -= falter.y > 40 ? 1.2f : 0;
        falterZeigen(falter.x, falter.y + sinf(falter.t / 3.0f) * 4, (falter.t / 2) % 2 == 0);
        if (moment.gehtNoch && (falter.t % 2)) {
          if (!moment.unter) { gangBeginnen(52); moment.unter = 1; }
          moment.gehtNoch = !gangSchritt();
        }
        falter.t++;
        return 1;
      }
      falter.sichtbar = false;
      if (!moment.unter) { gangBeginnen(52); moment.unter = 1; }
      return gangSchritt() ? 0 : 1;
    case A_VOGEL_REIN:
      // Der Vogel fliegt herein, während Evi ein Stück zum Fenster geht.
      if (moment.zaehler++ == 0) {
        vogel.x = 4; vogel.y = 18; vogel.t = 0;
        moment.gehtNoch = moment.fliegtNoch = true;
        gangBeginnen(-30);
      } else if (!moment.gehtNoch && !moment.fliegtNoch) {
        vogelZeigen(VOGEL_SITZ_X, VOGEL_SITZ_Y, 0);
        return 0;
      }
      if (moment.gehtNoch) moment.gehtNoch = !gangSchritt();
      if (moment.fliegtNoch) moment.fliegtNoch = !vogelFliegen(VOGEL_SITZ_X, VOGEL_SITZ_Y, 1.6f);
      return 1;
    case A_VOGEL_SINGEN:
      if (moment.zaehler < 4) {
        vogelZeigen(VOGEL_SITZ_X, VOGEL_SITZ_Y - (moment.zaehler % 2 ? 0 : 2), 0);
        moment.zaehler++;
        return 3;
      }
      return 0;
    case A_VOGEL_WEG:
      if (!vogelFliegen(104, -4, 2.2f)) return 1;
      vogel.sichtbar = false;
      return 0;
    case A_STERN:
      // Von oben rechts nach unten links quer durchs Fenster
      if (moment.zaehler++ == 0) { stern.x = a.a; stern.y = a.b; }
      if (stern.x > 6) {
        stern.px = stern.x; stern.py = stern.y; stern.sichtbar = true;
        stern.x -= 3; stern.y += 1.5f;
        return 1;
      }
      stern.sichtbar = false;
      return 0;
  }
  return 0;
}

void momentAusfuehren() {
  if (moment.warten > 0) { moment.warten--; return; }
  while (moment.pos < moment.anzahl) {
    const int r = aktSchritt(moment.pos);
    if (r > 0) { moment.warten = r - 1; return; }
    moment.pos++;
    moment.zaehler = 0;
    moment.unter = 0;
  }
  momentAufraeumen();
}

// ---------- Begrüßung (greet in app.js) ----------
// Was Evi sagt, wenn du wiederkommst; gross = besonders froh (mehr Hüpfer)
const char* grussText(ms_t jetzt, ms_t seit, bool& gross) {
  const int h = stundeVon(jetzt);
  const ms_t weg = jetzt - seit;
  // Der erste Besuch, seit sie heute Morgen aufgestanden ist
  const bool ersterHeute = tagKennung(seit) != tagKennung(jetzt) || stundeVon(seit) < 7;
  gross = true;
  if (ersterHeute && h >= 7 && h < 11) return "Guten Morgen!";
  if (weg >= LANGE_WEG) return "Da bist du ja endlich!";
  gross = false;
  if (h >= 18 && weg >= STUNDE) return "Guten Abend!";
  static const char* TEXTE[3] = {"Da bist du ja!", "Hallo!", "Schön, dass du da bist!"};
  return TEXTE[random(0, 3)];
}

// Evi freut sich, dass du da bist: Sie hüpft und sagt Hallo. Nicht im Schlaf, nicht krank, nur erwachsen.
void begruessen(ms_t seit) {
  const ms_t jetzt = jetztMs();
  if (phaseBei(pet, jetzt) != ERWACHSEN || ruhend(beschreiben(pet, jetzt)) || geweckt(pet, jetzt) || spiel.aktiv || ritual.aktiv) return;
  if (moment.letzterGruss != 0 && millis() - moment.letzterGruss < 5000) return;   // Einschalten und Aufwachen kommen oft zusammen
  moment.letzterGruss = millis();
  const int laune = launeHeute(jetzt);
  bool gross;
  const char* text = grussText(jetzt, seit, gross);
  const bool jubel = gross || laune == LAUNE_VERSCHMUST;   // verschmust hüpft sie immer etwas mehr
  momentStoppen(true);
  reagiere(jubel ? R_JUBEL : R_GRUSS, jubel ? 20 : 12);
  sage(text, &SYM_W_HERZ, 3500);
  moment.ruheBisTakt = evi.takt + RUHE_TAKTE;
  // Die erste Begrüßung des Tages: Sie sagt dir ihre Tageslaune.
  const int32_t heute = tagKennung(jetzt);
  if (moment.launeTag != heute) {
    moment.launeTag = heute;
    moment.launeSagen = laune;
    moment.launeSagenUm = millis() + 3600;
  }
}

// Der Bildschirm wurde geweckt (erster Tipper oder Tastendruck im Dunkeln)
void aufgeweckt() {
  const ms_t jetzt = jetztMs();
  if (dunkelSeit && jetzt - dunkelSeit >= GRUSS_NACH) begruessen(dunkelSeit);
}

// Nach dem Einschalten kurz warten, dann begrüßen
void begruessenPlanen(ms_t seit) {
  moment.grussGeplant = true;
  moment.grussSeit = seit;
  moment.grussUm = millis() + 700;
}

// ---------- Jeder Takt (runMoments in app.js) ----------
void momentTakt(ms_t jetzt) {
  const Stimmung s = beschreiben(pet, jetzt);
  if (ruhend(s) && (moment.art != M_KEINER || geher.x != 0 || geher.flip)) momentStoppen(true);
  if (moment.grussGeplant && (int32_t)(millis() - moment.grussUm) >= 0) {
    moment.grussGeplant = false;
    if (bildschirmAn) begruessen(moment.grussSeit);
  }
  if (moment.launeSagen >= 0 && (int32_t)(millis() - moment.launeSagenUm) >= 0) {
    if (!spiel.aktiv && !ritual.aktiv) sage(LAUNEN[moment.launeSagen].text, LAUNEN[moment.launeSagen].symbol, 4000);
    moment.launeSagen = -1;
  }
  if (moment.art != M_KEINER) { momentAusfuehren(); return; }
  if (evi.takt % 8 || !momentErlaubt()) return;
  if (phaseBei(pet, jetzt) != ERWACHSEN || ruhend(s) || geweckt(pet, jetzt)) return;
  MomentArt besuch;
  if (besuchFaellig(jetzt, besuch)) {
    moment.letzterBesonderer = jetzt;
    if (moment.naechster < jetzt + MOMENT_MIN) moment.naechster = jetzt + MOMENT_MIN;
    if (himmelBei(jetzt) == HIMMEL_NACHT) besuch = M_STERN;   // im Dunkeln eine Sternschnuppe
    bildschirmEin();                                          // Besuch kommt mit Ton: Bildschirm an
    momentStarten(besuch, GD_KEINER);
    return;
  }
  if (!bildschirmAn || jetzt < moment.naechster) return;     // kleine Momente nur, wenn jemand zuschaut
  moment.naechster = jetzt + naechsterAbstand();
  Gedanke gedanke = GD_KEINER;
  const MomentArt art = momentWaehlen(s, muede(pet, jetzt), launeHeute(jetzt), gedanke);
  if (art == M_KEINER) return;
  moment.letzter = art;
  momentStarten(art, gedanke);
}

// ---------- Zeichnen ----------
// Pixelbilder der App: Schmetterling 15 × 12 (doppelt groß), Vogel 14 × 10 (anderthalbfach)
const char* const FALTER_BILD[2][12] = {
  {"...#.......#...", "....#.....#....", ".####.#.#.####.", "######.#.######", "#...##.#.##...#", "#...##.#.##...#",
   "######.#.######", ".####..#..####.", "...##..#..##...", "..####.#.####..", ".#####.#.#####.", "..##...#...##.."},
  {"...#.......#...", "....#.....#....", ".....#.#.#.....", "....##.#.##....", "...###.#.###...", "...#.#.#.#.#...",
   "...###.#.###...", "....##.#.##....", ".....#.#.#.....", "....##.#.##....", "....##.#.##....", ".......#......."}};
const char* const VOGEL_BILD[3][10] = {
  {".......####...", "......######..", "......##.####.", "......########", "##...#######..", ".##.########..",
   "..##########..", "...#########..", "....#######...", ".....#...#...."},
  {"..###.........", "...###.####...", "....#######...", ".....##.####..", ".....########.", "##..#######...",
   ".##########...", "...#######....", "", ""},
  {"..............", ".......####...", "......######..", "......##.####.", "##....########", ".##.#######...",
   "..#########...", "...######.....", "....###.......", "...###........"}};

// Kreis mit Kommaradius wie <circle> im SVG
void kreisFuellen(float cx, float cy, float r, uint16_t farbe) {
  for (int y = (int)floorf(cy - r); y <= (int)ceilf(cy + r); y++) {
    for (int x = (int)floorf(cx - r); x <= (int)ceilf(cx + r); x++) {
      const float dx = x + 0.5f - cx, dy = y + 0.5f - cy;
      if (dx * dx + dy * dy <= r * r) bild.drawPixel(x, y, farbe);
    }
  }
}

// Gehört der Punkt zum Himmel im Fenster? (am Tag weiß, nachts schwarz; ohne Mond, Hügel, Rahmen)
bool himmelPunkt(int x, int y) {
  if (x < 0 || x >= BREITE || y < 0 || y >= 224) return false;
  const int i = y * 40 + x / 8, b = 7 - (x % 8);
  return !((ZIMMER_TAG[i] >> b) & 1) && ((ZIMMER_NACHT[i] >> b) & 1);
}

// Was im Fenster passiert (vor dem Abdunkeln bei Licht aus): Vogel und Sternschnuppe.
// Das Fenster der App liegt 23 Punkte tiefer (translate(0 23)).
void momentZeichnenHinten() {
  if (vogel.sichtbar) {
    const int bx = (int)lroundf(vogel.px), by = (int)lroundf(vogel.py) + 23;
    for (int r = 0; r < 10; r++) {
      const char* zeile = VOGEL_BILD[vogel.bild][r];
      for (int c = 0; zeile[c]; c++) {
        if (zeile[c] != '#') continue;
        const int x0 = bx + (3 * c) / 2, x1 = bx + (3 * (c + 1)) / 2, y0 = by + (3 * r) / 2, y1 = by + (3 * (r + 1)) / 2;
        for (int y = y0; y < y1; y++)
          for (int x = x0; x < x1; x++)
            if (x >= 33 && x < 95 && y >= 26 + 23 && y < 95 + 23) bild.drawPixel(x, y, 0);
      }
    }
    // Das Fensterkreuz liegt vor dem Vogel
    bild.fillRect(61, 24 + 23, 4, 75, 0);
    bild.fillRect(31, 58 + 23, 65, 4, 0);
  }
  if (stern.sichtbar) {
    static const int8_t TEILE[10][4] = {{2, 0, 1, 5}, {0, 2, 5, 1}, {1, 1, 3, 3}, {5, 1, 3, 1}, {8, 0, 3, 1},
                                        {11, -1, 3, 1}, {14, -2, 2, 1}, {17, -4, 2, 1}, {21, -6, 1, 1}, {25, -8, 1, 1}};
    const int sx = (int)lroundf(stern.px), sy = (int)lroundf(stern.py) + 23;
    for (int i = 0; i < 10; i++)
      for (int y = 0; y < TEILE[i][3]; y++)
        for (int x = 0; x < TEILE[i][2]; x++) {
          const int px = sx + TEILE[i][0] + x, py = sy + TEILE[i][1] + y;
          if (himmelPunkt(px, py)) bild.drawPixel(px, py, 1);
        }
  }
}

// Was vor Evi liegt: Gedankenwolke, Summ-Noten (beide gehen mit ihr) und der Schmetterling
void momentZeichnenVorne() {
  const int wx = geher.x;
  if (moment.gedanke != GD_KEINER) {
    static const float WOLKE[6][4] = {{222, 86, 4, 2}, {229, 76, 5.5f, 3.5f}, {238, 58, 13, 11},
                                      {252, 50, 15, 13}, {266, 58, 13, 11}, {252, 66, 12, 10}};
    for (int i = 0; i < 6; i++) kreisFuellen(WOLKE[i][0] + wx, WOLKE[i][1], WOLKE[i][2], 0);
    for (int i = 0; i < 6; i++) kreisFuellen(WOLKE[i][0] + wx, WOLKE[i][1], WOLKE[i][3], 1);
    const Bild& g = *GEDANKE_BILD[moment.gedanke];
    zeichneSymbol(g, 252 + wx - g.w / 2, 56 - g.h / 2, 0);
  }
  if (moment.summt) {
    // notes 1.6 s steps(4): 4, 0, -4, -8 Punkte
    const int dy = 4 - 4 * (int)((millis() % 1600) / 400);
    zeichneSymbol(SYM_NOTE_KLEIN, 214 + wx, 96 - SYM_NOTE_KLEIN.h + dy, 0);
    zeichneSymbol(SYM_NOTEN, 232 + wx, 80 - SYM_NOTEN.h + dy, 0);
  }
  if (falter.sichtbar) {
    const int fx = (int)lroundf(falter.px), fy = (int)lroundf(falter.py);
    for (int r = 0; r < 12; r++)
      for (int c = 0; c < 15; c++)
        if (FALTER_BILD[falter.bild][r][c] == '#') bild.fillRect(fx + 2 * c, fy + 2 * r, 2, 2, 0);
  }
}

// Welches Bild ein Moment gerade für Evi ausleiht (nullptr = das übliche)
const Bild* momentBild(Phase ph) {
  if (ph != ERWACHSEN || moment.bild == MB_KEINS) return nullptr;
  if (moment.bild == MB_GLUECKLICH) return ART[ERWACHSEN][1];
  return MOMENT_BILD[moment.bild];
}
