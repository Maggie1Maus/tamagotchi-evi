// Eigene Ansicht für Fundstücke. Das Zimmer bleibt frei, die Kerzen stehen weiter obenauf.
bool sideboardKannLaufen(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
  return ph != EI && ph != TOT && !schlaeft(pet, jetzt) && !evi.schluepfen;
}

void sideboardSofortZu() {
  sideboard = SideboardZustand();
  momentStoppen(true);
}

void sideboardOeffnen() {
  momentStoppen(true);
  evi.reaktion = R_KEINE;
  sagen.bis = 0;
  sideboard = SideboardZustand();
  sideboard.auswahl = sammlung.anzahl ? sammlung.anzahl - 1 : 0;
  sideboard.phase = sideboardKannLaufen(jetztMs()) ? SB_HIN : SB_OFFEN;
  sideboard.seit = millis();
}

void sideboardSchliessen() {
  if (!sideboardKannLaufen(jetztMs())) { sideboardSofortZu(); return; }
  sideboard.phase = SB_HEIM;
  sideboard.vonX = sideboard.x;
  sideboard.seit = millis();
}

void sideboardTakt(ms_t jetzt) {
  if (sideboard.phase == SB_ZU || !bildschirmAn) return;
  const uint32_t seit = millis() - sideboard.seit;
  if ((sideboard.phase == SB_HIN || sideboard.phase == SB_OEFFNEN || sideboard.phase == SB_HEIM) && !sideboardKannLaufen(jetzt)) {
    if (sideboard.phase == SB_HEIM) sideboardSofortZu();
    else sideboard.phase = SB_OFFEN;
    return;
  }
  if (sideboard.phase == SB_HIN || sideboard.phase == SB_HEIM) {
    const int ziel = sideboard.phase == SB_HIN ? 55 : BODEN_X;
    const int fortschritt = (int)min(seit, (uint32_t)1000);
    sideboard.x = sideboard.vonX + (ziel - sideboard.vonX) * fortschritt / 1000;
    if (seit >= 1000) {
      if (sideboard.phase == SB_HEIM) { sideboardSofortZu(); moment.ruheBisTakt = evi.takt + RUHE_TAKTE; }
      else { sideboard.phase = SB_OEFFNEN; sideboard.seit = millis(); }
    }
  } else if (sideboard.phase == SB_OEFFNEN && seit >= 650) sideboard.phase = SB_OFFEN;
}

void sideboardBlaettern(int richtung) {
  if (sideboard.phase != SB_OFFEN || !sammlung.anzahl) return;
  sideboard.auswahl = (sideboard.auswahl + sammlung.anzahl + richtung) % sammlung.anzahl;
}

void sideboardTaste(Taste k) {
  if (k == MITTE) { sideboardSchliessen(); return; }
  sideboardBlaettern(k == LINKS ? -1 : 1);
}

void sideboardEviZeichnen(Phase ph) {
  if (ph < BABY || ph > OMA) return;
  const bool hinten = sideboard.phase == SB_OEFFNEN;
  const Bild& b = *MOMENT_BILD[ph][hinten ? MB_FENSTER : MB_LAUFEN];
  const int lift = hinten ? 0 : ((millis() - sideboard.seit) / 100) % 2 * 2;
  const int fort = max(0,min(1000,(BODEN_X-sideboard.x)*1000/(BODEN_X-55)));
  // Erst vor dem Schrank entlang, dann seitlich bis zu dessen Vorderkante.
  const int fuss=RAUM_HEIM_Y - fort*30/1000;
  raumFigurZeichnen(b,ph,sideboard.x,fuss,sideboard.phase==SB_HEIM,lift,false,false);
}

void sammlungZeichnen() {
  bild.fillScreen(1);
  schreibe(bild, 24, 15, "Im Sideboard", 0, 2);
  bild.drawFastHLine(20, 38, 280, 0);
  if (!sammlung.anzahl) {
    schreibe(bild, 42, 90, "Noch keine Fundstücke.", 0);
    schreibe(bild, 42, 114, "Im Garten suchen wir", 0);
    schreibe(bild, 42, 129, "zusammen welche!", 0);
  } else {
    if (sideboard.auswahl >= sammlung.anzahl) sideboard.auswahl = sammlung.anzahl - 1;
    const Fundstueck& f = sammlung.fund[sideboard.auswahl];
    const Bild& b = *FUND_BILD[f.art];
    zeichneBild(b, (BREITE - b.w) / 2, 49 + (64 - b.h) / 2);
    const char* name = FUND_NAME[f.art];
    schreibe(bild, (BREITE - textBreite(name, 2)) / 2, 123, name, 0, 2);
    char zeile[64];
    snprintf(zeile, sizeof(zeile), "%s", f.ort == FUND_GARTEN ? GARTEN_STELLE[f.stelle] : "Beim echten Spaziergang");
    schreibe(bild, 35, 151, zeile, 0);
    snprintf(zeile, sizeof(zeile), "Am %02ld.%02ld.%04ld", (long)(f.tag % 100), (long)(f.tag / 100 % 100), (long)(f.tag / 10000));
    schreibe(bild, 35, 168, zeile, 0);
    int erinnerungsY = 184;
    umbrechen(fundErinnerung(f), 43, [&](const char* z) { if (erinnerungsY < 207) { schreibe(bild, 24, erinnerungsY, z, 0); erinnerungsY += 11; } });
    snprintf(zeile, sizeof(zeile), "%u / %u", sideboard.auswahl + 1, sammlung.anzahl);
    schreibe(bild, BREITE - 24 - textBreite(zeile), 21, zeile, 0);
  }
  bild.drawFastHLine(20, 209, 280, 0);
  schreibe(bild, 24, 221, "◀ Zurück", 0);
  schreibe(bild, 126, 221, "Schließen", 0);
  schreibe(bild, 244, 221, "Weiter ▶", 0);
}
