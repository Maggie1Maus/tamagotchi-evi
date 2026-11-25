// Gemeinsamer Bodenanker, perspektivische Skalierung und Verdeckung für Zimmer/Garten.
#pragma once
#include "assets.h"
struct Raumpunkt { int x, y; };
const int RAUM_HEIM_Y = 194;
struct FigurLage {
  const Bild* bild = nullptr;
  int x = 0, y = 0, w = 0, h = 0, fuss = RAUM_HEIM_Y;
  bool spiegeln = false, garten = false;
} figurLage;
int eviFuss = RAUM_HEIM_Y;
bool raumImBett = false;   // Evi liegt im Tierbett: dann in der alten Größe (bett.ino, raumtiefe.ino)
const int ZF_X = 79, ZF_Y = 46, ZF_W = 66, ZF_H = 53;
