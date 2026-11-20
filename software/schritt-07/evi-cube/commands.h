#ifndef TAMAGOTCHI_COMMANDS_H
#define TAMAGOTCHI_COMMANDS_H

#include <stdint.h>

// Diese Befehle gelten unabhaengig von Anzahl und Anschluss der Taster.
enum class Command : uint8_t { None, Next, Confirm, Back };

#endif
