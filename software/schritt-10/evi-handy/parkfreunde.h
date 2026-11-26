// Freunde im Park, Album und Evis Wesen (26.11.2026, nur Waveshare). Logik in parkfreunde.ino.
//  - Jede der 20 Figuren merkt sich, wie oft Evi sie getroffen hat (höchstens einmal pro Tag und Figur).
//    Eine neue Evi fängt wieder bei null an.
//    Danach ändert sich die Begrüßung: Fremde, Bekannte, Vertraute, Beste Freunde.
//  - Nach der Begrüßung sagt der Besuch noch einen zweiten Satz (A): zum Wetter, zur Jahreszeit, zu Funden,
//    über sich selbst oder, wenn er zum Wesen von Evi passt, dass sie ähnlich sind.
//  - Jede Evi hat ein Wesen (aus ihrem Zeitpunkt „geboren“ ausgewürfelt) mit kleinen Folgen für Freundschaft und Park.
//  - „Parkfreunde“ im Menü zeigt das Album aller 20 Figuren.
#pragma once

// Evis Wesen (und das der Figuren)
enum Wesen : uint8_t { W_NASCH, W_SPORT, W_WASSER, W_LANG, W_SCHMUSE, W_NEUGIER, WESEN_ZAHL };
const char* const WESEN_NAME[WESEN_ZAHL] = {"Naschkatze", "Sportlerin", "wasserscheu", "Langschläferin", "Schmusekatze", "Neugierige"};

// Reihenfolge wie BESUCH_NAME: Lotti, Bit, Blip, Blubber, Ella, Funke, Glöckchen, Groschen, Kiwi, Klecksi,
// Leo, Linus, Lumi, Mampf, Mia, Mimi, Moosi, Rolli, Spuki, Tetro (Wesen aus entwuerfe/neue-tamagotchis/vorschlaege.md)
const uint8_t FIGUR_WESEN[BESUCHER] = {W_LANG, W_NEUGIER, W_NEUGIER, W_SCHMUSE, W_WASSER, W_SPORT, W_SCHMUSE, W_NASCH, W_NASCH, W_SPORT,
                                       W_SPORT, W_NEUGIER, W_NEUGIER, W_NASCH, W_NASCH, W_LANG, W_WASSER, W_SPORT, W_WASSER, W_LANG};
const char* const FIGUR_ART[BESUCHER] = {"verträumt", "neugierig, schlau", "frech, chaotisch", "sanft, tollpatschig", "lieb, unsicher",
                                         "aufgedreht", "hilfsbereit", "sparsam", "fröhlich", "wild, verspielt", "selbstbewusst",
                                         "mutig", "redselig", "verfressen", "vornehm, eitel", "gemütlich", "versteckt sich gern",
                                         "flink, ungeduldig", "schüchtern, treu", "ruhig, ordentlich"};

// Wie gut Evi eine Figur kennt: 0 fremd (nie getroffen), 1 bekannt (1–2 Treffen), 2 vertraut (3–6), 3 beste Freunde (ab 7)
const char* const FIGUR_STUFE_NAME[4] = {"Fremd", "Bekannt", "Vertraut", "Beste Freunde"};
const uint8_t TREFFEN_VERTRAUT = 3, TREFFEN_BESTE = 7;

// Dauerhaft gespeichert (EEPROM). Die Treffen gehören zu einer Evi: Bei einer neuen Evi fängt alles von vorne an.
struct ParkFreundStand {
  uint32_t kennung;
  uint16_t version, groesse;
  ms_t evi;                      // zu welcher Evi die Treffen gehören (pet.geboren); neue Evi = von vorne
  uint8_t treffer[BESUCHER];     // wie oft getroffen (höchstens 255)
  int32_t heuteTag;              // Tag (tagKennung) zu heuteMaske
  uint32_t heuteMaske;           // Bit je Figur: heute schon begrüßt
  uint32_t pruefsumme;
};
const uint32_t PARKFREUND_KENNUNG = 0x50524B46;   // "PRKF"
const uint16_t PARKFREUND_VERSION = 2;   // 2: Treffen gehören zu einer Evi (Version 1 wird verworfen)
const int PARKFREUND_ADRESSE = 3856;              // hinter Freundschaft (3584 + 216), vor dem Geburtstags-Merker (4095)
static_assert(FREUND_ADRESSE + sizeof(FreundStand) <= PARKFREUND_ADRESSE, "Parkfreunde überschneiden die Freundschaft");
static_assert(PARKFREUND_ADRESSE + sizeof(ParkFreundStand) <= EEPROM_GROESSE - 1, "Parkfreunde überschneiden den Merker am Ende");
ParkFreundStand parkFreund;
bool parkFreundGeaendert = false;

// Album (läuft als Phase PARK_ALBUM im Park-Zustand, damit Tasten, Touch und Schlafen schon passen)
const int ALBUM_SPALTEN = 5, ALBUM_ZEILEN = 4;
uint8_t albumWahl = 0;
