// Schritt 10: Die Handy-App auf dem Waveshare RP2350-Touch-LCD-2.8 (Etappen 1–4)
//
// Alles an denselben Stellen wie in der Handy-App (handy-app/dist), nur ohne den Rahmen:
// Zimmer, Balken oben, Zeile unten, Menü rechts, Evi auf dem Teppich, Wunschblase.
// Bedienung über die Tasten des Retro Gamers. Touch ist seit 24.11.2026 zum Kuscheln da: Evi antippen oder
// streicheln (kuscheln.ino). Nur wenn die Tasten fehlen, ist der Bildschirm als Notlösung wie früher in drei
// Drittel geteilt (links = weiter, Mitte = Menü öffnen / OK, rechts = zurück / schließen).
// Echte Retro-Gamer-Tasten über den MCP23017 (tasten.h, Belegung vom 23.11.2026):
//   Steuerkreuz hoch/runter = Auswahl im Menü bewegen (Festhalten blättert)
//   A = Menü öffnen / OK, B = zurück / schließen, START = Menü öffnen oder schließen
//   L und R werden in den Spielen gebraucht, RESET schaltet den Bildschirm aus und an.
// Etappe 2: die drei Spiele (spiele.ino). Etappe 3 (Name ändern und Hilfe) seit 24.11.2026 wieder entfernt.
// Etappe 4: Evis eigene Momente, Begrüßung, Tageslaune und Besuch (momente.ino, ab Baby bis Oma).
// Gemeinsame Typen und Zustand dafür stehen in extras.h.
// Seit 24.11.2026: Evi spürt Bewegung (lage.h, bewegung.ino): Gassi gehen, kippen, aufs Gesicht
// legen (Schütteln seit 24.11.2026 wieder entfernt); dazu „Wie war dein Tag?“ und die Spiele Kekse fangen und Schnick, Schnack, Schnuck (nur auf dem Board).
//
// Board in der Arduino IDE: „Raspberry Pi Pico 2“. Bibliothek: „Adafruit GFX Library“.

#include "board.h"
#include "bildschirm.h"
#include "touch.h"
#include "tasten.h"
#include "lage.h"
#include "ton.h"
#include "text.h"
#include "zeit.h"
#include "uhr.h"
#include "pet.h"
#include "himmel.h"
#include "speicher.h"
#include "assets.h"
#include "momente.h"
#include "umgebung.h"
#include "fundbilder.h"
#include "extras.h"
#include "garten.h"
#include "gartenbilder.h"
#include "gartenlandschaft.h"
#include "zimmerlandschaft.h"
#include "raumtiefe.h"

// ---------- Einstellungen ----------
const int HELL_PROZENT = 80;
// Wunsch der Nutzerin vom 24.11.2026: Der Bildschirm bleibt an; RESET schaltet ihn aus und wieder an.
// Eine Zahl > 0 lässt ihn wieder von selbst ausgehen (z. B. 60000 = nach 1 Minute ohne Berührung).
const uint32_t DUNKEL_NACH_MS = 0;
const ms_t SPEICHERN_ALLE = 30 * MINUTE;     // Flash schonen: sonst nur bei Ereignissen
const uint32_t TAKT_MS = 120;                // wie TICK_MS in der App
const ms_t ERSTES_EI = MINUTE;               // wie RESTART_EGG_MS: das allererste Ei schlüpft nach 1 Minute

// Lage wie in der App (Szenenkoordinaten 320 x 224, hier 1:1)
const int BODEN_X = 167, BODEN_Y = 180;
const int STATUS_H = 27;                     // Balkenleiste oben
const int ZEILE_Y = 201;                     // Textzeile unten

// ---------- Zustand ----------
Tier pet;
Himmel himmel = HIMMEL_TAG;
ms_t letztesSpeichern = 0;
uint32_t letzterTakt = 0, letzteRunde = 0, letzteBeruehrung = 0, letztesBild = 0;
bool bildschirmAn = true;

// Typen stehen hier oben, weil die Arduino IDE Funktionsköpfe vor die erste Funktion setzt.
enum Eintrag : uint8_t { E_FUETTERN, E_NASCHEN, E_SPIELEN, E_PUTZEN, E_MEDIZIN, E_LICHT, E_AUFWECKEN, E_RITUAL, E_NEUES_EI,
                         E_TON, E_BALL, E_RATEN, E_MELODIE, E_KEKSE, E_SCHNICK,
                         E_TAG, E_TAG_GUT, E_TAG_LALA, E_TAG_SCHLECHT, E_SIDEBOARD, E_GARTEN };
enum Taste { LINKS, MITTE, RECHTS };
enum Reaktion : uint8_t { R_KEINE, R_FUETTERN, R_NASCHEN, R_SPIELEN, R_PUTZEN, R_MEDIZIN, R_SCHLUEPFEN, R_ENTWICKLUNG,
                          R_GRUSS, R_JUBEL,     // Begrüßung: 2 bzw. 4 Hüpfer
                          R_KUSCHELN };         // gekuschelt: wiegt sich, Herzen (kuscheln.ino)
struct {
  uint32_t takt = 0;
  bool atem = false;
  uint32_t naechsterAtem = 11;
  uint32_t wackelnBis = 0, naechstesWackeln = 30;
  uint32_t schluepfenBis = 0;
  bool schluepfen = false;
  Reaktion reaktion = R_KEINE;
  uint32_t reaktionBisTakt = 0;
  uint32_t reaktionSeit = 0;    // millis()
  int zStufe = 0;
} evi;

char meldung[100] = "";
uint32_t meldungBis = 0;

struct { char text[32]; const Bild* symbol; uint32_t bis; } sagen = {"", nullptr, 0};
ms_t gezeigterWunsch = -1;     // -1 = noch nichts angesehen

// Gute Nacht (Abendritual): Texte nacheinander, jede Taste überspringt.
struct { bool aktiv = false; uint32_t start = 0; int geschichte = 0; int schritt = -1; } ritual;

// ---------- Texte (aus app.js) ----------
const char* MAHLZEITEN[3] = {"Frühstück", "Mittagessen", "Abendessen"};
const char* GESCHICHTEN[10] = {
  "Ein kleiner Stern konnte nicht schlafen. Da summte ihm der Mond ein Lied, und der Stern funkelte ganz leise ein.",
  "Ein Igel suchte im Wald das weichste Blatt für sein Bett. Am Ende lag es direkt vor seiner Haustür.",
  "Ein Fuchs zählte abends die Sterne über seinem Bau. Bei hundertzwei fielen ihm die Augen zu.",
  "Die Maus Pina backte einen Käsekuchen für den Mond. Der Mond lächelte und schickte ihr dafür einen schönen Traum.",
  "Im Garten flüsterten die Blumen sich Gute-Nacht-Witze zu. Die Glühwürmchen lachten so sehr, dass sie leuchteten.",
  "Eine kleine Eule flog heute zum ersten Mal drei Bäume weit. Danach schlief sie stolz in ihrem Astloch ein.",
  "Ein Teddybär hält heimlich Wache, damit nur schöne Träume kommen. Dafür bekommt er morgen eine extra Umarmung.",
  "Ein Fuchs und ein Hase wetteten, wer schneller einschläft. Am Ende schnarchten beide gleichzeitig.",
  "Eine Schneeflocke landete auf {name}s Nase. Es war dort so gemütlich, dass sie gar nicht schmelzen wollte.",
  "Der Wind trug eine kleine Wolke über das Meer. Sie wurde ganz müde und kuschelte sich an einen Berg."
};

void zeigeMeldung(const char* text, uint32_t ms = 2400) {
  strncpy(meldung, text, sizeof(meldung) - 1);
  meldung[sizeof(meldung) - 1] = 0;
  meldungBis = millis() + ms;
}

void sage(const char* text, const Bild* symbol, uint32_t ms = 3000) {
  strncpy(sagen.text, text, sizeof(sagen.text) - 1);
  sagen.symbol = symbol;
  sagen.bis = millis() + ms;
}

void grundText(Grund g, Aktion a, char* aus, size_t n) {
  const char* name = pet.name;
  switch (g) {
    case G_TOT: snprintf(aus, n, "%s lebt nicht mehr.", name); break;
    case G_EI: snprintf(aus, n, "Erst muss %s schlüpfen.", name); break;
    case G_TAG: snprintf(aus, n, "Es ist noch hell. Schlafenszeit ist um 23 Uhr."); break;
    case G_SCHLAEFT: snprintf(aus, n, "%s schläft gerade.", name); break;
    case G_MITTAG: snprintf(aus, n, "Psst, %s macht Mittagsschlaf.", name); break;
    case G_FRUEH: snprintf(aus, n, "Gute Nacht gibt es ab %d Uhr.", R::RITUAL_AB); break;
    case G_SCHON: snprintf(aus, n, "Heute gab es schon Gute Nacht."); break;
    case G_WACH: snprintf(aus, n, "%s ist schon wach.", name); break;
    case G_BAUCH: snprintf(aus, n, "%s hat Bauchweh. Erst mal nichts Süßes.", name); break;
    case G_KRANK: snprintf(aus, n, "%s ist zu krank zum Spielen.", name); break;
    case G_GESUND: snprintf(aus, n, "%s ist gesund.", name); break;
    case G_VOLL:
      snprintf(aus, n, "%s", a == FUETTERN ? "Schon satt" : a == PUTZEN ? "Schon sauber" : a == SPIELEN ? "Schon glücklich" : "Gerade nicht nötig");
      break;
    default: snprintf(aus, n, "Das geht gerade nicht.");
  }
}

// ---------- Speichern ----------
void speichern(ms_t jetzt) {
  spielstandSpeichern(pet, tonAn);
  letztesSpeichern = jetzt;
}

// ---------- Bildschirm an/aus ----------
void bildschirmEin() {
  letzteBeruehrung = millis();
  if (!bildschirmAn) bildschirmAn = true;
  helligkeit(HELL_PROZENT);
}

// ---------- Evi bewegt sich (wie eviTick in app.js) ----------
void reagiere(Reaktion r, uint32_t takte = 2400 / TAKT_MS) {
  evi.reaktion = r;
  evi.reaktionBisTakt = evi.takt + takte;
  evi.reaktionSeit = millis();
}

void runde();

void eviTakt(ms_t jetzt) {
  const uint32_t t = ++evi.takt;
  const Stimmung s = beschreiben(pet, jetzt);
  const bool langsam = s == S_TRAURIG || s == S_HUNGRIG || s == S_KRANK || s == S_SCHLAEFT;
  if (t >= evi.naechsterAtem) { evi.atem = !evi.atem; evi.naechsterAtem = t + (s == S_SCHLAEFT ? 20 : langsam ? 15 : 11); }
  if (t >= evi.naechstesWackeln) { evi.wackelnBis = t + 8; evi.naechstesWackeln = t + random(30, 71); }
  if (evi.schluepfen && t == evi.schluepfenBis - 8) piep(TON(TON_RISS));
  if (evi.schluepfen && t >= evi.schluepfenBis) {
    evi.schluepfen = false;
    reagiere(R_SCHLUEPFEN, 25);
    piep(TON(TON_SCHLUEPFEN));
    char text[64];
    snprintf(text, sizeof(text), "%s ist geschlüpft! Willkommen!", pet.name);
    zeigeMeldung(text, 6000);
  }
  if (s == S_SCHLAEFT && t % 8 == 0) evi.zStufe = (t / 8) % 3;
  // Pünktlich schlüpfen, während jemand zuschaut
  if (phaseBei(pet, pet.aktualisiert) == EI && jetzt >= pet.schluepft) runde();
}

// ---------- Ereignisse (wie announce in app.js) ----------
void melde(const Tier& vorher, ms_t jetzt) {
  const Phase von = phaseBei(vorher, vorher.aktualisiert), zu = phaseBei(pet, jetzt);
  char text[80];
  if (von != zu) {
    if (von == EI && zu != TOT) {
      evi.schluepfenBis = evi.takt + 16;
      evi.schluepfen = true;
      zeigeMeldung("Das Ei bricht auf …", 2000);
    } else if (zu == TOT) {
      piep(TON(TON_TRAURIG));
      snprintf(text, sizeof(text), pet.ursache == ALTER ? "%s ist friedlich eingeschlafen." : "%s ist gestorben.", pet.name);
      zeigeMeldung(text, 8000);
    } else if (zu == TEEN || zu == ERWACHSEN || zu == OMA) {
      piep(TON(TON_ENTWICKLUNG));
      reagiere(R_ENTWICKLUNG, 25);
      snprintf(text, sizeof(text), zu == TEEN ? "%s ist jetzt ein Teenager!" : zu == ERWACHSEN ? "%s ist erwachsen geworden!" : "%s ist jetzt eine Oma!", pet.name);
      zeigeMeldung(text, 6000);
    }
    bildschirmEin();
    speichern(jetzt);
    return;
  }
  if (!vorher.krank && pet.krank) {
    piep(TON(TON_ACHTUNG));
    snprintf(text, sizeof(text), "%s ist krank geworden!", pet.name);
    zeigeMeldung(text, 5000);
    bildschirmEin();
    speichern(jetzt);
    return;
  }
  if (geweckt(vorher, vorher.aktualisiert) && schlafArt(pet, jetzt) == NACHTS) {
    snprintf(text, sizeof(text), "%s ist wieder eingeschlafen.", pet.name);
    zeigeMeldung(text, 5000);
    return;
  }
  if (!schlaeft(pet, jetzt) &&
      ((vorher.satt > R::NIEDRIG && pet.satt <= R::NIEDRIG) || (vorher.laune > R::NIEDRIG && pet.laune <= R::NIEDRIG) ||
       (vorher.sauber > R::NIEDRIG && pet.sauber <= R::NIEDRIG))) {
    piep(TON(TON_ACHTUNG));
    bildschirmEin();
  }
  // Guten Morgen: Sie wacht auf, während du zuschaust.
  if (bildschirmAn && schlafArt(vorher, vorher.aktualisiert) == NACHTS && !schlaeft(pet, jetzt) && !geweckt(pet, jetzt))
    begruessen(vorher.aktualisiert);
}

// Alle 10 Sekunden und bei Bedarf: Zeit nachrechnen (tick in app.js)
void runde() {
  const ms_t jetzt = jetztMs();
  const Tier vorher = pet;
  weiterrechnen(pet, jetzt);
  melde(vorher, jetzt);
  himmel = himmelBei(jetzt);
  // Neuer Wunsch: kurzer Ton und Bildschirm an
  Wunsch w;
  const bool hat = !ritual.aktiv && aktuellerWunsch(pet, jetzt, w);
  if (hat && w.beginn != gezeigterWunsch && gezeigterWunsch != -1) { piep(TON(TON_WUNSCH)); bildschirmEin(); }
  gezeigterWunsch = hat ? w.beginn : 0;
  if (jetzt - letztesSpeichern >= SPEICHERN_ALLE) speichern(jetzt);
}

// ---------- Zeichnen ----------
void ellipseFuellen(int cx, int cy, int rx, int ry, uint16_t farbe) {
  if (rx <= 0 || ry <= 0) return;
  for (int dy = -ry; dy <= ry; dy++) {
    const float f = 1.0f - (float)(dy * dy) / (float)(ry * ry);
    const int w = (int)(rx * sqrtf(f < 0 ? 0 : f) + 0.5f);
    bild.drawFastHLine(cx - w, cy + dy, 2 * w + 1, farbe);
  }
}

void zeichneBild(const Bild& b, int x, int y) {
  if (b.maske) bild.drawBitmap(x, y, b.maske, b.w, b.h, 1);
  bild.drawBitmap(x, y, b.linien, b.w, b.h, 0);
}

void zeichneSymbol(const Bild& b, int x, int y, uint16_t farbe) { bild.drawBitmap(x, y, b.linien, b.w, b.h, farbe); }

// Gespiegelt (Evi läuft nach rechts): innen weiß, Linien schwarz
void zeichneBildGespiegelt(const Bild& b, int x, int y) {
  const int zeile = (b.w + 7) / 8;
  for (int r = 0; r < b.h; r++) {
    for (int c = 0; c < b.w; c++) {
      const int i = r * zeile + c / 8, bit = 0x80 >> (c % 8);
      const int zx = x + b.w - 1 - c;
      if (b.linien[i] & bit) bild.drawPixel(zx, y + r, 0);
      else if (b.maske && (b.maske[i] & bit)) bild.drawPixel(zx, y + r, 1);
    }
  }
}

// Welches Bild passt gerade (artChoice in app.js)
const Bild* eviBild(Stimmung s, bool jubelt, bool istMuede, bool allesGut, Phase ph) {
  // Reihenfolge in ART: base, gluecklich, hungrig, traurig, muede, krank, schmutzig, schlafend
  const Bild* const* satz = ART[ph];
  int m;
  if (jubelt) m = 1;
  else if (s == S_SCHLAEFT) m = 7;
  else if (s == S_KRANK || s == S_BAUCH) m = 5;
  else if (s == S_HUNGRIG) m = 2;
  else if (s == S_SCHMUTZIG) m = 6;
  else if (s == S_TRAURIG) m = 3;
  else m = istMuede ? 4 : allesGut ? 1 : 0;
  if (satz[m]) return satz[m];
  if (satz[0]) return satz[0];
  return satz[1];
}

void zeichneEvi(ms_t jetzt, Stimmung s, Phase ph) {
  if (sideboard.phase != SB_ZU && sideboard.phase != SB_OFFEN) { sideboardEviZeichnen(ph); return; }
  const uint32_t t = evi.takt;
  const uint32_t seit = millis() - evi.reaktionSeit;
  const bool reagiert = seit < 2400 && evi.reaktion != R_KEINE;
  int dx = 0, dy = 0;
  float schatten = 1;
  const uint32_t hopDauer = evi.reaktion == R_SPIELEN ? 1260 : evi.reaktion == R_GRUSS ? 840 : evi.reaktion == R_JUBEL ? 1680 : 0;
  if (reagiert && seit < hopDauer) {                                    // hop .42s x3 (Gruß x2, Jubel x4)
    const int k = (seit / 140) % 3; dy = k ? -9 : 0; schatten = k ? 0.87f : 1;
  } else if (reagiert && (evi.reaktion == R_FUETTERN || evi.reaktion == R_NASCHEN) && seit < 1920) {   // munch .48s x4
    dy = ((seit / 240) % 2) ? 2 : 0;
  } else if (reagiert && evi.reaktion == R_PUTZEN && seit < 1350) {    // wiggle .45s x3
    const int k = (seit / 150) % 3; dx = k == 1 ? -1 : k == 2 ? 1 : 0;
  } else if (evi.reaktion == R_KUSCHELN && t < evi.reaktionBisTakt) {  // gekuschelt: sanft hin und her wiegen
    static const int8_t WIEGEN[8] = {0, -1, -2, -1, 0, 1, 2, 1};
    dx = WIEGEN[(seit / 120) % 8];
  }

  if (!spiel.aktiv && ph == TOT) { raumFigurZeichnen(ART_GRAB, ph, BODEN_X, RAUM_HEIM_Y, false, 0, false, false); return; }
  if (ph == TOT) {
    ellipseFuellen(BODEN_X, BODEN_Y - 1, 34, 4, 0);
    zeichneBild(ART_GRAB, BODEN_X + ART_GRAB.ox, BODEN_Y + ART_GRAB.oy);
    eviLinks = BODEN_X + ART_GRAB.ox; eviBreite = ART_GRAB.w; eviOben = BODEN_Y + ART_GRAB.oy; eviHoehe = ART_GRAB.h;
    return;
  }
  const bool schluepft = t < evi.schluepfenBis;
  if (ph == EI || schluepft) {
    const bool bald = ph == EI && pet.schluepft - jetzt < 2 * MINUTE;
    const bool schnell = schluepft || bald;
    static const int WACKEL[4] = {0, 1, 0, -1};
    const int w = (schnell || t < evi.wackelnBis) ? WACKEL[(schnell ? t : t / 2) % 4] : 0;
    const Bild& ei = w > 0 ? ART_EI_RECHTS : w < 0 ? ART_EI_LINKS : *ART[EI][0];
    if (!spiel.aktiv) { raumFigurZeichnen(ei, EI, BODEN_X + dx, RAUM_HEIM_Y, false, -dy, false, false); return; }
    ellipseFuellen(BODEN_X, BODEN_Y - 1, (int)(ART[EI][0]->w * 0.32f + 0.5f), 4, 0);
    zeichneBild(ei, BODEN_X + ei.ox + dx, BODEN_Y + ei.oy + dy);
    eviLinks = BODEN_X + ei.ox; eviBreite = ei.w; eviOben = BODEN_Y + ei.oy; eviHoehe = ei.h;
    return;
  }
  const bool jubelt = t < evi.reaktionBisTakt && evi.reaktion != R_MEDIZIN && evi.reaktion != R_KEINE;
  const bool istMuede = ritual.aktiv || (s == S_GLUECKLICH && muede(pet, jetzt));
  const bool allesGut = (pet.satt >= 60 && pet.laune >= 60 && pet.sauber >= 60) || ausgeschlafen(pet, jetzt);
  const Stimmung gezeigt = ritual.aktiv ? S_GLUECKLICH : s;
  // Welches Bild (artChoice in app.js): beim Begrüßen „hüpfen“, sonst ein Bild, das ein Moment ausleiht
  const Bild* b = nullptr;
  const bool gruss = jubelt && (evi.reaktion == R_GRUSS || evi.reaktion == R_JUBEL);
  if (gruss && momenteFuer(ph)) b = MOMENT_BILD[ph][MB_HUEPFEN];
  else if (!jubelt) b = momentBild(ph);
  if (!b) b = eviBild(gezeigt, jubelt, istMuede, allesGut, ph);
  // Wo: auf dem Teppich, im Moment oder Spiel verschoben, gespiegelt oder angehoben
  if (!spiel.aktiv) {
    raumFigurZeichnen(*b, ph, BODEN_X + geher.x + dx, geher.fuss, geher.flip, geher.lift - dy + (evi.atem ? 1 : 0), false, false);
    return;
  }
  const int wx = geher.x + spiel.eviX;
  eviFuss = BODEN_Y;
  ellipseFuellen(BODEN_X + wx, BODEN_Y - 1, (int)(b->w * 0.32f * schatten + 0.5f), 4, 0);
  const int oben = BODEN_Y + b->oy - (evi.atem ? 2 : 0);
  eviOben = oben;
  eviHoehe = b->h;
  eviLinks = geher.flip ? BODEN_X - b->ox - b->w + wx : BODEN_X + b->ox + wx;
  eviBreite = b->w;
  if (geher.flip) zeichneBildGespiegelt(*b, BODEN_X - b->ox - b->w + wx + dx, oben - geher.lift + dy);
  else zeichneBild(*b, BODEN_X + b->ox + wx + dx, oben - geher.lift + dy);
}

void zeichneZ() {
  const int x=min(272,eviLinks+eviBreite+5), y=max(35,eviOben-13);
  bild.fillRect(x,y,44,24,1);bild.drawRect(x-1,y-1,46,26,0);bild.drawRect(x,y,44,24,0);
  static const char* Z[3]={"z","zZ","zZz"};
  const char* z=Z[evi.zStufe];schreibe(bild,x+22-textBreite(z,2)/2,y+4,z,0,2,true);
}

// Die Uhrzeit, klein oben an der Wand zwischen Fenster und Evi (Wunsch der Nutzerin vom 24.11.2026).
// Dort ist in jeder Jahreszeit und auch zu Weihnachten Platz. Ein weißes Feld dahinter hält sie lesbar,
// auch bei Licht aus.
const int UHRZEIT_X = 124, UHRZEIT_Y = 31;
void zeichneUhrzeit(ms_t jetzt) {
  const Ortszeit o = ortszeit(jetzt);
  char z[8];
  snprintf(z, sizeof(z), "%02d:%02d", o.stunde, o.minute);
  bild.fillRect(UHRZEIT_X - 2, UHRZEIT_Y - 2, textBreite(z) + 3, 11, 1);
  schreibe(bild, UHRZEIT_X, UHRZEIT_Y, z, 0);
}

void zeichneReaktion() {
  const uint32_t seit = millis() - evi.reaktionSeit;
  if (evi.reaktion == R_KEINE || evi.reaktion == R_KUSCHELN || seit >= 2400) return;   // Kuscheln: eigene Herzen
  // spark 1.5s steps(4): von 6 Pixel unten nach 16 Pixel oben, danach oben stehen
  const int stufe = seit >= 1500 ? 4 : (int)(seit / 375);
  const int dy = 6 - stufe * 22 / 4;
  const Bild& gross = evi.reaktion == R_SPIELEN ? SYM_R_NOTE : evi.reaktion == R_PUTZEN || evi.reaktion == R_SCHLUEPFEN ||
                      evi.reaktion == R_ENTWICKLUNG ? SYM_R_STERN : evi.reaktion == R_MEDIZIN ? SYM_R_PLUS : SYM_R_HERZ;
  const int wx = geher.x + spiel.eviX;    // die Zeichen gehen mit Evi mit (evi-pos in der App)
  const int rx = spiel.aktiv ? 213 + wx : eviLinks + eviBreite + 5;
  const int ry = spiel.aktiv ? 105 : eviOben + eviHoehe / 3;
  zeichneSymbol(gross, rx - gross.w / 2, ry - gross.h + dy, 0);
  zeichneSymbol(SYM_R_STERN_KLEIN, rx + 17, ry + 11 - SYM_R_STERN_KLEIN.h + dy, 0);
}

void wunschText(const Wunsch& w, ms_t jetzt, const Bild*& sym, char* text, size_t n) {
  const int m = mahlzeitFaellig(pet, jetzt);
  switch (w.art) {
    case W_BALL: sym = &SYM_W_BALL; snprintf(text, n, "Ballfangen!"); break;
    case W_RATEN: sym = &SYM_W_PFEILE; snprintf(text, n, "Links oder rechts?"); break;
    case W_MELODIE: sym = &SYM_W_NOTE; snprintf(text, n, "Melodie merken!"); break;
    case W_NASCHEN: sym = &SYM_W_KEKS; snprintf(text, n, "Was Süßes!"); break;
    case W_PUTZEN: sym = &SYM_W_PUTZEN; snprintf(text, n, "Baden!"); break;
    default: sym = &SYM_W_FUTTER; if (m >= 0) snprintf(text, n, "%s!", MAHLZEITEN[m]); else snprintf(text, n, "Hunger!");
  }
}

void zeichneBlase(ms_t jetzt) {
  Wunsch w;
  const bool redet = millis() < sagen.bis;
  const bool wunsch = !ritual.aktiv && aktuellerWunsch(pet, jetzt, w);
  if (!redet && !wunsch) return;
  const Bild* sym;
  char text[40];
  if (redet) { sym = sagen.symbol; strncpy(text, sagen.text, sizeof(text)); }
  else wunschText(w, jetzt, sym, text, sizeof(text));
  // Wie .wish-bubble: oben 27 + 12 %, links 58 %, höchstens 40 % breit
  // Die Blase bleibt über ihrem Kopf, aber im Bild: links 58 % + x/3,2, zwischen 4 % und 58 %
  float prozent = 58 + geher.x / 3.2f;
  if (prozent < 4) prozent = 4;
  if (prozent > 58) prozent = 58;
  const int x = 186 + (int)lroundf(BREITE * (prozent - 58) / 100), y = max(34, eviOben - 43), maxB = 128;
  const int maxZeichen = (maxB - 4 - 16 - sym->w - 5) / 6;
  char zeilen[3][40];
  int n = 0;
  umbrechen(text, maxZeichen, [&](const char* z) { if (n < 3) { strncpy(zeilen[n], z, 39); zeilen[n][39] = 0; n++; } });
  int breiteste = 0;
  for (int i = 0; i < n; i++) breiteste = max(breiteste, textBreite(zeilen[i]) + 1);
  const int innenH = max((int)sym->h, n * 10 - 2);
  const int w2 = 2 + 8 + sym->w + 5 + breiteste + 8 + 2, h2 = 2 + 4 + innenH + 4 + 2;
  bild.fillRoundRect(x, y, w2, h2, 10, 1);
  bild.drawRoundRect(x, y, w2, h2, 10, 0);
  bild.drawRoundRect(x + 1, y + 1, w2 - 2, h2 - 2, 9, 0);
  // Spitze nach unten links zu Evis Kopf
  bild.fillTriangle(x + 12, y + h2 - 2, x + 12, y + h2 + 7, x + 22, y + h2 - 2, 1);
  bild.drawLine(x + 12, y + h2 - 2, x + 12, y + h2 + 7, 0);
  bild.drawLine(x + 13, y + h2 - 2, x + 13, y + h2 + 6, 0);
  bild.drawLine(x + 12, y + h2 + 7, x + 22, y + h2 - 1, 0);
  bild.drawLine(x + 13, y + h2 + 7, x + 23, y + h2 - 1, 0);
  zeichneSymbol(*sym, x + 10, y + (h2 - sym->h) / 2, 0);
  const int tx = x + 10 + sym->w + 5, ty = y + (h2 - (n * 10 - 2)) / 2;
  for (int i = 0; i < n; i++) schreibe(bild, tx, ty + i * 10, zeilen[i], 0, 1, true);
}

void ritualText(char* aus, size_t n) {
  const uint32_t seit = millis() - ritual.start;
  if (seit < 2600) snprintf(aus, n, "Zähne putzen … schrubb, schrubb!");
  else if (seit < 10600) {
    char g[160];
    const char* q = GESCHICHTEN[ritual.geschichte];
    const char* platz = strstr(q, "{name}");
    if (platz) snprintf(g, sizeof(g), "%.*s%s%s", (int)(platz - q), q, pet.name, platz + 6);
    else snprintf(g, sizeof(g), "%s", q);
    snprintf(aus, n, "Gute-Nacht-Geschichte: %s", g);
  } else if (seit < 13200) snprintf(aus, n, "Zudecken … schön kuschelig.");
  else snprintf(aus, n, "Licht aus. Gute Nacht, %s! ♥", pet.name);
}

void zeichneGeschichte() {
  char text[220];
  ritualText(text, sizeof(text));
  char zeilen[6][60];
  int n = 0;
  umbrechen(text, 47, [&](const char* z) { if (n < 6) { strncpy(zeilen[n], z, 59); zeilen[n][59] = 0; n++; } });
  const int x = 10, y = STATUS_H + 6, w = 300, h = 4 + 4 + n * 10 + 2 + 4;
  bild.fillRoundRect(x, y, w, h, 8, 1);
  bild.drawRoundRect(x, y, w, h, 8, 0);
  bild.drawRoundRect(x + 1, y + 1, w - 2, h - 2, 7, 0);
  for (int i = 0; i < n; i++) schreibe(bild, x + 9, y + 7 + i * 10, zeilen[i], 0);
}

void zeichneStatus(Phase ph) {
  bild.fillRect(0, 0, BREITE, STATUS_H - 1, 1);
  bild.drawFastHLine(0, STATUS_H - 1, BREITE, 0);
  const double werte[3] = {pet.satt, pet.laune, pet.sauber};
  const Bild* symbole[3] = {&SYM_NAPF, &SYM_HERZ, &SYM_GLANZ};
  for (int i = 0; i < 3; i++) {
    const int nutzbar = BREITE - RAND_LINKS;
    const int x0 = RAND_LINKS + (i * nutzbar + 1) / 3, x1 = RAND_LINKS + ((i + 1) * nutzbar + 1) / 3;
    const int wert = (int)(werte[i] + 0.5);
    const bool niedrig = wert <= R::NIEDRIG && ph != EI && ph != TOT;
    const uint16_t vorne = niedrig ? 1 : 0;
    if (niedrig) bild.fillRect(x0, 0, x1 - x0, STATUS_H - 1, 0);
    if (i > 0) bild.drawFastVLine(x0, 0, STATUS_H - 1, 0);
    zeichneSymbol(*symbole[i], x0 + 5 + (14 - symbole[i]->w) / 2, 13 - symbole[i]->h / 2, vorne);
    char zahl[8];
    snprintf(zahl, sizeof(zahl), "%d%%", wert);
    const int zw = textBreite(zahl) - 1;
    schreibe(bild, x1 - 5 - zw, 10, zahl, vorne, 1, true);
    const int mx = x0 + 5 + 14 + 4, mw = x1 - 5 - zw - 1 - 4 - mx;
    bild.drawRect(mx, 10, mw, 7, vorne);
    bild.fillRect(mx + 1, 11, (mw - 2) * wert / 100, 5, vorne);
  }
}

void stimmungsText(Stimmung s, ms_t jetzt, char* aus, size_t n) {
  const char* name = pet.name;
  if (ritual.aktiv) { snprintf(aus, n, "Gute Nacht …"); return; }
  switch (s) {
    case S_TOT: snprintf(aus, n, pet.ursache == ALTER ? "%s ist friedlich eingeschlafen. Menü: neues Ei" : "%s ist gestorben. Menü: neues Ei", name); return;
    case S_EI: {
      ms_t minuten = (pet.schluepft - jetzt + MINUTE - 1) / MINUTE;
      if (minuten < 1) minuten = 1;
      snprintf(aus, n, "Schlüpft in ca. %d Min.", (int)minuten);
      return;
    }
    case S_SCHLAEFT:
      if (schlafArt(pet, jetzt) == MITTAGSSCHLAF) snprintf(aus, n, "Psst … Mittagsschlaf bis %d Uhr", R::MITTAG_BIS);
      else snprintf(aus, n, "%s", pet.lichtAus ? "Psst … schläft" : "Schläft · Licht ausmachen?");
      return;
    case S_KRANK: snprintf(aus, n, "Krank · braucht Medizin"); return;
    case S_BAUCH: snprintf(aus, n, "Bauchweh … zu viel genascht"); return;
    case S_HUNGRIG: snprintf(aus, n, "Mein Bauch knurrt"); return;
    case S_SCHMUTZIG: snprintf(aus, n, "Zeit für ein Bad"); return;
    case S_TRAURIG: snprintf(aus, n, "Spielst du mit mir?"); return;
    default: {
      Wunsch w;
      if (geweckt(pet, jetzt)) { snprintf(aus, n, "Gähn … wach für %d Min.", (int)max((ms_t)1, (pet.wachBis - jetzt + MINUTE - 1) / MINUTE)); return; }
      if (gassiZeile(jetzt, aus, n)) return;
      if (aktuellerWunsch(pet, jetzt, w)) { snprintf(aus, n, "Ein Wunsch!"); return; }
      if (ausgeschlafen(pet, jetzt)) { snprintf(aus, n, "Ausgeschlafen! Guten Morgen"); return; }
      if (muede(pet, jetzt)) { snprintf(aus, n, "%s", stundeVon(jetzt) < 12 ? "Gähn … guten Morgen" : "Gähn … Zeit für Gute Nacht?"); return; }
      // Alle 10 Sekunden im Wechsel: Tageslaune (ab Baby), Schritte von heute, Hinweis zum Menü
      const int wechsel = (int)((jetzt / 10000) % 3);
      if (momenteFuer(phaseBei(pet, jetzt)) && wechsel == 1) { launeZeile(jetzt, aus, n); return; }
      if (wechsel == 2 && schritteZeile(jetzt, aus, n)) return;
      // Mit Tasten ist Touch zum Kuscheln da: im Wechsel daran erinnern
      if (tastenDa && (jetzt / 30000) % 2) snprintf(aus, n, "Tipp mich an zum Kuscheln ♥");
      else snprintf(aus, n, "%s", tastenDa ? "A öffnet das Menü" : "Mitte öffnet das Menü");
    }
  }
}

// ---------- Menü ----------
// symbol = Bild aus der App; ohne Bild eine gezeichnete Figur (zeichen: 0 Schere, 3 Wolke, 4 Regenwolke)
struct MenuePunkt { Eintrag e; const Bild* symbol; int8_t zeichen; char text[26]; };
MenuePunkt menue[14];
int menueAnzahl = 0, gewaehlt = 0;
bool menueOffen = false, spieleAuswahl = false, tagAuswahl = false;

void punkt(Eintrag e, const Bild* s, const char* t, int zeichen = -1) {
  if (menueAnzahl >= 14) return;
  menue[menueAnzahl].e = e;
  menue[menueAnzahl].symbol = s;
  menue[menueAnzahl].zeichen = (int8_t)zeichen;
  strncpy(menue[menueAnzahl].text, t, 25);
  menue[menueAnzahl].text[25] = 0;
  menueAnzahl++;
}

void menueBauen() {
  menueAnzahl = 0;
  const ms_t jetzt = jetztMs();
  if (spieleAuswahl) {
    punkt(E_BALL, &SYM_M_BALL, "Ballfangen");
    punkt(E_RATEN, &SYM_M_PFEILE, "Links oder rechts?");
    punkt(E_MELODIE, &SYM_M_NOTE, "Melodie merken");
    punkt(E_KEKSE, &SYM_M_KEKS, "Kekse fangen");
    punkt(E_SCHNICK, nullptr, "Schnick, Schnack", 0);
  } else if (tagAuswahl) {
    punkt(E_TAG_GUT, &SYM_M_SONNE, "Gut");
    punkt(E_TAG_LALA, nullptr, "So lala", 3);
    punkt(E_TAG_SCHLECHT, nullptr, "Nicht so gut", 4);
  } else {
    const Phase ph = phaseBei(pet, jetzt);
    if (ph == TOT) {
      punkt(E_NEUES_EI, &SYM_M_EI, "Neues Ei");
    } else {
      if (ph != EI) {
        const Schlaf s = schlafArt(pet, jetzt);
        if (s == NACHTS) {
          punkt(E_AUFWECKEN, &SYM_M_SONNE, "Aufwecken");
          punkt(E_LICHT, &SYM_M_LICHT, pet.lichtAus ? "Licht an" : "Licht aus");
        } else if (s == WACH) {
          if (gesperrt(pet, RITUAL, jetzt) == OK) punkt(E_RITUAL, &SYM_M_MOND, "Gute Nacht");
          if (tagImMenue(jetzt)) punkt(E_TAG, &SYM_M_HERZ, "Wie war dein Tag?");
          if (gesperrt(pet, MEDIZIN, jetzt) == OK) punkt(E_MEDIZIN, &SYM_M_MEDIZIN, "Medizin");
          const int m = mahlzeitFaellig(pet, jetzt);
          punkt(E_FUETTERN, &SYM_M_FUTTER, m >= 0 ? MAHLZEITEN[m] : "Füttern");
          punkt(E_NASCHEN, &SYM_M_KEKS, "Naschen");
          punkt(E_SPIELEN, &SYM_M_BALL, "Spielen");
          punkt(E_PUTZEN, &SYM_M_PUTZEN, "Saubermachen");
        }
      }
      punkt(E_TON, &SYM_M_TON, tonAn ? "Ton aus" : "Ton an");
    }
    // Uhr stellen, Name ändern und Hilfe sind seit 24.11.2026 nicht mehr im Menü (Wunsch der Nutzerin).
    // Die Uhr wird weiterhin beim Einschalten gestellt, wenn sie nicht läuft.
    punkt(E_SIDEBOARD, &SYM_M_HERZ, "Sideboard öffnen");
    if (gartenMoeglich(jetzt)) punkt(E_GARTEN, &SYM_M_SONNE, "In den Garten");
  }
  gewaehlt = 0;
}

void menueZeigen(bool offen, bool spiele = false, bool tag = false) {
  spieleAuswahl = spiele;
  tagAuswahl = tag;
  if (offen) menueBauen();
  menueOffen = offen;
}

void zeichneMenue() {
  const int w = 150, x = BREITE - 5 - w, unten = ZEILE_Y - 5, zeileH = menueAnzahl > 10 ? 15 : 17;
  const int sichtbar = min(menueAnzahl, 11), anfang = max(0, min(gewaehlt - 5, menueAnzahl - sichtbar));
  const int h = 2 + 3 + sichtbar * zeileH + 3 + 2, y = unten - h;
  bild.fillRoundRect(x + 2, y + 2, w, h, 6, 0);   // Schatten 2px
  bild.fillRoundRect(x, y, w, h, 6, 1);
  bild.drawRoundRect(x, y, w, h, 6, 0);
  bild.drawRoundRect(x + 1, y + 1, w - 2, h - 2, 5, 0);
  for (int i = anfang; i < anfang + sichtbar; i++) {
    const int iy = y + 5 + (i - anfang) * zeileH;
    const bool sel = i == gewaehlt;
    if (sel) bild.fillRect(x + 5, iy, w - 10, zeileH, 0);
    const uint16_t f = sel ? 1 : 0;
    if (menue[i].symbol) {
      const Bild& s = *menue[i].symbol;
      zeichneSymbol(s, x + 10 + (14 - s.w) / 2, iy + (zeileH - s.h) / 2, f);
    } else {
      zeichneZeichen(menue[i].zeichen, x + 17, iy + zeileH / 2, 6, f, sel ? 0 : 1);
    }
    schreibe(bild, x + 10 + 14 + 6, iy + 5, menue[i].text, f, 1, sel);
  }
}

void zeichneZeile(Phase ph, Stimmung s, ms_t jetzt) {
  bild.fillRect(0, ZEILE_Y, BREITE, HOEHE - ZEILE_Y, 1);
  bild.drawFastHLine(0, ZEILE_Y, BREITE, 0);
  char text[100];
  if (menueOffen) {
    snprintf(text, sizeof(text), "%s", tastenDa ? "▲▼ wählen · A OK · B zurück" : "Links weiter · Mitte OK · Rechts zurück");
    schreibe(bild, RAND_LINKS + (BREITE - RAND_LINKS - textBreite(text)) / 2, 216, text, 0);
    return;
  }
  if (spiel.aktiv) spielText(text, sizeof(text));
  else if (millis() < meldungBis) snprintf(text, sizeof(text), "%s", meldung);
  else stimmungsText(s, jetzt, text, sizeof(text));
  // Links: Name, Phase, Münzen
  int x = RAND_LINKS + 7;
  const int y = 216;
  schreibe(bild, x, y, pet.name, 0, 1, true);
  x += textBreite(pet.name) + 1 + 6;
  schreibe(bild, x, y, phasenName(ph), 0);
  x += textBreite(phasenName(ph)) + 6;
  zeichneSymbol(SYM_MUENZE, x, y, 0);
  x += SYM_MUENZE.w + 2;
  char m[12];
  snprintf(m, sizeof(m), "%lu", (unsigned long)pet.muenzen);
  schreibe(bild, x, y, m, 0);
  x += textBreite(m) + 8;
  // Rechts: Meldung, bei Bedarf in zwei Zeilen
  const int platz = (BREITE - 7 - x) / 6;
  if (textLaenge(text) <= platz) {
    schreibe(bild, BREITE - 7 - textBreite(text) + 1, y, text, 0);
  } else {
    char zeilen[3][60];
    int n = 0;
    umbrechen(text, platz, [&](const char* z) { if (n < 3) { strncpy(zeilen[n], z, 59); zeilen[n][59] = 0; n++; } });
    const int y0 = n >= 3 ? 204 : 210;
    for (int i = 0; i < n; i++) schreibe(bild, BREITE - 7 - textBreite(zeilen[i]) + 1, y0 + i * 11, zeilen[i], 0);
  }
}

void zeichneAlles() {
  if (garten.phase != GA_ZU) { gartenZeichnen(); zeigen(); return; }
  if (sideboard.phase == SB_OFFEN) { sammlungZeichnen(); zeigen(); return; }
  const ms_t jetzt = jetztMs();
  const Phase ph = phaseBei(pet, jetzt);
  const Stimmung s = beschreiben(pet, jetzt);
  // Zimmer „Gemütlich & schlicht“ mit Jahreszeit, Wetter und Weihnachtsdeko (umgebung.ino)
  uint8_t* puffer = bild.getBuffer();
  if (spiel.aktiv) {
    spielHintergrund();                            // Spiele nutzen den ganzen Bildschirm
  } else {
    zimmerZeichnen(jetzt);
    momentZeichnenHinten();                        // Vogel und Sternschnuppe im Fenster
  }
  memset(puffer + 40 * 224, 0xFF, 40 * (HOEHE - 224));
  // Licht aus: das Zimmer wird zu 50 % gepunktet, Evi bleibt hell
  if (!spiel.aktiv && s == S_SCHLAEFT && pet.lichtAus) {
    for (int y = 0; y < 224; y++) {
      const uint8_t m = (y % 2) ? 0xAA : 0x55;
      for (int i = 0; i < 40; i++) puffer[y * 40 + i] &= m;
    }
  }
  if (!spiel.aktiv) zeichneUhrzeit(jetzt);         // nach dem Abdunkeln: bleibt hell
  zeichneEvi(jetzt, s, ph);
  zeichneReaktion();
  if (!spiel.aktiv) kuschelHerzenZeichnen();     // aufsteigende Herzen beim Kuscheln (kuscheln.ino)
  if (spiel.aktiv) {
    spielZeichnen();                               // Ball, Pfeile oder Felder liegen vor Evi
  } else {
    bewegungZeichnenVorne();                       // der Ball, der beim Kippen rollt (vor Evi)
    momentZeichnenVorne();                         // Gedankenwolke, Noten, Schmetterling
    if (s == S_SCHLAEFT && !ritual.aktiv) zeichneZ();
    zeichneBlase(jetzt);
  }
  if (ritual.aktiv) zeichneGeschichte();
  if (!spiel.aktiv) zeichneStatus(ph);            // im Spiel tritt die Leiste zur Seite
  zeichneZeile(ph, s, jetzt);
  if (menueOffen) zeichneMenue();
  zeigen();
}

// ---------- Aktionen (performAction in app.js) ----------
void ritualBeginnen() {
  momentStoppen(true);
  ritual.aktiv = true;
  ritual.start = millis();
  ritual.geschichte = random(0, 10);
  ritual.schritt = -1;
}

void ritualBeenden() {
  if (!ritual.aktiv) return;
  ritual.aktiv = false;
  char text[60];
  snprintf(text, sizeof(text), "Psst … %s schläft.", pet.name);
  zeigeMeldung(text);
}

void ritualWeiter() {
  if (!ritual.aktiv) return;
  const uint32_t seit = millis() - ritual.start;
  const int schritt = seit < 2600 ? 0 : seit < 10600 ? 1 : seit < 13200 ? 2 : seit < 15600 ? 3 : 4;
  if (schritt != ritual.schritt) {
    ritual.schritt = schritt;
    if (schritt == 0) reagiere(R_PUTZEN);
    if (schritt == 2) reagiere(R_FUETTERN);
    if (schritt == 4) ritualBeenden();
  }
}

void aktionAusfuehren(Eintrag e) {
  char text[100];
  const ms_t jetzt = jetztMs();
  if (e == E_GARTEN) { gartenOeffnen(); return; }
  if (e == E_SIDEBOARD) { sideboardOeffnen(); return; }
  if (e == E_TAG) { tagFragen(); return; }
  if (e == E_TAG_GUT || e == E_TAG_LALA || e == E_TAG_SCHLECHT) { tagAntwort(e == E_TAG_GUT ? 0 : e == E_TAG_LALA ? 1 : 2); return; }
  if (e == E_TON) {
    tonAn = !tonAn;
    piep(TON(TON_MUENZE));
    zeigeMeldung(tonAn ? "Ton an" : "Ton aus");
    speichern(jetzt);
    return;
  }
  if (e == E_SPIELEN || e == E_BALL || e == E_RATEN || e == E_MELODIE || e == E_KEKSE || e == E_SCHNICK) {
    Tier probe = pet;
    weiterrechnen(probe, jetzt);
    const Grund g = gesperrt(probe, SPIELEN, jetzt);
    if (g != OK && g != G_VOLL) { grundText(g, SPIELEN, text, sizeof(text)); zeigeMeldung(text); return; }
    if (e == E_SPIELEN) menueZeigen(true, true);
    else spielStarten(e == E_BALL ? SPIEL_BALL : e == E_RATEN ? SPIEL_RATEN : e == E_MELODIE ? SPIEL_MELODIE :
                      e == E_KEKSE ? SPIEL_KEKSE : SPIEL_SCHNICK);
    return;
  }
  // Was Evi gerade selbst tut: Jetzt kümmerst du dich um sie.
  momentStoppen(false);
  moment.ruheBisTakt = evi.takt + RUHE_TAKTE;
  const Aktion a = e == E_FUETTERN ? FUETTERN : e == E_NASCHEN ? NASCHEN : e == E_PUTZEN ? PUTZEN : e == E_MEDIZIN ? MEDIZIN :
                   e == E_LICHT ? LICHT : e == E_RITUAL ? RITUAL : e == E_AUFWECKEN ? AUFWECKEN : NEUES_EI;
  const Tier vorher = pet;
  const Ergebnis r = pflegen(pet, a, jetzt);
  melde(vorher, jetzt);
  if (!r.geaendert) {
    grundText(r.grund, a, text, sizeof(text));
    zeigeMeldung(text);
  } else if (a == LICHT) {
    zeigeMeldung(pet.lichtAus ? (r.bonus ? "Gute Nacht! ♥" : "Licht aus") : "Licht an");
  } else if (a == RITUAL) {
    ritualBeginnen();
  } else if (a == AUFWECKEN) {
    snprintf(text, sizeof(text), "Gähn … %s ist wach.", pet.name);
    zeigeMeldung(text, 3000);
  } else if (a == NEUES_EI) {
    zeigeMeldung("Ein neues Ei! Es schlüpft in 30 Minuten.", 5000);
  } else {
    reagiere(a == FUETTERN ? R_FUETTERN : a == NASCHEN ? R_NASCHEN : a == PUTZEN ? R_PUTZEN : R_MEDIZIN);
    snprintf(text, sizeof(text), "%s", a == FUETTERN ? "Mmh, lecker!" : a == NASCHEN ? "Mmh, ein Keks!" : a == PUTZEN ? "Blitzblank!" : "Bäh … aber es hilft!");
    if (r.mahlzeit >= 0) snprintf(text, sizeof(text), "Mmh, %s!", MAHLZEITEN[r.mahlzeit]);
    if (a == NASCHEN && pet.naschZahl == R::NASCHEN_PRO_TAG) snprintf(text, sizeof(text), "Mmh! Jetzt ist aber genug Süßes.");
    if (r.bauchweh) { snprintf(text, sizeof(text), "Aua, Bauchweh! Zu viel genascht."); piep(TON(TON_DANEBEN)); }
    zeigeMeldung(text);
  }
  if (r.wunsch) { sage("Danke!", &SYM_W_HERZ, 3000); piep(TON(TON_MUENZE)); }
  if (r.geaendert) speichern(jetzt);
}

// Die drei Tasten der App
void taste(Taste k) {
  if (garten.phase != GA_ZU) { gartenTaste(k); return; }
  if (sideboard.phase != SB_ZU) { sideboardTaste(k); return; }
  const ms_t jetzt = jetztMs();
  if (geweckt(pet, jetzt)) wachHalten(pet, jetzt);   // touch() in app.js
  if (ritual.aktiv) { ritualBeenden(); return; }
  if (spiel.aktiv) { spielTaste(k == LINKS ? ST_LINKS : k == MITTE ? ST_MITTE : ST_RECHTS); return; }
  if (k == LINKS) {
    if (!menueOffen) { menueZeigen(true); return; }
    gewaehlt = (gewaehlt + 1) % menueAnzahl;
  } else if (k == MITTE) {
    if (!menueOffen) { menueZeigen(true); return; }
    const Eintrag e = menue[gewaehlt].e;
    menueZeigen(false);
    aktionAusfuehren(e);
  } else {
    if (menueOffen && spieleAuswahl) { menueZeigen(true); return; }
    const bool tag = menueOffen && tagAuswahl;
    menueZeigen(false);
    if (tag) tagAntwort(-1);       // „Wie war dein Tag?“ ohne Antwort geschlossen
  }
}

// RESET schaltet den Bildschirm aus. Ein laufendes Spiel wird dabei abgebrochen, Gute Nacht und
// Momente enden. Wieder an geht er mit jeder Taste, jedem Tipper und bei wichtigen Ereignissen.
void bildschirmAus() {
  if (garten.phase != GA_ZU) gartenSchliessen();
  if (sideboard.phase != SB_ZU) sideboardSofortZu();
  if (spiel.aktiv) spielEnde(true);
  if (ritual.aktiv) ritualBeenden();
  momentStoppen(true);
  menueZeigen(false);
  bildschirmAn = false;
  helligkeit(0);
  dunkelSeit = jetztMs();
}

// Die echten Tasten des Retro Gamers
void knopf(Knopf k) {
  if (k == K_RESET) { bildschirmAus(); return; }
  if (garten.phase != GA_ZU) { gartenKnopf(k); return; }
  if (sideboard.phase != SB_ZU) {
    if (k == K_B || k == K_A || k == K_START) sideboardSchliessen();
    else if (k == K_UP || k == K_L) sideboardBlaettern(-1);
    else if (k == K_DOWN || k == K_R) sideboardBlaettern(1);
    return;
  }
  if (spiel.aktiv) {                               // im Spiel gehören die Tasten dem Spiel
    const ms_t jetzt = jetztMs();
    if (geweckt(pet, jetzt)) wachHalten(pet, jetzt);
    spielKnopf(k);
    return;
  }
  if (k == K_A) { taste(MITTE); return; }
  if (k == K_B) { taste(RECHTS); return; }
  const ms_t jetzt = jetztMs();
  if (geweckt(pet, jetzt)) wachHalten(pet, jetzt);
  if (ritual.aktiv) { ritualBeenden(); return; }   // jede Taste überspringt Gute Nacht
  if (k == K_START) {
    const bool tag = menueOffen && tagAuswahl;
    menueZeigen(!menueOffen);
    if (tag) { menueZeigen(false); tagAntwort(-1); }
    return;
  }
  if (!menueOffen || menueAnzahl == 0) return;
  if (k == K_UP) gewaehlt = (gewaehlt + menueAnzahl - 1) % menueAnzahl;
  else if (k == K_DOWN) gewaehlt = (gewaehlt + 1) % menueAnzahl;
}

// ---------- Touch: einzelne Tipper ----------
bool fingerUnten = false;
uint32_t letzteMeldung = 0, fingerSeit = 0;
int fingerX = 0, fingerY = 0;
int letzterTippX = -1, letzterTippY = -1;   // für die Prüfung über USB („T“)
bool fingerHatGeweckt = false;               // dieser Finger hat nur den Bildschirm geweckt: nicht streicheln

bool tipper(int& x, int& y) {
  int tx, ty;
  bool neu = false;
  while (touchLesen(tx, ty)) {
    if (!fingerUnten) { neu = true; x = tx; y = ty; fingerSeit = millis(); letzterTippX = tx; letzterTippY = ty; }
    fingerUnten = true;
    fingerX = tx;
    fingerY = ty;
    letzteMeldung = millis();
  }
  if (fingerUnten && millis() - letzteMeldung > 150) fingerUnten = false;
  return neu;
}

Taste tasteBei(int x) { return x < BREITE / 3 ? LINKS : x < 2 * BREITE / 3 ? MITTE : RECHTS; }

// ---------- Uhr stellen ----------
// Links = weniger, Mitte = nächstes Feld (nach Minute: fertig), Rechts = mehr. Festhalten wiederholt.

void uhrStellenBildschirm(ms_t vorschlag) {
  Ortszeit o = ortszeit(vorschlag);
  int werte[5] = {o.tag, o.monat, o.jahr, o.stunde, o.minute};
  const int mins[5] = {1, 1, 2025, 0, 0}, maxs[5] = {31, 12, 2099, 23, 59};
  int feld = 0;
  uint32_t naechsteWiederholung = 0;
  helligkeit(HELL_PROZENT);
  while (true) {
    tonWeiter();
    int x, y;
    int schritt = 0;
    if (tipper(x, y)) {
      const Taste k = tasteBei(x);
      if (k == MITTE) {
        feld++;
        if (feld >= 5) break;
      } else {
        schritt = k == LINKS ? -1 : 1;
        naechsteWiederholung = millis() + 500;
      }
    } else if (fingerUnten && tasteBei(fingerX) != MITTE && millis() >= naechsteWiederholung) {
      schritt = tasteBei(fingerX) == LINKS ? -1 : 1;
      naechsteWiederholung = millis() + 90;
    }
    // Echte Tasten: hoch/runter ändert (Festhalten zählt schnell), A oder rechts = nächstes Feld,
    // B oder links = Feld zurück
    const int k = naechsteTaste();
    if (k == K_UP) schritt = 1;
    else if (k == K_DOWN) schritt = -1;
    else if (k == K_A || k == K_R || k == K_START) {
      feld++;
      if (feld >= 5) break;
    } else if ((k == K_B || k == K_L) && feld > 0) {
      feld--;
    }
    if (schritt) {
      werte[feld] += schritt;
      if (werte[feld] > maxs[feld]) werte[feld] = mins[feld];
      if (werte[feld] < mins[feld]) werte[feld] = maxs[feld];
    }
    // Tag an Monat anpassen
    const int tageImMonat = (int)(tageAusDatum(werte[1] == 12 ? werte[2] + 1 : werte[2], werte[1] == 12 ? 1 : werte[1] + 1, 1) -
                                  tageAusDatum(werte[2], werte[1], 1));
    if (werte[0] > tageImMonat) werte[0] = tageImMonat;

    bild.fillScreen(1);
    schreibe(bild, (BREITE - textBreite("Datum und Uhrzeit", 2)) / 2, 20, "Datum und Uhrzeit", 0, 2);
    char d[16], z[8];
    snprintf(d, sizeof(d), "%02d.%02d.%04d", werte[0], werte[1], werte[2]);
    snprintf(z, sizeof(z), "%02d:%02d", werte[3], werte[4]);
    const int dx = (BREITE - textBreite(d, 3)) / 2, zx = (BREITE - textBreite(z, 3)) / 2;
    schreibe(bild, dx, 70, d, 0, 3);
    schreibe(bild, zx, 120, z, 0, 3);
    // Markierung unter dem Feld
    const int fx[5] = {dx, dx + 3 * 18, dx + 6 * 18, zx, zx + 3 * 18}, fy[5] = {96, 96, 96, 146, 146};
    const int fb[5] = {2 * 18 - 3, 2 * 18 - 3, 4 * 18 - 3, 2 * 18 - 3, 2 * 18 - 3};
    bild.fillRect(fx[feld], fy[feld], fb[feld], 4, 0);
    bild.drawFastHLine(0, 196, BREITE, 0);
    bild.drawFastVLine(BREITE / 3, 196, 44, 0);
    bild.drawFastVLine(2 * BREITE / 3, 196, 44, 0);
    schreibe(bild, 53 - 6, 206, "-", 0, 3);
    schreibe(bild, 160 - textBreite(feld == 4 ? "Fertig" : "Weiter", 2) / 2, 210, feld == 4 ? "Fertig" : "Weiter", 0, 2);
    schreibe(bild, 267 - 9, 206, "+", 0, 3);
    const char* hinweis = tastenDa ? "▲▼ ändern · A weiter · B zurück" : "Festhalten zählt schnell";
    schreibe(bild, (BREITE - textBreite(hinweis)) / 2, 172, hinweis, 0);
    zeigen();
  }
  const ms_t utc = ausOrtszeit(werte[2], werte[1], werte[0], werte[3], werte[4]);
  uhrStellen(utc);
  uhrSetzen(utc);
  fingerUnten = true;   // den letzten Tipper nicht als Taste werten
  letzteMeldung = millis();
  tastenVergessen();
}

// Uhrzeit beim Übersetzen, als Vorschlag beim ersten Stellen
ms_t uebersetzungsZeit() {
  static const char* MON = "JanFebMarAprMayJunJulAugSepOctNovDec";
  const char* d = __DATE__;   // "Nov 23 2026"
  int monat = 1;
  for (int i = 0; i < 12; i++) if (strncmp(d, MON + 3 * i, 3) == 0) monat = i + 1;
  const int tag = atoi(d + 4), jahr = atoi(d + 7);
  const char* t = __TIME__;   // "18:45:12"
  return ausOrtszeit(jahr, monat, tag, atoi(t), atoi(t + 3));
}

// ---------- Bildschirmfoto über USB (für die Prüfung am PC) ----------
// Sendet nach dem Zeichen 'S' die Zeile "BILD 320 240" und 9600 Bytes (1 = weiß). Nach 'L' eine Zeile zum Lagesensor,
// nach 'T' eine Zeile zum letzten Tipper.
void seriellPruefen() {
  while (Serial.available()) {
    const int c = Serial.read();
    if (c == 'S') {
      Serial.println("BILD 320 240");
      Serial.write(bild.getBuffer(), 40 * 240);
      Serial.flush();
    }
    if (c == 'L') {
      // Lagesensor prüfen (Seriellen Monitor öffnen, „L“ senden): was er misst und was Evi gelernt hat
      char z[200];
      snprintf(z, sizeof(z), "LAGE da=%d adresse=0x%02X bytes=%s 1g=%.3f | x=%.2f y=%.2f z=%.2f g | Bildschirm-Achse=%d(%d) Oberkante=%d(%d) | "
               "Unruhe=%.2f Neigung=%.2f Serie=%d Schritte=%lu",
               lage.da, lage.adresse, lage.grossEndian ? "gross" : "klein", lage.eins, lage.x, lage.y, lage.z,
               lage.nAchse, lage.nVz, lage.uAchse, lage.uVz, lage.unruhe, lageKippen(), lage.serie, (unsigned long)lage.gassiSchritte);
      Serial.println(z);
    }
    if (c == 'T') {
      // Touch prüfen: wo der letzte Tipper war und wo Evi gerade steht
      char z[120];
      snprintf(z, sizeof(z), "TOUCH chip=%s tipp=%d,%d evi=%d..%d,%d..%d", touchName(), letzterTippX, letzterTippY,
               eviLinks, eviLinks + eviBreite - 1, eviOben, BODEN_Y);
      Serial.println(z);
    }
  }
}

// ---------- Start ----------
void setup() {
  Serial.begin(115200);
  pinMode(BAT_EN, OUTPUT);
  digitalWrite(BAT_EN, HIGH);
  bild.cp437(true);
  speicherStarten();
  bildschirmStarten();
  touchStarten();
  tastenStarten();
  lageStarten();                 // Lagesensor QMI8658 (Bewegung)
  tonStarten();
  randomSeed(micros() ^ analogRead(BAT_ADC));

  Tier geladen;
  bool ton = true;
  const bool hatStand = spielstandLaden(geladen, ton);
  tonAn = ton;

  ms_t utc;
  const ms_t vorschlag = hatStand && geladen.aktualisiert > uebersetzungsZeit() ? geladen.aktualisiert : uebersetzungsZeit();
  if (uhrLesen(utc) && (!hatStand || utc >= geladen.aktualisiert - TAG)) {
    uhrSetzen(utc);
  } else {
    uhrStellenBildschirm(vorschlag);
  }
  const ms_t jetzt = jetztMs();

  if (hatStand) {
    pet = geladen;
    const Tier vorher = pet;
    weiterrechnen(pet, jetzt);
    // Was passiert ist, während Evi aus war (missedEvents in app.js)
    const Phase von = phaseBei(vorher, vorher.aktualisiert), zu = phaseBei(pet, jetzt);
    char text[90] = "";
    if (zu == TOT && von != TOT) snprintf(text, sizeof(text), pet.ursache == ALTER ? "%s ist friedlich eingeschlafen, während du weg warst." : "%s ist gestorben, während du weg warst.", pet.name);
    else if (von == EI && zu != EI) snprintf(text, sizeof(text), "%s ist geschlüpft, während du weg warst.", pet.name);
    else if (von != zu && zu == TEEN) snprintf(text, sizeof(text), "%s ist jetzt ein Teenager!", pet.name);
    else if (von != zu && zu == ERWACHSEN) snprintf(text, sizeof(text), "%s ist erwachsen geworden!", pet.name);
    else if (von != zu && zu == OMA) snprintf(text, sizeof(text), "%s ist jetzt eine Oma!", pet.name);
    else if (!vorher.krank && pet.krank) snprintf(text, sizeof(text), "%s ist krank geworden.", pet.name);
    if (text[0]) zeigeMeldung(text, 8000);
  } else {
    pet = neuesTier(jetzt, ERSTES_EI);
  }
  himmel = himmelBei(jetzt);
  speichern(jetzt);
  letzteRunde = millis();
  moment.naechster = jetzt + naechsterAbstand();
  if (hatStand) begruessenPlanen(geladen.aktualisiert);   // wie beim Öffnen der App: Evi begrüßt dich
  bildschirmEin();
  zeichneAlles();
}

void loop() {
  tonWeiter();
  seriellPruefen();
  bewegungTakt();                 // Lagesensor: Gassi, kippen, aufs Gesicht
  sideboardTakt(jetztMs());
  gartenTakt(jetztMs());

  // Touch: Seit 24.11.2026 (Wunsch der Nutzerin) keine Tasten mehr, sondern Evi antippen oder streicheln = kuscheln.
  // Nur wenn die echten Tasten nicht gefunden werden, gelten als Notlösung die drei Drittel wie früher.
  int x, y;
  if (tipper(x, y)) {
    lageBeimBedienen();          // wer tippt, hält das Gerät mit dem Bildschirm nach oben
    fingerHatGeweckt = !bildschirmAn;
    if (!bildschirmAn) { bildschirmEin(); aufgeweckt(); }   // der erste Tipper weckt nur den Bildschirm
    else {
      bildschirmEin();
      if (tastenDa) beruehrt(x, y);
      else taste(tasteBei(x));
    }
  } else if (fingerUnten && bildschirmAn && tastenDa && !fingerHatGeweckt) {
    streicheln(fingerX, fingerY);                           // Finger bleibt auf Evi: weiter kuscheln
  }

  const int k = naechsteTaste();
  if (k >= 0) {
    lageBeimBedienen();
    if (!bildschirmAn) { bildschirmEin(); aufgeweckt(); }   // der erste Druck weckt nur den Bildschirm
    else {
      bildschirmEin();
      knopf((Knopf)k);
    }
  }

  spielTakt();
  const uint32_t m = millis();
  if (m - letzterTakt >= TAKT_MS) {
    letzterTakt = m;
    const ms_t jetzt = jetztMs();
    eviTakt(jetzt);
    ritualWeiter();
    if (sideboard.phase == SB_ZU && garten.phase == GA_ZU) momentTakt(jetzt);
    kuschelTakt();
    if (m - letzteRunde >= 10000) { letzteRunde = m; runde(); }
    // Von selbst dunkel nur, wenn DUNKEL_NACH_MS > 0 (und nie mitten in Gute Nacht, einem Spiel oder einem Moment)
    if (DUNKEL_NACH_MS > 0 && bildschirmAn && m - letzteBeruehrung > DUNKEL_NACH_MS && !ritual.aktiv && !spiel.aktiv &&
        moment.art == M_KEINER) {
      bildschirmAn = false;
      menueZeigen(false);
      helligkeit(0);
      dunkelSeit = jetzt;
    }
    if (bildschirmAn) { zeichneAlles(); letztesBild = millis(); }
  } else if (bildschirmAn && ((spiel.aktiv && m - letztesBild >= 12) ||
             (!spiel.aktiv && sideboard.phase != SB_OFFEN && m - letztesBild >= 40 &&
              (garten.phase != GA_ZU || sideboard.phase != SB_ZU || wetterBei(jetztMs()) == WT_REGEN || wetterBei(jetztMs()) == WT_SCHNEE)))) {
    // Spiele etwa 80-mal, Regen und Schnee etwa 25-mal pro Sekunde zeichnen.
    // Spielregeln und Evis Momente behalten ihren bisherigen Takt; gesendet werden nur Änderungen.
    zeichneAlles();
    letztesBild = millis();
  }
}
