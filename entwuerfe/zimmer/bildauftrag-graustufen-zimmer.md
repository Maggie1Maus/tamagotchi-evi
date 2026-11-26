# Bildauftrag: Zimmer in Graustufen

## So benutzen

Diese drei Bilder anhängen, **in dieser Reihenfolge**:

1. **Bild 1** – `entwuerfe/zimmer/grillme-2026-11-25/v2/bestaetigte-vorschau.png`
   → Das bestätigte Zimmer-Layout. Bindend für Aufbau, Kamera, Möbelanordnung und Proportionen.
2. **Bild 2** – Das Ergebnis des Graustufen-Evi-Auftrags (das Bild, das du gerade erhalten hast)
   → Bindend für den Graustufenstil und die Rendering-Sprache.
3. **Bild 3** – `entwuerfe/zimmer/vorschlag-bibliothek.jpg`
   → Nur als Stimmungsreferenz für Tiefenwirkung, Schattierung und Fensterhimmel. Kein Layout davon übernehmen.

Den englischen Auftrag unten vollständig einfügen.

**Falls Korrektur nötig** (nur eine einzige Folgenachricht senden):
> "Keep the exact room layout of Image 1. Only adjust shading — more grayscale depth, same furniture positions, same camera angle."

---

## Der Auftrag

```text
Use case: stylized-concept. Asset type: room scene design exploration for a black-and-white virtual pet game — grayscale shading version, preview only, no code changes.

OVERVIEW:
Redraw the room scene from Image 1 (the APPROVED ROOM LAYOUT) using the grayscale shading style shown in Image 2 (the GRAYSCALE EVI STYLE REFERENCE). The room layout, camera angle, furniture positions and proportions from Image 1 are binding. The rendering language — full grayscale range, smooth gray tones, soft shadows, depth through shading — comes from Image 2. Image 3 is used only as a mood and atmosphere reference for the window sky and depth shading; its layout is not to be used.

Do NOT place Evi in this scene. The front zone (rug area) stays empty.

ROOM LAYOUT (from Image 1 — do not change):
- Front view onto the back wall, no side walls visible
- Wall takes up roughly 70%, floor roughly 30% of the height
- Back zone: window on the left side of the wall; sideboard on the right; one empty shelf on the wall above the sideboard
- Middle zone: pet bed on the left, pulled visibly forward from the back wall
- Front zone: oval rug, centered, empty — no furniture, no character here
- These four objects only: pet bed, sideboard, empty wall shelf, oval rug. No other decoration.

GRAYSCALE RENDERING (apply this to everything):
Use the full range from pure black to pure white with smooth gray tones in between, as shown in Image 2. No strict 1-bit rendering. No color.

Wall and floor:
- Wall: soft light gray, slightly darker near the edges and corners to suggest enclosed space
- Floor: slightly warmer/darker light gray than the wall, with a faint grain line or two to distinguish materials
- Wall-floor junction: clean dark gray line
- Ceiling line: soft dark gray, not harsh black

Furniture shading:
- Sideboard: mid-gray body with darker gray top surface and a subtle drop shadow on the wall and floor behind and underneath it; slightly lighter front face; simple wood-grain suggestion with a few horizontal gray strokes
- Wall shelf: light gray plank with a soft gray shadow cast downward on the wall beneath it; brackets in slightly darker gray
- Pet bed: soft rounded form, nearly white cushion interior with light gray shadow curving along the inside edges; outer rim in mid-gray; a few faint pillow wrinkle lines in light gray
- Oval rug: light gray center, slightly darker oval border; faint radiating texture lines in very light gray; soft drop shadow underneath

Window:
- Wooden frame: mid-gray with slightly darker edges and a subtle wood-grain; lighter face toward viewer, darker side edges for depth
- Window cross / divider: same mid-gray with thin dark shadow on underside
- Glass panes: rendered as very faint light-gray tint — slightly lighter than the wall — with a subtle diagonal glare highlight (thin near-white diagonal strip) across the upper pane

Window outside view — AUTUMN, DAY, LIGHT CLOUDS (use Image 3 as atmosphere reference):
This is the most important area for depth and mood. Use shading freely here.
- Sky: soft gradient from light gray at the top to nearly white near the horizon; not pure white, not flat
- Clouds: 2-3 simple rounded cloud shapes in near-white with soft mid-gray shadows on their underside; slightly darker gray edges
- Tree: a simple autumn tree silhouette — trunk in mid-dark gray, branches in dark gray becoming thinner; sparse remaining leaves as small rounded shapes in mid-gray to light gray, a few falling leaves in lighter gray
- Ground / horizon hint: very subtle light-gray hill or horizon line at the bottom of the window, fading into the sky

Depth and shadows (overall scene):
- Objects further back (wall, shelf, window frame) get slightly lighter values than objects in front (pet bed, rug)
- Each object casts a soft gray drop shadow on the surface behind or below it
- The room as a whole should feel warm and cozy through soft contrasts — not harsh, not flat

STYLE CONSTRAINTS:
- Strictly grayscale — no color whatsoever, not even a warm tint
- Pixel art / retro handheld aesthetic (think Game Boy Advance or early DS style) — stepped pixel outlines, not smooth ink lines
- Same visual language as Image 2 (the grayscale Evi): consistent outline weight, soft stippling for texture, smooth fill grays
- No text, no UI elements, no watermark, no frame, no signature
- No Evi, no other characters or animals

FORMAT:
Landscape, approximately 1.84:1 ratio (same as Image 1). Must stay clearly readable when scaled down to 320×174 pixels.
```
