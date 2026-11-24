# Raumtiefe für Evi und neuer Zimmerentwurf
Stand: 24. November 2026

## Wunsch der Nutzerin

Evi soll grundsätzlich im ganzen Tamagotchi nach vorne und hinten gehen können, im Garten etwa zum Blumenbeet und ebenso im Zimmer. Ihre dargestellte Größe soll zur Entfernung passen. Zusätzlich ist ein Zimmer-Gesamtentwurf im Stil des bestätigten Gartens gewünscht. Dies ist zunächst ein Vorschlag; die allgemeine Tiefenbewegung ist noch nicht implementiert.

## Neuer Zimmerentwurf

Zwei aufeinander aufbauende Entwürfe unter `entwuerfe/zimmer/raumtiefe/`. Empfohlen ist `mehr_raumtiefe.png`: dieselbe feine, natürliche schwarz-weiße Linienführung wie im Garten, vertraute Möbel und mehr freie Bodenfläche mit räumlicher Perspektive. Sideboard mit Kerzen und Fenster links hinten; Tisch mit Topfpflanze und Wandregal rechts hinten; Teppich und Napf vorne. Keine zusätzliche Möblierung und keine neue Evi-Zeichnung. Noch nicht von der Nutzerin ausgewählt oder in die Firmware eingebaut.

## Vorschlag für die allgemeine Tiefenbewegung

- Jede Figur hat neben links/rechts eine Position auf dem Boden in der Tiefe. Die Füße bleiben auf diesem Boden verankert; die Figur wird von den Füßen aus skaliert.
- Die Größe verändert sich beim Laufen weich und kontinuierlich. Drei Bereiche dienen nur als Gestaltungsorientierung: hinten etwa 65–70 %, mittig etwa 85 %, vorne 100 % der jeweiligen Altersstufen-Größe. Diese Zahlen sind Vorschlagswerte und noch nicht abgestimmt.
- Die Altersstufe bestimmt die Grundgröße. Entfernung skaliert anschließend alle Altersstufen gleichmäßig, sodass die Größenverhältnisse von Baby, Teenager, Erwachsenen und Oma erhalten bleiben.
- Ein Objekt hat eine Vorderkante am Boden. Ist Evi dahinter, wird sie vor dieser Vordergrundebene gezeichnet und entsprechend verdeckt; steht sie davor, erscheint sie vollständig vor dem Objekt. Immer von hinten nach vorne zeichnen.
- Gartenbeispiel: Evi läuft erst zur Stelle und dann etwas in die Tiefe ins Beet, wird kleiner; die Blumen beziehungsweise vordere Beetkante verdecken ihre Pfoten teilweise. Bei der Rückkehr auf die Wiese wird sie wieder größer.
- Zimmerbeispiel: Evi läuft zum hinteren Fenster/Sideboard kleiner, kehrt über eine sichtbare freie Bodenfläche zum vorderen Teppich größer zurück. Seitliche Laufwege führen um feste Möbel herum. Kein Durchlaufen von Tischplatten oder Sideboard.
- Die Steuerung bleibt wie gewünscht über Befehle und Evis selbstständige Bewegungen. Laufziele und Zwischenpunkte genügen; die Nutzerin muss keine Figur direkt steuern.

## Umsetzungsschritte nach Auswahl des Zimmerstils

1. Die gewählte Gesamtszene in Raumgrund, Möbel und Vorderkanten, Fensterhimmel/-ausblick und optionale Dekoration aufteilen. Den Gesamteindruck dabei wie beim Garten zusammenhängend erhalten.
2. Einen gemeinsamen Zeichen- und Bewegungsmechanismus mit Tiefenposition, Skalierung, Bodenankern und korrekter Zeichenreihenfolge für Garten und Zimmer einführen. Erst dann Laufziele der bestehenden Momente, Sideboard und Suche darauf umstellen.
3. Vorhandene Wetter-/Saison- und Weihnachtsfunktionen, Bilder aller Altersstufen, Bedürfnisse, Touch/Tasten und Funddaten erhalten. Keine Änderung des Spielstandformats für reine Darstellungspositionen nötig.
4. Auf dem 320×240-Display prüfen: Lesbarkeit kleiner Figuren, weiße Deckmasken, Spiegelung, Laufwege, Verdeckung, Trefferfläche bei Touch, Jahreszeiten und Bildschirm aus.

Die bestehenden Garten- und Zimmerfunktionen bleiben bis zur eigentlichen Umsetzung unverändert.
