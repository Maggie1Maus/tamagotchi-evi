// Uhr vor- und wieder zurückgestellt (26.11.2026): keine Dauer-Meldung „ist jetzt ein Teenager“,
// Evi rechnet ab der zurückgestellten Zeit normal weiter. Dazu: Geburtstag wird einmal neu gefragt.
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
  const ms_t heute = ausOrtszeit(2026, 11, 26, 12, 0);
  uhrSetzen(heute);
  pet = neuesTier(heute - 3 * TAG, ERSTES_EI);
  pet.schluepft = heute - 2 * TAG - 12 * STUNDE;  // seit anderthalb Tagen Teenager, morgen früh erwachsen
  pet.aktualisiert = heute; pet.satt = pet.laune = pet.sauber = 90;
  PRUEFE(phaseBei(pet, heute) == TEEN, "Evi ist heute Teenager");
  // Uhr 18 Stunden vorstellen: Evi wird erwachsen
  uhrSetzen(heute + 18 * STUNDE);
  runde();
  PRUEFE(phaseBei(pet, jetztMs()) == ERWACHSEN, "dann erwachsen");
  // Uhr zurück auf heute
  uhrSetzen(heute);
  meldung[0] = 0;
  int teenMeldungen = 0;
  for (int i = 0; i < 30; i++) {
    runde();
    if (strstr(meldung, "Teenager")) teenMeldungen++;
    meldung[0] = 0;
    simMillis += 10000;
  }
  PRUEFE(teenMeldungen == 0, "keine Meldung „ist jetzt ein Teenager“ nach dem Zurückstellen");
  PRUEFE(pet.aktualisiert <= jetztMs() && pet.aktualisiert >= heute, "Stand wieder bei der echten Zeit");
  pet.satt = 90;   // gefüttert
  const float satt = pet.satt;
  uhrSetzen(heute + 3 * STUNDE);
  runde();
  PRUEFE(pet.satt < satt, "Evi rechnet danach normal weiter (Hunger nimmt ab)");
  PRUEFE(phaseBei(pet, jetztMs()) == TEEN, "weiter Teenager");

  // Geburtstag: falsch gespeichert, Merker noch nicht gesetzt → einmal vergessen und neu fragen
  freundLaden();
  freund.gebTag = 3; freund.gebMonat = 4; freund.gebGefeiert = 2026; freund.punkte = 20;
  freundSpeichern();
  EEPROM.write(GEB_NEU_ADRESSE, 0);
  freundLaden();
  PRUEFE(freund.gebTag == 0 && freund.gebMonat == 0, "falscher Geburtstag vergessen");
  PRUEFE(freund.gebGefeiert == 0, "Feier-Jahr zurückgesetzt");
  PRUEFE(freund.punkte == 20, "Freundschaft bleibt");
  freundSpeichern();
  // Neu eingeben und neu starten: bleibt
  freund.gebTag = 9; freund.gebMonat = 7;
  freundSpeichern();
  freundLaden();
  PRUEFE(freund.gebTag == 9 && freund.gebMonat == 7, "neuer Geburtstag bleibt nach Neustart");
  printf(fehler ? "%d FEHLER\n" : "Alles in Ordnung.\n", fehler);
  return fehler ? 1 : 0;
}
