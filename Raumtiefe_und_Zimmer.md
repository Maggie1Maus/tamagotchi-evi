# Raumtiefe für Evi und neuer Zimmerentwurf
Stand: 24. November 2026

## Wunsch der Nutzerin

Evi soll grundsätzlich im ganzen Tamagotchi nach vorne und hinten gehen können, im Garten etwa zum Blumenbeet und ebenso im Zimmer. Ihre dargestellte Größe soll zur Entfernung passen. Zusätzlich ist ein Zimmer-Gesamtentwurf im Stil des bestätigten Gartens gewünscht. Die Nutzerin hat die Umsetzung mit „Ok dann leg los“ beauftragt; die gemeinsame Tiefenbewegung ist jetzt in Zimmer und Garten implementiert.

## Neuer Zimmerentwurf

Zwei aufeinander aufbauende Entwürfe unter `entwuerfe/zimmer/raumtiefe/`. Die Nutzerin mag beide Entwürfe, bevorzugt aber `erste_fassung.png`: gemütlicher, kompakter und passend zu Evis kleinem Zuhause. `mehr_raumtiefe.png` wirkt ihr überdimensional und zu groß und ist nicht die Grundlage der weiteren Gestaltung. Den ersten Entwurf mit seinen Möbelgrößen und Proportionen erhalten. Tiefe auf der bestehenden Bodenfläche durch Wege, Skalierung und Verdeckung erzeugen; das Zimmer dafür nicht aufweiten. Sideboard mit Kerzen und Fenster links hinten; Tisch mit Topfpflanze und Wandregal rechts hinten; Teppich und Napf vorne. Keine zusätzliche Möblierung und keine neue Evi-Zeichnung. Die kompakte erste Fassung ist als gestalterische Grundlage ausgewählt, und in die Firmware eingebaut.

## Implementierte allgemeine Tiefenbewegung

- Jede Figur hat neben links/rechts eine Position auf dem Boden in der Tiefe. Die Füße bleiben auf diesem Boden verankert; die Figur wird von den Füßen aus skaliert.
- Die Größe verändert sich beim Laufen weich und kontinuierlich. Die Perspektive verändert sich kontinuierlich zwischen 65 % hinten und 100 % vorne. Die Grundhöhe pro Altersstufe vorne beträgt 54/64/68/64 Pixel; Proportionen der jeweiligen Pose bleiben erhalten.
- Die Altersstufe bestimmt die Grundgröße. Entfernung skaliert anschließend alle Altersstufen gleichmäßig, sodass die Größenverhältnisse von Baby, Teenager, Erwachsenen und Oma erhalten bleiben.
- Ein Objekt hat eine Vorderkante am Boden. Ist Evi dahinter, wird sie vor dieser Vordergrundebene gezeichnet und entsprechend verdeckt; steht sie davor, erscheint sie vollständig vor dem Objekt. Immer von hinten nach vorne zeichnen.
- Gartenbeispiel: Evi läuft erst zur Stelle und dann etwas in die Tiefe ins Beet, wird kleiner; die Blumen beziehungsweise vordere Beetkante verdecken ihre Pfoten teilweise. Bei der Rückkehr auf die Wiese wird sie wieder größer.
- Zimmerbeispiel: Evi läuft zum hinteren Fenster/Sideboard kleiner, kehrt über eine sichtbare freie Bodenfläche zum vorderen Teppich größer zurück. Seitliche Laufwege führen um feste Möbel herum. Kein Durchlaufen von Tischplatten oder Sideboard.
- Die Steuerung bleibt wie gewünscht über Befehle und Evis selbstständige Bewegungen. Laufziele und Zwischenpunkte genügen; die Nutzerin muss keine Figur direkt steuern.

## Umsetzung mit dem ausgewählten kompakten Zimmer

1. Ausgewählte kompakte Gesamtszene und Weihnachtsfassung übernommen; Vordergrundstempel für die vorhandenen Möbel und Beetkanten sowie separater lebendiger Fensterausblick.
2. Gemeinsamen Zeichen- und Bewegungsmechanismus mit Tiefenposition, Skalierung, Bodenankern und Verdeckung für Garten und Zimmer eingeführt; bestehende Momente, Sideboard und Suche umgestellt.
3. Wetter/Saisonen, Weihnachtszeitraum, Bilder aller Altersstufen, Bedürfnisse, Touch/Tasten und Funddaten erhalten. Reine Darstellungspositionen bleiben im Arbeitsspeicher und ändern das Spielstandformat nicht.
4. Alle neun PC-Prüfprogramme und echte Pico-2-Übersetzung erfolgreich, einschließlich kleiner Figuren, weißer Deckmasken, Spiegelung, Laufwegen, Verdeckung, Touch, Jahreszeiten und Bildschirm aus.

Die Darstellung ist eingebaut. Bedürfnisse, Sammlung und Version-3-Spielstand bleiben kompatibel. In den Minispielen gelten weiterhin deren eigene Spielkoordinaten und Figurengrößen.

## Dateien und Verhalten

- `software/schritt-10/evi-handy/raumtiefe.h/.ino`: gemeinsamer Bodenanker, kontinuierliche Skalierung, Spiegelung und Verdeckung. Verdeckte Figurteile nehmen keinen Kuschel-Touch an.
- `entwuerfe/zimmer/kompakt/`: der ausgewählte kompakte Zimmergrund und die passende Weihnachtsfassung. `make-zimmer-tiefe.py` erzeugt `zimmerlandschaft.h` und die dokumentierten Vordergrundstempel. Beim Verdecken bleiben die Originalpixel der Hintergrundzeichnung stehen; Möbel werden nicht neu gezeichnet.
- Das Fenster übernimmt einen Ausschnitt der tatsächlich gerenderten Gartenansicht, einschließlich Saison, Tag/Nacht und Niederschlag. Rahmen und Fensterkreuz bleiben davor. Vogel/Sternschnuppe werden auf die neue Scheibe abgebildet.
- Zimmergänge verwenden Bodenpunkte und führen für den Fensterbesuch seitlich um das feste Sideboard. Kuscheln lässt Evi an ihrer aktuellen Position und danach über den freien Weg zum Teppich zurückkehren.
- Gartensuche: erst seitlich auf der Wiese, dann in die Tiefe zum Baum/Busch/Beet. Die vordere Beetkante verdeckt Pfoten. Nach der Untersuchung kommt Evi vor, bevor sie das Ergebnis zeigt; A führt ganz zurück auf die freie Wiese.
- Weihnachten 18. November bis einschließlich 5. Januar: passender Baum auf dem Boden statt Tisch/Pflanze/Regal, Fenstersterne und Kerzentablett; ab 6. Januar wieder normales Zimmer.

## Prüfung

Eigene PC-Prüfung `raumtiefe.cpp`: alle Altersstufen nah/mittel/fern, kontinuierliche Skalierung, Bodenanker, Originalpixel bei Verdeckung, Touch auf sichtbare/verdeckte Teile, begehbarer Fensterweg und Rückweg, Garten-Tiefengang und Rückkehr, neuer Fensterausblick bei Regen/Schnee tags/nachts, RESET und unveränderte Minispielkoordinaten. Dazu die bisherigen Prüfprogramme und echte Pico-2-Übersetzung. Screenshots unter `entwuerfe/zimmer/kompakt/vorschauen/`. Am echten Board noch ausprobieren.
