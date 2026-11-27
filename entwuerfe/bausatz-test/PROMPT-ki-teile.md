# Prompt für die Bild-KI: freistehende Teile für den Bausatz

Stand 27. November 2026. Gilt für den **Bausatz** (`bausatz2.py`), nicht für die alten Evi-Teile in `baukasten/`.

## Warum so
Die KI soll **ein Teil allein** zeichnen (Ohr, Schwanz, später Flügel, Hörner, Gesichter), **nicht in ein fertiges Bild hinein**. Dann muss sie kein Raster, keinen Kopfrand und keine Figur treffen. Mein Skript setzt das Teil an den Ansatzpunkt, rechnet das Fell auf jeden Fellton um und verschmilzt den Umriss. Damit das klappt, braucht jedes Bild **feste Maße und feste Grauwerte**. Dieser Prompt ist deshalb eine Vorlage, die für jedes weitere Teil gleich bleibt: nur die Zeile `[TEIL]` wechselt.

## Anhängen (bei jedem Auftrag)
1. **Schablone**: `ki/schablone-ohr.png` (Ohren, Hörner …), `ki/schablone-ohr-haengend.png` (Hängeohren) oder `ki/schablone-schwanz.png` (Schwänze, Flügel …). Sie ist die Zeichenfläche: Raster 8 × 8 Pixel je Rasterpixel, roter Punkt = Ansatz.
2. **Stilvorlage**: `ki/stilvorlage-kopf.png` bzw. `ki/stilvorlage-schwanz.png` (so sieht ein fertiges Teil in unserem Stil aus: Umriss, Verlauf, Fellpunkte).

## Maße (immer gleich)
| Teil | Raster | Ausgabegröße (8-fach) | Ansatz (roter Punkt) |
|---|---|---|---|
| Ohr (linkes Ohr, nach oben) | 24 × 32 | 192 × 256 | unten Mitte (12, 29) |
| Hängeohr (linkes Ohr, nach unten) | 24 × 32 | 192 × 256 | oben Mitte (12, 2) |
| Schwanz (nach rechts) | 32 × 40 | 256 × 320 | links Mitte (3, 29) |

## Grauwerte (Rollen, immer gleich)
Das Skript erkennt die Rollen am Grauwert und färbt sie um. Deshalb **genau diese Werte**:
| Grauwert (0–255) | Rolle |
|---:|---|
| 34 | Umriss, 1 Rasterpixel breit |
| 119 | Innenohr / Innenseite |
| 170 | Schatten (unten rechts) |
| 204 | Fell (Grundton) |
| 238 | Licht (oben links) und Fellspitzen |
| 255 (Weiß) | Hintergrund, wird durchsichtig |

Nur diese Werte, dazu **Dithering** (Schachbrettmuster aus zwei benachbarten Werten) für Verläufe und vereinzelte **Fellpunkte** (ein Pixel 170 oder 238 im Fell).

## Prompt (zum Kopieren, Englisch)
```text
Create ONE standalone pixel-art game sprite part on a pure white background: [TEIL].
It must fit exactly on the attached template (grid of 8x8 screen pixels per sprite pixel, red dot = attachment point). The canvas size is [AUSGABEGROESSE] pixels. Put the part so that its attachment end sits on the red dot. Do not draw the grid, the red dot or any text.

STYLE (match the second attached image exactly):
- Strict pixel art on that grid: every sprite pixel is a flat 8x8 square. No anti-aliasing, no blur, no gradients, no half-transparent pixels, no stray single pixels outside the shape.
- A continuous dark outline exactly ONE sprite pixel wide, gray value 34, stepped pixel edges.
- Fill with a medium-light fur gray 204. Shade the lower right with gray 170 and the upper left with gray 238, using checkerboard dithering between the tones. Add a few scattered single-pixel fur dots (170 or 238).
- Inner/underside areas gray 119 (as the inner ear in the style image).
- ONLY these gray values: 34, 119, 170, 204, 238, 255 (background). No color.
- Fluffy fur: small tufts break the outline in a few places, like in the style image.
- Draw the part alone. No head, no body, no shadow on the ground.
- Orientation: [ORIENTIERUNG].
```

### Einzusetzen
| Teil | [TEIL] | [AUSGABEGROESSE] | [ORIENTIERUNG] |
|---|---|---|---|
| Ohr spitz (Katze) | a pointed upright cat ear with a darker inner ear | 192 x 256 | the LEFT ear of a character seen from the front, tip pointing up and leaning slightly outward (to the left) |
| Ohr rund (Bär) | a small round bear ear with a darker round inner ear | 192 x 256 | left ear, upright |
| Ohr lang (Hase) | a long narrow upright rabbit ear with a long inner ear | 192 x 256 | left ear, upright, leaning slightly outward |
| Ohr hängend (Hund) | a long floppy dog ear hanging down, fur dots, no inner ear | 192 x 256 | left ear, hanging down, rounded tip at the bottom; the attachment is at the TOP end |
| Ohr mit Büschel | a short pointed squirrel ear with a small tuft of three fur hairs at the tip | 192 x 256 | left ear, upright |
| Schwanz buschig | a big bushy squirrel tail, lighter tip (gray 238) | 256 x 320 | tail seen from the front side, rising up to the right, attachment on the left |
| Schwanz Puschel | a small round fluffy pom-pom tail | 256 x 320 | attachment on the left |
| Schwanz dünn | a thin curled cat tail with a small round tip | 256 x 320 | attachment on the left, curling up |
| Schwanz Blitz | a zigzag lightning-bolt shaped tail | 256 x 320 | attachment on the left, rising up to the right |

Bitte je Teil **3 Varianten** erzeugen lassen und die beste behalten.

## Ablage
`entwuerfe/bausatz-test/ki/teile/<art>-<name>-<nr>.png`, zum Beispiel `ohr-spitz-1.png`, `schwanz-buschig-2.png`. Nur die **linke** Version von Ohren liefern, ich spiegele das rechte Ohr.

## Was ich danach prüfe und automatisch mache
1. Größe stimmt (Rastermaß), nur die sechs Grauwerte, nur ganze Rasterpixel.
2. Hintergrund (Weiß) wird durchsichtig, kleine Reste werden entfernt.
3. Das Teil wird am roten Ansatzpunkt platziert (Ohren hinter dem Kopf, Schwanz hinter dem Körper) und der Umriss mit dem Kopf verschmolzen.
4. Fell, Licht und Schatten werden auf jeden Fellton umgerechnet, damit Kinder alle Töne bekommen.
Passt ein Bild nicht (falsche Größe, andere Grautöne, unsauber), sage ich, woran es liegt, und passe nur diesen Prompt an.

## Später dieselbe Vorlage nutzen für
Flügel, Hörner, Geweih, Fühler, Antennen, Stacheln, Rüssel, Mähne, Gesichter (Augen und Mund als Teile): Schablone und Maße je Teil festlegen, Rollen-Tabelle bleibt gleich.

## Gesichter als freistehende Teile

`ki/schablone-gesicht.png` und `ki/stilvorlage-kopf.png` anhängen.
Das Gesicht braucht 40 × 28 Rasterpixel bzw. 320 × 224 Bildpixel. Der rote
Punkt (20, 18) markiert die Nase. Die Augenmitten liegen ungefähr bei
(7, 11) und (31, 11), der Mund darunter, etwa bei (20, 24).

```text
Create ONE standalone grayscale pixel-art face overlay on the attached 320 x 224
template. Draw only two eyes, optional brows, a small centered nose, and a mouth.
Expression: [EXPRESSION]. Match the attached style reference.
Each pixel is a flat 8 x 8 block. Use ONLY gray 34 (dark strokes), 119 (nose
and dark accents), 170 (midtones), 204 (tongue), 238 (bright highlights)
and 255 (transparent background after import). Keep white eye highlights at
238 so they survive import. Leave the blank spaces between features white.
Place the nose on the red mark; omit the mark, grid, head, fur, cheeks,
whiskers and text. No antialiasing or partial blocks.
```

Je drei Varianten für freundlich, lachend, überrascht, traurig, wütend und
müde erzeugen. Die ausgewählte Variante als
`ki/teile/gesicht-<ausdruck>-<nr>.png` ablegen. Der Bausatz setzt das
gesamte Gesicht vor den Kopf. Die Gesichtstöne sind unabhängig vom Fellton.
