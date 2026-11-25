#pragma once
// zeigen() in bildschirm.h meldet jedes fertige Bild an die Nachbildung
#define NACHBILDUNG
void nachbildungBildFertig();
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
using std::max; using std::min;
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define FALLING 2
#define MSBFIRST 1
#define SPI_MODE0 0
extern uint32_t simMillis;
extern int simDC;   // Pin 14 (LCD_DC): 0 = Befehl, 1 = Daten; für das nachgebildete Display
inline int simLicht = -1;   // letzter Wert der Hintergrundbeleuchtung (Pin 16, 0–255)
inline void pinMode(int,int){} inline void digitalWrite(int p,int v){ if (p == 14) simDC = v; } inline void analogWrite(int p,int v){ if (p == 16) simLicht = v; } inline int analogRead(int){return 0;}
inline void delay(unsigned long ms){ simMillis += ms; } inline unsigned long millis(){return simMillis;} inline unsigned long micros(){return simMillis*1000;}
inline long random(long a,long b){ return b<=a ? a : a + rand() % (b-a);} inline long random(long b){return b<=0?0:rand()%b;} inline void randomSeed(unsigned long s){srand(s);}
inline int digitalPinToInterrupt(int p){return p;} inline void attachInterrupt(int, void(*)(), int){}
struct SerialC{void begin(long){} int available(){return 0;} int read(){return 0;} size_t write(const uint8_t*,size_t n){return n;} void flush(){}
 template<class T> void print(T){} template<class T> void println(T){} void println(){} };
extern SerialC Serial;
template<class T> T constrain(T x,T a,T b){return x<a?a:(x>b?b:x);}
