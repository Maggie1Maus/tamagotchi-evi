// Evi spürt Bewegung (Wunsch der Nutzerin vom 24.11.2026, nur auf dem Board, nicht in der Handy-App).
// Der Lagesensor und die Erkennung stehen in lage.h; hier steht, was Evi dann tut:
//  - Kippen: Ein Ball rollt zur tieferen Seite, und Evi rennt hinterher.
//  - Aufs Gesicht legen: Der Bildschirm geht aus (nachts macht das auch das Licht aus).
//    Umgedreht geht er wieder an, und Evi gähnt oder begrüßt dich.
// Dazu „Wie war dein Tag?“: Abends fragt Evi einmal danach und reagiert auf deine Antwort.
// Gassi gehen (Schritte zählen, mittraben) wurde am 25.11.2026 auf Wunsch der Nutzerin ganz entfernt (Schlafmodus).

const int TAG_FRAGEN_AB = 18, TAG_FRAGEN_BIS = 23;   // Uhrzeit
const uint32_t NICKERCHEN_AB_MS = 20000;

// ---------- Kleine Zeichnungen, mit Linien gezeichnet (für Menü und Spiel) ----------
// h = halbe Größe in Punkten; vorne = Linienfarbe, hinten = Füllung (0 schwarz, 1 weiß)
void dickeLinie(int x0, int y0, int x1, int y1, int dicke, uint16_t f) {
  for (int a = 0; a < dicke; a++)
    for (int b = 0; b < dicke; b++) bild.drawLine(x0 + a - dicke / 2, y0 + b - dicke / 2, x1 + a - dicke / 2, y1 + b - dicke / 2, f);
}

void zeichneSchere(int cx, int cy, int h, uint16_t vorne, uint16_t hinten) {
  const int d = h >= 10 ? 3 : 1;
  const int r = h * 3 / 10 + (h >= 10 ? 1 : 0);
  // Klingen kreuzen sich oberhalb der Mitte
  dickeLinie(cx - h * 4 / 10, cy + h * 2 / 10, cx + h * 6 / 10, cy - h * 9 / 10, d, vorne);
  dickeLinie(cx + h * 4 / 10, cy + h * 2 / 10, cx - h * 6 / 10, cy - h * 9 / 10, d, vorne);
  // Griffe: zwei Ringe
  for (int s = -1; s <= 1; s += 2) {
    const int gx = cx + s * h * 5 / 10, gy = cy + h * 6 / 10;
    bild.fillCircle(gx, gy, r, vorne);
    if (r - d > 0) bild.fillCircle(gx, gy, r - d, hinten);
  }
  bild.fillCircle(cx, cy - h * 2 / 10, h >= 10 ? 2 : 0, hinten);
}

void zeichneStein(int cx, int cy, int h, uint16_t vorne, uint16_t hinten) {
  const int d = h >= 10 ? 2 : 1;
  const int k[3][3] = {{-h * 3 / 10, h * 2 / 10, h * 6 / 10}, {h * 3 / 10, h * 3 / 10, h * 5 / 10}, {0, -h * 2 / 10, h * 6 / 10}};
  for (auto& c : k) bild.fillCircle(cx + c[0], cy + c[1], c[2], vorne);
  for (auto& c : k) bild.fillCircle(cx + c[0], cy + c[1], c[2] - d, hinten);
  if (h >= 10) {   // ein paar Flecken
    bild.fillRect(cx - h * 3 / 10, cy - h * 3 / 10, 2, 2, vorne);
    bild.fillRect(cx + h * 2 / 10, cy, 2, 2, vorne);
    bild.fillRect(cx - h / 10, cy + h * 4 / 10, 2, 2, vorne);
  }
}

void zeichnePapier(int cx, int cy, int h, uint16_t vorne, uint16_t hinten) {
  const int d = h >= 10 ? 2 : 1;
  const int x0 = cx - h * 6 / 10, x1 = cx + h * 6 / 10, y0 = cy - h * 9 / 10, y1 = cy + h * 9 / 10, ecke = h * 4 / 10;
  bild.fillRect(x0, y0, x1 - x0 + 1, y1 - y0 + 1, vorne);
  bild.fillRect(x0 + d, y0 + d, x1 - x0 + 1 - 2 * d, y1 - y0 + 1 - 2 * d, hinten);
  // Eselsohr oben rechts
  bild.fillTriangle(x1 - ecke, y0, x1, y0, x1, y0 + ecke, hinten);
  dickeLinie(x1 - ecke, y0, x1, y0 + ecke, d, vorne);
  dickeLinie(x1 - ecke, y0, x1 - ecke, y0 + ecke, d, vorne);
  dickeLinie(x1 - ecke, y0 + ecke, x1, y0 + ecke, d, vorne);
  // Schriftzeilen
  for (int i = 0; i < 3; i++) {
    const int y = cy - h * 2 / 10 + i * h * 4 / 10;
    bild.drawFastHLine(x0 + d + h / 5 + 1, y, x1 - x0 - 2 * d - 2 * (h / 5) - 1, vorne);
    if (h >= 10) bild.drawFastHLine(x0 + d + h / 5 + 1, y + 1, x1 - x0 - 2 * d - 2 * (h / 5) - 1, vorne);
  }
}

void zeichneWolke(int cx, int cy, int h, uint16_t vorne, uint16_t hinten, bool regen) {
  const int oy = regen ? -h * 3 / 10 : 0;
  const int k[3][3] = {{-h * 4 / 10, oy + h * 2 / 10, h * 4 / 10}, {h * 4 / 10, oy + h * 2 / 10, h * 4 / 10}, {0, oy - h / 10, h * 5 / 10}};
  for (auto& c : k) bild.fillCircle(cx + c[0], cy + c[1], c[2], vorne);
  bild.fillRect(cx - h * 4 / 10, cy + oy + h * 2 / 10, h * 8 / 10, h * 4 / 10 + 1, vorne);
  for (auto& c : k) bild.fillCircle(cx + c[0], cy + c[1], c[2] - 1, hinten);
  bild.fillRect(cx - h * 4 / 10, cy + oy + h * 2 / 10, h * 8 / 10, h * 4 / 10, hinten);
  if (regen) for (int i = -1; i <= 1; i++) bild.drawLine(cx + i * h * 4 / 10, cy + h * 5 / 10, cx + i * h * 4 / 10 - 1, cy + h * 9 / 10, vorne);
}

// Zeichen 0 Schere, 1 Stein, 2 Papier, 3 Wolke, 4 Regenwolke
void zeichneZeichen(int zeichen, int cx, int cy, int h, uint16_t vorne, uint16_t hinten) {
  switch (zeichen) {
    case 0: zeichneSchere(cx, cy, h, vorne, hinten); break;
    case 1: zeichneStein(cx, cy, h, vorne, hinten); break;
    case 2: zeichnePapier(cx, cy, h, vorne, hinten); break;
    case 3: zeichneWolke(cx, cy, h, vorne, hinten, false); break;
    case 4: zeichneWolke(cx, cy, h, vorne, hinten, true); break;
  }
}

// ---------- Was Evi tut ----------
bool eviKannReagieren(ms_t jetzt) {
  if (sideboard.phase != SB_ZU) return false;
  return bildschirmAn && !spiel.aktiv && !ritual.aktiv && !menueOffen && !evi.schluepfen &&
         momenteFuer(phaseBei(pet, jetzt)) && !ruhend(beschreiben(pet, jetzt));
}

bool besuchDa() { return moment.art == M_FALTER || moment.art == M_VOGEL || moment.art == M_STERN; }

void gekippt(ms_t jetzt, int seite) {
  if (!bildschirmAn || spiel.aktiv || ritual.aktiv || menueOffen) return;
  const Phase ph = phaseBei(pet, jetzt);
  if (ph == EI) { evi.wackelnBis = evi.takt + 12; return; }
  if (!eviKannReagieren(jetzt) || besuchDa() || moment.art == M_ROLLEN) return;
  momentStoppen(true);
  rollBall.x = BODEN_X + seite * 24;
  rollBall.v = seite * 2.0f;
  rollBall.gerade = 0;
  momentStarten(M_ROLLEN, GD_KEINER);
}

void aufsGesicht(ms_t jetzt) {
  if (!bildschirmAn) return;   // schon aus (zum Beispiel mit RESET)
  // Nachts macht das Hinlegen das Licht aus, wie im Menü
  if (schlafArt(pet, jetzt) == NACHTS && !pet.lichtAus) {
    const Ergebnis r = pflegen(pet, LICHT, jetzt);
    if (r.geaendert) speichern(jetzt);
  }
  bildschirmAus();
  bewegung.ausDurchLage = true;
}

void wiederUmgedreht(ms_t jetzt, uint32_t dauer) {
  if (!bewegung.ausDurchLage) return;
  bewegung.ausDurchLage = false;
  if (bildschirmAn) return;   // inzwischen von selbst angegangen
  bildschirmEin();
  if (dunkelSeit && jetzt - dunkelSeit >= GRUSS_NACH) { aufgeweckt(); return; }
  if (dauer >= NICKERCHEN_AB_MS && eviKannReagieren(jetzt) && !geweckt(pet, jetzt)) {
    momentStoppen(true);
    momentStarten(M_NICKERCHEN, GD_KEINER);
  }
}

// Jede Runde der Hauptschleife: Sensor lesen und reagieren
void bewegungTakt() {
  const uint8_t e = lageLesen();
  const ms_t jetzt = jetztMs();
  if (e & LE_KIPPEN) gekippt(jetzt, lage.kippSeite);
  if (e & LE_GESICHT_UNTEN) aufsGesicht(jetzt);
  if (e & LE_GESICHT_OBEN) wiederUmgedreht(jetzt, lage.untenDauer);
}

// ---------- „Wie war dein Tag?“ ----------
bool tagFrageZeit(ms_t jetzt) {
  const int h = stundeVon(jetzt);
  return h >= TAG_FRAGEN_AB && h < TAG_FRAGEN_BIS;
}

// Steht „Wie war dein Tag?“ im Menü? Abends, solange noch keine Antwort kam.
bool tagImMenue(ms_t jetzt) {
  return tagFrageZeit(jetzt) && bewegung.tagBeantwortet != tagKennung(jetzt) && momenteFuer(phaseBei(pet, jetzt)) &&
         !ruhend(beschreiben(pet, jetzt));
}

// Evi fragt von selbst, einmal am Abend (statt eines kleinen Moments)
bool tagVonSelbstFragen(ms_t jetzt) {
  if (!tagImMenue(jetzt) || bewegung.tagGefragt == tagKennung(jetzt) || !bildschirmAn) return false;
  tagFragen();
  return true;
}

void tagFragen() {
  const ms_t jetzt = jetztMs();
  bewegung.tagGefragt = tagKennung(jetzt);
  momentStoppen(true);
  sage("Wie war dein Tag?", &SYM_W_HERZ, 60000);
  menueZeigen(true, false, true);
  piep(TON(TON_WUNSCH));
}

// Antwort: 0 gut, 1 so lala, 2 nicht so gut; -1 = Menü ohne Antwort geschlossen
void tagAntwort(int antwort) {
  const ms_t jetzt = jetztMs();
  sagen.bis = 0;
  moment.ruheBisTakt = evi.takt + RUHE_TAKTE;
  if (antwort < 0) { sage("Erzählst du es mir später?", &SYM_W_HERZ, 3000); return; }
  bewegung.tagBeantwortet = tagKennung(jetzt);
  if (antwort == 0) {
    reagiere(R_JUBEL, 20);
    sage("Juhu! Das freut mich!", &SYM_W_HERZ, 4000);
  } else if (antwort == 1) {
    reagiere(R_GRUSS, 12);
    sage("Morgen wird es bestimmt schöner!", &SYM_W_HERZ, 4000);
  } else {
    momentStoppen(true);
    momentStarten(M_TROESTEN, GD_KEINER);
  }
}

// ---------- Zeichnen ----------
// Der rollende Ball liegt vor Evi auf dem Boden (wie der Ball beim Ballfangen, 14 × 14)
void bewegungZeichnenVorne() {
  if (!rollBall.sichtbar) return;
  const int bx = (int)lroundf(rollBall.x) - 7, by = RAUM_HEIM_Y - 14;
  for (int y = 0; y < 14; y++) {
    bild.drawFastHLine(bx + BALL_ZEILEN[y][0], by + y, BALL_ZEILEN[y][1] - BALL_ZEILEN[y][0] + 1, 0);
    if (y >= 3 && y <= 5) bild.drawFastHLine(bx + 4 + ((int)rollBall.x / 4) % 4, by + y, 3, 1);   // Glanzloch wandert: er rollt
  }
}
