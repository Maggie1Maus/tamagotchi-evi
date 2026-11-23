// Ton über den I2S-Wandler des Boards und den beiliegenden Lautsprecher.
// Leise eingestellt: Der Original-Lautsprecher im Retro Gamer verträgt nur 0,25 W.
#pragma once
#include <I2S.h>
#include "board.h"

const int ABTASTRATE = 22050;
const int LAUTSTAERKE = 2000;   // von 32767; höher = lauter

I2S i2s(OUTPUT);

void tonStarten() {
  i2s.setBCLK(I2S_BCK);   // LRCK = BCK + 1
  i2s.setDATA(I2S_DIN);
  i2s.setBitsPerSample(16);
  i2s.begin(ABTASTRATE);
}

// Spielt einen Ton (Rechteck) mit Frequenz in Hz für dauer Millisekunden.
// frequenz 0 = Pause.
void ton(int frequenz, int dauer) {
  const long proben = (long)ABTASTRATE * dauer / 1000;
  const long halbePeriode = frequenz > 0 ? ABTASTRATE / (2L * frequenz) : 0;
  for (long i = 0; i < proben; i++) {
    int16_t wert = 0;
    if (halbePeriode > 0) wert = ((i / halbePeriode) % 2) ? LAUTSTAERKE : -LAUTSTAERKE;
    i2s.write(wert);   // links
    i2s.write(wert);   // rechts
  }
}

void melodieHallo() {
  ton(523, 120); ton(659, 120); ton(784, 120); ton(1047, 250); ton(0, 50);
}

void melodiePiep() {
  ton(880, 80); ton(0, 40); ton(1175, 120); ton(0, 50);
}
