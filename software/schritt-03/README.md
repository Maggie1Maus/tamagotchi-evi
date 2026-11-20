# Schritt 3: Winken und Füttern im Menü

Das Tier kann jetzt wieder winken. Beide Aktionen stehen gleichzeitig zur Verfügung: **Winken** und **Fuettern**. Die hell hinterlegte Aktion ist ausgewählt. Beim Start ist das **Winken**.

## Bedienung mit einer Taste

- **Kurz drücken und loslassen:** Zur anderen Aktion wechseln.
- **Etwa 0,7 Sekunden gedrückt halten:** Die ausgewählte Aktion ausführen. Ein kleiner Haltebalken zeigt den Fortschritt.
- Sobald **Loslassen** erscheint, ist die Aktion ausgeführt. Die Taste kann losgelassen werden; die Auswahl bleibt erhalten.
- Weiteres Gedrückthalten löst keine zweite Aktion aus. Ein neuer langer Druck führt die ausgewählte Aktion erneut aus.

Das funktioniert mit dem grünen Knopf und mit der Leertaste bei fokussierter Schaltung. Die neue Bedienung ersetzt das direkte Füttern per kurzem Klick aus Schritt 2.

## Zum Ausprobieren

1. Halte den Knopf, bis das Tier mit **Hallo!** winkt. Dabei erhält es kein Futter.
2. Lass los und drücke einmal kurz: **Fuettern** wird ausgewählt.
3. Halte den Knopf erneut, bis **Mjam!** erscheint. Die Sättigung steigt um bis zu 25 Punkte.
4. Nach dem Loslassen bleibt **Fuettern** ausgewählt. Du kannst durch erneutes Halten nochmals füttern oder per kurzem Druck zu **Winken** wechseln.

Die Hungerregeln bleiben im beschleunigten Testtempo: Start bei 60 Prozent, ein Punkt weniger je simulierter Sekunde, hungrig bei höchstens 30 Prozent. Der Wert liegt zwischen null und 100. Beim Winken läuft die Zeit weiter; deshalb kann die Sättigung dabei weiter sinken. Tod und Spielstandspeicherung sind noch nicht umgesetzt.

## Dateien und Wiederherstellung

In einem [Nano-Projekt in Wokwi](https://wokwi.com/projects/new/arduino-nano) `sketch.ino` und `diagram.json` durch die Dateien aus diesem Ordner ersetzen, `pet.h` anlegen beziehungsweise ersetzen und im Library Manager **U8g2** hinzufügen. Danach starten. Die Dateien hier sind die lokale Sicherung; das Browserprojekt ist noch nicht dauerhaft online gespeichert.

- `pet.h`: Hunger, Füttern und Begrüßen ohne Abhängigkeit vom Display.
- `sketch.ino`: Darstellung, Menü und Unterscheidung zwischen kurzem und langem Drücken.
- `diagram.json`: Virtueller Aufbau mit Nano, OLED und einem Taster.
- `libraries.txt`: Die Displaybibliothek.

Die verlässliche Erkennung kurzer Klicks per Interrupt bleibt erhalten. Nach ruhigem Loslassen wird der Knopf für den nächsten Druck freigegeben. Schritt 1 und Schritt 2 liegen weiterhin separat in ihren bisherigen Ordnern.

## Geprüft

In Wokwi erfolgreich kompiliert und gestartet. Geprüft wurden Winken durch einen langen Mausklick ohne Sättigungszuwachs, Auswahlwechsel durch einen sehr kurzen Mausklick ohne Fütterung, Füttern durch zwei Sekunden Halten mit nur einer Fütterung, Beibehalten der Auswahl nach der Aktion sowie kurzer und langer Druck der Leertaste. Nach einem Neustart ist wieder Winken ausgewählt und die Sättigung startet bei 60 Prozent. Die Hungeranzeige und ihre Zustandsänderungen laufen während der Menübedienung weiter. `vorschau.jpg` zeigt das gestartete Menü.

Die Anschlüsse sind weiterhin nur für die Simulation vorbereitet. Vor dem Übertragen auf den echten Cube sind Anleitung und gelieferter Aufbau abzugleichen. Das spätere Gerät soll weiterhin drei physische Knöpfe bekommen.

Stand: 16. November 2026. Noch kein Test am echten Cube.
