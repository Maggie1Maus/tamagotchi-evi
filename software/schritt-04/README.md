# Schritt 4: Weiter, Bestätigen und Zurück

Die Bedienung mit einem Knopf erzeugt jetzt drei allgemeine Befehle. Das Menü kennt die Anzahl der Knöpfe nicht mehr. Für das spätere Gerät können drei einzelne Taster dieselben Befehle liefern.

| Bedienung am Cube-Prototyp | Befehl |
| --- | --- |
| Einmal kurz drücken | Weiter |
| Zweimal kurz hintereinander drücken | Zurück |
| Etwa 0,7 Sekunden halten | Bestätigen |

Die Leertaste funktioniert bei fokussierter Schaltung genauso. Nach einem Einzelklick wartet das Programm etwa 300 Millisekunden auf einen zweiten Klick. Währenddessen wird noch keine Auswahl verändert. Zwei getrennte Klicks mit einer längeren Pause werden als zwei Einzelklicks behandelt.

## Zum Ausprobieren

1. Du startest **Zuhause** beim Tier. Ein kurzer Druck oder langes Halten öffnet die **Aktionsansicht**.
2. Dort wechselt ein einzelner kurzer Druck zwischen **Winken** und **Fuettern**. Die helle Hinterlegung zeigt die Auswahl.
3. Halten führt die ausgewählte Aktion einmal aus. Danach bleibt die Auswahl erhalten.
4. Zwei kurze Klicks hintereinander führen wieder **Zuhause** zurück. Die Aktionsauswahl verschwindet. Bereits gegebenes Futter bleibt erhalten.
5. Zuhause hat ein weiterer Zurück-Befehl keine Auswirkung. Beim erneuten Öffnen der Aktionen ist die vorige Auswahl noch markiert.

Beim Halten hilft ein Fortschrittsbalken. Sobald **Loslassen** erscheint, ist der Befehl ausgeführt. Ein kurzer Druck direkt gefolgt von langem Halten zählt als **Weiter, dann Bestätigen**, auch wenn die Wartezeit für den ersten Klick noch nicht abgelaufen war.

Die Hungerregeln bleiben im schnellen Testtempo: Start bei 60 Prozent, ein Punkt weniger je simulierter Sekunde, bis zu 25 Punkte pro Fütterung, hungrig bei höchstens 30 Prozent. Der Wert bleibt zwischen null und 100. Winken gibt kein Futter. Tod und Spielstandspeicherung sind weiterhin nicht umgesetzt.

## Aufbau für den späteren Wechsel auf drei Knöpfe

- `commands.h`: Die gemeinsamen Befehle **Next**, **Confirm**, **Back**.
- `button_input.h`: Übersetzt Einzelklick, Doppelklick und Halten in diese Befehle. Zeit und Knopfzustand kommen von außen; die Datei enthält keine Arduino-Anschlüsse.
- `menu.h`: Verarbeitet diese Befehle, merkt sich Ansicht und Auswahl und gibt gegebenenfalls eine Spielaktion zurück. Die Datei kennt keine Knöpfe, Displaybibliothek oder Bedürfnisse.
- `pet.h`: Hunger, Füttern und Winken, unabhängig von der Bedienung.
- `sketch.ino`: Verbindet Eingabe, Menü, Tier und Darstellung mit der simulierten Hardware.
- `diagram.json` und `libraries.txt`: Virtueller Aufbau und Displaybibliothek.
- `checks.h`: Verhaltensprüfungen für Klickfolgen, Menü und Spielregeln; nur im gesonderten Prüflauf aktiv.

Mit drei Knöpfen wird später eine andere Eingabeanbindung benötigt. Sie liefert dieselben Befehle direkt. Die Menü- und Spielregeln müssen dafür nicht an drei bestimmte Anschlüsse gekoppelt werden. Die konkrete Belegung der drei Knöpfe wird mit der späteren Hardware festgelegt.

## Wiederherstellen und prüfen

In einem [Nano-Projekt in Wokwi](https://wokwi.com/projects/new/arduino-nano) `sketch.ino` und `diagram.json` ersetzen, die Dateien `pet.h`, `commands.h`, `button_input.h` und `menu.h` über das Dateimenü anlegen und ihre Inhalte einsetzen. Im Library Manager **U8g2** hinzufügen. Dann starten. Die lokale Kopie ist die dauerhafte Sicherung; die Browsersitzung ist noch nicht online gespeichert.

Für den optionalen gesonderten Prüflauf zusätzlich `checks.h` anlegen und vor den Includes in `sketch.ino` die Zeile `#define RUN_CHECKS` ergänzen. Die Ergebnisse erscheinen im seriellen Monitor. Für den normalen Spielbetrieb diese Zeile wieder entfernen. Die ausgelieferte `sketch.ino` aktiviert den Prüflauf nicht.

Die Eingabe wird auch zwischen Displayseiten abgefragt, während jede Seite dasselbe zuvor kopierte Bild zeigt. So bleibt die Anzeige konsistent und die Knopferkennung kann die Pause zwischen zwei Klicks erfassen. Die Displayübertragung ist in der Simulation mit 400 kHz vorbereitet. Anschlüsse, Interrupt und Displaytakt müssen vor dem Übertragen mit Anleitung und geliefertem Cube abgeglichen werden.

Schritt 1 bis 3 bleiben unverändert in ihren bisherigen Ordnern erhalten.

## Geprüft

Der gesonderte Prüflauf wurde auf dem simulierten Nano mit **32 bestandenen Prüfungen und null Fehlern** ausgeführt. Er prüft unter anderem verzögerten Einzelklick, Doppelklick ohne zusätzlichen Weiter-Befehl, sehr schnelle Doppelklicks mit 15 Millisekunden Loslasspause, langes Halten ohne Wiederholung, kurz gefolgt von lang, Kontaktprellen, Zeitgeberüberlauf, Menübefehle ohne konkrete Knöpfe sowie Füttern und Hungergrenzen. Die Prüfausgaben liegen im Flash-Speicher, um den knappen Arbeitsspeicher des Nano zu schonen.

Anschließend wurde die normale Version ohne aktivierten Prüflauf neu kompiliert. Direkt im Browser geprüft: Öffnen der Aktionen, Winken, Wechsel zu Füttern, einmaliges Füttern bei 1,8 Sekunden Halten, Doppelklick zurück nach Hause ohne Verlust der Sättigung, weiterer Doppelklick zuhause ohne unbeabsichtigtes Öffnen der Aktionen, erneutes Öffnen mit erhaltener Auswahl sowie Halten und Doppelklick mit der Leertaste. Nach der Korrektur werden auch sehr schnelle Loslasspausen ab zehn Millisekunden erkannt. Die Vorschau liegt als `vorschau.jpg` vor.

Stand: 16. November 2026. Noch kein Test am echten Cube.
