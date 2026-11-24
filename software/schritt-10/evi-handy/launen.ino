// Launenwünsche mit Folgen (Wunsch der Nutzerin vom 24.11.2026, nur auf dem Board, nicht in der Handy-App).
//
// Jede Tageslaune hat ihren eigenen Wunsch, ausgelöst als kleiner Moment (M_KUSCHELN, momente.ino):
//  - verspielt:  will spielen             → im Menü ein Spiel starten
//  - verschmust: will kuscheln            → Evi antippen
//  - verfressen: will etwas Leckeres      → Füttern oder Naschen
//  - verträumt:  will mit dir rausschauen → sie geht ans Fenster, Evi antippen
// Sie wartet WUNSCH_WARTEN lang. Erfüllt: +10 Stimmung, Freude, Münzton.
// Nicht erfüllt: -5 Stimmung, sie ist enttäuscht. Zwei verpasste Wünsche hintereinander: Sie ist 15 Minuten
// beleidigt (Schmollpose, kleine Regenwolke über dem Kopf, keine Spiele, Kuscheln wirkt nicht, keine
// kleinen Momente). Versöhnen: etwas zu essen geben oder sie dreimal streicheln. Danach ist alles wieder gut.
// Wünsche kommen nur, wenn in den letzten 10 Minuten jemand eine Taste gedrückt oder getippt hat.
// Alles steht nur im Arbeitsspeicher; der Spielstand bleibt unverändert.
// Zu jedem Anlass gibt es fünf Sätze, einer wird zufällig gewählt (Wunsch der Nutzerin).

const uint32_t WUNSCH_WARTEN = 45000;          // ms, so lange wartet sie auf dich
const uint32_t JEMAND_DA = 10UL * 60 * 1000;   // ms seit der letzten Bedienung
const uint32_t STREICHEL_ABSTAND = 700;        // ms zwischen zwei gezählten Streichlern beim Versöhnen

// ---------- Die Sätze ----------
const char* const WUNSCH_SAETZE[4][5] = {
  {"Spielst du mit mir?", "Mir ist sooo langweilig!", "Lass uns was spielen!", "Ich will toben!", "Komm, wir spielen was!"},
  {"Kuschelst du mit mir?", "Ich brauch eine Umarmung!", "Streichelst du mich?", "Zeit zum Kuscheln?", "Mir ist nach Kuscheln …"},
  {"Ich will einen Keks!", "Hast du was Leckeres?", "Mein Bauch knurrt …", "Nur ein Häppchen? Bitte!", "Ich riech was Leckeres!"},
  {"Schaust du mit mir raus?", "Guck mal, der Himmel!", "Kommst du mit ans Fenster?", "Träumst du mit mir?", "Wie schön es draußen ist …"},
};
const char* const WUNSCH_HINWEIS[4] = {
  "Möchte spielen · Menü: Spielen",
  "Möchte kuscheln · tipp sie an ♥",
  "Möchte was Leckeres · Menü: Naschen",
  "Möchte mit dir rausschauen · tipp sie an",
};
const char* const ERFUELLT[5] = {"Juhu! Hab dich lieb! ♥", "Genau das wollte ich! ♥", "Du bist toll! ♥", "Danke! Das war schön!",
                                 "Hach, schön! ♥"};
const char* const ENTTAEUSCHT[5] = {"Hmpf … dann eben nicht.", "Schade … keine Zeit für mich?", "Och menno …", "Pfff. Dann eben nicht!",
                                    "Na toll …"};
const char* const BELEIDIGT_WERDEN[5] = {"Jetzt bin ich beleidigt!", "Ich red nicht mehr mit dir!", "Hmpf! Du magst mich nicht!",
                                         "So! Jetzt bin ich sauer!", "Ich bin beleidigt. Basta!"};
const char* const BELEIDIGT_ANGETIPPT[5] = {"Lass mich!", "Hmpf!", "Ich bin noch beleidigt!", "Nö! Ich bin sauer!", "Ich schmolle gerade!"};
const char* const BELEIDIGT_KEIN_SPIEL[5] = {"Ich spiel nicht mit dir!", "Keine Lust! Ich bin sauer.", "Spiel doch alleine!",
                                            "Nö. Ich schmolle.", "Erst entschuldigen!"};
const char* const VERSOEHNT[5] = {"Na gut … ♥", "Ich bin nicht mehr böse.", "Schon vergessen! ♥", "Na schön. Hab dich lieb!", "Versöhnt! ♥"};
const char* const BELEIDIGT_ZEILE = "Ist beleidigt · Keks oder dreimal streicheln";

const char* beleidigtZeile() { return BELEIDIGT_ZEILE; }   // für die Zeile unten (evi-handy.ino)

const char* zufallsSatz(const char* const saetze[5]) { return saetze[random(0, 5)]; }

// ---------- Abfragen ----------
bool launenWunschOffen() { return launen.art != LW_KEINER; }
bool launenBeleidigt() { return launen.beleidigt; }

// Darf gerade ein Launenwunsch kommen? Nur mit Tasten, wenn jemand da ist, nicht beleidigt und
// nicht, während ein anderer Wunsch in der Blase steht.
bool launenWunschMoeglich(ms_t jetzt) {
  Wunsch w;
  return tastenDa && !launen.beleidigt && !launenWunschOffen() && launen.letzteBedienung != 0 &&
         millis() - launen.letzteBedienung < JEMAND_DA && !aktuellerWunsch(pet, jetzt, w) &&
         !freundFrageOffen() && !freundDialogOffen();   // nicht, während sie dich etwas fragt (freunde.ino)
}

// Ein Tipper auf Evi erfüllt den Kuschel- und den Fensterwunsch
bool launenWunschDurchTippen() { return launen.art == LW_KUSCHELN || launen.art == LW_FENSTER; }

// ---------- Ablauf ----------
// Der Moment M_KUSCHELN beginnt: Wunsch der heutigen Laune festlegen und den Ablauf planen (aus momentStarten).
void launenWunschPlanen(ms_t jetzt) {
  const LaunenWunsch art = (LaunenWunsch)launeHeute(jetzt);
  launen.art = art;
  launen.bis = millis() + WUNSCH_WARTEN;
  piep(TON(TON_WUNSCH));
  akt(A_SAGEN, 3500, 0, WUNSCH_SAETZE[art][random(0, 5)]);
  akt(A_MELDUNG, WUNSCH_WARTEN, 0, WUNSCH_HINWEIS[art]);
  if (art == LW_FENSTER) {
    akt(A_GEHEN, -84, 151, nullptr);
    akt(A_FLIP, 0, 0, nullptr);
    akt(A_BILD, MB_FENSTER, 0, nullptr);
    akt(A_DENKEN, himmelBei(jetzt) == HIMMEL_TAG ? GD_SONNE : GD_MOND, 0, nullptr);
  } else {
    akt(A_BILD, MB_STAUNEN, 0, nullptr);
    akt(A_WARTEN, 26, 0, nullptr);
    akt(A_BILD, MB_DENKEN, 0, nullptr);
    akt(A_DENKEN, art == LW_SPIELEN ? GD_BALL : art == LW_ESSEN ? GD_KEKS : GD_HERZ, 0, nullptr);
  }
  akt(A_WUNSCH_WARTEN, 0, 0, nullptr);
  akt(A_DENKEN, GD_KEINER, 0, nullptr);
  akt(A_BILD, MB_KEINS, 0, nullptr);
  akt(A_HEIM, 0, 0, nullptr);
}

// Nach einem verpassten Wunsch: traurig schauen oder schmollen, etwas sagen, zurück zum Teppich.
const char* enttaeuschtText = "";
void enttaeuschtPlanen() {
  akt(A_DENKEN, GD_KEINER, 0, nullptr);
  akt(A_BILD, launenBeleidigt() ? MB_KEINS : MB_TRAURIG, 0, nullptr);
  akt(A_SAGEN, 3500, 0, enttaeuschtText);
  akt(A_WARTEN, 30, 0, nullptr);
  akt(A_BILD, MB_KEINS, 0, nullptr);
  akt(A_HEIM, 0, 0, nullptr);
}

// Wunsch erfüllt. Die Stimmung gibt der Aufrufer (Kuscheln) oder plus = true (Spielen, Essen).
// Gibt false zurück, wenn gerade kein passender Wunsch offen war.
bool launenWunschErfuellen(LaunenWunsch passend, bool plus) {
  if (launen.art != passend) return false;
  launen.art = LW_KEINER;
  launen.verpasst = 0;
  freundWunschErfuellt();            // Freundschaft wächst (freunde.ino)
  if (plus) {
    const ms_t jetzt = jetztMs();
    const Tier vorher = pet;
    launeAendern(pet, R::KUSCHEL_LAUNE_WUNSCH, jetzt);
    melde(vorher, jetzt);
  }
  return true;
}

// Sich freuen (Satz, Herz, Münzton)
void launenFreuen() {
  piep(TON(TON_MUENZE));
  sage(zufallsSatz(ERFUELLT), &SYM_W_HERZ, 3500);
}

void launenVersoehnen() {
  launen.beleidigt = false;
  launen.streichler = 0;
  sage(zufallsSatz(VERSOEHNT), &SYM_W_HERZ, 3500);
  piep(TON(TON_MUENZE));
}

// Beleidigt angetippt: Kuscheln wirkt nicht. Drei Streichler versöhnen sie.
void launenBeleidigtAngetippt() {
  const uint32_t m = millis();
  if (launen.letzterStreichler != 0 && m - launen.letzterStreichler < STREICHEL_ABSTAND) return;
  launen.letzterStreichler = m;
  if (++launen.streichler >= 3) {
    launenVersoehnen();
    kuschelZeigen(false, true, KUSCHEL_WUNSCH_DAUER);
    reagiere(R_KUSCHELN, KUSCHEL_WUNSCH_DAUER / TAKT_MS);
    return;
  }
  sage(zufallsSatz(BELEIDIGT_ANGETIPPT), &SYM_W_HERZ, 2400);
  zeigeMeldung(BELEIDIGT_ZEILE, 3000);
}

// Ein Spiel soll starten (oder die Spielauswahl öffnen): beleidigt sagt sie nein (false).
// Erfüllt wird der Spielwunsch erst beim echten Start (launenSpielBeginnt), bedankt nach dem Spiel.
bool launenSpielErlaubt() {
  if (launen.beleidigt) {
    sage(zufallsSatz(BELEIDIGT_KEIN_SPIEL), &SYM_W_HERZ, 3000);
    zeigeMeldung(BELEIDIGT_ZEILE, 3000);
    return false;
  }
  return true;
}

// Ein Spiel startet wirklich (nicht nur die Spielauswahl): erfüllt den Spielwunsch.
void launenSpielBeginnt() {
  if (launenWunschErfuellen(LW_SPIELEN, true)) launen.dankeNachSpiel = true;
}

void launenNachSpiel() {
  if (!launen.dankeNachSpiel) return;
  launen.dankeNachSpiel = false;
  launenFreuen();
}

// Evi hat etwas zu essen bekommen (Füttern oder Naschen): erfüllt den Essenswunsch und versöhnt sie.
void launenGegessen() {
  if (launen.beleidigt) { launenVersoehnen(); return; }
  if (launenWunschErfuellen(LW_ESSEN, true)) launenFreuen();
}

// Wunsch nicht erfüllt: weniger Stimmung, beim zweiten Mal hintereinander beleidigt.
void launenVerpasst(ms_t jetzt) {
  launen.art = LW_KEINER;
  const Tier vorher = pet;
  launeAendern(pet, R::WUNSCH_VERPASST_LAUNE, jetzt);
  melde(vorher, jetzt);
  if (++launen.verpasst >= 2) {
    launen.verpasst = 0;
    launen.beleidigt = true;
    freundBeleidigt();               // Freundschaft leidet etwas (freunde.ino)
    launen.beleidigtBis = millis() + (uint32_t)R::BELEIDIGT_DAUER;
    launen.streichler = 0;
    enttaeuschtText = zufallsSatz(BELEIDIGT_WERDEN);
    piep(TON(TON_DANEBEN));
  } else {
    enttaeuschtText = zufallsSatz(ENTTAEUSCHT);
  }
  if (moment.art == M_KUSCHELN) momentAufraeumen();
  if (moment.art == M_KEINER && eviKannReagieren(jetzt)) momentStarten(M_ENTTAEUSCHT, GD_KEINER);
  else sage(enttaeuschtText, &SYM_W_HERZ, 3500);   // z. B. bei offenem Menü: nur der Satz
}

// Jeder Takt (120 ms): Ist die Wartezeit um? Ist das Beleidigtsein vorbei?
void launenTakt(ms_t jetzt) {
  if (launenWunschOffen()) {
    // Schläft sie ein, wird krank, oder beschäftigst du dich mit ihr (Spiel, Garten, Sideboard),
    // endet der Wunsch ohne Folgen.
    if (ruhend(beschreiben(pet, jetzt)) || spiel.aktiv || garten.phase != GA_ZU || sideboard.phase != SB_ZU) {
      launen.art = LW_KEINER;
    } else if ((int32_t)(millis() - launen.bis) >= 0) {
      launenVerpasst(jetzt);
    }
  }
  if (launen.beleidigt && (int32_t)(millis() - launen.beleidigtBis) >= 0) {
    launen.beleidigt = false;
    launen.streichler = 0;
    if (eviKannReagieren(jetzt)) sage(zufallsSatz(VERSOEHNT), &SYM_W_HERZ, 3500);
  }
}

// Kleine Regenwolke über dem Kopf, solange sie beleidigt ist (wach, im Zimmer)
void launenZeichnen(ms_t jetzt) {
  if (!launen.beleidigt || spiel.aktiv || ruhend(beschreiben(pet, jetzt))) return;
  const int cx = min(300, eviLinks + eviBreite / 2 + eviBreite / 3), cy = max(28, eviOben - 4);
  zeichneZeichen(4, cx, cy, 14, 0, 1);
}
