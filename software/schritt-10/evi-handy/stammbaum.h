// Stammbaum (26.11.2026, nur Waveshare). Logik in stammbaum.ino.
//  - Wenn eine neue Evi schlüpft, kommt die verstorbene Evi in den Stammbaum: Name, Wesen, Alter und ihr Bild
//    in der Lebensphase, in der sie gegangen ist (meist als Oma).
//  - Ein Elternteil ist das Tamagotchi davor (Mutter oder Vater, je nach seinem Geschlecht), der andere die Parkfreund-
//    Figur des anderen Geschlechts, die es am häufigsten getroffen hat (ohne Treffen: unbekannt). Der Name dieser Figur
//    wird mitgespeichert, denn die Figuren heißen bei jedem Tamagotchi anders (namen.h).
//  - Gespeichert werden die letzten 8 Tamagotchis; das aktuelle steht immer am Ende.
#pragma once

struct StammEintrag {
  char name[12];                 // gekürzt, passt in die Spalte
  uint8_t wesen, geschlecht;     // geschlecht: 0 Mädchen, 1 Junge
  uint8_t eltern, elternName;    // Parkfreund-Elternteil: Nummer der Figur (255 = unbekannt) und Namenscode (namen.ino)
  uint8_t phase, generation;     // phase: bei ihrem Tod
  uint16_t alterStd;             // so viele Stunden hat sie gelebt
};
const int STAMM_MAX = 8;
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
const uint16_t STAMM_VERSION = 2;   // 2: Geschlecht, Elternnamen, neue Namen (Version 1 wird verworfen)
const int STAMM_ADRESSE = 3904;              // hinter den Parkfreunden (3856 + 48), vor dem Geburtstags-Merker (4095)
static_assert(PARKFREUND_ADRESSE + sizeof(ParkFreundStand) <= STAMM_ADRESSE, "Stammbaum überschneidet die Parkfreunde");
static_assert(STAMM_ADRESSE + sizeof(StammStand) <= EEPROM_GROESSE - 1, "Stammbaum überschneidet den Merker am Ende");
StammStand stamm;
bool stammGeaendert = false;
uint8_t stammSeite = 0;            // angezeigte Evi: 0 = älteste gespeicherte, stamm.anzahl = jetzige
