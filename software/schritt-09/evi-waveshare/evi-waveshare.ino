// Schritt 9: Evi auf dem Waveshare RP2350-Touch-LCD-2.8
//
// Wie Evi auf dem Cube (Schritt 7): dieselben Spielregeln (pet.h), dieselben
// Zeichnungen (größer), derselbe Spielstand-Aufbau (save.h) und dieselben Melodien.
// Bedienung per Touch: unten vier Knöpfe Essen, Spiel, Putzen, Licht.
//
// Board in der Arduino IDE: „Raspberry Pi Pico 2“. Bibliothek: „Adafruit GFX Library“.

#include <EEPROM.h>
#include "board.h"
#include "bildschirm.h"
#include "touch.h"
#include "ton.h"
#include "pet.h"
#include "save.h"
#include "evi_art.h"

// ---------- Einstellungen ----------
const int HELL_PROZENT = 80;           // Beleuchtung tagsüber
const int NACHT_PROZENT = 15;          // Beleuchtung bei Licht aus
const uint32_t DUNKEL_NACH_MS = 60000; // ohne Berührung geht der Bildschirm nach 1 Minute aus
const uint32_t SPEICHERN_ALLE_MS = 30UL * 60UL * 1000UL;  // alle 30 Minuten
const uint32_t BILD_ALLE_MS = 100;     // 10 Bilder pro Sekunde

// ---------- Aufteilung des Bildschirms (320 x 240) ----------
const int EVI_BREITE = 200;    // links: Evi
const int EVI_BODEN = 188;     // Füße auf dieser Höhe
const int WERTE_X = 206;       // rechts: Satt, Laune, Sauber
const int WERTE_B = 108;
const int KNOPF_Y = 197, KNOPF_H = 40, KNOPF_B = 76;
const int KNOPF_X[4] = {4, 84, 164, 244};
const char* KNOPF_TEXT[4] = {"Essen", "Spiel", "Putzen", "Licht"};
enum Knopf { ESSEN = 0, SPIEL = 1, PUTZEN = 2, LICHT = 3, KEIN_KNOPF = -1 };

// ---------- Spielstand im Flash ----------
// Das Board hat kein echtes EEPROM; die EEPROM-Bibliothek legt die Bytes in
// einen 4-KB-Block des Flash. commit() löscht und beschreibt den Block neu.
// Flash hält etwa 100.000 Löschvorgänge. Deshalb nur alle 30 Minuten
// (und nach Pflege oder Licht) und nur, wenn sich etwas geändert hat.
struct FlashSpeicher {
  uint8_t read(uint16_t adresse) { return EEPROM.read(adresse); }
  void update(uint16_t adresse, uint8_t wert) {
    if (EEPROM.read(adresse) != wert) EEPROM.write(adresse, wert);
  }
};
FlashSpeicher flash;
SaveSlots<FlashSpeicher> spielstand(flash);

Pet pet;
uint32_t letztesSpeichern = 0;
uint32_t letztesBild = 0;
uint32_t letzteBeruehrung = 0;
bool bildschirmAn = true;
uint8_t gemeldet = 0;         // je Bedürfnis ein Bit: Hinweiston schon gespielt
int gedrueckterKnopf = KEIN_KNOPF;
uint32_t gedruecktSeit = 0;

void speichern(uint32_t jetzt) {
  if (spielstand.save(pet.state())) EEPROM.commit();
  letztesSpeichern = jetzt;
}

// ---------- Touch: aus dem Strom von Meldungen einzelne Tipper machen ----------
bool fingerUnten = false;
uint32_t letzteMeldung = 0;

// true, wenn der Finger gerade neu aufgesetzt wurde.
bool tipper(int& x, int& y) {
  int tx, ty;
  bool neu = false;
  while (touchLesen(tx, ty)) {
    if (!fingerUnten) {
      neu = true;
      x = tx;
      y = ty;
    }
    fingerUnten = true;
    letzteMeldung = millis();
  }
  if (fingerUnten && millis() - letzteMeldung > 150) fingerUnten = false;
  return neu;
}

int knopfBei(int x, int y) {
  if (y < KNOPF_Y - 6) return KEIN_KNOPF;
  for (int i = 0; i < 4; i++) {
    if (x >= KNOPF_X[i] - 2 && x < KNOPF_X[i] + KNOPF_B + 2) return i;
  }
  return KEIN_KNOPF;
}

// ---------- Zeichnen ----------
void textMittig(const char* text, int mitteX, int y, int groesse) {
  bild.setTextSize(groesse);
  bild.setCursor(mitteX - (int)strlen(text) * 3 * groesse, y);
  bild.print(text);
}

void zeichneEvi(uint32_t jetzt) {
  const Face gesicht = pet.face();
  const uint8_t* bits = ART_GLUECKLICH;
  int w = ART_GLUECKLICH_W, h = ART_GLUECKLICH_H;
  if (gesicht == Face::Hungry)        { bits = ART_HUNGRIG;   w = ART_HUNGRIG_W;   h = ART_HUNGRIG_H; }
  else if (gesicht == Face::Sad)      { bits = ART_TRAURIG;   w = ART_TRAURIG_W;   h = ART_TRAURIG_H; }
  else if (gesicht == Face::Dirty)    { bits = ART_SCHMUTZIG; w = ART_SCHMUTZIG_W; h = ART_SCHMUTZIG_H; }
  else if (gesicht == Face::Sleeping) { bits = ART_SCHLAFEND; w = ART_SCHLAFEND_W; h = ART_SCHLAFEND_H; }

  // Beim Spielen tanzt Evi hin und her, beim Putzen schüttelt sie sich (wie auf dem Cube).
  int dx = 0;
  if (pet.reaction() == Reaction::Playing) dx = ((jetzt / 200) % 2) ? 8 : -8;
  else if (pet.reaction() == Reaction::Cleaning) dx = ((jetzt / 70) % 2) ? 3 : -3;

  const int x = (EVI_BREITE - w) / 2 + dx;
  const int y = EVI_BODEN - h;
  bild.drawBitmap(x, y, bits, w, h, 0);

  if (gesicht == Face::Sleeping) {
    bild.setTextSize(2);
    bild.setCursor(x + w - 40, y - (((jetzt / 700) % 2) ? 26 : 34));
    bild.print("Zzz");
  }
}

void zeichneWert(int y, const char* name, uint8_t wert) {
  bild.setTextSize(2);
  bild.setCursor(WERTE_X, y);
  bild.print(name);
  if (wert <= Pet::CARE_AT) {
    bild.setCursor(WERTE_X + WERTE_B - 12, y);
    bild.print("!");
  }
  bild.drawRect(WERTE_X, y + 20, WERTE_B, 16, 0);
  const int fuellung = (int)wert * (WERTE_B - 4) / 100;
  if (fuellung > 0) bild.fillRect(WERTE_X + 2, y + 22, fuellung, 12, 0);
}

void zeichneKnopf(int i, bool gedrueckt) {
  if (gedrueckt) bild.fillRoundRect(KNOPF_X[i], KNOPF_Y, KNOPF_B, KNOPF_H, 6, 0);
  else bild.drawRoundRect(KNOPF_X[i], KNOPF_Y, KNOPF_B, KNOPF_H, 6, 0);
  bild.setTextColor(gedrueckt ? 1 : 0);
  textMittig(KNOPF_TEXT[i], KNOPF_X[i] + KNOPF_B / 2, KNOPF_Y + 13, 2);
  bild.setTextColor(0);
}

void zeichneAlles(uint32_t jetzt) {
  bild.fillScreen(1);  // weiß wie Papier
  bild.setTextColor(0);
  zeichneEvi(jetzt);

  const PetState s = pet.state();
  zeichneWert(4, "Satt", s.satiety);
  zeichneWert(52, "Laune", s.mood);
  zeichneWert(100, "Sauber", s.cleanliness);

  const Reaction r = pet.reaction();
  const char* ausruf = r == Reaction::Eating ? "Mjam!" : r == Reaction::Playing ? "Juhu!" :
                       r == Reaction::Cleaning ? "Sauber!" : nullptr;
  if (ausruf) textMittig(ausruf, WERTE_X + WERTE_B / 2, 156, 2);

  bild.drawFastHLine(0, 192, BREITE, 0);
  const bool gedrueckt = jetzt - gedruecktSeit < 200;
  if (pet.isAsleep()) {
    // Nachts gibt es nur den Lichtschalter.
    zeichneKnopf(LICHT, gedrueckt && gedrueckterKnopf == LICHT);
  } else {
    for (int i = 0; i < 4; i++) zeichneKnopf(i, gedrueckt && gedrueckterKnopf == i);
  }
  // Licht aus: Bild umgekehrt, also dunkel mit hellen Linien.
  zeigen(pet.isAsleep());
}

// ---------- Bildschirm an und aus ----------
void bildschirmEin(uint32_t jetzt) {
  letzteBeruehrung = jetzt;
  if (!bildschirmAn) {
    bildschirmAn = true;
    zeichneAlles(jetzt);
  }
  helligkeit(pet.isAsleep() ? NACHT_PROZENT : HELL_PROZENT);
}

void bildschirmPruefen(uint32_t jetzt) {
  if (bildschirmAn && jetzt - letzteBeruehrung > DUNKEL_NACH_MS) {
    bildschirmAn = false;
    helligkeit(0);
  }
}

// ---------- Spiel ----------
void knopfGedrueckt(int k, uint32_t jetzt) {
  gedrueckterKnopf = k;
  gedruecktSeit = jetzt;
  if (k == LICHT) {
    pet.update(jetzt);  // bis jetzt im alten Tempo rechnen
    pet.setAsleep(!pet.isAsleep());
    if (pet.isAsleep()) spieleMelodie(MELODIE(MELODIE_GUTE_NACHT));
    else spieleMelodie(MELODIE(MELODIE_AUFWACHEN));
    helligkeit(pet.isAsleep() ? NACHT_PROZENT : HELL_PROZENT);
  } else if (pet.isAsleep()) {
    return;  // im Schlaf nur Licht
  } else if (k == ESSEN) {
    pet.feed(jetzt);
    spieleMelodie(MELODIE(MELODIE_FUETTERN));
  } else if (k == SPIEL) {
    pet.play(jetzt);
    spieleMelodie(MELODIE(MELODIE_SPIELEN));
  } else if (k == PUTZEN) {
    pet.clean(jetzt);
    spieleMelodie(MELODIE(MELODIE_PUTZEN));
  }
  speichern(jetzt);
}

// Ein Hinweiston, sobald ein Wert auf 30 fällt. Im Schlaf erst nach dem Aufwachen.
void hinweisPruefen(uint32_t jetzt) {
  const PetState s = pet.state();
  const uint8_t werte[Pet::NEEDS] = {s.satiety, s.mood, s.cleanliness};
  bool neu = false;
  for (int i = 0; i < Pet::NEEDS; i++) {
    const uint8_t bit = 1 << i;
    if (werte[i] > Pet::CARE_AT) {
      gemeldet &= ~bit;
    } else if (!(gemeldet & bit) && !pet.isAsleep()) {
      gemeldet |= bit;
      neu = true;
    }
  }
  if (neu) {
    spieleMelodie(MELODIE(MELODIE_ACHTUNG));
    bildschirmEin(jetzt);  // Evi meldet sich: Bildschirm geht an
  }
}

// ---------- Neue Evi ----------
// Liegt beim Einschalten ein Finger auf dem Bildschirm, fragt Evi nach.
bool neueEviGewuenscht() {
  uint32_t start = millis();
  bool beruehrt = false;
  int x, y;
  while (millis() - start < 600) {
    if (touchLesen(x, y)) beruehrt = true;
  }
  if (!beruehrt) return false;

  bild.fillScreen(1);
  bild.setTextColor(0);
  textMittig("Neue Evi?", BREITE / 2, 40, 3);
  textMittig("Der alte Spielstand wird geloescht.", BREITE / 2, 90, 1);
  bild.drawRoundRect(30, 150, 110, 50, 8, 0);
  textMittig("Ja", 85, 168, 2);
  bild.drawRoundRect(180, 150, 110, 50, 8, 0);
  textMittig("Nein", 235, 168, 2);
  zeigen();
  helligkeit(HELL_PROZENT);

  // Erst loslassen, dann wählen.
  fingerUnten = true;
  letzteMeldung = millis();
  start = millis();
  while (millis() - start < 30000) {
    if (tipper(x, y) && y > 140) {
      if (x < BREITE / 2) return true;
      return false;
    }
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  pinMode(BAT_EN, OUTPUT);
  digitalWrite(BAT_EN, HIGH);
  EEPROM.begin(256);

  bildschirmStarten();
  touchStarten();
  tonStarten();

  const uint32_t jetzt = millis();
  PetState gespeichert;
  if (neueEviGewuenscht()) {
    spielstand.erase();
    EEPROM.commit();
    pet.restore(Pet::startState(), millis());
    speichern(millis());
    spieleMelodie(MELODIE(MELODIE_NEU));
  } else {
    if (spielstand.load(gespeichert)) pet.restore(gespeichert, jetzt);
    else speichern(jetzt);
    spieleMelodie(MELODIE(MELODIE_HALLO));
  }
  letztesSpeichern = millis();
  zeichneAlles(millis());
  bildschirmEin(millis());
}

void loop() {
  const uint32_t jetzt = millis();
  tonWeiter();

  int x, y;
  if (tipper(x, y)) {
    if (!bildschirmAn) {
      bildschirmEin(jetzt);  // erster Tipper weckt nur den Bildschirm
    } else {
      bildschirmEin(jetzt);
      const int k = knopfBei(x, y);
      // Kurz nacheinander zählt nur einmal, falls der Touch beim Festhalten stockt.
      if (k != KEIN_KNOPF && jetzt - gedruecktSeit > 350) knopfGedrueckt(k, jetzt);
    }
  }

  pet.update(jetzt);
  hinweisPruefen(jetzt);
  if (jetzt - letztesSpeichern >= SPEICHERN_ALLE_MS) speichern(jetzt);
  bildschirmPruefen(jetzt);

  if (bildschirmAn && jetzt - letztesBild >= BILD_ALLE_MS) {
    letztesBild = jetzt;
    zeichneAlles(jetzt);
  }
}
