# Bildauftrag: Zimmer nach der Grillme-Session (25.11.2026)

Ziel: eine saubere Fassung des bestätigten Zimmerentwurfs, mit allen Beschlüssen der Grillme-Session und mit dem Fensterausblick „Herbst · Dämmerung · Regen“, diesmal **ohne jede Grauabstufung**.

## So benutzen

1. Diese drei Bilder anhängen, in dieser Reihenfolge:
   - Bild 1: „Evi im gemütlichen Pixelzimmer.png“ (bestätigte Zimmergrundlage, ChatGPT-Bibliothek)
   - Bild 2: `entwuerfe/evi/evi-aktuell-referenz.png` (Evi)
   - Bild 3: „Evi am verregneten Herbstfenster.png“ (nur für das Motiv, die Grautöne sind abgelehnt)
2. Den Auftrag unten vollständig einfügen.
3. Wenn das Ergebnis trotzdem graue Kanten hat: Claude kann das Bild hinterher auf reines Schwarz-Weiß umstellen.

Der Auftrag ist auf Englisch, weil die Bild-KI englische Angaben genauer befolgt (wie bei den bisherigen Bildaufträgen).

## Der Auftrag

```text
Use case: precise-object-edit.

Input images:
- Image 1 = APPROVED ROOM. Binding base for layout, camera, room proportions, cozy mood and contour style.
- Image 2 = EVI REFERENCE. The only valid look of the character.
- Image 3 = WINDOW MOTIF PREVIEW. Use ONLY its subject (autumn branches, few leaves, rain at dusk). Its gray tones are REJECTED and must not be copied.

Goal: a clean, final-quality version of Image 1 for a small monochrome pet game, with the window showing AUTUMN, DUSK and RAIN in strict black and white.

COLOR (most important rule):
Pure 1-bit image. Every single pixel is either solid black or solid white. No gray at all: no gray fills, no gradients, no soft shadows, no anti-aliasing, no blurred or smoothed edges, no paper texture, no color. Darker areas (dusk sky, rain sky, small shadows) may ONLY be shown by many small, fully black dots on white, in a regular dot pattern. More dots = darker. Every dot is solid black.

STYLE:
Retro-cozy pixel art. Fine black pixel contours with the SAME line weight everywhere: room, window, furniture and Evi share one consistent visual language. Hard pixel edges. Less detail than Image 1: calm, soft, cozy shapes with only sparse material hints (a few wood-grain lines, a simple woven edge on the rug). Furniture may show a little more detail than walls and floor, but nothing busy. Quiet, peaceful atmosphere.

ROOM:
Straight front view onto the back wall, NO side walls. Wall roughly 70 %, floor roughly 30 % of the height (only a guide, choose what looks and plays best). Wall, floor and window are the fixed room structure. Keep the camera, room size and furniture arrangement of Image 1.
Three depth zones, NOT visibly marked:
- Back, against the wall: window on the left; sideboard on the right; one EMPTY wall shelf above the sideboard.
- Middle: pet bed on the left, clearly pulled forward from the wall, its front easy to reach.
- Front: oval rug and Evi. This zone stays open and free as play and walking space; no furniture here.
Only these four start objects: pet bed, sideboard, empty shelf, rug. The sideboard top is empty. The shelf is empty. NO books, plants, candles, vases, pictures, lamps, bowls, toys, curtains or any other decoration. No Christmas decoration.

EVI:
Copy Evi exactly from Image 2: same body shape, ears, face, eyes, dotted tail and pixel contour. Only scale her down. Do not redesign, restyle, smooth or add details, and do not invent a new similar-looking character. She sits on the rug in the front zone, slightly off center, facing the viewer, relaxed and content, about 30 % of the image height. She is the largest figure because she is closest to the viewer. Evi and the room are equally important; neither dominates.

WINDOW (AUTUMN, DUSK, RAIN):
Same wooden window frame and panes as in Image 1, all panes clear. Outside: a few simple autumn branches with only a few remaining leaves, one or two leaves falling. Dusk: the sky is a medium dot pattern, darker than day and lighter than night. Rain darkens it further: the dots get denser towards the top of the window. Rain as short, thin, slanted black dashes, each with a thin white gap around it so it stays readable against the dots. No moon, no stars, because the sky is clouded. Keep the view simple: it should bring life to the room without overloading it.

FORMAT:
Landscape, about 1.84 : 1, like Image 1. Must stay clearly readable when scaled down to 320 x 174 pixels. No people, no other animals, no user interface, no text, no frame, no signature.
```

## Abwandlungen

**Zimmer ohne Evi** (für die eigentliche Spielgrafik; das Spiel setzt Evi aus ihren Zeichnungen ein): den Abschnitt `EVI:` ersetzen durch

```text
NO EVI: Do not draw Evi or any other character. Keep the front zone and the rug completely free.
```

**Anderer Fensterausblick:** den Abschnitt `WINDOW` ersetzen und je einen Baustein aus Jahreszeit, Tageszeit und Wetter wählen. Die Regeln aus `COLOR` gelten immer.

```text
WINDOW: Same wooden window frame and panes as in Image 1, all panes clear. Keep the view simple. Outside:
```

| Wahl | Baustein |
| --- | --- |
| Frühling | `Thin branches with fresh small leaves and buds.` |
| Sommer | `Leafy branches with a few simple natural shapes.` |
| Herbst | `A few branches with only a few remaining leaves, one or two leaves falling.` |
| Winter | `Bare branches.` |
| Tag | `Day: white sky.` |
| Dämmerung | `Dusk: sky as a medium dot pattern, darker than day, lighter than night.` |
| Nacht | `Night: sky as a dense dot pattern.` |
| Sonne / klar | `Clear sky: by day a small simple outlined sun, at night a small moon and a few white stars left free in the dots.` |
| Bewölkt | `Cloudy: a few simple outlined clouds, no sun, moon or stars.` |
| Regen | `Rain: the sky is darker (denser dots, also by day), with short thin slanted black dashes, each with a thin white gap around it. No sun, moon or stars.` |
| Schnee (nur Winter) | `Snowfall: small snowflakes, on a white sky as tiny black outlined flakes, on a dotted sky as tiny white flakes left free of dots. No sun, moon or stars.` |
