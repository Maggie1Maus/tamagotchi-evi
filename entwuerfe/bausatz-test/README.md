# Bausatz-Test (27. November 2026)

Kleiner Versuch, ob Ohren, Schwänze und Fell **per Skript** statt per Bild-KI entstehen können. Nur ein Test, nichts davon ist im Spiel.

- `bausatz.py` erzeugt `pruefbogen-test.png` (`python3 bausatz.py`).
- **Runde Basis** (64 × 64, 16 Graustufen), nicht Evi: Kopf, Körper, Füße, Gesicht, Bauchfleck.
- **Teile sind nur Formen.** Füllung, Schattierung und der 1-Pixel-Umriss entstehen automatisch, deshalb passen alle zusammen. Ansatzpunkte stehen in `ANKER`.
- 5 Ohren (spitz, rund, lang, hängend, Büschel), 4 Schwänze (Puschel, buschig mit heller Spitze, dünn, Blitz), Fellton, Bauchton, Muster (Flecken, Streifen).
- Zeichnung und Gesicht der Basis sind eigene Formen, keine Evi-Pixel.

## Version 2 (größer, mit Details)
`bausatz2.py` → `pruefbogen-test2.png`: 96 × 96 Pixel, Fellpunkte, gerasterte Verläufe, flauschiger Rand, Augen mit Glanzpunkten, Mund mit Zunge, Schnurrhaare, Bauchfell, Zehen, Ohrinnenfell.
`schablonen.py` erzeugt die Schablonen für die Bild-KI. Prompt: [PROMPT-ki-teile.md](PROMPT-ki-teile.md).
