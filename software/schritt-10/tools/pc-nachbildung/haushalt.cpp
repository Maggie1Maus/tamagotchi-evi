// Haushalt: Verlieben, Klopfen, Partner, Ei, Baby, das Tamagotchi davor (26.11.2026)
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
void foto(const char* name) { FILE* f = fopen(name, "wb"); fprintf(f, "P5\n320 240\n255\n"); for (int y = 0; y < 240; y++) for (int x = 0; x < 320; x++) fputc(bild.stufe(x, y) * (bild.stufe(x, y) <= 15 ? 17 : 1), f); fclose(f); }
void laufen(uint32_t ms) { for (uint32_t t = 0; t < ms; t += 10) { simMillis += 10; loop(); } }
void zeit(ms_t t) { uhrSetzen(t); pet.aktualisiert = t; pet.satt = pet.laune = pet.sauber = 90; himmel = himmelBei(t); }
const ms_t SCHLUEPFT = ausOrtszeit(2026, 6, 3, 8, 0);              // Erwachsenenleben: 6.6. 8:00 bis 11.6. 8:00, letzter Tag ab 10.6. 8:00
const ms_t TAG_START = SCHLUEPFT + 7 * TAG, TAG_ENDE = SCHLUEPFT + 8 * TAG;
ms_t geburtFuer(int wesen) { for (int i = 0; i < 100000; i++) { const ms_t g = SCHLUEPFT - 1000 * (ms_t)(i + 1); if (wesenVon(g) == wesen) return g; } return 0; }
void neuesLeben(uint8_t geschlecht, const char* name = "Zora") {
  pet = neuesTier(SCHLUEPFT, 0, name, true);
  pet.geboren = geburtFuer(W_NASCH); pet.schluepft = SCHLUEPFT; pet.generation = 2; pet.muenzen = 77;
  stamm.geschlechtAktuell = geschlecht; stamm.elternAktuell = 255; stamm.elternNameAktuell = 255; stamm.anzahl = 0; stamm.neueNamenAb = 0;
  parkFreund.evi = pet.geboren; memset(parkFreund.treffer, 0, sizeof(parkFreund.treffer));
  haushaltLeeren(); partnerB = Bewohner(); vorigeB = Bewohner();
  park = ParkZustand(); sammlung = Sammlung();
}
int main() {
  bildschirmStarten(); bild.cp437(true);
  uhrSetzen(TAG_START);
  freundLaden(); parkFreundLaden(); stammLaden(); haushaltLaden(); tastenStarten();
  neuesLeben(0);
  moment.naechster = TAG_START + 90 * TAG; moment.letzterBesonderer = TAG_START;
  bildschirmAn = true; bildschirmEin();

  // ---- Zeiten ----
  PRUEFE(erwachsenEnde(pet) == TAG_ENDE && !istErwachsen(pet, SCHLUEPFT + TAG) && istErwachsen(pet, TAG_START), "Erwachsenenleben: 5 Tage ab Tag 3, endet bei 8 Tagen");
  // ---- Verlieben nur zwischen Mädchen und Junge ----
  {
    bool maedchen = true, junge = true;
    stamm.geschlechtAktuell = 0; for (int f = 0; f < BESUCHER; f++) if (gegengeschlecht(f) != !figurWeiblich(f)) maedchen = false;
    stamm.geschlechtAktuell = 1; for (int f = 0; f < BESUCHER; f++) if (gegengeschlecht(f) != figurWeiblich(f)) junge = false;
    stamm.geschlechtAktuell = 0;
    PRUEFE(maedchen && junge, "Mädchen verlieben sich nur in Jungen, Jungen nur in Mädchen");
  }
  // ---- Klopfen am letzten erwachsenen Tag ----
  neuesLeben(0);
  zeit(ausOrtszeit(2026, 6, 9, 17, 0));                     // einen Tag vorher
  haushaltTakt(jetztMs());
  PRUEFE(!parkOffen(), "einen Tag vorher klopft es nicht");
  zeit(ausOrtszeit(2026, 6, 10, 7, 30)); haushaltTakt(jetztMs());
  PRUEFE(!parkOffen(), "nachts/früh morgens klopft es nicht");
  zeit(ausOrtszeit(2026, 6, 10, 9, 30)); haushaltTakt(jetztMs());
  PRUEFE(parkOffen() && park.phase == PARK_LIEBE && park.liebeArt == 1, "9:30 am letzten Tag: es klopft, die Frage „Partner?“");
  const int kandidat1 = park.liebeKandidat;
  PRUEFE(!figurWeiblich(kandidat1), "das Mädchen bekommt einen Jungen vor die Tür");
  zeichneAlles(); foto("haus-klopfen.pgm");
  zeit(ausOrtszeit(2026, 6, 10, 9, 40)); park.liebeWahl = 1; knopf(K_A);
  PRUEFE(!parkOffen() && haushalt.partner == 255 && (haushalt.abgelehntKlopf >> kandidat1 & 1), "Nein: kein Partner, Kandidat abgelehnt");
  haushaltTakt(jetztMs());
  PRUEFE(!parkOffen(), "im selben Zeitfenster klopft es nicht noch einmal");
  zeit(ausOrtszeit(2026, 6, 10, 11, 30)); haushaltTakt(jetztMs());
  PRUEFE(park.phase == PARK_LIEBE && park.liebeKandidat != kandidat1, "zwei Stunden später: ein anderer Kandidat");
  const int kandidat2 = park.liebeKandidat;
  // Alle Kandidaten durchlaufen: kein Kandidat kommt zweimal, bis alle abgelehnt sind
  { bool doppelt = false; uint32_t gesehen = 1UL << kandidat1 | 1UL << kandidat2; park.liebeWahl = 1; knopf(K_B);
    for (int h = 13; h <= 19; h += 2) { zeit(ausOrtszeit(2026, 6, 10, h, 10)); haushaltTakt(jetztMs()); if (park.phase != PARK_LIEBE) { doppelt = true; break; } if (gesehen >> park.liebeKandidat & 1) doppelt = true; gesehen |= 1UL << park.liebeKandidat; knopf(K_B); }
    PRUEFE(!doppelt, "sechs Klopfzeiten, sechs verschiedene Kandidaten"); }
  zeit(ausOrtszeit(2026, 6, 10, 21, 30)); haushalt.klopfErledigt = 0; haushaltTakt(jetztMs());
  PRUEFE(!parkOffen(), "nach 21 Uhr klopft es nicht");

  // ---- Partner einladen ----
  neuesLeben(0);
  zeit(ausOrtszeit(2026, 6, 10, 9, 30)); haushaltTakt(jetztMs());
  const int partner = park.liebeKandidat;
  knopf(K_A);                                                   // Ja
  PRUEFE(!parkOffen() && haushalt.partner == partner && haushalt.partnerSeit > 0, "Ja: der Partner zieht ein");
  PRUEFE(nameVonCode(haushalt.partnerName)[0] && strcmp(nameVonCode(haushalt.partnerName), "unbekannt"), "sein Name wird mitgespeichert");
  const ms_t morgenMax = ausOrtszeit(2026, 6, 10, 11, 0);
  PRUEFE(haushalt.eiGeplant >= jetztMs() + 20 * MINUTE - 1000 && haushalt.eiGeplant <= morgenMax, "Ei am Vormittag, frühestens 20 Minuten nach dem Einzug");
  haushaltTakt(jetztMs()); PRUEFE(!parkOffen(), "mit Partner klopft es nicht mehr");
  bool nie = true; for (int i = 0; i < 2000; i++) if (parkBesucher(TAG_START + i * 4 * STUNDE) == partner) nie = false;
  PRUEFE(nie, "der Partner steht nie mehr im Park");
  // Dorf: wohnt bei dir
  parkFreund.treffer[partner] = 5;
  park = ParkZustand(); park.phase = PARK_DORF; dorfWaehle(partner); knopf(K_A);
  PRUEFE(park.phase == PARK_DORF && park.meldung == 3, "im Dorf: „wohnt jetzt bei dir“");
  zeichneAlles(); park = ParkZustand();
  // Ei wird nicht vor der Zeit gelegt
  haushaltTakt(haushalt.eiGeplant - MINUTE);
  PRUEFE(haushalt.eiZustand == 0, "vor der Zeit kein Ei");
  // Partner und Zimmer zeichnen, Bewohner laufen
  zeit(haushalt.eiGeplant - 5 * MINUTE); partnerB.x = 200; partnerB.fuss = 190;
  for (int i = 0; i < 200; i++) { simMillis += 120; haushaltBewegen(jetztMs()); }
  zeichneAlles(); foto("haus-partner.pgm");
  PRUEFE(partnerB.breite > 0 && partnerB.x >= 20 && partnerB.x <= 310 && partnerB.fuss >= 140 && partnerB.fuss <= 200, "der Partner läuft frei im Zimmer (Originalgröße)");
  // Ei legen
  const ms_t eiZeit = haushalt.eiGeplant;
  zeit(eiZeit); haushaltTakt(jetztMs());
  PRUEFE(haushalt.eiZustand == 1 && haushalt.eiSchluepftAm >= eiZeit + EI_HUEPFT - 2000, "Ei gelegt: es hüpft eine Minute");
  zeichneAlles(); foto("haus-ei.pgm");
  zeit(eiZeit + 30 * 1000); haushaltTakt(jetztMs());
  PRUEFE(haushalt.eiZustand == 1 && pet.generation == 2, "nach 30 Sekunden hüpft das Ei noch");
  // Schlüpfen
  const ms_t schluepfZeit = haushalt.eiSchluepftAm;
  const uint32_t muenzen = pet.muenzen;
  sammlung.anzahl = 0; fundAufnehmen(FUND_STEIN, FUND_GASSI, 20260601, 2);
  zeit(schluepfZeit + 1000);
  const uint8_t partnerCode = haushalt.partnerName; const int partnerFigur = haushalt.partner;
  haushaltTakt(jetztMs());
  PRUEFE(pet.generation == 3 && !pet.name[0] && phaseBei(pet, jetztMs()) == BABY && pet.muenzen == muenzen, "das Ei ist geschlüpft: neues Baby (Generation 3), ohne Namen, Münzen bleiben");
  PRUEFE(haushalt.partner == 255 && haushalt.eiZustand == 0, "der Partner ist wieder weg");
  PRUEFE(sammlung.anzahl == 0, "das Baby beginnt mit leerer Sammlung");
  PRUEFE(haushalt.vorigeDa == 1 && !strcmp(haushalt.vorigeName, "Zora"), "das Tamagotchi davor lebt im Haus weiter");
  PRUEFE(stamm.anzahl == 1 && !strcmp(stamm.eintrag[0].name, "Zora") && stamm.eintrag[0].alterStd == 240 && stamm.eintrag[0].phase == OMA, "Zora steht mit ihrem ganzen Leben (10 Tage) im Stammbaum");
  PRUEFE(stamm.elternAktuell == partnerFigur && stamm.elternNameAktuell == partnerCode, "Eltern des Babys: Zora und der Partner (Name gespeichert)");
  PRUEFE(evi.schluepfen, "Schlüpf-Animation läuft");
  // Das Baby bekommt einen Namen
  laufen(3000);
  PRUEFE(park.phase == PARK_NAME, "danach fragt die Namenseingabe");
  strcpy(nameEdit.text, "Hugo"); namenFertig(jetztMs());
  PRUEFE(!strcmp(pet.name, "Hugo") && stamm.neueNamenAb == 3, "Baby heißt Hugo, die Figuren heißen ab jetzt neu");
  // Stammbaum: Familie Zora + Partner → Hugo
  stammOeffnen(jetztMs()); zeichneAlles(); foto("haus-stamm.pgm"); parkSchliessen();
  // ---- Das Tamagotchi davor im Zimmer ----
  zeit(ausOrtszeit(2026, 6, 11, 14, 0)); pet.schluepft = jetztMs(); pet.aktualisiert = jetztMs();   // das Baby bleibt ein Baby
  vorigeB.x = 150; vorigeB.fuss = 190;
  for (int i = 0; i < 100; i++) { simMillis += 120; haushaltBewegen(jetztMs()); }
  zeichneAlles(); foto("haus-oma.pgm");
  PRUEFE(vorigeB.breite > 0, "Oma/Opa wird im Zimmer gezeichnet");
  { const int tx = vorigeB.links + vorigeB.breite / 2, ty = vorigeB.oben + vorigeB.hoehe / 2;
    sagen.bis = 0; beruehrt(tx, ty);
    PRUEFE(sagen.bis != 0 && strstr(sagen.text, "Zora"), "Antippen: sie reagiert mit einem Satz"); }
  // Nachts schläft sie auf dem Teppich
  { zeit(ausOrtszeit(2026, 6, 12, 2, 0)); pet.schluepft = jetztMs() - 4 * 60 * MINUTE; vorigeB.modus = 0; vorigeB.bis = 0;
    for (int i = 0; i < 400; i++) { simMillis += 120; haushaltBewegen(jetztMs()); }
    const auto& t = einrichtung.platz[Z_TEPPICH];
    PRUEFE(vorigeB.modus == 3 && abs(vorigeB.x - (t.x + ZG_TEPPICH.w / 2)) <= 4, "nachts schläft sie auf dem Teppich");
    zeichneAlles(); }
  // Tod und Begraben
  { const ms_t tod = haushalt.vorigeSchluepft + R::LEBENSDAUER;
    zeit(tod - 10 * MINUTE); pet.schluepft = jetztMs(); haushaltTakt(jetztMs());
    PRUEFE(haushalt.vorigeDa && !haushalt.vorigeGemeldet, "kurz vor dem Ende lebt sie noch");
    zeit(tod + MINUTE); pet.schluepft = jetztMs(); haushaltTakt(jetztMs()); }
  PRUEFE(haushalt.vorigeGemeldet && strstr(meldung, "friedlich eingeschlafen"), "sie schläft friedlich ein: kleine Meldung");
  for (int i = 0; i < 10; i++) { simMillis += 120; haushaltBewegen(jetztMs()); }
  zeichneAlles(); foto("haus-liegt.pgm");
  { const int tx = vorigeB.links + vorigeB.breite / 2, ty = vorigeB.oben + vorigeB.hoehe / 2;
    beruehrt(tx, ty);
    PRUEFE(park.phase == PARK_LIEBE && park.liebeArt == 2, "Antippen der Liegenden: „Begraben?“");
    zeichneAlles(); foto("haus-begraben.pgm");
    park.liebeWahl = 1; knopf(K_A);
    PRUEFE(!parkOffen() && haushalt.vorigeDa == 1, "„Noch nicht“: sie bleibt liegen");
    beruehrt(tx, ty); knopf(K_A);
    PRUEFE(!parkOffen() && haushalt.vorigeDa == 0, "Begraben: sie verschwindet sofort aus dem Zimmer"); }
  // Liegt 15 Minuten, dann ist sie von selbst weg
  { haushalt.vorigeDa = 1; haushalt.vorigeGemeldet = 1; const ms_t tod = haushalt.vorigeSchluepft + R::LEBENSDAUER;
    zeit(tod + 14 * MINUTE); haushaltTakt(jetztMs()); const bool noch = haushalt.vorigeDa;
    zeit(tod + 16 * MINUTE); haushaltTakt(jetztMs());
    PRUEFE(noch && !haushalt.vorigeDa, "nach 15 Minuten liegt sie nicht mehr im Zimmer"); }

  // ---- Ein Junge: Kandidatinnen ----
  neuesLeben(1, "Paul");
  zeit(ausOrtszeit(2026, 6, 10, 9, 30)); haushaltTakt(jetztMs());
  PRUEFE(park.phase == PARK_LIEBE && figurWeiblich(park.liebeKandidat), "ein Junge bekommt Mädchen vor die Tür");
  { const int f = park.liebeKandidat; knopf(K_A);
    parkFreund.evi = pet.geboren;
    zeit(haushalt.eiGeplant); haushaltTakt(jetztMs()); zeit(haushalt.eiSchluepftAm + 1000); haushaltTakt(jetztMs());
    PRUEFE(stamm.elternAktuell == f && figurWeiblich(f) && stamm.eintrag[0].geschlecht == 1, "Paul (Junge) und seine Partnerin: Elternteil ist eine weibliche Figur"); }

  // ---- Zu spät: kein Ei, der Partner zieht wieder aus ----
  neuesLeben(0);
  zeit(TAG_ENDE - 5 * MINUTE); haushalt.partner = 8; haushalt.partnerName = 100 + 8; haushalt.partnerSeit = jetztMs(); haushalt.eiGeplant = eiZeitPlanen(jetztMs());
  PRUEFE(haushalt.eiGeplant == 0, "5 Minuten vor Schluss: es bleibt keine Zeit für ein Ei");
  zeit(TAG_ENDE + MINUTE); meldung[0] = 0; haushaltTakt(jetztMs());
  PRUEFE(haushalt.partner == 255 && strstr(meldung, "ausgezogen") && pet.generation == 2, "kein Ei: der Partner zieht wieder aus, das Tamagotchi bleibt Oma ohne Baby");
  zeit(TAG_ENDE + MINUTE); haushalt.partner = 255;
  // später Einzug am letzten Tag
  neuesLeben(0);
  zeit(ausOrtszeit(2026, 6, 10, 15, 0)); haushaltTakt(jetztMs()); knopf(K_A);
  PRUEFE(haushalt.partner != 255 && haushalt.eiGeplant >= jetztMs() + 20 * MINUTE - 1000 && haushalt.eiGeplant < jetztMs() + 30 * MINUTE, "Partner erst am Nachmittag: Ei 20 Minuten später");
  // Liebe über Freundschaft (Stufe 7) im Dorf
  neuesLeben(0);
  zeit(ausOrtszeit(2026, 6, 9, 12, 0));                      // ein Tag vor dem Ende
  pet.schluepft = SCHLUEPFT;
  { int f = -1; for (int i = 0; i < BESUCHER; i++) if (!figurWeiblich(i)) { f = i; break; }
    parkFreund.treffer[f] = 6;
    PRUEFE(!liebeMoeglich(f, jetztMs()), "mit 6 Treffen noch keine Frage");
    parkFreund.treffer[f] = 7;
    PRUEFE(liebeMoeglich(f, jetztMs()), "ab 7 Treffen (beste Freunde) kann man sich verlieben");
    int w = -1; for (int i = 0; i < BESUCHER; i++) if (figurWeiblich(i)) { w = i; break; }
    parkFreund.treffer[w] = 9;
    PRUEFE(!liebeMoeglich(w, jetztMs()), "ein Mädchen verliebt sich nicht in eine weibliche Figur (nur beste Freundinnen)");
    // Dorfbesuch: nach dem Verlassen kommt die Frage
    ms_t z = ausOrtszeit(2026, 6, 9, 12, 0);
    while (parkBesucher(z) == f) z += 4 * STUNDE;
    zeit(z);
    parkFreund.heuteTag = 0;
    park = ParkZustand(); park.phase = PARK_DORF; dorfWaehle(f);
    knopf(K_A); PRUEFE(park.phase == PARK_BESUCHT, "Besuch bei dem besten Freund");
    knopf(K_B);
    PRUEFE(park.phase == PARK_LIEBE && park.liebeArt == 0 && park.liebeKandidat == f && park.liebeZurueck == PARK_DORF, "beim Verlassen des Hauses: „Soll er dein Partner werden?“");
    zeichneAlles(); foto("haus-liebe.pgm");
    park.liebeWahl = 1; knopf(K_A);
    PRUEFE(park.phase == PARK_DORF && haushalt.partner == 255, "Nein: zurück im Dorf, kein Partner");
    PRUEFE(!liebeMoeglich(f, jetztMs()), "heute wird nicht noch einmal gefragt");
    knopf(K_A); knopf(K_B);
    PRUEFE(park.phase == PARK_DORF, "weitere Besuche am selben Tag fragen nicht noch einmal");
    zeit(z + TAG); pet.schluepft = SCHLUEPFT;
    PRUEFE(jetztMs() < erwachsenEnde(pet) - STUNDE, "Testzeit liegt vor dem Ende");
    haushalt.abgelehntTag = 0;
    PRUEFE(liebeMoeglich(f, jetztMs()), "am nächsten Tag wird wieder gefragt");
    // Ja im Park-Treffen
    park = ParkZustand(); park.phase = PARK_GRUSS; park.besuch = f; park.zweiter = 1; park.geschenk = 0;
    knopf(K_A);
    PRUEFE(park.phase == PARK_LIEBE && park.liebeZurueck == PARK_WAHL, "nach der Begrüßung im Park: dieselbe Frage");
    knopf(K_A);
    PRUEFE(haushalt.partner == f && park.phase == PARK_WAHL, "Ja: der beste Freund zieht ein, zurück im Park");
    PRUEFE(!liebeMoeglich(f, jetztMs()) && !liebeMoeglich(1, jetztMs()), "mit Partner keine weitere Frage");
  }
  // ---- Speichern ----
  {
    haushalt.partner = 3; haushalt.partnerName = 12; haushalt.eiZustand = 1; haushalt.eiSchluepftAm = 12345; strcpy(haushalt.vorigeName, "Oma Lu");
    haushaltGeaendert = true; haushaltSpeichern();
    HaushaltStand merk = haushalt; memset(&haushalt, 0, sizeof(haushalt)); haushaltLaden();
    PRUEFE(!memcmp(&merk, &haushalt, offsetof(HaushaltStand, pruefsumme)) && haushalt.partner == 3, "Haushalt übersteht den Neustart");
    EEPROM.write(HAUSHALT_ADRESSE + 9, EEPROM.read(HAUSHALT_ADRESSE + 9) ^ 0x21);
    haushaltLaden();
    PRUEFE(haushalt.partner == 255 && !haushalt.eiZustand && !haushalt.vorigeDa, "kaputter Speicher: leerer Haushalt");
  }
  // ---- Frühes Sterben: Notlösung „Neues Ei“ bleibt ----
  neuesLeben(0, "Lena");
  zeit(SCHLUEPFT + 5 * TAG); pet.tot = true; pet.gestorben = jetztMs(); pet.ursache = VERNACHLAESSIGT;
  haushalt.partner = 8; haushaltTakt(jetztMs());
  PRUEFE(haushalt.partner == 255, "stirbt das Tamagotchi, zieht der Partner wieder aus");
  menueBauen(); { bool ei = false; for (int i = 0; i < menueAnzahl; i++) if (menue[i].e == E_NEUES_EI) ei = true; PRUEFE(ei, "„Neues Ei“ bleibt als Notlösung im Menü"); }
  parkFreund.treffer[13] = 5; aktionAusfuehren(E_NEUES_EI);
  PRUEFE(pet.generation == 3 && !pet.name[0] && stamm.anzahl == 1 && stamm.elternAktuell == 13, "Notlösung: neues Ei ohne Namen, Elternteil nach Freundschaft");
  printf(fehler ? "%d FEHLER\n" : "Alles in Ordnung.\n", fehler);
  return fehler ? 1 : 0;
}
