# Prompt für die Bild-KI: kleine Aufsätze (27. November 2026)

Sechs kleine Teile, die auf Kopf oder Hals sitzen: **Schleife** (Mia), **Glöckchen** (Glöckchen), **Halsband** (Klecksi), **Augenmaske** (Groschen), **Backenblitze** (Funke). Dazu optional ein **geringelter Schwanz** (Groschen). Gleiche Methode wie [PROMPT-ki-teile.md](PROMPT-ki-teile.md): ein Teil allein auf der Schablone, feste Grauwerte, Skript setzt zusammen.

## Wo sie sitzen
| Aufsatz | Lage | Ansatz im Teil | Wohin | Ebene |
|---|---|---|---|---|
| Schleife | auf dem Kopf am rechten Ohr | Knoten unten Mitte (12, 16) | rechtes Ohr, leicht innen | vor dem Kopf |
| Glöckchen | an einem Band unter dem Kinn | oben Mitte (8, 2) | unter dem Kinn, etwas links | vor dem Körper |
| Halsband | schmales Band um den Hals mit Marke | oben Mitte (22, 3) | unter dem Kinn | nach dem Kopf |
| Augenmaske | dunkle Fläche über beiden Augen | Mitte (26, 8) | über den Augen | **vor** dem Kopf, **hinter** dem Gesicht (die Augen liegen darüber) |
| Backenblitz | kleiner Blitz, **linke** Wange (rechter wird gespiegelt) | Mitte (6, 7) | Wange | nach dem Gesicht |

Die Figur schaut leicht nach links (3/4).

## Maße
| Teil | Datei | Raster | Ausgabe (8-fach) | Schablone |
|---|---|---|---|---|
| Schleife | `aufsatz-schleife-<nr>.png` | 24 × 20 | 192 × 160 | `ki/schablone-aufsatz-schleife.png` |
| Glöckchen | `aufsatz-gloeckchen-<nr>.png` | 16 × 20 | 128 × 160 | `…-gloeckchen.png` |
| Halsband | `aufsatz-halsband-<nr>.png` | 44 × 14 | 352 × 112 | `…-halsband.png` |
| Augenmaske | `aufsatz-augenmaske-<nr>.png` | 52 × 16 | 416 × 128 | `…-augenmaske.png` (blasse Linie = Augenhöhe, nicht zeichnen) |
| Backenblitz | `aufsatz-backenblitz-<nr>.png` | 12 × 14 | 96 × 112 | `…-backenblitz.png` |
| Schwanz geringelt | `schwanz-geringelt-<nr>.png` | 32 × 40 | 256 × 320 | `ki/schablone-schwanz.png` |

## Grauwerte (Rollen)
34 Umriss (1 Pixel), 119 dunkel, 170 Schatten, 204 Fell/Mittelton, 238 hell, 255 Hintergrund. Nur diese Werte, Dithering, keine Halbtöne.
- Schleife: Stoff 238, Schatten 170, Knoten 119.
- Glöckchen: Glocke 238, Schatten 170, Schlitz 34, kleine Öse oben mit Band 119.
- Halsband: Band 119, Schnalle und Marke 238, Umriss 34.
- Backentasche: wie Fell (204 mit 170-Schatten, 238-Licht), weil sie mit dem Fellton umgefärbt wird.
- Augenmaske: Fläche 119 mit weicheren Rändern in 170.
- Backenblitz: Blitz 34 (ein dunkles, deutlich lesbares Zickzack).
- Schwanz geringelt: Fell 204, 3–4 dunkle Ringe 119, Spitze 238.

## Prompt (zum Kopieren, Englisch)
```text
Create ONE standalone pixel-art game sprite part on a pure white background: [TEIL].
It must fit exactly on the attached template (8x8 screen pixels per sprite pixel; the red cross marks the attachment point; pale blue lines are only hints and must NOT be drawn). The canvas size is [AUSGABEGROESSE] pixels. Do not draw the grid, the cross, the hints or any text.

Style: match the attached style image in line weight and shading. The character faces slightly to the viewer's LEFT.
- Strict pixel art on the grid, every sprite pixel a flat 8x8 square, no anti-aliasing, no stray pixels, no half-transparency.
- Dark outline exactly one sprite pixel wide, gray 34.
- ONLY these gray values: 34, 119, 170, 204, 238, 255 (background). Checkerboard dithering for shading.
- Cute, clean, readable at small size. Part alone, no other body parts.
```

### Einzusetzen
| Teil | [TEIL] | [AUSGABEGROESSE] |
|---|---|---|
| Schleife | a cute bow (ribbon with two loops and a small knot in the middle), seen from the front, light cloth gray 238, knot 119; the knot center sits at the red cross | 192 x 160 |
| Glöckchen | a small round bell (jingle bell) with a slit and a tiny loop at the top, hanging down from the red cross | 128 x 160 |
| Halsband | a narrow collar band lying horizontally, slightly curved like it wraps around a neck, dark gray 119, with a small round tag or buckle in the middle (light gray 238); red cross at the top center | 352 x 112 |
| Augenmaske | a raccoon-style dark eye mask: one wide dark band (gray 119) that covers both eye areas, with soft slightly pointed ends going outward and down; the area in the middle between the eyes is connected; center at the red cross | 416 x 128 |
| Backenblitz | a small lightning-bolt mark for the LEFT cheek, dark gray 34, bold zigzag, 3 sprite pixels thick, readable at small size; center at the red cross | 96 x 112 |
| Schwanz geringelt (Groschen) | a bushy raccoon tail with 3 to 4 dark rings (gray 119) across it and a light tip (238), seen from the side, rising up to the right, attachment on the left | 256 x 320 |

Je Teil **3 Varianten** erzeugen, die beste behalten.

## Ablage
`ki/teile/aufsatz-<name>-<nr>.png` (zum Beispiel `aufsatz-schleife-1.png`), und `ki/teile/schwanz-geringelt-<nr>.png`. Backenblitz nur die **linke** Seite, ich spiegele die rechte.

## Was danach passiert
`formen.py` setzt die Aufsätze über den Parameter `aufsaetze` ein (Lage: `basis3_layout.py`, `AUFSAETZE`). Mit Platzhaltern habe ich die Lage schon geprüft; mit den echten Bildern prüfe ich sie noch einmal und passe die Position an. Dann kommt ein Prüfbogen mit Mia, Glöckchen, Klecksi, Mampf, Groschen und Funke.

## Grenzen
- Die Aufsätze sind für die **sitzende** Haltung und den 3/4-Kopf eingestellt. Bei Köpfen mit anderer Größe (klein-rund, breit) sitzen sie je nach Kopf etwas anders; das stelle ich über die Position ein.
- Die Augenmaske überdeckt Teile des Gesichts. Sie liegen deshalb **hinter** dem Gesicht (die Augen sind darüber).
