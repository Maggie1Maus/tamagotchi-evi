// Freunde im Park, Album und Evis Wesen (26.11.2026). Siehe parkfreunde.h.
// Alle Sätze: höchstens 38 Zeichen, damit „Glöckchen: “ und der Satz in eine Zeile passen.

// ---------- Evis Wesen ----------
// Jede Evi hat eins von sechs Wesen, aus ihrem Zeitpunkt „geboren“ ausgewürfelt. Es ändert sich nie.
int eviWesen() { return (int)(eviWuerfel("wesen", 0) % WESEN_ZAHL); }

// Kleine Sätze, mit denen Evi ihr Wesen verrät (beim Plaudern)
const char* const WESEN_PLAUDERN[WESEN_ZAHL][3] = {
  {"Ich bin so eine Naschkatze, ehrlich!", "Süßes ist mein Geheimnis, psst!", "Bei Keksen bin ich sofort wach!"},
  {"Ich renne so gern, bis ich schnaufe!", "Bewegung macht mich richtig froh!", "Ich wäre gern eine Sportlerin!"},
  {"Wasser mag ich nicht so gern, hihi.", "Pfützen? Lieber nicht, brr!", "Baden ist nicht meine Stärke."},
  {"Ich schlafe am liebsten ganz lange.", "Morgens bin ich noch so müde, gähn.", "Kissen sind mein Lieblingsplatz."},
  {"Kuscheln ist mein Lieblingshobby!", "Ich mag es, wenn du mich streichelst.", "Bei dir ist es so schön warm."},
  {"Ich will immer alles wissen!", "Was gibt es Neues? Erzähl mal!", "Ich schau gern, was dahinter ist."},
};
const char* wesenPlaudernSatz() { return WESEN_PLAUDERN[eviWesen()][random(0, 3)]; }

// Evi sagt etwas zu dem, was sie tut (jedes dritte Mal), je nach Wesen. art: 0 Naschen, 1 Spielen, 2 Putzen, 3 Kuscheln
void wesenSagen(int art) {
  static const char* const TEXT[4][2] = {
    {"Mmh, Naschen ist das Beste!", "Noch einen Keks, bitte!"},
    {"Noch eine Runde! Ich will rennen!", "Das macht Spaß! Nochmal!"},
    {"Brr … muss das Wasser sein?", "Hilfe, nass! Aber danke."},
    {"Schnurr … mehr kuscheln, bitte!", "Ach, so schön warm bei dir!"},
  };
  static const uint8_t PASST[4] = {W_NASCH, W_SPORT, W_WASSER, W_SCHMUSE};
  if (eviWesen() != PASST[art] || random(0, 3) != 0) return;
  sage(TEXT[art][random(0, 2)], &SYM_W_HERZ, 2500);
}

// ---------- Was gespeichert wird ----------
int parkFreundStufe(uint8_t treffer) {
  return treffer == 0 ? 0 : treffer < TREFFEN_VERTRAUT ? 1 : treffer < TREFFEN_BESTE ? 2 : 3;
}

int parkFreundGetroffen() {
  int n = 0;
  for (int i = 0; i < BESUCHER; i++) if (parkFreund.treffer[i]) n++;
  return n;
}

// Neue Evi (anderer Zeitpunkt „geboren“): alle Treffen von vorne
void parkFreundPruefen() {
  if (parkFreund.evi == pet.geboren) return;
  const uint32_t k = parkFreund.kennung;
  memset(&parkFreund, 0, sizeof(parkFreund));
  parkFreund.kennung = k ? k : PARKFREUND_KENNUNG;
  parkFreund.version = PARKFREUND_VERSION;
  parkFreund.groesse = sizeof(ParkFreundStand);
  parkFreund.evi = pet.geboren;
  parkFreundGeaendert = true;
}

// Wasserscheue Evis gehen bei Regen nicht in den Park (und gehen heim, wenn es dort anfängt zu regnen)
bool parkZuNass(ms_t jetzt) { return eviWesen() == W_WASSER && wetterBei(jetzt) == WT_REGEN; }

void parkFreundLaden() {
  uint8_t* b = (uint8_t*)&parkFreund;
  for (size_t i = 0; i < sizeof(parkFreund); i++) b[i] = EEPROM.read(PARKFREUND_ADRESSE + i);
  const bool gut = parkFreund.kennung == PARKFREUND_KENNUNG && parkFreund.version == PARKFREUND_VERSION &&
                   parkFreund.groesse == sizeof(ParkFreundStand) &&
                   parkFreund.pruefsumme == pruefsumme(b, offsetof(ParkFreundStand, pruefsumme));
  if (!gut) {
    memset(&parkFreund, 0, sizeof(parkFreund));
    parkFreund.kennung = PARKFREUND_KENNUNG;
    parkFreund.version = PARKFREUND_VERSION;
    parkFreund.groesse = sizeof(ParkFreundStand);
    parkFreund.evi = pet.geboren;
  }
  parkFreundPruefen();
}

void parkFreundSpeichern() {
  if (!parkFreundGeaendert) return;
  parkFreund.pruefsumme = pruefsumme((const uint8_t*)&parkFreund, offsetof(ParkFreundStand, pruefsumme));
  const uint8_t* b = (const uint8_t*)&parkFreund;
  bool anders = false;
  for (size_t i = 0; i < sizeof(parkFreund); i++) {
    if (EEPROM.read(PARKFREUND_ADRESSE + i) != b[i]) { EEPROM.write(PARKFREUND_ADRESSE + i, b[i]); anders = true; }
  }
  if (anders) EEPROM.commit();
  parkFreundGeaendert = false;
}

// ---------- Treffen ----------
// Wird beim Begrüßen aufgerufen (parkGrussWaehlen). Zählt das Treffen (einmal am Tag je Figur), merkt sich
// in park.stufe, wie gut Evi die Figur vorher kannte, und in park.passt, ob das Wesen übereinstimmt.
void parkFreundBegegnung(ms_t jetzt) {
  parkFreundPruefen();
  const int f = park.besuch;
  const int32_t heute = tagKennung(jetzt);
  if (parkFreund.heuteTag != heute) { parkFreund.heuteTag = heute; parkFreund.heuteMaske = 0; parkFreundGeaendert = true; }
  park.stufe = parkFreundStufe(parkFreund.treffer[f]);
  park.passt = FIGUR_WESEN[f] == eviWesen();
  if (parkFreund.heuteMaske >> f & 1) return;
  parkFreund.heuteMaske |= 1UL << f;
  if (parkFreund.treffer[f] < 255) parkFreund.treffer[f]++;
  parkFreundGeaendert = true;
  // Freundschaft mit dir: ein Treffen zählt wenig, bei gleichem Wesen mehr, neue Bekannte sind spannend (Neugierige)
  float plus = park.passt ? 2.0f : 0.5f;
  if (park.stufe == 0 && eviWesen() == W_NEUGIER) plus += 1;
  freundPlus(plus);
  speichern(jetzt);
}

// ---------- Begrüßung nach Stufe ----------
const char* const GRUSS_ERSTE[5] = {"Hallo! Du bist neu hier, oder?", "Oh, hallo! Ich kenne dich noch nicht.",
                                    "Hi, ich bin neu hier. Und du, *?", "Hallo *! Ich hab von dir gehört.",
                                    "Hallo! Wollen wir Freunde werden?"};
const char* const GRUSS_BEKANNT[5] = {"Hallo *! Wir kennen uns schon!", "Du bist ja wieder da, *!",
                                      "Schön, dich wiederzusehen, *!", "Hey *, schön, dass du kommst!", "Da bist du ja wieder!"};
const char* const GRUSS_VERTRAUT[5] = {"*! Ich hab mich auf dich gefreut!", "Hallo *! Ich hab dich vermisst!",
                                       "Juhu, *! Wie geht's dir heute?", "Schön, dass du da bist, liebe *!",
                                       "*, komm, erzähl mir was!"};
const char* const GRUSS_BESTE[5] = {"*! Meine allerbeste Parkfreundin!", "Endlich! Ich hab schon gewartet, *!",
                                    "Mein Lieblingsmensch im Park: *!", "*, du machst jeden Tag schöner!",
                                    "Beste Freunde im Park! Hallo, *!"};
const char* const* const GRUSS_STUFE[4] = {GRUSS_ERSTE, GRUSS_BEKANNT, GRUSS_VERTRAUT, GRUSS_BESTE};

const char* parkStufenGruss(int stufe, int nr) { return GRUSS_STUFE[stufe][nr]; }

// ---------- Zweiter Satz ----------
const char* const NACH_PASST[WESEN_ZAHL][3] = {
  {"Wir beide lieben Süßes, was?", "Naschkatzen halten zusammen!", "Ich hab auch immer Hunger auf Kekse."},
  {"Wir zwei sind echte Sportler!", "Los, wir rennen um die Wette!", "Du bist so flink wie ich!"},
  {"Wir mögen beide kein Wasser, hihi.", "Pfützen sind doof, oder?", "Mit dir umgehe ich jede Pfütze!"},
  {"Du schläfst auch gern lange, oder?", "Wir zwei schlafen gern lange!", "Gähn … zusammen ist es schön."},
  {"Wir zwei kuscheln beide so gern!", "Komm, wir kuscheln gleich!", "Bei dir ist es so schön kuschelig."},
  {"Wir wollen beide alles wissen!", "Ich bin auch so neugierig wie du!", "Was entdecken wir heute zusammen?"},
};
const char* const NACH_EIGEN[WESEN_ZAHL][3] = {
  {"Ich muss unbedingt was Süßes naschen.", "Ich hab immer Hunger auf Kekse.", "Ich glaub, ich bin eine Naschkatze."},
  {"Ich muss mich heute noch austoben!", "Ich bin schon dreimal gerannt!", "Sport ist mein Lieblingsspiel!"},
  {"Pfützen? Nein danke, bleib trocken.", "Ich mag kein Wasser, brr!", "Ich geh Pfützen immer aus dem Weg."},
  {"Ich bin heute noch ganz müde … gähn.", "Ausschlafen ist das Allerschönste!", "Ich könnte schon wieder schlafen."},
  {"Hast du mal Lust auf Kuscheln?", "Ich mag es, wenn man mich krault.", "Kuscheln macht mich so glücklich!"},
  {"Was ist denn da hinten? Ich guck mal!", "Ich will alles im Park erkunden!", "Hast du schon das Neueste gehört?"},
};
// Wetter, Jahreszeit und Tageszeit: 0 Weihnachten, 1 Schnee, 2 Regen, 3 Nacht, 4 Dämmerung, 5 sonniger Sommer,
// 6 Herbst, 7 Frühling, 8 Winter, 9 sonst (grau)
const char* const NACH_WETTER[10][3] = {
  {"Es riecht schon nach Weihnachten!", "Hast du auch schon Sterne gebastelt?", "Bald gibt es Plätzchen!"},
  {"Schnee! Ich liebe Schnee!", "Brr, ist das kalt und weiß!", "Schau, der Schneemann ist schon da!"},
  {"Regen! Hoffentlich bleibt es trocken.", "Platsch! Ich mag den Regenduft.", "Regentropfen auf der Nase, hihi!"},
  {"So still ist der Park bei Nacht.", "Schau, wie die Laternen leuchten!", "Nachts ist der Park ganz zauberhaft."},
  {"Gleich wird es dunkel.", "Der Himmel sieht gerade so schön aus!", "Schau, das Licht wird ganz weich."},
  {"Die Sonne ist heute herrlich warm!", "Perfekt für einen Sommertag!", "Hier unterm Baum ist es schön kühl."},
  {"Das Laub liegt überall, raschel!", "Hast du die Pilze unterm Baum gesehen?", "Der Herbst ist so bunt!"},
  {"Es blüht überall! Riechst du das?", "Frühling, endlich!", "Die Blüten am Baum sind so schön!"},
  {"Ganz schön kalt heute, was?", "Der Baum ist ganz kahl und still.", "Brr, ich brauch eine warme Mütze!"},
  {"Der Himmel ist heute ganz grau.", "Ob die Sonne noch rauskommt?", "Graues Wetter, aber du bist da!"},
};
const char* const NACH_FUND_DA[3] = {"Du hast heute was gefunden, oder?", "Zeig mal deinen Fund von heute!", "Was Schönes im Park gefunden, hm?"};
const char* const NACH_FUND_NOCH[3] = {"Ich glaub, am Weg liegt was!", "Schau mal, da glitzert etwas!", "Hast du schon nach Funden geschaut?"};

int parkWetterArt(ms_t jetzt) {
  const Himmel h = himmelBei(jetzt);
  const Wetter w = wetterBei(jetzt);
  const Jahreszeit j = jahreszeitBei(jetzt);
  if (w == WT_SCHNEE) return 1;
  if (w == WT_REGEN) return 2;
  if (h == HIMMEL_NACHT) return 3;
  if (h == HIMMEL_DAEMMERUNG) return 4;
  if (weihnachtsZeit(jetzt)) return 0;
  if (j == JZ_SOMMER && (w == WT_SONNE || w == WT_SONNE_WOLKEN)) return 5;
  if (j == JZ_HERBST) return 6;
  if (j == JZ_FRUEHLING) return 7;
  if (j == JZ_WINTER) return 8;
  return 9;
}

// Wählt den zweiten Satz: 0 passt zu Evis Wesen, 1 über sich selbst, 2 Wetter/Jahreszeit, 3 Funde
void parkNachsatzWaehlen(ms_t jetzt) {
  const int stelle = parkFundstelle(jetzt);
  const bool fundDa = stelle == -1, fundNoch = stelle >= 0 && sammlung.anzahl < SAMMLUNG_MAX;
  const int gew[4] = {park.passt ? 4 : 0, 2, 3, (fundDa || fundNoch) ? 2 : 0};
  int r = random(0, gew[0] + gew[1] + gew[2] + gew[3]);
  int art = 0;
  while (art < 3 && r >= gew[art]) { r -= gew[art]; art++; }
  park.nachArt = art;
  park.nachSatz = art == 2 ? parkWetterArt(jetzt) * 3 + random(0, 3) : art == 3 ? (fundDa ? 0 : 3) + random(0, 3) : random(0, 3);
}

const char* parkNachsatz() {
  const int f = park.besuch;
  switch (park.nachArt) {
    case 0: return NACH_PASST[FIGUR_WESEN[f]][park.nachSatz];
    case 1: return NACH_EIGEN[FIGUR_WESEN[f]][park.nachSatz];
    case 2: return NACH_WETTER[park.nachSatz / 3][park.nachSatz % 3];
    default: return park.nachSatz < 3 ? NACH_FUND_DA[park.nachSatz] : NACH_FUND_NOCH[park.nachSatz - 3];
  }
}

// „Name: Satz“ mit Evis Namen für das *
void parkBesuchSatz(char* ziel, size_t platz, const char* satz) {
  size_t n = snprintf(ziel, platz, "%s: ", BESUCH_NAME[park.besuch]);
  for (const char* s = satz; *s && n + 1 < platz; s++) {
    if (*s == '*') n += snprintf(ziel + n, platz - n, "%s", pet.name[0] ? pet.name : "Evi");
    else ziel[n++] = *s;
    if (n >= platz) n = platz - 1;
  }
  ziel[n] = 0;
}

// ---------- Album ----------
static const int ALBUM_KOPF_Y = STATUS_H + 4, ALBUM_GITTER_Y = STATUS_H + 18;
static const int ALBUM_ZELLE_B = 64, ALBUM_ZELLE_H = 39;

void albumOeffnen(ms_t jetzt) {
  if (!parkMoeglich(jetzt)) { zeigeMeldung("Evi ruht gerade."); return; }
  parkFreundPruefen();
  momentStoppen(true);
  menueZeigen(false);
  kuschel = {};
  evi.reaktion = R_KEINE;
  sagen.bis = 0;
  park = ParkZustand();
  park.phase = PARK_ALBUM;
  park.besuch = parkBesucher(jetzt);
  albumWahl = park.besuch;
}

void albumKnopf(Knopf k) {
  const int n = BESUCHER;
  int delta = 0;
  if (k == K_L) delta = -1;
  else if (k == K_R || k == K_A) delta = 1;
  else if (k == K_UP) delta = -ALBUM_SPALTEN;
  else if (k == K_DOWN) delta = ALBUM_SPALTEN;
  albumWahl = (albumWahl + n + delta % n) % n;
}

void albumTouch(int x, int y) {
  if (y >= ZEILE_Y) { if (x >= 240) parkSchliessen(); else albumKnopf(K_A); return; }
  const int sp = x / ALBUM_ZELLE_B, ze = (y - ALBUM_GITTER_Y) / ALBUM_ZELLE_H;
  if (y < ALBUM_GITTER_Y || sp < 0 || sp >= ALBUM_SPALTEN || ze < 0 || ze >= ALBUM_ZEILEN) return;
  albumWahl = ze * ALBUM_SPALTEN + sp;
}

// Verkleinerte Zeichnung (halbe Größe) oder, wenn Evi die Figur noch nicht kennt, nur ihr Schatten
void albumFigur(int i, Phase ph, int x0, int y0, bool bekannt) {
  const BesuchBild& b = BESUCH_BILD[i][ph - BABY];
  for (int ty = 0; ty < BESUCH_HOEHE / 2; ty++) {
    for (int tx = 0; tx < BESUCH_BREITE / 2; tx++) {
      int dunkel = 256;
      for (int q = 0; q < 4; q++) {
        const int sx = tx * 2 + (q & 1) - b.x, sy = ty * 2 + (q >> 1) - b.y;
        if (sx < 0 || sy < 0 || sx >= b.bild.w || sy >= b.bild.h || !b.bild.deckt(sx, sy)) continue;
        dunkel = min(dunkel, bekannt ? (int)b.bild.wert(sx, sy) : 90);
      }
      if (dunkel < 256) bild.drawPixel(x0 + tx, y0 + ty, dunkel);
    }
  }
}

void albumZeichnen(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
  if (ph < BABY || ph > OMA) { parkSchliessen(); return; }
  parkFreundPruefen();
  bild.fillScreen(1);
  zeichneStatus(ph);
  char kopf[40];
  snprintf(kopf, sizeof(kopf), "Parkfreunde %d von %d", parkFreundGetroffen(), BESUCHER);
  schreibe(bild, RAND_LINKS, ALBUM_KOPF_Y, kopf, 0);
  snprintf(kopf, sizeof(kopf), "%s: %s", pet.name[0] ? pet.name : "Evi", WESEN_NAME[eviWesen()]);
  schreibe(bild, BREITE - RAND_LINKS - textBreite(kopf), ALBUM_KOPF_Y, kopf, 0);
  for (int i = 0; i < BESUCHER; i++) {
    const int sp = i % ALBUM_SPALTEN, ze = i / ALBUM_SPALTEN;
    const int x = sp * ALBUM_ZELLE_B, y = ALBUM_GITTER_Y + ze * ALBUM_ZELLE_H;
    const int stufe = parkFreundStufe(parkFreund.treffer[i]);
    albumFigur(i, ph, x + (ALBUM_ZELLE_B - BESUCH_BREITE / 2) / 2, y + 1, stufe > 0);
    for (int d = 0; d < 3; d++) {                       // Punkte unter der Figur: Wie gut Evi sie kennt
      const int dx = x + ALBUM_ZELLE_B / 2 - 9 + d * 7;
      if (d < stufe) bild.fillRect(dx, y + 34, 5, 3, 0);
      else bild.drawRect(dx, y + 34, 5, 3, 170);
    }
    if (i == albumWahl) {
      bild.drawRect(x + 2, y, ALBUM_ZELLE_B - 4, ALBUM_ZELLE_H - 1, 0);
      bild.drawRect(x + 3, y + 1, ALBUM_ZELLE_B - 6, ALBUM_ZELLE_H - 3, 0);
    }
  }
  bild.fillRect(0, ZEILE_Y, BREITE, HOEHE - ZEILE_Y, 1);
  bild.drawFastHLine(0, ZEILE_Y, BREITE, 0);
  const int treffer = parkFreund.treffer[albumWahl], stufe = parkFreundStufe(treffer);
  char zeile1[64], zeile2[64];
  if (treffer) {
    snprintf(zeile1, sizeof(zeile1), "%s: %s", BESUCH_NAME[albumWahl], FIGUR_ART[albumWahl]);
    snprintf(zeile2, sizeof(zeile2), "%dx getroffen · %s", treffer, FIGUR_STUFE_NAME[stufe]);
  } else {
    snprintf(zeile1, sizeof(zeile1), "???");
    snprintf(zeile2, sizeof(zeile2), "Noch nicht getroffen");
  }
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 6, zeile1, 0);
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 22, zeile2, 0);
  schreibe(bild, 242, ZEILE_Y + 22, tastenDa ? "B Zurück" : "Zurück", 0);
  zeigen();
}
