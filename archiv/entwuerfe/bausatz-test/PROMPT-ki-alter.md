# Prompt für die Bild-KI: Altersstufen (27. November 2026)

Ziel: **Baby, Teenager und Senior** als eigene Körper und Köpfe, damit das Spiel Generationen durchspielen kann. Der Erwachsene ist die bestehende Basis (`koerper-sitzend-2`, `kopf-3-4-2`). Ohren, Schwänze, Gesichter, Muster und Aufsätze werden wiederverwendet. Gleiche Methode wie [PROMPT-ki-basis.md](PROMPT-ki-basis.md) und [PROMPT-ki-formen.md](PROMPT-ki-formen.md): ein Teil allein auf der Schablone, feste Grauwerte, Skript setzt zusammen.

**Nur sitzend, nur diese eine Körperform und ein Kopf je Stufe.** Danach prüfen wir, ob die vorhandenen Teile passen, und bestellen erst dann weitere Formen.

## Anhängen (bei jedem Auftrag)
1. Die **Schablone** des Teils (Kreuze: rot = Hals, blau = Schwanz bzw. Ohren, grün = Gesicht).
2. **Stil und Fell:** `ki/teile/koerper-sitzend-2.png` (bei Körpern) bzw. `ki/teile/kopf-3-4-2.png` (bei Köpfen). Dieselbe Figur, nur eine andere Lebensphase: gleiches Fell, gleiche Linie, gleiche Pfotenart.

## Körper (sitzend, ohne Kopf, Schwanz, Ohren)
| Stufe | Datei | Raster | Ausgabe (8-fach) | Hals | Schwanz | Charakter |
|---|---|---|---|---|---|---|
| Baby | `koerper-baby-<nr>.png` | 56 × 34 | 448 × 272 | (28, 3) | (46, 18) | winzig und kugelig: dicker runder Bauch, ganz kurze Beine, kleine Patschpfoten, fast kein Hals |
| Teenager | `koerper-teenager-<nr>.png` | 72 × 44 | 576 × 352 | (36, 3) | (60, 24) | schlaksig: etwas dünner als der Erwachsene, längere Beine, noch nicht ganz ausgewachsen, Pfoten wirken groß |
| Senior | `koerper-senior-<nr>.png` | 76 × 46 | 608 × 368 | (34, 8) | (62, 28) | gebeugt: rundlicher Rücken, Hals nach vorn gesenkt (Halsöffnung tiefer als bei den anderen), etwas hängender Bauch, dünne ruhige Beine |

## Köpfe (leer: ohne Ohren, Augen, Nase, Mund)
| Stufe | Datei | Raster | Ausgabe (8-fach) | Hals | Ohren l/r | Gesicht | Charakter |
|---|---|---|---|---|---|---|---|
| Baby | `kopf-baby-<nr>.png` | 56 × 44 | 448 × 352 | (28, 40) | (11, 9) / (45, 9) | (28, 25) | sehr groß und fast kreisrund, pausbäckig, hohe runde Stirn, kurzes glattes Babyfell ohne Büschel |
| Teenager | `kopf-teenager-<nr>.png` | 58 × 42 | 464 × 336 | (29, 38) | (12, 8) / (46, 8) | (29, 23) | etwas länglicher als der Babykopf, schlanke Wangen, ein kleines freches Stirnbüschel |
| Senior | `kopf-senior-<nr>.png` | 58 × 44 | 464 × 352 | (29, 40) | (12, 9) / (46, 9) | (29, 25) | breit und leicht hängend, schwere Hängebacken, etwas zerzaust, ruhig |

## Grauwerte, Stil, Richtung (wie bisher)
34 Umriss (1 Pixel), 119 innen, 170 Schatten, 204 Fell, 238 Licht, 255 Hintergrund. Nur diese Werte, Dithering, ein paar Fellpunkte, keine Halbtöne, kein Anti-Aliasing. Blickrichtung leicht nach **links** (3/4). Artneutral (Katze, Hund, Hase oder Eichhörnchen sollen darauf passen).

## Prompt (zum Kopieren, Englisch)
```text
Create ONE standalone pixel-art game sprite part on a pure white background: [TEIL].
It must fit exactly on the attached template (8x8 screen pixels per sprite pixel; colored crosses mark attachment points: red "Hals" = open neck edge, blue = tail or ears, green "Gesicht" = where eyes and mouth will be placed later). The canvas size is [AUSGABEGROESSE] pixels. Do not draw the grid, the crosses or any text.

This is the SAME character as in the second attached image, only at another life stage: same fur, same line weight, same shading, same paw style. Only the proportions and posture change as described.
- Strict pixel art on the grid, every sprite pixel a flat 8x8 square, no anti-aliasing, no stray pixels.
- Dark outline exactly one sprite pixel wide, gray 34.
- ONLY these gray values: 34, 119, 170, 204, 238, 255 (background). Checkerboard dithering for shading, a few fur dots.
- Species-neutral small mammal, three-quarter view slightly turned to the viewer's LEFT.
- Part alone: no other body parts, no ground shadow.
```

### Einzusetzen
| Teil | [TEIL] | [AUSGABEGROESSE] |
|---|---|---|
| Körper Baby | the headless body of a BABY version of the character sitting upright: tiny and very round, a plump round belly, extremely short stubby legs, small chubby paws, almost no neck. NO head, NO ears, NO tail. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 448 x 272 |
| Körper Teenager | the headless body of a TEENAGER version of the character sitting upright: a bit gangly and leggy, slimmer than an adult, longer thin legs, slightly oversized paws, still growing. NO head, NO ears, NO tail. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 576 x 352 |
| Körper Senior | the headless body of an ELDERLY version of the character sitting upright: hunched rounded back, neck sloping forward and lower (the open neck edge at the red cross is lower than the shoulders), slightly sagging belly, thin calm legs, paws resting. NO head, NO ears, NO tail. Plain fur at the blue cross, paws on the last grid row. | 608 x 368 |
| Kopf Baby | the head of a BABY version of the character: very large and almost perfectly round, chubby cheeks, high round forehead, short smooth baby fur with no tufts, plain blank face (NO eyes, NO nose, NO mouth, NO whiskers, NO ears). Closed chin/neck at the bottom, centered on the red cross. | 448 x 352 |
| Kopf Teenager | the head of a TEENAGER version of the character: a bit longer and leaner than a baby head, slimmer cheeks, one small cheeky tuft of fur on the forehead, plain blank face (NO eyes, NO nose, NO mouth, NO whiskers, NO ears). Closed chin/neck at the bottom, centered on the red cross. | 464 x 336 |
| Kopf Senior | the head of an ELDERLY version of the character: broad with heavy drooping jowls, slightly tousled fur, a calm tired shape, plain blank face (NO eyes, NO nose, NO mouth, NO whiskers, NO ears). Closed chin/neck at the bottom, centered on the red cross. | 464 x 352 |

Je Teil **3 Varianten** erzeugen, die beste behalten.

## Ablage
`ki/teile/koerper-baby-<nr>.png`, `koerper-teenager-<nr>.png`, `koerper-senior-<nr>.png`, `kopf-baby-<nr>.png`, `kopf-teenager-<nr>.png`, `kopf-senior-<nr>.png`.

## Was danach passiert
- Die Zusammensetzung kennt die neuen Formen schon (`basis3_layout.py`, `KOERPER_FORMEN` und `KOPF_FORMEN`). Ich baue einen Prüfbogen mit allen vier Stufen mit demselben Ohr, Schwanz, Gesicht und Muster, damit man sieht, ob es **dasselbe Tier in vier Lebensphasen** ist.
- Passt ein Teil nicht (Ohren zu groß fürs Baby, Gesicht zu klein für den Kopf), stelle ich zuerst die Zahlen ein und sage dir dann, wofür wir neue Teile brauchen.

## Grenzen
- **Ohren, Schwänze, Gesichter und Aufsätze haben nur eine Größe.** Beim Baby wirken sie deshalb im Verhältnis groß. Das kann süß aussehen, wenn nicht, bestellen wir kleine Babyohren und Babyschwänze.
- **Nur sitzend.** Laufen, hüpfen und schlafen gibt es für die Altersstufen noch nicht.
- **Nur eine Körper- und Kopfform je Stufe.** Schlank, rundlich und kräftig als Baby oder Senior wären weitere Aufträge.
