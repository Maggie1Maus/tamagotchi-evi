# Prompt für die Bild-KI: Posen der Altersstufen (27. November 2026)

Entscheidung der Nutzerin: **Senioren brauchen nur Schlafen, Laufen und Hüpfen**, weil man mit ihnen nicht mehr interagiert. **Baby und Teenager** bekommen zusätzlich **Geduckt** (Untersuchen, Schnuppern). Das sind **11 Körper**, jeweils für die **Standardform** (Körper `sitzend`, Kopf 3-4). Ob wir die Posen auch je Körperform brauchen, entscheiden wir, wenn wir sehen, wie stark die Silhouette dabei abweicht.

Gleiche Methode wie [PROMPT-ki-posen.md](PROMPT-ki-posen.md): Der Körper wird **ohne Kopf, Ohren und Schwanz** gezeichnet. Kopf, Ohren, Gesichter und Schwänze bleiben die vorhandenen Teile. Die **Gefühle** der Gruppe A (ruhe, glücklich, hungrig, traurig, müde, krank, schmutzig, beleidigt, staunen, singen, gähnen) brauchen keinen neuen Körper, nur Gesicht und Effekt.

## Anhängen (bei jedem Auftrag)
1. Die **Schablone** der Pose (Kreuze: rot = Hals, blau = Schwanz).
2. **Pose und Stil:** der **erwachsene Körper derselben Pose** (zum Beispiel `ki/teile/koerper-laufen-2.png`).
3. **Die Lebensphase:** der **sitzende Körper der Stufe** (`koerper-baby-3.png`, `koerper-teenager-2.png`, `koerper-senior-2.png`), damit Proportionen, Fell und Pfoten zur Stufe passen.

## Maße und Ansatzpunkte
| Pose und Stufe | Datei | Raster | Ausgabe (8-fach) | Hals | Schwanz | Schablone |
|---|---|---|---|---|---|---|
| laufen baby | `koerper-laufen-baby-<nr>.png` | 62 × 34 | 496 × 272 | (15, 6) | (53, 10) | `ki/schablone-koerper-laufen-baby.png` |
| huepfen baby | `koerper-huepfen-baby-<nr>.png` | 56 × 36 | 448 × 288 | (21, 6) | (48, 13) | `ki/schablone-koerper-huepfen-baby.png` |
| geduckt baby | `koerper-geduckt-baby-<nr>.png` | 62 × 30 | 496 × 240 | (10, 15) | (55, 6) | `ki/schablone-koerper-geduckt-baby.png` |
| eingerollt baby | `koerper-eingerollt-baby-<nr>.png` | 62 × 28 | 496 × 224 | (17, 7) | (46, 13) | `ki/schablone-koerper-eingerollt-baby.png` |
| laufen teenager | `koerper-laufen-teenager-<nr>.png` | 80 × 44 | 640 × 352 | (20, 7) | (68, 13) | `ki/schablone-koerper-laufen-teenager.png` |
| huepfen teenager | `koerper-huepfen-teenager-<nr>.png` | 72 × 48 | 576 × 384 | (27, 7) | (61, 16) | `ki/schablone-koerper-huepfen-teenager.png` |
| geduckt teenager | `koerper-geduckt-teenager-<nr>.png` | 80 × 40 | 640 × 320 | (13, 20) | (70, 7) | `ki/schablone-koerper-geduckt-teenager.png` |
| eingerollt teenager | `koerper-eingerollt-teenager-<nr>.png` | 80 × 36 | 640 × 288 | (22, 9) | (59, 16) | `ki/schablone-koerper-eingerollt-teenager.png` |
| laufen senior | `koerper-laufen-senior-<nr>.png` | 84 × 46 | 672 × 368 | (21, 8) | (72, 13) | `ki/schablone-koerper-laufen-senior.png` |
| huepfen senior | `koerper-huepfen-senior-<nr>.png` | 76 × 50 | 608 × 400 | (29, 8) | (65, 17) | `ki/schablone-koerper-huepfen-senior.png` |
| eingerollt senior | `koerper-eingerollt-senior-<nr>.png` | 84 × 38 | 672 × 304 | (23, 10) | (63, 17) | `ki/schablone-koerper-eingerollt-senior.png` |

## Grauwerte, Stil, Richtung (wie bisher)
34 Umriss (1 Pixel), 119 innen, 170 Schatten, 204 Fell, 238 Licht, 255 Hintergrund. Nur diese Werte, Dithering, ein paar Fellpunkte, keine Halbtöne, kein Anti-Aliasing. Blickrichtung nach **links**.

## Prompt (zum Kopieren, Englisch)
```text
Create ONE standalone pixel-art game sprite part on a pure white background: [TEIL].
It must fit exactly on the attached template (8x8 screen pixels per sprite pixel; the red cross "Hals" marks the open neck where a head will be attached, the blue cross "Schwanz" marks where a tail will be attached). The canvas size is [AUSGABEGROESSE] pixels. Do not draw the grid, the crosses or any text.

Take the POSE from the first attached image (the adult body in this pose) and the PROPORTIONS, fur, line weight, paw size and leg thickness from the second attached image (the sitting body of this life stage). It must be the same character at that life stage, just in this pose.
- Strict pixel art on the grid, every sprite pixel a flat 8x8 square, no anti-aliasing, no stray pixels.
- Dark outline exactly one sprite pixel wide, gray 34.
- ONLY these gray values: 34, 119, 170, 204, 238, 255 (background). Checkerboard dithering for shading, a few fur dots.
- Species-neutral small mammal. NO head, NO ears, NO tail, NO neck ruff, NO face. The neck opening is a flat open edge at the red cross (the head will cover it). Leave the area at the blue cross plain fur.
- The pose faces to the viewer's LEFT.
```

### Einzusetzen
| Pose und Stufe | [TEIL] | [AUSGABEGROESSE] |
|---|---|---|
| laufen baby | the headless body of a BABY version: much smaller and rounder, plump, short stubby legs, small chubby paws of the character, walking to the left in a three-quarter side view: mid-stride, front near paw lifted, hind legs pushing off, back slightly arched. Neck opening at the red cross at the upper left, the rump rises to the right. NO head, NO ears, NO tail. | 496 x 272 |
| huepfen baby | the headless body of a BABY version: much smaller and rounder, plump, short stubby legs, small chubby paws of the character, jumping with joy: all four paws off the ground, front paws forward, hind legs stretched back, body slightly curved, the whole body diagonal in the air. NO head, NO ears, NO tail. | 448 x 288 |
| geduckt baby | the headless body of a BABY version: much smaller and rounder, plump, short stubby legs, small chubby paws of the character, crouching low to sniff the ground: front legs stretched forward and low, chest near the ground, rump raised, hind legs bent. The neck opening is at the lower left near the ground (red cross), the rump with the blue cross is higher on the right. NO head, NO ears, NO tail. | 496 x 240 |
| eingerollt baby | the headless body of a BABY version: much smaller and rounder, plump, short stubby legs, small chubby paws of the character, sleeping curled up lying on the ground: a round compact body resting on its side, front paws tucked forward under where the head will rest (red cross), hind legs folded, a rounded back. Leave a flat resting spot for the head at the red cross. NO head, NO ears, NO tail. | 496 x 224 |
| laufen teenager | the headless body of a TEENAGER version: a bit gangly and lanky, slightly smaller than an adult, slightly oversized paws of the character, walking to the left in a three-quarter side view: mid-stride, front near paw lifted, hind legs pushing off, back slightly arched. Neck opening at the red cross at the upper left, the rump rises to the right. NO head, NO ears, NO tail. | 640 x 352 |
| huepfen teenager | the headless body of a TEENAGER version: a bit gangly and lanky, slightly smaller than an adult, slightly oversized paws of the character, jumping with joy: all four paws off the ground, front paws forward, hind legs stretched back, body slightly curved, the whole body diagonal in the air. NO head, NO ears, NO tail. | 576 x 384 |
| geduckt teenager | the headless body of a TEENAGER version: a bit gangly and lanky, slightly smaller than an adult, slightly oversized paws of the character, crouching low to sniff the ground: front legs stretched forward and low, chest near the ground, rump raised, hind legs bent. The neck opening is at the lower left near the ground (red cross), the rump with the blue cross is higher on the right. NO head, NO ears, NO tail. | 640 x 320 |
| eingerollt teenager | the headless body of a TEENAGER version: a bit gangly and lanky, slightly smaller than an adult, slightly oversized paws of the character, sleeping curled up lying on the ground: a round compact body resting on its side, front paws tucked forward under where the head will rest (red cross), hind legs folded, a rounded back. Leave a flat resting spot for the head at the red cross. NO head, NO ears, NO tail. | 640 x 288 |
| laufen senior | the headless body of an ELDERLY version: slower and calmer, hunched rounded back, slightly sagging belly, thin legs; movements are gentle (a slow trot, a small careful hop, a deep sleep) of the character, walking to the left in a three-quarter side view: mid-stride, front near paw lifted, hind legs pushing off, back slightly arched. Neck opening at the red cross at the upper left, the rump rises to the right. NO head, NO ears, NO tail. | 672 x 368 |
| huepfen senior | the headless body of an ELDERLY version: slower and calmer, hunched rounded back, slightly sagging belly, thin legs; movements are gentle (a slow trot, a small careful hop, a deep sleep) of the character, jumping with joy: all four paws off the ground, front paws forward, hind legs stretched back, body slightly curved, the whole body diagonal in the air. NO head, NO ears, NO tail. | 608 x 400 |
| eingerollt senior | the headless body of an ELDERLY version: slower and calmer, hunched rounded back, slightly sagging belly, thin legs; movements are gentle (a slow trot, a small careful hop, a deep sleep) of the character, sleeping curled up lying on the ground: a round compact body resting on its side, front paws tucked forward under where the head will rest (red cross), hind legs folded, a rounded back. Leave a flat resting spot for the head at the red cross. NO head, NO ears, NO tail. | 672 x 304 |

Je Teil **3 Varianten** erzeugen, die beste behalten. Am besten gruppiert nach Stufe (alle Posen des Babys, dann Teenager, dann Senior).

## Ablage
`ki/teile/koerper-<pose>-<stufe>-<nr>.png`, zum Beispiel `koerper-laufen-baby-2.png`, `koerper-eingerollt-senior-1.png`.

## Was danach passiert
Ich setze die Posen mit den Köpfen der Stufe, den Ohren, Schwänzen und Gesichtern zusammen. Die Zielpunkte je Pose und Stufe stehen schon in `basis3_layout.py` (`POSEN`). Ein Prüfbogen zeigt dasselbe Tier in allen Posen der drei Stufen.

## Grenzen
- Nur die Standardform. Ein schlankes oder kräftiges Tier wechselt in den Posen auf den Standardkörper. Wenn das auffällt, kommen die Formen später dazu.
- Bei den Senioren sind die Bewegungen bewusst sanft beschrieben, damit sie nicht wie junge Tiere aussehen.
