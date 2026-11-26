// Stammbaum (26.11.2026). Siehe stammbaum.h.

// Nur männliche Figuren kommen als Vater in Frage.
// Bit je Figur (Reihenfolge BESUCH_NAME): Bit, Blip, Blubber, Funke, Groschen, Leo, Linus, Mampf, Rolli, Tetro
const uint32_t STAMM_VAETER = (1UL << 1) | (1UL << 2) | (1UL << 3) | (1UL << 5) | (1UL << 7) | (1UL << 10) | (1UL << 11) |
                              (1UL << 13) | (1UL << 17) | (1UL << 19);

void stammLaden() {
  uint8_t* b = (uint8_t*)&stamm;
  for (size_t i = 0; i < sizeof(stamm); i++) b[i] = EEPROM.read(STAMM_ADRESSE + i);
  bool gut = stamm.kennung == STAMM_KENNUNG && stamm.version == STAMM_VERSION && stamm.groesse == sizeof(StammStand) &&
             stamm.anzahl <= STAMM_MAX && stamm.pruefsumme == pruefsumme(b, offsetof(StammStand, pruefsumme));
  if (!gut) {
    memset(&stamm, 0, sizeof(stamm));
    stamm.kennung = STAMM_KENNUNG;
    stamm.version = STAMM_VERSION;
    stamm.groesse = sizeof(StammStand);
    stamm.vaterAktuell = 255;
  }
  stammGeaendert = false;
  stammSeite = stamm.anzahl;
}

void stammSpeichern() {
  if (!stammGeaendert) return;
  stamm.pruefsumme = pruefsumme((const uint8_t*)&stamm, offsetof(StammStand, pruefsumme));
  const uint8_t* b = (const uint8_t*)&stamm;
  bool anders = false;
  for (size_t i = 0; i < sizeof(stamm); i++) {
    if (EEPROM.read(STAMM_ADRESSE + i) != b[i]) { EEPROM.write(STAMM_ADRESSE + i, b[i]); anders = true; }
  }
  if (anders) EEPROM.commit();
  stammGeaendert = false;
}

// Name kürzen, ohne einen Umlaut (zwei Bytes) in der Mitte abzuschneiden
void stammNameKuerzen(const char* von, char* nach, size_t platz) {
  size_t n = 0;
  for (const char* c = von; *c && n < platz - 1; c++) nach[n++] = *c;
  if (*(von + n) && n > 0) {                                  // es wurde abgeschnitten: nicht mitten im Zeichen
    while (n > 0 && ((uint8_t)von[n] & 0xC0) == 0x80) n--;
  }
  nach[n] = 0;
}

// Lebensphase nach der gelebten Zeit
Phase stammPhaseNachAlter(ms_t alter) {
  if (alter < R::BABY) return BABY;
  if (alter < R::BABY + R::TEEN) return TEEN;
  if (alter < R::BABY + R::TEEN + R::ERWACHSEN) return ERWACHSEN;
  return OMA;
}

// Männliche Figur mit den meisten Treffen, 255 ohne Treffen
int stammBesterVater(ms_t evi) {
  if (parkFreund.evi != evi) return 255;     // die Treffen gehören zu einer anderen Evi
  int best = 255, meist = 0;
  for (int i = 0; i < BESUCHER; i++)
    if ((STAMM_VAETER >> i & 1) && parkFreund.treffer[i] > meist) { meist = parkFreund.treffer[i]; best = i; }
  return best;
}

// Die verstorbene Evi kommt in den Stammbaum, und ihr bester männlicher Parkfreund wird Vater der neuen Evi.
// Muss aufgerufen werden, solange die Treffen der alten Evi noch gespeichert sind (aktionAusfuehren, NEUES_EI).
void stammEintragen(const Tier& alt, ms_t jetzt) {
  // Achtung: Die Treffen der alten Evi dürfen hier noch nicht zurückgesetzt sein (parkFreundPruefen erst später).
  StammEintrag e;
  memset(&e, 0, sizeof(e));
  stammNameKuerzen(alt.name, e.name, sizeof(e.name));
  if (!e.name[0]) strcpy(e.name, "Evi");
  const ms_t ende = alt.tot && alt.gestorben > alt.schluepft ? alt.gestorben : jetzt;
  const ms_t gelebt = ende > alt.schluepft ? ende - alt.schluepft : 0;
  e.wesen = (uint8_t)wesenVon(alt.geboren);
  e.vater = stamm.vaterAktuell;
  e.phase = (uint8_t)stammPhaseNachAlter(gelebt);
  e.generation = (uint8_t)min((int)alt.generation, 255);
  e.alterStd = (uint16_t)min((long long)(gelebt / STUNDE), 65535LL);
  if (stamm.anzahl >= STAMM_MAX) {
    for (int i = 1; i < STAMM_MAX; i++) stamm.eintrag[i - 1] = stamm.eintrag[i];
    stamm.anzahl = STAMM_MAX - 1;
  }
  stamm.eintrag[stamm.anzahl++] = e;
  stamm.vaterAktuell = (uint8_t)stammBesterVater(alt.geboren);
  stammGeaendert = true;
  stammSeite = stamm.anzahl;
  speichern(jetzt);
}

// Der Stammbaum geht auch bei Ei und verstorbener Evi, sonst nur wenn Evi wach und nicht beschäftigt ist.
bool stammMoeglich(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
  if (ph == TOT || ph == EI) return true;
  return ph >= BABY && ph <= OMA && !schlaeft(pet, jetzt) && !evi.schluepfen && !ritual.aktiv;
}

void stammOeffnen(ms_t jetzt) {
  if (!stammMoeglich(jetzt)) { zeigeMeldung("Evi ruht gerade."); return; }
  momentStoppen(true);
  menueZeigen(false);
  kuschel = {};
  evi.reaktion = R_KEINE;
  sagen.bis = 0;
  park = ParkZustand();
  park.phase = PARK_STAMM;
  stammSeite = stamm.anzahl;
}

void stammKnopf(Knopf k) {
  const int n = stamm.anzahl + 1;
  if (k == K_L || k == K_UP) stammSeite = (stammSeite + n - 1) % n;
  else if (k == K_R || k == K_DOWN || k == K_A) stammSeite = (stammSeite + 1) % n;
}

void stammTouch(int x, int y) {
  if (y >= ZEILE_Y) { if (x >= 240) parkSchliessen(); else stammKnopf(K_R); return; }
  stammKnopf(x < BREITE / 2 ? K_L : K_R);
}

// Evis Bild in eine Fläche einpassen (Mitte, Unterkante)
static void stammEviBild(Phase ph, int cx, int unten, int maxB, int maxH) {
  const Bild* b = ph == EI ? ART[EI][0] : ART[ph][1];
  if (!b) return;
  int h = maxH, w = max(1, (int)b->w * h / b->h);
  if (w > maxB) { w = maxB; h = max(1, (int)b->h * w / b->w); }
  raumRasterZeichnen(*b, cx - w / 2, unten - h + 1, w, h, false, false, false, 0);
}

static void stammFragezeichen(int cx, int y) {
  bild.fillRoundRect(cx - 17, y, 34, 36, 6, 238);
  bild.drawRoundRect(cx - 17, y, 34, 36, 6, 170);
  schreibe(bild, cx - textBreite("?", 3) / 2, y + 7, "?", 0, 3);
}

static void stammMittig(int cx, int y, const char* text) {
  schreibe(bild, cx - textBreite(text) / 2, y, text, 0);
}

void stammAlterText(int std, bool lebt, char* aus, size_t n) {
  if (std >= 24) snprintf(aus, n, "%s %d Tage alt", lebt ? "ist" : "wurde", std / 24);
  else snprintf(aus, n, "%s %d Std. alt", lebt ? "ist" : "wurde", std);
}

void stammZeichnen(ms_t jetzt) {
  const Phase jetzt_ph = phaseBei(pet, jetzt);
  bild.fillScreen(1);
  zeichneStatus(jetzt_ph);
  const int n = stamm.anzahl + 1;
  if (stammSeite >= n) stammSeite = stamm.anzahl;
  const bool aktuell = stammSeite == stamm.anzahl;
  // Die angezeigte Evi: gespeichert (verstorben) oder die jetzige (lebt, ist Ei oder ist gerade gegangen)
  char name[24] = "";
  int wesen, vater, alterStd, generation;
  Phase phase;
  bool lebt;
  if (aktuell) {
    stammNameKuerzen(pet.name[0] ? pet.name : "Evi", name, 12);
    wesen = wesenVon(pet.geboren);
    vater = stamm.vaterAktuell;
    generation = pet.generation;
    lebt = !pet.tot;
    const ms_t ende = pet.tot && pet.gestorben > pet.schluepft ? pet.gestorben : jetzt;
    const ms_t gelebt = ende > pet.schluepft ? ende - pet.schluepft : 0;
    alterStd = (int)(gelebt / STUNDE);
    phase = pet.tot ? stammPhaseNachAlter(gelebt) : jetzt_ph;
  } else {
    const StammEintrag& e = stamm.eintrag[stammSeite];
    snprintf(name, sizeof(name), "%s", e.name);
    wesen = e.wesen < WESEN_ZAHL ? e.wesen : 0;
    vater = e.vater;
    generation = e.generation;
    alterStd = e.alterStd;
    phase = (Phase)(e.phase >= BABY && e.phase <= OMA ? (int)e.phase : (int)OMA);
    lebt = false;
  }
  // Mutter: die Evi davor (gespeichert), sonst unbekannt
  const bool hatMutter = stammSeite > 0;
  const StammEintrag* mutter = hatMutter ? &stamm.eintrag[stammSeite - 1] : nullptr;
  const int mx = 90, vx = 230, kx = 160;
  schreibe(bild, mx - textBreite("Mutter") / 2, STATUS_H + 3, "Mutter", 0);
  schreibe(bild, vx - textBreite("Vater") / 2, STATUS_H + 3, "Vater", 0);
  if (mutter) {
    const Phase mp = (Phase)(mutter->phase >= BABY && mutter->phase <= OMA ? (int)mutter->phase : (int)OMA);
    stammEviBild(mp, mx, STATUS_H + 51, 44, 38);
    stammMittig(mx, STATUS_H + 54, mutter->name);
    stammMittig(mx, STATUS_H + 64, WESEN_NAME[mutter->wesen < WESEN_ZAHL ? mutter->wesen : 0]);
  } else {
    stammFragezeichen(mx, STATUS_H + 15);
    stammMittig(mx, STATUS_H + 54, "unbekannt");
  }
  if (vater >= 0 && vater < BESUCHER) {
    albumFigur(vater, ERWACHSEN, vx - BESUCH_BREITE / 4, STATUS_H + 15, true);
    stammMittig(vx, STATUS_H + 54, BESUCH_NAME[vater]);
    stammMittig(vx, STATUS_H + 64, WESEN_NAME[FIGUR_WESEN[vater]]);
  } else {
    stammFragezeichen(vx, STATUS_H + 15);
    stammMittig(vx, STATUS_H + 54, "unbekannt");
  }
  // Verbindungslinien Eltern → Kind
  const int ly = STATUS_H + 76;
  bild.drawFastVLine(mx, ly, 6, 0); bild.drawFastVLine(vx, ly, 6, 0);
  bild.drawFastHLine(mx, ly + 6, vx - mx + 1, 0);
  bild.drawFastVLine(kx, ly + 6, 7, 0);
  stammEviBild(phase, kx, STATUS_H + 148, 90, 64);
  // Unten: Name, Wesen, Alter
  bild.fillRect(0, ZEILE_Y, BREITE, HOEHE - ZEILE_Y, 1);
  bild.drawFastHLine(0, ZEILE_Y, BREITE, 0);
  char alter[32], zeile1[64], zeile2[64];
  stammAlterText(alterStd, lebt, alter, sizeof(alter));
  snprintf(zeile1, sizeof(zeile1), "%s · %s · %s", name, WESEN_NAME[wesen], alter);
  snprintf(zeile2, sizeof(zeile2), "%d von %d · %s", stammSeite + 1, n, tastenDa ? "◀ Mutter  Kind ▶" : "Links Mutter, rechts Kind");
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 6, zeile1, 0);
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 22, zeile2, 0);
  schreibe(bild, 242, ZEILE_Y + 22, tastenDa ? "B Zurück" : "Zurück", 0);
  (void)generation;
  zeigen();
}
