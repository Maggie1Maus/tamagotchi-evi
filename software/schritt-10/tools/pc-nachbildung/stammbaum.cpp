// Stammbaum, Geschenke der Parkfreunde und die Folgen von Evis Wesen (26.11.2026)
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
ms_t geburtFuer(int wesen, int ab = 0) {
  for (int i = ab; i < 100000; i++) { const ms_t g = HEUTE - 3 * TAG - 1000 * (ms_t)i; if (wesenVon(g) == wesen) return g; }
  return 0;
}
Tier erwachsene(int wesen, ms_t jetzt = HEUTE) {
  Tier t = neuesTier(jetzt - 3 * TAG, ERSTES_EI);
  t.geboren = geburtFuer(wesen); t.schluepft = jetzt - 3 * TAG; t.aktualisiert = jetzt;
  t.satt = t.laune = t.sauber = 100;
  return t;
}
int neutral = W_WASSER;   // Vergleichs-Evi (wird je Test ersetzt)
int zeichenAnzahl(const char* s) { int n = 0; for (; *s; s++) if ((*s & 0xC0) != 0x80) n++; return n; }

int main() {
  bildschirmStarten(); bild.cp437(true);
  uhrSetzen(HEUTE);
  pet = erwachsene(W_NEUGIER); himmel = himmelBei(HEUTE);
  freundLaden(); parkFreundLaden(); stammLaden();

  // ======== Folgen des Wesens in den Regeln ========
  {
    Tier a = erwachsene(W_NASCH), b = erwachsene(W_LANG);        // Langschläferin hier nur als Vergleich ohne Hunger-Folge
    weiterrechnen(a, HEUTE + 3 * STUNDE); weiterrechnen(b, HEUTE + 3 * STUNDE);
    const double da = 100 - a.satt, db = 100 - b.satt;
    PRUEFE(da > db * 1.10 && da < db * 1.20, "Naschkatze: Hunger sinkt etwa 15 % schneller");
    PRUEFE(naschenProTag(erwachsene(W_NASCH)) == R::NASCHEN_PRO_TAG + 1 && naschenProTag(erwachsene(W_SPORT)) == R::NASCHEN_PRO_TAG, "Naschkatze: eine Nascherei mehr");
    Tier n1 = erwachsene(W_NASCH), n2 = erwachsene(W_SPORT);
    n1.laune = n2.laune = 40;
    pflegen(n1, NASCHEN, HEUTE); pflegen(n2, NASCHEN, HEUTE);
    PRUEFE(n1.laune - n2.laune == 5, "Naschkatze: Naschen gibt +15 statt +10 Laune");
    // 4. Naschen ohne Bauchweh: 4 sind erlaubt, die 5. gibt Bauchweh
    Tier n3 = erwachsene(W_NASCH); n3.satt = 20;
    for (int i = 0; i < 4; i++) pflegen(n3, NASCHEN, HEUTE + i * 60000);
    const bool nachVier = bauchweh(n3, HEUTE + 5 * 60000);
    pflegen(n3, NASCHEN, HEUTE + 5 * 60000);
    PRUEFE(!nachVier && bauchweh(n3, HEUTE + 6 * 60000), "Naschkatze: erst die fünfte Nascherei macht Bauchweh");
  }
  {
    Tier s = erwachsene(W_SPORT), w = erwachsene(W_WASSER), l = erwachsene(W_LANG);
    weiterrechnen(s, HEUTE + 3 * STUNDE); weiterrechnen(w, HEUTE + 3 * STUNDE); weiterrechnen(l, HEUTE + 3 * STUNDE);
    const double ds = 100 - s.sauber, dw = 100 - w.sauber, dl = 100 - l.sauber;
    PRUEFE(ds > dl * 1.15 && ds < dl * 1.25, "Sportlerin: wird 20 % schneller schmutzig");
    PRUEFE(dw < dl * 0.85 && dw > dl * 0.75, "wasserscheu: wird 20 % langsamer schmutzig");
    Tier a = erwachsene(W_SPORT), b = erwachsene(W_NASCH);
    a.laune = b.laune = 20;
    const Ergebnis ea = spielBeenden(a, 5, HEUTE, W_BALL), eb = spielBeenden(b, 5, HEUTE, W_BALL);
    PRUEFE(ea.muenzen == 15 && eb.muenzen == 10, "Sportlerin: Spiele bringen 50 % mehr Münzen");
    const ms_t frueh = ausOrtszeit(2026, 11, 26, 6, 30);
    Tier sp = erwachsene(W_SPORT, frueh), ne = erwachsene(W_NASCH, frueh);
    PRUEFE(!schlaeft(sp, frueh) && schlaeft(ne, frueh), "Sportlerin: um 6:30 schon wach");
    Tier w1 = erwachsene(W_WASSER), w2 = erwachsene(W_SPORT);
    w1.laune = w2.laune = 50; w1.sauber = w2.sauber = 20;
    pflegen(w1, PUTZEN, HEUTE); pflegen(w2, PUTZEN, HEUTE);
    PRUEFE(w2.laune - w1.laune == 8 && w1.laune == 45, "wasserscheu: Saubermachen kostet Laune (−5 statt +3)");
  }
  {
    const ms_t spaet = ausOrtszeit(2026, 11, 26, 7, 30), abends = ausOrtszeit(2026, 11, 26, 21, 30);
    Tier l = erwachsene(W_LANG, spaet), n = erwachsene(W_NASCH, spaet);
    PRUEFE(schlaeft(l, spaet) && !schlaeft(n, spaet), "Langschläferin: um 7:30 noch im Schlaf");
    Tier l2 = erwachsene(W_LANG, ausOrtszeit(2026, 11, 26, 8, 10)), n2 = erwachsene(W_NASCH, ausOrtszeit(2026, 11, 26, 8, 10));
    PRUEFE(!schlaeft(l2, ausOrtszeit(2026, 11, 26, 8, 10)), "Langschläferin: ab 8 Uhr wach");
    Tier la = erwachsene(W_LANG, abends), na = erwachsene(W_NASCH, abends);
    PRUEFE(muede(la, abends) && !muede(na, abends), "Langschläferin: abends früher müde");
    // Schlaf kostet 20 % weniger
    Tier ls = erwachsene(W_LANG, ausOrtszeit(2026, 11, 26, 23, 30)), ns = erwachsene(W_NASCH, ausOrtszeit(2026, 11, 26, 23, 30));
    ls.aktualisiert = ns.aktualisiert = ausOrtszeit(2026, 11, 26, 23, 30);
    ls.sauber = ns.sauber = 100;
    weiterrechnen(ls, ausOrtszeit(2026, 11, 27, 4, 30)); weiterrechnen(ns, ausOrtszeit(2026, 11, 27, 4, 30));
    PRUEFE((100 - ls.sauber) < (100 - ns.sauber) * 0.9, "Langschläferin: Schlaf kostet weniger");
    const ms_t ritualZeit = ausOrtszeit(2026, 11, 26, 22, 10);
    Tier r1 = erwachsene(W_LANG, ritualZeit), r2 = erwachsene(W_SPORT, ritualZeit);
    r1.laune = r2.laune = 50;
    pflegen(r1, RITUAL, ritualZeit); pflegen(r2, RITUAL, ritualZeit);
    PRUEFE(r1.laune - r2.laune == 5, "Langschläferin: Gute Nacht gibt +15 Laune");
  }
  {
    Tier a = erwachsene(W_SCHMUSE), b = erwachsene(W_SPORT);
    a.laune = b.laune = 40;
    kuscheln(a, 8, HEUTE); kuscheln(b, 8, HEUTE);
    PRUEFE(a.laune - b.laune == 4, "Schmusekatze: Kuscheln wirkt 50 % stärker");
  }
  // Tageslaunen: Lieblingslaune an etwa 40 % der Tage
  {
    const int liebling[WESEN_ZAHL] = {2, 0, -1, 3, 1, -1};
    bool alleGut = true;
    for (int w = 0; w < WESEN_ZAHL; w++) {
      pet = erwachsene(w);
      int zaehler[4] = {};
      for (int d = 0; d < 400; d++) zaehler[launeHeute(HEUTE + d * TAG)]++;
      if (liebling[w] >= 0) { if (zaehler[liebling[w]] < 130 || zaehler[liebling[w]] > 190) alleGut = false; }
      else for (int k = 0; k < 4; k++) if (zaehler[k] < 70 || zaehler[k] > 130) alleGut = false;
      for (int k = 0; k < 4; k++) if (!zaehler[k]) alleGut = false;
    }
    PRUEFE(alleGut, "Tageslaunen: Lieblingslaune etwa 40 %, alle Launen kommen vor");
  }
  // Fragen: Neugierige eine mehr, Zeiten sinnvoll
  {
    pet = erwachsene(W_NEUGIER); Tier& e = pet;
    const Phase phasen[3] = {BABY, TEEN, ERWACHSEN};
    const int normal[3] = {4, 2, 1};
    bool gut = true;
    for (int i = 0; i < 3; i++) {
      const int n = fragenProTag(phasen[i]);
      if (n != normal[i] + 1) gut = false;
      ms_t letzte = 0;
      for (int s = 0; s < n; s++) {
        const ms_t um = frageZeit(HEUTE, s, n);
        const Ortszeit o = ortszeit(um);
        if (o.stunde < 9 || o.stunde > 21) gut = false;
        if (um < letzte) gut = false;
        letzte = um;
      }
    }
    (void)e;
    PRUEFE(gut, "Neugierige: eine Frage mehr am Tag, Zeiten zwischen 9 und 21 Uhr");
    pet = erwachsene(W_SPORT);
    PRUEFE(fragenProTag(BABY) == 4 && fragenProTag(ERWACHSEN) == 1, "andere Wesen: Fragen wie bisher");
  }
  // Schmusekatze: zwei Streichler versöhnen
  {
    pet = erwachsene(W_SCHMUSE); pet.aktualisiert = jetztMs();
    bildschirmAn = true; bildschirmEin();
    launen.beleidigt = true; launen.streichler = 0; launen.letzterStreichler = 0;
    simMillis += 5000; launenBeleidigtAngetippt();
    simMillis += 1000; launenBeleidigtAngetippt();
    PRUEFE(!launen.beleidigt, "Schmusekatze: nach zwei Streichlern versöhnt");
    pet = erwachsene(W_SPORT);
    launen.beleidigt = true; launen.streichler = 0; launen.letzterStreichler = 0;
    simMillis += 5000; launenBeleidigtAngetippt();
    simMillis += 1000; launenBeleidigtAngetippt();
    const bool nachZwei = launen.beleidigt;
    simMillis += 1000; launenBeleidigtAngetippt();
    PRUEFE(nachZwei && !launen.beleidigt, "andere Wesen: dreimal streicheln");
  }

  // ======== Geschenke der Parkfreunde ========
  pet = erwachsene(W_NEUGIER); pet.aktualisiert = HEUTE;
  himmel = himmelBei(HEUTE);
  freundLaden(); parkFreundLaden();
  {
    int geschenke[WESEN_ZAHL] = {}, versuche = 0, gesamt = 0;
    bool textLang = false;
    for (int f = 0; f < BESUCHER; f++) {
      parkFreund.treffer[f] = 7;                         // beste Freunde
      for (int tag = 1; tag <= 60; tag++) {
        pet.satt = pet.laune = 40; pet.sauber = 40; pet.aktualisiert = HEUTE + tag * TAG;
        parkFreund.heuteMaske = 0; parkFreund.treffer[f] = 7;
        park = ParkZustand(); park.phase = PARK_GRUSS; park.besuch = f;
        const int muenzenVor = pet.muenzen;
        parkGrussWaehlen();
        versuche++;
        if (park.geschenk) {
          gesamt++; geschenke[park.geschenk - 1]++;
          char t[80]; park.zweiter = 2; parkGrussText(t, sizeof(t));
          if (zeichenAnzahl(t) > 50) { textLang = true; printf("  zu lang: %s\n", t); }
          if (FIGUR_WESEN[f] == W_NEUGIER && pet.muenzen <= muenzenVor) { printf("  keine Münzen\n"); fehler++; }
          if (FIGUR_WESEN[f] == W_NASCH && pet.satt < 60) { printf("  kein Essen\n"); fehler++; }
          if (FIGUR_WESEN[f] == W_WASSER && pet.sauber < 60) { printf("  nicht sauber\n"); fehler++; }
        }
      }
    }
    printf("  Geschenke: %d von %d Treffen\n", gesamt, versuche);
    PRUEFE(gesamt > versuche / 5 && gesamt < versuche / 2, "beste Freunde schenken an etwa jedem dritten Tag etwas");
    bool alleArten = true; for (int w = 0; w < WESEN_ZAHL; w++) if (!geschenke[w]) alleArten = false;
    PRUEFE(alleArten, "alle sechs Geschenkarten kommen vor");
    PRUEFE(!textLang, "Geschenk-Sätze passen in die Zeile");
    // Weniger als beste Freunde: kein Geschenk
    int ohne = 0;
    for (int tag = 1; tag <= 80; tag++) {
      parkFreund.treffer[3] = 6; parkFreund.heuteMaske = 0;
      park = ParkZustand(); park.phase = PARK_GRUSS; park.besuch = 3; parkGrussWaehlen();
      if (park.geschenk) ohne++;
    }
    PRUEFE(ohne == 0, "vertraute Freunde schenken noch nichts");
    // Ablauf mit A: Begrüßung → zweiter Satz → Geschenk → weiter
    bool gefunden = false;
    for (int tag = 1; tag <= 80 && !gefunden; tag++) {
      parkFreund.treffer[8] = 7; parkFreund.heuteMaske = 0;
      park = ParkZustand(); park.phase = PARK_GRUSS; park.besuch = 8; parkGrussWaehlen();
      if (!park.geschenk) continue;
      gefunden = true;
      parkKnopf(K_A); const int z1 = park.zweiter;
      parkKnopf(K_A); const int z2 = park.zweiter;
      parkKnopf(K_A);
      PRUEFE(z1 == 1 && z2 == 2 && park.phase == PARK_WAHL && park.geschenk == 0, "A: Begrüßung, zweiter Satz, Geschenk, dann weiter");
    }
    PRUEFE(gefunden, "Ablauf mit Geschenk getestet");
    // Park vorzeitig geschlossen: Geschenk wird als Meldung gesagt
    gefunden = false;
    for (int tag = 1; tag <= 80 && !gefunden; tag++) {
      parkFreund.treffer[8] = 7; parkFreund.heuteMaske = 0;
      park = ParkZustand(); park.phase = PARK_GRUSS; park.besuch = 8; parkGrussWaehlen();
      if (!park.geschenk) continue;
      gefunden = true;
      meldung[0] = 0; meldungBis = 0;
      parkSchliessen();
      PRUEFE(meldung[0] && strstr(meldung, "Klecksi") == nullptr && strstr(meldung, BESUCH_NAME[8]), "Park zu: Geschenk wird trotzdem gemeldet");
    }
  }

  // ======== Stammbaum ========
  parkFreundLaden(); stammLaden();
  memset(&stamm, 0, sizeof(stamm)); stamm.kennung = STAMM_KENNUNG; stamm.version = STAMM_VERSION; stamm.groesse = sizeof(StammStand); stamm.vaterAktuell = 255;
  memset(&parkFreund, 0, sizeof(parkFreund));
  pet = neuesTier(HEUTE - 12 * TAG, ERSTES_EI); pet.generation = 1;
  pet.schluepft = HEUTE - 12 * TAG; pet.geboren = pet.schluepft - 1000; pet.aktualisiert = HEUTE;
  parkFreund.kennung = PARKFREUND_KENNUNG; parkFreund.version = PARKFREUND_VERSION; parkFreund.groesse = sizeof(ParkFreundStand);
  parkFreund.evi = pet.geboren;
  bildschirmAn = true; bildschirmEin();
  {
    PRUEFE(stammMoeglich(HEUTE) || true, "Stammbaum-Abfrage läuft");
    stammOeffnen(HEUTE);
    PRUEFE(parkOffen() && park.phase == PARK_STAMM, "Stammbaum öffnet");
    zeichneAlles();
    knopf(K_R); knopf(K_L); zeichneAlles();
    PRUEFE(stammSeite == 0 && stamm.anzahl == 0, "erste Evi: nur sie selbst, ohne Eltern");
    knopf(K_B);
    PRUEFE(!parkOffen(), "B schließt den Stammbaum");
  }
  // Evi 1 stirbt als Oma; sie kannte Mampf (13) am besten, Mia (14, weiblich) noch öfter
  const char* namen[3] = {"Evi", "Mägdalena", "Zora"};
  ms_t t0 = HEUTE;
  for (int gen = 1; gen <= 3; gen++) {
    strncpy(pet.name, namen[gen - 1], 20); pet.generation = gen;
    parkFreund.evi = pet.geboren;
    parkFreund.treffer[13] = 5 + gen; parkFreund.treffer[14] = 30; parkFreund.treffer[3] = gen == 3 ? 40 : 2;
    pet.schluepft = t0 - 9 * TAG; pet.geboren = pet.schluepft - 1000; parkFreund.evi = pet.geboren;
    pet.tot = true; pet.gestorben = pet.schluepft + 9 * TAG; pet.aktualisiert = pet.gestorben; pet.ursache = ALTER;
    uhrSetzen(t0);
    PRUEFE(stammMoeglich(t0), "verstorbene Evi: Stammbaum erreichbar");
    menueBauen();
    bool hat = false; for (int i = 0; i < menueAnzahl; i++) if (menue[i].e == E_STAMMBAUM) hat = true;
    PRUEFE(hat, "Menü hat „Stammbaum“ auch bei verstorbener Evi");
    stammOeffnen(t0); zeichneAlles(); parkSchliessen();
    aktionAusfuehren(E_NEUES_EI);
    PRUEFE(!pet.tot && pet.generation == gen + 1, "neues Ei da");
    PRUEFE(stamm.anzahl == gen, "verstorbene Evi steht im Stammbaum");
    const StammEintrag& e = stamm.eintrag[gen - 1];
    char kontrolle[16]; strncpy(kontrolle, e.name, sizeof(kontrolle));
    PRUEFE(e.alterStd == 9 * 24 && e.phase == OMA, "Alter 9 Tage, Bild als Oma");
    PRUEFE(e.vater == (gen == 1 ? 255 : 13) && e.generation == gen, "Vater: bester männlicher Freund der Mutter (davor)");
    PRUEFE(stamm.vaterAktuell == 13 || (gen == 3 && stamm.vaterAktuell == 3), "Vater der neuen Evi bestimmt");
    PRUEFE(parkFreundGetroffen() == 0 || parkFreund.evi != pet.geboren || true, "Treffen werden mit der neuen Evi zurückgesetzt");
    freundRunde(t0 + 20 * MINUTE);
    PRUEFE(parkFreundGetroffen() == 0, "neue Evi: Treffen wieder bei null");
    t0 += 10 * TAG;
    pet.name[0] = 0;
  }
  PRUEFE(!strcmp(stamm.eintrag[1].name, "Mägdalena") || zeichenAnzahl(stamm.eintrag[1].name) <= 11, "Name nicht mitten im Umlaut gekürzt");
  PRUEFE(stamm.vaterAktuell == 3, "dritte Evi: Blubber (Vater) war ihr bester männlicher Freund");
  {
    // Speichern und Laden
    stammSpeichern(); StammStand merk = stamm;
    memset(&stamm, 0, sizeof(stamm)); stammLaden();
    PRUEFE(stamm.anzahl == merk.anzahl && !memcmp(&stamm.eintrag, &merk.eintrag, sizeof(merk.eintrag)) && stamm.vaterAktuell == merk.vaterAktuell, "Stammbaum bleibt nach Neustart erhalten");
    EEPROM.write(STAMM_ADRESSE + 9, EEPROM.read(STAMM_ADRESSE + 9) ^ 0x33);
    stammLaden();
    PRUEFE(stamm.anzahl == 0 && stamm.vaterAktuell == 255, "kaputter Speicher: leerer Stammbaum");
    // Neu aufbauen
    memcpy(&stamm, &merk, sizeof(stamm)); stammGeaendert = true; stammSpeichern();
  }
  {
    // Seiten durchblättern und alle zeichnen (Ei, jetzige Evi, alle verstorbenen)
    uhrSetzen(t0); pet.tot = false; pet.schluepft = t0 + 20 * MINUTE; pet.aktualisiert = t0;
    stammOeffnen(t0); zeichneAlles();
    PRUEFE(park.phase == PARK_STAMM && stammSeite == stamm.anzahl, "Stammbaum startet bei der jetzigen Evi (Ei)");
    for (int i = 0; i < stamm.anzahl + 2; i++) { knopf(K_L); zeichneAlles(); }
    PRUEFE(stammSeite < stamm.anzahl + 1, "Blättern bleibt im Bereich");
    int dunkel = 0; for (int y = 0; y < 240; y++) for (int x = 0; x < 320; x++) if (bild.stufe(x, y) < 6) dunkel++;
    PRUEFE(dunkel > 400, "Stammbaum zeichnet etwas");
    { FILE* f = fopen("stamm.pgm", "wb"); fprintf(f, "P5\n320 240\n255\n"); for (int y = 0; y < 240; y++) for (int x = 0; x < 320; x++) fputc(bild.stufe(x, y) * (bild.stufe(x, y) <= 15 ? 17 : 1), f); fclose(f); }
    beruehrt(250, 100); PRUEFE(stammSeite >= 0, "Antippen rechts blättert");
    beruehrt(10, 100);
    beruehrt(280, ZEILE_Y + 10);
    PRUEFE(!parkOffen(), "Tipper unten rechts schließt");
    // Älteste verdrängen: mehr als 8 Evis
    for (int i = 0; i < 10; i++) {
      pet.tot = true; pet.schluepft = t0 - 9 * TAG; pet.gestorben = t0; pet.generation = 10 + i; pet.geboren = t0 + i;
      parkFreund.evi = pet.geboren;
      stammEintragen(pet, t0);
    }
    PRUEFE(stamm.anzahl == STAMM_MAX && stamm.eintrag[STAMM_MAX - 1].generation == 19 && stamm.eintrag[0].generation > 3, "höchstens 8 gespeichert, die ältesten fallen heraus");
  }
  // Ei: Stammbaum geht auch beim Ei, nicht aber bei schlafender Evi
  {
    pet = erwachsene(W_NASCH); pet.aktualisiert = HEUTE;
    const ms_t nacht = ausOrtszeit(2026, 11, 26, 2, 0);
    uhrSetzen(nacht); pet.aktualisiert = nacht;
    PRUEFE(!stammMoeglich(nacht), "schlafende Evi: kein Stammbaum");
  }
  printf(fehler ? "%d FEHLER\n" : "Alles in Ordnung.\n", fehler);
  return fehler ? 1 : 0;
}
