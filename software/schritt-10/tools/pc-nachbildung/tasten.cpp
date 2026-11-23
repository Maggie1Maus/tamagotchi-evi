// Prüft tasten.h mit dem nachgebildeten MCP23017: kurzes Antippen, Festhalten, Prellen.
#include "Arduino.h"
uint32_t simMillis = 1000;
SerialC Serial;
int simDC = 1;
void nachbildungBildFertig() {}
#include "Wire.h"
TwoWire Wire1;
#include "tasten.h"
int fehler = 0;
#define PRUEFE(b, t) do { if (!(b)) { printf("FEHLER: %s\n", t); fehler++; } else printf("ok: %s\n", t); } while (0)
int ereignisse[K_ANZAHL];
// Zeit vergehen lassen; abfrage = Abstand, in dem das Programm nach Tasten schaut
void warten(uint32_t ms, uint32_t abfrage) {
  for (uint32_t t = 0; t < ms; t++) { simMillis++; if (simMillis % abfrage == 0) { int k; while ((k = naechsteTaste()) >= 0) ereignisse[k]++; } }
}
void leeren() { for (int& e : ereignisse) e = 0; }
int main() {
  PRUEFE(tastenStarten() && tastenDa, "MCP23017 gefunden und eingerichtet");
  // 1. Kurzes Antippen (10 ms), während das Programm 40 ms lang zeichnet
  leeren(); warten(100, 40);
  simTaste(K_A, true); warten(10, 1000); simTaste(K_A, false); warten(200, 40);
  PRUEFE(ereignisse[K_A] == 1, "10-ms-Tipper zwischen zwei Abfragen wird erkannt");
  // 2. A eine halbe Sekunde halten: genau ein Druck
  leeren(); simTaste(K_A, true); warten(500, 5); simTaste(K_A, false); warten(100, 5);
  PRUEFE(ereignisse[K_A] == 1, "A halten zählt einmal");
  // 3. UP eine Sekunde halten: Wiederholung nach 0,4 s, dann alle 0,12 s
  leeren(); simTaste(K_UP, true); warten(1000, 5); simTaste(K_UP, false); warten(100, 5);
  printf("   UP 1 s gehalten: %d Ereignisse\n", ereignisse[K_UP]);
  PRUEFE(ereignisse[K_UP] >= 5 && ereignisse[K_UP] <= 7, "UP halten wiederholt");
  // 4. Flackernder Kontakt beim Loslassen: ein Druck
  leeren(); simTaste(K_L, true); warten(100, 3);
  for (int i = 0; i < 4; i++) { simTaste(K_L, false); warten(2, 1); simTaste(K_L, true); warten(2, 1); }
  simTaste(K_L, false); warten(100, 3);
  PRUEFE(ereignisse[K_L] == 1, "Flackern beim Loslassen zählt nicht doppelt");
  // 5. Zwei schnelle Tipper (je 30 ms, 60 ms Abstand): zwei Drücke
  leeren();
  simTaste(K_R, true); warten(30, 3); simTaste(K_R, false); warten(60, 3);
  simTaste(K_R, true); warten(30, 3); simTaste(K_R, false); warten(100, 3);
  PRUEFE(ereignisse[K_R] == 2, "zwei schnelle Tipper zählen zweimal");
  // 6. Zwei Tasten fast gleichzeitig (3 ms nacheinander, je 25 ms), Abfrage alle 10 ms
  leeren(); warten(10, 10);
  simTaste(K_DOWN, true); warten(3, 10); simTaste(K_B, true); warten(25, 10); simTaste(K_DOWN, false); simTaste(K_B, false); warten(200, 10);
  PRUEFE(ereignisse[K_DOWN] == 1 && ereignisse[K_B] == 1, "zwei Tasten fast gleichzeitig");
  // Grenze des Chips: Zwei verschiedene Tasten innerhalb weniger ms angetippt UND losgelassen, bevor
  // das Programm nachschaut: INTCAP hält nur die erste Änderung fest, die zweite geht verloren.
  leeren(); warten(10, 40);
  simTaste(K_DOWN, true); warten(3, 1000); simTaste(K_B, true); warten(3, 1000); simTaste(K_DOWN, false); simTaste(K_B, false); warten(200, 40);
  printf("   (Grenze: zwei Tipper in 3 ms, beide vor der nächsten Abfrage los: DOWN %d, B %d)\n", ereignisse[K_DOWN], ereignisse[K_B]);
  // 7. Ohne Druck: nichts
  leeren(); warten(2000, 3);
  int summe = 0; for (int e : ereignisse) summe += e;
  PRUEFE(summe == 0, "in Ruhe keine Drücke");
  printf("%s: %d Fehler\n", fehler ? "NICHT OK" : "OK", fehler);
  return fehler;
}
