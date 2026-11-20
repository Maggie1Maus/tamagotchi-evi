# Schritt 2: Hunger und Füttern

Das Tier hat jetzt einen Sättigungswert. Die Prozentzahl und der Balken oben zeigen, wie satt es ist. Der grüne Knopf **Fuettern** gibt ihm Futter; alternativ funktioniert die Leertaste, wenn die Schaltung den Tastaturfokus hat.

## Dein erster Spielversuch

1. Beobachte den sinkenden Sättigungswert.
2. Klicke einmal auf **Fuettern**. Der Wert steigt, das Tier freut sich kurz und sagt **Mjam! Danke.**
3. Warte, bis der Wert auf höchstens 30 Prozent sinkt. Das Tier bekommt einen traurigen Mund und bittet um Futter.
4. Füttere es wieder und beobachte den Gesichtsausdruck.

## Vorläufige Testregeln

- Start bei 60 Prozent Sättigung.
- Jede simulierte Sekunde sinkt der Wert um einen Prozentpunkt.
- Ein Tastendruck gibt 25 Punkte zurück, bis höchstens 100 Prozent.
- Langes Gedrückthalten füttert nur einmal. Für weiteres Futter loslassen und neu drücken.
- Ab einschließlich 30 Prozent gilt das Tier als hungrig.
- Bei null bleibt der Wert bei null. In diesem Lernschritt kann das Tier noch nicht sterben.

**TEST** auf dem Display kennzeichnet das beschleunigte Tempo. Das sind vorläufige Regeln zum Ausprobieren, keine beschlossenen Zeiten für das spätere Spiel. Es zählt die Zeit in der laufenden Simulation. Stoppen, Pausieren und Schließen sind noch kein Echtzeitbetrieb mit Speicherung. Beim Neustart beginnt der Wert wieder bei 60 Prozent.

## Dateien und Wiederherstellung

- `sketch.ino`: Display und Eingabe; die zuverlässige Tastenerkennung aus Schritt 1 bleibt erhalten.
- `pet.h`: Sättigung, Füttern, Zeitablauf und Zustände des Tieres.
- `diagram.json`: Nano, OLED und ein Taster in der Simulation.
- `libraries.txt`: Benötigte Bibliothek U8g2.

Zum Wiederherstellen ein [neues Nano-Projekt in Wokwi](https://wokwi.com/projects/new/arduino-nano) öffnen und `sketch.ino` sowie `diagram.json` durch die Dateien aus diesem Ordner ersetzen. Über das Pfeilmenü neben den Dateireitern `pet.h` als neue Datei anlegen und deren Inhalt einsetzen. Im Library Manager **U8g2** hinzufügen und die Simulation starten. Das Browserprojekt ist noch nicht dauerhaft online gespeichert; diese Dateien sind die lokale Sicherung.

## Geprüft

In Wokwi erfolgreich kompiliert und ausgeführt. Beobachtet wurden der zeitliche Rückgang der Sättigung, das hungrige Gesicht, das Stehenbleiben bei null, Füttern mit kurzem Mausklick, einmaliges Füttern bei 1,8 Sekunden Gedrückthalten, Füttern per Leertaste und die Begrenzung bei 100 Prozent. Nach dem Füttern verschwindet die kurze Rückmeldung wieder und der normale Gesichtsausdruck richtet sich nach dem aktuellen Wert. Die Vorschau liegt als `vorschau.jpg` in diesem Ordner.

Schritt 1 liegt weiterhin separat im Nachbarordner. Die Tiergrafik bleibt ein Platzhalter. Weitere Bedürfnisse, Menüs, Einrichtung, Uhrzeit und Spielstand folgen in eigenen Schritten. Die vorläufigen Anschlüsse und das Display müssen vor einer Übertragung auf den echten Cube mit der Anleitung und dem gelieferten Aufbau abgeglichen werden.

Stand: 16. November 2026. Noch kein Test am echten Cube.
