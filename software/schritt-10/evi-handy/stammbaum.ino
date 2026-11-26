// Stammbaum (26.11.2026). Siehe stammbaum.h.


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
    stamm.elternAktuell = 255;
    stamm.elternNameAktuell = 255;
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

// Figur aus der Maske (Bit je Figur) mit den meisten Treffen, 255 ohne Treffen
int stammBesterElter(ms_t evi, uint32_t maske) {
  if (parkFreund.evi != evi) return 255;     // die Treffen gehören zu einem anderen Tamagotchi
  int best = 255, meist = 0;
  for (int i = 0; i < BESUCHER; i++)
    if ((maske >> i & 1) && parkFreund.treffer[i] > meist) { meist = parkFreund.treffer[i]; best = i; }
  return best;
}

// Das verstorbene Tamagotchi kommt in den Stammbaum. Es wird Elternteil des neuen; der andere Elternteil ist die
// Parkfreund-Figur des anderen Geschlechts, die es am häufigsten getroffen hat (Name wird gespeichert, weil er sich ändert).
// Muss aufgerufen werden, solange die Treffen des alten Tamagotchis noch gespeichert sind (aktionAusfuehren, NEUES_EI).
void stammEintragen(const Tier& alt, ms_t jetzt) {
  // Achtung: Die Treffen des alten Tamagotchis dürfen hier noch nicht zurückgesetzt sein (parkFreundPruefen erst später).
  StammEintrag e;
  memset(&e, 0, sizeof(e));
  stammNameKuerzen(alt.name[0] ? alt.name : "Tamagotchi", e.name, sizeof(e.name));
  const ms_t ende = alt.tot && alt.gestorben > alt.schluepft ? alt.gestorben : jetzt;
  const ms_t gelebt = ende > alt.schluepft ? ende - alt.schluepft : 0;
  e.wesen = (uint8_t)wesenVon(alt.geboren);
  e.geschlecht = stamm.geschlechtAktuell;
  e.eltern = stamm.elternAktuell;
  e.elternName = stamm.elternNameAktuell;
  e.phase = (uint8_t)stammPhaseNachAlter(gelebt);
  e.generation = (uint8_t)min((int)alt.generation, 255);
  e.alterStd = (uint16_t)min((long long)(gelebt / STUNDE), 65535LL);
  if (stamm.anzahl >= STAMM_MAX) {
    for (int i = 1; i < STAMM_MAX; i++) stamm.eintrag[i - 1] = stamm.eintrag[i];
    stamm.anzahl = STAMM_MAX - 1;
  }
  stamm.eintrag[stamm.anzahl++] = e;
  const int elter = stammBesterElter(alt.geboren, e.geschlecht ? FIGUR_WEIBLICH_MASKE : FIGUR_MAENNLICH_MASKE);
  stamm.elternAktuell = (uint8_t)elter;
  stamm.elternNameAktuell = elter == 255 ? 255 : besuchNameCode(elter, alt);
  stamm.geschlechtAktuell = 0;               // das Baby bekommt sein Geschlecht beim Benennen
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
  if (!stammMoeglich(jetzt)) { tamaRuht(); return; }
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

// Ein Elternteil auf seiner Seite: Tamagotchi (Bild der Lebensphase bei seinem Tod), Parkfreund (Figur) oder unbekannt
static void stammElter(int cx, const char* titel, const StammEintrag* evi, int figur, uint8_t nameCode) {
  schreibe(bild, cx - textBreite(titel) / 2, STATUS_H + 3, titel, 0);
  if (evi) {
    const Phase mp = (Phase)(evi->phase >= BABY && evi->phase <= OMA ? (int)evi->phase : (int)OMA);
    stammEviBild(mp, cx, STATUS_H + 51, 44, 38);
    stammMittig(cx, STATUS_H + 54, evi->name);
    stammMittig(cx, STATUS_H + 64, WESEN_NAME[evi->wesen < WESEN_ZAHL ? evi->wesen : 0]);
  } else if (figur >= 0 && figur < BESUCHER) {
    albumFigur(figur, ERWACHSEN, cx - BESUCH_BREITE / 4, STATUS_H + 15, true);
    stammMittig(cx, STATUS_H + 54, nameVonCode(nameCode));
    stammMittig(cx, STATUS_H + 64, WESEN_NAME[FIGUR_WESEN[figur]]);
  } else {
    stammFragezeichen(cx, STATUS_H + 15);
    stammMittig(cx, STATUS_H + 54, "unbekannt");
  }
}

void stammZeichnen(ms_t jetzt) {
  const Phase jetzt_ph = phaseBei(pet, jetzt);
  bild.fillScreen(1);
  zeichneStatus(jetzt_ph);
  const int n = stamm.anzahl + 1;
  if (stammSeite >= n) stammSeite = stamm.anzahl;
  const bool aktuell = stammSeite == stamm.anzahl;
  // Das angezeigte Tamagotchi: gespeichert (verstorben) oder das jetzige (lebt, ist Ei oder ist gerade gegangen)
  char name[24] = "";
  int wesen, eltern, alterStd, geschlecht;
  uint8_t elternName;
  Phase phase;
  bool lebt;
  if (aktuell) {
    stammNameKuerzen(pet.name[0] ? pet.name : "Neues Ei", name, 12);
    wesen = wesenVon(pet.geboren);
    eltern = stamm.elternAktuell;
    elternName = stamm.elternNameAktuell;
    geschlecht = stamm.geschlechtAktuell;
    lebt = !pet.tot;
    const ms_t ende = pet.tot && pet.gestorben > pet.schluepft ? pet.gestorben : jetzt;
    const ms_t gelebt = ende > pet.schluepft ? ende - pet.schluepft : 0;
    alterStd = (int)(gelebt / STUNDE);
    phase = pet.tot ? stammPhaseNachAlter(gelebt) : jetzt_ph;
  } else {
    const StammEintrag& e = stamm.eintrag[stammSeite];
    snprintf(name, sizeof(name), "%s", e.name);
    wesen = e.wesen < WESEN_ZAHL ? e.wesen : 0;
    eltern = e.eltern;
    elternName = e.elternName;
    geschlecht = e.geschlecht ? 1 : 0;
    alterStd = e.alterStd;
    phase = (Phase)(e.phase >= BABY && e.phase <= OMA ? (int)e.phase : (int)OMA);
    lebt = false;
  }
  // Elternteil 1: das Tamagotchi davor (gespeichert); Elternteil 2: die Parkfreund-Figur des anderen Geschlechts.
  // Mutter steht links, Vater rechts: das Geschlecht des Tamagotchis davor bestimmt die Seite.
  const StammEintrag* davor = stammSeite > 0 ? &stamm.eintrag[stammSeite - 1] : nullptr;
  bool davorLinks;                                   // true: das Tamagotchi davor ist die Mutter
  if (davor) davorLinks = davor->geschlecht == 0;
  else if (eltern >= 0 && eltern < BESUCHER) davorLinks = !figurWeiblich(eltern);   // Figur weiblich = Mutter links
  else davorLinks = true;
  const int links = 90, rechts = 230, kx = 160;
  const bool figurLinks = davor ? !davorLinks : (eltern >= 0 && eltern < BESUCHER ? figurWeiblich(eltern) : false);
  stammElter(links, "Mutter", davorLinks ? davor : nullptr, figurLinks ? eltern : -1, elternName);
  stammElter(rechts, "Vater", davorLinks ? nullptr : davor, !figurLinks ? eltern : -1, elternName);
  // Verbindungslinien Eltern → Kind
  const int ly = STATUS_H + 76;
  bild.drawFastVLine(links, ly, 6, 0); bild.drawFastVLine(rechts, ly, 6, 0);
  bild.drawFastHLine(links, ly + 6, rechts - links + 1, 0);
  bild.drawFastVLine(kx, ly + 6, 7, 0);
  stammEviBild(phase, kx, STATUS_H + 148, 90, 64);
  // Unten: Name, Geschlecht, Wesen, Alter
  bild.fillRect(0, ZEILE_Y, BREITE, HOEHE - ZEILE_Y, 1);
  bild.drawFastHLine(0, ZEILE_Y, BREITE, 0);
  char alter[32], zeile1[64], zeile2[64];
  stammAlterText(alterStd, lebt, alter, sizeof(alter));
  snprintf(zeile1, sizeof(zeile1), "%s · %s · %s", name, geschlecht ? "Junge" : "Mädchen", WESEN_NAME[wesen]);
  snprintf(zeile2, sizeof(zeile2), "%s · %d von %d%s", alter, stammSeite + 1, n, tastenDa ? " ◀▶" : "");
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 6, zeile1, 0);
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 22, zeile2, 0);
  schreibe(bild, 242, ZEILE_Y + 22, tastenDa ? "B Zurück" : "Zurück", 0);
  zeigen();
}
