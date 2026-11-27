# Prompt für die Bild-KI: Körper für die Posen (27. November 2026)

Fortsetzung von [PROMPT-ki-basis.md](PROMPT-ki-basis.md). Der sitzende Körper (`ki/teile/koerper-sitzend-2.png`, Empfehlung) steht. Jetzt kommen die anderen Haltungen als **weitere Körperteile**, jeweils **ohne Kopf, ohne Ohren, ohne Schwanz, ohne Halskrause**. Kopf, Ohren, Gesicht und Schwanz bleiben die vorhandenen Teile; sie wandern mit dem Hals- und Schwanzansatz.

Welche Spielposen damit abgedeckt sind: siehe [POSEN-UND-EMOTIONEN.md](POSEN-UND-EMOTIONEN.md). Die 11 Gefühle der Gruppe A brauchen keinen neuen Körper (Gesicht + Effekt).

## Anhängen (bei jedem Auftrag)
1. Die **Schablone der Pose** (Kreuze: rot = Hals, blau = Schwanz).
2. **Stil und Proportion:** `ki/teile/koerper-sitzend-2.png` (derselbe Fellton, dieselbe Linie, dieselbe Pfotengröße und Beinstärke).

## Maße und Ansatzpunkte
| Pose | Datei (Ablage) | Raster | Ausgabe (8-fach) | Hals (rot) | Schwanz (blau) |
|---|---|---|---|---|---|
| laufen | `koerper-laufen-<nr>.png` | 88 × 48 | 704 × 384 | (22, 8) | (76, 14) |
| hüpfen | `koerper-huepfen-<nr>.png` | 80 × 52 | 640 × 416 | (30, 8) | (68, 18) |
| geduckt (untersuchen, schnuppern) | `koerper-geduckt-<nr>.png` | 88 × 44 | 704 × 352 | (14, 22) | (78, 8) |
| eingerollt (schlafen) | `koerper-eingerollt-<nr>.png` | 88 × 40 | 704 × 320 | (24, 10) | (66, 18) |

Grauwerte und Stil exakt wie in PROMPT-ki-basis.md (34 Umriss, 119 innen, 170 Schatten, 204 Fell, 238 Licht, 255 Hintergrund; Dithering; 1-Pixel-Umriss; keine Halbtöne). Blickrichtung immer nach **links**.

## Prompt (zum Kopieren, Englisch)
```text
Create ONE standalone pixel-art game sprite part on a pure white background: [TEIL].
It must fit exactly on the attached template (8x8 screen pixels per sprite pixel; the red cross "Hals" marks the open neck where a head will be attached, the blue cross "Schwanz" marks where a tail will be attached). The canvas size is [AUSGABEGROESSE] pixels. Do not draw the grid, the crosses or any text.

Draw it in EXACTLY the style, fur tones, line weight, paw size and leg thickness of the second attached image (the sitting body). Same character, same fur, only another pose.
- Strict pixel art on the grid, every sprite pixel a flat 8x8 square, no anti-aliasing, no stray pixels.
- Dark outline exactly one sprite pixel wide, gray 34.
- ONLY these gray values: 34, 119, 170, 204, 238, 255 (background). Checkerboard dithering for shading, a few fur dots.
- Species-neutral small mammal. NO head, NO ears, NO tail, NO neck ruff, NO face. The neck opening is a flat open edge at the red cross (the head will cover it). Leave the area at the blue cross plain fur.
- The pose faces to the viewer's LEFT.
```

### Einzusetzen
| Pose | [TEIL] | [AUSGABEGROESSE] |
|---|---|---|
| laufen | the headless body of the character walking to the left in a three-quarter side view: mid-stride, front near paw lifted, hind legs pushing off, back slightly arched. Neck opening at the red cross at the upper left, the rump rises to the right. | 704 x 384 |
| hüpfen | the headless body of the character jumping with joy: all four paws off the ground, front paws forward, hind legs stretched back, body slightly curved, the whole body diagonal in the air. | 640 x 416 |
| geduckt | the headless body of the character crouching low to sniff the ground: front legs stretched forward and low, chest near the ground, rump raised, hind legs bent. The neck opening is at the lower left near the ground (red cross), the rump with the blue cross is higher on the right. | 704 x 352 |
| eingerollt | the headless body of the character sleeping curled up lying on the ground: a round compact body resting on its side, front paws tucked forward under where the head will rest (red cross), hind legs folded, a rounded back. Leave a flat resting spot for the head at the red cross. | 704 x 320 |

Je Pose **3 Varianten** erzeugen und die beste behalten.

## Was danach passiert
Ich setze die Körper mit den vorhandenen Köpfen, Ohren, Gesichtern und Schwänzen zusammen. Die Ansatzpunkte und Zielpunkte je Pose stehen in `basis3_layout.py` (`POSEN`). Passt ein Kopf nicht gut auf den Körper (zu hoch, zu weit vorn), ändere ich nur die Zielpunkte, nicht das Bild.

## Noch offen (nicht in diesem Auftrag)
- **Pfote hoch** (denken, zeigen): braucht zusätzlich eine Vorderpfote als Teil, die **vor** dem Kopf liegt.
- **Kopf gedreht** (Fenster) und **Rückansicht** (drehen): eigene Köpfe bzw. Körper.
- **Profil-Köpfe** für laufen und hüpfen: Beim Laufen schaut der Kopf nach links; der 3/4-Kopf kann das mitmachen, ein reines Profil wäre schöner.
