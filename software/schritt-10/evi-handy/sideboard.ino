// Collection access follows the current sideboard placement and safe room paths.
uint32_t sideboardSchrittMs=0;
bool sideboardKannLaufen(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
  return einrichtung.platz[Z_SIDEBOARD].aufgestellt && ph != EI && ph != TOT && !schlaeft(pet, jetzt) && !evi.schluepfen;
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
  if(sideboard.phase==SB_HIN){const auto&p=einrichtung.platz[Z_SIDEBOARD];gangBeginnenTief(p.x+ZG_SIDEBOARD.w/2-BODEN_X,p.fuss+5);sideboardSchrittMs=millis();}
}

void sideboardSchliessen() {
  if (!sideboardKannLaufen(jetztMs())) { sideboardSofortZu(); return; }
  sideboard.phase = SB_HEIM;
  sideboard.vonX = sideboard.x;
  sideboard.seit = millis();
  gangBeginnen(0);sideboardSchrittMs=millis();
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
    if(millis()-sideboardSchrittMs<20)return;
    sideboardSchrittMs=millis();
    const bool fertig=gangSchritt();sideboard.x=BODEN_X+geher.x;
    if (fertig) {
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
  raumFigurZeichnen(b,ph,BODEN_X+geher.x,geher.fuss,sideboard.phase==SB_HEIM,lift,false,false);
}

void sammlungZeichnen() {
  bild.fillScreen(1);
  schreibe(bild, 24, 15, "Im Sideboard", 0, 2);
  bild.drawFastHLine(20, 38, 280, 0);
  if (!sammlung.anzahl) {
    schreibe(bild, 42, 90, "Noch keine Fundstücke.", 0);
    schreibe(bild, 42, 114, "Im Park suchen wir", 0);
    schreibe(bild, 42, 129, "zusammen welche!", 0);
  } else {
    if (sideboard.auswahl >= sammlung.anzahl) sideboard.auswahl = sammlung.anzahl - 1;
    const Fundstueck& f = sammlung.fund[sideboard.auswahl];
    const Bild& b = *FUND_BILD[f.art];
    zeichneBild(b, (BREITE - b.w) / 2, 49 + (64 - b.h) / 2);
    const char* name = FUND_NAME[f.art];
    schreibe(bild, (BREITE - textBreite(name, 2)) / 2, 123, name, 0, 2);
    char zeile[64];
    const char* ort = f.ort == FUND_PARK ? PARK_STELLE[f.stelle] :
                      f.ort == FUND_GARTEN ? GARTEN_STELLE[f.stelle] : "Beim echten Spaziergang";
    snprintf(zeile, sizeof(zeile), "%s", ort);
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
