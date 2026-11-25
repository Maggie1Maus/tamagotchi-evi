// Töne wie in der Handy-App (SOUNDS in app.js): Rechteckwellen, nebenbei abgespielt.
#pragma once
#include <I2S.h>
#include "board.h"
#include "schlaf.h"

const int ABTASTRATE = 22050;
const int LAUTSTAERKE = 700;   // von 32767; 2000 war laut Nutzerin recht laut

struct Note { uint16_t freq; uint16_t ms; };

// Aus app.js. Zwischen zwei Tönen lässt die App 30 ms Pause; das steckt hier in den Pausen.
const Note TON_SCHLUEPFEN[] = {{660, 110}, {0, 30}, {880, 110}, {0, 30}, {1320, 260}};
const Note TON_ENTWICKLUNG[] = {{523, 100}, {0, 30}, {659, 100}, {0, 30}, {784, 220}};
const Note TON_ACHTUNG[] = {{880, 90}, {0, 90}, {880, 90}};
const Note TON_TRAURIG[] = {{330, 220}, {0, 30}, {262, 360}};
const Note TON_MUENZE[] = {{1320, 60}, {0, 30}, {1760, 90}};
const Note TON_DANEBEN[] = {{196, 140}};
const Note TON_RISS[] = {{440, 40}};
const Note TON_WUNSCH[] = {{784, 90}, {0, 30}, {1047, 140}};
// Seit Version 0.10/0.11 der App: Besuch (special) und der Vogel (chirp)
const Note TON_SPEZIAL[] = {{1047, 70}, {0, 30}, {1319, 70}, {0, 30}, {1568, 70}, {0, 30}, {2093, 200}};
const Note TON_ZWITSCHERN[] = {{1976, 50}, {0, 90}, {2349, 70}, {0, 180}, {1976, 50}, {0, 90}, {2349, 70}};

#define TON(name) name, int(sizeof(name) / sizeof(Note))

I2S i2s(OUTPUT);
bool tonAn = true;

// Schlafmodus (seit 25.11.2026, schlaf.h): Der Ton-Chip bekommt nur Takt, während ein Ton spielt. Ohne Takt
// schaltet er sich selbst in Bereitschaft. Knackt es am Board beim Piepen, hier false eintragen.
const bool TON_PAUSE_AUS = true;
const uint32_t TON_NACHLAUF_MS = 2000;  // so lange läuft er nach dem letzten Ton weiter (nicht bei jedem Ton neu starten)
bool i2sLaeuft = false;
static uint32_t tonZuletzt = 0;

static const Note* melodie = nullptr;
static int melodieLaenge = 0, notenNummer = 0;
static long probenUebrig = 0, halbePeriode = 0, probe = 0;

static bool i2sAn() {
  if (i2sLaeuft) return true;
  taktVoll();                      // der Ton-Takt wird beim Start aus dem Chiptakt berechnet
  if (!i2s.begin(ABTASTRATE)) return false;
  i2sLaeuft = true;
  tonZuletzt = millis();
  return true;
}

void tonStarten() {
  i2s.setBCLK(I2S_BCK);
  i2s.setDATA(I2S_DIN);
  i2s.setBitsPerSample(16);
  i2s.setBuffers(6, 256);
  if (!TON_PAUSE_AUS) i2sAn();
}

// Spielt gerade ein Ton (auch das Ende, das noch im Puffer steht, etwa 70 ms)? Dann bleibt der volle Takt
// (sonst stimmt die Tonhöhe nicht), und der Chip schläft nur kurz, damit der Puffer nicht leerläuft.
bool tonSpielt() { return melodie != nullptr || (i2sLaeuft && millis() - tonZuletzt < 100); }

void piep(const Note* noten, int laenge) {
  if (!tonAn) return;
  taktVoll();
  if (!i2sAn()) return;
  melodie = noten;
  melodieLaenge = laenge;
  notenNummer = 0;
  probenUebrig = 0;
}

// Ein einzelner Ton, z. B. für die Felder in „Melodie merken“ (tone() in app.js)
static Note einzelNote[1];
void tonEinzeln(uint16_t freq, uint16_t ms) {
  einzelNote[0].freq = freq;
  einzelNote[0].ms = ms;
  piep(einzelNote, 1);
}

static bool naechsteNote() {
  if (melodie == nullptr || notenNummer >= melodieLaenge) { melodie = nullptr; return false; }
  const Note n = melodie[notenNummer++];
  probenUebrig = (long)ABTASTRATE * n.ms / 1000;
  halbePeriode = n.freq > 0 ? ABTASTRATE / (2L * n.freq) : 0;
  probe = 0;
  return true;
}

void tonWeiter() {
  if (melodie == nullptr) {
    // Still: den Ton-Chip nach dem Nachlauf abschalten
    if (TON_PAUSE_AUS && i2sLaeuft && millis() - tonZuletzt > TON_NACHLAUF_MS) {
      i2s.end();
      i2sLaeuft = false;
    }
    return;
  }
  tonZuletzt = millis();
  while (melodie != nullptr && i2s.availableForWrite() >= 4) {
    if (probenUebrig <= 0 && !naechsteNote()) return;
    int16_t wert = 0;
    if (halbePeriode > 0) wert = ((probe / halbePeriode) % 2) ? LAUTSTAERKE : -LAUTSTAERKE;
    i2s.write16(wert, wert);
    probe++;
    probenUebrig--;
  }
}
