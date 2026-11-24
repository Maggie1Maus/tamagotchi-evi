// Schritt 10, Etappe 3: Name ändern und Hilfe (in der App zwei Dialoge, hier eigene Bildschirme).
// Beide warten in einer eigenen Schleife wie „Uhr stellen“; danach geht das Spiel normal weiter.

// ---------- Name ändern ----------
// Buchstabentafel: 3 Reihen Buchstaben, darunter Umschalten, Bindestrich, Leerzeichen, Löschen, Zurück, OK.
const char* const TAFEL_GROSS[30] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
                                     "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "Ä", "Ö", "Ü", "ß"};
const char* const TAFEL_KLEIN[30] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o",
                                     "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "ä", "ö", "ü", "ß"};
// Untere Reihe: Beschriftung, erste Spalte, Breite in Spalten
const char* const TAFEL_UNTEN[6] = {"Aa", "-", "Leer", "Löschen", "Zurück", "OK"};
const uint8_t TAFEL_UNTEN_SPALTE[6] = {0, 1, 2, 4, 6, 8};
const uint8_t TAFEL_UNTEN_BREITE[6] = {1, 1, 2, 2, 2, 2};
const int T_UMSCHALTEN = 30, T_STRICH = 31, T_LEER = 32, T_LOESCHEN = 33, T_ZURUECK = 34, T_OK = 35;
const int TAFEL_X = RAND_LINKS + 6, TAFEL_Y = 92, ZELLE_B = 30, ZELLE_H = 26;

// Lage einer Taste der Tafel (0–35)
void tafelLage(int t, int& reihe, int& spalte, int& breite) {
  if (t < 30) { reihe = t / 10; spalte = t % 10; breite = 1; return; }
  reihe = 3;
  spalte = TAFEL_UNTEN_SPALTE[t - 30];
  breite = TAFEL_UNTEN_BREITE[t - 30];
}

// Welche Taste liegt in dieser Reihe und Spalte?
int tafelTasteBei(int reihe, int spalte) {
  if (reihe < 3) return reihe * 10 + spalte;
  for (int i = 5; i >= 0; i--) if (spalte >= TAFEL_UNTEN_SPALTE[i]) return 30 + i;
  return 30;
}

// Letztes Zeichen (UTF-8) entfernen
void letztesZeichenWeg(char* s) {
  int n = strlen(s);
  while (n > 0) {
    n--;
    const uint8_t c = (uint8_t)s[n];
    s[n] = 0;
    if ((c & 0xC0) != 0x80) break;   // Anfang des Zeichens erreicht
  }
}

void nameAnhaengen(char* name, const char* teil) {
  if (strlen(name) + strlen(teil) > 20) return;   // Platz im Spielstand: 20 Bytes
  strcat(name, teil);
}

void nameAendernBildschirm() {
  char name[24];
  strncpy(name, pet.name, 20);
  name[20] = 0;
  bool gross = name[0] == 0;
  int gewaehlteTaste = 0;
  char hinweis[64] = "";
  uint32_t hinweisBis = 0;
  helligkeit(HELL_PROZENT);
  tastenVergessen();
  while (true) {
    tonWeiter();
    int gedrueckt = -1;          // Taste der Tafel, die ausgelöst wird
    bool fertig = false, abbrechen = false;
    int x, y;
    if (tipper(x, y)) {
      if (y >= TAFEL_Y && y < TAFEL_Y + 4 * ZELLE_H && x >= TAFEL_X && x < TAFEL_X + 10 * ZELLE_B) {
        gedrueckt = tafelTasteBei((y - TAFEL_Y) / ZELLE_H, (x - TAFEL_X) / ZELLE_B);
        gewaehlteTaste = gedrueckt;
      }
    }
    const int k = naechsteTaste();
    int reihe, spalte, breite;
    tafelLage(gewaehlteTaste, reihe, spalte, breite);
    if (k == K_A) gedrueckt = gewaehlteTaste;
    else if (k == K_B) abbrechen = true;
    else if (k == K_START) fertig = true;
    else if (k == K_UP) gewaehlteTaste = tafelTasteBei((reihe + 3) % 4, spalte);
    else if (k == K_DOWN) gewaehlteTaste = tafelTasteBei((reihe + 1) % 4, spalte);
    else if (k == K_L || k == K_R) {
      const int erste = reihe < 3 ? reihe * 10 : 30, anzahl = reihe < 3 ? 10 : 6;
      gewaehlteTaste = erste + (gewaehlteTaste - erste + (k == K_R ? 1 : anzahl - 1)) % anzahl;
    }

    if (gedrueckt >= 0 && gedrueckt < 30) {
      nameAnhaengen(name, gross ? TAFEL_GROSS[gedrueckt] : TAFEL_KLEIN[gedrueckt]);
      gross = false;                                   // nach dem ersten Buchstaben klein weiter
    } else if (gedrueckt == T_UMSCHALTEN) gross = !gross;
    else if (gedrueckt == T_STRICH) { nameAnhaengen(name, "-"); gross = true; }
    else if (gedrueckt == T_LEER) {
      const int n = strlen(name);
      if (n > 0 && name[n - 1] != ' ') nameAnhaengen(name, " ");
      gross = true;
    } else if (gedrueckt == T_LOESCHEN) { letztesZeichenWeg(name); if (!name[0]) gross = true; }
    else if (gedrueckt == T_ZURUECK) abbrechen = true;
    else if (gedrueckt == T_OK) fertig = true;

    if (abbrechen) break;
    if (fertig) {
      // Leerzeichen am Anfang und Ende weg (renamePet in pet.js)
      char* anfang = name;
      while (*anfang == ' ') anfang++;
      int n = strlen(anfang);
      while (n > 0 && anfang[n - 1] == ' ') anfang[--n] = 0;
      if (n == 0) {
        snprintf(hinweis, sizeof(hinweis), "Bitte gib einen Namen ein.");
        hinweisBis = millis() + 2000;
      } else {
        strncpy(pet.name, anfang, 20);
        pet.name[20] = 0;
        pet.eigenerName = true;
        speichern(jetztMs());
        break;
      }
    }

    // Zeichnen
    bild.fillScreen(1);
    const char* titel = "Wie soll dein Tier heißen?";
    schreibe(bild, RAND_LINKS + (BREITE - RAND_LINKS - textBreite(titel)) / 2, 10, titel, 0, 1, true);
    bild.drawRect(TAFEL_X, 36, 10 * ZELLE_B, 40, 0);
    char zeige[28];
    snprintf(zeige, sizeof(zeige), "%s%s", name, (millis() / 500) % 2 ? "_" : " ");
    schreibe(bild, TAFEL_X + 8, 49, zeige, 0, 2);
    for (int t = 0; t < 36; t++) {
      int r, s, b;
      tafelLage(t, r, s, b);
      const int zx = TAFEL_X + s * ZELLE_B, zy = TAFEL_Y + r * ZELLE_H, zb = b * ZELLE_B;
      const bool sel = t == gewaehlteTaste && tastenDa;
      if (sel) bild.fillRect(zx, zy, zb, ZELLE_H, 0);
      bild.drawRect(zx, zy, zb, ZELLE_H, 0);
      const char* text = t < 30 ? (gross ? TAFEL_GROSS[t] : TAFEL_KLEIN[t]) : TAFEL_UNTEN[t - 30];
      const int groesse = t < 30 ? 2 : 1;
      schreibe(bild, zx + (zb - textBreite(text, groesse)) / 2 + 1, zy + (ZELLE_H - 7 * groesse) / 2, text, sel ? 1 : 0, groesse,
               t == T_OK);
    }
    const char* unten = millis() < hinweisBis ? hinweis
                        : tastenDa ? "◀▲▼▶ wählen · A tippen · START fertig · B zurück"
                                   : "Tippe auf die Buchstaben, dann auf OK.";
    schreibe(bild, RAND_LINKS + (BREITE - RAND_LINKS - textBreite(unten)) / 2, 210, unten, 0);
    zeigen();
  }
  fingerUnten = true;   // den letzten Tipper nicht als Taste werten
  letzteMeldung = millis();
  tastenVergessen();
}

// ---------- Hilfe ----------
// Wie der Hilfe-Dialog der App, angepasst an den Retro Gamer.
struct HilfeAbsatz { const char* titel; const char* text; };
const HilfeAbsatz HILFE[] = {
  {"Tasten", "Das Steuerkreuz ▲▼ wählt im Menü, A bestätigt, B geht zurück, START öffnet und schließt das Menü. "
             "Ohne Tasten geht es mit Tippen: linkes Drittel weiter, Mitte OK, rechtes Drittel zurück. "
             "Der Bildschirm bleibt an. RESET schaltet ihn aus; ist er aus, macht ihn der erste Druck oder Tipper "
             "nur wieder an. Bei wichtigen Ereignissen und bei Besuch geht er von selbst an."},
  {"Anzeige", "Napf, Herz und Glanz zeigen Sättigung, Stimmung und Sauberkeit. Die Bedürfnisse verändern sich mit "
              "echter Zeit, auch wenn der Bildschirm aus ist."},
  {"Lebenslauf", "Nach 30 Minuten schlüpft Evi aus dem Ei. Als Baby braucht sie etwa alle 1–2 Stunden Pflege, nach "
                 "einem Tag als Teenager alle 2–3 Stunden, danach 5 Tage als Erwachsene 3–4 Mal am Tag. Zum Schluss ist "
                 "sie 2 Tage lang Oma und schläft dann friedlich für immer ein."},
  {"Nachts", "Evi schläft von 23 bis 7 Uhr. Mach ihr das Licht aus, dann schläft sie besser und freut sich. Ab 22 Uhr "
             "ist sie müde, morgens bis halb acht auch noch."},
  {"Gute Nacht", "Ab 22 Uhr kannst du Evi ins Bett bringen: Zähne putzen, eine Geschichte, zudecken, Licht aus. Jede "
                 "Taste überspringt den Ablauf. Evi schläft dann sofort, freut sich und ist am nächsten Morgen "
                 "ausgeschlafen statt müde."},
  {"Essen", "Frühstück (7–9 Uhr), Mittagessen (11–14 Uhr) und Abendessen (17–20 Uhr) machen satter und froher als "
            "Füttern zwischendurch. Naschen macht glücklich, aber mehr als 3 Süßigkeiten am Tag geben 2 Stunden "
            "Bauchweh. Medizin hilft sofort."},
  {"Wünsche", "Ein paar Mal am Tag wünscht sich Evi etwas, zum Beispiel ein bestimmtes Spiel. Erfüllst du den Wunsch "
              "innerhalb einer Stunde, bekommst du 3 Münzen und Evi freut sich. Verpasst du ihn, ist das nicht schlimm."},
  {"Spielen", "Im Menü gibt es fünf Spiele mit je 5 Runden. Jeder Erfolg bringt 2 Münzen. "
              "Ballfangen: Drück A, wenn der Ball im gepunkteten Feld ist. "
              "Links oder rechts?: Rate mit ◀ oder ▶, wohin Evi springt. "
              "Melodie merken: Evi spielt Töne auf drei Feldern vor. Spiel sie mit ◀, A und ▶ nach; ein falscher Ton "
              "beendet das Spiel. "
              "Kekse fangen: Kekse fallen von oben. Lauf mit ◀ ▶ hin oder kipp das Gerät, dann fängt Evi sie mit dem Mund. "
              "Schnick, Schnack: Schere (◀), Stein (A) oder Papier (▶). Schere schneidet Papier, Papier wickelt Stein "
              "ein, Stein macht Schere stumpf. Bei Gleichstand gibt es eine neue Runde. "
              "B beendet jedes Spiel vorzeitig."},
  {"Evi spürt Bewegung", "Gassi gehen: Nimm Evi beim Spazierengehen mit. Sie zählt die Schritte und trabt mit, wenn "
                         "du hinschaust. Ab 100 Schritten freut sie sich nach dem Spaziergang (+10 Stimmung, mehr bei "
                         "langen Wegen, 1 Münze je 500 Schritte), wird aber etwas hungrig und hat schmutzige Pfoten. "
                         "Kippen: Ein Ball rollt zur tieferen Seite, und Evi rennt "
                         "hinterher. Aufs Gesicht legen: Der Bildschirm geht aus, nachts auch das Licht für Evi. "
                         "Umgedreht geht er wieder an. Kippen und Hinlegen kennt Evi, sobald du ein paarmal Tasten "
                         "gedrückt hast, während du das Gerät schräg in der Hand hältst."},
  {"Wie war dein Tag?", "Abends zwischen 18 und 23 Uhr fragt Evi einmal, wie dein Tag war. Antworte mit Gut, So lala "
                        "oder Nicht so gut. Bis du antwortest, steht die Frage auch im Menü."},
  {"Evi tut auch selbst etwas", "Als Baby, Teenager, Erwachsene und Oma läuft sie ab und zu herum, schaut aus dem Fenster, summt, gähnt, dreht "
                                "sich im Kreis oder denkt an etwas. Ein- bis zweimal am Tag kommt Besuch: ein "
                                "Schmetterling oder ein Vogel, und wenn es draußen dunkel ist, eine Sternschnuppe. Dann "
                                "hörst du einen Ton, und der Bildschirm geht an. Weckst du den Bildschirm nach einer "
                                "Weile wieder, begrüßt sie dich."},
  {"Tageslaune", "Jeden Tag hat Evi eine Laune: verspielt, verschmust, verfressen oder verträumt. Sie sagt sie dir bei der "
                 "ersten Begrüßung, und je nach Laune tut sie manches öfter."},
  {"Himmel", "Im Fenster geht die Sonne auf und unter wie in Deutschland. Erst wird es dämmrig, dann dunkel mit Mond "
             "und Sternen."},
  {"Jahreszeiten und Wetter", "Der Baum vor dem Fenster zeigt die Jahreszeit: Frühling ab 1. Februar, Sommer ab 1. Juni, "
                              "Herbst ab 21. September, Winter ab 1. November. Das Wetter wechselt alle 3 Stunden: Sonne, "
                              "Sonne und Wolken, Regen oder bewölkt, im Winter auch Schnee. Vom 18. November bis 5. Januar "
                              "steht ein Weihnachtsbaum im Zimmer, Sterne hängen am Fenster und Kerzen stehen auf dem Sideboard."},
  {"Aufwecken", "Nachts kannst du Evi aufwecken, zum Beispiel wenn sie Hunger hat. Solange du Tasten drückst, bleibt sie "
                "wach. Nach 5 Minuten ohne Tastendruck schläft sie wieder ein, im selben Licht wie vorher."},
  {"Mittagsschlaf", "Das Baby schläft von 12 bis 13 Uhr. Dann bitte nicht stören."},
  {"Vorsicht", "Bleibt ein Wert lange bei null, wird Evi krank und braucht Medizin. Wer sie zu lange vergisst, verliert "
               "sie."},
  {"Name ändern", "Im Menü unter „Name ändern“: Buchstaben mit dem Steuerkreuz wählen, A tippt, START speichert, B bricht "
                  "ab. Oder einfach auf die Buchstaben tippen."},
  {"Uhr", "Solange die kleine Pufferbatterie fehlt, fragt Evi nach dem Ausstecken nach Datum und Uhrzeit. Im Menü "
          "unter „Uhr stellen“ geht es jederzeit: ▲▼ ändert, A weiter, B zurück."},
  {"Spielstand", "Evi merkt sich alles im Speicher des Boards, auch ohne Strom."},
};
const int HILFE_ZEILEN_MAX = 200, HILFE_PRO_SEITE = 16, HILFE_ZEICHEN = 50;
static char hilfeZeile[HILFE_ZEILEN_MAX][100];   // Platz für Umlaute und ▲▼ (in UTF-8 bis zu 3 Bytes je Zeichen)
static bool hilfeFett[HILFE_ZEILEN_MAX];

void hilfeBildschirm() {
  // Alle Zeilen vorbereiten: Überschrift fett, Text umbrochen, Leerzeile dazwischen
  int n = 0;
  for (const HilfeAbsatz& a : HILFE) {
    if (n + 1 < HILFE_ZEILEN_MAX) { snprintf(hilfeZeile[n], sizeof(hilfeZeile[n]), "%s", a.titel); hilfeFett[n++] = true; }
    umbrechen(a.text, HILFE_ZEICHEN, [&](const char* z) {
      if (n < HILFE_ZEILEN_MAX) { snprintf(hilfeZeile[n], sizeof(hilfeZeile[n]), "%s", z); hilfeFett[n++] = false; }
    });
    if (n < HILFE_ZEILEN_MAX) { hilfeZeile[n][0] = 0; hilfeFett[n++] = false; }
  }
  // Seitenanfänge: keine Seite beginnt mit einer Leerzeile
  int seitenAnfang[30], seiten = 0;
  for (int z = 0; z < n && seiten < 30;) {
    while (z < n && !hilfeZeile[z][0]) z++;
    if (z >= n) break;
    seitenAnfang[seiten++] = z;
    z += HILFE_PRO_SEITE;
  }
  int seite = 0;
  helligkeit(HELL_PROZENT);
  tastenVergessen();
  while (true) {
    tonWeiter();
    int richtung = 0;
    bool schliessen = false;
    int x, y;
    if (tipper(x, y)) {
      const Taste t = tasteBei(x);
      if (t == LINKS) richtung = -1;
      else if (t == RECHTS) richtung = 1;
      else schliessen = true;
    }
    const int k = naechsteTaste();
    if (k == K_L || k == K_UP) richtung = -1;
    else if (k == K_R || k == K_DOWN || k == K_A) richtung = 1;
    else if (k == K_B || k == K_START) schliessen = true;
    if (richtung > 0 && seite == seiten - 1 && k == K_A) schliessen = true;   // A auf der letzten Seite: fertig
    if (schliessen) break;
    seite = constrain(seite + richtung, 0, seiten - 1);

    bild.fillScreen(1);
    schreibe(bild, RAND_LINKS + 4, 5, "Hilfe", 0, 2, true);
    char s[24];
    snprintf(s, sizeof(s), "Seite %d/%d", seite + 1, seiten);
    schreibe(bild, BREITE - 6 - textBreite(s), 9, s, 0);
    bild.drawFastHLine(RAND_LINKS, 24, BREITE - RAND_LINKS, 0);
    const int ende = seite + 1 < seiten ? seitenAnfang[seite + 1] : n;
    for (int z = seitenAnfang[seite], i = 0; z < ende && i < HILFE_PRO_SEITE; z++, i++)
      schreibe(bild, RAND_LINKS + 4, 30 + i * 11, hilfeZeile[z], 0, 1, hilfeFett[z]);
    bild.drawFastHLine(RAND_LINKS, 210, BREITE - RAND_LINKS, 0);
    const char* unten = tastenDa ? "◀▶ blättern · B zurück" : "links zurück · Mitte schließen · rechts weiter";
    schreibe(bild, RAND_LINKS + (BREITE - RAND_LINKS - textBreite(unten)) / 2, 222, unten, 0);
    zeigen();
  }
  fingerUnten = true;
  letzteMeldung = millis();
  tastenVergessen();
}
