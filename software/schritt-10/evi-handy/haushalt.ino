// Haushalt: Verlieben, Partner, Ei, Baby, das Tamagotchi davor (26.11.2026). Siehe haushalt.h.

// ---------- Speichern ----------
void haushaltLeeren() {
  haushalt = HaushaltStand();
  haushalt.kennung = HAUSHALT_KENNUNG;
  haushalt.version = HAUSHALT_VERSION;
  haushalt.groesse = sizeof(HaushaltStand);
  haushalt.partner = 255;
  haushalt.partnerName = 255;
}

void haushaltLaden() {
  uint8_t* b = (uint8_t*)&haushalt;
  for (size_t i = 0; i < sizeof(haushalt); i++) b[i] = EEPROM.read(HAUSHALT_ADRESSE + i);
  const bool gut = haushalt.kennung == HAUSHALT_KENNUNG && haushalt.version == HAUSHALT_VERSION &&
                   haushalt.groesse == sizeof(HaushaltStand) &&
                   haushalt.pruefsumme == pruefsumme(b, offsetof(HaushaltStand, pruefsumme)) &&
                   (haushalt.partner < BESUCHER || haushalt.partner == 255) && haushalt.eiZustand <= 1;
  if (!gut) haushaltLeeren();
  haushaltGeaendert = false;
}

void haushaltSpeichern() {
  if (!haushaltGeaendert) return;
  haushalt.pruefsumme = pruefsumme((const uint8_t*)&haushalt, offsetof(HaushaltStand, pruefsumme));
  const uint8_t* b = (const uint8_t*)&haushalt;
  bool anders = false;
  for (size_t i = 0; i < sizeof(haushalt); i++) {
    if (EEPROM.read(HAUSHALT_ADRESSE + i) != b[i]) { EEPROM.write(HAUSHALT_ADRESSE + i, b[i]); anders = true; }
  }
  if (anders) EEPROM.commit();
  haushaltGeaendert = false;
}

// ---------- Zeiten ----------
ms_t erwachsenEnde(const Tier& t) { return t.schluepft + R::BABY + R::TEEN + R::ERWACHSEN; }
bool istErwachsen(const Tier& t, ms_t jetzt) { return !t.tot && phaseBei(t, jetzt) == ERWACHSEN; }

// Das Ei: am Morgen/Vormittag (8–11 Uhr) des letzten erwachsenen Tages, spätestens 20 Minuten nach dem Einzug des Partners.
// 0 = es bleibt keine Zeit mehr für ein Ei.
ms_t eiZeitPlanen(ms_t jetzt) {
  const ms_t ende = erwachsenEnde(pet), start = ende - TAG;
  const Ortszeit o = ortszeit(start);
  char s[40];
  snprintf(s, sizeof(s), "%lld:eizeit", (long long)pet.geboren);
  ms_t morgen = ausOrtszeit(o.jahr, o.monat, o.tag, 8, 0) + (ms_t)(streuwert(s) % 180) * MINUTE;
  if (morgen < start) morgen += TAG;
  const ms_t t = max(morgen, jetzt + 20 * MINUTE);
  return t > ende - 10 * MINUTE ? 0 : t;
}

bool gegengeschlecht(int f) { return stamm.geschlechtAktuell == 0 ? !figurWeiblich(f) : figurWeiblich(f); }

// ---------- Die Frage „Partner?“ (Freundschaft, Klopfen) und „Begraben?“ ----------
void liebeStarten(uint8_t art, int f, ParkPhase zurueck) {
  park.phase = PARK_LIEBE;
  park.liebeArt = art;
  park.liebeKandidat = (uint8_t)f;
  park.liebeWahl = 0;
  park.liebeZurueck = zurueck;
}

bool liebeMoeglich(int f, ms_t jetzt) {
  if (!pet.name[0] || haushalt.partner != 255 || haushalt.eiZustand || !istErwachsen(pet, jetzt)) return false;
  if (!gegengeschlecht(f) || parkFreund.treffer[f] < TREFFEN_BESTE) return false;
  if (jetzt > erwachsenEnde(pet) - STUNDE) return false;
  if (haushalt.abgelehntTag == tagKennung(jetzt) && (haushalt.abgelehntLiebe >> f & 1)) return false;
  return true;
}

// Nach einem Treffen im Park oder einem Besuch: Wenn es passt, kommt die Frage. true = die Frage ist offen.
bool liebeAngebot(int f, ParkPhase zurueck, ms_t jetzt) {
  if (!liebeMoeglich(f, jetzt)) return false;
  liebeStarten(0, f, zurueck);
  piep(TON(TON_WUNSCH));
  return true;
}

void liebeAbschluss() {
  if (park.liebeZurueck == PARK_ZU) { parkSchliessen(); return; }
  park.phase = park.liebeZurueck;
}

void liebeJa(ms_t jetzt) {
  const int f = park.liebeKandidat;
  if (park.liebeArt == 2) {                      // Begraben
    haushalt.vorigeDa = 0;
    haushaltGeaendert = true;
    haushaltSpeichern();
    piep(TON(TON_TRAURIG));
    liebeAbschluss();
    return;
  }
  haushalt.partner = (uint8_t)f;
  haushalt.partnerName = besuchNameCode(f, pet);
  haushalt.partnerSeit = jetzt;
  haushalt.eiGeplant = eiZeitPlanen(jetzt);
  haushalt.eiZustand = 0;
  haushalt.abgelehntKlopf = 0;
  haushaltGeaendert = true;
  partnerB.x = 285; partnerB.fuss = 194; partnerB.modus = 0; partnerB.bis = 0;     // kommt zur Tür herein
  speichern(jetzt);
  piep(TON(TON_MUENZE));
  static char text[80];
  snprintf(text, sizeof(text), "%s zieht bei dir ein! ♥", nameVonCode(haushalt.partnerName));
  liebeAbschluss();
  zeigeMeldung(text, 6000);
}

void liebeNein(ms_t jetzt) {
  const int f = park.liebeKandidat;
  if (park.liebeArt == 1) haushalt.abgelehntKlopf |= 1UL << f;
  else if (park.liebeArt == 0) { haushalt.abgelehntLiebe |= 1UL << f; haushalt.abgelehntTag = tagKennung(jetzt); }
  haushaltGeaendert = true;
  haushaltSpeichern();
  liebeAbschluss();
}

void liebeKnopf(Knopf k) {
  const ms_t jetzt = jetztMs();
  if (k == K_L || k == K_R || k == K_UP || k == K_DOWN) park.liebeWahl ^= 1;
  else if (k == K_A) { if (park.liebeWahl == 0) liebeJa(jetzt); else liebeNein(jetzt); }
  else if (k == K_B || k == K_START) liebeNein(jetzt);
}

void liebeTouch(int x, int y) {
  if (y < 150 || y > ZEILE_Y) return;
  if (x < 160) liebeJa(jetztMs()); else liebeNein(jetztMs());
}

// Eine Figur ohne Möbel davor (Bild 72 × 64, Standlinie auf fuss) oder im Zimmer hinter Möbeln verdeckt
static void bildFigurBesuch(int f, Phase ph, int cx, int fuss, bool spiegeln, bool tiefentest) {
  const BesuchBild& b = BESUCH_BILD[f][ph - BABY];
  ellipseFuellen(cx, fuss, 18, 2, 119);
  const int oy = fuss - BESUCH_STANDLINIE + b.y;
  for (int y = 0; y < b.bild.h; y++) for (int x = 0; x < b.bild.w; x++) {
    if (!b.bild.deckt(x, y)) continue;
    const int cxPix = spiegeln ? BESUCH_BREITE - 1 - (b.x + x) : b.x + x;
    const int px = cx - BESUCH_BREITE / 2 + cxPix, py = oy + y;
    if (tiefentest && raumVerdeckt(false, fuss, px, py)) continue;
    bild.drawPixel(px, py, b.bild.wert(x, y));
  }
}

void liebeZeichnen(ms_t jetzt) {
  const Phase ph = phaseBei(pet, jetzt);
  bild.fillScreen(1);
  zeichneStatus(ph == EI || ph == TOT ? BABY : ph);
  char z1[80], z2[80];
  const char* ja = "Ja, einziehen!", *nein = "Nein, danke";
  if (park.liebeArt == 2) {
    snprintf(z1, sizeof(z1), "%s ist friedlich eingeschlafen.", haushalt.vorigeName);
    snprintf(z2, sizeof(z2), "Soll %s begraben werden?", haushalt.vorigeName);
    ja = "Begraben"; nein = "Noch nicht";
    const Phase mp = OMA;
    const Bild* b = ART[mp][7] ? ART[mp][7] : ART[mp][1];
    const int h = 70, w = max(1, (int)b->w * h / b->h);
    raumRasterZeichnen(*b, 160 - w / 2, 148 - h, w, h, false, false, false, 0);
  } else {
    const char* name = nameVonCode(park.liebeArt == 1 ? besuchNameCode(park.liebeKandidat, pet) : besuchNameCode(park.liebeKandidat, pet));
    if (park.liebeArt == 1) snprintf(z1, sizeof(z1), "Es klopft! Vor der Tür steht %s.", name);
    else snprintf(z1, sizeof(z1), "%s und %s sind beste Freunde.", name, tamaName());
    snprintf(z2, sizeof(z2), "Soll %s dein Partner werden?", name);
    const Bild* b = ART[ph == ERWACHSEN || ph == OMA || ph == TEEN || ph == BABY ? ph : ERWACHSEN][1];
    const int h = 72, w = max(1, (int)b->w * h / b->h);
    ellipseFuellen(95, 148, w / 4, 2, 119);
    raumRasterZeichnen(*b, 95 - w / 2, 148 - h, w, h, true, false, false, 0);
    bildFigurBesuch(park.liebeKandidat, ERWACHSEN, 225, 148, false, false);
  }
  schreibe(bild, (BREITE - textBreite(z1)) / 2, STATUS_H + 6, z1, 0);
  schreibe(bild, (BREITE - textBreite(z2)) / 2, STATUS_H + 20, z2, 0);
  for (int i = 0; i < 2; i++) {
    const int x = i ? 170 : 10, w = 140, y = 158;
    const bool sel = park.liebeWahl == i;
    if (sel) bild.fillRoundRect(x, y, w, 26, 6, 0); else bild.drawRoundRect(x, y, w, 26, 6, 0);
    const char* t = i ? nein : ja;
    schreibe(bild, x + (w - textBreite(t, 1)) / 2, y + 9, t, sel ? 1 : 0);
  }
  bild.fillRect(0, ZEILE_Y, BREITE, HOEHE - ZEILE_Y, 1);
  bild.drawFastHLine(0, ZEILE_Y, BREITE, 0);
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 6, park.liebeArt == 2 ? "Dann ist sie ganz aus dem Zimmer verschwunden." : "Der Partner zieht ein und versorgt sich selbst.", 0);
  schreibe(bild, RAND_LINKS + 3, ZEILE_Y + 22, tastenDa ? "◀▶ wählen · A bestätigen · B Nein" : "Tippe auf deine Antwort.", 0);
  zeigen();
}

// ---------- Klopfen: Am letzten erwachsenen Tag ohne Partner alle zwei Stunden ----------
void klopfenPruefen(ms_t jetzt) {
  if (haushalt.partner != 255 || haushalt.eiZustand || !istErwachsen(pet, jetzt)) return;
  const ms_t ende = erwachsenEnde(pet);
  if (jetzt < ende - TAG || jetzt > ende - STUNDE) return;
  const Ortszeit o = ortszeit(jetzt);
  if (o.stunde < 9 || o.stunde >= 21) return;                       // nur tagsüber
  const int slot = (o.stunde - 9) / 2;                              // 9, 11, 13, 15, 17, 19 Uhr
  const int32_t heute = tagKennung(jetzt);
  if (haushalt.klopfTag != heute) { haushalt.klopfTag = heute; haushalt.klopfErledigt = 0; haushaltGeaendert = true; }
  if (haushalt.klopfErledigt >> slot & 1) return;
  if (parkOffen() || zimmerEditor.offen || sideboard.phase != SB_ZU || spiel.aktiv || ritual.aktiv || gespraech.gebDialog ||
      freundFrageOffen() || !pet.name[0]) return;                   // gerade beschäftigt: später im selben Zeitfenster
  // Ein Kandidat des anderen Geschlechts, der noch nicht abgelehnt wurde
  int kand[BESUCHER], n = 0;
  for (int pass = 0; pass < 2 && !n; pass++) {
    for (int f = 0; f < BESUCHER; f++) if (gegengeschlecht(f) && !(haushalt.abgelehntKlopf >> f & 1)) kand[n++] = f;
    if (!n) haushalt.abgelehntKlopf = 0;                            // alle abgelehnt: wieder von vorn
  }
  if (!n) return;
  const int f = kand[random(0, n)];
  haushalt.klopfErledigt |= 1 << slot;
  haushaltGeaendert = true;
  haushaltSpeichern();
  bildschirmEin();
  momentStoppen(true);
  menueZeigen(false);
  kuschel = {};
  evi.reaktion = R_KEINE;
  sagen.bis = 0;
  park = ParkZustand();
  liebeStarten(1, f, PARK_ZU);
  piep(TON(TON_KLOPF));
}

// ---------- Ei und Baby ----------
void haushaltSchluepfen(ms_t jetzt) {
  parkSchliessen();
  const Tier alt = pet;
  const uint8_t geschlecht = stamm.geschlechtAktuell;
  // Das bisherige Tamagotchi kommt mit seinem ganzen Leben in den Stammbaum und lebt im Haus weiter
  stammEintragenKern(alt, R::LEBENSDAUER, jetzt, haushalt.partner, haushalt.partnerName);
  haushalt.vorigeDa = 1;
  haushalt.vorigeGeschlecht = geschlecht;
  haushalt.vorigeGemeldet = 0;
  haushalt.vorigeGeboren = alt.geboren;
  haushalt.vorigeSchluepft = alt.schluepft;
  stammNameKuerzen(alt.name[0] ? alt.name : "Tamagotchi", haushalt.vorigeName, sizeof(haushalt.vorigeName));
  haushalt.partner = 255;
  haushalt.partnerName = 255;
  haushalt.eiZustand = 0;
  haushalt.eiGeplant = 0;
  haushalt.abgelehntKlopf = 0;
  haushaltGeaendert = true;
  vorigeB.x = 200; vorigeB.fuss = 190; vorigeB.modus = 0; vorigeB.bis = 0;
  // Das Baby ist das neue Tamagotchi der Nutzerin
  pet = neuesTier(jetzt, 0, "", false, alt.muenzen, alt.generation + 1);
  sammlung = Sammlung();
  momentStoppen(true);
  sagen.bis = 0;
  evi.reaktion = R_KEINE;
  evi.schluepfenBis = evi.takt + 16;
  evi.schluepfen = true;
  bildschirmEin();
  piep(TON(TON_RISS));
  speichern(jetzt);
}

// Alle 10 Sekunden (und beim Start): Ei legen und schlüpfen, Partner, das Tamagotchi davor
void haushaltTakt(ms_t jetzt) {
  // Das Tamagotchi davor: lebt weiter, stirbt nach seiner Lebenszeit, liegt 15 Minuten, verschwindet
  if (haushalt.vorigeDa) {
    const ms_t tod = haushalt.vorigeSchluepft + R::LEBENSDAUER;
    if (jetzt >= tod + LIEGT_NACH_TOD) { haushalt.vorigeDa = 0; haushaltGeaendert = true; }
    else if (jetzt >= tod && !haushalt.vorigeGemeldet && bildschirmAn && !parkOffen()) {
      haushalt.vorigeGemeldet = 1;
      haushaltGeaendert = true;
      static char text[80];
      snprintf(text, sizeof(text), "%s ist friedlich eingeschlafen.", haushalt.vorigeName);
      zeigeMeldung(text, 7000);
      piep(TON(TON_TRAURIG));
    }
  }
  const bool erw = istErwachsen(pet, jetzt);
  if (haushalt.partner != 255) {
    const ms_t ende = erwachsenEnde(pet);
    if (haushalt.eiZustand == 1) {
      if (jetzt >= haushalt.eiSchluepftAm) haushaltSchluepfen(jetzt);
    } else if (!erw || jetzt >= ende) {
      // Es kam kein Ei mehr zustande: Der Partner zieht wieder aus
      if (bildschirmAn) {
        static char text[80];
        snprintf(text, sizeof(text), "%s ist wieder ausgezogen.", nameVonCode(haushalt.partnerName));
        zeigeMeldung(text, 5000);
      }
      haushalt.partner = 255; haushalt.partnerName = 255; haushalt.eiGeplant = 0;
      haushaltGeaendert = true;
    } else if (haushalt.eiGeplant && jetzt >= haushalt.eiGeplant && jetzt < ende) {
      haushalt.eiZustand = 1;
      haushalt.eiSchluepftAm = jetzt + EI_HUEPFT;
      haushaltGeaendert = true;
      if (bildschirmAn) { zeigeMeldung("Ein Ei! Es hüpft gleich …", 5000); piep(TON(TON_WUNSCH)); }
    }
  }
  klopfenPruefen(jetzt);
  haushaltSpeichern();
}

// ---------- Bewohner im Zimmer ----------
static void bewohnerZiel(Bewohner& b, bool nachts, ms_t jetzt) {
  const auto& t = einrichtung.platz[Z_TEPPICH];
  const int tx = t.aufgestellt ? t.x + ZG_TEPPICH.w / 2 : 120, tf = t.aufgestellt ? t.fuss - 2 : 190;
  if (nachts) { b.zielX = tx; b.zielFuss = tf; b.modus = 1; b.bis = millis() + 60000; return; }
  const int wurf = random(0, 10);
  if (wurf < 4) { b.modus = 0; b.bis = millis() + random(3000, 9000); return; }
  if (wurf < 6) { b.zielX = 72; b.zielFuss = 162; b.modus = 2; b.bis = millis() + 12000; return; }   // aus dem Fenster schauen
  for (int i = 0; i < 12; i++) {
    const int x = random(40, 290), f = random(150, 196);
    if (zimmerBodenFrei(einrichtung, x, f, false)) { b.zielX = x; b.zielFuss = f; b.modus = 1; b.bis = millis() + 9000; return; }
  }
  b.zielX = tx; b.zielFuss = tf; b.modus = 1; b.bis = millis() + 9000;
}

static void bewohnerBewegen(Bewohner& b, bool nachts, bool liegt, ms_t jetzt) {
  if (liegt) { b.modus = 3; return; }
  if (b.modus == 0 || (int32_t)(millis() - b.bis) >= 0) {
    if (b.modus == 0 && (int32_t)(millis() - b.bis) < 0) return;
    bewohnerZiel(b, nachts, jetzt);
  }
  if (b.modus == 1 || b.modus == 2) {
    const int dx = max(-4, min(4, b.zielX - b.x)), dy = max(-3, min(3, b.zielFuss - b.fuss));
    if (dx) b.flip = dx > 0;
    b.x += dx; b.fuss += dy;
    if (!dx && !dy) {                        // angekommen
      if (b.modus == 1 && nachts) b.modus = 3;                       // auf dem Teppich einschlafen
      else if (b.modus == 1) { b.modus = 0; b.bis = millis() + random(3000, 8000); }
      else { b.flip = false; }               // am Fenster: bleibt stehen, bis die Zeit um ist
    }
  } else if (b.modus == 3 && !nachts) {
    b.modus = 0; b.bis = 0;
  }
}

// Jeder Takt (120 ms)
void haushaltBewegen(ms_t jetzt) {
  const bool nachts = schlafenszeit(pet, jetzt);
  if (haushalt.partner != 255) bewohnerBewegen(partnerB, nachts, false, jetzt);
  if (haushalt.vorigeDa) {
    const ms_t tod = haushalt.vorigeSchluepft + R::LEBENSDAUER;
    bewohnerBewegen(vorigeB, nachts, jetzt >= tod, jetzt);
  }
}

// Das Tamagotchi davor: Phase nach seinem Alter
static Phase vorigePhase(ms_t jetzt) {
  Tier t;
  memset(&t, 0, sizeof(t));
  t.schluepft = haushalt.vorigeSchluepft;
  const ms_t alter = jetzt - t.schluepft;
  return alter >= R::LEBENSDAUER ? OMA : phaseBei(t, jetzt);
}

static int bewohnerHopf(const Bewohner& b) {
  const uint32_t seit = millis() - b.reaktionSeit;
  return b.reaktionSeit && seit < 700 ? (seit / 140 % 2 ? 7 : 0) : 0;
}

void haushaltZeichnen(ms_t jetzt) {
  // Das Ei
  if (haushalt.eiZustand == 1) {
    const int fuss = 192, x = 230;
    const int w = (millis() / 160) % 4;
    const Bild& ei = w == 1 ? ART_EI_RECHTS : w == 3 ? ART_EI_LINKS : *ART[EI][0];
    const int h = raumFigurHoehe(ei, EI, fuss, false, false), bw = max(1, (int)ei.w * h / ei.h);
    const int huepf = (millis() / 140) % 4 == 0 ? 6 : 0;
    ellipseFuellen(x, fuss, bw / 3, 2, 119);
    raumRasterZeichnen(ei, x - bw / 2, fuss - h - huepf, bw, h, false, true, false, fuss);
  }
  // Der Partner (eine Figur aus dem Park, Originalgröße)
  if (haushalt.partner != 255) {
    Bewohner& b = partnerB;
    const int lift = bewohnerHopf(b);
    bildFigurBesuch(haushalt.partner, ERWACHSEN, b.x, b.fuss - lift, b.flip, true);
    const BesuchBild& bb = BESUCH_BILD[haushalt.partner][ERWACHSEN - BABY];
    b.links = b.x - BESUCH_BREITE / 2 + bb.x; b.breite = bb.bild.w; b.oben = b.fuss - BESUCH_STANDLINIE + bb.y; b.hoehe = bb.bild.h;
    if (b.modus == 3) schreibe(bild, b.x + 8, b.oben - 6, "z", 0, 1, true);
  }
  // Das Tamagotchi davor (Evi-Zeichnung seiner Lebensphase)
  if (haushalt.vorigeDa) {
    Bewohner& b = vorigeB;
    const ms_t tod = haushalt.vorigeSchluepft + R::LEBENSDAUER;
    const Phase ph = vorigePhase(jetzt);
    const bool liegt = jetzt >= tod || b.modus == 3;
    const Bild* bi = liegt && ART[ph][7] ? ART[ph][7] : ART[ph][1];
    const int h = raumFigurHoehe(*bi, ph, b.fuss, false, false), w = max(1, (int)bi->w * h / bi->h);
    const int lift = jetzt >= tod ? 0 : bewohnerHopf(b);
    ellipseFuellen(b.x, b.fuss, w / 3, 2, 119);
    raumRasterZeichnen(*bi, b.x - w / 2, b.fuss - h - lift, w, h, b.flip, true, false, b.fuss);
    b.links = b.x - w / 2; b.oben = b.fuss - h; b.breite = w; b.hoehe = h;
    if (b.modus == 3 && jetzt < tod) schreibe(bild, b.x + 10, b.oben - 6, "z", 0, 1, true);
  }
}

static const char* const PARTNER_SPRUECHE[6] = {"Schön, dass du da bist!", "Ich fühl mich hier richtig wohl.", "Wollen wir zusammen aus dem Fenster schauen?",
                                               "Ich pass auf, dass es dir gut geht.", "Du hast ein schönes Zuhause.", "Hihi, du bist die Beste!"};
static const char* const VORIGE_SPRUECHE[6] = {"Na, mein Schatz? Alles gut?", "Früher war alles besser … hihi.", "Komm, setz dich zu mir.",
                                              "Das Baby wächst so schnell!", "Ich ruh mich nur ein bisschen aus.", "Du machst das wunderbar!"};

// Antippen eines Bewohners im Zimmer: Er reagiert kurz. Die Oma, die gestorben ist, kann man begraben.
bool haushaltTippen(int x, int y, ms_t jetzt) {
  static char text[64];
  if (haushalt.vorigeDa) {
    const Bewohner& b = vorigeB;
    if (x >= b.links - 4 && x < b.links + b.breite + 4 && y >= b.oben - 4 && y < b.oben + b.hoehe + 6 && b.breite) {
      if (jetzt >= haushalt.vorigeSchluepft + R::LEBENSDAUER) {
        park = ParkZustand();
        liebeStarten(2, 0, PARK_ZU);
      } else {
        vorigeB.reaktionSeit = millis();
        snprintf(text, sizeof(text), "%s: %s", haushalt.vorigeName, VORIGE_SPRUECHE[random(0, 6)]);
        sage(text, &SYM_W_HERZ, 3500);
      }
      return true;
    }
  }
  if (haushalt.partner != 255) {
    const Bewohner& b = partnerB;
    if (b.breite && x >= b.links - 4 && x < b.links + b.breite + 4 && y >= b.oben - 4 && y < b.oben + b.hoehe + 6) {
      partnerB.reaktionSeit = millis();
      snprintf(text, sizeof(text), "%s: %s", nameVonCode(haushalt.partnerName), PARTNER_SPRUECHE[random(0, 6)]);
      sage(text, &SYM_W_HERZ, 3500);
      return true;
    }
  }
  return false;
}
