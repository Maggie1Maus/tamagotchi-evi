// Namen (26.11.2026, nur Waveshare). Logik in namen.ino.
//  - Jedes neue Baby bekommt von der Nutzerin einen Namen (höchstens 11 Buchstaben) und ein Geschlecht.
//    Die Tiere heißen allgemein „Tamagotchi“; das allererste hieß Evi und behält den Namen.
//  - Die 20 Park-Figuren heißen ab dem ersten benannten Baby anders: 50 Jungen- und 50 Mädchennamen rotieren.
//    Jede Figur behält ihren Namen, solange dieses Tamagotchi lebt, und heißt nie wie das Tamagotchi selbst.
//    Beim nächsten Tamagotchi wandert jede Figur in ein anderes Zehnerfenster der Liste (5 Leben, dann wieder von vorn).
//  - Das Tamagotchi, das beim Einspielen schon lebt, behält die alten Namen (Lotti, Mampf …).
#pragma once

const char* const NAMEN_M[50] = {
  "Alf", "Ben", "Paul", "Finn", "Jonas", "Luca", "Noah", "Emil", "Anton", "Felix",
  "Max", "Tom", "Theo", "Oskar", "Henri", "Karl", "Otto", "Ole", "Jan", "Tim",
  "Nils", "Lars", "Hugo", "Klaus", "Fritz", "Bruno", "Rudi", "Egon", "Willi", "Gustav",
  "Konrad", "Moritz", "Leander", "Valentin", "Benno", "Jasper", "Milan", "Elias", "David", "Simon",
  "Jakob", "Lukas", "Daniel", "Matteo", "Levi", "Arthur", "Ludwig", "Hannes", "Florian", "Jörn"};
const char* const NAMEN_W[50] = {
  "Paula", "Lena", "Anna", "Emma", "Hanna", "Clara", "Frieda", "Greta", "Ida", "Lotta",
  "Marie", "Nele", "Pia", "Sophie", "Tilda", "Vera", "Zoe", "Lilly", "Maja", "Nora",
  "Rosa", "Selma", "Thea", "Ulla", "Wanda", "Yara", "Elsa", "Jule", "Karla", "Luise",
  "Mathilda", "Nina", "Olga", "Rieke", "Svenja", "Tessa", "Uta", "Viola", "Wilma", "Amelie",
  "Berta", "Dora", "Emilia", "Fiona", "Gisela", "Helga", "Ilka", "Jette", "Klara", "Lisbeth"};

// Geschlecht der Figuren (Bit je Figur, Reihenfolge BESUCH_NAME): weiblich Lotti, Ella, Glöckchen, Kiwi, Klecksi, Lumi, Mia,
// Mimi, Moosi, Spuki; männlich Bit, Blip, Blubber, Funke, Groschen, Leo, Linus, Mampf, Rolli, Tetro
const uint32_t FIGUR_WEIBLICH_MASKE = (1UL << 0) | (1UL << 4) | (1UL << 6) | (1UL << 8) | (1UL << 9) | (1UL << 12) |
                                      (1UL << 14) | (1UL << 15) | (1UL << 16) | (1UL << 18);
const uint32_t FIGUR_MAENNLICH_MASKE = ((1UL << BESUCHER) - 1) & ~FIGUR_WEIBLICH_MASKE;

const int NAME_MAX_ZEICHEN = 11, NAME_MAX_BYTES = 19;   // pet.name hat 21 Bytes; 19 lässt Platz für Umlaute

// Namenseingabe (läuft als Park-Phase PARK_NAME)
struct NameEditor {
  char text[24];
  uint8_t pos;          // 0–29 Buchstaben, 30 Löschen, 31 Vorschlag, 32 Geschlecht, 33 Fertig
  uint8_t geschlecht;   // 0 Mädchen, 1 Junge
  uint8_t vorschlag;    // wievielter Vorschlag
};
NameEditor nameEdit;
