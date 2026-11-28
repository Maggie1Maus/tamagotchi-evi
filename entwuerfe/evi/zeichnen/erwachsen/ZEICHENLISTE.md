# Zeichenliste: Was fehlt für die neue erwachsene Evi im Spiel (Stand 28.11.2026)

Das Spiel zeigt für die erwachsene Evi **19 Bilder** (`Sprites/04-Erwachsen/`). Die neue Evi soll sie alle ersetzen.
Alles wird auf Evis 2048er-Vorlage gezeichnet (`vorlagen/`), Dateinamen nach den Regeln in `../ANLEITUNG.md`.

## Schon vorhanden (frontal, sitzend)
Kopf, Körper, Krause, Buschelohr, Buschelschwanz, Gesichter: neutral, glücklich, beleidigt, hungrig, krank, müde, schlafend, schmutzig, staunen, traurig.

## Runde 1: die sitzenden Bilder (schnell spielbar)
Damit entstehen ruhe, glücklich, traurig, hungrig, krank, müde, schmutzig, staunen, singen, gähnen, denken, beleidigt, garten-zeigen, garten-untersuchen (14 von 19).

| Was | Ordner / Dateiname | Wofür |
|---|---|---|
| 3 Gesichter | `gesichter/frontal/gesicht-singen.png`, `-gaehnen.png`, `-denken.png` | singen, gähnen, denken |
| Hängende Ohren | `ohren/frontal/ohr-buschel-haengend.png` (beide Seiten in einer Datei, dann `__vorne` nur wenn sie vor dem Kopf liegen sollen) | traurig, müde, krank, hungrig, beleidigt, schmutzig, gähnen, singen |
| Pfote am Bauch | `pfoten/frontal/pfote-bauch.png` | hungrig |
| Pfote am Kinn | `pfoten/frontal/pfote-kinn.png` | denken |
| Pfote am Mund | `pfoten/frontal/pfote-mund.png` | gähnen |
| Arme verschränkt | `pfoten/frontal/arme-verschraenkt.png` | beleidigt |
| Pfoten hoch | `pfoten/frontal/pfoten-hoch.png` | garten-zeigen |
| Pfote bei staunen | `pfoten/frontal/pfote-staunen.png` | staunen |
| Thermometer | `halsschmuck/` ist falsch dafür, besser neuer Ordner `zubehoer/frontal/thermometer.png` | krank |
| Schmutz + Stinklinien | `muster/frontal/muster-schmutz-koerper.png`, `zubehoer/frontal/stinklinien.png` | schmutzig |

Der neue Ordner `pfoten/` (und `zubehoer/`) bekommt wie alle die Ansichtsordner. Ein Teil, das nur ein Stück Evi zeigt (eine Pfote), liegt als eigene Ebene über dem Körper.

## Runde 2: andere Haltungen (je ein eigener Körper, teils Kopf in anderer Ansicht)
| Bild | Was gezeichnet werden muss |
|---|---|
| schlafend | Körper eingerollt (`koerper-schlafend`), dazu Schwanz um den Körper |
| hüpfen | Körper hüpfend (`koerper-huepfen`), Arm hoch |
| laufen | Körper laufend von der Seite (`koerper/seite/koerper-laufen`), Kopf `koepfe/seite/`, Gesicht `gesichter/seite/`, Ohren `ohren/seite/` |
| drehen | Körper schräg beim Drehen (Dreiviertel), Kopf und Gesicht dazu |
| fenster | Rückansicht sitzend (`koerper/hinten/koerper-sitzend`), `koepfe/hinten/`, `ohren/hinten/`, Schwanz hinten |
| garten-schnuppern | Körper nach vorn gebeugt, Kopf unten (`schnueffeln`), Gesicht dazu |
| garten-untersuchen | Körper geduckt, Kopf `schnueffeln` oder `frontal` |

## Nicht neu zu zeichnen
Farben, Graustufen, Schatten, Pixelfassung und die Kombination macht das Programm.

## Stand 28.11. (nach den Pfoten-Uploads)
Fertig: Gesichter singen/gähnen/denken, hängende Ohren, gesenkte Schwänze, `pfote-finger`, `pfote-gesicht`, `pfoten-hoch`, `arme-verschraenkt`, Körper `koerper-sitzend-ohne-vorderpfoten` und `koerper-sitzend-ohne-pfote-links`.
Noch offen: Pfote Bauch (hungrig), Pfote Staunen, Thermometer, Schmutzflecken + Stinklinien, Runde 2 (schlafend, hüpfen, laufen, drehen, Fenster, Garten schnuppern/untersuchen).

## Grundsatz: Pfotenhaltungen sind allgemein, nicht an ein Gefühl gebunden (Wunsch der Nutzerin, 28.11.)
Eine Pfotenhaltung heißt nach dem, was sie zeigt (`pfote-finger`, `pfote-gesicht`, `pfoten-hoch`, `arme-verschraenkt`), nicht nach der Stimmung. Die Stimmung entsteht aus **Gesicht + Ohren-/Schwanzhaltung + Pfotenhaltung**; das Gesicht macht eindeutig, was gemeint ist. So kann man eine Pfotenhaltung für mehrere Bilder nutzen und muss weniger zeichnen.
"Links" im Dateinamen `koerper-sitzend-ohne-pfote-links` heißt: von uns aus gesehen links (technisch Evis rechte Pfote). `pfote-finger` ist die Pfote für denken, geht aber auch für hungrig.

Vorläufige Zuordnung (zum Ändern, sobald die Nutzerin etwas anderes will):
| Bild | Körper | Pfoten | Gesicht |
|---|---|---|---|
| denken | ohne-pfote-links | pfote-finger | gesicht-denken |
| hungrig | ohne-pfote-links | pfote-finger | gesicht-hungrig |
| gähnen | ohne-pfote-links | pfote-gesicht | gesicht-gaehnen |
| beleidigt | ohne-vorderpfoten | arme-verschraenkt | gesicht-beleidigt |
| garten-zeigen | ohne-vorderpfoten | pfoten-hoch | gesicht-gluecklich |
