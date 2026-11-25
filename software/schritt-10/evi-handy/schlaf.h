// Schlafmodus (Wunsch der Nutzerin vom 25.11.2026): Vier neue AAA waren nach weniger als einem Tag leer,
// obwohl der Bildschirm meist aus war. „Bildschirm aus“ hat bis dahin nur das Licht abgeschaltet; der Chip
// lief ohne Pause mit vollem Takt weiter. Jetzt spart Evi an vier Stellen:
//  1. Der Chip schläft zwischen zwei Abfragen (delay() wartet beim RP2350 schlafend, nicht rechnend).
//     Dunkel: bis zur nächsten Messung des Lagesensors (alle 20 ms, damit Gassi weiter Schritte zählt).
//     Hell: kurze Pausen von 2 ms, außer in Spielen und solange ein Finger auf dem Bildschirm liegt.
//  2. Dunkel läuft der Chip langsamer (48 statt 150 MHz). Bildschirm an oder ein Ton: sofort wieder voll.
//  3. Der Bildschirm-Chip schläft, solange der Bildschirm aus ist (bildschirm.h: lcdSchlafen/lcdAufwachen).
//  4. Der Ton-Chip bekommt nur Takt, während ein Ton spielt (ton.h).
// Jede Stelle lässt sich hier einzeln abschalten (false), falls am Board etwas nicht stimmt.
// Im Seriellen Monitor zeigt „P“, wie viel der Chip seit der letzten Abfrage geschlafen hat.
#pragma once
#ifndef NACHBILDUNG
#include <hardware/clocks.h>
#endif

const bool SCHLAF_CHIP = true;           // 1. zwischen den Abfragen schlafen
const bool SCHLAF_TAKT = true;           // 2. dunkel langsamer laufen
const uint32_t SCHLAF_HELL_MS = 2;       // Pause je Runde bei hellem Bildschirm
const uint32_t SCHLAF_DUNKEL_MS = 20;    // längste Pause je Runde bei dunklem Bildschirm (wie der Lagesensor)
const uint32_t TAKT_DUNKEL_KHZ = 48000;  // Takt bei dunklem Bildschirm
const uint32_t TAKT_SENKEN_NACH = 3000;  // so lange (ms) muss es dunkel und still sein, bevor der Takt sinkt

// Zum Nachsehen über „P“ und für die Prüfung am PC
uint32_t schlafMs = 0, schlafSeit = 0;   // geschlafen seit der letzten Abfrage, und seit wann gezählt wird
bool taktGesenkt = false;
static uint32_t taktVollKhz = 0;

// Wartet schlafend. Am Board: delay() ruht beim RP2350 (sleep_ms, der Kern wartet mit WFE auf den Zeitgeber).
// In der Nachbildung am PC wird nur gezählt; die Zeit stellen dort die Prüfprogramme weiter.
void schlafen(uint32_t ms) {
  schlafMs += ms;
#ifndef NACHBILDUNG
  delay(ms);
#endif
}

// Chip auf den langsamen Takt (nur aufrufen, wenn der Bildschirm aus ist und kein Ton spielt)
void taktSenken() {
  if (!SCHLAF_TAKT || taktGesenkt) return;
#ifndef NACHBILDUNG
  if (taktVollKhz == 0) taktVollKhz = clock_get_hz(clk_sys) / 1000;   // was beim Start eingestellt war (150 MHz)
  if (!set_sys_clock_khz(TAKT_DUNKEL_KHZ, false)) return;
#endif
  taktGesenkt = true;
}

// Wieder voller Takt (vor dem Zeichnen und vor jedem Ton)
void taktVoll() {
  if (!taktGesenkt) return;
#ifndef NACHBILDUNG
  if (taktVollKhz) set_sys_clock_khz(taktVollKhz, false);
#endif
  taktGesenkt = false;
}

// Takt in MHz, für „P“
uint32_t taktMhz() {
#ifndef NACHBILDUNG
  return clock_get_hz(clk_sys) / 1000000;
#else
  return taktGesenkt ? TAKT_DUNKEL_KHZ / 1000 : 150;
#endif
}
