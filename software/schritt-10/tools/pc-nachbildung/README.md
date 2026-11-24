# Nachbildung am PC (nur zum Prüfen)

In der Cloud gibt es keine Arduino IDE. Damit Änderungen an `evi-handy` trotzdem vor dem Aufspielen geprüft werden können:

```
bash software/schritt-10/tools/pc-nachbildung/pruefen.sh
```

- `arduino_zusammensetzen.py` setzt `evi-handy.ino` und die anderen `.ino`-Dateien so zusammen wie die Arduino IDE (Hauptdatei zuerst, dann alphabetisch, Funktionsankündigungen vor die erste Funktion).
- `ersatz/`: sehr einfache Ersatz-Bibliotheken (Arduino, Wire, SPI, EEPROM, I2S, Adafruit GFX). Die Zeit läuft nur, wenn das Prüfprogramm sie weiterstellt. Die Schrift ist ein Ersatz aus DejaVu Sans Mono, nicht die echte 5×7-Schrift.
- `bilder.cpp`: Spiele, alle Momente, Begrüßung; speichert Bildschirmbilder als PBM.
- `ablauf.cpp`: Bildschirm geht aus, Besuch schaltet ihn ein, Begrüßung beim Aufwecken, kleiner Moment.
- `tasten.cpp`: Tastenlogik (`tasten.h`) mit einem nachgebildeten MCP23017 (`ersatz/Wire.h`, mit Merken von Änderungen in INTF/INTCAP): kurzes Antippen, Festhalten mit Wiederholung, Flackern beim Loslassen, zwei Tasten zugleich.
- `bewegung.cpp`: Lagesensor (nachgebildeter QMI8658 in `ersatz/Wire.h`, `simLage`): Gassi gehen, schütteln, kippen, aufs Gesicht legen, „Wie war dein Tag?“, Kekse fangen, Schnick, Schnack, Schnuck.
- `umgebung.cpp`: Jahreszeiten, Weihnachtszeit und Wetter; fotografiert das Zimmer in allen Kombinationen.
- `kuscheln.cpp`: Touch zum Kuscheln (nachgebildeter Touch-Chip CST328 in `ersatz/Wire.h`, `simFinger`), Kuschelwunsch, Menü ohne Uhr/Name/Hilfe, kleine Uhrzeit, trüber Regenhimmel.
- Das Display ist mit nachgebildet (`ersatz/SPI.h`: Spalten, Zeilen, Schreiben). Nach jedem Bild wird verglichen, ob auf dem Display genau das Bild des Programms steht; so wird geprüft, dass `zeigen()` alles Geänderte schickt.

Ersetzt nicht den Test am Board: Ton, echter Touch, Tasten und Geschwindigkeit gibt es nur dort.
