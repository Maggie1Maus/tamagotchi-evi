#!/usr/bin/env python3
"""Skaliert die Ausgangsentwürfe ins Displayformat, ohne neue Motive zu zeichnen."""
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[3]
DEST = ROOT / "entwuerfe/park/entwurf-2"

for saison in ("fruehling", "sommer", "herbst", "winter"):
    with Image.open(DEST / "quellen" / f"{saison}.png") as src:
        # Nur Größen-/Formatanpassung: 16 feste Graustufen ohne Farbstich/Dithering.
        bild = src.convert("L").resize((320, 174), Image.Resampling.LANCZOS)
        bild = bild.point(lambda g: min(255, (g + 8) // 17 * 17)).convert("RGBA")
        bild.save(DEST / f"park-{saison}.png")

# Freigestellte Zeichnungen nur zuschneiden, skalieren und ins Displayformat bringen.
for name, hoehe in (("laterne", 96), ("stern", 24)):
    with Image.open(DEST / "quellen" / f"{name}.png") as src:
        rgba = src.convert("RGBA")
        rgba = rgba.crop(rgba.getchannel("A").getbbox())
        breite = max(1, round(rgba.width * hoehe / rgba.height))
        rgba = rgba.resize((breite, hoehe), Image.Resampling.LANCZOS)
        g = rgba.convert("L").point(lambda v: min(255, (v + 8) // 17 * 17))
        a = rgba.getchannel("A").point(lambda v: 255 if v >= 96 else 0)
        bild = Image.merge("RGBA", (g, g, g, a))
        bild.save(DEST / f"{name}.png")
