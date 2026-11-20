#ifndef TAMAGOTCHI_MELODY_H
#define TAMAGOTCHI_MELODY_H

#include <stdint.h>
#include <avr/pgmspace.h>

// Ein Ton: Hoehe in Hertz (0 = Pause) und Dauer in Millisekunden.
struct Note {
  uint16_t freq;
  uint16_t ms;
};

// Die Melodien liegen im Flash-Speicher, um Arbeitsspeicher zu sparen.
const Note MELODY_HELLO[] PROGMEM = {
    {523, 90}, {0, 30}, {659, 90}, {0, 30}, {784, 90}, {0, 30}, {1047, 180}};
const Note MELODY_TICK[] PROGMEM = {{2093, 12}};
const Note MELODY_BACK[] PROGMEM = {{1047, 12}};
const Note MELODY_PLAY[] PROGMEM = {
    {1047, 70}, {0, 30}, {1319, 70}, {0, 30}, {1568, 70}, {0, 30}, {2093, 140}};
const Note MELODY_CLEAN[] PROGMEM = {
    {1568, 50}, {0, 40}, {1568, 50}, {0, 40}, {1568, 50}, {0, 60}, {2093, 120}};
const Note MELODY_NEW[] PROGMEM = {
    {784, 120}, {0, 60}, {784, 120}, {0, 60}, {1047, 260}};
const Note MELODY_FEED[] PROGMEM = {
    {784, 80}, {0, 40}, {988, 80}, {0, 40}, {1319, 140}};
// Hinweiston, sobald ein Beduerfnis auf 30 % faellt.
const Note MELODY_ATTENTION[] PROGMEM = {
    {880, 120}, {0, 120}, {880, 120}, {0, 120}, {659, 260}};
const Note MELODY_WAKE[] PROGMEM = {{659, 80}, {0, 30}, {880, 120}};

#define MELODY(name) name, uint8_t(sizeof(name) / sizeof(Note))

#endif
