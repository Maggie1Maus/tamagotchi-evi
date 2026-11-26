#!/bin/bash
# Prüft evi-handy am PC (Linux, z. B. in der Cloud-Sitzung): setzt die .ino-Dateien zusammen wie die
# Arduino IDE, übersetzt mit g++ gegen einfache Ersatz-Bibliotheken und lässt die Prüfprogramme laufen.
# bilder.cpp speichert Bildschirmbilder (PBM) nach $AUS/bilder, ablauf.cpp prüft Bildschirm-aus, Besuch und Begrüßung,
# kuscheln.cpp Touch zum Kuscheln, die Uhrzeit und den Regenhimmel, schlaf.cpp den Schlafmodus.
# Die Schrift in den Bildern ist nur ein Ersatz; Abstände und Grafik stimmen.
set -e
HIER="$(cd "$(dirname "$0")" && pwd)"
SKETCH="$HIER/../../evi-handy"
AUS="${1:-$(mktemp -d)}"
mkdir -p "$AUS/bilder"
AUS="$(cd "$AUS" && pwd)"
cp "$HIER"/ersatz/*.h "$SKETCH"/*.h "$AUS"/
python3 -I "$HIER/arduino_zusammensetzen.py" "$SKETCH" "$AUS/evi-handy.ino"
if [ "${2:-}" = "--zimmer" ]; then
  TESTS=(einrichtung umgebung kuscheln park-ansichten)
else
  TESTS=(bilder ablauf tasten bewegung umgebung sideboard kuscheln raumtiefe schlaf einrichtung park-ansichten)
fi
for test in "${TESTS[@]}"; do
  cp "$HIER/$test.cpp" "$AUS/"
done
cd "$AUS"
echo "Übersetzen mit Warnungen:"
g++ -std=gnu++17 -Wall -Wextra -Wno-unused-parameter -I. -fsyntax-only -x c++ -include Arduino.h evi-handy.ino
for test in "${TESTS[@]}"; do
  g++ -std=gnu++17 -O1 -w -I. -o "$test-test" "$test.cpp"
  "./$test-test"
done
python3 "$HIER/../pruefe-zimmer-grau.py" "$AUS/bilder"
echo "Bilder in $AUS/bilder"
