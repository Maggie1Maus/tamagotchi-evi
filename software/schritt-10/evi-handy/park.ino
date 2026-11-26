// Öffentlicher Parkeingang. Evi läuft selbst zu den drei antippbaren Zielen.
static constexpr ParkPunkt PARK_WEGPUNKT = {137, 146};
static constexpr ParkPunkt PARK_ANKUNFT[2] = {{158, 150}, {110, 120}};
// Das Dorf: Die Häuser stehen am fernen Ende des Kieswegs. Evi läuft bis dorthin.
static constexpr ParkPunkt PARK_DORF_ANKUNFT = {148, 84};
static constexpr int PARK_DORF_X0 = 124, PARK_DORF_X1 = 196, PARK_DORF_Y0 = 56, PARK_DORF_Y1 = 71;   // antippbare Fläche
static constexpr ParkPunkt PARK_FUNDPLATZ[2] = {{179, 149}, {104, 105}};
// Dort steht der Besuch (früher immer Lotti). Füße auf derselben Höhe wie Lotti bisher.
static constexpr int PARK_BESUCH_X = 185, PARK_BESUCH_FUSS = 133;
// Figuren im Park 10 % größer als früher (Wunsch vom 25.11.2026).
static constexpr int PARK_GROESSE_Z = 11, PARK_GROESSE_N = 10;
// Der Besuch wechselt alle paar Stunden, nicht bei jedem Parkbesuch (Wunsch vom 25.11.2026).
static constexpr int PARK_BESUCH_STUNDEN = 4;

// 50 Begrüßungen, zufällig (Wunsch vom 25.11.2026). * wird durch Evis Namen ersetzt.
// Höchstens 40 Zeichen, damit „Glöckchen: “ und der Satz in eine Zeile passen.
static const char* const PARK_GRUSS_TEXT[] = {
  "Hallo, *! Schön, dass du da bist.",
  "Hallo, *! Schön, dich zu sehen.",
  "Da bist du ja, *!",
  "Hui, *! Wie schön!",
  "Hallihallo, *!",
  "Hallo, *! Wie geht es dir?",
  "Na, *? Spielst du mit mir?",
  "Oh, *! Ich hab an dich gedacht.",
  "Juhu, Besuch! Hallo, *!",
  "Hallo, *! Magst du den Park auch?",
  "Schön, dass wir uns treffen, *!",
  "Hey, *! Lange nicht gesehen!",
  "Ich hab auf dich gewartet, *!",
  "Hallo, *! Hast du was gefunden?",
  "Komm, *, wir laufen eine Runde!",
  "*! Wollen wir Verstecken spielen?",
  "Hallo, *! Ich freu mich so!",
  "Na so was, *! Auch im Park?",
  "Hallöchen, *! Alles gut bei dir?",
  "Guck mal, *! Hier ist es schön.",
  "*! Erzähl mal, was gibt's Neues?",
  "Huhu, *! Hier drüben!",
  "Hallo, *! Du siehst fröhlich aus.",
  "Schön, dass du vorbeikommst, *!",
  "Moin, *! Wie war dein Tag?",
  "Servus, *! Gehen wir ein Stück?",
  "Grüß dich, *! Was machst du heute?",
  "Oh, Besuch! Hallo, *!",
  "*! Ich wollte dich gerade besuchen!",
  "Hallo, *! Schön ruhig hier, oder?",
  "Hey, *! Fangen spielen?",
  "Da ist ja *! Hallo!",
  "Hallo, *! Heute sind wir Parkfreunde.",
  "*! Mit dir ist der Park viel schöner.",
  "Hallo, *! Riechst du die frische Luft?",
  "Pssst, *! Ich hab was Tolles gesehen.",
  "Hallo, *! Wollen wir zusammen sitzen?",
  "Endlich, *! Mir war schon langweilig.",
  "*! Du bist mein Lieblingsbesuch.",
  "Hallo, *! Heute ist ein guter Tag.",
  "Oh, *! Ich hab mich gerade ausgeruht.",
  "Hallo, *! Komm, ich zeig dir was.",
  "Na, *, auch ein bisschen frische Luft?",
  "Hallo, *! Wie lieb, dass du kommst.",
  "Juchhu, *! Jetzt sind wir zu zweit.",
  "Hallo, *! Erzählst du mir was?",
  "*! Wollen wir um die Wette hüpfen?",
  "*! Hier ist ein schönes Plätzchen.",
  "Ach, *! Du kommst wie gerufen.",
  "Tschüss Langeweile, hallo *!",
};
static constexpr int PARK_GRUESSE = sizeof(PARK_GRUSS_TEXT) / sizeof(PARK_GRUSS_TEXT[0]);
static_assert(PARK_GRUESSE == 50, "50 Begrüßungen");

// Wer gerade im Park ist. Lotti (Evis Freundin) etwa jedes dritte Mal, sonst eine der anderen 19.
// Seit 26.11.2026 kommt von diesen jedes dritte Mal jemand, der zu Evis Wesen passt (parkfreunde.h).
int parkBesucher(ms_t jetzt) {
  char schluessel[40];
  snprintf(schluessel, sizeof(schluessel), "%ld:park:besuch:%d", (long)tagKennung(jetzt),
           stundeVon(jetzt) / PARK_BESUCH_STUNDEN);
  const uint32_t h = streuwert(schluessel);
  if (h % 3 == 0) return 0;
  const uint32_t r = h / 3;
  if (r % 3 == 0) {
    int passend[BESUCHER], n = 0;
    for (int i = 1; i < BESUCHER; i++) if (FIGUR_WESEN[i] == eviWesen()) passend[n++] = i;
    if (n) return passend[(r / 3) % n];
  }
  return 1 + (int)(r % (BESUCHER - 1));
}

// Der Besuch hat dieselbe Lebensphase wie Evi.
static const BesuchBild& parkBesuchBild(Phase ph) { return BESUCH_BILD[park.besuch][ph - BABY]; }

// Bildschirmlage des Besuchs (ohne STATUS_H): das 72 x 64 große Bild um 10 % vergrößert,
// mittig auf PARK_BESUCH_X, Standlinie auf PARK_BESUCH_FUSS.
static ParkRahmen parkBesuchRahmen(Phase ph) {
  const BesuchBild& b = parkBesuchBild(ph);
  ParkRahmen r;
  r.ursprungX = PARK_BESUCH_X - BESUCH_BREITE / 2 * PARK_GROESSE_Z / PARK_GROESSE_N;
  r.ursprungY = PARK_BESUCH_FUSS - ((BESUCH_STANDLINIE + 1) * PARK_GROESSE_Z - 1) / PARK_GROESSE_N;
  r.links = r.ursprungX + (b.x * PARK_GROESSE_Z + PARK_GROESSE_N - 1) / PARK_GROESSE_N;
  r.rechts = r.ursprungX + ((b.x + b.bild.w) * PARK_GROESSE_Z + PARK_GROESSE_N - 1) / PARK_GROESSE_N - 1;
  r.oben = r.ursprungY + (b.y * PARK_GROESSE_Z + PARK_GROESSE_N - 1) / PARK_GROESSE_N;
  return r;
}

// Evi bleibt links vor dem Besuch stehen, auch wenn er breit ist (Tetro, Mimi).
static ParkPunkt parkAnkunft(ParkZiel ziel, Phase ph) {
  if (ziel == PARK_ZIEL_DORF) return PARK_DORF_ANKUNFT;
  if (ziel != PARK_ZIEL_BESUCH) return PARK_ANKUNFT[ziel];
  return {min(155, parkBesuchRahmen(ph).links - 12), 135};
}

// Neuer Satz, nie zweimal hintereinander derselbe. Dazu zählt das Treffen und der zweite Satz wird gewählt
// (parkfreunde.ino). Fremde werden anders begrüßt als Bekannte, Vertraute und beste Freunde.
static void parkGrussWaehlen() {
  static int letzter = -1;
  parkFreundBegegnung(jetztMs());
  parkNachsatzWaehlen(jetztMs());
  park.zweiter = 0;
  if (park.stufe == 0 || random(0, 2) == 0) {
    park.grussArt = 1;
    park.gruss = random(0, 5);
    return;
  }
  park.grussArt = 0;
  int neu = random(PARK_GRUESSE - (letzter >= 0 ? 1 : 0));
  if (letzter >= 0 && neu >= letzter) neu++;
  park.gruss = neu;
  letzter = neu;
}

static void parkGrussText(char* ziel, size_t platz) {
  if (park.zweiter == 2) { parkGeschenkText(ziel, platz); return; }
  const char* satz = park.zweiter ? parkNachsatz() : park.grussArt == 2 ? GRUSS_HAUS[park.gruss] : park.grussArt ? parkStufenGruss(park.stufe, park.gruss) : PARK_GRUSS_TEXT[park.gruss];
  parkBesuchSatz(ziel, platz, satz);
}

bool parkOffen() { return park.phase != PARK_ZU; }

bool parkMoeglich(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
  return ph >= BABY && ph <= OMA && !schlaeft(pet, jetzt) && !evi.schluepfen && !ritual.aktiv;
}

void parkSchliessen() {
  if (!parkOffen()) return;
  if (park.phase == PARK_GRUSS && park.geschenk && park.zweiter < 2) {   // Geschenk wurde schon gegeben: wenigstens sagen
    static char geschenk[64];
    parkGeschenkText(geschenk, sizeof(geschenk));
    park = ParkZustand();
    zeigeMeldung(geschenk, 4000);
  }
  park = ParkZustand();
  momentStoppen(true);
  moment.ruheBisTakt = evi.takt + RUHE_TAKTE;
}

void parkOeffnen(ms_t jetzt) {
  if (!parkMoeglich(jetzt)) { tamaRuht(); return; }
  if (parkZuNass(jetzt)) {
    static char nass[64];
    snprintf(nass, sizeof(nass), "%s mag bei Regen nicht raus.", tamaName());
    zeigeMeldung(nass, 3500);
    return;
  }
  momentStoppen(true);
  menueZeigen(false);
  kuschel = {};
  evi.reaktion = R_KEINE;
  sagen.bis = 0;
  park = ParkZustand();
  park.phase = PARK_WAHL;
  park.besuch = parkBesucher(jetzt);
}

const Fundstueck* parkLetzterFund() {
  for (int i = sammlung.anzahl - 1; i >= 0; i--)
    if (sammlung.fund[i].ort == FUND_PARK) return &sammlung.fund[i];
  return nullptr;
}

int parkFundstelle(ms_t jetzt) {
  const int32_t tag = tagKennung(jetzt);
  const Fundstueck* letzter = parkLetzterFund();
  if (!letzter) return PARK_KIESWEG;
  // Der Fund speichert zugleich die tägliche Suche, auch wenn die Uhr zurückgestellt wird.
  for (int i = sammlung.anzahl - 1; i >= 0; i--)
    if (sammlung.fund[i].ort == FUND_PARK && sammlung.fund[i].tag == tag) return -1;
  char schluessel[32];
  snprintf(schluessel, sizeof(schluessel), "%ld:park:stelle", (long)tag);
  return streuwert(schluessel) % 2;
}

static void parkGehen(ParkPunkt nach, uint32_t m) {
  park.von = park.fuesse;
  park.nach = nach;
  if (nach.x != park.von.x) park.rechts = nach.x > park.von.x;
  park.seit = m;
  park.dauer = max(140, (abs(nach.x - park.von.x) + abs(nach.y - park.von.y)) * 7);
}

static void parkZielStarten(ParkZiel ziel) {
  park.ziel = ziel;
  park.fund = -1;
  park.phase = PARK_LAEUFT;
  park.amWegpunkt = false;
  parkGehen(PARK_WEGPUNKT, millis());
}

static Fundart parkFundart(ms_t jetzt) {
  const Fundstueck* letzter = parkLetzterFund();
  if (!letzter) return FUND_STEIN;
  const int32_t tag = tagKennung(jetzt);
  char schluessel[32];
  snprintf(schluessel, sizeof(schluessel), "%ld:park:fund", (long)tag);
  return (Fundart)((letzter->art + 1 + streuwert(schluessel) % (FUNDARTEN - 1)) % FUNDARTEN);
}

static void parkUntersuchen(ms_t jetzt) {
  park.phase = PARK_FUND;
  if (parkFundstelle(jetzt) != park.ziel) return;
  const int32_t tag = tagKennung(jetzt);
  const Fundart art = parkFundart(jetzt);
  if (fundAufnehmen(art, FUND_PARK, tag, pet.generation, wetterBei(jetzt), jahreszeitBei(jetzt), park.ziel)) {
    park.fund = sammlung.anzahl - 1;
    speichern(jetzt);
    piep(TON(TON_MUENZE));
  }
}

static void parkHelfen() {
  park.phase = PARK_SUCHT;
  park.seit = millis();
}

void parkTakt(ms_t jetzt, uint32_t m) {
  if (!parkOffen()) return;
  if (park.phase == PARK_NAME) return;     // die Namenseingabe bleibt offen, bis ein Name da ist
  if (park.phase == PARK_STAMM) { if (!stammMoeglich(jetzt)) parkSchliessen(); else stammTakt(); return; }   // geht auch für Ei und verstorbene Evi
  if (!parkMoeglich(jetzt)) { parkSchliessen(); return; }
  if (park.phase != PARK_ALBUM && parkZuNass(jetzt)) {
    static char nass[64];
    snprintf(nass, sizeof(nass), "%s will heim, es regnet!", tamaName());
    parkSchliessen();
    zeigeMeldung(nass, 4000);
    return;
  }
  const uint32_t seit = m - park.seit;
  if (park.phase == PARK_LAEUFT) {
    const int fortschritt = (int)min(seit, park.dauer);
    park.fuesse.x = park.von.x + (park.nach.x - park.von.x) * fortschritt / (int)park.dauer;
    park.fuesse.y = park.von.y + (park.nach.y - park.von.y) * fortschritt / (int)park.dauer;
    if (seit >= park.dauer) {
      if (!park.amWegpunkt) {
        park.amWegpunkt = true;
        parkGehen(parkAnkunft(park.ziel, phaseBei(pet, jetzt)), m);
      } else {
        park.seit = m;
        park.phase = park.ziel == PARK_ZIEL_BESUCH ? PARK_GRUSS : PARK_SCHNUPPERT;
        if (park.phase == PARK_GRUSS) { park.rechts = true; parkGrussWaehlen(); }
        if (park.ziel == PARK_ZIEL_DORF) { park.rechts = true; dorfOeffnen(jetzt); }
      }
    }
  } else if (park.phase == PARK_SCHNUPPERT && seit >= 650) {
    park.phase = PARK_HILFE;
  } else if (park.phase == PARK_SUCHT && seit >= 500) {
    parkUntersuchen(jetzt);
  }
}

void parkKnopf(Knopf k) {
  if (park.phase == PARK_NAME) { nameKnopf(k); return; }
  if (park.phase == PARK_DORF || park.phase == PARK_BESUCHT) { dorfKnopf(k); return; }   // Namenseingabe: B löscht, START ist Fertig
  if (k == K_B || k == K_START) { parkSchliessen(); return; }
  if (park.phase == PARK_ALBUM) { albumKnopf(k); return; }
  if (park.phase == PARK_STAMM) { stammKnopf(k); return; }
  if (k == K_A) {
    if (park.phase == PARK_WAHL) parkZielStarten(park.ziel);
    else if (park.phase == PARK_HILFE) parkHelfen();
    else if (park.phase == PARK_GRUSS && park.zweiter == 0) park.zweiter = 1;   // erst der zweite Satz, dann ein Geschenk, dann weiter
    else if (park.phase == PARK_GRUSS && park.zweiter == 1 && park.geschenk) { park.zweiter = 2; piep(TON(TON_MUENZE)); }
    else if (park.phase == PARK_GRUSS || park.phase == PARK_FUND) { park.zweiter = 0; park.geschenk = 0; park.phase = PARK_WAHL; }
    return;
  }
  if (park.phase != PARK_WAHL) return;
  const int delta = k == K_UP || k == K_L ? -1 : k == K_DOWN || k == K_R ? 1 : 0;
  park.ziel = (ParkZiel)((park.ziel + PARK_ZIELE + delta) % PARK_ZIELE);
}

void parkTouch(int x, int y) {
  if (park.phase == PARK_NAME) { nameTouch(x, y); return; }
  if (park.phase == PARK_DORF || park.phase == PARK_BESUCHT) { dorfTouch(x, y); return; }
  if (park.phase == PARK_ALBUM) { albumTouch(x, y); return; }
  if (park.phase == PARK_STAMM) { stammTouch(x, y); return; }
  if (y >= ZEILE_Y) { parkKnopf(x >= 240 ? K_B : K_A); return; }
  y -= STATUS_H;
  if (y < 0 || park.phase == PARK_LAEUFT || park.phase == PARK_SCHNUPPERT || park.phase == PARK_SUCHT) return;
  ParkZiel ziel = PARK_ZIELE;
  const ParkRahmen besuch = parkBesuchRahmen(phaseBei(pet, jetztMs()));
  if (x >= PARK_DORF_X0 && x <= PARK_DORF_X1 && y >= PARK_DORF_Y0 && y <= PARK_DORF_Y1) ziel = PARK_ZIEL_DORF;
  else if (x >= besuch.links - 4 && x <= besuch.rechts + 4 && y >= besuch.oben - 4 && y <= PARK_BESUCH_FUSS + 3)
    ziel = PARK_ZIEL_BESUCH;
  else if (abs(x - PARK_FUNDPLATZ[0].x) <= 21 && abs(y - PARK_FUNDPLATZ[0].y) <= 13) ziel = PARK_KIESWEG;
  else if (abs(x - PARK_FUNDPLATZ[1].x) <= 23 && abs(y - PARK_FUNDPLATZ[1].y) <= 20) ziel = PARK_WIESE;
  if (ziel == PARK_ZIELE) return;
  if (park.phase == PARK_HILFE && ziel == park.ziel) parkHelfen();
  else parkZielStarten(ziel);
}

static bool parkIstHimmel(const uint8_t* maske, int x, int y) {
  const int i = y * BREITE + x;
  return y < PARK_HIMMEL_H && (maske[i >> 3] & (128 >> (i & 7)));
}

static void parkHintergrund(Jahreszeit saison, Himmel h, Wetter w) {
  const ZimmerBild& grund = *PARK_BODEN[saison];
  const uint8_t* himmelmaske = PARK_HIMMEL[saison];
  const bool trueb = w == WT_BEWOELKT || w == WT_REGEN || w == WT_SCHNEE;
  const int erde = (h == HIMMEL_NACHT ? 7 : h == HIMMEL_DAEMMERUNG ? 11 : 15) - (trueb ? 1 : 0);
  uint8_t* puffer = bild.getBuffer() + STATUS_H * BREITE;
  for (int y = 0; y < grund.h; y++) {
    for (int x = 0; x < grund.w; x++) {
      const uint8_t stufe = grauStufe(grund.grau, grund.w, x, y);
      int wert = (stufe * erde + 7) / 15;
      if (parkIstHimmel(himmelmaske, x, y)) {
        // Nachts durchgehend schwarz, tags sonnig fast weiß. Wolken/Regen
        // bleiben heller als zuvor; die Landschaft behält ihre 16 Graustufen.
        if (h == HIMMEL_NACHT) wert = 0;
        else if (h == HIMMEL_DAEMMERUNG) wert = (trueb ? 5 : 7) + y / 35;
        else if (w == WT_SONNE) wert = 15;
        else if (w == WT_SONNE_WOLKEN) wert = max(13, (int)stufe);
        else wert = (w == WT_REGEN ? 10 : 11) + (stufe == 15 ? 1 : 0);
      }
      *puffer++ = min(15, wert) * 17;
    }
  }
  bild.markieren();
  if (trueb || h == HIMMEL_DAEMMERUNG) return;
  const bool nacht = h == HIMMEL_NACHT;
  for (int y = 13; y <= 35; y++) for (int x = 253; x <= 277; x++) {
    const int dx = x - 265, dy = y - 24;
    if (dx * dx + dy * dy > 100 || !parkIstHimmel(himmelmaske, x, y)) continue;
    if (!nacht || (dx - 5) * (dx - 5) + (dy + 4) * (dy + 4) > 81)
      bild.drawPixel(x, STATUS_H + y, nacht ? 238 : dx * dx + dy * dy >= 81 ? 204 : 255);
  }
  if (nacht) {
    const ParkPunkt sterne[] = {{12, 12}, {22, 36}, {162, 15}, {193, 31}, {222, 9}, {241, 39}, {291, 10}, {301, 32}};
    for (const auto& s : sterne)
      if (parkIstHimmel(himmelmaske, s.x, s.y)) bild.drawPixel(s.x, STATUS_H + s.y, 221);
  }
}

// Fenster sind eine getrennte Lichtebene, keine neuen auswählbaren Parkziele.
static bool parkLichtAn(Himmel h, Wetter w) {
  return h != HIMMEL_TAG || w == WT_REGEN;
}

static void parkLichtFleck(int cx, int cy, int rx, int ry, int staerke) {
  const int aussen = rx * rx * ry * ry;
  for (int dy = -ry; dy <= ry; dy++) for (int dx = -rx; dx <= rx; dx++) {
    const int abstand = dx * dx * ry * ry + dy * dy * rx * rx;
    if (abstand > aussen) continue;
    const int x = cx + dx, y = cy + dy;
    if (x < 0 || x >= BREITE || y < 0 || y >= ZEILE_Y - STATUS_H) continue;
    // Heller Kern, abgestufter Rand. Die Wegtextur wird aufgehellt, nicht übermalt.
    if (abstand * 5 > aussen * 4 && (x * 13 + y * 7) % 3) continue;
    const int zusatz = 17 * (1 + (staerke / 17 - 1) * (aussen - abstand) / aussen);
    uint8_t& pixel = bild.getBuffer()[(STATUS_H + y) * BREITE + x];
    pixel = min(255, (int)pixel + zusatz);
  }
}

// Zeichnungen mit Graustufen und Deckkraft statt der früheren Strich-Platzhalter.
static void parkLaterneZeichnen(int x, int fuss, int hoehe, Jahreszeit saison,
                                Himmel h, bool an, bool weihnacht) {
  const ZimmerBild& b = PARK_LATERNE;
  const int breite = max(1, b.w * hoehe / b.h), links = x - breite / 2;
  const int oben = fuss - hoehe + 1;
  for (int y = 0; y < hoehe; y++) for (int dx = 0; dx < breite; dx++) {
    const int sx = dx * b.w / breite, sy = y * b.h / hoehe;
    if (!b.deckt(sx, sy)) continue;
    const int i = sy * b.w + sx;
    const bool glas = PARK_LATERNENGLAS[i >> 3] & (128 >> (i & 7));
    const int grund = b.wert(sx, sy);
    int ton = h == HIMMEL_NACHT ? max(51, grund * 9 / 15 / 17 * 17) : grund;
    if (an && glas) ton = max(238, grund);
    // Schneedecke auf den hellen oberen Dachflächen, passend zur Form des Dachs.
    if (saison == JZ_WINTER && sy * 100 >= b.h * 6 && sy * 100 <= b.h * 14 && grund >= 153)
      ton = an ? 255 : 238;
    bild.drawPixel(links + dx, STATUS_H + oben + y, ton);
  }
  if (weihnacht) {
    const ZimmerBild& stern = PARK_STERN;
    const int groesse = max(7, hoehe / 5);
    const int sw = max(1, stern.w * groesse / stern.h);
    const int sx0 = x + max(5, breite / 2) + 2;
    const int sy0 = oben + hoehe * 30 / 100;
    const int mitte = sx0 + sw / 2;
    const uint8_t metall = h == HIMMEL_NACHT ? 119 : 85;
    bild.drawFastHLine(x + 1, STATUS_H + sy0 - 4, mitte - x, metall);
    bild.drawFastVLine(mitte, STATUS_H + sy0 - 4, 5, metall);
    for (int sy = 0; sy < groesse; sy++) for (int sx = 0; sx < sw; sx++) {
      const int px = sx * stern.w / sw, py = sy * stern.h / groesse;
      if (!stern.deckt(px, py)) continue;
      const int g = stern.wert(px, py);
      bild.drawPixel(sx0 + sx, STATUS_H + sy0 + sy, an ? max(170, g) : g);
    }
  }
}

static void parkBeleuchtung(Jahreszeit saison, Himmel h, Wetter w, bool weihnacht) {
  const bool an = parkLichtAn(h, w);
  if (an) {
    // Je ein kleines Fenster pro Haus; dieselben festen Bildkoordinaten in
    // allen vier Jahreszeiten. Ohne großen Halo über den winzigen Häusern.
    for (const ParkPunkt& p : {ParkPunkt{134, 73}, ParkPunkt{173, 74}, ParkPunkt{183, 70}})
      bild.fillRect(p.x, STATUS_H + p.y, 2, 2, 255);
  }
  // Laternen als eigenständige Elemente: mit der Tiefe kleiner, am Weg verankert.
  const int lampen[][5] = {{162, 83, 34, 151, 84}, {196, 96, 52, 184, 98}, {50, 118, 72, 68, 120}};
  for (const auto& l : lampen) {
    const int x = l[0], fuss = l[1], hoehe = l[2];
    const int kopf = fuss - hoehe;
    if (an) {
      const int licht = h == HIMMEL_TAG ? 68 : 102;
      parkLichtFleck(l[3], l[4], hoehe / 2, max(3, hoehe / 8), licht);
      parkLichtFleck(x, kopf + hoehe / 5, max(4, hoehe / 7), max(5, hoehe / 6), 51);
    }
    parkLaterneZeichnen(x, fuss, hoehe, saison, h, an, weihnacht);
  }
  bild.markieren();
}

static void parkNiederschlag(Wetter w, Himmel h, uint32_t m) {
  if (w != WT_REGEN && w != WT_SCHNEE) return;
  const bool regen = w == WT_REGEN;
  const int schritt = m / (regen ? 24 : 85) % 11040;
  const int ton = h == HIMMEL_TAG ? (regen ? 136 : 255) : (regen ? 204 : 238);
  for (int i = 0; i < (regen ? 110 : 75); i++) {
    const int fall = (i * 47 + i * i * 13 + schritt * (regen ? 4 : 1)) % 184;
    const int x = (i * 137 + i * i * 17 + BREITE - fall / (regen ? 3 : 12)) % BREITE;
    const int y = STATUS_H + fall - 5;
    if (y >= STATUS_H && y + (regen ? 3 : 0) < ZEILE_Y) {
      if (regen) bild.drawLine(x, y, x - 1, y + 3, ton);
      else bild.drawPixel(x, y, i % 4 ? ton : ton - 34);
    }
  }
}

static void parkGegenstand(Fundart art, int cx, int fuss, int groesse) {
  const Bild& b = *FUND_BILD[art];
  const int mass = max(b.w, b.h);
  const int w = max(1, b.w * groesse / mass), h = max(1, b.h * groesse / mass);
  raumRasterZeichnen(b, cx - w / 2, fuss - h + 1, w, h, false, false, true, fuss);
}

// Der Besuch, Pixel für Pixel aus der Zeichnung (nur um 10 % vergrößert, wie Lotti bisher).
static void parkBesuchZeichnen(Phase ph) {
  const BesuchBild& b = parkBesuchBild(ph);
  const ParkRahmen r = parkBesuchRahmen(ph);
  ellipseFuellen(PARK_BESUCH_X, STATUS_H + PARK_BESUCH_FUSS, max(10, (r.rechts - r.links) / 4), 1, 119);
  for (int y = r.oben; y <= PARK_BESUCH_FUSS; y++) {
    const int sy = (y - r.ursprungY) * PARK_GROESSE_N / PARK_GROESSE_Z - b.y;
    if (sy < 0 || sy >= b.bild.h) continue;
    for (int x = r.links; x <= r.rechts; x++) {
      const int sx = (x - r.ursprungX) * PARK_GROESSE_N / PARK_GROESSE_Z - b.x;
      if (sx >= 0 && sx < b.bild.w && b.bild.deckt(sx, sy))
        bild.drawPixel(x, STATUS_H + y, b.bild.wert(sx, sy));
    }
  }
}

static void parkFigurZeichnen(Phase ph, uint32_t m) {
  const int fuss = STATUS_H + park.fuesse.y;
  const bool gefunden = park.phase == PARK_FUND && park.fund >= 0;
  const int pose = gefunden ? 2 : park.phase == PARK_SUCHT ? 1 :
                   park.phase == PARK_SCHNUPPERT || park.phase == PARK_HILFE ? 0 : -1;
  if (pose >= 0) {
    const ZimmerBild& e = *PARK_EVI[ph - BABY][pose];
    const int basis[] = {70, 82, 86, 82};
    const int h = max(1, raumFigurHoehe(*ART[ph][1], ph, fuss, true, false) * 4 * PARK_GROESSE_Z * e.h /
                                  (5 * PARK_GROESSE_N * basis[ph - BABY]));
    const int w = max(1, e.w * h / e.h);
    ellipseFuellen(park.fuesse.x, fuss, max(3, w / 5), 1, 119);
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
      const int sx = park.rechts ? e.w - 1 - x * e.w / w : x * e.w / w;
      const int sy = y * e.h / h;
      if (e.deckt(sx, sy)) bild.drawPixel(park.fuesse.x - w / 2 + x, fuss - h + 1 + y, e.wert(sx, sy));
    }
    if (gefunden) parkGegenstand((Fundart)sammlung.fund[park.fund].art, park.fuesse.x, fuss - h / 3 + 10, 23);
  } else {
    const bool unterwegs = park.phase == PARK_LAEUFT;
    const Bild& e = unterwegs ? *MOMENT_BILD[ph][MB_LAUFEN] : *ART[ph][1];
    const int h = max(1, raumFigurHoehe(e, ph, fuss, true, false) * 4 * PARK_GROESSE_Z / (5 * PARK_GROESSE_N));
    const int w = max(1, e.w * h / e.h);
    ellipseFuellen(park.fuesse.x, fuss, max(3, w / 5), 1, 119);
    raumRasterZeichnen(e, park.fuesse.x - w / 2, fuss - h + 1 - (unterwegs ? m / 125 % 2 * 2 : 0),
                       w, h, park.rechts, false, true, fuss);
  }
}

void parkZeichnen(ms_t jetzt) {
  if (park.phase == PARK_NAME) { nameZeichnen(jetzt); return; }
  if (park.phase == PARK_DORF || park.phase == PARK_BESUCHT) { dorfZeichnen(jetzt); return; }
  if (park.phase == PARK_ALBUM) { albumZeichnen(jetzt); return; }
  if (park.phase == PARK_STAMM) { stammZeichnen(jetzt); return; }
  const Phase ph = phaseBei(pet, jetzt);
  if (ph < BABY || ph > OMA) { parkSchliessen(); return; }
  const Himmel h = himmelBei(jetzt);
  const Wetter w = wetterBei(jetzt);
  const uint32_t m = millis();
  const Jahreszeit saison = jahreszeitBei(jetzt);
  parkHintergrund(saison, h, w);
  parkBeleuchtung(saison, h, w, weihnachtsZeit(jetzt));
  parkNiederschlag(w, h, m);
  parkBesuchZeichnen(ph);
  const int stelle = parkFundstelle(jetzt);
  if (stelle >= 0 && sammlung.anzahl < SAMMLUNG_MAX) {
    const auto& pos = PARK_FUNDPLATZ[stelle];
    parkGegenstand(parkFundart(jetzt), pos.x, STATUS_H + pos.y, 14);
    if ((m / 330) % 3 != 2) {
      bild.drawFastHLine(pos.x + 8, STATUS_H + pos.y - 11, 5, 238);
      bild.drawFastVLine(pos.x + 10, STATUS_H + pos.y - 13, 5, 238);
    }
  }
  parkFigurZeichnen(ph, m);
  if (park.phase == PARK_WAHL && tastenDa) {
    const auto& ziel = park.ziel == PARK_ZIEL_BESUCH ? ParkPunkt{PARK_BESUCH_X, parkBesuchRahmen(ph).oben - 4} :
                       park.ziel == PARK_ZIEL_DORF ? ParkPunkt{(PARK_DORF_X0 + PARK_DORF_X1) / 2, PARK_DORF_Y0 + 8} : PARK_FUNDPLATZ[park.ziel];
    const int y = STATUS_H + ziel.y - 17;
    bild.fillTriangle(ziel.x - 4, y, ziel.x + 4, y, ziel.x, y + 5, 0);
  }
  zeichneStatus(ph);
  bild.fillRect(0, ZEILE_Y, BREITE, HOEHE - ZEILE_Y, 1);
  bild.drawFastHLine(0, ZEILE_Y, BREITE, 0);
  const char* zieltext = park.ziel == PARK_ZIEL_BESUCH ? besuchName(park.besuch) : park.ziel == PARK_ZIEL_DORF ? "Zum Dorf" : PARK_STELLE[park.ziel];
  char grusstext[64];
  const char* text = zieltext;
  const char* aktion = tastenDa ? "A Losgehen" : "Ziel antippen";
  switch (park.phase) {
    case PARK_LAEUFT: snprintf(grusstext, sizeof(grusstext), "%s läuft los …", tamaName()); text = grusstext; aktion = ""; break;
    case PARK_SCHNUPPERT: snprintf(grusstext, sizeof(grusstext), "%s schnuppert …", tamaName()); text = grusstext; aktion = ""; break;
    case PARK_HILFE: text = "Was ist da?"; aktion = tastenDa ? "A Nachsehen" : "Nachsehen"; break;
    case PARK_SUCHT: snprintf(grusstext, sizeof(grusstext), "%s schaut nach …", tamaName()); text = grusstext; aktion = ""; break;
    case PARK_GRUSS: parkGrussText(grusstext, sizeof(grusstext)); text = grusstext; aktion = tastenDa ? "A Weiter" : "Weiter"; break;
    case PARK_FUND:
      text = park.fund >= 0 ? FUND_NAME[sammlung.fund[park.fund].art] :
             sammlung.anzahl >= SAMMLUNG_MAX ? "Das Sideboard ist voll." : "Hier liegt heute nichts.";
      aktion = tastenDa ? "A Weiter" : "Weiter";
      break;
    default: break;
  }
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 6, text, 0);
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 22, aktion, 0);
  schreibe(bild, 242, ZEILE_Y + 22, tastenDa ? "B Nach Hause" : "Nach Hause", 0);
  zeigen();
}
