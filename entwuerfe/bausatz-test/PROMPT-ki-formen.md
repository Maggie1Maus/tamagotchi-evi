# Prompt für die Bild-KI: weitere Körper- und Kopfformen (27. November 2026)

Ziel: **mehr Variation in der Silhouette.** Der Variationstest (`pruefbogen-aufstellung.png`) zeigt: Mit einer einzigen Körper- und Kopfform sehen alle Figuren gleich aus, nur Ohren, Schwanz, Fellton und Muster unterscheiden sich. Dieser Auftrag liefert **3 weitere Körper (sitzend)** und **3 weitere Köpfe**. Danach gibt es 4 × 4 Grundformen.

Gleiche Methode wie [PROMPT-ki-basis.md](PROMPT-ki-basis.md) und [PROMPT-ki-posen.md](PROMPT-ki-posen.md): ein Teil allein auf der Schablone, feste Grauwerte, Skript setzt zusammen. Der sitzende Körper 2 und der Kopf 2 sind die Vorbilder für Stil, Linie und Fellton.

## Anhängen (bei jedem Auftrag)
1. Die **Schablone** des Teils (Kreuze: rot = Hals, blau = Schwanz bzw. Ohren, grün = Gesicht).
2. **Stil:** bei Körpern `ki/teile/koerper-sitzend-2.png`, bei Köpfen `ki/teile/kopf-3-4-2.png`.

## Körper (sitzend, ohne Kopf, Schwanz, Ohren, Halskrause)
| Form | Datei | Raster | Ausgabe (8-fach) | Hals | Schwanz | Charakter |
|---|---|---|---|---|---|---|
| rundlich | `koerper-rundlich-<nr>.png` | 72 × 40 | 576 × 320 | (36, 3) | (60, 22) | klein, kugelig, kurze Beine, dicker runder Bauch (Hamster, Igel, Schildkröte) |
| schlank | `koerper-schlank-<nr>.png` | 76 × 56 | 608 × 448 | (36, 3) | (62, 34) | hoch, schmale Brust, lange schlanke Beine, schmaler Rücken (Reh, Hase, Windhund) |
| kräftig | `koerper-kraeftig-<nr>.png` | 84 × 50 | 672 × 400 | (42, 3) | (70, 28) | breite Schultern, dicke Vorderbeine, muskulös (Löwe, Bär, Bulldogge) |

## Köpfe (leer: ohne Ohren, Augen, Nase, Mund)
| Form | Datei | Raster | Ausgabe (8-fach) | Hals | Ohren links/rechts | Gesicht | Charakter |
|---|---|---|---|---|---|---|---|
| klein-rund | `kopf-klein-rund-<nr>.png` | 52 × 40 | 416 × 320 | (26, 36) | (11, 7) / (41, 7) | (26, 22) | klein, fast kreisrund (Hamster, Maus, Küken) |
| breit | `kopf-breit-<nr>.png` | 68 × 44 | 544 × 352 | (34, 40) | (15, 8) / (53, 8) | (34, 24) | sehr breit mit dicken Backen und Backenfell (Löwe, Waschbär, Katze) |
| spitz | `kopf-spitz-<nr>.png` | 56 × 46 | 448 × 368 | (28, 42) | (11, 8) / (45, 8) | (28, 26) | herzförmig, schmales spitzes Kinn (Fuchs, Eichhörnchen, Reh) |

## Grauwerte, Stil, Richtung (wie bisher)
34 Umriss (1 Pixel), 119 innen, 170 Schatten, 204 Fell, 238 Licht, 255 Hintergrund. Nur diese Werte, Dithering, ein paar Fellpunkte, keine Halbtöne, kein Anti-Aliasing. Blickrichtung leicht nach **links** (3/4). Artneutral.

## Prompt (zum Kopieren, Englisch)
```text
Create ONE standalone pixel-art game sprite part on a pure white background: [TEIL].
It must fit exactly on the attached template (8x8 screen pixels per sprite pixel; colored crosses mark attachment points: red "Hals" = open neck edge, blue = tail or ears, green "Gesicht" = where eyes and mouth will be placed later). The canvas size is [AUSGABEGROESSE] pixels. Do not draw the grid, the crosses or any text.

Draw it in EXACTLY the style, line weight, fur tones and level of detail of the second attached image (same character family, only a different build). It must be clearly different in silhouette from that image.
- Strict pixel art on the grid, every sprite pixel a flat 8x8 square, no anti-aliasing, no stray pixels.
- Dark outline exactly one sprite pixel wide, gray 34.
- ONLY these gray values: 34, 119, 170, 204, 238, 255 (background). Checkerboard dithering for shading, a few fur dots.
- Species-neutral small mammal, cute, three-quarter view slightly turned to the viewer's LEFT.
- Part alone: no other body parts, no ground shadow.
```

### Einzusetzen
| Teil | [TEIL] | [AUSGABEGROESSE] |
|---|---|---|
| Körper rundlich | the headless body of the character sitting upright, small and round: a plump spherical belly, short thick legs, tiny paws, very short chest. NO head, NO ears, NO tail, NO neck ruff. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 576 x 320 |
| Körper schlank | the headless body of the character sitting upright, tall and slender: narrow chest, long thin front legs, slim back, slender folded hind leg. NO head, NO ears, NO tail, NO neck ruff. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 608 x 448 |
| Körper kräftig | the headless body of the character sitting upright, sturdy and strong: broad shoulders, wide chest, thick front legs, big haunch. NO head, NO ears, NO tail, NO neck ruff. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 672 x 400 |
| Kopf klein-rund | the head of the character: small and almost perfectly round, short soft cheeks, plain blank face (NO eyes, NO nose, NO mouth, NO whiskers, NO ears). Closed chin/neck at the bottom, centered on the red cross. | 416 x 320 |
| Kopf breit | the head of the character: very wide with big fluffy cheek tufts on both sides, low and broad, plain blank face (NO eyes, NO nose, NO mouth, NO whiskers, NO ears). Closed chin/neck at the bottom, centered on the red cross. | 544 x 352 |
| Kopf spitz | the head of the character: heart-shaped with a narrow pointed chin and wider forehead, plain blank face (NO eyes, NO nose, NO mouth, NO whiskers, NO ears). Closed chin/neck at the bottom, centered on the red cross. | 448 x 368 |

Je Teil **3 Varianten** erzeugen, die beste behalten.

## Ablage
`ki/teile/koerper-rundlich-<nr>.png`, `koerper-schlank-<nr>.png`, `koerper-kraeftig-<nr>.png`, `kopf-klein-rund-<nr>.png`, `kopf-breit-<nr>.png`, `kopf-spitz-<nr>.png`.

## Was danach passiert
- Ich erweitere die Zusammensetzung um diese Formen. Platzierung: Boden bei y = 110, Körper-Hals mittig. Die Regeln stehen in `basis3_layout.py` (`KOERPER_FORMEN`, `KOPF_FORMEN`).
- Ein neuer Aufstellungsbogen zeigt, ob die 21 Figuren damit genug Variation haben.
- Ohren, Schwänze und Gesichter sitzen an den Ansatzpunkten des jeweiligen Kopfes. Wenn ein Kopf sehr klein oder breit ist, kann ein Ohr schlecht sitzen; dann ändere ich nur den Ansatzpunkt.

## Grenzen
- **Nur sitzend.** Laufen, hüpfen, geduckt und eingerollt gibt es erst für die Normalform. Jede neue Körperform braucht pro Pose einen weiteren Körper. Deshalb prüfen wir zuerst, ob die Sitz-Formen reichen, bevor wir sie für alle Posen bestellen.
- **Gesichter:** Das vorhandene Gesicht (Augen, Nase, Mund) passt auf alle Köpfe etwa gleich. Bei einem spitzen oder sehr kleinen Kopf kann es zu groß wirken.
