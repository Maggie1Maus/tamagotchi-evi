# Schritt 1: Unser Tier begruessen

Ziel: Ein Tier erscheint auf dem virtuellen Display, blinzelt und winkt bei einem Tastendruck fuer etwa 1,5 Sekunden. Die Figur ist ein Platzhalter; ihr Aussehen legen wir spaeter gemeinsam fest.

## Bereits ausprobiert

Am 16. November 2026 in Wokwi erfolgreich kompiliert und gestartet. Nach einem Fehlerbericht wurde nachvollzogen, dass die urspruengliche Abfrage kurze Mausklicks uebersehen konnte; der erste Test hatte nur laengeres Druecken abgedeckt. Die Eingabe merkt Tastendruecke nun per Interrupt vor und gibt die Taste nach ruhigem Loslassen erneut frei. Mehrere einzelne kurze Mausklicks, langes Gedrueckthalten ohne Mehrfachzaehlung, die Leertaste und die Rueckkehr zur normalen Anzeige wurden mit der korrigierten Version im Browser geprueft. Die echte Hardware wurde noch nicht getestet.

Die vorbereitete Browsersitzung ist zum Ausprobieren geoeffnet. Sie ist noch nicht als Onlineprojekt gespeichert. Die Dateien in diesem Ordner sind die dauerhafte Sicherung; mit der folgenden Anleitung laesst sich die Simulation wiederherstellen.

## Im Browser starten

1. https://wokwi.com/projects/new/arduino-nano oeffnen.
2. Den Inhalt von `sketch.ino` in den gleichnamigen Editor-Reiter kopieren.
3. Den Inhalt von `diagram.json` in den gleichnamigen Reiter kopieren. Dadurch erscheinen Display und Taste mit ihren Verbindungen.
4. Ueber das kleine Pfeilmenue neben den Dateireitern eine neue Datei namens `pet.h` anlegen und den Inhalt unserer Datei hineinkopieren.
5. Unter **Library Manager** mit **+** nach **U8g2** suchen und diese Bibliothek hinzufuegen. Wokwi legt dann `libraries.txt` an; unsere gleichnamige Datei dokumentiert die benoetigte Bibliothek.
6. Den gruenen Startknopf der Simulation anklicken.
7. Den gruenen Taster **Hallo** betaetigen. Alternativ einmal in die Schaltung klicken und die Leertaste druecken.

Erwartung: Oben steht erst **Dein erstes Tier**, beim Druecken **Hallo zurueck!**. Das Tier hebt einen Arm und huepft kurz. Unten zaehlt **Hallo: 0, 1, 2, ...** die erkannten Begruessungen; so bleibt der Tastendruck auch nach der kurzen Animation erkennbar. Beim Gedrueckthalten wird die Begruessung nicht staendig wiederholt; erneutes Druecken startet sie wieder. Der Zaehler beginnt beim Neustart von null.

## Was wir hier vorbereiten

- `sketch.ino`: Start, Eingabe und Zeichnen auf dem Display.
- `pet.h`: Der Zustand des Tieres, unabhaengig von der Hardware.
- `diagram.json`: Der virtuelle Aufbau.
- `libraries.txt`: Die Displaybibliothek.

Dieser Schritt enthaelt noch keine Beduerfnisse, kein Menue und keinen gespeicherten Spielstand. Beim Neustart beginnt er von vorn. Als naechster Lernschritt bietet sich ein Saettigungswert mit einer Fuetteraktion an.

## Bezug zum bestellten Cube

Wir simulieren einen Nano und ein SSD1306-I2C-Display mit 128 x 64 Pixeln. Die Displayansteuerung, die Adresse und insbesondere der Taster an D2 sind vorlaeufige Annahmen fuer die Simulation. Vor dem Uebertragen auf den Cube muessen wir diese mit dessen Anleitung und dem gelieferten Aufbau abgleichen. Die Simulation ist keine Verdrahtungsanleitung fuer den Cube.

Die Bibliothek zeichnet in kleinen Seiten, damit der knappe Arbeitsspeicher des Nano geschont wird. Die Displayuebertragung braucht trotzdem Zeit. Deshalb merkt ein Interrupt an D2 den Tastendruck auch waehrend des Zeichnens vor. Der Interrupt setzt nur eine Markierung; Tierzustand und Bild werden im normalen Programmablauf aktualisiert. Bei einer spaeteren anderen Tasterbelegung muss auch die passende Eingabemethode geprueft werden.

## Offizielle Dokumentation

- Nano: https://docs.wokwi.com/parts/wokwi-arduino-nano
- Display: https://docs.wokwi.com/parts/board-ssd1306
- Taster und Tastatursteuerung: https://docs.wokwi.com/parts/wokwi-pushbutton
- Bibliotheken: https://docs.wokwi.com/guides/libraries
- Arduino-Interrupts: https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/

Stand: 16. November 2026. Der echte Cube wurde noch nicht aufgebaut oder getestet.
