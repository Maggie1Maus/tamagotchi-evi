#!/usr/bin/env python3
"""Erzeugt die Schablonen und die Stilvorlage für die Bild-KI (siehe PROMPT-ki-teile.md)."""
from PIL import Image, ImageDraw
from ki_teile import RASTER, SCHABLONEN
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
hier = os.path.dirname(os.path.abspath(__file__))
Z = RASTER
def schablone(name, gw, gh, anker, text):
    im = Image.new('RGB', (gw * Z, gh * Z), 'white'); d = ImageDraw.Draw(im)
    for x in range(0, gw + 1):
        d.line([(x * Z, 0), (x * Z, gh * Z)], fill=(236, 236, 236) if x % 4 else (214, 214, 214))
    for y in range(0, gh + 1):
        d.line([(0, y * Z), (gw * Z, y * Z)], fill=(236, 236, 236) if y % 4 else (214, 214, 214))
    ax, ay = anker
    d.line([(ax * Z - 10, ay * Z), (ax * Z + 10, ay * Z)], fill=(220, 0, 0), width=2)
    d.line([(ax * Z, ay * Z - 10), (ax * Z, ay * Z + 10)], fill=(220, 0, 0), width=2)
    d.rectangle([0, 0, gw * Z - 1, gh * Z - 1], outline=(220, 0, 0))
    d.text((6, 4), text, fill=(220, 0, 0))
    im.save(os.path.join(hier, 'ki', name))
# Ohr: 24 x 32 Rasterpixel, Ansatz = unten Mitte; Schwanz: 32 x 40, Ansatz = links Mitte
schablone('schablone-ohr.png', 24, 32, (12, 29), 'Ohr 24x32, roter Punkt = Ansatz')
schablone('schablone-ohr-haengend.png', 24, 32, (12, 2), 'Hängeohr 24x32, roter Punkt = Ansatz')
schablone('schablone-schwanz.png', 32, 40, (3, 29), 'Schwanz 32x40, roter Punkt = Ansatz')
(gw, gh), anker = SCHABLONEN['gesicht']
schablone('schablone-gesicht.png', gw, gh, anker, 'Gesicht 40x28, roter Punkt = Nase')
# Stilvorlage: Ausschnitt der neuen Basis (Kopf mit Ohr, Schwanz) auf 8x
import bausatz2 as b
t = b.tiere('spitz', 'buschig')
im = t.bild(255, Z)
im.crop((12 * Z, 4 * Z, 84 * Z, 60 * Z)).save(os.path.join(hier, 'ki', 'stilvorlage-kopf.png'))
im.crop((50 * Z, 40 * Z, 96 * Z, 96 * Z)).save(os.path.join(hier, 'ki', 'stilvorlage-schwanz.png'))
print('fertig')
