#!/bin/bash
# Prüft evi-handy am PC (Linux, z. B. in der Cloud-Sitzung): setzt die .ino-Dateien zusammen wie die
# Arduino IDE, übersetzt mit g++ gegen einfache Ersatz-Bibliotheken und lässt die Prüfprogramme laufen.
# bilder.cpp speichert Bildschirmbilder (PBM) nach $AUS/bilder, ablauf.cpp prüft Bildschirm-aus, Besuch und Begrüßung.
# Die Schrift in den Bildern ist nur ein Ersatz; Abstände und Grafik stimmen.
set -e
HIER="$(cd "$(dirname "$0")" && pwd)"
SKETCH="$HIER/../../evi-handy"
AUS="${1:-$(mktemp -d)}"
mkdir -p "$AUS/bilder"
cp "$HIER"/ersatz/*.h "$SKETCH"/*.h "$AUS"/
python3 -I "$HIER/arduino_zusammensetzen.py" "$SKETCH" "$AUS/evi-handy.ino"
cp "$HIER/bilder.cpp" "$HIER/ablauf.cpp" "$HIER/tasten.cpp" "$HIER/bewegung.cpp" "$HIER/umgebung.cpp" "$AUS"/
cd "$AUS"
echo "Übersetzen mit Warnungen:"
g++ -std=gnu++17 -Wall -Wextra -Wno-unused-parameter -I. -fsyntax-only -x c++ -include Arduino.h evi-handy.ino
g++ -std=gnu++17 -O1 -w -I. -o bilder-test bilder.cpp
./bilder-test
g++ -std=gnu++17 -O1 -w -I. -o ablauf-test ablauf.cpp
./ablauf-test
g++ -std=gnu++17 -O1 -w -I. -o tasten-test tasten.cpp
./tasten-test
g++ -std=gnu++17 -O1 -w -I. -o bewegung-test bewegung.cpp
./bewegung-test
g++ -std=gnu++17 -O1 -w -I. -o umgebung-test umgebung.cpp
./umgebung-test
echo "Bilder in $AUS/bilder"
