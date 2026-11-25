# Umsetzung der Grillme-Entscheidungen vom 25. November 2026

Diese Umsetzung betrifft ausschließlich die Waveshare-Firmware unter `software/schritt-10/`. Die Handy-App bleibt auf dem bisherigen Stand, wie am 24. November beschlossen.

## Neues Zimmer und Fensterausblick

Die bestätigte V2-Vorschau ist jetzt eingebaut: Frontalansicht ohne Seitenwände, Tierbett, Sideboard, leeres Regal und Teppich. Evi verwendet weiterhin die bestehenden Originalzeichnungen aller Altersstufen und Posen.

Der Fensterausblick besteht aus getrennten Ebenen für Himmel, Himmelszeichen, Wolken, Landschaft, saisonalen Baum, Laub und Niederschlag. Der Baum besitzt Stamm und Bodenbezug. Fensterrahmen und Fensterkreuz liegen davor. Alle 51 gültigen Kombinationen aus vier Jahreszeiten, drei Tageszeiten und Wetter werden aus dem geprüften Grafikpaket zusammengesetzt. Schnee fällt ausschließlich im Winter; Regen verdichtet zusätzlich das schwarze Punktraster. Es gibt keine Grauflächen oder Helligkeitsverläufe.

Die vorhandenen Kalender-, Wetter- und Sonnenzeitregeln bleiben erhalten. Weihnachten gilt weiterhin vom 18. November bis einschließlich 5. Januar: Baum anstelle des Regals, Sterne am Fenster und Kerzentablett auf einem aufgestellten Sideboard. Der rechte mittlere Stellbereich bleibt für diesen saisonalen Baum frei. Das Regal wird nach der Weihnachtszeit wieder sichtbar; seine gespeicherte Position geht nicht verloren.

Die Quellen liegen unter `entwuerfe/zimmer/grillme-2026-11-25/v2/`, einschließlich bestätigter PNG, Jahreszeiten- und Ebenenübersichten sowie vollständigem Grafikpaket. `make-zimmer-v2.py` prüft sämtliche Paket-Prüfsummen und erzeugt den Firmware-Header ohne erneute Bildgenerierung.

## Zimmer einrichten

Im normalen Menü gibt es **Zimmer einrichten**. Zunächst stehen die vier vorhandenen Startmöbel zur Verfügung. Es werden keine neuen Anschaffungen oder Preise erfunden.

| Bedienung | Auswahl | Verschieben |
| --- | --- | --- |
| Hoch/runter | Möbel wählen | nach hinten/vorne verschieben |
| Links/rechts | links: voriges Möbel; rechts: ins Inventar beziehungsweise erneut aufstellen | seitlich verschieben |
| A | Platzierung als Vorschau öffnen | gültige Position bestätigen und speichern |
| B oder START | Einrichten schließen | Vorschau abbrechen |
| RESET | Vorschau verwerfen, Bildschirm ausschalten | Vorschau verwerfen, Bildschirm ausschalten |

Touch unterstützt die Auswahl in der Liste und das Setzen einer Vorschauposition im Zimmer. Die untere Zeile bietet Inventar, Bestätigen und Zurück. Bei echten Tasten bleibt normales Antippen außerhalb dieses Modus weiterhin Kuscheln.

Eine Vorschau verändert noch keine gespeicherte Position. Abbrechen, Ausschalten oder ein ungültiger Platz lassen den bisherigen Stand stehen. Nicht aufgestellte Möbel bleiben im Inventar und können wieder aufgestellt werden. Die Sammlung bleibt auch bei einem eingelagerten Sideboard erreichbar; Evi läuft dann nicht zu einem unsichtbaren Möbelstück.

## Tiefenzonen, Wandfläche und Wege

- Hinten: Bodenanker bis y = 155. Hinten aufgestellte hohe Möbel belegen ihre Wandfläche und dürfen weder das feste Fenster noch Wandmöbel überlagern.
- Mitte: Bodenanker y = 156–173. Möbel können Wandmöbel optisch verdecken, ohne deren Wandplatz zu belegen.
- Vorne: Bodenanker ab y = 174. Diese Fläche bleibt für Teppich, Evi und Bewegung frei; Bett und Sideboard werden dort abgewiesen.

Die vorhandene kontinuierliche Tiefenskalierung bleibt erhalten. Figuren werden vom Fußpunkt aus gezeichnet. Verdeckung und Touch prüfen jetzt die tatsächlichen Positionen und Masken der verschiebbaren Möbel.

Vor jeder Bestätigung müssen Fenster, Bett und Sideboard erreichbar bleiben. Ein kleines Navigationsnetz berechnet Wege um die aktuellen Bodenflächen. Es prüft auch den saisonalen Baum. Die Gänge zum Fenster und zur Sammlung verwenden dieselbe Prüfung; die Weglänge ist deshalb nicht mehr fest. Der Suchpuffer liegt wiederverwendbar im RAM, nicht als großer Puffer auf dem Mikrocontroller-Stack.

## Stellplätze: vorbereitet und geprüft

Der Möbelkatalog enthält Breiten- und Höhenbelegung sowie Ablagekapazität. Sideboard und Regal haben zunächst zwei Ablageplätze. Ein breites Kerzentablett benötigt beide Plätze; zwei kleine Gegenstände können jeweils einen Platz belegen. Die Weihnachtsdarstellung verwendet diese Kapazitätsprüfung.

Die Bedienung weiterer kleiner Gegenstände, alternative Pflanzenstandorte, neue Möbel, Möbelwirkungen und Käufe sind noch nicht eingebaut. Das endgültige sichtbare Möbelraster und seine Maße sind laut Grillme-Datei offen. Die jetzige Platzierung bewegt sich in konfigurierbaren Vier-Pixel-Schritten; dies ist kein neuer endgültiger Rasterbeschluss.

## Spielstand erhalten

Einrichtung und Inventar werden als eigener versionierter Bereich mit Prüfsumme ab EEPROM-Adresse 3328 gespeichert. Statische Größenprüfungen halten Abstand zu den vorhandenen Spielständen und zum Freundschafts-/Geburtstagsbereich ab 3584. Das bestehende Version-3-Spielstandformat und die Übernahme der Versionen 1 und 2 ändern sich nicht. Ein beschädigter Einrichtungsstand setzt nur die Möbel auf die Startanordnung zurück. Bedürfnisse, Name, Münzen, Funde, Freundschaft und Geburtstag bleiben erhalten.

## Weitere beschlossene Punkte

Der Zielstand aus der Grillme-Datei bleibt verbindlich: öffentlicher Park zu Beginn, eigener Garten später kaufbar und erweiterbar; selbstständige Freundschaften und gegenseitige romantische Gefühle; Partnerauswahl und Familiengründung mit Zustimmung der Nutzerin; weitere Zimmer statt Vergrößerung des ersten Zimmers.

Diese Systeme werden mit diesem Schritt nicht vorweggenommen. Insbesondere gibt es keine erfundenen Kaufpreise, kein unbestätigtes Wirtschaftssystem und keine neue Familienmechanik. Der bisherige Garten-Prototyp bleibt erreichbar, bis Park, privater Garten und Kaufablauf gemeinsam ausgearbeitet werden. Die bestehende Freundschaft zwischen Evi und der Nutzerin bleibt unverändert und ist ein anderes System als die geplanten Beziehungen zwischen Figuren.

## Prüfung

Die PC-Nachbildung prüft weiterhin Darstellung, Abläufe, Tasten, Bewegung, Umwelt, Sammlung, Kuscheln, Garten, Raumtiefe und Schlafmodus. Neu ist `einrichtung.cpp`: Platzierung und Wandregeln, geschützte Vorderzone, freie Wege, Inventar, Vorschau/Abbrechen, RESET, Speichern, beschädigte Daten und getrennte EEPROM-Bereiche. Ein unabhängiger Vergleich prüft alle 51 von der echten Firmware gerenderten Fensterausblicke pixelgenau gegen die PNGs aus dem bestätigten Grafikpaket.

Alle elf PC-Prüfprogramme sowie der unabhängige Vergleich der 51 Fensterausblicke sind ohne Fehler durchgelaufen. Die Übersetzung mit `-Wall -Wextra` meldet keine Warnungen. Nach der abschließenden Speicherkorrektur wurden die neuen Einrichtungs-/Speicherprüfungen und der Pixelvergleich erneut erfolgreich ausgeführt.

Eine echte Pico-2-Übersetzung und ein Test am physischen Board stehen aus; das Arduino-Werkzeug ist in dieser Arbeitsumgebung nicht vorhanden. Die Bildschirmfotos der PC-Nachbildung verwenden deren Ersatzschrift.
