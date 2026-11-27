# Prompt für die Bild-KI: Altersstufen für alle Körper- und Kopfformen (27. November 2026)

Entscheidung der Nutzerin: Jede Körper- und Kopfform braucht Baby, Teenager und Senior. Das sind **9 Körper** (rundlich, schlank, kräftig × 3 Stufen) und **6 Köpfe** (klein-rund, breit × 3 Stufen), jeweils sitzend. Die Standardform (sitzend / Kopf 3-4) ist mit [PROMPT-ki-alter.md](PROMPT-ki-alter.md) schon fertig.

Gleiche Methode wie dort: ein Teil allein auf der Schablone, feste Grauwerte, Skript setzt zusammen. Die **Posen** der Altersstufen kommen in einem zweiten Auftrag, nachdem diese Formen stehen.

## Anhängen (bei jedem Auftrag)
1. Die **Schablone** des Teils (Kreuze: rot = Hals, blau = Schwanz bzw. Ohren, grün = Gesicht): `ki/schablone-koerper-<form>-<stufe>.png` bzw. `ki/schablone-kopf-<form>-<stufe>.png`.
2. **Stil und Proportion:** die **erwachsene Form derselben Art**: `ki/teile/koerper-rundlich-<nr>.png` (bzw. schlank, kraeftig) oder `ki/teile/kopf-klein-rund-<nr>.png` (bzw. breit). Zusätzlich die **Standardstufe**, zum Beispiel `ki/teile/koerper-baby-3.png` bzw. `kopf-baby-2.png`, damit die Lebensphase überall gleich aussieht. Also: gleiches Tier, gleiche Form, nur jünger oder älter.

## Körper (sitzend, ohne Kopf, Schwanz, Ohren)
| Form und Stufe | Datei | Raster | Ausgabe (8-fach) | Hals | Schwanz |
|---|---|---|---|---|---|
| rundlich baby | `koerper-rundlich-baby-<nr>.png` | 50 × 28 | 400 × 224 | (25, 3) | (42, 16) |
| rundlich teenager | `koerper-rundlich-teenager-<nr>.png` | 64 × 36 | 512 × 288 | (32, 3) | (54, 20) |
| rundlich senior | `koerper-rundlich-senior-<nr>.png` | 68 × 38 | 544 × 304 | (30, 7) | (58, 24) |
| schlank baby | `koerper-schlank-baby-<nr>.png` | 54 × 40 | 432 × 320 | (26, 3) | (44, 26) |
| schlank teenager | `koerper-schlank-teenager-<nr>.png` | 68 × 50 | 544 × 400 | (33, 3) | (56, 31) |
| schlank senior | `koerper-schlank-senior-<nr>.png` | 72 × 52 | 576 × 416 | (32, 8) | (58, 32) |
| kraeftig baby | `koerper-kraeftig-baby-<nr>.png` | 58 × 36 | 464 × 288 | (29, 3) | (49, 20) |
| kraeftig teenager | `koerper-kraeftig-teenager-<nr>.png` | 76 × 46 | 608 × 368 | (38, 3) | (64, 25) |
| kraeftig senior | `koerper-kraeftig-senior-<nr>.png` | 80 × 48 | 640 × 384 | (36, 8) | (66, 27) |

## Köpfe (leer: ohne Ohren, Augen, Nase, Mund)
| Form und Stufe | Datei | Raster | Ausgabe (8-fach) | Hals | Ohren l/r | Gesicht |
|---|---|---|---|---|---|---|
| klein-rund baby | `kopf-klein-rund-baby-<nr>.png` | 48 × 40 | 384 × 320 | (24, 36) | (9, 8) / (39, 8) | (24, 23) |
| klein-rund teenager | `kopf-klein-rund-teenager-<nr>.png` | 50 × 38 | 400 × 304 | (25, 34) | (10, 7) / (40, 7) | (25, 21) |
| klein-rund senior | `kopf-klein-rund-senior-<nr>.png` | 52 × 40 | 416 × 320 | (26, 36) | (11, 8) / (41, 8) | (26, 23) |
| breit baby | `kopf-breit-baby-<nr>.png` | 60 × 44 | 480 × 352 | (30, 40) | (12, 9) / (48, 9) | (30, 25) |
| breit teenager | `kopf-breit-teenager-<nr>.png` | 64 × 42 | 512 × 336 | (32, 38) | (13, 8) / (51, 8) | (32, 23) |
| breit senior | `kopf-breit-senior-<nr>.png` | 68 × 44 | 544 × 352 | (34, 40) | (14, 9) / (54, 9) | (34, 25) |

## Grauwerte, Stil, Richtung (wie bisher)
34 Umriss (1 Pixel), 119 innen, 170 Schatten, 204 Fell, 238 Licht, 255 Hintergrund. Nur diese Werte, Dithering, ein paar Fellpunkte, keine Halbtöne, kein Anti-Aliasing. Blickrichtung leicht nach **links** (3/4). Artneutral.

## Prompt (zum Kopieren, Englisch)
```text
Create ONE standalone pixel-art game sprite part on a pure white background: [TEIL].
It must fit exactly on the attached template (8x8 screen pixels per sprite pixel; colored crosses mark attachment points: red "Hals" = open neck edge, blue = tail or ears, green "Gesicht" = where eyes and mouth will be placed later). The canvas size is [AUSGABEGROESSE] pixels. Do not draw the grid, the crosses or any text.

This is the SAME build and the SAME character as in the attached adult image of this form, only at another life stage (as described); the other attached image shows the same life stage on the standard build. Same fur, same line weight, same shading, same paw style. Keep the silhouette type of the build (slender stays slender, round stays round, sturdy stays sturdy).
- Strict pixel art on the grid, every sprite pixel a flat 8x8 square, no anti-aliasing, no stray pixels.
- Dark outline exactly one sprite pixel wide, gray 34.
- ONLY these gray values: 34, 119, 170, 204, 238, 255 (background). Checkerboard dithering for shading, a few fur dots.
- Species-neutral small mammal, three-quarter view slightly turned to the viewer's LEFT.
- Part alone: no other body parts, no ground shadow.
```

### Einzusetzen
| Teil | [TEIL] | [AUSGABEGROESSE] |
|---|---|---|
| Körper rundlich baby | the headless body of the character sitting upright, small and round, a plump spherical belly, short thick legs, drawn as a BABY: much smaller and rounder than the adult, even shorter stubby legs, small chubby paws, almost no neck. NO head, NO ears, NO tail. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 400 x 224 |
| Körper rundlich teenager | the headless body of the character sitting upright, small and round, a plump spherical belly, short thick legs, drawn as a TEENAGER: a bit gangly and lanky, slightly smaller than the adult, slightly oversized paws. NO head, NO ears, NO tail. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 512 x 288 |
| Körper rundlich senior | the headless body of the character sitting upright, small and round, a plump spherical belly, short thick legs, drawn as an ELDERLY animal: hunched rounded back, neck sloping forward and lower (the open neck edge at the red cross is lower than the shoulders), slightly sagging belly, calm resting paws. NO head, NO ears, NO tail. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 544 x 304 |
| Körper schlank baby | the headless body of the character sitting upright, tall and slender, narrow chest, long thin legs, slim back, drawn as a BABY: much smaller and rounder than the adult, even shorter stubby legs, small chubby paws, almost no neck. NO head, NO ears, NO tail. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 432 x 320 |
| Körper schlank teenager | the headless body of the character sitting upright, tall and slender, narrow chest, long thin legs, slim back, drawn as a TEENAGER: a bit gangly and lanky, slightly smaller than the adult, slightly oversized paws. NO head, NO ears, NO tail. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 544 x 400 |
| Körper schlank senior | the headless body of the character sitting upright, tall and slender, narrow chest, long thin legs, slim back, drawn as an ELDERLY animal: hunched rounded back, neck sloping forward and lower (the open neck edge at the red cross is lower than the shoulders), slightly sagging belly, calm resting paws. NO head, NO ears, NO tail. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 576 x 416 |
| Körper kraeftig baby | the headless body of the character sitting upright, sturdy and strong, broad shoulders, wide chest, thick front legs, drawn as a BABY: much smaller and rounder than the adult, even shorter stubby legs, small chubby paws, almost no neck. NO head, NO ears, NO tail. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 464 x 288 |
| Körper kraeftig teenager | the headless body of the character sitting upright, sturdy and strong, broad shoulders, wide chest, thick front legs, drawn as a TEENAGER: a bit gangly and lanky, slightly smaller than the adult, slightly oversized paws. NO head, NO ears, NO tail. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 608 x 368 |
| Körper kraeftig senior | the headless body of the character sitting upright, sturdy and strong, broad shoulders, wide chest, thick front legs, drawn as an ELDERLY animal: hunched rounded back, neck sloping forward and lower (the open neck edge at the red cross is lower than the shoulders), slightly sagging belly, calm resting paws. NO head, NO ears, NO tail. Open neck edge at the red cross, plain fur at the blue cross, paws on the last grid row. | 640 x 384 |
| Kopf klein-rund baby | the head of the character: small and almost perfectly round, drawn as a BABY: larger relative to the body, chubby cheeks, high round forehead, short smooth baby fur without tufts. Plain blank face (NO eyes, NO nose, NO mouth, NO whiskers, NO ears). Closed chin/neck at the bottom, centered on the red cross. | 384 x 320 |
| Kopf klein-rund teenager | the head of the character: small and almost perfectly round, drawn as a TEENAGER: slightly leaner and longer, slimmer cheeks, one small cheeky tuft of fur on the forehead. Plain blank face (NO eyes, NO nose, NO mouth, NO whiskers, NO ears). Closed chin/neck at the bottom, centered on the red cross. | 400 x 304 |
| Kopf klein-rund senior | the head of the character: small and almost perfectly round, drawn as an ELDERLY animal: heavy drooping jowls, slightly tousled fur, a calm tired shape. Plain blank face (NO eyes, NO nose, NO mouth, NO whiskers, NO ears). Closed chin/neck at the bottom, centered on the red cross. | 416 x 320 |
| Kopf breit baby | the head of the character: very wide with big fluffy cheek tufts on both sides, low and broad, drawn as a BABY: larger relative to the body, chubby cheeks, high round forehead, short smooth baby fur without tufts. Plain blank face (NO eyes, NO nose, NO mouth, NO whiskers, NO ears). Closed chin/neck at the bottom, centered on the red cross. | 480 x 352 |
| Kopf breit teenager | the head of the character: very wide with big fluffy cheek tufts on both sides, low and broad, drawn as a TEENAGER: slightly leaner and longer, slimmer cheeks, one small cheeky tuft of fur on the forehead. Plain blank face (NO eyes, NO nose, NO mouth, NO whiskers, NO ears). Closed chin/neck at the bottom, centered on the red cross. | 512 x 336 |
| Kopf breit senior | the head of the character: very wide with big fluffy cheek tufts on both sides, low and broad, drawn as an ELDERLY animal: heavy drooping jowls, slightly tousled fur, a calm tired shape. Plain blank face (NO eyes, NO nose, NO mouth, NO whiskers, NO ears). Closed chin/neck at the bottom, centered on the red cross. | 544 x 352 |

Je Teil **3 Varianten** erzeugen, die beste behalten. Es sind 15 Teile, am besten in Gruppen: zuerst alle Körper einer Form (3 Stufen), dann die Köpfe.

## Ablage
`ki/teile/koerper-<form>-<stufe>-<nr>.png` (zum Beispiel `koerper-schlank-baby-2.png`) und `ki/teile/kopf-<form>-<stufe>-<nr>.png` (zum Beispiel `kopf-breit-senior-1.png`).

## Was danach passiert
- Ich erweitere die Zusammensetzung um die Form-×-Alter-Kombinationen und baue einen Prüfbogen: dieselbe Art als Baby, Teenager, Erwachsener, Senior für alle 4 Körper und 3 Köpfe.
- **Zweiter Auftrag (Posen der Altersstufen):** Schlafen, Laufen und Hüpfen für Baby, Teenager und Senior, **nur Senioren ohne die anderen Posen** (Entscheidung der Nutzerin, weil man mit Senioren nicht mehr interagiert). Baby und Teenager bekommen zusätzlich Geduckt. Zuerst für die Standardform; ob auch je Form, entscheiden wir, wenn wir sehen, wie stark die Silhouette dabei abweicht.

## Grenzen
- Das ist viel Arbeit auf einmal (15 Teile mal 3 Varianten). Wenn dir das zu viel ist, fangen wir mit einer Form an (zum Beispiel schlank) und prüfen, ob der Aufwand sich lohnt.
- Ohren, Schwänze und Gesichter haben nur eine Größe. Das hat bei den Standard-Altersstufen gut funktioniert.
