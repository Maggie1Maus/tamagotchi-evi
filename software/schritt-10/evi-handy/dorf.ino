// Das Dorf (26.11.2026): Im Park läuft das Tamagotchi den Kiesweg entlang zu den Häusern. Dort zeigt der Bildschirm das Dorf
// (zur Weihnachtszeit die Dorfbilder aus entwuerfe/dorf/weihnachten, sonst der Park); eine Navigation im Dorf gibt es nicht.
// Mit A öffnet sich die Liste „Wen besuchst du?“: alle, die man aus dem Park kennt, mit „zu Hause“, „im Park“ oder
// „wohnt bei dir“. Das Haus sieht aus wie das eigene Zimmer (ohne Weihnachtsdeko).
//  - Wen man nicht kennt, steht nicht in der Liste. Steht die Figur gerade im Park (parkBesucher), ist sie nicht zu Hause.
//  - Jeder Besuch zählt als Treffen (mehr Freundschaft, auch mehrmals am Tag). Beste Freunde schenken höchstens einmal
//    am Tag etwas.
//  - Im Haus: Begrüßung, zweiter Satz, vielleicht ein Geschenk, dann plaudern (A: nächster Satz), B geht zurück ins Dorf.

const char* const NICHT_ZUHAUSE[8] = {"%s ist gerade im Park.", "%s ist nicht zu Hause. Vielleicht im Park?", "Niemand öffnet. %s ist im Park!",
                                      "Klopf, klopf … %s ist unterwegs.", "%s ist heute draußen im Park.",
                                      "Die Tür bleibt zu: %s spielt im Park.", "%s ist nicht da. Schau im Park!", "Keiner da. %s ist im Park."};
const char* const KENN_ICH_NICHT[3] = {"Das Haus kennst du noch nicht.", "Dort wohnt jemand, den du noch nicht kennst.",
                                       "Erst im Park treffen, dann besuchen!"};

// Wer gerade im Park steht, ist nicht zu Hause
bool dorfZuHause(int f, ms_t jetzt) { return f != parkBesucher(jetzt) && f != haushalt.partner; }

int dorfPos = 0;            // Zeile in der Liste

// Alle Figuren, die man aus dem Park kennt (mindestens ein Treffen), in der Reihenfolge der Figuren
int dorfListe(int* liste) {
  int n = 0;
  for (int i = 0; i < BESUCHER; i++) if (parkFreund.treffer[i]) liste[n++] = i;
  return n;
}

// Wählt eine Figur in der Liste (öffnet sie, wenn nötig); false, wenn man sie nicht kennt
bool dorfWaehle(int f) {
  int liste[BESUCHER];
  const int n = dorfListe(liste);
  for (int i = 0; i < n; i++) if (liste[i] == f) { dorfPos = i; albumWahl = f; park.dorfMenue = true; return true; }
  return false;
}

// Angekommen am Dorf: Bild und Evi, die Liste ist noch zu
void dorfOeffnen(ms_t jetzt) {
  park.phase = PARK_DORF;
  park.meldung = 0;
  park.zweiter = 0;
  park.dorfMenue = false;
  dorfPos = 0;
}

void dorfMeldung(uint8_t art, int nr, uint32_t ms) {
  park.meldung = art;
  park.meldungNr = (uint8_t)nr;
  park.meldungBis = millis() + ms;
}

// Besuchen: nur Bekannte, und nur wenn sie zu Hause sind
void dorfBesuchen(ms_t jetzt) {
  int liste[BESUCHER];
  const int n = dorfListe(liste);
  if (!n) { park.dorfMenue = false; return; }
  dorfPos = max(0, min(dorfPos, n - 1));
  const int f = liste[dorfPos];
  albumWahl = f;
  if (!parkFreund.treffer[f]) { dorfMeldung(2, random(0, 3), 3500); piep(TON(TON_DANEBEN)); return; }
  if (f == haushalt.partner) { dorfMeldung(3, 0, 4000); piep(TON(TON_DANEBEN)); return; }
  if (!dorfZuHause(f, jetzt)) { dorfMeldung(1, random(0, 8), 4000); piep(TON(TON_DANEBEN)); return; }
  park.besuch = f;
  park.phase = PARK_BESUCHT;
  park.rechts = true;
  parkFreundBesuchZaehlen(jetzt);
  // Begrüßung: Hausgruß oder ein Satz der Stufe
  park.zweiter = 0;
  park.grussArt = random(0, 2) == 0 ? 2 : 1;
  park.gruss = random(0, 5);
  parkNachsatzWaehlen(jetzt);
}

// Ein Besuch zählt als Treffen. Mehrere am Tag bringen jeweils mehr Freundschaft; beste Freunde schenken einmal am Tag.
void parkFreundBesuchZaehlen(ms_t jetzt) {
  parkFreundPruefen();
  parkFreundTag(jetzt);
  const int f = park.besuch;
  park.stufe = parkFreundStufe(parkFreund.treffer[f]);
  park.passt = FIGUR_WESEN[f] == eviWesen();
  park.geschenk = 0;
  if (parkFreund.treffer[f] < 255) parkFreund.treffer[f]++;
  parkFreundGeaendert = true;
  freundPlus(park.passt ? 2.0f : 0.5f);
  if (park.stufe == 3 && !(stamm.geschenkMaske[f / 8] >> (f % 8) & 1) && random(0, 3) == 0) {
    stamm.geschenkMaske[f / 8] |= 1 << (f % 8);
    stammGeaendert = true;
    parkGeschenkGeben(jetzt);
  }
  speichern(jetzt);
}

// A im Haus: Begrüßung → zweiter Satz → Geschenk (falls) → danach immer neue Sätze zum Plaudern
void dorfBesuchWeiter(ms_t jetzt) {
  if (park.zweiter == 0) { park.zweiter = 1; return; }
  if (park.zweiter == 1 && park.geschenk) { park.zweiter = 2; piep(TON(TON_MUENZE)); return; }
  const uint8_t art = park.nachArt, satz = park.nachSatz;
  for (int i = 0; i < 6; i++) {                          // nie zweimal derselbe Satz hintereinander
    parkNachsatzWaehlen(jetzt);
    if (park.nachArt != art || park.nachSatz != satz) break;
  }
  park.zweiter = 3;
}

// Zurück ins Dorf (ein Geschenk, das man nicht mehr gesehen hat, wird gemeldet)
void dorfBesuchEnde() {
  if (park.geschenk && park.zweiter < 2) {
    static char geschenk[64];
    parkGeschenkText(geschenk, sizeof(geschenk));
    zeigeMeldung(geschenk, 4000);
  }
  park.geschenk = 0;
  park.zweiter = 0;
  const int f = park.besuch;
  park.phase = PARK_DORF;
  park.meldung = 0;
  liebeAngebot(f, PARK_DORF, jetztMs());                  // beste Freunde des anderen Geschlechts: „Partner?“ (haushalt.ino)
}

void dorfKnopf(Knopf k) {
  const ms_t jetzt = jetztMs();
  if (park.phase == PARK_BESUCHT) {
    if (k == K_A) dorfBesuchWeiter(jetzt);
    else if (k == K_B || k == K_START) dorfBesuchEnde();
    return;
  }
  if (!park.dorfMenue) {                                  // nur das Dorfbild: A öffnet die Liste, B geht zurück in den Park
    if (k == K_A) { park.dorfMenue = true; park.meldung = 0; }
    else if (k == K_B || k == K_START) park.phase = PARK_WAHL;
    return;
  }
  int liste[BESUCHER];
  const int n = dorfListe(liste);
  if (k == K_B || k == K_START) { park.dorfMenue = false; park.meldung = 0; return; }
  if (k == K_A) { dorfBesuchen(jetzt); return; }
  if (!n) return;
  if (k == K_UP || k == K_L) dorfPos = (dorfPos + n - 1) % n;
  else if (k == K_DOWN || k == K_R) dorfPos = (dorfPos + 1) % n;
  park.meldung = 0;
}

// Die Liste: rechts neben dem Dorf, 7 Zeilen sichtbar
static const int DORF_LISTE_X = 148, DORF_LISTE_B = 168, DORF_ZEILE_H = 17, DORF_ZEILEN = 7;
static int dorfListeOben() { return STATUS_H + 4; }
static int dorfErsteZeile(int n) { return max(0, min(dorfPos - DORF_ZEILEN / 2, n - DORF_ZEILEN)); }

void dorfTouch(int x, int y) {
  const ms_t jetzt = jetztMs();
  if (park.phase == PARK_BESUCHT) {
    if (y >= ZEILE_Y) { if (x >= 240) dorfBesuchEnde(); else dorfBesuchWeiter(jetzt); }
    else dorfBesuchWeiter(jetzt);
    return;
  }
  if (y >= ZEILE_Y) {                                      // untere Zeile: links A, rechts B
    dorfKnopf(x >= 240 ? K_B : K_A);
    return;
  }
  if (!park.dorfMenue) { park.dorfMenue = true; park.meldung = 0; return; }   // das Dorf antippen öffnet die Liste
  int liste[BESUCHER];
  const int n = dorfListe(liste);
  const int y0 = dorfListeOben() + 18;
  if (x >= DORF_LISTE_X && x < DORF_LISTE_X + DORF_LISTE_B && y >= y0 && y < y0 + DORF_ZEILEN * DORF_ZEILE_H && n) {
    const int zeile = dorfErsteZeile(n) + (y - y0) / DORF_ZEILE_H;
    if (zeile >= n) return;
    if (zeile == dorfPos) dorfBesuchen(jetzt);             // zweites Antippen derselben Zeile besucht
    else { dorfPos = zeile; park.meldung = 0; }
    return;
  }
  if (x < DORF_LISTE_X || y < dorfListeOben()) { park.dorfMenue = false; park.meldung = 0; }   // daneben: Liste zu
}

// Die Figur des Besuchs in Originalgröße (Bild 72 × 64, Standlinie auf dem Boden), hinter Möbeln verdeckt
static void dorfBesucherZeichnen(int f, Phase ph, int cx, int fuss) {
  const BesuchBild& b = BESUCH_BILD[f][ph - BABY];
  ellipseFuellen(cx, fuss, 18, 2, 119);
  const int ox = cx - BESUCH_BREITE / 2 + b.x, oy = fuss - BESUCH_STANDLINIE + b.y;
  for (int y = 0; y < b.bild.h; y++) for (int x = 0; x < b.bild.w; x++) {
    if (!b.bild.deckt(x, y)) continue;
    if (raumVerdeckt(false, fuss, ox + x, oy + y)) continue;
    bild.drawPixel(ox + x, oy + y, b.bild.wert(x, y));
  }
}

// Hintergrund des Dorfs: zur Weihnachtszeit die Dorfbilder (Tageszeit × Wetter), sonst der Park (park.ino)
void dorfHintergrund(ms_t jetzt) {
  if (weihnachtsZeit(jetzt)) {
    const ZimmerBild& b = *DORF_WEIHNACHT[himmelBei(jetzt)][wetterBei(jetzt)];
    uint8_t* puffer = bild.getBuffer() + STATUS_H * BREITE;
    for (int y = 0; y < b.h; y++) for (int x = 0; x < b.w; x++) *puffer++ = grauStufe(b.grau, b.w, x, y) * 17;
    bild.markieren();
    return;
  }
  parkHintergrundKomplett(jetzt);
}

void dorfZeichnen(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
  if (park.phase == PARK_DORF && (ph < BABY || ph > OMA)) { parkSchliessen(); return; }
  if (park.phase == PARK_BESUCHT) {
    dorfHaus = true;                                       // das eigene Zimmer ohne Weihnachtsdeko
    zimmerZeichnen(jetzt);
    const int fuss = RAUM_HEIM_Y - 6;
    ellipseFuellen(110, fuss, 24, 2, 119);
    raumFigurZeichnen(*ART[ph][1], ph, 110, fuss, true, 0, false, false);
    dorfBesucherZeichnen(park.besuch, ph, 225, fuss);
    dorfHaus = false;
    zeichneStatus(ph);
    bild.fillRect(0, ZEILE_Y, BREITE, HOEHE - ZEILE_Y, 1);
    bild.drawFastHLine(0, ZEILE_Y, BREITE, 0);
    char text[64];
    if (park.zweiter == 2) parkGeschenkText(text, sizeof(text));
    else {
      const char* satz = park.zweiter == 0 ? (park.grussArt == 2 ? GRUSS_HAUS[park.gruss] : parkStufenGruss(park.stufe < 1 ? 1 : park.stufe, park.gruss))
                                           : parkNachsatz();
      parkBesuchSatz(text, sizeof(text), satz);
    }
    schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 6, text, 0);
    schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 22, tastenDa ? "A Weiter" : "Weiter", 0);
    schreibe(bild, 242, ZEILE_Y + 22, tastenDa ? "B Nach Hause" : "Nach Hause", 0);
    zeigen();
    return;
  }
  // Das Dorf: Bild, Evi vorne links, darüber die Liste
  dorfHintergrund(jetzt);
  {
    const int fuss = STATUS_H + 163, x = 62;
    const Bild& e = *ART[ph][1];
    const int h = max(1, raumFigurHoehe(e, ph, fuss, true, false) * 4 * 11 / (5 * 10)), w = max(1, e.w * h / e.h);
    ellipseFuellen(x, fuss, max(3, w / 5), 1, 119);
    raumRasterZeichnen(e, x - w / 2, fuss - h + 1, w, h, true, false, true, fuss);
  }
  zeichneStatus(ph);
  int liste[BESUCHER];
  const int n = dorfListe(liste);
  if (park.dorfMenue) {
    const int x0 = DORF_LISTE_X, y0 = dorfListeOben(), w = DORF_LISTE_B, h = 18 + DORF_ZEILEN * DORF_ZEILE_H + 4;
    bild.fillRoundRect(x0 + 2, y0 + 2, w, h, 7, 0);
    bild.fillRoundRect(x0, y0, w, h, 7, 1);
    bild.drawRoundRect(x0, y0, w, h, 7, 0);
    schreibe(bild, x0 + 9, y0 + 6, "Wen besuchst du?", 0, 1, true);
    bild.drawFastHLine(x0 + 6, y0 + 16, w - 12, 0);
    if (!n) {
      schreibe(bild, x0 + 9, y0 + 28, "Du kennst noch niemanden.", 0);
      schreibe(bild, x0 + 9, y0 + 42, "Triff erst Freunde im Park!", 0);
    }
    const int ab = dorfErsteZeile(n);
    for (int z = 0; z < DORF_ZEILEN && ab + z < n; z++) {
      const int f = liste[ab + z], y = y0 + 18 + z * DORF_ZEILE_H;
      const bool sel = ab + z == dorfPos;
      if (sel) bild.fillRect(x0 + 5, y, w - 10, DORF_ZEILE_H - 1, 0);
      const char* status = f == haushalt.partner ? "wohnt bei dir" : dorfZuHause(f, jetzt) ? "zu Hause" : "im Park";
      schreibe(bild, x0 + 9, y + 5, besuchName(f), sel ? 1 : 0);
      schreibe(bild, x0 + w - 9 - textBreite(status), y + 5, status, sel ? 1 : 0);
    }
    if (ab > 0) schreibe(bild, x0 + w - 14, y0 + 6, "▲", 0);
    if (ab + DORF_ZEILEN < n) schreibe(bild, x0 + w - 14, y0 + h - 12, "▼", 0);
  }
  bild.fillRect(0, ZEILE_Y, BREITE, HOEHE - ZEILE_Y, 1);
  bild.drawFastHLine(0, ZEILE_Y, BREITE, 0);
  char zeile1[64];
  if (park.meldung && millis() < park.meldungBis) {
    const int f = n ? liste[max(0, min(dorfPos, n - 1))] : 0;
    if (park.meldung == 1) snprintf(zeile1, sizeof(zeile1), NICHT_ZUHAUSE[park.meldungNr % 8], besuchName(f));
    else if (park.meldung == 3) snprintf(zeile1, sizeof(zeile1), "%s wohnt jetzt bei dir! ♥", besuchName(f));
    else snprintf(zeile1, sizeof(zeile1), "%s", KENN_ICH_NICHT[park.meldungNr % 3]);
  } else if (park.dorfMenue) {
    snprintf(zeile1, sizeof(zeile1), "%s", n ? "Wähle, wen du besuchen möchtest." : "Hier wohnen deine Freunde.");
  } else {
    snprintf(zeile1, sizeof(zeile1), "Das Dorf. Hier wohnen deine Freunde.");
  }
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 6, zeile1, 0);
  const char* aktion = park.dorfMenue ? (n ? (tastenDa ? "A Besuchen" : "Antippen, nochmal = Besuchen") : "") : (tastenDa ? "A Besuchen …" : "Dorf antippen");
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 22, aktion, 0);
  schreibe(bild, 242, ZEILE_Y + 22, park.dorfMenue ? (tastenDa ? "B Schließen" : "Schließen") : (tastenDa ? "B Zurück" : "Zurück"), 0);
  zeigen();
}
