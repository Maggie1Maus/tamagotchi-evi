# Bildaufträge Paket 1 (für die Bild-KI)

Stand 27. November 2026. Gleiche Arbeitsweise wie beim Probelauf (Schritt B in [PROMPT-bauteile.md](PROMPT-bauteile.md)). Die Bild-KI bearbeitet nur, Claude macht danach Raster, Schutzmaske, Ebenen und Prüfbogen.

## Anhängen
- **Basis ohne Ohren und Schwanz:** `baukasten/werkzeug/basis-weiblich-ki.png` bzw. `basis-maennlich-ki.png` (gibt es schon, kein neues B1 nötig).
- **Formvorlage:** `baukasten/werkzeug/<figur>-8x.png` (Mia, Glöckchen, Lotti, Klecksi, Funke; Parkbild erwachsen, glücklich).
- Ergebnisse als PNG **in derselben Größe wie die Basis** unter `baukasten/werkzeug/ki/` ablegen, Namen: `<weiblich|maennlich>-ohren-<figur>.png`, `<weiblich|maennlich>-schwanz-<figur>.png`, `<weiblich|maennlich>-zubehoer-halsband.png`.

Jeder Auftrag gilt für **beide Geschlechter** (zweimal ausführen, jeweils mit der passenden Basis). Vorlage-Text für Ohren (B2) und Schwanz (B3):

```text
Edit the first attached image (a pixel-art character without [ears | a tail]). Add [ears | a tail] ONLY - nothing else may change.
The [ears | tail] must have the SHAPE of the [ears | tail] of the second attached character ([BESCHREIBUNG]), but be drawn in the EXACT STYLE of the first image: same dark-gray stepped pixel outline thickness, same light-gray fur with soft gray dots, 16 gray levels only.
Place them where the original [ears | tail] were attached, symmetrical (ears), in proportion. Same canvas size and position, white background, no color, no text.
```

## [BESCHREIBUNG] je Figur

| Figur | Ohren | Schwanz |
|---|---|---|
| **Mia** | pointed upright cat ears, dark-gray inner ear | fluffy cat tail curling upward, wide at the base, light fur |
| **Glöckchen** | long floppy dog ears hanging down the sides of the head with soft gray fur spots; they may fall over the cheeks but must NEVER cover eyes or mouth | short curled dog tail |
| **Lotti** | large broad deer ears, dark-gray inner ear, slightly angled outward | short upright stub tail with a white tip |
| **Klecksi** | floppy dalmatian ears hanging down; ONE ear dark gray, ONE ear light (copy exactly like the second image) | thin straight dalmatian tail |
| **Funke** | short pointed ears with small fur tufts (brushes) at the tips | big bushy squirrel tail in a zigzag lightning shape with a light tip; may stick out behind the body but must not cover the paws |

Hinweise zu den Parkbildern (am 27.11. geprüft): Mia trägt eine Schleife auf dem Kopf (die gibt es schon als Zubehör, nicht mit zeichnen). Klecksis Parkbild zeigt ein dunkles Ohr oben links, ein dunkler Fleck um ein Auge, ein Halsband mit Marke und runde Flecken am Körper. Funke hat sternförmige Blitze auf den Wangen und eine gezackte helle Schwanzspitze.

## Neue Zusatzteile (Ebene allein, ohne Figur ändern)

**Halsband (Klecksi, Zubehör `halsband`)** - auf Basis anhängen:
```text
Edit the attached pixel-art sprite. Add ONLY a narrow dark-gray collar around the neck, lying above the neck ruff, with a small round tag in the center front. Same outline thickness and 16 gray levels, white background, nothing else may change, same canvas size and position.
```

**Backenblitze (Funke, Besonderheit `backenblitze`)**:
```text
Edit the attached pixel-art sprite. Add ONLY one small lightning-bolt mark on each cheek (like the second attached character's cheek marks), dark-gray, lying on the cheeks. Eyes and mouth must stay untouched. Same style, 16 gray levels, white background, same canvas size and position.
```

**Muster `kitz-tupfen`, `augenfleck`, angepasste `flecken`** werden von Claude als Masken über die Bereichskarten gerechnet (Tupfen: hell, nur auf `fell`; Augenfleck: dunkel um ein Auge, Augenlinien bleiben). Dafür ist keine Bild-KI nötig.

---

## Zweite Runde Ohren (27. November 2026, nach der ersten Sichtung)

Probleme in der ersten Runde: Ohren sitzen nicht richtig am Kopf, die Kanten sind unsauber (pixelig), Funkes Ohren sind nur kleine Büschel, Klecksis rechtes Ohr hängt lose neben dem Kopf. Die Aufträge unten ersetzen B2 für **Funke, Klecksi, Glöckchen, Lotti**. Wieder je Geschlecht, gleiche Dateinamen wie vorher (alte Dateien überschreiben).

Zusätzlich an **jeden** Auftrag anhängen:
```text
IMPORTANT: Work on a clean pixel grid. Every visible pixel step must be exactly one pixel of the original sprite (the image is an 8x nearest-neighbour enlargement). No anti-aliasing, no blurry or half-tone edge pixels, no stray single pixels. The outline of the ear must be ONE continuous dark-gray stepped line of the same thickness as the head outline. The base of each ear must OVERLAP the head outline and merge into it with no gap, so the ear clearly grows out of the head. Do not touch anything else.
```

**Funke** (Ohren deutlich größer):
```text
Add two clearly visible, upright, pointed squirrel ears on top of the head, about one fifth of the head height tall and clearly taller than the forehead tufts, each with a small brush tuft of fur at the tip. Dark-gray inner ear. They sit at the upper left and upper right of the head and grow out of the head outline.
```
**Klecksi** (beide Ohren am Kopf, ein Ohr dunkel, eins hell):
```text
Add two floppy dalmatian ears that hang from the upper sides of the head: the LEFT ear (viewer's left) solid dark gray, the RIGHT ear light with a few dark spots. Both are attached to the head outline at the top corner and hang down along the side of the head, touching the head, never floating beside it. The dark ear must be one even dark shape without lighter squares inside.
```
**Glöckchen**:
```text
Add two long floppy dog ears that start at the top corners of the head, overlap the head outline, and hang down along the sides of the head with soft gray fur dots. They must never cover eyes or mouth.
```
**Lotti**:
```text
Add two large broad deer ears that grow out of the upper sides of the head (overlapping the head outline), slightly angled outward, dark-gray inner ear, one clean continuous outline.
```
