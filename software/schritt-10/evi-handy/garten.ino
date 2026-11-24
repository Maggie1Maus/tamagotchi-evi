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
  garten.vonX = garten.x; garten.vonY = garten.y; garten.seit = millis(); garten.fund = -1;
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
  garten.phase = GA_HERAUS; garten.vonY = garten.y; garten.seit = millis();
  if (garten.fund >= 0) piep(TON(TON_MUENZE));
}
void gartenTakt(ms_t jetzt) {
  if (garten.phase == GA_ZU || !bildschirmAn) return;
  if (!gartenMoeglich(jetzt)) { gartenSchliessen(); return; }
  const uint32_t seit = millis() - garten.seit;
  if (garten.phase == GA_HIN) {
    // Auf dem freien Vordergrund seitwärts, dann in die Tiefe zum Beet/Baum/Busch.
    const int quer=min(600,(int)seit);
    garten.x=garten.vonX+(GARTEN_X[garten.stelle]-garten.vonX)*quer/600;
    const int tief=max(0,min(500,(int)seit-600));
    garten.y=garten.vonY+(GARTEN_Y[garten.stelle]-garten.vonY)*tief/500;
    if (seit >= 1100) { garten.phase = GA_SCHNUPPERN; garten.seit = millis(); }
  } else if (garten.phase == GA_SCHNUPPERN && seit >= 900) garten.phase = GA_WARTEN;
  else if (garten.phase == GA_UNTERSUCHEN && seit >= 1000) gartenErgebnis(jetzt);
  else if (garten.phase == GA_HERAUS || garten.phase == GA_ZURUECK) {
    const int ziel=garten.phase==GA_HERAUS?184:RAUM_HEIM_Y;
    garten.y=garten.vonY+(ziel-garten.vonY)*(int)min(seit,(uint32_t)700)/700;
    if(seit>=700) garten.phase=garten.phase==GA_ZURUECK ? GA_WAHL : garten.fund>=0 ? GA_ZEIGEN : GA_LEER;
  }
}
void gartenWeiter() {
  if (garten.phase == GA_WAHL) gartenSuchen();
  else if (garten.phase == GA_WARTEN) gartenHelfen();
  else if (garten.phase == GA_ZEIGEN || garten.phase == GA_LEER) { garten.phase = GA_ZURUECK; garten.vonY = garten.y; garten.seit = millis(); }
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
  raumRasterZeichnen(b,x0,y0,w,h,gespiegelt,false,true,garten.y);
}
// Fundstück bleibt eine eigene Ebene über Evis Pfoten.
void gartenFundZeichnen(const Bild& b, int cx, int cy) {
  const int w = max(1, b.w * 21 / max(b.w, b.h)), h = max(1, b.h * 21 / max(b.w, b.h));
  gartenBildZeichnen(b, cx - w / 2, cy - h / 2, w, h, false);
}
// Die freigegebene Gesamtszene bleibt eine zusammenhängende Ebene.
const Bild& gartenLandschaftBei(ms_t jetzt) {
  return weihnachtsZeit(jetzt) ? LANDSCHAFT_WEIHNACHTEN : *GARTEN_LANDSCHAFT[jahreszeitBei(jetzt)];
}

// Atmosphäre statt einzelner Wetterzeichen: ruhiger Himmel, breite Wolkenschichten,
// nachts wenige Sterne. Sonne/Wolke aus dem Menü werden hier nicht als Bilder eingesetzt.
void gartenHimmelZeichnen(ms_t jetzt) {
  const Himmel h = himmelBei(jetzt);
  const Wetter w = wetterBei(jetzt);
  if (h == HIMMEL_NACHT) bild.fillRect(0, 27, 320, 72, 0);
  else if (h != HIMMEL_TAG || w != WT_SONNE) {
    // Nur die sichtbare Himmelszone; Sinus je Spalte statt je Bildpunkt berechnen.
    for (int x = 0; x < 320; x++) {
      const int kante = 40 + (int)(4 * sinf(x * .025f) + 2 * sinf(x * .061f));
      for (int y = 27; y < 99; y++) {
        uint16_t f = 1;
        if (h == HIMMEL_DAEMMERUNG) f = (x + 3 * y) % 5 != 0;
        else {
          const bool wolke = w != WT_SONNE_WOLKEN || (y >= kante && y < kante + 7);
          const int abstand = w == WT_REGEN ? 5 : w == WT_BEWOELKT ? 7 : 11;
          if (wolke) f = (x + 3 * y) % abstand != 0;
        }
        bild.drawPixel(x, y, f);
      }
    }
  }
  if (h == HIMMEL_NACHT && w != WT_REGEN && w != WT_BEWOELKT && w != WT_SCHNEE) {
    for (int i = 0; i < (w == WT_SONNE_WOLKEN ? 7 : 14); i++)
      bild.drawPixel(128 + i * 43 % 181, 32 + i * 17 % 42, 1);
  }
}

// Regen und Schnee hinter Evi, vor dem offenen Rasen; Pflanzen und Zaun bleiben lesbar.
void gartenWetterZeichnen(ms_t jetzt) {
  const Wetter w = wetterBei(jetzt);
  if (w != WT_REGEN && w != WT_SCHNEE) return;
  const bool nacht = himmelBei(jetzt) == HIMMEL_NACHT;
  const Bild& szene = gartenLandschaftBei(jetzt);
  auto punkt = [&](int x, int y) {
    if (x < 0 || x >= 320 || y < 27 || y >= 201) return;
    const bool himmel = !bildDeckt(szene, x, y - 27);
    const bool rasen = y >= 140 && x > 88 - (y - 140) / 2 && x < 258 + (y - 140) / 2;
    if (himmel || rasen) bild.drawPixel(x, y, nacht && himmel ? 1 : 0);
  };
  for (int i = 0; i < 32; i++) {
    const int lauf = (millis() / (w == WT_REGEN ? 22 : 90) + i * 31) % 168;
    int x = 5 + i * 47 % 310, y = 29 + lauf;
    if (w == WT_REGEN) {
      x = (x + 320 - lauf / 4) % 320;
      for (int k = 0; k < 4; k++) punkt(x - k / 2, y + k);
    } else {
      x += (int)lroundf(2 * sinf((millis() % 6000) * .002f + i));
      punkt(x, y);
      if (i % 3 == 0) { punkt(x - 1, y); punkt(x + 1, y); punkt(x, y - 1); punkt(x, y + 1); }
    }
  }
}

void gartenSzeneZeichnen(ms_t jetzt) {
  bild.fillScreen(1);
  gartenHimmelZeichnen(jetzt);
  const Bild& b = gartenLandschaftBei(jetzt);
  if (himmelBei(jetzt) == HIMMEL_NACHT) {
    // Ein Pixel Mondlicht um die tatsächlichen Silhouetten, damit schwarze Zweige sichtbar bleiben.
    for (int y = 0; y < 72; y++) for (int x = 0; x < 320; x++) {
      if (!bildDeckt(b, x, y)) continue;
      static const int8_t R[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
      for (auto& r : R) {
        const int nx = x + r[0], ny = y + r[1];
        if (nx >= 0 && nx < 320 && ny >= 0 && !bildDeckt(b, nx, ny)) bild.drawPixel(nx, ny + 27, 1);
      }
    }
  }
  zeichneBild(b, 0, 27);
  if (jahreszeitBei(jetzt) == JZ_WINTER && !weihnachtsZeit(jetzt)) zeichneBild(LANDSCHAFT_LICHTER, 173, 85);
  gartenWetterZeichnen(jetzt);
}
void gartenZeichnen() {
  const ms_t jetzt = jetztMs(); const Phase ph = phaseBei(pet, jetzt);
  gartenSzeneZeichnen(jetzt);
  if (ph >= BABY && ph <= OMA) {
    const bool laeuft = garten.phase == GA_HIN || garten.phase == GA_HERAUS || garten.phase == GA_ZURUECK;
    const int pose = garten.phase == GA_ZEIGEN ? 2 : garten.phase == GA_UNTERSUCHEN ? 1 : 0;
    const Bild& b = laeuft ? *MOMENT_BILD[ph][MB_LAUFEN] : garten.phase == GA_WAHL ? *ART[ph][1] : *GARTEN_EVI[ph][pose];
    const int lift = laeuft ? (millis() / 140 % 2) : 0;
    const bool fundPose=!laeuft && garten.phase!=GA_WAHL;
    raumFigurZeichnen(b,ph,garten.x,garten.y,laeuft && garten.phase==GA_HIN && GARTEN_X[garten.stelle]>garten.vonX,lift,true,fundPose);
    const int y=eviOben, hoehe=eviHoehe;
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
  else if (garten.phase == GA_HERAUS) text = "Evi kommt zu dir …";
  else if (garten.phase == GA_ZURUECK) text = "Zurück auf die Wiese …";
  else if (garten.phase == GA_UNTERSUCHEN) text = "Was sich hier wohl versteckt?";
  else if (garten.phase == GA_ZEIGEN) text = FUND_NAME[sammlung.fund[garten.fund].art];
  else if (garten.phase == GA_LEER) text = garten.voll ? "Das Sideboard ist voll." : garten.schon ? "Hier haben wir heute schon gesucht." : "Heute nur ein Rascheln. Schön war's!";
  schreibe(bild, 9, 208, text, 0);
  const char* hilfe = garten.phase == GA_WAHL ? "◀▶ wählen · A suchen" : garten.phase == GA_ZEIGEN || garten.phase == GA_LEER ? "A weiter" : "Gemeinsam entdecken";
  schreibe(bild, 9, 225, hilfe, 0);
  schreibe(bild, 258, 225, "B Heim", 0);
}
