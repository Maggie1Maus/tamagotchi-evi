// Eingezäunter Garten. Himmel, Wetter, Jahreszeit und Weihnachtszeit wie am Zimmerfenster.
bool gartenMoeglich(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
  return ph >= BABY && ph <= OMA && !schlaeft(pet, jetzt) && !evi.schluepfen;
}
void gartenSchliessen() {
  garten = GartenZustand();
  momentStoppen(true);
  moment.ruheBisTakt = evi.takt + RUHE_TAKTE;
}
void gartenOeffnen() {
  if (!gartenMoeglich(jetztMs())) { zeigeMeldung("Evi ruht gerade. Später geht es raus."); return; }
  momentStoppen(true); menueZeigen(false);
  evi.reaktion = R_KEINE; sagen.bis = 0; kuschel.seit = kuschel.bis = 0;
  garten = GartenZustand(); garten.phase = GA_WAHL;
}
void gartenSuchen() {
  garten.vonX = garten.x; garten.seit = millis(); garten.fund = -1;
  garten.schon = garten.voll = false; garten.phase = GA_HIN;
}
void gartenHelfen() {
  if (garten.phase != GA_WARTEN) return;
  garten.phase = GA_UNTERSUCHEN; garten.seit = millis();
}
void gartenErgebnis(ms_t jetzt) {
  const int32_t tag = tagKennung(jetzt);
  if (sammlung.gartenTag != tag) { sammlung.gartenTag = tag; sammlung.gartenVersucht = 0; }
  const uint8_t bit = 1 << garten.stelle;
  garten.schon = sammlung.gartenVersucht & bit;
  garten.voll = sammlung.anzahl >= SAMMLUNG_MAX;
  if (!garten.schon && !garten.voll) {
    sammlung.gartenVersucht |= bit;
    char schluessel[48]; snprintf(schluessel, sizeof(schluessel), "%ld:garten", (long)tag);
    const uint32_t tagZufall = streuwert(schluessel);
    snprintf(schluessel, sizeof(schluessel), "%ld:garten:%u", (long)tag, garten.stelle);
    const uint32_t zufall = streuwert(schluessel);
    // Mindestens eine der drei Stellen pro Tag hat einen Fund. Andere Stellen: 20 %.
    if (garten.stelle == tagZufall % 3 || zufall % 100 < 20) {
      const Fundart art = (Fundart)((zufall / 100) % FUNDARTEN);
      if (fundAufnehmen(art, FUND_GARTEN, tag, pet.generation, wetterBei(jetzt), jahreszeitBei(jetzt), garten.stelle))
        garten.fund = sammlung.anzahl - 1;
    }
    speichern(jetzt); // auch erfolglose Suche merken, damit Neustarts keinen neuen Versuch schenken
  }
  garten.phase = garten.fund >= 0 ? GA_ZEIGEN : GA_LEER;
  if (garten.fund >= 0) piep(TON(TON_MUENZE));
}
void gartenTakt(ms_t jetzt) {
  if (garten.phase == GA_ZU || !bildschirmAn) return;
  if (!gartenMoeglich(jetzt)) { gartenSchliessen(); return; }
  const uint32_t seit = millis() - garten.seit;
  if (garten.phase == GA_HIN) {
    garten.x = garten.vonX + (GARTEN_X[garten.stelle] - garten.vonX) * (int)min(seit, (uint32_t)1100) / 1100;
    if (seit >= 1100) { garten.phase = GA_SCHNUPPERN; garten.seit = millis(); }
  } else if (garten.phase == GA_SCHNUPPERN && seit >= 900) garten.phase = GA_WARTEN;
  else if (garten.phase == GA_UNTERSUCHEN && seit >= 1000) gartenErgebnis(jetzt);
}
void gartenWeiter() {
  if (garten.phase == GA_WAHL) gartenSuchen();
  else if (garten.phase == GA_WARTEN) gartenHelfen();
  else if (garten.phase == GA_ZEIGEN || garten.phase == GA_LEER) garten.phase = GA_WAHL;
}
void gartenKnopf(Knopf k) {
  if (k == K_B || k == K_START) { gartenSchliessen(); return; }
  if (k == K_A) { gartenWeiter(); return; }
  if (garten.phase == GA_WAHL) {
    if (k == K_L || k == K_UP) garten.stelle = (garten.stelle + 1) % 3;
    if (k == K_R || k == K_DOWN) garten.stelle = (garten.stelle + 2) % 3;
  }
}
void gartenTaste(Taste k) {
  if (k == RECHTS) gartenSchliessen();
  else if (k == MITTE) gartenWeiter();
  else if (garten.phase == GA_WAHL) garten.stelle = (garten.stelle + 2) % 3;
}
void gartenBeruehrt(int x, int y) {
  if (y >= 201 && x >= 250) { gartenSchliessen(); return; }
  if (garten.phase == GA_ZEIGEN || garten.phase == GA_LEER) { gartenWeiter(); return; }
  if (y < 45 || y >= 201) return;
  if (garten.phase == GA_WAHL) {
    garten.stelle = x < 105 ? 0 : x > 215 ? 1 : 2;
    gartenSuchen();
  } else if (garten.phase == GA_WARTEN && abs(x - GARTEN_X[garten.stelle]) <= 65) gartenHelfen();
}

// Skalierung direkt aus den vorhandenen 1-Bit-Bildern, ohne einen zusätzlichen Bildpuffer.
void gartenBildZeichnen(const Bild& b, int x0, int y0, int w, int h, bool gespiegelt) {
  for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
    const int sx = gespiegelt ? b.w - 1 - x * b.w / w : x * b.w / w, sy = y * b.h / h;
    if (!bildDeckt(b, sx, sy)) continue;
    const bool schwarz = b.linien[sy * ((b.w + 7) / 8) + sx / 8] & (0x80 >> (sx % 8));
    bild.drawPixel(x0 + x, y0 + y, schwarz ? 0 : 1);
  }
}
// Fundstück bleibt eine eigene Ebene über Evis Pfoten.
void gartenFundZeichnen(const Bild& b, int cx, int cy) {
  const int w = max(1, b.w * 21 / max(b.w, b.h)), h = max(1, b.h * 21 / max(b.w, b.h));
  gartenBildZeichnen(b, cx - w / 2, cy - h / 2, w, h, false);
}
void gartenWetterZeichnen(ms_t jetzt) {
  const Wetter w = wetterBei(jetzt);
  const Himmel h = himmelBei(jetzt);
  if (h == HIMMEL_NACHT) {
    for (int i = 0; i < 18; i++) bild.drawPixel(12 + i * 47 % 295, 34 + i * 19 % 45, 1);
    bild.fillCircle(235, 52, 9, 1); bild.fillCircle(239, 49, 8, 0);
  } else {
    if (w == WT_SONNE || w == WT_SONNE_WOLKEN) zeichneBild(WETTER_SONNE, 205, 35);
    if (w != WT_SONNE) {
      zeichneBild(w == WT_REGEN ? WETTER_REGENWOLKE : WETTER_WOLKE, 142, 36);
      if (w == WT_BEWOELKT) zeichneBild(WETTER_WOLKE, 234, 53);
    }
  }
  // Niederschlag hinter den Objekten; 40-ms-Bilder, Schnee langsamer mit seitlichem Pendeln.
  if (w != WT_REGEN && w != WT_SCHNEE) return;
  const uint16_t f = h == HIMMEL_NACHT ? 1 : 0;
  for (int i = 0; i < 32; i++) {
    const int lauf = (millis() / (w == WT_REGEN ? 22 : 90) + i * 31) % 160;
    int x = 5 + i * 47 % 310, y = 34 + lauf;
    if (w == WT_REGEN) { x = (x + 320 - lauf / 4) % 320; bild.drawLine(x, y, x - 2, min(y + 5, 197), f); }
    else {
      x += (int)lroundf(2 * sinf((millis() % 6000) * .002f + i));
      bild.drawPixel(x, y, f);
      if (i % 3 == 0 && y < 196) { bild.drawPixel(x - 1, y, f); bild.drawPixel(x + 1, y, f); bild.drawPixel(x, y - 1, f); bild.drawPixel(x, y + 1, f); }
    }
  }
}
// Helle Mondlichtkante: kahle Zweige und Zaunkonturen verschwinden nachts nicht im Himmel.
void gartenObjektZeichnen(const Bild& b, int x0, int y0, bool nacht) {
  if (nacht) for (int y = 0; y < b.h; y++) for (int x = 0; x < b.w; x++) {
    if (!bildDeckt(b, x, y)) continue;
    static const int8_t RICHTUNG[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (auto& r : RICHTUNG) {
      const int nx = x + r[0], ny = y + r[1];
      if (y0 + ny >= 27 && y0 + ny < 123 && !bildDeckt(b, nx, ny)) bild.drawPixel(x0 + nx, y0 + ny, 1);
    }
  }
  zeichneBild(b, x0, y0);
}
void gartenSzeneZeichnen(ms_t jetzt) {
  bild.fillScreen(1);
  const Himmel h = himmelBei(jetzt);
  if (h == HIMMEL_NACHT) bild.fillRect(0, 27, BREITE, 96, 0);
  else if (h == HIMMEL_DAEMMERUNG || wetterBei(jetzt) == WT_REGEN)
    for (int y = 27; y < 123; y++) for (int x = 0; x < BREITE; x++) bild.drawPixel(x, y, (x + y) & 1);
  gartenWetterZeichnen(jetzt);
  // Heller Weg und wenige Grashalme; Winter bleibt ruhig und verschneit.
  bild.drawLine(148, 123, 123, 198, 0); bild.drawLine(172, 123, 193, 198, 0);
  if (jahreszeitBei(jetzt) != JZ_WINTER) for (int i = 0; i < 14; i++) {
    const int x = 9 + i * 41 % 302, y = 147 + i * 17 % 46;
    if (x > 123 && x < 193) continue;
    bild.drawLine(x, y, x - 2, y - 3, 0); bild.drawLine(x, y, x + 2, y - 4, 0);
  }
  const Jahreszeit j = jahreszeitBei(jetzt);
  gartenObjektZeichnen(GA_ZAUN, 160 - GA_ZAUN.w, 121 - GA_ZAUN.h, h == HIMMEL_NACHT);
  gartenObjektZeichnen(GA_ZAUN, 160, 121 - GA_ZAUN.h, h == HIMMEL_NACHT);
  gartenObjektZeichnen(GA_GARTENTUER, 160 - GA_GARTENTUER.w / 2, 128 - GA_GARTENTUER.h, h == HIMMEL_NACHT);
  gartenObjektZeichnen(GA_LICHTERKETTE, 160 - GA_LICHTERKETTE.w, 77, h == HIMMEL_NACHT);
  gartenObjektZeichnen(GA_LICHTERKETTE, 160, 77, h == HIMMEL_NACHT);
  if (weihnachtsZeit(jetzt)) gartenObjektZeichnen(GA_KRANZ, 160 - GA_KRANZ.w / 2, 91, h == HIMMEL_NACHT);
  gartenObjektZeichnen(*GARTEN_BAUM[j], 49 - GARTEN_BAUM[j]->w / 2, 163 - GARTEN_BAUM[j]->h, h == HIMMEL_NACHT);
  gartenObjektZeichnen(*GARTEN_BUSCH[j], 263 - GARTEN_BUSCH[j]->w / 2, 171 - GARTEN_BUSCH[j]->h, h == HIMMEL_NACHT);
  gartenObjektZeichnen(*GARTEN_BEET[j], 161 - GARTEN_BEET[j]->w / 2, 153 - GARTEN_BEET[j]->h, h == HIMMEL_NACHT);
  if (weihnachtsZeit(jetzt)) gartenObjektZeichnen(GA_SCHNEEMANN, 205, 174 - GA_SCHNEEMANN.h, h == HIMMEL_NACHT);
}
void gartenZeichnen() {
  const ms_t jetzt = jetztMs(); const Phase ph = phaseBei(pet, jetzt);
  gartenSzeneZeichnen(jetzt);
  if (ph >= BABY && ph <= OMA) {
    const bool laeuft = garten.phase == GA_HIN;
    const int pose = garten.phase == GA_ZEIGEN ? 2 : garten.phase == GA_UNTERSUCHEN ? 1 : 0;
    const Bild& b = laeuft ? *MOMENT_BILD[ph][MB_LAUFEN] : garten.phase == GA_WAHL ? *ART[ph][1] : *GARTEN_EVI[ph][pose];
    static const int MAX_H[5] = {0, 70, 82, 86, 82};
    const int hoehe = min((int)b.h, MAX_H[ph]), breite = b.w * hoehe / b.h;
    const int lift = laeuft ? (millis() / 140 % 2) * 2 : 0;
    const int x = max(2, min(garten.x - breite / 2, 318 - breite)), y = 195 - hoehe - lift;
    gartenBildZeichnen(b, x, y, breite, hoehe, laeuft && GARTEN_X[garten.stelle] > garten.vonX);
    if (garten.phase == GA_ZEIGEN && garten.fund >= 0)
      gartenFundZeichnen(*FUND_BILD[sammlung.fund[garten.fund].art], garten.x, y + hoehe * 63 / 100);
    if (garten.phase == GA_WARTEN) {
      bild.fillRoundRect(garten.x - 10, y - 25, 21, 21, 5, 1); bild.drawRoundRect(garten.x - 10, y - 25, 21, 21, 5, 0);
      schreibe(bild, garten.x - 5, y - 21, "?", 0, 2);
    }
  }
  if (garten.phase == GA_WAHL) {
    const int cx = GARTEN_X[garten.stelle];
    bild.fillTriangle(cx - 4, 191, cx + 4, 191, cx, 197, 0);
  }
  zeichneStatus(ph);
  bild.fillRect(0, 201, 320, 39, 1); bild.drawFastHLine(0, 201, 320, 0);
  const char* text = "Evi schnuppert …";
  if (garten.phase == GA_WAHL) text = GARTEN_STELLE[garten.stelle];
  else if (garten.phase == GA_HIN) text = "Wir schauen zusammen nach …";
  else if (garten.phase == GA_WARTEN) text = "Hier raschelt etwas! A oder antippen.";
  else if (garten.phase == GA_UNTERSUCHEN) text = "Was sich hier wohl versteckt?";
  else if (garten.phase == GA_ZEIGEN) text = FUND_NAME[sammlung.fund[garten.fund].art];
  else if (garten.phase == GA_LEER) text = garten.voll ? "Das Sideboard ist voll." : garten.schon ? "Hier haben wir heute schon gesucht." : "Heute nur ein Rascheln. Schön war's!";
  schreibe(bild, 9, 208, text, 0);
  const char* hilfe = garten.phase == GA_WAHL ? "◀▶ wählen · A suchen" : garten.phase == GA_ZEIGEN || garten.phase == GA_LEER ? "A weiter" : "Gemeinsam entdecken";
  schreibe(bild, 9, 225, hilfe, 0);
  schreibe(bild, 258, 225, "B Heim", 0);
}
