// Ton über den I2S-Wandler des Boards. Melodien laufen nebenbei:
// tonWeiter() füllt bei jedem Durchlauf nur so viel nach, wie gerade Platz ist,
// damit Bild und Touch nicht warten müssen.
#pragma once
#include <I2S.h>
#include "board.h"

const int ABTASTRATE = 22050;
const int LAUTSTAERKE = 700;   // von 32767; 2000 war laut Nutzerin recht laut

// Ein Ton: Höhe in Hertz (0 = Pause) und Dauer in Millisekunden.
struct Note {
  uint16_t freq;
  uint16_t ms;
};

// Dieselben Melodien wie auf dem Cube (Schritt 7)
const Note MELODIE_HALLO[] = {{523, 90}, {0, 30}, {659, 90}, {0, 30}, {784, 90}, {0, 30}, {1047, 180}};
const Note MELODIE_TIPP[] = {{2093, 12}};
const Note MELODIE_SPIELEN[] = {{1047, 70}, {0, 30}, {1319, 70}, {0, 30}, {1568, 70}, {0, 30}, {2093, 140}};
const Note MELODIE_PUTZEN[] = {{1568, 50}, {0, 40}, {1568, 50}, {0, 40}, {1568, 50}, {0, 60}, {2093, 120}};
const Note MELODIE_NEU[] = {{784, 120}, {0, 60}, {784, 120}, {0, 60}, {1047, 260}};
const Note MELODIE_FUETTERN[] = {{784, 80}, {0, 40}, {988, 80}, {0, 40}, {1319, 140}};
const Note MELODIE_ACHTUNG[] = {{880, 120}, {0, 120}, {880, 120}, {0, 120}, {659, 260}};
const Note MELODIE_AUFWACHEN[] = {{659, 80}, {0, 30}, {880, 120}};
const Note MELODIE_GUTE_NACHT[] = {{880, 100}, {0, 30}, {659, 160}};

#define MELODIE(name) name, int(sizeof(name) / sizeof(Note))

I2S i2s(OUTPUT);

static const Note* melodie = nullptr;
static int melodieLaenge = 0;
static int notenNummer = 0;
static long probenUebrig = 0;   // in der aktuellen Note
static long halbePeriode = 0;   // 0 = Pause
static long probe = 0;

void tonStarten() {
  i2s.setBCLK(I2S_BCK);   // LRCK = BCK + 1
  i2s.setDATA(I2S_DIN);
  i2s.setBitsPerSample(16);
  i2s.setBuffers(6, 256);  // etwa 70 ms Vorrat, damit das Bildzeichnen keine Lücken reißt
  i2s.begin(ABTASTRATE);
}

void spieleMelodie(const Note* noten, int laenge) {
  melodie = noten;
  melodieLaenge = laenge;
  notenNummer = 0;
  probenUebrig = 0;
}

static bool naechsteNote() {
  if (melodie == nullptr || notenNummer >= melodieLaenge) {
    melodie = nullptr;
    return false;
  }
  const Note n = melodie[notenNummer++];
  probenUebrig = (long)ABTASTRATE * n.ms / 1000;
  halbePeriode = n.freq > 0 ? ABTASTRATE / (2L * n.freq) : 0;
  probe = 0;
  return true;
}

// Bei jedem loop()-Durchlauf aufrufen. Ohne Melodie gibt der Wandler Stille aus.
void tonWeiter() {
  while (melodie != nullptr && i2s.availableForWrite() >= 4) {
    if (probenUebrig <= 0 && !naechsteNote()) return;
    int16_t wert = 0;
    if (halbePeriode > 0) wert = ((probe / halbePeriode) % 2) ? LAUTSTAERKE : -LAUTSTAERKE;
    i2s.write16(wert, wert);
    probe++;
    probenUebrig--;
  }
}
