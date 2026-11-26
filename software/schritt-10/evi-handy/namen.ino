// Namen der Tamagotchis und der Park-Figuren (26.11.2026). Siehe namen.h.

const char* tamaName() { return pet.name[0] ? pet.name : "Tamagotchi"; }

void tamaRuht() {
  static char text[48];
  snprintf(text, sizeof(text), "%s ruht gerade.", tamaName());
  zeigeMeldung(text);
}

bool figurWeiblich(int f) { return FIGUR_WEIBLICH_MASKE >> f & 1; }

// Gelten für dieses Tamagotchi die neuen Namen? Erst ab dem ersten benannten Baby (stamm.neueNamenAb).
bool neueNamen(const Tier& t) { return stamm.neueNamenAb != 0 && t.generation >= stamm.neueNamenAb; }

static bool nameGleich(const char* a, const char* b) {
  for (; *a && *b; a++, b++) {
    char x = *a, y = *b;
    if (x >= 'A' && x <= 'Z') x += 32;
    if (y >= 'A' && y <= 'Z') y += 32;
    if (x != y) return false;
  }
  return *a == *b;
}

// Code des Namens einer Figur für ein Tamagotchi: 0–49 Jungenname, 50–99 Mädchenname, 100 + Nummer = alter fester Name.
// Alle zehn Figuren eines Geschlechts bekommen zehn verschiedene Namen aus einem Zehnerfenster der Liste; das Fenster
// wandert mit jedem Tamagotchi weiter, die Reihenfolge innerhalb des Fensters wird aus dem Zeitpunkt „geboren“ gemischt.
uint8_t besuchNameCode(int f, const Tier& t) {
  if (!neueNamen(t)) return (uint8_t)(100 + f);
  const bool w = figurWeiblich(f);
  int k = 0;
  for (int i = 0; i < f; i++) if (figurWeiblich(i) == w) k++;
  char s[40];
  snprintf(s, sizeof(s), "%lld:figurnamen", (long long)t.geboren);
  const int mischen = (int)(streuwert(s) % 10);
  int idx = ((t.generation % 5) * 10 + (k * 7 + mischen) % 10) % 50;
  const char* const* pool = w ? NAMEN_W : NAMEN_M;
  if (nameGleich(pool[idx], t.name)) idx = (idx + 10) % 50;   // keine Figur heißt wie das Tamagotchi
  return (uint8_t)(w ? 50 + idx : idx);
}

const char* nameVonCode(uint8_t code) {
  if (code < 50) return NAMEN_M[code];
  if (code < 100) return NAMEN_W[code - 50];
  if (code < 100 + BESUCHER) return BESUCH_NAME[code - 100];
  return "unbekannt";
}

const char* besuchName(int f) { return nameVonCode(besuchNameCode(f, pet)); }

// ---------- Namenseingabe ----------
static const char* const NAME_TASTEN[30] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
                                            "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "Ä", "Ö", "Ü", "ß"};
static const int NAME_X0 = 0, NAME_Y0 = 92, NAME_ZELLE_B = 32, NAME_ZELLE_H = 24;

int namenZeichen(const char* s) {
  int n = 0;
  for (; *s; s++) if (((uint8_t)*s & 0xC0) != 0x80) n++;
  return n;
}

// Nur der erste Buchstabe ist groß
static void namenKlein(char* z) {
  if (z[0] >= 'A' && z[0] <= 'Z') z[0] += 32;
  else if ((uint8_t)z[0] == 0xC3 && ((uint8_t)z[1] == 0x84 || (uint8_t)z[1] == 0x96 || (uint8_t)z[1] == 0x9C)) z[1] += 0x20;
}

void namenTippen(int taste) {
  char z[4];
  snprintf(z, sizeof(z), "%s", NAME_TASTEN[taste]);
  const size_t b = strlen(nameEdit.text);
  if (namenZeichen(nameEdit.text) >= NAME_MAX_ZEICHEN || b + strlen(z) > (size_t)NAME_MAX_BYTES) { piep(TON(TON_DANEBEN)); return; }
  if (b > 0) namenKlein(z);
  strcat(nameEdit.text, z);
}

void namenLoeschen() {
  size_t b = strlen(nameEdit.text);
  while (b > 0) { b--; if (((uint8_t)nameEdit.text[b] & 0xC0) != 0x80) break; }
  nameEdit.text[b] = 0;
}

void namenVorschlag() {
  char s[40];
  snprintf(s, sizeof(s), "%lld:vorschlag", (long long)pet.geboren);
  const int idx = (int)((streuwert(s) + nameEdit.vorschlag * 7u) % 50);
  snprintf(nameEdit.text, sizeof(nameEdit.text), "%s", (nameEdit.geschlecht ? NAMEN_M : NAMEN_W)[idx]);
  nameEdit.vorschlag++;
}

void namenFertig(ms_t jetzt) {
  if (!nameEdit.text[0]) { piep(TON(TON_DANEBEN)); return; }
  snprintf(pet.name, sizeof(pet.name), "%s", nameEdit.text);
  pet.eigenerName = true;
  stamm.geschlechtAktuell = nameEdit.geschlecht;
  if (stamm.neueNamenAb == 0) stamm.neueNamenAb = (uint8_t)max(1, min((int)pet.generation, 255));
  stammGeaendert = true;
  park = ParkZustand();
  moment.ruheBisTakt = evi.takt + RUHE_TAKTE;
  speichern(jetzt);
  piep(TON(TON_SCHLUEPFEN));
  char text[64];
  snprintf(text, sizeof(text), "%s ist geschlüpft! Willkommen!", pet.name);
  zeigeMeldung(text, 6000);
}

void namenAktivieren(int pos, ms_t jetzt) {
  nameEdit.pos = (uint8_t)pos;
  if (pos < 30) namenTippen(pos);
  else if (pos == 30) namenLoeschen();
  else if (pos == 31) namenVorschlag();
  else namenFertig(jetzt);
}

void namenOeffnen(ms_t jetzt) {
  momentStoppen(true);
  menueZeigen(false);
  kuschel = {};
  evi.reaktion = R_KEINE;
  sagen.bis = 0;
  park = ParkZustand();
  park.phase = PARK_NAME;
  memset(&nameEdit, 0, sizeof(nameEdit));
  char s[40];
  snprintf(s, sizeof(s), "%lld:geschlecht", (long long)pet.geboren);
  nameEdit.geschlecht = streuwert(s) & 1;   // das Geschlecht wird ausgewürfelt (Wunsch der Nutzerin)
  bildschirmEin();
  piep(TON(TON_WUNSCH));
}

// Ein Baby ohne Namen (neues Ei, nach dem Schlüpfen) bekommt von der Nutzerin einen Namen.
void namenPruefen(ms_t jetzt) {
  if (!bildschirmAn || parkOffen() || pet.name[0] || evi.schluepfen) return;
  const Phase ph = phaseBei(pet, jetzt);
  if (ph == EI || ph == TOT) return;
  if (zimmerEditor.offen || sideboard.phase != SB_ZU || spiel.aktiv || ritual.aktiv || gespraech.gebDialog) return;
  namenOeffnen(jetzt);
}

void nameKnopf(Knopf k) {
  const ms_t jetzt = jetztMs();
  int pos = nameEdit.pos;
  if (k == K_A) { namenAktivieren(pos, jetzt); return; }
  if (k == K_B) { namenLoeschen(); return; }
  if (k == K_START) { namenFertig(jetzt); return; }
  if (pos < 30) {
    int sp = pos % 10, ze = pos / 10;
    if (k == K_L) sp = (sp + 9) % 10;
    else if (k == K_R) sp = (sp + 1) % 10;
    else if (k == K_UP) { if (ze == 0) { pos = 30 + min(2, sp / 4); nameEdit.pos = (uint8_t)pos; return; } ze--; }
    else if (k == K_DOWN) { if (ze == 2) { pos = 30 + min(2, sp / 4); nameEdit.pos = (uint8_t)pos; return; } ze++; }
    nameEdit.pos = (uint8_t)(ze * 10 + sp);
  } else {
    int b = pos - 30;
    if (k == K_L) b = (b + 2) % 3;
    else if (k == K_R) b = (b + 1) % 3;
    else if (k == K_UP) { nameEdit.pos = (uint8_t)(20 + b * 4 + 1); return; }
    else if (k == K_DOWN) { nameEdit.pos = (uint8_t)(b * 4 + 1); return; }
    nameEdit.pos = (uint8_t)(30 + b);
  }
}

void nameTouch(int x, int y) {
  const int ze = (y - NAME_Y0) / NAME_ZELLE_H;
  if (y < NAME_Y0 || ze < 0 || ze > 3) return;
  if (ze < 3) namenAktivieren(ze * 10 + min(9, x / NAME_ZELLE_B), jetztMs());
  else namenAktivieren(30 + min(2, x / 107), jetztMs());
}

void nameZeichnen(ms_t jetzt) {
  bild.fillScreen(1);
  zeichneStatus(phaseBei(pet, jetzt));
  char titel[64];
  snprintf(titel, sizeof(titel), "Es ist ein %s! Wie soll es heißen?", nameEdit.geschlecht ? "Junge" : "Mädchen");
  schreibe(bild, (BREITE - textBreite(titel)) / 2, STATUS_H + 5, titel, 0);
  char zeile[40];
  snprintf(zeile, sizeof(zeile), "%s%s", nameEdit.text, namenZeichen(nameEdit.text) < NAME_MAX_ZEICHEN ? "_" : "");
  schreibe(bild, (BREITE - textBreite(zeile, 2)) / 2, STATUS_H + 20, zeile, 0, 2);
  bild.drawFastHLine(60, STATUS_H + 39, BREITE - 120, 0);
  char info[40];
  snprintf(info, sizeof(info), "%d von %d Buchstaben", namenZeichen(nameEdit.text), NAME_MAX_ZEICHEN);
  schreibe(bild, (BREITE - textBreite(info)) / 2, STATUS_H + 44, info, 0);
  for (int p = 0; p < 30; p++) {
    const int x = NAME_X0 + (p % 10) * NAME_ZELLE_B, y = NAME_Y0 + (p / 10) * NAME_ZELLE_H;
    const bool sel = nameEdit.pos == p;
    if (sel) bild.fillRect(x + 1, y, NAME_ZELLE_B - 2, NAME_ZELLE_H - 2, 0);
    else bild.drawRect(x + 1, y, NAME_ZELLE_B - 2, NAME_ZELLE_H - 2, 170);
    schreibe(bild, x + (NAME_ZELLE_B - textBreite(NAME_TASTEN[p], 2)) / 2, y + 4, NAME_TASTEN[p], sel ? 1 : 0, 2);
  }
  const char* knoepfe[3] = {"Löschen", "Vorschlag", "Fertig"};
  for (int b = 0; b < 3; b++) {
    const int x = b * 107, y = NAME_Y0 + 3 * NAME_ZELLE_H;
    const bool sel = nameEdit.pos == 30 + b;
    if (sel) bild.fillRect(x + 2, y, 103, NAME_ZELLE_H - 2, 0);
    else bild.drawRect(x + 2, y, 103, NAME_ZELLE_H - 2, 0);
    schreibe(bild, x + (107 - textBreite(knoepfe[b])) / 2, y + 8, knoepfe[b], sel ? 1 : 0);
  }
  bild.fillRect(0, ZEILE_Y, BREITE, HOEHE - ZEILE_Y, 1);
  bild.drawFastHLine(0, ZEILE_Y, BREITE, 0);
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 6, "Mit den Tasten wählen oder antippen.", 0);
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 22, tastenDa ? "A Tippen · B Löschen · START Fertig" : "Buchstaben antippen, dann „Fertig“.", 0);
  zeigen();
}
