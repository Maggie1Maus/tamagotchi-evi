#!/usr/bin/env python3
"""Exportiert die 51 Fensteransichten aus den echten PC-Bildschirmfotos."""
from pathlib import Path
import sys
from PIL import Image, ImageChops, ImageDraw, ImageFont
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
                neu = bild.convert("RGB")
                ziel = aus / f"{zeit}-{wetter}.png"
                if ziel.exists():
                    with Image.open(ziel) as alt:
                        if not ImageChops.difference(alt.convert("RGB"), neu).getbbox():
                            continue
                neu.save(ziel)
print("51 Fensteransichten exportiert.")

# Übersichten direkt aus den geprüften Firmware-Bildschirmfotos.
font = ImageFont.truetype("DejaVuSans.ttf", 22)
klein = ImageFont.truetype("DejaVuSans.ttf", 17)
def foto(j, h, w):
    with Image.open(fotos / f"grau-fenster-{j}-{h}-{w}-0.pgm") as im:
        return im.convert("RGB")
tafel = Image.new("RGB", (1052, 1408), "#fafafa")
d = ImageDraw.Draw(tafel)
d.text((16, 12), "Evis Fenster · Jahreszeiten und Tageszeiten", font=font, fill="#222222")
for h, name in enumerate(("Tag", "Dämmerung", "Nacht")):
    d.text((16 + h * 344, 50), name, font=klein, fill="#333333")
for j, name in enumerate(("Frühling", "Sommer", "Herbst", "Winter")):
    y = 86 + j * 326
    d.text((16, y), name, font=klein, fill="#333333")
    for h in range(3):
        tafel.paste(foto(j, h, 0).resize((332, 284), Image.Resampling.NEAREST), (16 + h * 344, y + 25))
tafel.save(dest.parent / "fenster-jahreszeiten-tageszeiten.png")
tafel = Image.new("RGB", (1324, 816), "#fafafa")
d = ImageDraw.Draw(tafel)
d.text((16, 12), "Winterfenster · Wetter bei Tag, Dämmerung und Nacht", font=font, fill="#222222")
for w, name in enumerate(("Sonne", "Sonne und Wolken", "Regen", "Bewölkt", "Schneefall")):
    d.text((16 + w * 260, 50), name, font=klein, fill="#333333")
for h in range(3):
    for w in range(5):
        tafel.paste(foto(3, h, w).resize((249, 213), Image.Resampling.NEAREST), (16 + w * 260, 84 + h * 234))
tafel.save(dest.parent / "fenster-winter-wetter.png")
