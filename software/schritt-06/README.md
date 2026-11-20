# Schritt 6: Evi im echten Tempo

Gleich wie Schritt 5, aber Evi wird nicht mehr in Sekunden hungrig, sondern in Stunden. Die Werte entsprechen der erwachsenen Evi in der Handy-App (`handy-app/dist/pet.js`).

## Aufspielen

In der Arduino IDE `software/schritt-06/evi-cube/evi-cube.ino` öffnen und mit dem Pfeil hochladen. Board und Port bleiben wie bei Schritt 5.

## So schnell wird Evi hungrig

| Was | Wert |
| --- | --- |
| Start | 80 % satt |
| Hunger sinkt | 20 Punkte pro Stunde, also 1 Punkt alle 3 Minuten |
| Im Schlaf (Licht aus) | nur 15 % davon: 1 Punkt alle 20 Minuten, eine Nacht von 8 Stunden kostet 24 Punkte |
| Füttern | +40 Punkte, höchstens 100 % |
| Hunger (rote LED, Piep) | ab 30 % |

Beispiel ab dem Einschalten: grün bis 1,5 Stunden, dann gelb, nach 2,5 Stunden rot mit Hinweiston. Einmal Füttern reicht für etwa 2 Stunden.

Die Zahlen stehen oben in `pet.h` und lassen sich dort ändern.

## Was sich geändert hat

- `pet.h`: echtes Tempo, langsamer im Schlaf, Füttern +40 statt +25, Start bei 80 statt 60.
- `evi-cube.ino`: Beim Einschlafen und Aufwachen bekommt Evi Bescheid. Die Anzeige „TEST“ oben rechts ist weg.
- `checks.h`: Prüfungen für das neue Tempo, den Schlaf und lange Pausen.
- Alles andere ist unverändert aus Schritt 5. Der Cube-Test liegt weiter unter `software/schritt-05/cube-test`.

## Noch offen

- **Kein Spielstand:** Nach dem Ausstecken fängt Evi wieder bei 80 % an. Die Zeit ohne Strom kann der Cube nicht zählen, weil er keine Uhr mit Batterie hat.
- Die Figur ist noch der Platzhalter.

## Geprüft (20. November 2026)

- Für den Nano übersetzt (ohne echte U8g2, wie in Schritt 5): 8,0 KB Programm, 631 Byte Arbeitsspeicher.
- 42 Prüfungen aus `checks.h` am PC bestanden, darunter das neue Tempo, 8 Stunden Schlaf und der Überlauf der Uhr nach 49 Tagen.
- 12 Prüfungen in einer Nachbildung des Sketches über mehrere Stunden Spielzeit bestanden: 60 % nach einer Stunde, gelb nach 1,5 Stunden, Hunger mit Ton nach 2,5 Stunden, Füttern +40, 24 Punkte in einer dunklen Nacht.
- Noch nicht auf dem echten Cube getestet.
