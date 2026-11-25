// Gemeinsames Entdecken: Befehle statt direkter Steuerung. Zustand nur für den aktuellen Besuch.
#pragma once
enum GartenPhase : uint8_t { GA_ZU, GA_WAHL, GA_HIN, GA_SCHNUPPERN, GA_WARTEN, GA_UNTERSUCHEN, GA_ZEIGEN, GA_LEER, GA_HERAUS, GA_ZURUECK };
struct GartenZustand {
  GartenPhase phase = GA_ZU;
  uint32_t seit = 0;
  uint8_t stelle = 0;
  int x = 160, vonX = 160, y = 194, vonY = 194;
  int fund = -1;
  bool schon = false, voll = false;
} garten;
const int GARTEN_X[3] = {65, 250, 205};
const int GARTEN_Y[3] = {157,168,139};
const char* const GARTEN_STELLE[3] = {"Am Baum", "Beim Busch", "Am Blumenbeet"};
