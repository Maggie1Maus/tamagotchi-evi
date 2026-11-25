// Lagesensor QMI8658 auf dem Board (I2C 0x6B oder 0x6A, an Wire1 wie Touch, Uhr und Tasten).
// Wunsch der Nutzerin vom 24.11.2026: Evi spürt Bewegung – kippen und aufs Gesicht legen.
// Hier stehen der Sensor und die Erkennung; was Evi dann tut, steht in bewegung.ino.
// Gassi gehen (Schritte zählen) und das Erkennen von Schütteln wurden am 25.11.2026 auf Wunsch der Nutzerin
// ganz entfernt, damit der Chip im Dunkeln länger schlafen kann (schlaf.h): Dunkel misst der Sensor nur noch
// etwa alle 120 ms statt alle 20 ms, das reicht fürs Umdrehen.
//
// Gemessen wird die Kraft, die das Gerät hält: liegt es still mit dem Bildschirm nach oben, zeigt der
// Sensor etwa +1 g in Richtung aus dem Bildschirm heraus. Werte hier immer in g.
//
// Achsen: n zeigt aus dem Bildschirm heraus, u zur Oberkante des Bildes, r = u × n zur rechten Kante.
// Wie der Chip auf der Platine liegt, muss niemand wissen: Evi lernt es beim Benutzen. Wer eine Taste
// drückt oder tippt, schaut auf den Bildschirm, hält das Gerät also mit dem Bildschirm nach oben und
// der Oberkante oben. Nach drei solchen Momenten (Gerät dabei ruhig) kennt Evi n; u kennt sie, sobald
// das Gerät dabei schräg gehalten wird (nicht flach auf dem Tisch). Kippen und Aufs-Gesicht-Legen
// wirken erst danach.
#pragma once
#include <Wire.h>
#include <math.h>

// ---------- Erkennung (Zahlen) ----------
// Bewegung allein, ohne die Schwerkraft: „dyn“ = Messung minus langsam geglättete Messung (das ist die
// Schwerkraft), in g. Daraus die Unruhe: Kippen zählt nur, wenn das Gerät dabei sonst ruhig ist.
namespace L {
// Kippen zur Seite: etwa 27° für 0,5 s, während das Gerät sonst ruhig ist; erneut, nachdem es 1 s gerade war (unter 9°).
const float KIPPEN_START = 0.45f, KIPPEN_GERADE = 0.15f, KIPPEN_RUHIG = 0.25f;
const uint32_t KIPPEN_START_MS = 500, KIPPEN_GERADE_MS = 1000;
// Aufs Gesicht: 1,5 s mit dem Bildschirm nach unten; wieder umgedreht nach 0,4 s.
const float GESICHT_UNTEN = -0.8f, GESICHT_OBEN = -0.5f;
const uint32_t GESICHT_UNTEN_MS = 1500, GESICHT_OBEN_MS = 400;
// Lernen der Achsen bei Tastendruck, nur wenn das Gerät ruhig ist (0,85 bis 1,15 g)
const float LERN_RUHIG_MIN = 0.85f, LERN_RUHIG_MAX = 1.15f, LERN_ANTEIL = 0.25f, LERN_OBEN_MIN = 0.35f, LERN_NORMAL_MIN = 0.3f;
const int LERN_PROBEN = 3;
const uint32_t ABTAST_MS = 20;   // 50 Messungen pro Sekunde (bei dunklem Bildschirm seltener, siehe lage.abtastMs)
}

// Was die Erkennung meldet (mehrere zugleich möglich, als Bits)
enum LageEreignis : uint8_t { LE_KIPPEN = 8, LE_GESICHT_UNTEN = 16, LE_GESICHT_OBEN = 32 };

struct Lage {
  bool da = false;               // Sensor gefunden?
  uint8_t adresse = 0;
  bool grossEndian = false;      // Reihenfolge der Bytes (beim Start erkannt)
  uint32_t letzteMessung = 0;
  uint32_t abtastMs = L::ABTAST_MS;   // Abstand der Messungen; im Dunkeln länger (schlaf.h)
  uint32_t letzteZeit = 0;            // Zeit der zuletzt verarbeiteten Messung (fürs Glätten)
  float x = 0, y = 0, z = 1;     // letzte Messung in g (nach dem Maßstab)
  // Maßstab: Liegt das Gerät still, misst der Sensor genau 1 g. Falls er anders eingestellt ist als erwartet,
  // wird das beim Stillliegen gelernt (eins = so viel zeigt er für 1 g).
  float eins = 1;
  float ruheMittel = 1, ruheStreuung = 1;
  int ruhig = 0;
  // Erkennung
  bool glattDa = false;
  float glatt[3] = {0, 0, 1};    // geglättet (für Lage und Kippen)
  float schwer[3] = {0, 0, 1};   // langsam geglättet: die Schwerkraft
  float unruhe = 0;              // wie stark sich das Gerät gerade bewegt (geglättet)
  bool kippBereit = true;
  bool kippSeitDa = false, geradeSeitDa = false;
  uint32_t kippSeit = 0, geradeSeit = 0;
  int kippSeite = 0;
  bool unten = false;
  bool untenSeitDa = false, obenSeitDa = false;
  uint32_t untenSeit = 0, obenSeit = 0, untenUm = 0, untenDauer = 0;
  // Gelernte Achsen: Index 0–2 (x, y, z) und Vorzeichen; -1 = noch unbekannt
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
  // Maßstab vorläufig aus dem Start (liegt das Gerät dabei ruhig, stimmt er gleich)
  uint8_t r[6];
  if (qmiRoh(r)) {
    float x, y, z;
    rohInG(r, lage.grossEndian, x, y, z);
    const float g = sqrtf(x * x + y * y + z * z);
    if (g > 0.2f && g < 5) lage.eins = g;
  }
  return true;
}

// ---------- Erkennung ----------
static inline float lageLaenge(const float v[3]) { return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }

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

// Eine Messung verarbeiten, roh in g laut Einstellung des Sensors (auch von der Nachbildung am PC aufgerufen)
void lageVerarbeiten(float rx, float ry, float rz, uint32_t t) {
  // Maßstab: Ist die Stärke 1 s lang fast gleich (das Gerät liegt still), ist das 1 g.
  const float roh = sqrtf(rx * rx + ry * ry + rz * rz);
  lage.ruheMittel += (roh - lage.ruheMittel) * 0.1f;
  lage.ruheStreuung += ((roh - lage.ruheMittel) * (roh - lage.ruheMittel) - lage.ruheStreuung) * 0.1f;
  lage.ruhig = lage.ruheStreuung < 0.0004f * lage.ruheMittel * lage.ruheMittel ? lage.ruhig + 1 : 0;
  if (lage.ruhig >= 50 && lage.ruheMittel > 0.2f && lage.ruheMittel < 5) lage.eins += (lage.ruheMittel - lage.eins) * 0.05f;
  const float mx = rx / lage.eins, my = ry / lage.eins, mz = rz / lage.eins;
  lage.x = mx; lage.y = my; lage.z = mz;
  const float v[3] = {mx, my, mz};
  // Glätten unabhängig vom Messabstand: Im Dunkeln (etwa alle 120 ms) wirkt eine Messung wie sechs im 20-ms-Takt.
  // Bei hellem Bildschirm bleibt alles wie vorher (eine Messung = ein Schritt).
  float n = 1;
  if (lage.abtastMs != L::ABTAST_MS && lage.glattDa) n = fminf(fmaxf((float)(t - lage.letzteZeit) / (float)L::ABTAST_MS, 1.0f), 8.0f);
  lage.letzteZeit = t;
  const float aGlatt = n <= 1 ? 0.15f : 1 - powf(0.85f, n), aSchwer = n <= 1 ? 0.04f : 1 - powf(0.96f, n),
              aUnruhe = n <= 1 ? 0.1f : 1 - powf(0.9f, n);
  if (!lage.glattDa) {
    for (int i = 0; i < 3; i++) { lage.glatt[i] = lage.schwer[i] = v[i]; }
    lage.glattDa = true;
  } else {
    for (int i = 0; i < 3; i++) {
      lage.glatt[i] += (v[i] - lage.glatt[i]) * aGlatt;
      lage.schwer[i] += (v[i] - lage.schwer[i]) * aSchwer;
    }
  }
  // Bewegung ohne Schwerkraft
  const float d[3] = {v[0] - lage.schwer[0], v[1] - lage.schwer[1], v[2] - lage.schwer[2]};
  lage.unruhe += (lageLaenge(d) - lage.unruhe) * aUnruhe;
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
  // Kippen zur Seite (nur, wenn das Gerät dabei sonst ruhig ist)
  const float seite = lageKippen();
  if (lage.kippBereit) {
    if (fabsf(seite) > L::KIPPEN_START && lage.unruhe < L::KIPPEN_RUHIG && !lage.unten) {
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

// Oft aufrufen: misst alle 20 ms (dunkel seltener). Gibt die neuen Ereignisse zurück (Bits) und vergisst sie.
uint8_t lageLesen() {
  if (!lage.da) return 0;
  const uint32_t m = millis();
  if (m - lage.letzteMessung < lage.abtastMs) return 0;
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
