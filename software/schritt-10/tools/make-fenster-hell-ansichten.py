#!/usr/bin/env python3
"""Exportiert die 51 Fensteransichten aus den echten PC-Bildschirmfotos."""
from pathlib import Path
import sys
from PIL import Image
root = Path(__file__).resolve().parents[3]
dest = root / "entwuerfe/zimmer/fenster-hell-2026-11-26/ansichten"
fotos = Path(sys.argv[1])
for j, saison in enumerate(("fruehling", "sommer", "herbst", "winter")):
    aus = dest / saison
    aus.mkdir(parents=True, exist_ok=True)
    for h, zeit in enumerate(("tag", "daemmerung", "nacht")):
        for w, wetter in enumerate(("klar", "leicht-bewoelkt", "regen", "bedeckt", "schneefall")):
            if w == 4 and j != 3:
                continue
            with Image.open(fotos / f"grau-fenster-{j}-{h}-{w}-0.pgm") as bild:
                bild.convert("RGB").save(aus / f"{zeit}-{wetter}.png")
print("51 Fensteransichten exportiert.")
