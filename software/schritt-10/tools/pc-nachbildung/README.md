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
- `bewegung.cpp`: Lagesensor (nachgebildeter QMI8658 in `ersatz/Wire.h`, `simLage`): Gehen und Schütteln lösen nichts aus (Gassi seit 25.11.2026 entfernt), kippen, aufs Gesicht legen, „Wie war dein Tag?“, Kekse fangen, Schnick, Schnack, Schnuck.
- `umgebung.cpp`: Jahreszeiten, Weihnachtszeit und Wetter; fotografiert das Zimmer in allen Kombinationen.
- `kuscheln.cpp`: Touch zum Kuscheln (nachgebildeter Touch-Chip CST328 in `ersatz/Wire.h`, `simFinger`), Kuschelwunsch, Menü ohne Uhr/Name/Hilfe, kleine Uhrzeit, trüber Regenhimmel.
- Das Display ist mit nachgebildet (`ersatz/SPI.h`: Spalten, Zeilen, Schreiben). Nach jedem Bild wird verglichen, ob auf dem Display genau das Bild des Programms steht; so wird geprüft, dass `zeigen()` alles Geänderte schickt.

Ersetzt nicht den Test am Board: Ton, echter Touch, Tasten und Geschwindigkeit gibt es nur dort.

- `garten.cpp`: Gartenmenü, Suche mit Hilfe per A/Touch, alle zwölf Lebensphasen-Posen, ältere Spaziergang-Funde behalten ihre Erinnerung, volle Sammlung, Version-2-Migration, Version-3-Metadaten, Jahreszeiten- und Wetterbilder, Heimkehr/RESET/Schlafenszeit. Version 1 wird zusätzlich in `sideboard.cpp` geprüft.

- `raumtiefe.cpp`: alle Altersstufen nah/mittel/fern, Bodenanker und stetige Skalierung, Originalpixel-Verdeckung, Touch, begehbare Wege, Garten-Tiefengang/Rückkehr, aktueller Fensterausblick bei Regen/Schnee tags/nachts, RESET und Minispielkoordinaten.

- `schlaf.cpp`: Schlafmodus (`schlaf.h`, seit 25.11.2026). Hier vergeht beim Schlafen auch die Zeit, wie am Board. Helligkeit 50 %, Pausen hell und dunkel, keine Pausen mit Finger oder im Spiel, langsamer Takt im Dunkeln, Bildschirm-Chip schläft und wacht (`ersatz/SPI.h` merkt sich 0x10/0x11/0x28/0x29), Ton-Chip nur bei Tönen an (`ersatz/I2S.h` spielt jetzt mit der Zeit ab), Wecken mit Taste und Antippen, Gassi im Dunkeln. `ersatz/Arduino.h` merkt sich die Helligkeit (`simLicht`).
  Seit der zweiten Fassung (25.11.2026): dunkel nur etwa 8 Runden pro Sekunde, Touch-Chip im Reset (`simTouchRst` in `ersatz/Arduino.h`), Antippen weckt nicht, Taste weckt, höchstens 20 ms Pause während eines Tons, Umdrehen weckt nach etwa 0,5 s.
