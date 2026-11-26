// Stammbaum (26.11.2026, nur Waveshare). Logik in stammbaum.ino.
//  - Wenn eine neue Evi schlüpft, kommt die verstorbene Evi in den Stammbaum: Name, Wesen, Alter und ihr Bild
//    in der Lebensphase, in der sie gegangen ist (meist als Oma).
//  - Mutter ist die Evi davor. Vater ist die männliche Parkfreund-Figur, die die Mutter am häufigsten getroffen hat
//    (ohne Treffen: unbekannt).
//  - Gespeichert werden die letzten 8 Evis; die aktuelle Evi steht immer am Ende.
#pragma once

struct StammEintrag {
  char name[12];                 // gekürzt, passt in die Spalte
  uint8_t wesen, vater, phase, generation;   // vater: Nummer der Figur (BESUCH_NAME), 255 = unbekannt; phase: bei ihrem Tod
  uint16_t alterStd;             // so viele Stunden hat sie gelebt
};
const int STAMM_MAX = 8;
struct StammStand {
  uint32_t kennung;
  uint16_t version, groesse;
  uint8_t anzahl;                // wie viele verstorbene Evis gespeichert sind
  uint8_t vaterAktuell;          // Vater der jetzigen Evi (255 = unbekannt)
  uint8_t reserviert[2];
  StammEintrag eintrag[STAMM_MAX];   // älteste zuerst
  uint32_t pruefsumme;
};
const uint32_t STAMM_KENNUNG = 0x53544D4D;   // "STMM"
const uint16_t STAMM_VERSION = 1;
const int STAMM_ADRESSE = 3904;              // hinter den Parkfreunden (3856 + 48), vor dem Geburtstags-Merker (4095)
static_assert(PARKFREUND_ADRESSE + sizeof(ParkFreundStand) <= STAMM_ADRESSE, "Stammbaum überschneidet die Parkfreunde");
static_assert(STAMM_ADRESSE + sizeof(StammStand) <= EEPROM_GROESSE - 1, "Stammbaum überschneidet den Merker am Ende");
StammStand stamm;
bool stammGeaendert = false;
uint8_t stammSeite = 0;            // angezeigte Evi: 0 = älteste gespeicherte, stamm.anzahl = jetzige
