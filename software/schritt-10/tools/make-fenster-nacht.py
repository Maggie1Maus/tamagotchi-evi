#!/usr/bin/env python3
"""Erzeugt gedämpfte Nachtpaletten aus den vorhandenen Tagesbäumen."""
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[3]
EBENEN = ROOT / "entwuerfe/zimmer/fenster-hell-2026-11-26/ebenen"

for saison in ("fruehling", "sommer", "herbst", "winter"):
    with Image.open(EBENEN / f"baum-{saison}.png") as bild:
        rgba = bild.convert("RGBA")
        # Keine Kantenaufhellung: dieselbe matte Nachtpalette für die gesamte Form.
        g = rgba.convert("L").point(
            lambda v: 85 if saison == "winter" and v >= 238
            else min(51, ((v + 42) // 85) * 17))
        alpha = rgba.getchannel("A")
        Image.merge("RGBA", (g, g, g, alpha)).save(EBENEN / f"baum-{saison}-dunkel.png")
print("Vier Nachtbäume ohne leuchtende Kontur erzeugt.")
