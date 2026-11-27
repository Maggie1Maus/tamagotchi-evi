# Prompt für die Bild-KI: Basis im 3/4-Blick (Körper, Kopf, Halskrause)

Stand 27. November 2026. Ersetzt die runde Basis aus `bausatz2.py`. Gleiche Methode wie [PROMPT-ki-teile.md](PROMPT-ki-teile.md): **ein Teil allein** auf der Schablone, feste Grauwert-Rollen, danach setzt das Skript zusammen.

**Entscheidung der Nutzerin (27.11.):** 3/4-Blick und Leinwand **112 × 112**.

## Zielbild
Ein **sitzendes kleines Säugetier** auf vier Pfoten, wie die Spielposen, aber **keine Evi** und **ohne Artmerkmale**: Ohren, Schwanz und Gesicht kommen als Teile dazu. Körper und Kopf sollen deshalb zu Katze, Hund, Eichhörnchen oder Hase gleichermaßen passen (neutral, kompakt, großer Kopf, kleine Pfoten).

**Blickrichtung (immer gleich):** Der Körper und der Kopf sind **leicht nach links gedreht** (aus Sicht der Betrachterin), etwa 3/4. Der Schwanz kommt rechts hinten dahinter. Die Teile müssen alle in diese Richtung passen.

## Leinwand und Anordnung
112 × 112 Rasterpixel. Reihenfolge von hinten nach vorn: Schwanz, Körper, Ohren, Kopf, Halskrause, Gesicht. Die Ansatzpunkte stehen in `basis3_layout.py`.

## Anhängen
1. Die Schablone des Teils (Raster 8 × 8 Pixel je Rasterpixel, farbige Kreuze = Ansatzpunkte):
   `ki/schablone-koerper.png`, `ki/schablone-kopf.png`, `ki/schablone-kruse.png`
2. Als **Stilvorlage nur für Linie und Verlauf**: `ki/teile/ohr-spitz-1.png` und `ki/teile/schwanz-buschig-1.png`. Nicht die Formen übernehmen.

## Maße
| Teil | Raster | Ausgabe (8-fach) | Ansatzpunkte (Kreuze in der Schablone) |
|---|---|---|---|
| Körper | 80 × 48 | 640 × 384 | Hals oben Mitte (40, 3); Schwanz rechts hinten (66, 26) |
| Kopf | 60 × 44 | 480 × 352 | Hals unten Mitte (30, 40); Ohr links (13, 8); Ohr rechts (47, 8); Gesicht (30, 24) |
| Halskrause | 56 × 20 | 448 × 160 | Hals oben Mitte (28, 2) |

## Grauwerte (Rollen, wie bei den Ohren)
| Grauwert | Rolle |
|---:|---|
| 34 | Umriss, 1 Rasterpixel breit |
| 119 | Innenseite (Ballen, Innenbein, Schatten unter Kinn) |
| 170 | Schatten (unten rechts) |
| 204 | Fell (Grundton) |
| 238 | Licht (oben links), Fellspitzen; bei der **Halskrause die Hauptfläche** (wird als Kranzton umgefärbt) |
| 255 | Hintergrund |

Nur diese Werte. Verläufe durch **Schachbrett-Dithering**, dazu vereinzelte Fellpunkte (ein Pixel 170 oder 238). Kleine Fellbüschel dürfen den Umriss an wenigen Stellen unterbrechen.

## Prompt (zum Kopieren, Englisch)
```text
Create ONE standalone pixel-art game sprite part on a pure white background: [TEIL].
It must fit exactly on the attached template (grid of 8x8 screen pixels per sprite pixel; colored crosses mark attachment points). The canvas size is [AUSGABEGROESSE] pixels. Place the part so that its attachment points sit on the crosses. Do not draw the grid, the crosses, or any text.

STYLE (match the attached style images in line and shading only, NOT in shape):
- Strict pixel art on that grid: every sprite pixel is a flat 8x8 square. No anti-aliasing, no blur, no gradients, no half-transparent pixels, no stray single pixels.
- A continuous dark outline exactly ONE sprite pixel wide, gray value 34, stepped pixel edges.
- Fill with fur gray 204. Shade the lower right with 170 and the upper left with 238, using checkerboard dithering between tones. A few scattered single-pixel fur dots (170 or 238). Inner areas gray 119.
- ONLY these gray values: 34, 119, 170, 204, 238, 255 (background). No color.
- Cute, compact, slightly stylized small mammal, species-neutral (could be a cat, dog, squirrel or rabbit). NOT a fox. Large head, small paws.
- The character is turned slightly (three-quarter view) toward the viewer's LEFT. Keep that direction.
- Draw the part alone, no ground shadow, no other body parts.
```

### Einzusetzen
| Teil | [TEIL] | [AUSGABEGROESSE] |
|---|---|---|
| Körper | the BODY of a small mammal sitting upright on four paws, seen in three-quarter view turned slightly to the viewer's left, WITHOUT head, WITHOUT ears, WITHOUT tail and WITHOUT neck ruff: chest and shoulders, two straight front legs with small paws and toe lines, one folded hind leg (haunch) visible on the right side. The top edge is the open neck, centered on the red cross "Hals" (a head will cover it). Leave the spot at the blue cross "Schwanz" plain fur (a tail will be attached there). Bottom of the paws on the last grid row. | 640 x 384 |
| Kopf | the HEAD of a small mammal, three-quarter view turned slightly to the viewer's left, with a plain blank face: NO eyes, NO nose, NO mouth, NO whiskers, NO ears. Round head with soft cheeks, a gently rounded muzzle area, forehead tuft optional. The bottom is the chin and neck, closed by the outline, centered on the red cross "Hals". Keep the face area plain fur (the green cross marks where eyes and mouth will be placed later). | 480 x 352 |
| Halskrause | a fluffy neck ruff (collar of light fur) with a scalloped, tufted lower edge, seen from the front slightly turned left, wide at the shoulders; main surface gray 238, shading 204 and 170, outline 34. The top edge is straight enough to sit under the chin, centered on the red cross. | 448 x 160 |

Bitte je Teil **3 Varianten** erzeugen und die beste behalten.

## Ablage
`entwuerfe/bausatz-test/ki/teile/koerper-sitzend-<nr>.png`, `kopf-3-4-<nr>.png`, `kruse-flauschig-<nr>.png`.

## Wichtig für später
Weil die Basis jetzt im 3/4-Blick gedreht ist, **passen die bisherigen frontalen Ohren, Schwänze und Gesichter nicht mehr genau**. Sie müssen danach noch einmal für diese Blickrichtung bestellt werden (Ohr links = nah, Ohr rechts = fern, Gesichter mit dem Prompt aus PROMPT-ki-teile.md). Deshalb zuerst nur diese drei Teile, dann prüfen wir, ob dir die Basis gefällt, bevor wir den Rest neu machen.
