// Öffentlicher Parkeingang. Evi läuft selbst zu den drei antippbaren Zielen.
static constexpr ParkPunkt PARK_WEGPUNKT = {137, 146};
static constexpr ParkPunkt PARK_ANKUNFT[PARK_ZIELE] = {{158, 150}, {110, 120}, {155, 135}};
static constexpr ParkPunkt PARK_FUNDPLATZ[2] = {{179, 149}, {104, 105}};
static constexpr int PARK_LOTTI_X = 185, PARK_LOTTI_FUSS = 133;

bool parkOffen() { return park.phase != PARK_ZU; }

bool parkMoeglich(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
  return ph >= BABY && ph <= OMA && !schlaeft(pet, jetzt) && !evi.schluepfen && !ritual.aktiv;
}

void parkSchliessen() {
  if (!parkOffen()) return;
  park = ParkZustand();
  momentStoppen(true);
  moment.ruheBisTakt = evi.takt + RUHE_TAKTE;
}

void parkOeffnen(ms_t jetzt) {
  if (!parkMoeglich(jetzt)) { zeigeMeldung("Evi ruht gerade."); return; }
  momentStoppen(true);
  menueZeigen(false);
  kuschel = {};
  evi.reaktion = R_KEINE;
  sagen.bis = 0;
  park = ParkZustand();
  park.phase = PARK_WAHL;
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
  if (!parkMoeglich(jetzt)) { parkSchliessen(); return; }
  const uint32_t seit = m - park.seit;
  if (park.phase == PARK_LAEUFT) {
    const int fortschritt = (int)min(seit, park.dauer);
    park.fuesse.x = park.von.x + (park.nach.x - park.von.x) * fortschritt / (int)park.dauer;
    park.fuesse.y = park.von.y + (park.nach.y - park.von.y) * fortschritt / (int)park.dauer;
    if (seit >= park.dauer) {
      if (!park.amWegpunkt) {
        park.amWegpunkt = true;
        parkGehen(PARK_ANKUNFT[park.ziel], m);
      } else {
        park.seit = m;
        park.phase = park.ziel == PARK_ZIEL_LOTTI ? PARK_GRUSS : PARK_SCHNUPPERT;
      }
    }
  } else if (park.phase == PARK_SCHNUPPERT && seit >= 650) {
    park.phase = PARK_HILFE;
  } else if (park.phase == PARK_SUCHT && seit >= 500) {
    parkUntersuchen(jetzt);
  }
}

void parkKnopf(Knopf k) {
  if (k == K_B || k == K_START) { parkSchliessen(); return; }
  if (k == K_A) {
    if (park.phase == PARK_WAHL) parkZielStarten(park.ziel);
    else if (park.phase == PARK_HILFE) parkHelfen();
    else if (park.phase == PARK_GRUSS || park.phase == PARK_FUND) park.phase = PARK_WAHL;
    return;
  }
  if (park.phase != PARK_WAHL) return;
  const int delta = k == K_UP || k == K_L ? -1 : k == K_DOWN || k == K_R ? 1 : 0;
  park.ziel = (ParkZiel)((park.ziel + PARK_ZIELE + delta) % PARK_ZIELE);
}

void parkTouch(int x, int y) {
  if (y >= ZEILE_Y) { parkKnopf(x >= 240 ? K_B : K_A); return; }
  y -= STATUS_H;
  if (y < 0 || park.phase == PARK_LAEUFT || park.phase == PARK_SCHNUPPERT || park.phase == PARK_SUCHT) return;
  ParkZiel ziel = PARK_ZIELE;
  if (x >= 163 && x <= 207 && y >= 76 && y <= 136) ziel = PARK_ZIEL_LOTTI;
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
  const int erde = (h == HIMMEL_NACHT ? 7 : h == HIMMEL_DAEMMERUNG ? 10 : 15) - (trueb ? 2 : 0);
  uint8_t* puffer = bild.getBuffer() + STATUS_H * BREITE;
  for (int y = 0; y < grund.h; y++) {
    const int sky = h == HIMMEL_NACHT ? (trueb ? 2 : 0) :
                    h == HIMMEL_DAEMMERUNG ? (trueb ? 4 : 6) : (trueb ? 8 : 12);
    for (int x = 0; x < grund.w; x++) {
      const uint8_t stufe = grauStufe(grund.grau, grund.w, x, y);
      int wert = (stufe * erde + 7) / 15;
      if (parkIstHimmel(himmelmaske, x, y) && (h != HIMMEL_TAG || w != WT_SONNE_WOLKEN)) {
        const int band = (y + (trueb ? (x / 28 + y / 6) % 8 : 0)) / 23;
        wert = sky + band + (w == WT_SONNE_WOLKEN ? (stufe >= 14 ? 2 : 0) : 0);
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
      bild.drawPixel(x, STATUS_H + y, nacht ? 238 : 255);
  }
  if (nacht) {
    const ParkPunkt sterne[] = {{12, 12}, {22, 36}, {162, 15}, {193, 31}, {222, 9}, {241, 39}, {291, 10}, {301, 32}};
    for (const auto& s : sterne)
      if (parkIstHimmel(himmelmaske, s.x, s.y)) bild.drawPixel(s.x, STATUS_H + s.y, 221);
  }
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

static void parkFigurZeichnen(Phase ph, uint32_t m) {
  const int fuss = STATUS_H + park.fuesse.y;
  const bool gefunden = park.phase == PARK_FUND && park.fund >= 0;
  const int pose = gefunden ? 2 : park.phase == PARK_SUCHT ? 1 :
                   park.phase == PARK_SCHNUPPERT || park.phase == PARK_HILFE ? 0 : -1;
  if (pose >= 0) {
    const ZimmerBild& e = *PARK_EVI[ph - BABY][pose];
    const int basis[] = {70, 82, 86, 82};
    const int h = max(1, raumFigurHoehe(*ART[ph][1], ph, fuss, true, false) * 4 * e.h / (5 * basis[ph - BABY]));
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
    const int h = max(1, raumFigurHoehe(e, ph, fuss, true, false) * 4 / 5);
    const int w = max(1, e.w * h / e.h);
    ellipseFuellen(park.fuesse.x, fuss, max(3, w / 5), 1, 119);
    raumRasterZeichnen(e, park.fuesse.x - w / 2, fuss - h + 1 - (unterwegs ? m / 125 % 2 * 2 : 0),
                       w, h, park.rechts, false, true, fuss);
  }
}

void parkZeichnen(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
  if (ph < BABY || ph > OMA) { parkSchliessen(); return; }
  const Himmel h = himmelBei(jetzt);
  const Wetter w = wetterBei(jetzt);
  const uint32_t m = millis();
  parkHintergrund(jahreszeitBei(jetzt), h, w);
  parkNiederschlag(w, h, m);
  ellipseFuellen(PARK_LOTTI_X, STATUS_H + PARK_LOTTI_FUSS, 9, 1, 119);
  zimmerEbene(PARK_LOTTI, PARK_LOTTI_X - PARK_LOTTI.w / 2,
              STATUS_H + PARK_LOTTI_FUSS - PARK_LOTTI.h + 1);
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
    const auto& ziel = park.ziel == PARK_ZIEL_LOTTI ? ParkPunkt{PARK_LOTTI_X, 76} : PARK_FUNDPLATZ[park.ziel];
    const int y = STATUS_H + ziel.y - 17;
    bild.fillTriangle(ziel.x - 4, y, ziel.x + 4, y, ziel.x, y + 5, 0);
  }
  zeichneStatus(ph);
  bild.fillRect(0, ZEILE_Y, BREITE, HOEHE - ZEILE_Y, 1);
  bild.drawFastHLine(0, ZEILE_Y, BREITE, 0);
  const char* zieltext = park.ziel == PARK_ZIEL_LOTTI ? "Lotti" : PARK_STELLE[park.ziel];
  const char* text = zieltext;
  const char* aktion = tastenDa ? "A Losgehen" : "Ziel antippen";
  switch (park.phase) {
    case PARK_LAEUFT: text = "Evi läuft los …"; aktion = ""; break;
    case PARK_SCHNUPPERT: text = "Evi schnuppert …"; aktion = ""; break;
    case PARK_HILFE: text = "Was ist da?"; aktion = tastenDa ? "A Nachsehen" : "Nachsehen"; break;
    case PARK_SUCHT: text = "Evi schaut nach …"; aktion = ""; break;
    case PARK_GRUSS: text = "Lotti: Hallo, Evi! Schön, dass du da bist."; aktion = tastenDa ? "A Weiter" : "Weiter"; break;
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
