// Kuscheln: Evi antippen oder mit dem Finger über sie streichen (Wunsch der Nutzerin vom 24.11.2026).
// Seitdem ist Touch keine Taste mehr. Nur wenn die echten Tasten fehlen, gelten als Notlösung die drei
// Drittel wie früher (evi-handy.ino, loop).
//
//  - Wach: Evi strahlt (ihr glückliches Bild), wiegt sich sanft hin und her, Herzen steigen auf, und sie sagt
//    etwas Liebes. Die Stimmung steigt um 5, an verschmusten Tagen um 8 (Regeln in pet.h: kuscheln),
//    höchstens einmal pro Minute; öfter Antippen zeigt nur Herzen. Finger auf Evi lassen = weiter kuscheln.
//  - Tageslaune „verschmust“: Evi möchte öfter von selbst kuscheln (Moment „Kuschelwunsch“: „Kuschelst du mit
//    mir?“, Herz in der Gedankenwolke). Wer sie dann antippt, macht ihr eine besondere Freude: +10 und ein Ton.
//    An anderen Tagen kommt der Kuschelwunsch selten.
//  - Schläft sie, kuschelt sie sich nur im Schlaf an (ein Herz, sie wacht nicht auf, keine Wirkung).
//  - Das Ei wackelt ein wenig. Nach dem Tod passiert nichts.
//  - Bei Besuch, Gassi, dem rollenden Ball und beim Trösten gibt es nur Herzen; das läuft weiter.
//    Andere Momente hören auf; Evi bleibt beim Kuscheln stehen, wo sie ist, und geht danach zurück.
//  - Nicht im Menü, in Spielen, bei Gute Nacht und vor dem Sideboard.

const uint32_t KUSCHEL_DAUER = 2600, KUSCHEL_WUNSCH_DAUER = 3600;   // ms
const uint32_t HERZ_STEIGT = 1400;                                  // so lange steigt ein Herz auf (ms)
const int KUSCHEL_RAND = 12;   // so viele Punkte neben Evi zählen noch als „auf Evi“ (Touch durch die Scheibe)

// Liegt der Punkt auf Evi (oder dem Ei)? Ihr Bild vom letzten Zeichnen, mit etwas Rand.
bool aufEvi(int x, int y) {
  return x >= eviLinks - KUSCHEL_RAND && x < eviLinks + eviBreite + KUSCHEL_RAND && y >= eviOben - KUSCHEL_RAND &&
         y < BODEN_Y + 6;
}

bool kuschelnLaeuft() { return kuschel.seit && (int32_t)(millis() - kuschel.bis) < 0; }

void herzDazu() {
  static const int8_t VERSATZ[6] = {-24, 8, -10, 16, -30, 0};
  const uint8_t i = kuschel.naechstes++ % 8;
  kuschel.herzStart[i] = millis();
  kuschel.herzX[i] = VERSATZ[kuschel.herzZahl++ % 6];
  kuschel.letztesHerz = millis();
}

// Herzen zeigen: klein = nur Herzen (Evi bleibt, wie sie ist), gross = erfüllter Kuschelwunsch
void kuschelZeigen(bool klein, bool gross, uint32_t dauer) {
  const uint32_t m = millis();
  const bool neu = !kuschelnLaeuft();
  if (neu) { kuschel.seit = m; kuschel.klein = klein; kuschel.gross = gross; herzDazu(); }
  if (!klein) kuschel.klein = false;
  if (gross) kuschel.gross = true;
  if (neu || (int32_t)(m + dauer - kuschel.bis) > 0) kuschel.bis = m + dauer;
}

// Was Evi sagt, wenn du sie kuschelst
const char* kuschelText(Stimmung s, int laune) {
  switch (s) {
    case S_KRANK:
    case S_BAUCH: return "Danke … das tut gut.";
    case S_HUNGRIG: return "Schön! Aber ich hab Hunger …";
    case S_SCHMUTZIG: return "Hihi! Ich bin ganz schmutzig!";
    case S_TRAURIG: return "Danke … das tut gut ♥";
    default: break;
  }
  static const char* const VERSCHMUST[4] = {"Mehr kuscheln! ♥", "Ich hab dich sooo lieb!", "Schnurr … ♥", "Noch mal! ♥"};
  static const char* const SONST[4] = {"Hihi! ♥", "Schön kuschelig!", "Hab dich lieb! ♥", "Das ist schön!"};
  return (laune == LAUNE_VERSCHMUST ? VERSCHMUST : SONST)[random(0, 4)];
}

// Evi wurde angetippt (oder der Finger kam beim Streicheln auf sie)
void kuschelnBeginnen() {
  const ms_t jetzt = jetztMs();
  const Phase ph = phaseBei(pet, jetzt);
  if (ph == TOT) return;
  const bool laeuftSchon = kuschelnLaeuft();
  const Stimmung s = beschreiben(pet, jetzt);
  char text[64];
  if (ph == EI || evi.schluepfen) {
    evi.wackelnBis = evi.takt + 8;
    kuschelZeigen(true, false, 900);
    if (!laeuftSchon) zeigeMeldung("Das Ei ist ganz warm.", 2400);
    return;
  }
  if (s == S_SCHLAEFT) {
    kuschelZeigen(true, false, 900);
    snprintf(text, sizeof(text), "%s kuschelt sich im Schlaf an dich.", pet.name);
    if (!laeuftSchon) zeigeMeldung(text, 3000);
    return;
  }
  if (geweckt(pet, jetzt)) wachHalten(pet, jetzt);   // wie jede Berührung in der App
  const bool wunsch = moment.art == M_KUSCHELN;
  const bool besonders = moment.art == M_FALTER || moment.art == M_VOGEL || moment.art == M_STERN || moment.art == M_GASSI ||
                         moment.art == M_ROLLEN || moment.art == M_TROESTEN;
  const int laune = launeHeute(jetzt);
  // Stimmung: höchstens einmal pro Minute, der erfüllte Kuschelwunsch immer
  if (wunsch || kuschel.letzteWirkung == 0 || jetzt - kuschel.letzteWirkung >= R::KUSCHEL_PAUSE) {
    const double plus = wunsch ? R::KUSCHEL_LAUNE_WUNSCH : laune == LAUNE_VERSCHMUST ? R::KUSCHEL_LAUNE_VERSCHMUST : R::KUSCHEL_LAUNE;
    const Tier vorher = pet;
    if (kuscheln(pet, plus, jetzt).geaendert) kuschel.letzteWirkung = jetzt;
    melde(vorher, jetzt);
  }
  if (besonders) { kuschelZeigen(true, false, 1500); return; }
  // Was Evi gerade selbst tut, hört auf. Sie bleibt, wo sie ist, und geht nach dem Kuscheln zurück.
  if (moment.art != M_KEINER) {
    const int x = geher.x;
    const bool flip = geher.flip;
    momentAufraeumen();
    geher.x = x;
    geher.flip = flip;
  }
  kuschel.heim = geher.x != 0 || geher.flip;
  const uint32_t dauer = wunsch ? KUSCHEL_WUNSCH_DAUER : KUSCHEL_DAUER;
  kuschelZeigen(false, wunsch, dauer);
  if (laeuftSchon && evi.reaktion == R_KUSCHELN) {
    const uint32_t bis = evi.takt + dauer / TAKT_MS;
    if (bis > evi.reaktionBisTakt) evi.reaktionBisTakt = bis;   // weiter wiegen, ohne neu anzufangen
  } else {
    reagiere(R_KUSCHELN, dauer / TAKT_MS);                      // glückliches Bild, sanft wiegen
  }
  moment.ruheBisTakt = evi.takt + RUHE_TAKTE;
  snprintf(text, sizeof(text), "%s wird gekuschelt ♥", pet.name);
  zeigeMeldung(text, dauer);
  if (wunsch) {
    piep(TON(TON_MUENZE));
    sage("Juhu! Ich hab dich lieb! ♥", &SYM_W_HERZ, 3500);
  } else if (!laeuftSchon) {
    sage(kuschelText(s, laune), &SYM_W_HERZ, 2600);
  }
}

// Ein Tipper auf den Bildschirm (nur wenn die Tasten da sind; sonst sind es die drei Drittel)
void beruehrt(int x, int y) {
  if (sideboard.phase != SB_ZU || spiel.aktiv || menueOffen || ritual.aktiv) return;
  if (aufEvi(x, y)) kuschelnBeginnen();
}

// Der Finger bleibt auf dem Bildschirm: Streicht er über Evi, geht das Kuscheln weiter.
void streicheln(int x, int y) {
  if (sideboard.phase != SB_ZU || spiel.aktiv || menueOffen || ritual.aktiv || !aufEvi(x, y)) return;
  if (!kuschelnLaeuft()) { kuschelnBeginnen(); return; }
  if (kuschel.klein) { kuschelZeigen(true, false, 900); return; }
  kuschelZeigen(false, false, 1200);
  if (evi.reaktion == R_KUSCHELN && evi.reaktionBisTakt < evi.takt + 1200 / TAKT_MS) evi.reaktionBisTakt = evi.takt + 1200 / TAKT_MS;
  if ((int32_t)(meldungBis - kuschel.bis) < 0) meldungBis = kuschel.bis;   // „… wird gekuschelt ♥“ bleibt stehen
  moment.ruheBisTakt = evi.takt + RUHE_TAKTE;
}

// Jeder Takt (120 ms): neue Herzen, danach zurück zum Teppich
void kuschelTakt() {
  if (!kuschel.seit) return;
  const uint32_t m = millis();
  if ((int32_t)(m - kuschel.bis) < 0) {
    const uint32_t abstand = kuschel.klein ? 720 : kuschel.gross ? 240 : 360;
    if (m - kuschel.letztesHerz >= abstand) herzDazu();
    return;
  }
  if (kuschel.heim) {
    kuschel.heim = false;
    if (moment.art == M_KEINER && (geher.x != 0 || geher.flip)) momentStarten(M_HEIM, GD_KEINER);
  }
  if (m - kuschel.bis >= HERZ_STEIGT) kuschel.seit = 0;   // alle Herzen sind oben
}

// Ein Herz mit hellem Rand, damit es vor Tapete, Fenster und Evi zu sehen ist
void herzZeichnen(const Bild& h, int x, int y) {
  zeichneSymbol(h, x - 1, y, 1);
  zeichneSymbol(h, x + 1, y, 1);
  zeichneSymbol(h, x, y - 1, 1);
  zeichneSymbol(h, x, y + 1, 1);
  zeichneSymbol(h, x, y, 0);
}

// Die Herzen steigen von Evis Kopf auf und schaukeln dabei leicht
void kuschelHerzenZeichnen() {
  if (!kuschel.seit) return;
  const uint32_t m = millis();
  const int mitte = eviLinks + eviBreite / 2;
  for (int i = 0; i < 8; i++) {
    if (!kuschel.herzStart[i]) continue;
    const uint32_t alter = m - kuschel.herzStart[i];
    if (alter >= HERZ_STEIGT) { kuschel.herzStart[i] = 0; continue; }
    const Bild& h = (i % 2 == 0 || kuschel.gross) ? SYM_R_HERZ : SYM_HERZ;
    const int schaukeln = (int)lroundf(2 * sinf(alter / 180.0f + i));
    const int x = mitte + kuschel.herzX[i] + schaukeln - h.w / 2;
    const int y = eviOben + 16 - (int)(alter * 40 / HERZ_STEIGT);
    herzZeichnen(h, x, y);
  }
}
