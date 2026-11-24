# Fundstücke: Gemütlich & schlicht

Vier neue, mit Bildgenerierung erstellte Fundstücke für Evis Sideboard: glatter Stein, kleines Blatt, weiche Feder, Tannenzapfen. Die PNGs sind die vorbereiteten Spielgrafiken: höchstens 60 × 60 Pixel, schwarze Linien, weißes Inneres, außen transparent. Evis Zeichnungen wurden dafür nicht verändert.

Erzeugen des Board-Headers: `python software/schritt-10/tools/make-sammlung.py`. `--prepare` bereitet neu hinzugefügte größere Generierungen einmalig als kleine, streng schwarz-weiße Spielgrafiken auf.

Die Bildaufträge hatten jeweils diesen gemeinsamen Rahmen (eingebaute Bildgenerierung, transparente Ausgabe):

> One collectible item sprite for the cozy monochrome virtual pet Evi. A single centered isolated object, generous clear margin. Charming simple and cozy black-and-white pixel art for a tiny 320x240 LCD game, deliberate square stepped pixels, clear silhouette legible at 48x48 pixels. Pure black outline and pure white opaque interior; only outside transparent. Flat 2D front view, sparse black pixel details. No gray, colors, gradients, shadow, text, labels, border, other objects, character or background.

Motive:
- Stein: a single smooth rounded pebble, with two tiny natural markings.
- Blatt: a single small gently curved deciduous leaf with a short stem and a few visible veins.
- Feder: a single small soft bird feather, diagonal with a simple central shaft and a few fine notches.
- Zapfen: a single small pine cone, upright oval silhouette with simple overlapping scales.

## Erweiterung vom 24. November 2026

Sechs neue Motive sind als PNG und im Board-Header vorhanden:

| ID | Datei | Name im Spiel |
|---|---|---|
| 4 | micky.png | Micky-Knopf |
| 5 | muschel.png | Arielles Muschel |
| 6 | drache.png | Kleiner Gartendrache |
| 7 | triforce.png | Triforce-Anhänger |
| 8 | fossil.png | Spiralfossil (Animal Crossing) |
| 9 | junimo.png | Junimo-Figur (Stardew Valley) |

Die vier bisherigen Bilddateien und Fund-IDs 0–3 bleiben unverändert. Die neuen Motive sind ebenfalls höchstens 60 × 60 Pixel groß. Innen deckend weiß, außen transparent; keine Graustufen. Vorbereitete PNGs und erzeugter Header sind die im Spiel verwendeten Bilder. Die vollständigen Bildaufträge stehen in `bildauftraege.json`; verwendet wurde die eingebaute Bildgenerierung.

Die Erzeugung des gewünschten Spyro- und Stitch-Bilds wurde vom Bildgenerator ohne konkreten Grund abgelehnt. Auf Wunsch der Nutzerin wurde anschließend eine eigene kleine Drachenfigur gestaltet, als eigenständiger Gartendrache mit runden Hörnern, Blattflügeln und Kleeblatt-Schwanzspitze. Das ist keine Spyro-Zeichnung. Stitch bleibt offen und wird nicht durch ein anderes Motiv ersetzt. Aktuell sind daher **zehn Arten** im Spiel verfügbar; mit Stitch wären es die gewünschten elf.
