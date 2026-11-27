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
