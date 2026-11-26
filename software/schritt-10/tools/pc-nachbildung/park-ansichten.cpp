// Alle 51 zulässigen Kombinationen mit dem echten Firmware-Zeichencode.
#define main einrichtungPruefungMain
#include "einrichtung.cpp"
#undef main

int main() {
  bildschirmStarten();
  bild.cp437(true);
  bereit(ausOrtszeit(2026, 11, 26, 12, 0));
  int kombinationen = 0;
  bool fensterPasst = true, raenderPassen = true, grauPasst = true, nachtSchwarz = true;
  for (int s = 0; s < 4; s++) for (int h = 0; h < 3; h++) for (int w = 0; w < 5; w++) {
    if (w == WT_SCHNEE && s != JZ_WINTER) continue;
    bild.fillScreen(1);
    const uint8_t randOben = bild.getBuffer()[0];
    const uint8_t randUnten = bild.getBuffer()[ZEILE_Y * BREITE];
    parkHintergrund((Jahreszeit)s, (Himmel)h, (Wetter)w);
    parkBeleuchtung((Jahreszeit)s, (Himmel)h, (Wetter)w, s == JZ_WINTER);
    if (h == HIMMEL_NACHT) nachtSchwarz &= bild.stufe(200, STATUS_H + 5) == 0;
    const bool erwartetLicht = h == HIMMEL_DAEMMERUNG || h == HIMMEL_NACHT || w == WT_REGEN;
    for (const ParkPunkt& p : {ParkPunkt{134, 73}, ParkPunkt{173, 74}, ParkPunkt{183, 70}}) {
      fensterPasst &= erwartetLicht ? bild.stufe(p.x, STATUS_H + p.y) == 15 :
                                      bild.stufe(p.x, STATUS_H + p.y) < 15;
      if (erwartetLicht)
        fensterPasst &= bild.stufe(p.x, STATUS_H + p.y) > bild.stufe(p.x + 3, STATUS_H + p.y);
    }
    parkNiederschlag((Wetter)w, (Himmel)h, 300);
    char name[64]; snprintf(name, sizeof(name), "park-%d-%d-%d", s, h, w);
    fotoGrau(name, 0, STATUS_H, BREITE, ZEILE_Y - STATUS_H);
    for (int y = 0; y < HOEHE; y++) for (int x = 0; x < BREITE; x++) {
      const uint8_t pixel = bild.getBuffer()[y * BREITE + x];
      if (y < STATUS_H) raenderPassen &= pixel == randOben;
      else if (y >= ZEILE_Y) raenderPassen &= pixel == randUnten;
      else grauPasst &= pixel % 17 == 0;
    }
    kombinationen++;
  }
  PRUEFE(kombinationen == 51, "Alle 51 Kombinationen gezeichnet, Schneefall nur im Winter");
  PRUEFE(fensterPasst, "Alle drei Hausfenster leuchten nachts, bei Dämmerung und Regen in allen Jahreszeiten");
  PRUEFE(raenderPassen, "Park, Beleuchtung und Niederschlag bleiben innerhalb der Szene");
  PRUEFE(grauPasst, "Alle Ansichten verwenden ausschließlich die 16 Displaygraustufen");
  PRUEFE(nachtSchwarz, "Der freie Nachthimmel ist bei jedem Wetter schwarz");
  PRUEFE(!parkIstHimmel(PARK_HIMMEL[JZ_SOMMER], 100, 5),
         "Die helle Sommerkrone am oberen Bildrand bleibt Landschaft");
  parkHintergrund(JZ_SOMMER, HIMMEL_NACHT, WT_SONNE);
  const int wegVorher = bild.getBuffer()[(STATUS_H + 120) * BREITE + 68];
  parkBeleuchtung(JZ_SOMMER, HIMMEL_NACHT, WT_SONNE, false);
  const int wegNachher = bild.getBuffer()[(STATUS_H + 120) * BREITE + 68];
  PRUEFE(wegNachher >= wegVorher + 85, "Die vordere Laterne hellt den Kiesweg deutlich sichtbar auf");
  bool lampenGrau[16] = {};
  for (int y = 0; y < PARK_LATERNE.h; y++) for (int x = 0; x < PARK_LATERNE.w; x++)
    if (PARK_LATERNE.deckt(x, y)) lampenGrau[PARK_LATERNE.wert(x, y) / 17] = true;
  int grautoene = 0;
  for (bool aktiv : lampenGrau) grautoene += aktiv;
  PRUEFE(grautoene >= 8, "Die neue Laternenzeichnung enthält mindestens acht Grautöne");
  // Häuser und Laternen sind reine Kulisse, keine zusätzliche Parkbedienung.
  parkOeffnen(jetztMs());
  const ParkZiel zielVorher = park.ziel;
  parkTouch(134, STATUS_H + 73);
  PRUEFE(park.phase == PARK_WAHL && park.ziel == zielVorher, "Antippen des Hauses startet weder Lauf noch Besuchsmenü");
  printf("Parkansichten: %d Fehler\n", fehler);
  return fehler ? 1 : 0;
}
