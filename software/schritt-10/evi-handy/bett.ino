// Evi schläft im Tierbett (Wunsch der Nutzerin vom 25.11.2026, nur Waveshare).
// Schläft sie ein (nachts oder Mittagsschlaf), läuft sie bei hellem Bildschirm zum Bett und legt sich hinein;
// bei dunklem Bildschirm liegt sie gleich dort. Wacht sie auf, läuft sie zurück auf den Teppich.
// Gezeichnet wird ihr vorhandenes Schlafbild (keine neue Zeichnung), so groß wie es zur Tiefe des Betts passt.
// Danach kommt der vordere Rand des Korbs noch einmal darüber: Sie liegt im Bett, nicht davor. Das Schlafbild
// ist innen ganz deckend (geprüft), vom Bett scheint also nichts durch ihr Fell.
// Ausgeschlafener wird sie dadurch nicht (so gewünscht). Ist das Bett im Inventar, schläft sie wie bisher auf dem Teppich.

const int BETT_LIEGEN = 37;   // Zeile im Bettbild, auf der Evis Unterkante liegt (hinter dem vorderen Rand)

bool bettDa() { return einrichtung.platz[Z_BETT].aufgestellt; }
int bettMitteX() { return einrichtung.platz[Z_BETT].x + ZG_TIERBETT.w / 2; }

static void bettHeim() { geher.x = 0; geher.fuss = RAUM_HEIM_Y; geher.lift = 0; bettZustand.phase = BETT_NEIN; }
static void bettHinlegen() {
  geher.x = bettMitteX() - BODEN_X; geher.fuss = einrichtung.platz[Z_BETT].fuss + 5; geher.lift = 0; geher.flip = false;
  bettZustand.phase = BETT_LIEGT;
}

// Jeden Spielschritt (120 ms)
void bettTakt(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
  const bool imSchlaf = ph != EI && ph != TOT && !evi.schluepfen && beschreiben(pet, jetzt) == S_SCHLAEFT && bettDa();
  const bool frei = !spiel.aktiv && sideboard.phase == SB_ZU && !zimmerEditor.offen;
  switch (bettZustand.phase) {
    case BETT_NEIN:
      if (!imSchlaf || !frei) return;
      if (moment.art != M_KEINER) momentStoppen(true);
      if (!bildschirmAn) { bettHinlegen(); return; }
      gangBeginnenTief(bettMitteX() - BODEN_X, einrichtung.platz[Z_BETT].fuss + 5);
      bettZustand.phase = gang.laeuft ? BETT_HIN : BETT_NEIN;
      if (!gang.laeuft && geher.x == bettMitteX() - BODEN_X) bettHinlegen();
      return;
    case BETT_HIN:
      if (!imSchlaf || !frei) { gang.laeuft = false; moment.bild = MB_KEINS; bettHeim(); return; }
      if (!bildschirmAn || gangSchritt()) { gang.laeuft = false; moment.bild = MB_KEINS; bettHinlegen(); }
      return;
    case BETT_LIEGT:
      if (imSchlaf) return;
      if (bildschirmAn && frei && moment.art == M_KEINER) {
        gangBeginnen(0);
        bettZustand.phase = gang.laeuft ? BETT_ZURUECK : BETT_NEIN;
        if (!gang.laeuft) bettHeim();
      } else if (moment.art == M_KEINER) {
        bettHeim();
      } else {
        bettZustand.phase = BETT_NEIN;   // ein Moment (z. B. Begrüßung) hat Evi schon übernommen
      }
      return;
    case BETT_ZURUECK:
      if (moment.art != M_KEINER || !frei) { bettZustand.phase = BETT_NEIN; return; }
      if (!bildschirmAn || gangSchritt()) { gang.laeuft = false; moment.bild = MB_KEINS; bettHeim(); }
      return;
  }
}

bool bettLiegt() { return bettZustand.phase == BETT_LIEGT; }

// Evi im Bett zeichnen (aus zeichneEvi): b ist ihr Schlafbild
void bettEviZeichnen(const Bild& b, Phase ph, int atem) {
  const auto& p = einrichtung.platz[Z_BETT];
  const int oben = p.fuss - ZG_TIERBETT.h;
  raumImBett = true;
  raumFigurZeichnen(b, ph, bettMitteX(), p.fuss, false, ZG_TIERBETT.h - BETT_LIEGEN + atem, false, false);
  raumImBett = false;
  const auto& rand = ZG_TIERBETT_VORDERRAND;
  for (int y = 0; y < rand.h; y++)
    for (int x = 0; x < rand.w; x++) {
      const int px = p.x + x, py = oben + BETT_RAND + y;
      if (rand.deckt(x, y) && !raumVerdeckt(false, p.fuss, px, py)) {
        const uint8_t wert = rand.wert(x, y);
        bild.drawPixel(px, py, pet.lichtAus ? zimmerDunkelwert(wert) : wert);
      }
    }
}
