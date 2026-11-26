#!/usr/bin/env python3
"""Ordnet unveränderte Firmware-Bildschirmfotos zu beschrifteten Übersichten."""
import sys
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[3]
DEST = ROOT / "entwuerfe/park/entwurf-2"
FOTOS = Path(sys.argv[1])
SAISON = ("fruehling", "sommer", "herbst", "winter")
HIMMEL = ("tag", "daemmerung", "nacht")
WETTER = ("sonne", "sonne-wolken", "regen", "bewoelkt", "schnee")
FONT = ImageFont.truetype("DejaVuSans.ttf", 22)
KLEIN = ImageFont.truetype("DejaVuSans.ttf", 17)

for s, saison in enumerate(SAISON):
    aus = DEST / "ansichten" / saison
    aus.mkdir(parents=True, exist_ok=True)
    for h, himmel in enumerate(HIMMEL):
        for w, wetter in enumerate(WETTER):
            if w == 4 and s != 3:
                continue
            with Image.open(FOTOS / f"park-{s}-{h}-{w}.pgm") as bild:
                bild.save(aus / f"{himmel}-{wetter}.png")

# 4 Jahreszeiten × 4 häufige Situationen, direkt aus dem Firmware-Zeichencode.
U = Image.new("RGB", (1336, 910), "#fafafa")
d = ImageDraw.Draw(U)
d.text((20, 14), "Evis Park · Jahreszeiten und Beleuchtung", font=FONT, fill="#222222")
spalten = ((0, 0, "Tag · Sonne"), (0, 2, "Tag · Regen"),
           (1, 1, "Dämmerung"), (2, 1, "Nacht"))
for col, (_, _, titel) in enumerate(spalten):
    d.text((16 + col * 332, 55), titel, font=KLEIN, fill="#333333")
for s, saison in enumerate(SAISON):
    y = 88 + s * 202
    d.text((16, y), ("Frühling", "Sommer", "Herbst", "Winter")[s], font=KLEIN, fill="#333333")
    for col, (h, w, _) in enumerate(spalten):
        with Image.open(FOTOS / f"park-{s}-{h}-{w}.pgm") as bild:
            U.paste(bild.convert("RGB"), (16 + col * 332, y + 24))
U.save(DEST / "park-jahreszeiten-beleuchtung.png")

# Winter mit jedem Wetter tagsüber und nachts, damit liegender und fallender
# Schnee sowie die eingeschalteten Regenfenster getrennt sichtbar sind.
U = Image.new("RGB", (1668, 455), "#fafafa")
d = ImageDraw.Draw(U)
d.text((16, 12), "Winter · Wetter am Tag und bei Nacht", font=FONT, fill="#222222")
for w, titel in enumerate(("Sonne", "Sonne und Wolken", "Regen", "Bewölkt", "Schneefall")):
    x = 16 + 332 * w
    d.text((x, 52), titel, font=KLEIN, fill="#333333")
    for row, h in enumerate((0, 2)):
        with Image.open(FOTOS / f"park-3-{h}-{w}.pgm") as bild:
            U.paste(bild.convert("RGB"), (x, 82 + 186 * row))
U.save(DEST / "park-winter-wetter.png")

# Die korrigierten Laternen im Zusammenhang mit dem Park vergrößert ansehen.
U = Image.new("RGB", (1328, 816), "#fafafa")
d = ImageDraw.Draw(U)
d.text((16, 12), "Neue Graustufenlaternen · Sommer und Winter", font=FONT, fill="#222222")
for col, s in enumerate((1, 3)):
    for row, h in enumerate((0, 2)):
        x, y = 16 + col * 656, 58 + row * 378
        titel = ("Sommer" if s == 1 else "Winter mit Weihnachtssternen") + (" · Tag" if h == 0 else " · Nacht")
        d.text((x, y), titel, font=KLEIN, fill="#333333")
        with Image.open(FOTOS / f"park-{s}-{h}-0.pgm") as bild:
            U.paste(bild.convert("RGB").resize((640, 348), Image.Resampling.NEAREST), (x, y + 24))
U.save(DEST / "park-laternen-detail.png")
print("51 Einzelansichten und drei Übersichten gespeichert.")
