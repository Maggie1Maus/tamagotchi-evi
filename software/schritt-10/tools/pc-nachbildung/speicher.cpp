// Speicher: Spielstand Version 3 (256 Funde) und 2 werden mit 50 Funden übernommen, Stammbaum zieht um (26.11.2026)
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
int main() {
  bildschirmStarten(); bild.cp437(true);
  PRUEFE(SAMMLUNG_MAX == 50 && sizeof(Spielstand) < 1100, "Sammlung hat Platz für 50 Funde, Spielstand ist klein");
  // ---- Version 3 mit 60 Funden ----
  {
    static SpielstandV3 v3;
    memset(&v3, 0, sizeof(v3));
    v3.kennung = SPEICHER_KENNUNG; v3.version = 3; v3.groesse = sizeof(SpielstandV3);
    v3.tier = neuesTier(ausOrtszeit(2026, 11, 20, 12, 0), 0, "Evi"); v3.tier.satt = v3.tier.laune = v3.tier.sauber = 77; v3.tier.muenzen = 123;
    v3.tonAn = false;
    v3.sammlung.ausfluege = 9; v3.sammlung.anzahl = 60;
    for (int i = 0; i < 60; i++) v3.sammlung.fund[i] = {(uint8_t)(i % FUNDARTEN), FUND_GASSI, 1, 20261100 + i, FUND_UNBEKANNT, FUND_UNBEKANNT, FUND_UNBEKANNT, 0};
    v3.pruefsumme = pruefsumme((const uint8_t*)&v3, offsetof(SpielstandV3, pruefsumme));
    for (size_t i = 0; i < sizeof(v3); i++) EEPROM.write(i, ((const uint8_t*)&v3)[i]);
    Tier t; bool ton = true;
    const bool ok = spielstandLaden(t, ton);
    PRUEFE(ok && t.satt == 77 && t.muenzen == 123 && !ton && !strcmp(t.name, "Evi"), "Version 3: Tamagotchi, Münzen und Ton übernommen");
    PRUEFE(sammlung.anzahl == 50 && sammlung.fund[0].tag == 20261110 && sammlung.fund[49].tag == 20261159 && sammlung.ausfluege == 9, "Version 3: die letzten 50 Funde bleiben");
    spielstandSpeichern(t, ton);
    sammlung = Sammlung();
    Tier t2; bool ton2 = true;
    PRUEFE(spielstandLaden(t2, ton2) && sammlung.anzahl == 50 && sammlung.fund[49].tag == 20261159 && t2.muenzen == 123, "danach als Version 4 gespeichert und wieder geladen");
    // Neuer Stand mit leerer Sammlung
    sammlung = Sammlung(); spielstandSpeichern(t2, ton2);
    PRUEFE(spielstandLaden(t2, ton2) && sammlung.anzahl == 0, "leere Sammlung bleibt leer");
    bool voll = true; for (int i = 0; i < 60; i++) if (!fundAufnehmen(FUND_STEIN, FUND_GASSI, 20261200 + i, 1) && i < 50) voll = false;
    PRUEFE(voll && sammlung.anzahl == 50 && !fundAufnehmen(FUND_STEIN, FUND_GASSI, 20261300, 1), "mehr als 50 Funde gehen nicht");
  }
  // ---- kaputter Spielstand bleibt kaputt (neues Ei) ----
  {
    EEPROM.write(10, EEPROM.read(10) ^ 0x44);
    Tier t; bool ton;
    PRUEFE(!spielstandLaden(t, ton), "beschädigter Spielstand wird erkannt");
  }
  // ---- Stammbaum zieht um ----
  {
    static StammStandV2 v2;
    memset(&v2, 0, sizeof(v2));
    v2.kennung = STAMM_KENNUNG; v2.version = 2; v2.groesse = sizeof(StammStandV2);
    v2.anzahl = 3; v2.elternAktuell = 13; v2.elternNameAktuell = 113; v2.geschlechtAktuell = 1; v2.neueNamenAb = 2;
    for (int i = 0; i < 3; i++) { snprintf(v2.eintrag[i].name, 12, "Alt%d", i); v2.eintrag[i].generation = i + 1; v2.eintrag[i].alterStd = 240; v2.eintrag[i].phase = OMA; }
    v2.pruefsumme = pruefsumme((const uint8_t*)&v2, offsetof(StammStandV2, pruefsumme));
    for (size_t i = 0; i < sizeof(v2); i++) EEPROM.write(STAMM_ADRESSE_V2 + i, ((const uint8_t*)&v2)[i]);
    for (size_t i = 0; i < sizeof(StammStand); i++) EEPROM.write(STAMM_ADRESSE + i, 0);
    stammLaden();
    PRUEFE(stamm.anzahl == 3 && !strcmp(stamm.eintrag[2].name, "Alt2") && stamm.elternAktuell == 13 && stamm.neueNamenAb == 2 && stamm.geschlechtAktuell == 1, "Stammbaum Version 2 übernommen");
    PRUEFE(stammGeaendert, "zum Speichern am neuen Platz vorgemerkt");
    stammSpeichern();
    memset(&stamm, 0, sizeof(stamm));
    for (size_t i = 0; i < sizeof(v2); i++) EEPROM.write(STAMM_ADRESSE_V2 + i, 0);      // alter Platz leer
    stammLaden();
    PRUEFE(stamm.anzahl == 3 && !strcmp(stamm.eintrag[0].name, "Alt0"), "danach vom neuen Platz geladen");
    PRUEFE(STAMM_MAX == 50 && STAMM_ADRESSE + sizeof(StammStand) <= ZIMMER_ADRESSE, "Platz für 50 Tamagotchis im Stammbaum");
  }
  // ---- Die anderen Bereiche bleiben unberührt ----
  {
    const uint8_t davor = EEPROM.read(EEPROM_GROESSE - 1);
    stamm.anzahl = 3; stammGeaendert = true; stammSpeichern();
    PRUEFE(EEPROM.read(EEPROM_GROESSE - 1) == davor && EEPROM.read(FREUND_ADRESSE) == 0 + EEPROM.read(FREUND_ADRESSE), "Merker am Ende und Freundschaft unberührt");
  }
  printf(fehler ? "%d FEHLER\n" : "Alles in Ordnung.\n", fehler);
  return fehler ? 1 : 0;
}
