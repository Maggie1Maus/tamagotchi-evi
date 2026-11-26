// Haushalt (26.11.2026, nur Waveshare): Verlieben, Partner, Ei, Baby und das Tamagotchi davor. Logik in haushalt.ino.
//  - Verlieben nur zwischen Mädchen und Junge. Ein erwachsenes Tamagotchi ohne Partner kann sich in eine Figur des anderen
//    Geschlechts verlieben, die es als beste Freundin/besten Freund (Stufe 7) kennt: Im Dorf oder im Park kommt die Frage.
//  - Am letzten erwachsenen Tag (die letzten 24 Stunden des Erwachsenenlebens) klopft es ohne Partner alle zwei Stunden
//    tagsüber (9–21 Uhr); ein Kandidat steht vor der Tür. Bei „Nein“ kommt zwei Stunden später ein neuer.
//  - Der Partner zieht ein (lebt frei im Zimmer, versorgt sich selbst). Das Ei wird am Morgen/Vormittag des letzten
//    erwachsenen Tages gelegt (später, wenn der Partner später einzieht), hüpft etwa eine Minute und schlüpft.
//  - Mit dem Schlüpfen geht der Partner. Das Baby ist das neue Tamagotchi der Nutzerin; das bisherige lebt als Oma/Opa im
//    Zimmer weiter (ohne Pflege), schläft auf dem Teppich, und bleibt nach seinem Tod 15 Minuten liegen (oder wird begraben).
#pragma once

const uint32_t HAUSHALT_KENNUNG = 0x48415553;   // "HAUS"
const uint16_t HAUSHALT_VERSION = 1;
const int HAUSHALT_ADRESSE = 2200;              // frei seit dem verkleinerten Spielstand (2120–3328)
const ms_t EI_HUEPFT = 60 * 1000;               // so lange hüpft das Ei, bis es schlüpft
const ms_t LIEGT_NACH_TOD = 15 * MINUTE;        // so lange liegt die Oma nach ihrem Tod im Zimmer

struct HaushaltStand {
  uint32_t kennung;
  uint16_t version, groesse;
  uint8_t partner = 255;      // Figur, die bei uns wohnt (255 = keiner)
  uint8_t partnerName = 255;  // Namenscode des Partners (namen.ino), damit er Namen behält
  uint8_t eiZustand;          // 0 kein Ei, 1 Ei hüpft
  uint8_t vorigeDa;           // 1 = das Tamagotchi davor ist noch im Haus
  uint8_t vorigeGeschlecht;   // 0 Mädchen, 1 Junge
  uint8_t vorigeGemeldet;     // „ist eingeschlafen“ wurde schon gesagt
  uint8_t klopfErledigt;      // Bit je Zeitfenster (9, 11, … 19 Uhr) heute schon geklopft
  uint8_t reserviert;
  ms_t partnerSeit, eiGeplant, eiSchluepftAm;     // eiGeplant: wann das Ei gelegt wird (0 = kein Ei)
  ms_t vorigeGeboren, vorigeSchluepft;
  int32_t klopfTag, abgelehntTag;
  uint32_t abgelehntKlopf;    // Bit je Figur: als Kandidat an der Tür abgelehnt (in diesem Erwachsenenleben)
  uint32_t abgelehntLiebe;    // Bit je Figur: heute die Frage „Partner?“ abgelehnt
  char vorigeName[12];
  uint32_t pruefsumme;
};
static_assert(HAUSHALT_ADRESSE + sizeof(HaushaltStand) <= ZIMMER_ADRESSE, "Haushalt überschneidet das Zimmer");
static_assert(STAMM_ADRESSE + sizeof(StammStand) <= HAUSHALT_ADRESSE, "Haushalt überschneidet den Stammbaum");
HaushaltStand haushalt;   // ohne Partner (255), bis haushaltLaden gelesen hat
bool haushaltGeaendert = false;

// Bewohner im Zimmer: Partner und das Tamagotchi davor laufen frei herum (nur Arbeitsspeicher)
struct Bewohner {
  int x = 160, fuss = 190, zielX = 160, zielFuss = 190;
  uint32_t bis = 0;        // millis(): so lange bleibt der Modus
  uint8_t modus = 0;       // 0 steht, 1 läuft, 2 schaut aus dem Fenster, 3 schläft auf dem Teppich
  bool flip = false;
  uint32_t reaktionSeit = 0;
  int links = 0, oben = 0, breite = 0, hoehe = 0;   // wo er zuletzt gezeichnet wurde (zum Antippen)
};
Bewohner partnerB, vorigeB;
const Note TON_KLOPF[] = {{150, 70}, {0, 90}, {150, 70}, {0, 260}, {150, 70}, {0, 90}, {150, 70}};
