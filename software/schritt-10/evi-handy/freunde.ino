// Freundschaft, Fragen und Plaudern (Fragerunde mit der Nutzerin vom 25.11.2026, nur auf dem Board).
//
//  - Evi fragt dich von selbst etwas, mit Piep, zu wechselnden Uhrzeiten: als Baby etwa 4-mal am Tag,
//    als Teenager 2-mal, als Erwachsene und Oma einmal. Die Frage bleibt 5 Minuten stehen, A antwortet.
//    Jede Altersstufe hat eigene Fragen. Nach deiner Antwort verrät sie ihre eigene („Wir sind gleich!“).
//  - „Plaudern“ im Menü: Evi erzählt von sich (ihre Antworten, Meinungen, Geheimnisse, Träume) oder erinnert
//    sich an deine Antworten. Je enger die Freundschaft, desto persönlicher.
//  - Jede Evi ist anders: Ihre Antworten und Meinungen werden aus ihrem Zeitpunkt „geboren“ ausgewürfelt.
//    Deine Antworten gelten nur für diese Evi; dein Name und dein Geburtstag bleiben für immer.
//  - Verborgene Freundschaft (0–100) in 5 Stufen: schüchtern, bekannt, vertraut, Freunde, beste Freunde.
//    Steigt durch Antworten, Plaudern, Pflege, Kuscheln, Spiele und erfüllte Launenwünsche (höchstens
//    PLUS_PRO_TAG am Tag), sinkt durch Beleidigtsein, lange niedrige Balken und Krankheit.
//  - Beste Freunde: ein großer Moment, frühestens als Erwachsene, nur wenn du gerade da bist. Kann
//    verloren gehen („Ich dachte, wir wären beste Freunde …“) und muss dann neu verdient werden.
//  - Ab „Freunde“: „Weißt du noch?“ (sie fragt ab, was sie dir erzählt hat) und kleine Geschenke.
//  - Dein Geburtstag wird einmal per Steuerkreuz abgefragt; an dem Tag feiert Evi dich.
// Zu jedem Anlass gibt es fünf Sätze, einer wird zufällig gewählt.

const char* const NUTZER_NAME = "Maggie";
const uint32_t FREUND_KENNUNG = 0x46524E44;   // "FRND"
const uint16_t FREUND_VERSION = 1;

// Wie schnell es geht
const float PLUS_PRO_TAG = 12;                          // höchstens so viel Freundschaft am Tag
const float PLUS_ANTWORT = 4, PLUS_GLEICH = 2, PLUS_WEISST = 6, PLUS_PLAUDERN = 1, PLUS_PFLEGE = 0.5f,
            PLUS_KUSCHELN = 0.5f, PLUS_SPIEL = 1, PLUS_LAUNENWUNSCH = 2, PLUS_GEBURTSTAG = 5;
const float MINUS_BELEIDIGT = 4, MINUS_KRANK = 8, MINUS_NIEDRIG_JE_STUNDE = 2;
const float STUFE_AB[5] = {0, 6, 18, 36, 70};           // Stufe 1–5
const float BESTE_WEG_UNTER = 62;
const uint8_t PLAUDERN_MIT_WIRKUNG = 3;                 // so oft am Tag bringt Plaudern etwas
const uint32_t FRAGE_STEHT = 5UL * 60 * 1000;           // ms
const uint32_t DA_GERADE = 2UL * 60 * 1000;             // ms: jemand hat gerade bedient

// ---------- Fragen (Nummern nie ändern, sie stehen im Spielstand) ----------
// ich: Evis eigene Antwort, deine: sie erinnert sich an deine, weisst: „Weißt du noch?“
struct FrageText { uint8_t phase; const char* frage; uint8_t n; const char* antw[4]; const char* ich; const char* weisst; const char* deine; };
const FrageText FRAGEN[] = {
  // Baby
  {BABY, "Was isst du am liebsten?", 4, {"Nudeln", "Pizza", "Obst", "Schokolade"},
   "Ich mag am liebsten %s!", "Weißt du noch, was ich gern esse?", "Du isst doch so gern %s!"},
  {BABY, "Welche Tiere magst du?", 4, {"Hunde", "Katzen", "Vögel", "Pferde"},
   "Ich mag %s am liebsten!", "Weißt du noch, welche Tiere ich mag?", "Du magst doch %s so gern!"},
  {BABY, "Was ist deine Lieblingsfarbe?", 4, {"Rot", "Blau", "Gelb", "Lila"},
   "Meine Lieblingsfarbe ist %s!", "Weißt du noch meine Lieblingsfarbe?", "Deine Lieblingsfarbe ist %s!"},
  {BABY, "Was machst du am liebsten?", 4, {"Malen", "Singen", "Toben", "Kuscheln"},
   "Ich mag am liebsten %s!", "Weißt du noch, was ich gern mache?", "Du magst doch %s so gern!"},
  {BABY, "Wovor hast du Angst?", 4, {"Gewitter", "Spinnen", "Dunkelheit", "Monstern"},
   "Ich hab Angst vor %s!", "Weißt du noch, wovor ich Angst hab?", "Du hast doch Angst vor %s …"},
  {BABY, "Welches Wetter magst du?", 4, {"Sonne", "Regen", "Schnee", "Wind"},
   "Ich mag %s am liebsten!", "Weißt du noch, welches Wetter ich mag?", "Du magst doch %s so gern!"},
  {BABY, "Wo würdest du gern wohnen?", 4, {"im Wald", "am Meer", "auf dem Mond", "im Schloss"},
   "Ich würd gern %s wohnen!", "Weißt du noch, wo ich wohnen will?", "Du willst doch %s wohnen!"},
  {BABY, "Was naschst du am liebsten?", 4, {"Gummibärchen", "Eis", "Kekse", "Lollis"},
   "Ich nasch am liebsten %s!", "Weißt du noch, was ich gern nasche?", "Du naschst doch so gern %s!"},
  // Teenager
  {TEEN, "Welche Musik hörst du?", 4, {"Pop", "Rock", "Rap", "Klassik"},
   "Ich hör am liebsten %s!", "Weißt du noch, welche Musik ich hör?", "Du hörst doch gern %s!"},
  {TEEN, "Was nervt dich am meisten?", 4, {"Hausaufgaben", "Aufräumen", "früh aufstehen", "Langeweile"},
   "Mich nervt %s total!", "Weißt du noch, was mich nervt?", "Dich nervt doch %s so!"},
  {TEEN, "Was wärst du gern?", 4, {"berühmt", "reich", "unsichtbar", "ein Superheld"},
   "Ich wär so gern %s!", "Weißt du noch, was ich gern wär?", "Du wärst doch gern %s!"},
  {TEEN, "Was machst du am Wochenende?", 4, {"Ausschlafen", "Freunde treffen", "Zocken", "Lesen"},
   "Am Wochenende mag ich %s!", "Weißt du noch, was ich am Wochenende mag?", "Am Wochenende magst du %s!"},
  {TEEN, "Was ist voll peinlich?", 4, {"Stolpern", "Rot werden", "alte Fotos", "Mamas Küsse"},
   "Voll peinlich find ich %s!", "Weißt du noch, was ich peinlich finde?", "Du findest %s so peinlich!"},
  {TEEN, "Was willst du mal werden?", 4, {"Tierärztin", "Künstlerin", "Astronautin", "Köchin"},
   "Ich werd mal %s!", "Weißt du noch, was ich mal werden will?", "Du wolltest doch %s werden!"},
  // Erwachsen
  {ERWACHSEN, "Was trinkst du morgens?", 4, {"Kaffee", "Tee", "Kakao", "Saft"},
   "Morgens trink ich am liebsten %s!", "Weißt du noch, was ich morgens trinke?", "Du trinkst morgens doch %s!"},
  {ERWACHSEN, "Welche Jahreszeit magst du?", 4, {"Frühling", "Sommer", "Herbst", "Winter"},
   "Ich mag den %s am liebsten!", "Weißt du noch, welche Jahreszeit ich mag?", "Du magst doch den %s so!"},
  {ERWACHSEN, "Bist du früh wach oder spät?", 2, {"Frühaufsteherin", "Nachteule"},
   "Ich bin eine %s!", "Weißt du noch: früh wach oder Nachteule?", "Du bist doch eine %s!"},
  {ERWACHSEN, "Wohin würdest du reisen?", 4, {"ans Meer", "in die Berge", "in eine Großstadt", "nach Japan"},
   "Ich würd so gern %s reisen!", "Weißt du noch, wohin ich reisen will?", "Du willst doch %s reisen!"},
  {ERWACHSEN, "Wie entspannst du dich?", 4, {"Baden", "Lesen", "Spazieren", "Serien schauen"},
   "Zum Entspannen mag ich %s!", "Weißt du noch, wie ich entspanne?", "Zum Entspannen magst du %s!"},
  {ERWACHSEN, "Was ist dein Lieblingsessen?", 4, {"Pizza", "Pasta", "Sushi", "Suppe"},
   "Mein Lieblingsessen ist %s!", "Weißt du noch mein Lieblingsessen?", "Dein Lieblingsessen ist %s!"},
  {ERWACHSEN, "Hund oder Katze?", 2, {"Hund", "Katze"},
   "Ich bin Team %s!", "Weißt du noch: Hund oder Katze?", "Du bist doch Team %s!"},
  // Oma
  {OMA, "Was machst du an Regentagen?", 4, {"Stricken", "Backen", "Rätseln", "Mittagsschlaf"},
   "An Regentagen mag ich %s!", "Weißt du noch, was ich bei Regen mache?", "Bei Regen magst du %s!"},
  {OMA, "Welchen Kuchen magst du?", 4, {"Apfelkuchen", "Käsekuchen", "Schokokuchen", "Streuselkuchen"},
   "Ich back am liebsten %s!", "Weißt du noch, welchen Kuchen ich backe?", "Du magst doch %s so gern!"},
  {OMA, "Welche Blumen magst du?", 4, {"Rosen", "Tulpen", "Sonnenblumen", "Gänseblümchen"},
   "Ich mag %s am liebsten!", "Weißt du noch, welche Blumen ich mag?", "Du magst doch %s so gern!"},
  {OMA, "Was war früher besser?", 4, {"die Musik", "das Fernsehen", "das Wetter", "die Mode"},
   "Früher war %s viel besser!", "Weißt du noch, was früher besser war?", "Bei dir war früher %s besser!"},
  {OMA, "Was trinkst du am liebsten?", 4, {"Kräutertee", "Kakao", "Limonade", "Kaffee"},
   "Ich trink am liebsten %s!", "Weißt du noch, was ich gern trinke?", "Du trinkst doch so gern %s!"},
  {OMA, "Was spielst du gern?", 4, {"Karten", "Rätsel", "Schach", "Bingo"},
   "Ich spiel am liebsten %s!", "Weißt du noch, was ich gern spiele?", "Du spielst doch so gern %s!"},
};
const int FRAGEN_ANZAHL = sizeof(FRAGEN) / sizeof(FRAGEN[0]);
static_assert(sizeof(FRAGEN) / sizeof(FRAGEN[0]) <= FRAGEN_MAX, "Zu viele Fragen für den Spielstand");

// ---------- Meinungen zum Plaudern: je Evi wird eine der beiden Fassungen ausgewürfelt ----------
// stufe = ab welcher Freundschaftsstufe sie das erzählt. %s = dein Name.
struct Meinung { uint8_t stufe; const char* a; const char* b; };
const Meinung MEINUNGEN[] = {
  // 1: Harmloses
  {1, "Ich mag Kekse mit Schokolade!", "Ich mag Kekse ohne alles!"},
  {1, "Ich schlaf am liebsten auf dem Rücken.", "Ich schlaf am liebsten eingerollt."},
  {1, "Regen klingt so schön am Fenster.", "Regen ist doof. Alles wird nass!"},
  {1, "Ich zähl gern die Sterne.", "Ich guck gern Wolken an."},
  {1, "Schnee schmeckt nach nichts.", "Schnee schmeckt nach Winter!"},
  {1, "Ich mag es, wenn es ruhig ist.", "Ich mag es, wenn was los ist!"},
  // 2: Was sie nicht mag
  {2, "Ich mag keine nassen Pfoten.", "Nasse Pfoten find ich lustig!"},
  {2, "Baden ist nicht so meins …", "Ich liebe Schaumbäder!"},
  {2, "Gewitter find ich gruselig.", "Gewitter find ich spannend!"},
  {2, "Früh aufstehen ist doof.", "Morgens bin ich am fröhlichsten!"},
  {2, "Brokkoli ist eklig!", "Brokkoli sieht aus wie kleine Bäume. Lecker!"},
  {2, "Ich mag es nicht, allein zu sein.", "Manchmal bin ich gern allein."},
  // 3: Kleine Geheimnisse
  {3, "Psst … ich rede manchmal mit dem Mond.", "Psst … mein Schatten macht mir Angst."},
  {3, "Geheimnis: Ich hab einen Keks versteckt.", "Geheimnis: Ich kann nicht pfeifen."},
  {3, "Manchmal tu ich nur so, als ob ich schlafe.", "Ich schnarche. Sag's keinem!"},
  {3, "Ich glaube, der Mond ist aus Käse.", "Ich glaube, Wolken sind aus Watte."},
  {3, "Ich hab heimlich für dich ein Lied geübt.", "Ich hab heute schon dreimal an dich gedacht."},
  // 4: Freunde (mit deinem Namen)
  {4, "Mit dir ist alles schöner, %s.", "Du bist mein Lieblingsmensch, %s!"},
  {4, "Ich denk oft an dich, wenn du weg bist.", "Ich zähl die Minuten, bis du kommst."},
  {4, "Weißt du, %s? Ich vertrau dir.", "Bei dir fühl ich mich sicher, %s."},
  // 5: Beste Freunde: Träume
  {5, "Ich träume davon, mal zu fliegen.", "Ich träume von einem Haus aus Keksen."},
  {5, "Ich wünsch mir, dass wir immer Freunde sind.", "Mein Traum? Mit dir ans Meer, %s!"},
  {5, "Du bist meine allerbeste Freundin, %s.", "Ich bin so froh, dass es dich gibt, %s."},
};
const int MEINUNGEN_ANZAHL = sizeof(MEINUNGEN) / sizeof(MEINUNGEN[0]);

// ---------- Sätze, je fünf ----------
const char* const S_GLEICH[5] = {"Ich auch! Wir sind gleich! ♥", "Echt? Genau wie ich! ♥", "Wir passen zusammen! ♥", "Juhu, ich auch!",
                                 "Das ist auch meins! ♥"};
const char* const S_SPAETER[5] = {"Überleg es dir ruhig noch!", "Ich warte noch ein bisschen.", "Du darfst auch später antworten!",
                                  "Hmm? Sag es mir gleich!", "Denk ruhig nach …"};
const char* const S_VERPASST[5] = {"Na gut, ein andermal!", "Schon gut, frag ich dich später.", "Dann eben morgen!",
                                   "Ist nicht schlimm!", "Vielleicht nächstes Mal."};
const char* const S_RICHTIG[5] = {"Genau! Du hörst mir zu! ♥", "Richtig! Du kennst mich! ♥", "Ja! Du hast es dir gemerkt!",
                                  "Stimmt! Hach, schön! ♥", "Juhu, richtig!"};
const char* const S_FALSCH[5] = {"Nein … es war %s!", "Hmm, nein. %s!", "Fast! Es war %s.", "Nö! Ich mag %s!", "Ach du … es war %s!"};
const char* const S_GESCHENK[5] = {"Ich hab was für dich, %s!", "Überraschung, %s!", "Das hab ich für dich gefunden!",
                                   "Für dich, %s! ♥", "Guck mal, ein Geschenk!"};
const char* const S_BESTE_VOR[5] = {"%s … darf ich dir was sagen?", "%s, weißt du was?", "Du, %s … hör mal!",
                                    "%s! Ich muss dir was sagen!", "Psst, %s … komm mal her!"};
const char* const S_BESTE_WEG[5] = {"Ich dachte, wir wären beste Freunde …", "Bin ich dir nicht mehr wichtig?",
                                    "Früher waren wir beste Freunde …", "Hast du mich vergessen?", "Ich vermisse unsere Freundschaft …"};
const char* const S_GEBURTSTAG[5] = {"Alles Gute zum Geburtstag, %s!", "Happy Birthday, %s! ♥", "Hurra! %s hat Geburtstag!",
                                     "Heute ist dein Tag, %s! ♥", "Geburtstag! Ich hab dich lieb, %s!"};
const char* const S_GEBURTSTAG_PLAUDERN[5] = {"Heute ist dein Geburtstag! ♥", "Wie alt bist du jetzt? Hihi!", "Ich hab heute nur Augen für dich!",
                                              "Geburtstagskind! ♥", "Kriegst du heute Kuchen?"};
const char* const S_GEBURTSTAG_MORGEN[5] = {"Morgen ist dein Geburtstag, oder?", "Nur noch einmal schlafen! ♥", "Ich freu mich schon auf morgen!",
                                            "Psst … morgen ist was Besonderes!", "Bald ist dein Geburtstag!"};
// Begrüßung: schüchtern, Freunde (mit Name), beste Freunde
const char* const S_GRUSS_SCHUECHTERN[5] = {"Oh … hallo.", "Hallo …", "Ähm, hi.", "Oh, du bist es.", "Hallo … *versteckt sich*"};
const char* const S_GRUSS_FREUNDE[5] = {"Hallo, %s!", "Da bist du ja, %s!", "%s! Endlich!", "Schön, dass du da bist, %s!", "Hey %s! ♥"};
const char* const S_GRUSS_BESTE[5] = {"Meine beste Freundin! ♥", "%s! Ich hab dich vermisst!", "Juhu, %s ist da! ♥",
                                      "Beste Freundin, hallo! ♥", "Da ist ja mein Lieblingsmensch!"};

// Melodien: Beste Freunde (kleine Fanfare) und „Zum Geburtstag viel Glück“ (Volkslied, frei)
const Note TON_BESTE[] = {{523, 120}, {0, 30}, {659, 120}, {0, 30}, {784, 120}, {0, 30}, {1047, 240}, {0, 60}, {784, 120}, {0, 30}, {1047, 400}};
const Note TON_GEBURTSTAG[] = {{392, 220}, {0, 20}, {392, 120}, {0, 20}, {440, 360}, {0, 20}, {392, 360}, {0, 20}, {523, 360}, {0, 20}, {494, 700},
                               {0, 120}, {392, 220}, {0, 20}, {392, 120}, {0, 20}, {440, 360}, {0, 20}, {392, 360}, {0, 20}, {587, 360}, {0, 20}, {523, 700}};

static char freundSatz[64];   // für Sätze mit Namen oder Antwort (Momente brauchen sie etwas länger)
static char freundSatz2[64];

// ---------- Kleine Helfer ----------
const char* fuenf(const char* const s[5]) { return s[random(0, 5)]; }
const char* mitName(const char* vorlage, char* puffer, size_t n) { snprintf(puffer, n, vorlage, NUTZER_NAME); return puffer; }

uint32_t eviWuerfel(const char* wofuer, int nummer) {
  char s[64];
  snprintf(s, sizeof(s), "%lld:%s:%d", (long long)pet.geboren, wofuer, nummer);
  return streuwert(s);
}
int evisAntwort(int q) { return (int)(eviWuerfel("frage", q) % FRAGEN[q].n); }
const char* evisMeinung(int i) {
  const Meinung& m = MEINUNGEN[i];
  return (eviWuerfel("meinung", i) & 1) ? m.b : m.a;
}

bool freundFrageOffen() { return gespraech.frage >= 0; }
bool freundDialogOffen() { return gespraech.gebDialog; }

// Jemand ist gerade da (hat in den letzten 2 Minuten bedient), und Evi kann reagieren
bool freundJemandDa(ms_t jetzt) {
  return launen.letzteBedienung != 0 && millis() - launen.letzteBedienung < DA_GERADE && eviKannReagieren(jetzt) &&
         moment.art == M_KEINER && !freundFrageOffen() && !gespraech.gebDialog;
}

// ---------- Speichern ----------
void freundNeueEvi() {
  const uint8_t t = freund.gebTag, m = freund.gebMonat;
  const int16_t g = freund.gebGefeiert;
  memset(&freund, 0, sizeof(freund));
  freund.kennung = FREUND_KENNUNG;
  freund.version = FREUND_VERSION;
  freund.groesse = sizeof(FreundStand);
  freund.gebTag = t; freund.gebMonat = m; freund.gebGefeiert = g;
  freund.evi = pet.geboren;
  freund.plusTag = freund.geschenkTag = freund.fragenTag = -1;
  gespraech.frage = -1;
  gespraech.geaendert = true;
}

void freundLaden() {
  uint8_t* b = (uint8_t*)&freund;
  for (size_t i = 0; i < sizeof(freund); i++) b[i] = EEPROM.read(FREUND_ADRESSE + i);
  const bool gut = freund.kennung == FREUND_KENNUNG && freund.version == FREUND_VERSION && freund.groesse == sizeof(FreundStand) &&
                   freund.pruefsumme == pruefsumme(b, offsetof(FreundStand, pruefsumme)) && freund.punkte >= 0 && freund.punkte <= 100 &&
                   freund.gebTag <= 31 && freund.gebMonat <= 12;
  if (!gut) { memset(&freund, 0, sizeof(freund)); freundNeueEvi(); }
  if (freund.evi != pet.geboren) freundNeueEvi();
}

void freundSpeichern() {
  freund.pruefsumme = pruefsumme((const uint8_t*)&freund, offsetof(FreundStand, pruefsumme));
  const uint8_t* b = (const uint8_t*)&freund;
  bool anders = false;
  for (size_t i = 0; i < sizeof(freund); i++) {
    if (EEPROM.read(FREUND_ADRESSE + i) != b[i]) { EEPROM.write(FREUND_ADRESSE + i, b[i]); anders = true; }
  }
  if (anders) EEPROM.commit();
  gespraech.geaendert = false;
}

// ---------- Freundschaft ----------
void freundTagPruefen(ms_t jetzt) {
  const int32_t heute = tagKennung(jetzt);
  if (freund.plusTag != heute) { freund.plusTag = heute; freund.plusHeute = 0; freund.plauderHeute = 0; }
}

void freundPlus(float wert) {
  const ms_t jetzt = jetztMs();
  if (phaseBei(pet, jetzt) == EI || phaseBei(pet, jetzt) == TOT) return;
  freundTagPruefen(jetzt);
  if (wert > PLUS_PRO_TAG - freund.plusHeute) wert = PLUS_PRO_TAG - freund.plusHeute;
  if (wert <= 0) return;
  freund.plusHeute += wert;
  freund.punkte = min(100.0f, freund.punkte + wert);
  gespraech.geaendert = true;
}

void freundMinus(float wert) {
  freund.punkte = max(0.0f, freund.punkte - wert);
  gespraech.geaendert = true;
}

// Anlässe aus den anderen Dateien
void freundKrank() { freundMinus(MINUS_KRANK); }
void freundBeleidigt() { freundMinus(MINUS_BELEIDIGT); }
void freundGepflegt() { freundPlus(PLUS_PFLEGE); }
void freundGekuschelt() { freundPlus(PLUS_KUSCHELN); }
void freundGespielt() { freundPlus(PLUS_SPIEL); }
void freundWunschErfuellt() { freundPlus(PLUS_LAUNENWUNSCH); }

// 1 schüchtern, 2 bekannt, 3 vertraut, 4 Freunde, 5 beste Freunde (erst nach dem großen Moment)
int freundStufe() {
  if (freund.beste) return 5;
  int s = 1;
  for (int i = 1; i < 4; i++) if (freund.punkte >= STUFE_AB[i]) s = i + 1;
  return s;
}

// ---------- Fragen ----------
int fragenProTag(Phase ph) { return ph == BABY ? 4 : ph == TEEN ? 2 : (ph == ERWACHSEN || ph == OMA) ? 1 : 0; }

// Wann an diesem Tag gefragt wird: Baby 9/12/15/18 Uhr + bis zu 3 Stunden, Teenager 9/15 Uhr + bis zu 6, sonst 10 Uhr + bis zu 10
ms_t frageZeit(ms_t jetzt, int slot, int anzahl) {
  const Ortszeit o = ortszeit(jetzt);
  const int start = anzahl == 4 ? 9 + 3 * slot : anzahl == 2 ? 9 + 6 * slot : 10;
  const int laenge = anzahl == 4 ? 3 : anzahl == 2 ? 6 : 10;
  char s[48];
  snprintf(s, sizeof(s), "%lld:%d-%d-%d:frage:%d", (long long)pet.geboren, o.jahr, o.monat, o.tag, slot);
  return ausOrtszeit(o.jahr, o.monat, o.tag, start, (int)(streuwert(s) % (uint32_t)(laenge * 60 - 6)));
}

// Welche Frage als Nächstes (noch nicht beantwortet, in der aktuellen Lebensphase), -1 = keine
int naechsteFrage(Phase ph) {
  int nummern[FRAGEN_MAX], n = 0;
  for (int q = 0; q < FRAGEN_ANZAHL; q++) if (FRAGEN[q].phase == ph && freund.antwort[q] == 0) nummern[n++] = q;
  return n ? nummern[random(0, n)] : -1;
}

// Eine Frage für „Weißt du noch?“: Evi hat dir ihre Antwort erzählt, aber noch nicht abgefragt. -1 = keine
int weisstDuNochFrage() {
  int nummern[FRAGEN_MAX], n = 0;
  for (int q = 0; q < FRAGEN_ANZAHL; q++) if ((freund.verraten >> q & 1) && !(freund.abgefragt >> q & 1)) nummern[n++] = q;
  return n ? nummern[random(0, n)] : -1;
}

void frageStellen(int q, bool weisst, int slot) {
  gespraech.frage = q;
  gespraech.weisstDuNoch = weisst;
  gespraech.slot = slot;
  gespraech.bis = millis() + FRAGE_STEHT;
  bildschirmEin();
  piep(TON(TON_WUNSCH));
  sage(weisst ? FRAGEN[q].weisst : FRAGEN[q].frage, &SYM_W_HERZ, FRAGE_STEHT);
  zeigeMeldung(weisst ? "Weißt du noch? · A antwortet" : "Eine Frage an dich · A antwortet", 6000);
}

void frageSchliessen() {
  if (menueOffen && frageAuswahl) menueZeigen(false);
  gespraech.frage = -1;
  sagen.bis = 0;
}

// A (oder START) bei offener Frage: die Antworten zeigen
void freundFrageMenue() { menueZeigen(true, false, false, true); }

// Die Antworten im Menü (menueBauen in evi-handy.ino)
int freundAntwortenZahl() { return gespraech.frage >= 0 ? FRAGEN[gespraech.frage].n : 0; }
const char* freundAntwortText(int i) { return FRAGEN[gespraech.frage].antw[i]; }

void freundAntwort(int i) {
  if (gespraech.frage < 0) return;
  const int q = gespraech.frage;
  const FrageText& f = FRAGEN[q];
  const int ihre = evisAntwort(q);
  gespraech.frage = -1;
  sagen.bis = 0;
  moment.ruheBisTakt = evi.takt + RUHE_TAKTE;
  if (gespraech.weisstDuNoch) {
    freund.abgefragt |= 1UL << q;
    if (i == ihre) {
      freundPlus(PLUS_WEISST);
      reagiere(R_JUBEL, 20);
      piep(TON(TON_MUENZE));
      sage(fuenf(S_RICHTIG), &SYM_W_HERZ, 4000);
    } else {
      snprintf(freundSatz, sizeof(freundSatz), fuenf(S_FALSCH), f.antw[ihre]);
      sage(freundSatz, &SYM_W_HERZ, 4000);
    }
  } else {
    freund.antwort[q] = (uint8_t)(i + 1);
    freund.verraten |= 1UL << q;
    freundPlus(PLUS_ANTWORT);
    if (i == ihre) {
      freundPlus(PLUS_GLEICH);
      reagiere(R_JUBEL, 20);
      piep(TON(TON_MUENZE));
      sage(fuenf(S_GLEICH), &SYM_W_HERZ, 4000);
    } else {
      reagiere(R_GRUSS, 12);
      snprintf(freundSatz, sizeof(freundSatz), f.ich, f.antw[ihre]);
      sage(freundSatz, &SYM_W_HERZ, 4500);
    }
  }
  speichern(jetztMs());
}

// Menü mit den Antworten ohne Antwort geschlossen: Die Frage bleibt stehen.
void freundFrageGeschlossen() {
  if (gespraech.frage < 0) return;
  sage(fuenf(S_SPAETER), &SYM_W_HERZ, 2500);
}

// ---------- Plaudern ----------
bool istGeburtstag(ms_t t) {
  const Ortszeit o = ortszeit(t);
  return freund.gebTag && o.tag == freund.gebTag && o.monat == freund.gebMonat;
}

void freundPlaudern() {
  const ms_t jetzt = jetztMs();
  freundTagPruefen(jetzt);
  const Phase ph = phaseBei(pet, jetzt);
  const int stufe = freundStufe();
  momentStoppen(false);
  moment.ruheBisTakt = evi.takt + RUHE_TAKTE;
  const char* satz = nullptr;
  // Geburtstag heute oder morgen: oft davon
  if (freund.gebTag && random(0, 2) == 0) {
    if (istGeburtstag(jetzt)) satz = fuenf(S_GEBURTSTAG_PLAUDERN);
    else if (istGeburtstag(jetzt + TAG)) satz = fuenf(S_GEBURTSTAG_MORGEN);
  }
  if (!satz) {
    // Was in Frage kommt: 0 ihre eigene Antwort, 1 eine Meinung, 2 deine Antwort (ab „bekannt“)
    int eigene[FRAGEN_MAX], ne = 0, deine[FRAGEN_MAX], nd = 0, meinungen[32], nm = 0;
    for (int q = 0; q < FRAGEN_ANZAHL; q++) {
      if (FRAGEN[q].phase == ph) eigene[ne++] = q;
      if (freund.antwort[q]) deine[nd++] = q;
    }
    for (int i = 0; i < MEINUNGEN_ANZAHL && nm < 32; i++) {
      if (MEINUNGEN[i].stufe > stufe) continue;
      meinungen[nm++] = i;
      if (MEINUNGEN[i].stufe == stufe && nm < 32) meinungen[nm++] = i;   // Neues aus der aktuellen Stufe öfter
    }
    const int gewEigene = ne ? 3 : 0, gewMeinung = nm ? 4 : 0, gewDeine = (stufe >= 2 && nd) ? 2 : 0;
    int r = random(0, max(1, gewEigene + gewMeinung + gewDeine));
    if (r < gewEigene) {
      const int q = eigene[random(0, ne)];
      snprintf(freundSatz, sizeof(freundSatz), FRAGEN[q].ich, FRAGEN[q].antw[evisAntwort(q)]);
      freund.verraten |= 1UL << q;
      satz = freundSatz;
    } else if ((r -= gewEigene) < gewMeinung) {
      satz = mitName(evisMeinung(meinungen[random(0, nm)]), freundSatz, sizeof(freundSatz));
    } else if (nd) {
      const int q = deine[random(0, nd)];
      snprintf(freundSatz, sizeof(freundSatz), FRAGEN[q].deine, FRAGEN[q].antw[freund.antwort[q] - 1]);
      satz = freundSatz;
    } else {
      satz = "Hihi! ♥";
    }
  }
  reagiere(R_GRUSS, 12);
  sage(satz, &SYM_W_HERZ, 5000);
  char text[48];
  snprintf(text, sizeof(text), "%s erzählt dir was", pet.name);
  zeigeMeldung(text, 5000);
  if (freund.plauderHeute < PLAUDERN_MIT_WIRKUNG) { freund.plauderHeute++; freundPlus(PLUS_PLAUDERN); }
  gespraech.geaendert = true;
}

// ---------- Begrüßung je nach Freundschaft (aus begruessen in momente.ino) ----------
const char* freundGruss(const char* normal, bool& jubel) {
  const int s = freundStufe();
  if (s == 1) { jubel = false; return fuenf(S_GRUSS_SCHUECHTERN); }
  if (s >= 3) jubel = true;                              // ab „vertraut“ hüpft sie dir entgegen
  if (s == 5) return mitName(fuenf(S_GRUSS_BESTE), freundSatz2, sizeof(freundSatz2));
  if (s == 4 && random(0, 2) == 0) return mitName(fuenf(S_GRUSS_FREUNDE), freundSatz2, sizeof(freundSatz2));
  return normal;
}

// ---------- Geschenk (ab „Freunde“, einmal am Tag, als kleiner Moment) ----------
bool freundGeschenkFaellig(ms_t jetzt) {
  return freundStufe() >= 4 && freund.geschenkTag != tagKennung(jetzt) && random(0, 4) == 0;
}

void freundGeschenkGeben(int muenzen) {
  pet.muenzen += muenzen;
  piep(TON(TON_MUENZE));
  char text[48];
  snprintf(text, sizeof(text), "%s schenkt dir %d Münzen ♥", pet.name, muenzen);
  zeigeMeldung(text, 5000);
  speichern(jetztMs());
}

void freundMelodie(int welche) {
  if (welche == 0) piep(TON(TON_BESTE));
  else piep(TON(TON_GEBURTSTAG));
}

// Abläufe der Momente (aus momentStarten)
void geschenkPlanen() {
  freund.geschenkTag = tagKennung(jetztMs());
  gespraech.geaendert = true;
  akt(A_SAGEN, 3500, 0, mitName(fuenf(S_GESCHENK), freundSatz, sizeof(freundSatz)));
  akt(A_HUEPFEN, 2, 8, nullptr);
  akt(A_GESCHENK, 3, 0, nullptr);
  akt(A_HERZEN, 2500, 0, nullptr);
  akt(A_WARTEN, 25, 0, nullptr);
}

void bestePlanen() {
  static char zeile[64];
  snprintf(zeile, sizeof(zeile), "♥ %s und %s sind beste Freunde ♥", pet.name, NUTZER_NAME);
  akt(A_SAGEN, 4000, 0, mitName(fuenf(S_BESTE_VOR), freundSatz, sizeof(freundSatz)));
  akt(A_BILD, MB_STAUNEN, 0, nullptr);
  akt(A_WARTEN, 34, 0, nullptr);
  akt(A_BILD, MB_KEINS, 0, nullptr);
  akt(A_MELODIE, 0, 0, nullptr);
  akt(A_HUEPFEN, 3, 9, nullptr);
  akt(A_HERZEN, 9000, 0, nullptr);
  akt(A_SAGEN, 6000, 0, "Wir sind jetzt beste Freunde! ♥");
  akt(A_MELDUNG, 15000, 0, zeile);
  akt(A_WARTEN, 45, 0, nullptr);
  akt(A_HUEPFEN, 2, 8, nullptr);
  akt(A_WARTEN, 20, 0, nullptr);
}

void besteWegPlanen() {
  piep(TON(TON_TRAURIG));
  akt(A_BILD, MB_TRAURIG, 0, nullptr);
  akt(A_SAGEN, 5000, 0, fuenf(S_BESTE_WEG));
  akt(A_WARTEN, 45, 0, nullptr);
  akt(A_BILD, MB_KEINS, 0, nullptr);
}

void geburtstagPlanen() {
  akt(A_MELODIE, 1, 0, nullptr);
  akt(A_SAGEN, 6000, 0, mitName(fuenf(S_GEBURTSTAG), freundSatz, sizeof(freundSatz)));
  akt(A_MELDUNG, 15000, 0, "Alles Gute zum Geburtstag! ♥");
  akt(A_HUEPFEN, 4, 9, nullptr);
  akt(A_HERZEN, 9000, 0, nullptr);
  akt(A_WARTEN, 30, 0, nullptr);
  akt(A_GESCHENK, 10, 0, nullptr);
  akt(A_HUEPFEN, 2, 8, nullptr);
  akt(A_WARTEN, 20, 0, nullptr);
}

// ---------- Geburtstag einstellen (einmal, mit dem Steuerkreuz) ----------
const uint8_t MONATSTAGE[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void gebDialogOeffnen() {
  gespraech.gebDialog = true;
  gespraech.gebTag = 1; gespraech.gebMonat = 1; gespraech.gebFeld = 0;
  menueZeigen(false);
  bildschirmEin();
  piep(TON(TON_WUNSCH));
  snprintf(freundSatz2, sizeof(freundSatz2), "Wann hast du Geburtstag, %s?", NUTZER_NAME);
  sage(freundSatz2, &SYM_W_HERZ, 600000);
}

// Tasten, solange der Geburtstag eingestellt wird. true = Taste verbraucht.
bool freundKnopf(Knopf k) {
  if (!gespraech.gebDialog) return false;
  uint8_t& wert = gespraech.gebFeld == 0 ? gespraech.gebTag : gespraech.gebMonat;
  const uint8_t hoechst = gespraech.gebFeld == 0 ? MONATSTAGE[gespraech.gebMonat - 1] : 12;
  if (k == K_UP) wert = wert >= hoechst ? 1 : wert + 1;
  else if (k == K_DOWN) wert = wert <= 1 ? hoechst : wert - 1;
  else if (k == K_L || k == K_R) gespraech.gebFeld ^= 1;
  else if (k == K_A) {
    freund.gebTag = gespraech.gebTag;
    freund.gebMonat = gespraech.gebMonat;
    gespraech.gebDialog = false;
    piep(TON(TON_MUENZE));
    sage("Das merk ich mir für immer! ♥", &SYM_W_HERZ, 4000);
    speichern(jetztMs());
  } else if (k == K_B || k == K_START) {
    gespraech.gebDialog = false;
    gespraech.gebNaechsterVersuch = millis() + 60UL * 60 * 1000;
    sage("Dann frag ich dich später!", &SYM_W_HERZ, 3000);
  }
  if (gespraech.gebTag > MONATSTAGE[gespraech.gebMonat - 1]) gespraech.gebTag = MONATSTAGE[gespraech.gebMonat - 1];
  return true;
}

void freundZeichnen() {
  if (!gespraech.gebDialog) return;
  const int w = 176, h = 70, x = (BREITE - w) / 2, y = 96;
  bild.fillRoundRect(x + 2, y + 2, w, h, 8, 0);
  bild.fillRoundRect(x, y, w, h, 8, 1);
  bild.drawRoundRect(x, y, w, h, 8, 0);
  bild.drawRoundRect(x + 1, y + 1, w - 2, h - 2, 7, 0);
  const char* titel = "Mein Geburtstag";
  schreibe(bild, x + (w - textBreite(titel)) / 2, y + 8, titel, 0, 1, true);
  char t[4], m[4];
  snprintf(t, sizeof(t), "%02d", gespraech.gebTag);
  snprintf(m, sizeof(m), "%02d", gespraech.gebMonat);
  const int zy = y + 24, tx = x + w / 2 - 34, mx = x + w / 2 + 10;
  for (int f = 0; f < 2; f++) {
    const int fx = f == 0 ? tx : mx;
    const bool sel = gespraech.gebFeld == f;
    if (sel) bild.fillRect(fx - 3, zy - 3, 30, 20, 0);
    else bild.drawRect(fx - 3, zy - 3, 30, 20, 0);
    schreibe(bild, fx, zy, f == 0 ? t : m, sel ? 1 : 0, 2);
  }
  schreibe(bild, x + w / 2 - 4, zy + 2, ".", 0, 2);
  const char* hilfe = "▲▼ ändern ◀▶ Feld · A OK";
  schreibe(bild, x + (w - textBreite(hilfe)) / 2, y + h - 14, hilfe, 0);
}

// ---------- Jede Runde (10 s, aus runde): lange niedrige Balken ----------
void freundRunde(ms_t jetzt) {
  if (freund.evi != pet.geboren) freundNeueEvi();
  if (wach(pet, jetzt) && (pet.satt <= R::NIEDRIG || pet.laune <= R::NIEDRIG || pet.sauber <= R::NIEDRIG)) {
    gespraech.niedrigMs += 10000;
    if (gespraech.niedrigMs >= 60UL * 60 * 1000) { gespraech.niedrigMs = 0; freundMinus(MINUS_NIEDRIG_JE_STUNDE); }
  }
  if (gespraech.geaendert) freundSpeichern();
}

// ---------- Jeder Takt (120 ms) ----------
void freundTakt(ms_t jetzt) {
  if (freund.evi != pet.geboren) freundNeueEvi();
  const Phase ph = phaseBei(pet, jetzt);
  const Stimmung s = beschreiben(pet, jetzt);
  // Beste Freunde verloren?
  if (freund.beste && freund.punkte < BESTE_WEG_UNTER) {
    freund.beste = 0;
    gespraech.besteWegSagen = true;
    gespraech.geaendert = true;
  }
  // Offene Frage: Zeit um, eingeschlafen oder krank → ohne Folgen vorbei
  if (gespraech.frage >= 0) {
    if ((int32_t)(millis() - gespraech.bis) >= 0 || ruhend(s) || garten.phase != GA_ZU || sideboard.phase != SB_ZU) {
      const bool sagen_ = !ruhend(s) && bildschirmAn;
      frageSchliessen();
      if (sagen_) sage(fuenf(S_VERPASST), &SYM_W_HERZ, 3000);
    } else if (!spiel.aktiv && (int32_t)(millis() - sagen.bis) >= 0) {
      // Etwas anderes wurde gesagt und ist vorbei: Die Frage steht wieder in der Blase.
      sage(gespraech.weisstDuNoch ? FRAGEN[gespraech.frage].weisst : FRAGEN[gespraech.frage].frage, &SYM_W_HERZ, gespraech.bis - millis());
    }
    return;
  }
  if (evi.takt % 8 || !tastenDa || !momenteFuer(ph) || ruhend(s) || launenBeleidigt() || launenWunschOffen()) return;
  // Große Momente: nur, wenn du gerade da bist
  if (freundJemandDa(jetzt)) {
    if (gespraech.besteWegSagen) { gespraech.besteWegSagen = false; momentStarten(M_BESTE_WEG, GD_KEINER); return; }
    const Ortszeit o = ortszeit(jetzt);
    if (istGeburtstag(jetzt) && freund.gebGefeiert != o.jahr && o.stunde >= 7) {
      freund.gebGefeiert = (int16_t)o.jahr;
      freundPlus(PLUS_GEBURTSTAG);
      speichern(jetzt);
      bildschirmEin();
      momentStarten(M_GEBURTSTAG, GD_KEINER);
      return;
    }
    if (!freund.beste && freund.punkte >= STUFE_AB[4] && (ph == ERWACHSEN || ph == OMA)) {
      freund.beste = 1;
      speichern(jetzt);
      bildschirmEin();
      momentStarten(M_BESTE, GD_KEINER);
      return;
    }
    if (freund.gebTag == 0 && (int32_t)(millis() - gespraech.gebNaechsterVersuch) >= 0) { gebDialogOeffnen(); return; }
  }
  // Fragen zu den festen Zeiten des Tages
  if (!bildschirmAn || !eviKannReagieren(jetzt) || moment.art != M_KEINER || gespraech.gebDialog) return;
  const int32_t heute = tagKennung(jetzt);
  if (freund.fragenTag != heute) { freund.fragenTag = heute; freund.slotsErledigt = 0; gespraech.geaendert = true; }
  const int anzahl = fragenProTag(ph);
  for (int i = 0; i < anzahl; i++) {
    if (freund.slotsErledigt >> i & 1) continue;
    const ms_t um = frageZeit(jetzt, i, anzahl);
    if (jetzt < um || jetzt >= um + (ms_t)FRAGE_STEHT) continue;
    freund.slotsErledigt |= 1UL << i;
    gespraech.geaendert = true;
    int q = naechsteFrage(ph);
    bool weisst = false;
    const int w = freundStufe() >= 4 ? weisstDuNochFrage() : -1;
    if (w >= 0 && (q < 0 || random(0, 3) == 0)) { q = w; weisst = true; }
    if (q >= 0) frageStellen(q, weisst, i);
    return;
  }
}
