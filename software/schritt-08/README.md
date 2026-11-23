# Schritt 8: Erster Test mit dem Waveshare-Board

Das Programm `waveshare-test` prüft das neue Board **Waveshare RP2350-Touch-LCD-2.8**, bevor Evi darauf umzieht:

- **Bildschirm** im Querformat 320 × 240, schwarz-weiß: oben „Hallo Evi!“
- **Touch:** Im großen Feld mit dem Finger malen. Oben rechts steht, welcher Touch-Chip gefunden wurde.
- **Ton:** Beim Start spielt eine kleine Melodie, der Knopf **TON** piept. **NEU** leert das Malfeld.
- Die Zeile unter der Überschrift zeigt, wo der Finger erkannt wurde.

Der Lautsprecher, der dem Board beiliegt, wird in den kleinen zweipoligen Lautsprecher-Stecker am Board gesteckt. Ohne Lautsprecher funktioniert alles außer dem Ton.

## Einmalig: Arduino IDE vorbereiten

1. **Board-Paket:** Die Erweiterung „Raspberry Pi Pico/RP2040/RP2350“ von Earle Philhower ist auf dem Haupt-PC seit dem 23. November 2026 installiert (Version 6.4.0). Auf einem anderen PC: *Datei → Einstellungen → Zusätzliche Boardverwalter-URLs* und dort eintragen:
   `https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json`
   Dann *Werkzeuge → Board → Boardverwalter*, nach **pico** suchen und „Raspberry Pi Pico/RP2040/RP2350“ installieren.
2. **Bibliothek:** „Adafruit GFX Library“ (ist auf dem Haupt-PC schon da). Sonst über *Werkzeuge → Bibliotheken verwalten* installieren und „Alle installieren“ wählen.
3. **Board auswählen:** *Werkzeuge → Board → Raspberry Pi Pico/RP2040/RP2350 → Raspberry Pi Pico 2*. Wichtig ist die **2**: Mit „Raspberry Pi Pico“ meldet die IDE nur 2 MB Speicher, und das Board nimmt das Programm nicht an.

Beim ersten Start nach der Installation kann die IDE eine Weile nur das Arduino-Logo zeigen. Abwarten oder neu starten.

## Aufspielen

1. `software/schritt-08/waveshare-test/waveshare-test.ino` öffnen.
2. **Beim ersten Mal** muss das Board in den Lademodus: die kleine Taste **BOOT** gedrückt halten, das USB-Kabel einstecken und die Taste loslassen. Am PC erscheint ein Laufwerk (z. B. „RP2350“).
3. *Werkzeuge → Port:* **UF2 Board** auswählen.
4. Mit dem **Pfeil** hochladen. Danach startet das Programm von selbst.

Steht bei „Port“ kein „UF2 Board“: *Sketch → Kompilierte Binärdatei exportieren* und die `.uf2`-Datei aus dem Unterordner `build` auf das Laufwerk des Boards ziehen.

Ab dem zweiten Mal reicht meist der Pfeil, der Port heißt dann „COM…“. Klappt es nicht, wieder mit BOOT starten.

Das Malprogramm vom Werk wird dabei überschrieben. Waveshare stellt es auf seiner Wiki-Seite zum Download bereit, falls es einmal gebraucht wird.

## Ergebnis am Board (23. November 2026, laut Nutzerin)

- „Hallo Evi!“ steht richtig herum, wenn der USB-Stecker **rechts** ist. `BILD_DREHEN` bleibt 0.
- Der Malpunkt liegt genau unter dem Finger, die Spiegel-Schalter bleiben 0.
- Touch-Chip: **CST328 (V1)**.
- Melodie, TON und NEU funktionieren. Der Ton war recht laut; `LAUTSTAERKE` danach von 2000 auf 700 gesenkt (noch nicht angehört).

## Was zurückmelden

- Steht „Hallo Evi!“ richtig herum und lesbar da? Wenn das Bild auf dem Kopf steht: in `bildschirm.h` `BILD_DREHEN` auf `1` setzen.
- Erscheint der Malpunkt genau unter dem Finger? Sonst bitte sagen, in welche Richtung er falsch liegt. In `touch.h` gibt es dafür `TOUCH_SPIEGELN_X` und `TOUCH_SPIEGELN_Y`.
- Welcher Touch-Chip steht oben rechts?
- Hörst du die Melodie beim Start und den Piepton bei TON? Ist er zu laut oder zu leise? Die Lautstärke steht in `ton.h` (`LAUTSTAERKE`).

## Technik

- `board.h`: alle Anschlüsse. Sie stammen aus Waveshares Beispielprogramm „Demo V2“ vom 14. Oktober 2026 (`DEV_Config.h`, `bsp_*.h`). Bildschirm an SPI1 (10, 11, 13, 14, 15), Beleuchtung 16, Touch, Uhr und Lagesensor an I²C1 (6, 7), Touch-Reset 17 und -Meldung 18, Ton über I²S (2, 3, 4).
- `bildschirm.h`: eigener kleiner Treiber für den ST7789T3 mit Waveshares Einstellungen. Gezeichnet wird in ein 1-Bit-Bild (9,6 KB, Adafruit GFX `GFXcanvas1`). `zeigen()` überträgt es als Schwarz und Weiß. Das passt zu Evis Zeichnungen und lässt später Farbe zu.
- `touch.h`: Seit Oktober 2026 baut Waveshare den Touch-Chip **CST3530** ein, vorher den **CST328**. Das Programm erkennt den Chip an seiner I²C-Adresse (0x58 bzw. 0x1A). Der CST3530 wird gelesen wie in Waveshares Beispiel. Der CST328 ist nach dessen Registerbelegung geschrieben. **Das Board der Nutzerin hat den CST328 (V1)**, obwohl es im November 2026 gekauft wurde.
- `ton.h`: Rechtecktöne über die I²S-Bibliothek des Board-Pakets, absichtlich leise.
