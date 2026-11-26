// Das Dorf (26.11.2026): Im Park läuft das Tamagotchi den Kiesweg entlang zu den Häusern, wählt ein Tamagotchi aus,
// das es schon aus dem Park kennt, und besucht es zu Hause. Das Haus sieht aus wie das eigene Zimmer (ohne Weihnachtsdeko).
//  - Wen man nicht kennt, kann man nicht besuchen. Steht die Figur gerade im Park (parkBesucher), ist sie nicht zu Hause.
//  - Jeder Besuch zählt als Treffen (mehr Freundschaft, auch mehrmals am Tag). Beste Freunde schenken höchstens einmal
//    am Tag etwas.
//  - Im Haus: Begrüßung, zweiter Satz, vielleicht ein Geschenk, dann plaudern (A: nächster Satz), B geht zurück ins Dorf.

const char* const NICHT_ZUHAUSE[8] = {"%s ist gerade im Park.", "%s ist nicht zu Hause. Vielleicht im Park?", "Niemand öffnet. %s ist im Park!",
                                      "Klopf, klopf … %s ist unterwegs.", "%s ist heute draußen im Park.",
                                      "Die Tür bleibt zu: %s spielt im Park.", "%s ist nicht da. Schau im Park!", "Keiner da. %s ist im Park."};
const char* const KENN_ICH_NICHT[3] = {"Das Haus kennst du noch nicht.", "Dort wohnt jemand, den du noch nicht kennst.",
                                       "Erst im Park treffen, dann besuchen!"};

// Wer gerade im Park steht, ist nicht zu Hause
bool dorfZuHause(int f, ms_t jetzt) { return f != parkBesucher(jetzt); }

// Angekommen am Dorf: Auswahl öffnen
void dorfOeffnen(ms_t jetzt) {
  park.phase = PARK_DORF;
  park.meldung = 0;
  park.zweiter = 0;
  if (!parkFreund.treffer[albumWahl]) {                // Auswahl beginnt bei jemandem, den man kennt
    for (int i = 0; i < BESUCHER; i++) if (parkFreund.treffer[i]) { albumWahl = i; break; }
  }
}

void dorfMeldung(uint8_t art, int nr, uint32_t ms) {
  park.meldung = art;
  park.meldungNr = (uint8_t)nr;
  park.meldungBis = millis() + ms;
}

// Besuchen: nur Bekannte, und nur wenn sie zu Hause sind
void dorfBesuchen(ms_t jetzt) {
  const int f = albumWahl;
  if (!parkFreund.treffer[f]) { dorfMeldung(2, random(0, 3), 3500); piep(TON(TON_DANEBEN)); return; }
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
  park.phase = PARK_DORF;
  park.meldung = 0;
}

void dorfKnopf(Knopf k) {
  const ms_t jetzt = jetztMs();
  if (park.phase == PARK_BESUCHT) {
    if (k == K_A) dorfBesuchWeiter(jetzt);
    else if (k == K_B || k == K_START) dorfBesuchEnde();
    return;
  }
  // Auswahl im Dorf
  if (k == K_B || k == K_START) { park.phase = PARK_WAHL; park.meldung = 0; return; }
  if (k == K_A) { dorfBesuchen(jetzt); return; }
  albumKnopf(k);
  park.meldung = 0;
}

void dorfTouch(int x, int y) {
  const ms_t jetzt = jetztMs();
  if (y >= ZEILE_Y) {
    if (park.phase == PARK_BESUCHT) { if (x >= 240) dorfBesuchEnde(); else dorfBesuchWeiter(jetzt); }
    else if (x >= 240) { park.phase = PARK_WAHL; park.meldung = 0; }
    else dorfBesuchen(jetzt);
    return;
  }
  if (park.phase == PARK_BESUCHT) { dorfBesuchWeiter(jetzt); return; }
  const int sp = x / ALBUM_ZELLE_B, ze = (y - ALBUM_GITTER_Y) / ALBUM_ZELLE_H;
  if (y < ALBUM_GITTER_Y || sp < 0 || sp >= ALBUM_SPALTEN || ze < 0 || ze >= ALBUM_ZEILEN) return;
  const int neu = ze * ALBUM_SPALTEN + sp;
  if (neu == albumWahl) dorfBesuchen(jetzt);             // zweites Antippen derselben Figur besucht sie
  else { albumWahl = neu; park.meldung = 0; }
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

void dorfZeichnen(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
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
  albumRaster(jetzt, "Dorf: Bekannte");
  bild.fillRect(0, ZEILE_Y, BREITE, HOEHE - ZEILE_Y, 1);
  bild.drawFastHLine(0, ZEILE_Y, BREITE, 0);
  char zeile1[64];
  const int treffer = parkFreund.treffer[albumWahl];
  if (park.meldung && millis() < park.meldungBis) {
    if (park.meldung == 1) snprintf(zeile1, sizeof(zeile1), NICHT_ZUHAUSE[park.meldungNr % 8], besuchName(albumWahl));
    else snprintf(zeile1, sizeof(zeile1), "%s", KENN_ICH_NICHT[park.meldungNr % 3]);
  } else if (treffer) {
    snprintf(zeile1, sizeof(zeile1), "%s: %s", besuchName(albumWahl), dorfZuHause(albumWahl, jetzt) ? "zu Hause" : "gerade im Park");
  } else {
    snprintf(zeile1, sizeof(zeile1), "???");
  }
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 6, zeile1, 0);
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 22, tastenDa ? "A Besuchen" : "Antippen, nochmal = Besuchen", 0);
  schreibe(bild, 242, ZEILE_Y + 22, tastenDa ? "B Zurück" : "Zurück", 0);
  zeigen();
}
