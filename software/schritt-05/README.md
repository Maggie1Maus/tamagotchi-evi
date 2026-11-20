# Schritt 5: Evi auf dem echten Funduino Cube

Ab hier läuft Evi auf dem selbst gelöteten Cube statt nur im Simulator. Die Anschlüsse stammen aus der Platinenbeschriftung der offiziellen Lötanleitung (siehe [Pinbelegung](../../Tamagotchi_Hardware_Cube.md#pinbelegung-des-cube)).

| Ordner | Wozu |
| --- | --- |
| `cube-test` | **Zuerst aufspielen.** Prüft Display, Taster, Lautsprecher, LEDs und Fotowiderstand. |
| `evi-cube` | Evi mit der Bedienung aus Schritt 4, jetzt mit den echten Anschlüssen, Tönen, LEDs und Schlafen bei Dunkelheit. |

Gebraucht werden nur die schon gelöteten Teile: Nano, Display, Taster, Lautsprecher, die drei LEDs, Fotowiderstand und die Widerstände.

## Einmal vorbereiten

1. **Arduino IDE** installieren: [arduino.cc](https://www.arduino.cc/en/software) → herunterladen → installieren.
2. **Bibliothek U8g2** installieren: *Werkzeuge → Bibliotheken verwalten*, nach „U8g2“ suchen, *Installieren*.
3. Cube per USB anschließen. *Werkzeuge → Board → Arduino AVR Boards → Arduino Nano*. Unter *Werkzeuge → Port* den neuen COM-Port wählen.
4. Bricht das Hochladen mit einer Fehlermeldung ab (zum Beispiel „not in sync“): *Werkzeuge → Prozessor →* „ATmega328P (Old Bootloader)“ wählen und noch einmal hochladen.

## 1. Cube-Test

`software/schritt-05/cube-test/cube-test.ino` öffnen und mit dem Pfeil-Knopf hochladen.

So sollte es aussehen:

- Beim Start piept es kurz.
- Das Display zeigt „Cube-Test“ und daneben die Adresse (0x3C oder 0x3D).
- Die LEDs leuchten der Reihe nach: rot, gelb, grün.
- Taster drücken: Es erscheint „GEDRUECKT“, der Zähler steigt, es piept.
- Hand über den Fotowiderstand halten: Die Zahl bei „Licht A7“ wird kleiner.

| Was passiert | Wahrscheinliche Ursache |
| --- | --- |
| Display bleibt schwarz | Lötstellen an den vier Display-Pins. Im seriellen Monitor (9600 Baud) steht, ob das Display gefunden wurde. |
| Taster tut nichts | Lötstellen am Taster SW1 oder am Widerstand R4 |
| Kein Piepen | Lautsprecher verkehrt herum („+“ muss zu D7) oder eine Lötstelle |
| Eine LED bleibt dunkel | LED verkehrt herum (kurzes Bein zu „GND“) oder Lötstelle am Widerstand davor |
| Licht-Zahl ändert sich nicht | Lötstellen am Fotowiderstand (R5) oder an R6 |

**Licht-Zahl merken:** einmal bei normalem Zimmerlicht und einmal mit Hand darüber oder bei ausgeschaltetem Licht. Evi schläft unter **120** ein und wacht über **220** wieder auf. Passen die Zahlen nicht, oben in `evi-cube.ino` die Werte `DARK_BELOW` und `BRIGHT_ABOVE` ändern.

## 2. Evi

`software/schritt-05/evi-cube/evi-cube.ino` öffnen und hochladen. Die anderen Dateien im Ordner öffnet die IDE automatisch als Reiter.

| Bedienung am Taster | Befehl |
| --- | --- |
| Einmal kurz drücken | Weiter |
| Zweimal kurz hintereinander | Zurück |
| Etwa 0,7 Sekunden halten | Bestätigen |

Neu gegenüber Schritt 4:

- **Töne:** Startmelodie, ein leiser Klick bei jedem Tastendruck, eine Melodie beim Winken und beim Füttern, ein Hinweiston, sobald Evi Hunger bekommt.
- **LEDs:** grün = satt (über 50 %), gelb = bald hungrig (31–50 %), rot blinkend = Hunger.
- **Licht aus = Schlafenszeit:** Wird es dunkel, schließt Evi die Augen und zeigt „Zzz“. Das Display wird dunkler, die LEDs gehen aus, es piept nicht. Wird es wieder hell, wacht Evi mit einem Ton auf. Hat sie im Dunkeln Hunger bekommen, meldet sie sich nach dem Aufwachen.

Noch nicht fertig:

- Die Spielregeln laufen weiter im **Testtempo**: ein Punkt Sättigung pro Sekunde, nach etwa 30 Sekunden ist Evi hungrig.
- Kein Spielstand: Nach dem Ausstecken fängt Evi neu an.
- Die Figur ist noch der Platzhalter aus Schritt 4. Evis eigene Zeichnungen für das kleine Display sind der nächste Schritt.

## Technik

- Taster an **D9**, gedrückt = HIGH (R4 zieht nach GND). D9 hat keinen eigenen Interrupt, deshalb meldet ein Pin-Change-Interrupt kurze Klicks auch während der Displayübertragung.
- Lautsprecher an **D7**, LEDs an **D12** (rot), **D11** (gelb), **D10** (grün), Fotowiderstand an **A7**.
- Die Displayadresse (0x3C oder 0x3D) wird beim Start automatisch gesucht. Laut Funduino kommen beide vor.
- I²C-Takt 400 kHz. Zeigt das Display Streifen oder Fehler, die Zeile `display.setBusClock(400000);` löschen.
- `pet.h`, `menu.h`, `button_input.h`, `commands.h` und `checks.h` sind unverändert aus Schritt 4. Neu ist `melody.h` mit den Tönen.
- Der Prüflauf funktioniert wie in Schritt 4: `#define RUN_CHECKS` vor die Includes, Ausgabe im seriellen Monitor mit 115200 Baud.

## Geprüft (20. November 2026)

- Beide Programme mit avr-gcc 7.3 und dem Arduino-AVR-Kern 1.8.6 für den Nano (ATmega328P) übersetzt, ohne Warnungen. U8g2 war in dieser Umgebung nicht verfügbar und wurde durch einen Stellvertreter mit denselben Aufrufen ersetzt. Ohne U8g2 belegt Evi 7,7 KB Programmspeicher und 623 Byte Arbeitsspeicher, der Cube-Test 6,8 KB. U8g2 kommt mit schätzungsweise 10–14 KB dazu; der Nano hat 30 KB.
- Die 32 Prüfungen der Spielregeln aus `checks.h` am PC bestanden.
- 26 Prüfungen in einer Nachbildung des Sketches am PC bestanden: Taster an D9 über den Interrupt, Klick, Halten und Doppelklick, Töne, LED-Farben, Einschlafen und Aufwachen ohne Flackern, Hungerton erst nach dem Aufwachen und Displayadresse 0x3D.
- **Auf dem echten Cube (20. November 2026, laut Nutzerin):** `evi-cube` ließ sich in der Arduino IDE mit der echten U8g2-Bibliothek übersetzen und aufspielen. Anzeige, Taster, Hunger-LEDs, Hungerton und Fütter-Melodie funktionieren. Noch nicht ausprobiert: Einschlafen bei Dunkelheit und `cube-test`.
