// Namen: Namenseingabe beim Schlüpfen, wechselnde Namen der Park-Figuren, Stammbaum mit Geschlecht (26.11.2026)
#include "Arduino.h"
uint32_t simMillis = 1000;
SerialC Serial;
#include "Wire.h"
TwoWire Wire1;
#include "SPI.h"
SPIClass SPI1;
void (*simNachBild)() = nullptr;
unsigned long simGesendet = 0;
int simDC = 1;
uint8_t simDisplay[240][320];
void nachbildungBildFertig() { simMillis += 5; }
#include "EEPROM.h"
EEPROMC EEPROM;
#include "evi-handy.ino"
int fehler = 0;
#define PRUEFE(b, t) do { if (!(b)) { printf("FEHLER: %s\n", t); fehler++; } else printf("ok: %s\n", t); } while (0)
const ms_t HEUTE = ausOrtszeit(2026, 11, 26, 12, 0);
void laufen(uint32_t ms) { for (uint32_t t = 0; t < ms; t += 10) { simMillis += 10; loop(); } }
// Buchstabe antippen: Mitte der Zelle
void tipp(int pos) { beruehrt((pos % 10) * 32 + 16, 92 + (pos / 10) * 24 + 10); }
void taste(char c) { tipp(c - 'A'); }
void knopfAn(int btn) { beruehrt(btn * 107 + 50, 92 + 3 * 24 + 10); }   // 0 Löschen, 1 Vorschlag, 2 Fertig
int zeichen(const char* s) { int n = 0; for (; *s; s++) if ((*s & 0xC0) != 0x80) n++; return n; }
bool gleich(const char* a, const char* b) { return strcasecmp(a, b) == 0; }

int main() {
  bildschirmStarten(); bild.cp437(true);
  uhrSetzen(HEUTE);
  pet = neuesTier(HEUTE - 3 * TAG, ERSTES_EI); pet.generation = 1; pet.satt = pet.laune = pet.sauber = 90; pet.aktualisiert = HEUTE;
  himmel = himmelBei(HEUTE); moment.naechster = HEUTE + 10 * MINUTE;
  freundLaden(); parkFreundLaden(); stammLaden();
  tastenStarten();
  bildschirmAn = true; bildschirmEin();

  // ---- Namenslisten ----
  {
    bool gut = true;
    for (int i = 0; i < 50; i++) {
      if (zeichen(NAMEN_M[i]) > NAME_MAX_ZEICHEN || zeichen(NAMEN_W[i]) > NAME_MAX_ZEICHEN) { gut = false; printf("  zu lang: %s / %s\n", NAMEN_M[i], NAMEN_W[i]); }
      for (int j = 0; j < 50; j++) {
        if (j != i && (gleich(NAMEN_M[i], NAMEN_M[j]) || gleich(NAMEN_W[i], NAMEN_W[j]))) { gut = false; printf("  doppelt: %s / %s\n", NAMEN_M[i], NAMEN_W[i]); }
        if (gleich(NAMEN_M[i], NAMEN_W[j])) { gut = false; printf("  in beiden Listen: %s\n", NAMEN_M[i]); }
      }
      for (int f = 0; f < BESUCHER; f++) if (gleich(NAMEN_M[i], BESUCH_NAME[f]) || gleich(NAMEN_W[i], BESUCH_NAME[f])) { gut = false; printf("  wie alter Name: %s\n", BESUCH_NAME[f]); }
    }
    PRUEFE(gut, "50 Jungen- und 50 Mädchennamen, höchstens 11 Buchstaben, keine doppelt");
    PRUEFE(!strcmp(NAMEN_M[0], "Alf") && !strcmp(NAMEN_W[0], "Paula"), "Alf und Paula stehen in den Listen");
    int w = 0, m = 0;
    for (int i = 0; i < BESUCHER; i++) { if (figurWeiblich(i)) w++; else m++; }
    PRUEFE(w == 10 && m == 10 && !strcmp(BESUCH_NAME[0], "Lotti") && figurWeiblich(0) && !figurWeiblich(13), "10 weibliche und 10 männliche Figuren (Lotti weiblich, Mampf männlich)");
  }
  // ---- Das jetzige Tamagotchi behält die alten Namen ----
  {
    bool alle = true;
    for (int i = 0; i < BESUCHER; i++) if (strcmp(besuchName(i), BESUCH_NAME[i])) alle = false;
    PRUEFE(alle && stamm.neueNamenAb == 0, "vor dem ersten benannten Baby: alte Namen (Lotti, Mampf …)");
  }
  // ---- Evi stirbt, neues Ei: kein Name, Elternteil aus den Parkfreunden ----
  parkFreund.treffer[13] = 9; parkFreund.treffer[4] = 12; parkFreund.evi = pet.geboren;      // Mampf (m) und Ella (w)
  pet.tot = true; pet.gestorben = pet.schluepft + 9 * TAG; pet.aktualisiert = pet.gestorben; pet.ursache = ALTER;
  const ms_t jetzt1 = pet.gestorben;
  uhrSetzen(jetzt1);
  menueBauen();
  aktionAusfuehren(E_NEUES_EI);
  PRUEFE(!pet.name[0] && pet.generation == 2 && !pet.eigenerName, "neues Ei hat noch keinen Namen");
  PRUEFE(stamm.anzahl == 1 && !strcmp(stamm.eintrag[0].name, "Evi") && stamm.eintrag[0].geschlecht == 0, "Evi kommt in den Stammbaum (Mädchen)");
  PRUEFE(stamm.elternAktuell == 13 && stamm.elternNameAktuell == 100 + 13, "Evi war ein Mädchen: Elternteil ist der männliche Freund Mampf (alter Name)");
  // Ei schlüpft
  PRUEFE(!parkOffen(), "beim Ei noch keine Namenseingabe");
  uhrSetzen(jetzt1 + 40 * MINUTE);
  laufen(8000);
  PRUEFE(parkOffen() && park.phase == PARK_NAME, "nach dem Schlüpfen fragt die Namenseingabe");
  zeichneAlles();
  PRUEFE(parkOffen(), "Namenseingabe zeichnet");
  // leerer Name geht nicht
  knopfAn(2);
  PRUEFE(park.phase == PARK_NAME && !pet.name[0], "ohne Buchstaben kein Fertig");
  // tippen per Touch: Groß-/Kleinschreibung
  taste('M'); taste('I'); taste('A');
  PRUEFE(!strcmp(nameEdit.text, "Mia"), "Tippen: erster Buchstabe groß, Rest klein");
  knopf(K_B);
  PRUEFE(!strcmp(nameEdit.text, "Mi"), "B löscht den letzten Buchstaben");
  knopfAn(0);
  knopfAn(0); knopfAn(0);
  PRUEFE(nameEdit.text[0] == 0, "Löschen-Taste leert den Namen");
  // Höchstens 11 Buchstaben
  for (int i = 0; i < 15; i++) taste('B');
  PRUEFE(zeichen(nameEdit.text) == 11 && nameEdit.text[0] == 'B' && nameEdit.text[1] == 'b', "höchstens 11 Buchstaben");
  for (int i = 0; i < 12; i++) knopf(K_B);
  // Umlaute: Bytegrenze
  for (int i = 0; i < 15; i++) tipp(26);       // Ä
  PRUEFE(!strcmp(nameEdit.text, "Äääääääää") && strlen(nameEdit.text) <= 19, "Umlaute: höchstens 19 Bytes");
  for (int i = 0; i < 12; i++) knopf(K_B);
  // Tasten: Steuerkreuz
  nameEdit.pos = 0;
  knopf(K_R); knopf(K_R); knopf(K_DOWN);
  PRUEFE(nameEdit.pos == 12, "Steuerkreuz bewegt die Auswahl (zu M)");
  knopf(K_A);
  PRUEFE(!strcmp(nameEdit.text, "M"), "A tippt den gewählten Buchstaben");
  knopf(K_B);
  nameEdit.pos = 25; knopf(K_DOWN);
  PRUEFE(nameEdit.pos >= 30, "Runter von der letzten Zeile: zu den Knöpfen");
  knopf(K_UP);
  PRUEFE(nameEdit.pos < 30, "Hoch von den Knöpfen: zurück zu den Buchstaben");
  // Vorschlag und Geschlecht
  nameEdit.geschlecht = 1; knopfAn(1);
  bool inM = false; for (int i = 0; i < 50; i++) if (!strcmp(nameEdit.text, NAMEN_M[i])) inM = true;
  char ersterVorschlag[24]; strcpy(ersterVorschlag, nameEdit.text);
  knopfAn(1);
  PRUEFE(inM && strcmp(ersterVorschlag, nameEdit.text), "Vorschlag: Jungenname, jeder neue anders");
  nameEdit.geschlecht = 0; knopfAn(1);
  bool inW = false; for (int i = 0; i < 50; i++) if (!strcmp(nameEdit.text, NAMEN_W[i])) inW = true;
  PRUEFE(inW, "Mädchen: Vorschläge sind Mädchennamen");
  { int j = 0, m = 0; for (int i = 0; i < 40; i++) { pet.geboren += 1234; char s2[40]; snprintf(s2, sizeof(s2), "%lld:geschlecht", (long long)pet.geboren); (streuwert(s2) & 1 ? m : j)++; }
    PRUEFE(j > 5 && m > 5, "Geschlecht wird ausgewürfelt (beide kommen vor)"); pet.geboren -= 40 * 1234; }
  // Namen "Hugo" (liegt im Fenster der zweiten Generation) und Junge
  for (int i = 0; i < 12; i++) knopf(K_B);
  taste('H'); taste('U'); taste('G'); taste('O');
  nameEdit.geschlecht = 1;
  knopfAn(2);
  PRUEFE(!parkOffen() && !strcmp(pet.name, "Hugo") && pet.eigenerName, "Fertig: Name gesetzt, Eingabe geschlossen");
  PRUEFE(stamm.geschlechtAktuell == 1 && stamm.neueNamenAb == 2, "Junge gespeichert, neue Namen ab Generation 2");
  PRUEFE(strstr(meldung, "Hugo ist geschlüpft"), "Willkommensmeldung mit Namen");
  // Nach Neustart
  speichern(jetztMs());
  { Tier t; bool ton; spielstandLaden(t, ton); PRUEFE(!strcmp(t.name, "Hugo"), "Name übersteht den Neustart"); }
  // Neue Figurennamen
  {
    bool verschieden = true, kein = true, inPool = true;
    for (int i = 0; i < BESUCHER; i++) {
      const char* n = besuchName(i);
      if (gleich(n, pet.name)) kein = false;
      for (int j = i + 1; j < BESUCHER; j++) if (gleich(n, besuchName(j))) verschieden = false;
      bool da = false; for (int k = 0; k < 50; k++) if (!strcmp(n, figurWeiblich(i) ? NAMEN_W[k] : NAMEN_M[k])) da = true;
      if (!da) inPool = false;
    }
    PRUEFE(inPool && !strcmp(besuchName(13), besuchName(13)) && strcmp(besuchName(13), "Mampf"), "Figuren haben neue Namen aus der richtigen Liste (Jungen/Mädchen)");
    PRUEFE(verschieden, "alle 20 Figuren heißen verschieden");
    PRUEFE(kein, "keine Figur heißt wie das Tamagotchi (Hugo)");
    // Hugo wäre sonst vergeben worden
    Tier t = pet; strcpy(t.name, "Zzz");
    bool hugoSonstDa = false; for (int i = 0; i < BESUCHER; i++) if (!figurWeiblich(i) && gleich(nameVonCode(besuchNameCode(i, t)), "Hugo")) hugoSonstDa = true;
    PRUEFE(hugoSonstDa, "Test ist aussagekräftig: Hugo wäre sonst bei einer Figur gelandet");
    // Namen bleiben gleich, solange das Tamagotchi lebt
    char vorher[BESUCHER][12];
    for (int i = 0; i < BESUCHER; i++) strcpy(vorher[i], besuchName(i));
    uhrSetzen(HEUTE + 30 * TAG);
    bool gleichBlieb = true; for (int i = 0; i < BESUCHER; i++) if (strcmp(vorher[i], besuchName(i))) gleichBlieb = false;
    PRUEFE(gleichBlieb, "Namen bleiben gleich, solange dasselbe Tamagotchi lebt (jede Lebensphase)");
  }
  // Rotation über Generationen: aufeinanderfolgende Leben ohne gemeinsame Namen, nach fünf Leben alle Namen durch
  {
    bool getrennt = true; int sichtM[50] = {}, sichtW[50] = {};
    char vorigeNamen[BESUCHER][12] = {};
    for (int g = 2; g <= 6; g++) {
      Tier t = pet; t.generation = g; t.geboren = HEUTE + g * 977; strcpy(t.name, "Zzz");
      for (int i = 0; i < BESUCHER; i++) {
        const uint8_t code = besuchNameCode(i, t);
        if (code < 50) sichtM[code]++; else sichtW[code - 50]++;
        const char* n = nameVonCode(code);
        if (g > 2) for (int j = 0; j < BESUCHER; j++) if (figurWeiblich(i) == figurWeiblich(j) && !strcmp(n, vorigeNamen[j])) getrennt = false;
      }
      for (int i = 0; i < BESUCHER; i++) strcpy(vorigeNamen[i], nameVonCode(besuchNameCode(i, t)));
    }
    bool alle = true; for (int i = 0; i < 50; i++) if (sichtM[i] != 1 || sichtW[i] != 1) alle = false;
    PRUEFE(getrennt, "nächstes Tamagotchi: alle Figuren heißen anders als zuvor");
    PRUEFE(alle, "nach 5 Leben sind alle 50 + 50 Namen genau einmal dran");
  }
  // ---- Stammbaum mit Junge: Elternteil ist eine weibliche Figur, Seitenwahl ----
  {
    pet.generation = 2;
    const char* vorherName = besuchName(4);   // Ella-Position (weiblich) mit neuem Namen
    parkFreund.evi = pet.geboren; memset(parkFreund.treffer, 0, sizeof(parkFreund.treffer));
    parkFreund.treffer[13] = 40; parkFreund.treffer[8] = 15; parkFreund.treffer[14] = 4;    // Mampf (m), Kiwi (w), Mia (w)
    char kiwiName[12]; strcpy(kiwiName, besuchName(8));
    (void)vorherName;
    pet.tot = true; pet.gestorben = pet.schluepft + 9 * TAG; pet.aktualisiert = pet.gestorben; pet.ursache = ALTER;
    uhrSetzen(pet.gestorben);
    aktionAusfuehren(E_NEUES_EI);
    const StammEintrag& e = stamm.eintrag[stamm.anzahl - 1];
    PRUEFE(!strcmp(e.name, "Hugo") && e.geschlecht == 1, "Hugo (Junge) im Stammbaum");
    PRUEFE(stamm.elternAktuell == 8 && figurWeiblich(8), "Hugo war ein Junge: Elternteil ist die weibliche Figur mit den meisten Treffen (Kiwi-Figur)");
    PRUEFE(!strcmp(nameVonCode(stamm.elternNameAktuell), kiwiName), "ihr Name von damals ist gespeichert");
    // Das neue Tamagotchi hat andere Figurennamen, der gespeicherte Name bleibt
    pet.name[0] = 0; uhrSetzen(pet.gestorben + 40 * MINUTE);
    pet.schluepft = pet.aktualisiert; 
    strcpy(pet.name, "Paula"); pet.eigenerName = true; stamm.geschlechtAktuell = 0; stamm.neueNamenAb = 2;
    PRUEFE(strcmp(besuchName(8), kiwiName) != 0, "neues Tamagotchi: die Figur heißt jetzt anders");
    PRUEFE(!strcmp(nameVonCode(stamm.elternNameAktuell), kiwiName), "Stammbaum zeigt weiter den alten Namen");
    // Zeichnen: neues Tamagotchi (Paula) mit Vater-Seite/Mutter-Seite
    pet.tot = false; pet.schluepft = HEUTE - 2 * TAG; pet.aktualisiert = pet.schluepft + 3 * STUNDE;
    uhrSetzen(pet.aktualisiert);
    stammOeffnen(jetztMs());
    PRUEFE(park.phase == PARK_STAMM, "Stammbaum offen");
    zeichneAlles();
    { FILE* f = fopen("stamm2.pgm", "wb"); fprintf(f, "P5\n320 240\n255\n"); for (int y = 0; y < 240; y++) for (int x = 0; x < 320; x++) fputc(bild.stufe(x, y) * (bild.stufe(x, y) <= 15 ? 17 : 1), f); fclose(f); }
    for (int i = 0; i < 5; i++) { knopf(K_L); zeichneAlles(); }
    parkSchliessen();
  }
  // ---- Texte ----
  {
    pet.name[0] = 0; meldung[0] = 0; tamaRuht();
    PRUEFE(!strcmp(meldung, "Tamagotchi ruht gerade."), "ohne Namen heißt es „Tamagotchi“ (nicht Evi)");
  }
  // ---- Namenseingabe wartet, wenn gerade etwas anderes läuft; Bildschirm aus schließt sie, sie kommt wieder ----
  {
    strcpy(pet.name, ""); pet.tot = false; pet.schluepft = HEUTE - 3 * TAG; pet.aktualisiert = HEUTE; uhrSetzen(HEUTE);
    himmel = himmelBei(HEUTE); bildschirmAn = true;
    namenOeffnen(HEUTE);
    PRUEFE(park.phase == PARK_NAME, "Namenseingabe offen");
    bildschirmAus();
    PRUEFE(!parkOffen() && !bildschirmAn, "Bildschirm aus schließt die Eingabe");
    bildschirmEin();
    laufen(2000);
    PRUEFE(park.phase == PARK_NAME, "Bildschirm an: die Namenseingabe kommt wieder, bis ein Name da ist");
  }
  printf(fehler ? "%d FEHLER\n" : "Alles in Ordnung.\n", fehler);
  return fehler ? 1 : 0;
}
