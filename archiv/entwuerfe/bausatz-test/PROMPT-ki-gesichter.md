# Prompt für die Bild-KI: fehlende Gesichter (27. November 2026)

Es fehlen die Gesichter für **hungrig, krank, gähnen, singen** und **schmutzig**. Bisher gibt es freundlich, lachend, überrascht, traurig, wütend und müde. Mit den neuen sind alle 11 Gefühle der Spielposen abgedeckt. Effekte (Sabber, Schweißtropfen, Träne, Schmutzflecken, Zzz, Noten) zeichnet das Skript, nicht die KI.

Gleiches Verfahren wie bei den bisherigen Gesichtern, siehe [PROMPT-ki-teile.md](PROMPT-ki-teile.md), Abschnitt „Gesichter als freistehende Teile“.

## Anhängen
`ki/schablone-gesicht.png` und `ki/stilvorlage-kopf.png`. Als Stil zusätzlich ein vorhandenes Gesicht, zum Beispiel `ki/teile/gesicht-freundlich-1.png`, damit Augengröße und Strichstärke gleich bleiben.

## Maße
Das Gesicht hat 40 × 28 Rasterpixel bzw. **320 × 224 Bildpixel**. Der rote Punkt (20, 18) markiert die Nase. Die Augenmitten liegen ungefähr bei (7, 11) und (31, 11), der Mund bei etwa (20, 24).

## Prompt (zum Kopieren, Englisch)
```text
Create ONE standalone grayscale pixel-art face overlay on the attached 320 x 224
template. Draw only two eyes, optional brows, a small centered nose, and a mouth.
Expression: [EXPRESSION]. Match the attached style reference exactly in eye size,
line weight and shading.
Each pixel is a flat 8 x 8 block. Use ONLY gray 34 (dark strokes), 119 (nose
and dark accents), 170 (midtones), 204 (tongue), 238 (bright highlights)
and 255 (transparent background after import). Keep white eye highlights at
238 so they survive import. Leave the blank spaces between features white.
Place the nose on the red mark; omit the mark, grid, head, fur, cheeks,
whiskers and text. No antialiasing or partial blocks.
```

### Einzusetzen ([EXPRESSION])
| Ablage-Name | Ausdruck |
|---|---|
| hungrig | hungry: big round eyes with large shining highlights, eyebrows slightly raised and tilted up in the middle, a small open mouth with the tongue licking the upper lip (a hint of a drool drop is NOT needed, it is added by script) |
| krank | sick and dizzy: half-closed heavy eyelids with small dark eyes looking aside, brows tilted, a wobbly wavy mouth line, slightly pained look; no sweat drop (added by script) |
| gaehnen | yawning: both eyes squeezed shut as curved lines, a very wide open mouth shaped like a big rounded oval with a visible tongue, nose scrunched slightly |
| singen | singing: both eyes closed as happy upward arches, a wide round open mouth in an O shape with a small tongue, brows relaxed and raised; no music notes (added by script) |
| schmutzig | grumpy and dirty: eyes half open with a flat annoyed look, one brow lower than the other, mouth a small flat line pulled to one side; no dirt spots (added by script) |

Je Ausdruck **3 Varianten** erzeugen und die beste behalten.

## Ablage
`ki/teile/gesicht-<name>-<nr>.png`, zum Beispiel `gesicht-hungrig-1.png`, `gesicht-gaehnen-2.png`. Die Namen müssen genau so geschrieben sein (ohne Umlaute), damit das Skript sie findet.

## Was danach passiert
Ich ordne die neuen Gesichter den Gefühlen zu (`emotionen3.py`), ersetze die Platzhalter und prüfe den Bogen mit allen 11 Gefühlen.

## Optional für später
- **Baby- und Senior-Gesichter** (größere Augen beim Baby, Brauen und Falten beim Senior). Das brauchen wir nur, wenn die Standardgesichter bei den Altersstufen nicht passen.
