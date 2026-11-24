// Lagesensor QMI8658 auf dem Board (I2C 0x6B oder 0x6A, an Wire1 wie Touch, Uhr und Tasten).
// Wunsch der Nutzerin vom 24.11.2026: Evi spürt Bewegung – Gassi gehen (Schritte), schütteln,
// kippen und aufs Gesicht legen. Hier stehen der Sensor und die Erkennung; was Evi dann tut,
// steht in bewegung.ino.
//
// Gemessen wird die Kraft, die das Gerät hält: liegt es still mit dem Bildschirm nach oben, zeigt der
// Sensor etwa +1 g in Richtung aus dem Bildschirm heraus. Werte hier immer in g.
//
// Achsen: n zeigt aus dem Bildschirm heraus, u zur Oberkante des Bildes, r = u × n zur rechten Kante.
// Wie der Chip auf der Platine liegt, muss niemand wissen: Evi lernt es beim Benutzen. Wer eine Taste
// drückt oder tippt, schaut auf den Bildschirm, hält das Gerät also mit dem Bildschirm nach oben und
// der Oberkante oben. Nach drei solchen Momenten (Gerät dabei ruhig) kennt Evi n; u kennt sie, sobald
// das Gerät dabei schräg gehalten wird (nicht flach auf dem Tisch). Kippen und Aufs-Gesicht-Legen
// wirken erst danach. Schritte und Schütteln brauchen keine Achsen.
#pragma once
#include <Wire.h>
#include <math.h>

// ---------- Erkennung (Zahlen) ----------
namespace L {
// Ein Schritt: Die geglättete Kraft steigt um 0,12 g über ihren Mittelwert, nachdem sie unter -0,05 g war.
// Schritte zählen nur im Takt: 6 hintereinander, jeder 0,25 bis 1,5 s nach dem vorigen.
const float SCHRITT_HOCH = 0.12f, SCHRITT_TIEF = -0.05f;
const uint32_t SCHRITT_MIN_MS = 250, SCHRITT_MAX_MS = 1500;
const int SCHRITT_BESTAETIGT = 6;
// Ein Spaziergang endet nach 60 s ohne Schritt. Evi trabt mit, solange vor höchstens 2,5 s ein Schritt kam.
const uint32_t GASSI_ENDE_MS = 60000, GASSI_ZEIGEN_MS = 2500;
// Schütteln: mindestens 4 Stöße über 1,8 g innerhalb von 1,2 s; danach 3 s Pause.
const float SCHUETTELN_G = 1.8f;
const int SCHUETTELN_STOESSE = 4;
const uint32_t SCHUETTELN_FENSTER_MS = 1200, SCHUETTELN_PAUSE_MS = 3000;
// Kippen zur Seite: etwa 20° für 0,3 s; erneut, nachdem das Gerät 1 s gerade war (unter 9°).
const float KIPPEN_START = 0.35f, KIPPEN_GERADE = 0.15f;
const uint32_t KIPPEN_START_MS = 300, KIPPEN_GERADE_MS = 1000;
// Aufs Gesicht: 1,5 s mit dem Bildschirm nach unten; wieder umgedreht nach 0,4 s.
const float GESICHT_UNTEN = -0.8f, GESICHT_OBEN = -0.5f;
const uint32_t GESICHT_UNTEN_MS = 1500, GESICHT_OBEN_MS = 400;
// Lernen der Achsen bei Tastendruck, nur wenn das Gerät ruhig ist (0,85 bis 1,15 g)
const float LERN_RUHIG_MIN = 0.85f, LERN_RUHIG_MAX = 1.15f, LERN_ANTEIL = 0.25f, LERN_OBEN_MIN = 0.35f, LERN_NORMAL_MIN = 0.3f;
const int LERN_PROBEN = 3;
const uint32_t ABTAST_MS = 20;   // 50 Messungen pro Sekunde
}

// Was die Erkennung meldet (mehrere zugleich möglich, als Bits)
enum LageEreignis : uint8_t { LE_SCHRITTE = 1, LE_GASSI_ENDE = 2, LE_SCHUETTELN = 4, LE_KIPPEN = 8, LE_GESICHT_UNTEN = 16, LE_GESICHT_OBEN = 32 };

struct Lage {
  bool da = false;               // Sensor gefunden?
  uint8_t adresse = 0;
  bool grossEndian = false;      // Reihenfolge der Bytes (beim Start erkannt)
  uint32_t letzteMessung = 0;
  float x = 0, y = 0, z = 1;     // letzte Messung in g
  // Erkennung
  bool glattDa = false;
  float glatt[3] = {0, 0, 1};    // langsam geglättet (für Lage und Kippen)
  float schnell = 1, langsam = 1;
  bool scharf = false;
  uint32_t letzterSchritt = 0;
  bool schrittDa = false;
  int serie = 0;
  bool gassi = false;
  uint32_t gassiSchritte = 0, gassiEnde = 0;   // gassiEnde: Schritte des eben beendeten Spaziergangs
  uint32_t letzterGezaehlt = 0;
  uint32_t neu = 0;              // Schritte seit dem letzten Abholen (bewegung.ino)
  bool ueber = false;
  uint32_t stoesse[8];
  int stossAnzahl = 0;
  uint32_t letztesSchuetteln = 0;
  bool geschuettelt = false;
  bool kippBereit = true;
  bool kippSeitDa = false, geradeSeitDa = false;
  uint32_t kippSeit = 0, geradeSeit = 0;
  int kippSeite = 0;
  bool unten = false;
  bool untenSeitDa = false, obenSeitDa = false;
  uint32_t untenSeit = 0, obenSeit = 0, untenUm = 0, untenDauer = 0;
  // Gelernte Achsen: Index 0–2 (x, y, z) und Vorzeichen; 0 = noch unbekannt
  float mittel[3] = {0, 0, 0};
  int proben = 0;
  int8_t nAchse = -1, nVz = 0, uAchse = -1, uVz = 0;
  uint8_t ereignisse = 0;
};
Lage lage;

// ---------- Sensor ----------
static bool qmiSchreiben(uint8_t reg, uint8_t wert) {
  Wire1.beginTransmission(lage.adresse);
  Wire1.write(reg);
  Wire1.write(wert);
  return Wire1.endTransmission() == 0;
}

static bool qmiLesen(uint8_t reg, uint8_t* ziel, int anzahl) {
  Wire1.beginTransmission(lage.adresse);
  Wire1.write(reg);
  if (Wire1.endTransmission(false) != 0) return false;
  if ((int)Wire1.requestFrom((int)lage.adresse, anzahl, (int)true) != anzahl) return false;
  for (int i = 0; i < anzahl; i++) ziel[i] = (uint8_t)Wire1.read();
  return true;
}

// Rohwerte: 6 Bytes ab AX_L (0x35). ±4 g = 8192 je g.
static bool qmiRoh(uint8_t r[6]) { return qmiLesen(0x35, r, 6); }

static void rohInG(const uint8_t r[6], bool gross, float& x, float& y, float& z) {
  int16_t v[3];
  for (int i = 0; i < 3; i++) v[i] = gross ? (int16_t)((r[2 * i] << 8) | r[2 * i + 1]) : (int16_t)((r[2 * i + 1] << 8) | r[2 * i]);
  x = v[0] / 8192.0f; y = v[1] / 8192.0f; z = v[2] / 8192.0f;
}

// Sucht den Sensor (WHO_AM_I = 0x05) und stellt ihn ein: nur Beschleunigung, ±4 g, etwa 62,5 Messungen pro Sekunde.
// touchStarten() muss vorher laufen (Wire1).
bool lageStarten() {
  lage.da = false;
  const uint8_t adressen[2] = {0x6B, 0x6A};
  for (uint8_t a : adressen) {
    lage.adresse = a;
    uint8_t wer = 0;
    if (qmiLesen(0x00, &wer, 1) && wer == 0x05) { lage.da = true; break; }
  }
  if (!lage.da) return false;
  qmiSchreiben(0x60, 0xB0);   // Neustart des Sensors
  delay(20);
  qmiSchreiben(0x02, 0x40);   // CTRL1: Adressen zählen beim Lesen weiter
  qmiSchreiben(0x03, 0x17);   // CTRL2: ±4 g, 62,5 Hz
  qmiSchreiben(0x08, 0x01);   // CTRL7: nur Beschleunigung an (spart Strom)
  delay(40);
  // Reihenfolge der Bytes: Im Ruhezustand misst der Sensor etwa 1 g. Welche Lesart passt besser?
  float fehlerKlein = 0, fehlerGross = 0;
  for (int i = 0; i < 6; i++) {
    uint8_t r[6];
    if (!qmiRoh(r)) continue;
    float x, y, z;
    rohInG(r, false, x, y, z); fehlerKlein += fabsf(sqrtf(x * x + y * y + z * z) - 1);
    rohInG(r, true, x, y, z);  fehlerGross += fabsf(sqrtf(x * x + y * y + z * z) - 1);
    delay(18);
  }
  lage.grossEndian = fehlerGross < fehlerKlein;
  return true;
}

// ---------- Erkennung ----------
static inline float lageLaenge(const float v[3]) { return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }

static bool lageGehtGerade(uint32_t t) { return lage.gassi && t - lage.letzterGezaehlt < L::GASSI_ZEIGEN_MS; }

// Wie weit der Bildschirm nach oben zeigt: +1 nach oben, 0 senkrecht, -1 nach unten. false = unbekannt.
bool lageGesicht(float& wert) {
  if (lage.nAchse < 0 || !lage.glattDa) return false;
  const float g = lageLaenge(lage.glatt);
  if (g < 0.3f) return false;
  wert = lage.glatt[lage.nAchse] * lage.nVz / g;
  return true;
}

// Neigung zur Seite: positiv = rechte Kante tiefer, etwa der Sinus des Winkels; 0 = unbekannt.
float lageKippen() {
  if (lage.nAchse < 0 || lage.uAchse < 0 || !lage.glattDa) return 0;
  const float g = lageLaenge(lage.glatt);
  if (g < 0.3f) return 0;
  float n[3] = {0, 0, 0}, u[3] = {0, 0, 0};
  n[lage.nAchse] = lage.nVz;
  u[lage.uAchse] = lage.uVz;
  const float r[3] = {u[1] * n[2] - u[2] * n[1], u[2] * n[0] - u[0] * n[2], u[0] * n[1] - u[1] * n[0]};
  return -(lage.glatt[0] * r[0] + lage.glatt[1] * r[1] + lage.glatt[2] * r[2]) / g;
}

static void lageSchritt(uint32_t t) {
  const uint32_t abstand = t - lage.letzterSchritt;
  const bool erster = !lage.schrittDa;
  lage.letzterSchritt = t;
  lage.schrittDa = true;
  lage.serie = (!erster && abstand <= L::SCHRITT_MAX_MS) ? lage.serie + 1 : 1;
  if (lage.geschuettelt && t - lage.letztesSchuetteln < 2000) { lage.serie = 0; return; }   // Schütteln ist kein Gehen
  if (lage.serie < L::SCHRITT_BESTAETIGT) return;
  const uint32_t dazu = lage.serie == L::SCHRITT_BESTAETIGT ? L::SCHRITT_BESTAETIGT : 1;
  lage.gassiSchritte += dazu;
  lage.neu += dazu;
  lage.gassi = true;
  lage.letzterGezaehlt = t;
  lage.ereignisse |= LE_SCHRITTE;
}

// Eine Messung verarbeiten (auch von der Nachbildung am PC aufgerufen)
void lageVerarbeiten(float mx, float my, float mz, uint32_t t) {
  lage.x = mx; lage.y = my; lage.z = mz;
  const float v[3] = {mx, my, mz};
  const float g = lageLaenge(v);
  if (!lage.glattDa) { for (int i = 0; i < 3; i++) lage.glatt[i] = v[i]; lage.glattDa = true; }
  else for (int i = 0; i < 3; i++) lage.glatt[i] += (v[i] - lage.glatt[i]) * 0.15f;
  // Schritte
  lage.schnell += (g - lage.schnell) * 0.3f;
  lage.langsam += (g - lage.langsam) * 0.05f;
  const float d = lage.schnell - lage.langsam;
  if (d < L::SCHRITT_TIEF) lage.scharf = true;
  if (lage.scharf && d > L::SCHRITT_HOCH && (!lage.schrittDa || t - lage.letzterSchritt >= L::SCHRITT_MIN_MS)) {
    lage.scharf = false;
    lageSchritt(t);
  }
  // Schütteln
  if (!lage.ueber && g > L::SCHUETTELN_G) {
    lage.ueber = true;
    if (lage.stossAnzahl < 8) lage.stoesse[lage.stossAnzahl++] = t;
  } else if (lage.ueber && g < L::SCHUETTELN_G - 0.4f) {
    lage.ueber = false;
  }
  int k = 0;
  for (int i = 0; i < lage.stossAnzahl; i++) if (t - lage.stoesse[i] <= L::SCHUETTELN_FENSTER_MS) lage.stoesse[k++] = lage.stoesse[i];
  lage.stossAnzahl = k;
  if (lage.stossAnzahl >= L::SCHUETTELN_STOESSE && (!lage.geschuettelt || t - lage.letztesSchuetteln > L::SCHUETTELN_PAUSE_MS) &&
      !lageGehtGerade(t)) {
    lage.geschuettelt = true;
    lage.letztesSchuetteln = t;
    lage.stossAnzahl = 0;
    lage.serie = 0;
    lage.ereignisse |= LE_SCHUETTELN;
  }
  // Aufs Gesicht und wieder umgedreht
  float fz;
  if (lageGesicht(fz)) {
    if (!lage.unten) {
      if (fz < L::GESICHT_UNTEN) {
        if (!lage.untenSeitDa) { lage.untenSeitDa = true; lage.untenSeit = t; }
        if (t - lage.untenSeit >= L::GESICHT_UNTEN_MS) {
          lage.unten = true; lage.untenUm = t; lage.obenSeitDa = false;
          lage.ereignisse |= LE_GESICHT_UNTEN;
        }
      } else lage.untenSeitDa = false;
    } else if (fz > L::GESICHT_OBEN) {
      if (!lage.obenSeitDa) { lage.obenSeitDa = true; lage.obenSeit = t; }
      if (t - lage.obenSeit >= L::GESICHT_OBEN_MS) {
        lage.unten = false; lage.untenSeitDa = false;
        lage.untenDauer = t - lage.untenUm;
        lage.ereignisse |= LE_GESICHT_OBEN;
      }
    } else lage.obenSeitDa = false;
  }
  // Kippen zur Seite
  const float seite = lageKippen();
  if (lage.kippBereit) {
    if (fabsf(seite) > L::KIPPEN_START && !lage.unten && !lageGehtGerade(t)) {
      if (!lage.kippSeitDa) { lage.kippSeitDa = true; lage.kippSeit = t; }
      if (t - lage.kippSeit >= L::KIPPEN_START_MS) {
        lage.kippBereit = false;
        lage.geradeSeitDa = false;
        lage.kippSeite = seite > 0 ? 1 : -1;
        lage.ereignisse |= LE_KIPPEN;
      }
    } else lage.kippSeitDa = false;
  } else if (fabsf(seite) < L::KIPPEN_GERADE) {
    if (!lage.geradeSeitDa) { lage.geradeSeitDa = true; lage.geradeSeit = t; }
    if (t - lage.geradeSeit >= L::KIPPEN_GERADE_MS) { lage.kippBereit = true; lage.kippSeitDa = false; }
  } else lage.geradeSeitDa = false;
  // Spaziergang zu Ende?
  if (lage.gassi && t - lage.letzterGezaehlt > L::GASSI_ENDE_MS) {
    lage.gassiEnde = lage.gassiSchritte;
    lage.gassi = false;
    lage.gassiSchritte = 0;
    lage.ereignisse |= LE_GASSI_ENDE;
  }
}

// Bei jedem Tastendruck und Tipper: Das Gerät wird mit dem Bildschirm nach oben gehalten. Lernt die Achsen.
void lageBeimBedienen() {
  if (!lage.glattDa) return;
  const float g = lageLaenge(lage.glatt);
  if (g < L::LERN_RUHIG_MIN || g > L::LERN_RUHIG_MAX) return;
  for (int i = 0; i < 3; i++) {
    const float v = lage.glatt[i] / g;
    lage.mittel[i] = lage.proben == 0 ? v : lage.mittel[i] + (v - lage.mittel[i]) * L::LERN_ANTEIL;
  }
  lage.proben++;
  if (lage.proben < L::LERN_PROBEN) return;
  const float ax = lage.mittel[0], ay = lage.mittel[1], az = lage.mittel[2];
  if (fabsf(az) >= L::LERN_NORMAL_MIN) { lage.nAchse = 2; lage.nVz = az > 0 ? 1 : -1; }
  const float gross = fmaxf(fabsf(ax), fabsf(ay)), klein = fminf(fabsf(ax), fabsf(ay));
  if (gross >= L::LERN_OBEN_MIN && gross >= 1.5f * klein) {
    if (fabsf(ax) > fabsf(ay)) { lage.uAchse = 0; lage.uVz = ax > 0 ? 1 : -1; }
    else { lage.uAchse = 1; lage.uVz = ay > 0 ? 1 : -1; }
  }
}

// Oft aufrufen: misst alle 20 ms. Gibt die neuen Ereignisse zurück (Bits) und vergisst sie.
uint8_t lageLesen() {
  if (!lage.da) return 0;
  const uint32_t m = millis();
  if (m - lage.letzteMessung < L::ABTAST_MS) return 0;
  lage.letzteMessung = m;
  uint8_t r[6];
  if (qmiRoh(r)) {
    float x, y, z;
    rohInG(r, lage.grossEndian, x, y, z);
    lageVerarbeiten(x, y, z, m);
  }
  const uint8_t e = lage.ereignisse;
  lage.ereignisse = 0;
  return e;
}

// Ereignisse aus lageVerarbeiten abholen (für die Nachbildung)
uint8_t lageEreignisse() { const uint8_t e = lage.ereignisse; lage.ereignisse = 0; return e; }
