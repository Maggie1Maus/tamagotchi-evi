# Prompt für die Bild-KI: Artmerkmale (27. November 2026)

Ziel: Aus den katzenartigen Körpern echte Arten machen. Erste Runde: **Mähne** (Leo), **Stacheln** (Rolli), **Panzer** (Tetro), **Flügel** (Lumi, Blubber), **Antennen** (Bit, Blip), **Rüssel** (Ella). Gleiche Methode wie [PROMPT-ki-teile.md](PROMPT-ki-teile.md): ein Teil allein auf der Schablone, feste Grauwerte, Skript setzt zusammen.

Nicht in dieser Runde: Schnecke (Mimi) und Geist (Spuki) sind andere Tiere, nicht nur ein Zusatzteil; Backentaschen (Mampf), Augenmaske und Ringe (Groschen), Schleife, Glöckchen, Halsband, Backenblitze (kleine Aufsätze, später).

## Wie die Teile sitzen
| Merkmal | Lage | Ansatz im Teil | Wohin auf der Figur |
|---|---|---|---|
| Mähne | **hinter dem Kopf**, rahmt ihn ein | Kopfmitte (44, 38) | Mitte des Kopfes |
| Stacheln | **hinter dem Körper**, auf dem Rücken | Rücken (6, 26) | Rückenlinie, vom Nacken bis zum Hinterteil |
| Panzer | **vor dem Körper**, deckt Rücken und Hinterteil | Rücken (6, 40) | Rückenlinie |
| Flügel | **hinter dem Körper**, am Rücken | Schulter (6, 50) | Schulterblatt |
| Antennen | wie **Ohren** (zwei, symmetrisch) | Ohr-Ansatz | Ohr-Ansätze des Kopfes |
| Rüssel | **vor dem Gesicht** (deckt Nase und Mund) | Nase oben (12, 2) | Nase |

Der Körper schaut nach links, der Rücken liegt rechts und fällt von der Schulter zum Hinterteil nach rechts unten ab. Die blasse blaue Hilfslinie in den Schablonen zeigt das. **Nicht mitzeichnen.**

## Anhängen
1. Die Schablone (Kreuz = Ansatz, blasse blaue Form = Hilfe, wird nicht gezeichnet):
   `ki/schablone-merkmal-maehne.png` (88 × 68), `…-stacheln.png` (48 × 32), `…-panzer.png` (56 × 44), `…-fluegel.png` (56 × 56), `…-ruessel.png` (24 × 40); für Antennen `ki/schablone-ohr.png` (24 × 32).
2. **Stil:** `ki/teile/koerper-sitzend-2.png` (Körper, Mähne, Stacheln, Panzer, Flügel) bzw. `ki/teile/kopf-3-4-2.png` (Mähne, Rüssel, Antennen).

## Maße
| Teil | Datei | Raster | Ausgabe (8-fach) |
|---|---|---|---|
| Mähne | `merkmal-maehne-<nr>.png` | 88 × 68 | 704 × 544 |
| Stacheln | `merkmal-stacheln-<nr>.png` | 48 × 32 | 384 × 256 |
| Panzer | `merkmal-panzer-<nr>.png` | 56 × 44 | 448 × 352 |
| Flügel Fee | `merkmal-fluegel-fee-<nr>.png` | 56 × 56 | 448 × 448 |
| Flügel Drache | `merkmal-fluegel-drache-<nr>.png` | 56 × 56 | 448 × 448 |
| Antenne | `ohr-antenne-<nr>.png` | 24 × 32 | 192 × 256 |
| Rüssel | `merkmal-ruessel-<nr>.png` | 24 × 40 | 192 × 320 |

## Grauwerte (Rollen, wie bisher; zusätzlich Hinweise je Teil)
34 Umriss (1 Pixel), 119 innen/dunkle Fugen, 170 Schatten, 204 Fell, 238 Licht, 255 Hintergrund. Nur diese Werte, Dithering, keine Halbtöne.
- **Mähne:** Fell 204 mit 170-Schatten, Licht 238, viele kleine Büschel am Rand.
- **Stacheln:** Stachel 170, Schatten 119, Spitzen 204.
- **Panzer:** Panzerfläche 170, Fugen und Platten 119/34, Lichter 204.
- **Flügel:** Fee: Fläche 238, Adern 170. Drache: Fläche 170, Knochen 119, Spitzen 204.

## Prompt (zum Kopieren, Englisch)
```text
Create ONE standalone pixel-art game sprite part on a pure white background: [TEIL].
It must fit exactly on the attached template (8x8 screen pixels per sprite pixel; the red cross marks the attachment point; the pale blue guide shape is only a hint for position and size and must NOT be drawn). The canvas size is [AUSGABEGROESSE] pixels. Do not draw the grid, the cross, the guide or any text.

Draw it in EXACTLY the style, line weight and fur/shading look of the second attached image. The character faces to the viewer's LEFT (three-quarter view); its back is on the RIGHT.
- Strict pixel art on the grid, every sprite pixel a flat 8x8 square, no anti-aliasing, no stray pixels, no half-transparency.
- Dark outline exactly one sprite pixel wide, gray 34.
- ONLY these gray values: 34, 119, 170, 204, 238, 255 (background). Checkerboard dithering for shading.
- Cute, compact, game-like. Part alone: no other body parts, no ground shadow.
```

### Einzusetzen
| Teil | [TEIL] | [AUSGABEGROESSE] |
|---|---|---|
| Mähne | a big fluffy lion-style mane seen from the front, drawn as a ring of tufted fur that frames a head: the middle (where the pale blue ellipse is) stays EMPTY because a head will be placed over it; the mane is wider and taller than the head, fur tufts point outward, slightly longer at the top and sides, shorter at the bottom | 704 x 544 |
| Stacheln | a row of short pointed hedgehog spines standing up along a back line, seen from the side; the spines rise from the line shown in the guide and lean slightly to the right; about 9 to 11 spines, each 5 to 8 sprite pixels tall | 384 x 256 |
| Panzer | a turtle-style shell, a rounded dome with a hexagon plate pattern, seen from the side, meant to cover the back and rump of a small sitting animal; the flat lower edge lies on the guide line; the rim has a thick band | 448 x 352 |
| Flügel Fee | a folded fairy wing seen from the side, slender and rounded with fine veins, pointing up and back (to the right) from the red cross | 448 x 448 |
| Flügel Drache | a folded small dragon/bat wing seen from the side with bone spars and a scalloped membrane, pointing up and back (to the right) from the red cross | 448 x 448 |
| Antenne | the LEFT one of a pair of thin robot/alien antennae: a straight thin stalk with a round bulb on top, leaning slightly outward (to the left); stalk 2 sprite pixels wide | 192 x 256 |
| Rüssel | a short elephant trunk hanging down from the nose, seen from the front, slightly curled at the tip, with fine wrinkle lines; the top end attaches at the red cross | 192 x 320 |

Je Teil **3 Varianten** erzeugen, die beste behalten.

## Ablage
In `ki/teile/` mit den Dateinamen aus der Tabelle (zum Beispiel `merkmal-maehne-1.png`, `ohr-antenne-2.png`). Antennen nur die **linke**, ich spiegele sie.

## Was danach passiert
Ich ergänze `formen.py` um diese Merkmale (Ebenenreihenfolge, Rücken-Ansatz je Körperform, Mähne hinter dem Kopf, Panzer vor dem Körper) und baue einen Prüfbogen mit Leo, Rolli, Tetro, Lumi, Blubber, Bit, Blip und Ella.

## Grenzen
- Die Rückenlinie ist bei jedem Körper etwas anders (schlank fällt steiler ab). Stacheln, Panzer und Flügel werden deshalb am Ansatz ausgerichtet und können an einer Körperform leicht abstehen. Das korrigiere ich über den Ansatzpunkt.
- Die Merkmale sind für die **sitzende** Haltung gezeichnet. Für laufen, hüpfen und schlafen bräuchten sie eigene Ansätze oder neue Teile. Die Mähne und die Antennen sind davon am wenigsten betroffen, Panzer und Stacheln am meisten.
- Der Rüssel überdeckt das Gesicht (Nase und Mund). Die Gesichter der Rüssel-Figuren bräuchten eigene Teile ohne Mund, falls der Rüssel gut aussieht.
