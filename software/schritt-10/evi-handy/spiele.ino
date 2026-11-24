// Schritt 10, Etappe 2: die drei Spiele wie in app.js (Abschnitt „Games“).
// Fünf Runden, jeder Erfolg zählt als Treffer (2 Münzen, Regeln in pet.h: spielBeenden).
// Während ein Spiel läuft, füllt es den Bildschirm, und die Tasten gehören dem Spiel.
//
// Tasten am Gerät:
//   Ballfangen:         A (oder Steuerkreuz) fängt, B oder START beendet
//   Links oder rechts?: ◀ oder ▶ raten, B oder START beendet
//   Melodie merken:     ◀ linkes Feld, A (oder ▲▼) mittleres Feld, ▶ rechtes Feld, B oder START bricht ab
//   Kekse fangen:       ◀ ▶ (festhalten läuft weiter) oder das Gerät kippen; B oder START beendet
//   Schnick, Schnack, Schnuck: ◀ Schere, A (oder ▲▼) Stein, ▶ Papier; B oder START bricht ab
// Die letzten beiden gibt es seit 24.11.2026 nur auf dem Board (Wunsch der Nutzerin: neue Spiele).
// Touch nur noch als Notlösung, wenn die Tasten fehlen: linkes Drittel, Mitte, rechtes Drittel (seit 24.11.2026
// ist Touch sonst zum Kuscheln da und im Spiel ohne Wirkung).

const float BALL_VON = 40, BALL_BIS = 266;     // TRACK: linke Kante des 14 Punkte breiten Balls
const float FELD_VON = 146, FELD_BIS = 174;    // ZONE: das gepunktete Fangfeld
const uint16_t FELD_TOENE[3] = {523, 659, 784};
// Kekse fangen: Evi darf so weit laufen; ein Tastendruck macht einen Sprung von 26 Punkten
const int KEKS_EVI_MIN = -120, KEKS_EVI_MAX = 110, KEKS_SCHRITT = 26, KEKS_FANGBREITE = 26, KEKS_R = 9;
const float KEKS_START_TEMPO = 55, KEKS_KIPP_TEMPO = 330;   // Punkte pro Sekunde (Kippen: bei voller Neigung)
const char* const SCHNICK_NAMEN[3] = {"Schere", "Stein", "Papier"};
// Wer gegen wen gewinnt: Satz für Gewinner a gegen b (a schlägt b, wenn (a - b + 3) % 3 == 1)
const char* schnickSatz(int sieger) {
  return sieger == 0 ? "Schere schneidet Papier." : sieger == 1 ? "Stein macht Schere stumpf." : "Papier wickelt Stein ein.";
}

void spielPlanLeeren() { spiel.planAnzahl = 0; }

void spielPlanen(SpielEreignis was, int wert, uint32_t inMs) {
  if (spiel.planAnzahl >= 12) return;
  spiel.plan[spiel.planAnzahl].was = was;
  spiel.plan[spiel.planAnzahl].wert = (int8_t)wert;
  spiel.plan[spiel.planAnzahl].um = millis() + inMs;
  spiel.planAnzahl++;
}

void schnickFrage() {
  snprintf(spiel.notiz, sizeof(spiel.notiz), "%s", tastenDa ? "◀ Schere · A Stein · ▶ Papier" : "Schere, Stein oder Papier?");
}

void keksFallenLassen() {
  spiel.kx = random(40, 281);
  spiel.ky = 28;
  spiel.keksFaellt = true;
  spiel.pause = 0.5f;
  spiel.letzteZeit = millis();
}

void rateFrage() {
  snprintf(spiel.notiz, sizeof(spiel.notiz), "Wohin springt %s? %s", pet.name, tastenDa ? "◀ oder ▶" : "Links oder rechts");
}

void spielStarten(int art) {
  menueZeigen(false);
  momentStoppen(true);
  spielPlanLeeren();
  spiel.aktiv = true;
  spiel.art = (SpielArt)art;
  spiel.runde = 1;
  spiel.treffer = 0;
  spiel.notiz[0] = 0;
  spiel.beschaeftigt = false;
  spiel.eviX = 0;
  spiel.leuchtet = -1;
  if (spiel.art == SPIEL_BALL) {
    spiel.x = BALL_VON;
    spiel.richtung = 1;
    spiel.tempo = 125;
    spiel.pause = 0.4f;
    spiel.letzteZeit = millis();
  } else if (spiel.art == SPIEL_RATEN) {
    rateFrage();
  } else if (spiel.art == SPIEL_KEKSE) {
    spiel.eviGenau = 0;
    spiel.eviRechts = false;
    spiel.kTempo = KEKS_START_TEMPO;
    keksFallenLassen();
  } else if (spiel.art == SPIEL_SCHNICK) {
    spiel.meins = spiel.evis = -1;
    schnickFrage();
  } else {
    spiel.folge[0] = random(0, 3);
    spiel.folge[1] = random(0, 3);
    spiel.folgeLaenge = 2;
    folgeVorspielen();
  }
  bildschirmEin();
}

// Nach einem Versuch: Treffer zählen. false = das war die letzte Runde.
bool naechsteRunde(bool treffer) {
  if (treffer) {
    spiel.treffer++;
    piep(TON(TON_MUENZE));
    evi.reaktion = R_SPIELEN;
    evi.reaktionBisTakt = evi.takt + 6;
  } else {
    piep(TON(TON_DANEBEN));
  }
  if (spiel.runde >= R::SPIEL_RUNDEN) {
    spiel.beschaeftigt = true;           // nach der letzten Runde zählt nichts mehr
    spielPlanen(SE_ENDE, 0, 900);
    return false;
  }
  return true;
}

// ---------- Ballfangen ----------
// Der Ball läuft hin und her, mit echter Zeit, damit er auch bei langsamem Zeichnen sein Tempo hält.
void ballBewegen() {
  const uint32_t m = millis();
  float dt = (m - spiel.letzteZeit) / 1000.0f;
  if (dt > 1) dt = 1;
  spiel.letzteZeit = m;
  if (spiel.pause > 0) {
    const float genutzt = spiel.pause < dt ? spiel.pause : dt;
    spiel.pause -= genutzt;
    dt -= genutzt;
  }
  float weg = spiel.tempo * dt;
  while (weg > 0) {
    const float platz = spiel.richtung > 0 ? BALL_BIS - spiel.x : spiel.x - BALL_VON;
    const float schritt = platz < weg ? platz : weg;
    spiel.x += spiel.richtung * schritt;
    weg -= schritt;
    if (weg > 0 || schritt == platz) spiel.richtung = -spiel.richtung;
  }
}

void ballFangen() {
  if (spiel.pause > 0 || spiel.beschaeftigt) return;
  ballBewegen();
  const float mitte = spiel.x + 7;
  const bool treffer = mitte >= FELD_VON && mitte <= FELD_BIS;
  snprintf(spiel.notiz, sizeof(spiel.notiz), "%s", treffer ? "Gefangen!" : "Daneben!");
  spiel.pause = 0.7f;
  if (naechsteRunde(treffer)) {
    spiel.tempo *= 1.2f;
    spielPlanen(SE_BALL_WEITER, 0, 700);
  }
}

// ---------- Links oder rechts? ----------
void raten(int seite) {
  if (spiel.beschaeftigt) return;
  spiel.beschaeftigt = true;
  const bool links = random(0, 2) == 0;
  spiel.eviX = links ? -96 : 96;
  reagiere(R_SPIELEN, 8);
  const bool treffer = (seite == ST_LINKS) == links;
  snprintf(spiel.notiz, sizeof(spiel.notiz), "%s %s ist nach %s gesprungen.", treffer ? "Richtig!" : "Leider nicht.",
           pet.name, links ? "links" : "rechts");
  const bool mehr = naechsteRunde(treffer);
  spielPlanen(SE_RATEN_ZURUECK, mehr ? 1 : 0, 1300);
}

// ---------- Kekse fangen ----------
// Kekse fallen von oben; Evi läuft mit ◀ ▶ oder durch Kippen hin und fängt sie mit dem Mund.
void keksEviSetzen(float x) {
  if (x < KEKS_EVI_MIN) x = KEKS_EVI_MIN;
  if (x > KEKS_EVI_MAX) x = KEKS_EVI_MAX;
  if (x > spiel.eviGenau + 0.5f) spiel.eviRechts = true;
  else if (x < spiel.eviGenau - 0.5f) spiel.eviRechts = false;
  spiel.eviGenau = x;
  spiel.eviX = (int)lroundf(x);
  geher.flip = spiel.eviRechts;          // Evi schaut, wohin sie läuft (gezeichnet nach links)
}

void keksBewegen() {
  const uint32_t m = millis();
  float dt = (m - spiel.letzteZeit) / 1000.0f;
  if (dt > 0.2f) dt = 0.2f;
  spiel.letzteZeit = m;
  // Kippen bewegt Evi (kleine Neigungen zählen nicht, damit sie ruhig steht)
  const float neigung = lageKippen();
  if (fabsf(neigung) > 0.08f) keksEviSetzen(spiel.eviGenau + neigung * KEKS_KIPP_TEMPO * dt);
  if (!spiel.keksFaellt || spiel.beschaeftigt) return;
  if (spiel.pause > 0) { spiel.pause -= dt; return; }
  const float vorher = spiel.ky;
  spiel.ky += spiel.kTempo * dt;
  // Auf Höhe ihres Mundes: Ist Evi da, fängt sie ihn.
  const float mund = eviOben + eviHoehe * 0.4f;
  if (vorher < mund && spiel.ky >= mund && fabsf(spiel.kx - (BODEN_X + spiel.eviX)) <= KEKS_FANGBREITE) {
    spiel.keksFaellt = false;
    snprintf(spiel.notiz, sizeof(spiel.notiz), "Mampf!");
    if (naechsteRunde(true)) spielPlanen(SE_KEKS_WEITER, 0, 800);
    return;
  }
  if (spiel.ky >= 184 - KEKS_R) {   // auf den Boden gefallen
    spiel.keksFaellt = false;
    snprintf(spiel.notiz, sizeof(spiel.notiz), "Daneben!");
    if (naechsteRunde(false)) spielPlanen(SE_KEKS_WEITER, 0, 800);
  }
}

void keksTaste(int k) {
  if (k == ST_MITTE) { spielEnde(true); return; }
  keksEviSetzen(spiel.eviGenau + (k == ST_LINKS ? -KEKS_SCHRITT : KEKS_SCHRITT));
}

// ---------- Schnick, Schnack, Schnuck ----------
// Du wählst Schere, Stein oder Papier; Evi zählt „Schnick, Schnack, Schnuck!“ und zeigt ihres.
// Unentschieden zählt nicht: dann noch einmal.
void schnickWaehlen(int wahl) {
  if (spiel.beschaeftigt) return;
  spiel.beschaeftigt = true;
  spiel.meins = wahl;
  spiel.evis = -1;
  spiel.leuchtet = wahl;
  spiel.leuchtetBis = millis() + 60000;
  snprintf(spiel.notiz, sizeof(spiel.notiz), "Schnick …");
  tonEinzeln(523, 90);
  reagiere(R_SPIELEN, 10);                  // drei Hüpfer: Schnick, Schnack, Schnuck
  spielPlanen(SE_SCHNICK_WORT, 1, 420);
  spielPlanen(SE_SCHNICK_WORT, 2, 840);
  spielPlanen(SE_SCHNICK_ZEIGEN, 0, 1300);
}

void schnickZeigen() {
  spiel.evis = random(0, 3);
  const int a = spiel.meins, b = spiel.evis;
  if (a == b) {
    snprintf(spiel.notiz, sizeof(spiel.notiz), "Beide %s! Unentschieden, nochmal.", SCHNICK_NAMEN[a]);
    tonEinzeln(659, 120);
    spielPlanen(SE_SCHNICK_WEITER, 0, 1700);
    return;
  }
  const bool gewonnen = (a - b + 3) % 3 == 1;
  if (gewonnen) snprintf(spiel.notiz, sizeof(spiel.notiz), "Gewonnen! %s", schnickSatz(a));
  else snprintf(spiel.notiz, sizeof(spiel.notiz), "%s gewinnt! %s", pet.name, schnickSatz(b));
  if (naechsteRunde(gewonnen)) spielPlanen(SE_SCHNICK_WEITER, 1, 1900);
}

// ---------- Melodie merken ----------
void feldLeuchten(int feld, int ms) {
  spiel.leuchtet = feld;
  spiel.leuchtetBis = millis() + ms;
  tonEinzeln(FELD_TOENE[feld], ms);
}

void folgeVorspielen() {
  spiel.beschaeftigt = true;
  spiel.schritt = 0;
  snprintf(spiel.notiz, sizeof(spiel.notiz), "Hör gut zu … (%d Töne)", spiel.folgeLaenge);
  for (int i = 0; i < spiel.folgeLaenge; i++) spielPlanen(SE_TON, spiel.folge[i], 700 + i * 560);
  spielPlanen(SE_DU_BIST_DRAN, 0, 700 + spiel.folgeLaenge * 560);
}

void melodieTaste(int feld) {
  if (spiel.beschaeftigt) return;
  feldLeuchten(feld, 220);
  if (feld != spiel.folge[spiel.schritt]) {
    // Ein falscher Ton beendet das Spiel; geschaffte Runden zählen trotzdem.
    spiel.beschaeftigt = true;
    snprintf(spiel.notiz, sizeof(spiel.notiz), "Oh, falscher Ton!");
    piep(TON(TON_DANEBEN));
    spielPlanen(SE_ENDE, 0, 1000);
    return;
  }
  spiel.schritt++;
  if (spiel.schritt < spiel.folgeLaenge) return;
  spiel.beschaeftigt = true;
  snprintf(spiel.notiz, sizeof(spiel.notiz), "Super!");
  if (naechsteRunde(true) && spiel.folgeLaenge < 8) {
    spiel.folge[spiel.folgeLaenge++] = random(0, 3);
    spielPlanen(SE_MELODIE_WEITER, 0, 800);
  }
}

// ---------- Ende ----------
void spielEnde(bool abgebrochen) {
  if (!spiel.aktiv) return;
  spiel.aktiv = false;
  spielPlanLeeren();
  spiel.eviX = 0;
  spiel.leuchtet = -1;
  spiel.meins = spiel.evis = -1;
  geher.flip = false;
  if (abgebrochen) { zeigeMeldung("Spiel abgebrochen"); return; }
  const ms_t jetzt = jetztMs();
  // Die neuen Spiele erfüllen keinen Wunsch (die Wünsche bleiben wie in der App)
  const Wunschart w = spiel.art == SPIEL_BALL ? W_BALL : spiel.art == SPIEL_RATEN ? W_RATEN :
                      spiel.art == SPIEL_MELODIE ? W_MELODIE : (Wunschart)255;
  const Ergebnis r = spielBeenden(pet, spiel.treffer, jetzt, w);
  char text[100];
  if (r.wunsch) { sage("Danke!", &SYM_W_HERZ, 3000); piep(TON(TON_MUENZE)); }
  if (r.geaendert) {
    reagiere(R_SPIELEN);
    const char* geschafft = spiel.art == SPIEL_BALL ? "gefangen" : spiel.art == SPIEL_RATEN ? "richtig geraten" :
                            spiel.art == SPIEL_KEKSE ? "Kekse gefangen" : spiel.art == SPIEL_SCHNICK ? "gewonnen" : "Melodien geschafft";
    snprintf(text, sizeof(text), "%d von %d %s · +%d Münzen", spiel.treffer, R::SPIEL_RUNDEN, geschafft, (int)r.muenzen);
    zeigeMeldung(text, 4000);
    speichern(jetzt);
  } else {
    grundText(r.grund, SPIELEN, text, sizeof(text));
    zeigeMeldung(text);
  }
}

void spielEreignis(SpielEreignis was, int wert) {
  switch (was) {
    case SE_ENDE: spielEnde(false); break;
    case SE_BALL_WEITER: spiel.runde++; spiel.notiz[0] = 0; break;
    case SE_RATEN_ZURUECK:
      spiel.eviX = 0;
      spiel.beschaeftigt = false;
      if (wert) { spiel.runde++; rateFrage(); }
      break;
    case SE_TON: feldLeuchten(wert, 380); break;
    case SE_DU_BIST_DRAN:
      spiel.beschaeftigt = false;
      snprintf(spiel.notiz, sizeof(spiel.notiz), "Jetzt du!");
      break;
    case SE_MELODIE_WEITER: spiel.runde++; folgeVorspielen(); break;
    case SE_KEKS_WEITER:
      spiel.runde++;
      spiel.notiz[0] = 0;
      spiel.kTempo *= 1.2f;
      keksFallenLassen();
      break;
    case SE_SCHNICK_WORT:
      snprintf(spiel.notiz, sizeof(spiel.notiz), "%s", wert == 1 ? "Schnack …" : "Schnuck!");
      tonEinzeln(wert == 1 ? 587 : 784, 90);
      break;
    case SE_SCHNICK_ZEIGEN: schnickZeigen(); break;
    case SE_SCHNICK_WEITER:
      spiel.meins = spiel.evis = -1;
      spiel.leuchtet = -1;
      spiel.beschaeftigt = false;
      if (wert) spiel.runde++;
      schnickFrage();
      break;
    default: break;
  }
}

// So oft wie möglich aufrufen: Ball bewegen, Felder ausschalten, Geplantes erledigen.
void spielTakt() {
  if (!spiel.aktiv) return;
  const uint32_t m = millis();
  if (spiel.art == SPIEL_BALL) ballBewegen();
  if (spiel.art == SPIEL_KEKSE) keksBewegen();
  if (spiel.leuchtet >= 0 && (int32_t)(m - spiel.leuchtetBis) >= 0) spiel.leuchtet = -1;
  for (int i = 0; i < spiel.planAnzahl;) {
    if ((int32_t)(m - spiel.plan[i].um) < 0) { i++; continue; }
    const SpielPlan p = spiel.plan[i];
    for (int j = i; j < spiel.planAnzahl - 1; j++) spiel.plan[j] = spiel.plan[j + 1];
    spiel.planAnzahl--;
    spielEreignis(p.was, p.wert);
    if (!spiel.aktiv) return;
    i = 0;
  }
}

// Die drei Tasten der App (und Abbruch)
void spielTaste(int k) {
  if (!spiel.aktiv) return;
  if (k == ST_ABBRUCH) { spielEnde(true); return; }
  if (spiel.art == SPIEL_BALL) {
    if (k == ST_RECHTS) spielEnde(true);
    else ballFangen();
  } else if (spiel.art == SPIEL_RATEN) {
    if (k == ST_MITTE) spielEnde(true);
    else raten(k);
  } else if (spiel.art == SPIEL_KEKSE) {
    keksTaste(k);
  } else if (spiel.art == SPIEL_SCHNICK) {
    schnickWaehlen(k);
  } else {
    melodieTaste(k);
  }
}

// Die echten Tasten des Retro Gamers im Spiel
void spielKnopf(int k) {
  if (k == K_B || k == K_START) { spielTaste(ST_ABBRUCH); return; }
  if (k == K_RESET) return;          // RESET schaltet den Bildschirm (knopf in evi-handy.ino)
  if (spiel.art == SPIEL_BALL) {
    spielTaste(ST_LINKS);                                  // A und das ganze Steuerkreuz fangen
  } else if (spiel.art == SPIEL_RATEN) {
    if (k == K_L) spielTaste(ST_LINKS);
    else if (k == K_R) spielTaste(ST_RECHTS);
  } else if (spiel.art == SPIEL_KEKSE) {
    if (k == K_L) spielTaste(ST_LINKS);
    else if (k == K_R) spielTaste(ST_RECHTS);
  } else {
    spielTaste(k == K_L ? ST_LINKS : k == K_R ? ST_RECHTS : ST_MITTE);
  }
}

// Text für die Zeile unten (renderCaption in app.js)
void spielText(char* aus, size_t n) {
  if (spiel.art == SPIEL_BALL) {
    if (spiel.notiz[0]) snprintf(aus, n, "%s", spiel.notiz);
    else snprintf(aus, n, "Ball %d/%d · %d gefangen · %s", spiel.runde, R::SPIEL_RUNDEN, spiel.treffer,
                  tastenDa ? "B beendet" : "rechts beendet");
  } else if (spiel.art == SPIEL_KEKSE) {
    if (spiel.notiz[0]) snprintf(aus, n, "%s", spiel.notiz);
    else snprintf(aus, n, "Keks %d/%d · %d gefangen · %s", spiel.runde, R::SPIEL_RUNDEN, spiel.treffer,
                  lage.uAchse >= 0 ? "kippen oder ◀▶" : tastenDa ? "◀▶ laufen" : "links/rechts laufen");
  } else {
    snprintf(aus, n, "%d/%d · %s", spiel.runde, R::SPIEL_RUNDEN, spiel.notiz);
  }
}

// ---------- Zeichnen (wie die .minigame-Gruppen in index.html) ----------

void spielHintergrund() {
  // Spiele nutzen den ganzen Bildschirm: weiß mit einer Bodenlinie
  bild.fillRect(0, 0, BREITE, 224, 1);
  bild.fillRect(0, 184, BREITE, 2, 0);
}

void spielZeichnen() {
  if (spiel.art == SPIEL_BALL) {
    bild.fillRect(35, 21, 250, 28, 0);
    bild.fillRect(37, 23, 246, 24, 1);
    // gepunktetes Fangfeld
    for (int y = 25; y < 45; y++)
      for (int x = 147; x < 173; x++)
        if (((x + y) & 1) == 0) bild.drawPixel(x, y, 0);
    bild.drawRect(146, 24, 28, 22, 0);
    const int bx = ((int)(spiel.x / 2 + 0.5f)) * 2, by = 28;
    for (int y = 0; y < 14; y++) {
      bild.drawFastHLine(bx + BALL_ZEILEN[y][0], by + y, BALL_ZEILEN[y][1] - BALL_ZEILEN[y][0] + 1, 0);
      if (y >= 3 && y <= 5) bild.drawFastHLine(bx + 4, by + y, 3, 1);   // Glanzloch
    }
  } else if (spiel.art == SPIEL_RATEN) {
    bild.fillTriangle(14, 60, 34, 42, 34, 78, 0);
    bild.fillTriangle(306, 60, 286, 42, 286, 78, 0);
  } else if (spiel.art == SPIEL_KEKSE) {
    if (spiel.keksFaellt) {
      // Ein Keks: weiß mit schwarzem Rand und Krümeln
      const int x = (int)lroundf(spiel.kx), y = (int)lroundf(spiel.ky);
      bild.fillCircle(x, y, KEKS_R, 0);
      bild.fillCircle(x, y, KEKS_R - 2, 1);
      bild.fillRect(x - 4, y - 3, 2, 2, 0);
      bild.fillRect(x + 2, y - 4, 2, 2, 0);
      bild.fillRect(x + 3, y + 1, 2, 2, 0);
      bild.fillRect(x - 3, y + 3, 2, 2, 0);
      bild.fillRect(x, y, 2, 2, 0);
    }
  } else if (spiel.art == SPIEL_SCHNICK) {
    // Drei Felder: Schere, Stein, Papier. Gewählt = schwarz.
    static const int FELD_X[3] = {30, 128, 226};
    for (int i = 0; i < 3; i++) {
      bild.fillRect(FELD_X[i] - 1, 29, 67, 43, 0);
      const bool an = spiel.leuchtet == i;
      if (!an) bild.fillRect(FELD_X[i] + 2, 32, 61, 37, 1);
      zeichneZeichen(i, FELD_X[i] + 32, 50, 13, an ? 1 : 0, an ? 0 : 1);
    }
    // Evis Wahl in einer Blase über ihrem Kopf
    if (spiel.evis >= 0) {
      const int bx = 234, by = 82, bw = 44, bh = 40;
      bild.fillRoundRect(bx, by, bw, bh, 8, 1);
      bild.drawRoundRect(bx, by, bw, bh, 8, 0);
      bild.drawRoundRect(bx + 1, by + 1, bw - 2, bh - 2, 7, 0);
      bild.fillTriangle(bx + 8, by + bh - 2, bx + 4, by + bh + 8, bx + 18, by + bh - 2, 1);
      bild.drawLine(bx + 8, by + bh - 1, bx + 4, by + bh + 8, 0);
      bild.drawLine(bx + 4, by + bh + 8, bx + 18, by + bh - 1, 0);
      zeichneZeichen(spiel.evis, bx + bw / 2, by + bh / 2, 13, 0, 1);
    }
  } else {
    static const int FELD_X[3] = {30, 128, 226};
    for (int i = 0; i < 3; i++) {
      bild.fillRect(FELD_X[i] - 1, 29, 67, 43, 0);
      if (spiel.leuchtet != i) bild.fillRect(FELD_X[i] + 2, 32, 61, 37, 1);
    }
    bild.fillTriangle(54, 50, 64, 42, 64, 58, 0);
    bild.fillRect(156, 46, 8, 8, 0);
    bild.fillTriangle(266, 50, 256, 42, 256, 58, 0);
  }
}
