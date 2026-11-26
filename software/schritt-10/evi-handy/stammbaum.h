// Stammbaum (26.11.2026, nur Waveshare). Logik in stammbaum.ino.
//  - Wenn eine neue Evi schlüpft, kommt die verstorbene Evi in den Stammbaum: Name, Wesen, Alter und ihr Bild
//    in der Lebensphase, in der sie gegangen ist (meist als Oma).
//  - Ein Elternteil ist das Tamagotchi davor (Mutter oder Vater, je nach seinem Geschlecht), der andere die Parkfreund-
//    Figur des anderen Geschlechts, die es am häufigsten getroffen hat (ohne Treffen: unbekannt). Der Name dieser Figur
//    wird mitgespeichert, denn die Figuren heißen bei jedem Tamagotchi anders (namen.h).
//  - Gespeichert werden bis zu 50 Tamagotchis; das aktuelle steht immer am Ende.
//  - Die Ansicht ist eine lange Fläche von oben (älteste Familie) nach unten, die man verschiebt (Tasten oder Finger).
#pragma once

struct StammEintrag {
  char name[12];                 // gekürzt, passt in die Spalte
  uint8_t wesen, geschlecht;     // geschlecht: 0 Mädchen, 1 Junge
  uint8_t eltern, elternName;    // Parkfreund-Elternteil: Nummer der Figur (255 = unbekannt) und Namenscode (namen.ino)
  uint8_t phase, generation;     // phase: bei ihrem Tod
  uint16_t alterStd;             // so viele Stunden hat sie gelebt
};
const int STAMM_MAX = 50;   // seit 26.11.2026 (vorher 8)
struct StammStand {
  uint32_t kennung;
  uint16_t version, groesse;
  uint8_t anzahl;                // wie viele verstorbene Tamagotchis gespeichert sind
  uint8_t elternAktuell;         // Parkfreund-Elternteil des jetzigen Tamagotchis (255 = unbekannt)
  uint8_t elternNameAktuell;     // sein Namenscode
  uint8_t geschlechtAktuell;     // Geschlecht des jetzigen Tamagotchis (0 Mädchen, 1 Junge), gesetzt beim Benennen
  uint8_t neueNamenAb;           // ab dieser Generation heißen die Figuren neu (0 = noch nie ein Baby benannt)
  uint8_t reserviert[3];
  StammEintrag eintrag[STAMM_MAX];   // älteste zuerst
  uint32_t pruefsumme;
};
const uint32_t STAMM_KENNUNG = 0x53544D4D;   // "STMM"
const uint16_t STAMM_VERSION = 3;   // 3: 50 Einträge, neuer Platz (Version 2 mit 8 Einträgen bei 3904 wird übernommen)
// Neuer Platz im Bereich, den der verkleinerte Spielstand freigegeben hat; der alte Platz (Version 2) wird nur noch gelesen.
const int STAMM_ADRESSE = 1100, STAMM_ADRESSE_V2 = 3904;
static_assert(sizeof(Spielstand) + 16 <= STAMM_ADRESSE, "Stammbaum überschneidet den Spielstand");
static_assert(STAMM_ADRESSE + sizeof(StammStand) <= ZIMMER_ADRESSE, "Stammbaum überschneidet das Zimmer");
// Version 2 (8 Einträge), nur zum Übernehmen. Nicht verändern.
struct StammStandV2 {
  uint32_t kennung;
  uint16_t version, groesse;
  uint8_t anzahl, elternAktuell, elternNameAktuell, geschlechtAktuell, neueNamenAb, reserviert[3];
  StammEintrag eintrag[8];
  uint32_t pruefsumme;
};
static_assert(STAMM_ADRESSE_V2 + sizeof(StammStandV2) <= EEPROM_GROESSE - 1, "Stammbaum Version 2 überschneidet den Merker");
StammStand stamm;
bool stammGeaendert = false;
int stammScroll = 0;               // Verschiebung der Ansicht in Pixeln (0 = ganz oben, älteste Familie)
int stammZiehY = -1;               // letzte Fingerhöhe beim Ziehen, -1 = kein Finger

// Alle Angaben zu einer Familie (steht hier, weil die Arduino IDE Typen vor den Funktionsköpfen braucht): das Kind und seine Eltern
struct StammAnsicht {
  char name[24];
  int wesen, eltern, alterStd, geschlecht, generation;
  uint8_t elternName;
  Phase phase;
  bool lebt;
  const StammEintrag* davor;   // das Tamagotchi davor (Elternteil), sonst nullptr
};
