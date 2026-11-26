// Stammbaum (26.11.2026). Siehe stammbaum.h.


void stammLaden() {
  uint8_t* b = (uint8_t*)&stamm;
  for (size_t i = 0; i < sizeof(stamm); i++) b[i] = EEPROM.read(STAMM_ADRESSE + i);
  bool gut = stamm.kennung == STAMM_KENNUNG && stamm.version == STAMM_VERSION && stamm.groesse == sizeof(StammStand) &&
             stamm.anzahl <= STAMM_MAX && stamm.pruefsumme == pruefsumme(b, offsetof(StammStand, pruefsumme));
  stammGeaendert = false;
  if (!gut) {
    memset(&stamm, 0, sizeof(stamm));
    stamm.kennung = STAMM_KENNUNG;
    stamm.version = STAMM_VERSION;
    stamm.groesse = sizeof(StammStand);
    stamm.elternAktuell = 255;
    stamm.elternNameAktuell = 255;
    // Version 2 (8 Einträge, alter Platz) übernehmen
    StammStandV2 alt;
    uint8_t* a = (uint8_t*)&alt;
    for (size_t i = 0; i < sizeof(alt); i++) a[i] = EEPROM.read(STAMM_ADRESSE_V2 + i);
    if (alt.kennung == STAMM_KENNUNG && alt.version == 2 && alt.groesse == sizeof(StammStandV2) && alt.anzahl <= 8 &&
        alt.pruefsumme == pruefsumme(a, offsetof(StammStandV2, pruefsumme))) {
      stamm.anzahl = alt.anzahl;
      for (int i = 0; i < alt.anzahl; i++) stamm.eintrag[i] = alt.eintrag[i];
      stamm.elternAktuell = alt.elternAktuell;
      stamm.elternNameAktuell = alt.elternNameAktuell;
      stamm.geschlechtAktuell = alt.geschlechtAktuell;
      stamm.neueNamenAb = alt.neueNamenAb;
      stammGeaendert = true;                 // am neuen Platz speichern
    }
  }
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
  speichern(jetzt);
}

// Der Stammbaum geht auch bei Ei und verstorbener Evi, sonst nur wenn Evi wach und nicht beschäftigt ist.
bool stammMoeglich(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
  if (ph == TOT || ph == EI) return true;
  return ph >= BABY && ph <= OMA && !schlaeft(pet, jetzt) && !evi.schluepfen && !ritual.aktiv;
}

// ---------- Ansicht: eine lange Fläche von oben nach unten ----------
static const int STAMM_TAKT_H = 176;      // Höhe einer Familie (Mutter, Vater, Kind) samt Abstand
static const int STAMM_SICHT_H = 174;     // sichtbare Höhe zwischen Statusleiste und Textzeile

int stammScrollMax() {
  const int n = stamm.anzahl + 1;
  return max(0, (n - 1) * STAMM_TAKT_H + 166 - STAMM_SICHT_H);
}

void stammScrollen(int um) { stammScroll = max(0, min(stammScrollMax(), stammScroll + um)); }

void stammOeffnen(ms_t jetzt) {
  if (!stammMoeglich(jetzt)) { tamaRuht(); return; }
  momentStoppen(true);
  menueZeigen(false);
  kuschel = {};
  evi.reaktion = R_KEINE;
  sagen.bis = 0;
  park = ParkZustand();
  park.phase = PARK_STAMM;
  stammZiehY = -1;
  stammScroll = stammScrollMax();           // ganz unten: das jetzige Tamagotchi
}

void stammKnopf(Knopf k) {
  if (k == K_UP || k == K_L) stammScrollen(-40);
  else if (k == K_DOWN || k == K_R) stammScrollen(40);
  else if (k == K_A) stammScrollen(STAMM_TAKT_H);                     // eine Familie weiter
}

void stammTouch(int x, int y) {
  if (y >= ZEILE_Y) {
    if (x >= 242) parkSchliessen();
    else if (x < 90) stammScrollen(-STAMM_TAKT_H / 2);
    else if (x < 180) stammScrollen(STAMM_TAKT_H / 2);
    return;
  }
  stammZiehY = y;                           // Finger aufgesetzt: ab jetzt zieht er die Ansicht
}

// Jeden Takt: Der Finger zieht die Ansicht (wie am Handy: Finger nach oben = weiter nach unten)
void stammTakt() {
  if (fingerUnten && stammZiehY >= 0) {
    stammScrollen(stammZiehY - fingerY);
    stammZiehY = fingerY;
  } else if (!fingerUnten) {
    stammZiehY = -1;
  }
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
static void stammElter(int cx, int y0, const char* titel, const StammEintrag* evi, int figur, uint8_t nameCode) {
  schreibe(bild, cx - textBreite(titel) / 2, y0, titel, 0);
  if (evi) {
    const Phase mp = (Phase)(evi->phase >= BABY && evi->phase <= OMA ? (int)evi->phase : (int)OMA);
    stammEviBild(mp, cx, y0 + 49, 44, 38);
    stammMittig(cx, y0 + 52, evi->name);
    stammMittig(cx, y0 + 62, WESEN_NAME[evi->wesen < WESEN_ZAHL ? evi->wesen : 0]);
  } else if (figur >= 0 && figur < BESUCHER) {
    albumFigur(figur, ERWACHSEN, cx - BESUCH_BREITE / 4, y0 + 12, true);
    stammMittig(cx, y0 + 52, nameVonCode(nameCode));
    stammMittig(cx, y0 + 62, WESEN_NAME[FIGUR_WESEN[figur]]);
  } else {
    stammFragezeichen(cx, y0 + 12);
    stammMittig(cx, y0 + 52, "unbekannt");
  }
}


static void stammAnsichtFuellen(int i, ms_t jetzt, StammAnsicht& d) {
  if (i == stamm.anzahl) {                         // das jetzige Tamagotchi (lebt, ist Ei oder ist gerade gegangen)
    stammNameKuerzen(pet.name[0] ? pet.name : "Neues Ei", d.name, 12);
    d.wesen = wesenVon(pet.geboren);
    d.eltern = stamm.elternAktuell;
    d.elternName = stamm.elternNameAktuell;
    d.geschlecht = pet.name[0] ? stamm.geschlechtAktuell : 2;     // 2 = noch nicht bekannt (Ei)
    d.generation = pet.generation;
    d.lebt = !pet.tot;
    const ms_t ende = pet.tot && pet.gestorben > pet.schluepft ? pet.gestorben : jetzt;
    const ms_t gelebt = ende > pet.schluepft ? ende - pet.schluepft : 0;
    d.alterStd = (int)(gelebt / STUNDE);
    d.phase = pet.tot ? stammPhaseNachAlter(gelebt) : phaseBei(pet, jetzt);
  } else {
    const StammEintrag& e = stamm.eintrag[i];
    snprintf(d.name, sizeof(d.name), "%s", e.name);
    d.wesen = e.wesen < WESEN_ZAHL ? e.wesen : 0;
    d.eltern = e.eltern;
    d.elternName = e.elternName;
    d.geschlecht = e.geschlecht ? 1 : 0;
    d.generation = e.generation;
    d.alterStd = e.alterStd;
    d.phase = (Phase)(e.phase >= BABY && e.phase <= OMA ? (int)e.phase : (int)OMA);
    d.lebt = false;
  }
  d.davor = i > 0 ? &stamm.eintrag[i - 1] : nullptr;
}

// Das Tamagotchi davor steht links, wenn es die Mutter ist
static bool stammDavorLinks(const StammAnsicht& d) {
  if (d.davor) return d.davor->geschlecht == 0;
  if (d.eltern >= 0 && d.eltern < BESUCHER) return !figurWeiblich(d.eltern);   // Figur weiblich = Mutter links
  return true;
}

static void stammFamilieZeichnen(int i, int y0, ms_t jetzt) {
  StammAnsicht d;
  stammAnsichtFuellen(i, jetzt, d);
  const int links = 90, rechts = 230, kx = 160;
  const bool davorLinks = stammDavorLinks(d);
  const bool figurLinks = d.davor ? !davorLinks : (d.eltern >= 0 && d.eltern < BESUCHER ? figurWeiblich(d.eltern) : false);
  stammElter(links, y0, "Mutter", davorLinks ? d.davor : nullptr, figurLinks ? d.eltern : -1, d.elternName);
  stammElter(rechts, y0, "Vater", davorLinks ? nullptr : d.davor, !figurLinks ? d.eltern : -1, d.elternName);
  const int ly = y0 + 74;                                   // Linien Eltern → Kind
  bild.drawFastVLine(links, ly, 6, 0); bild.drawFastVLine(rechts, ly, 6, 0);
  bild.drawFastHLine(links, ly + 6, rechts - links + 1, 0);
  bild.drawFastVLine(kx, ly + 6, 7, 0);
  stammEviBild(d.phase, kx, y0 + 131, 80, 44);
  char zeile[64], alter[32];
  const char* geschl = d.geschlecht == 2 ? "" : d.geschlecht ? " · Junge" : " · Mädchen";
  snprintf(zeile, sizeof(zeile), "%s%s", d.name, geschl);
  stammMittig(kx, y0 + 134, zeile);
  snprintf(zeile, sizeof(zeile), "Generation %d · %s", d.generation, WESEN_NAME[d.wesen]);
  stammMittig(kx, y0 + 144, zeile);
  stammAlterText(d.alterStd, d.lebt, alter, sizeof(alter));
  stammMittig(kx, y0 + 154, alter);
  // Verbindung zur nächsten Familie: Das Kind ist dort ein Elternteil, auf der Seite seines Geschlechts
  if (i < stamm.anzahl) {
    const int nx = d.geschlecht == 1 ? rechts : links;
    bild.drawFastVLine(kx, y0 + 164, 4, 0);
    bild.drawFastHLine(min(kx, nx), y0 + 168, abs(nx - kx) + 1, 0);
    bild.drawFastVLine(nx, y0 + 168, STAMM_TAKT_H - 168 - 1, 0);
  }
}

void stammZeichnen(ms_t jetzt) {
  const Phase jetzt_ph = phaseBei(pet, jetzt);
  bild.fillScreen(1);
  stammScrollen(0);                                        // Grenzen einhalten
  const int n = stamm.anzahl + 1;
  for (int i = 0; i < n; i++) {
    const int y0 = STATUS_H + 3 + i * STAMM_TAKT_H - stammScroll;
    if (y0 > ZEILE_Y || y0 + STAMM_TAKT_H < STATUS_H) continue;     // nicht sichtbar
    stammFamilieZeichnen(i, y0, jetzt);
  }
  zeichneStatus(jetzt_ph);                                 // Statusleiste und Textzeile decken die Ränder ab
  bild.fillRect(0, ZEILE_Y, BREITE, HOEHE - ZEILE_Y, 1);
  bild.drawFastHLine(0, ZEILE_Y, BREITE, 0);
  // Schieber am Rand
  const int max_ = stammScrollMax();
  if (max_ > 0) {
    const int spur = STAMM_SICHT_H - 4, griff = max(12, spur * STAMM_SICHT_H / (STAMM_SICHT_H + max_));
    bild.drawFastVLine(317, STATUS_H + 2, spur, 187);
    bild.fillRect(316, STATUS_H + 2 + (spur - griff) * stammScroll / max_, 3, griff, 0);
  }
  char zeile[64];
  snprintf(zeile, sizeof(zeile), "Stammbaum · %d Generation%s", n, n == 1 ? "" : "en");
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 6, zeile, 0);
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 22, "▲ hoch", 0);
  schreibe(bild, 100, ZEILE_Y + 22, "▼ runter", 0);
  schreibe(bild, 242, ZEILE_Y + 22, tastenDa ? "B Zurück" : "Zurück", 0);
  zeigen();
}
