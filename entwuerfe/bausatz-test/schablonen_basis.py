#!/usr/bin/env python3
"""Schablonen für Körper, Kopf und Halskrause (3/4-Basis, Leinwand 112 x 112)."""
from PIL import Image, ImageDraw
import os
hier = os.path.dirname(os.path.abspath(__file__))
Z = 8

def schablone(name, gw, gh, anker, text):
    im = Image.new('RGB', (gw * Z, gh * Z), 'white'); d = ImageDraw.Draw(im)
    for x in range(gw + 1):
        d.line([(x * Z, 0), (x * Z, gh * Z)], fill=(236, 236, 236) if x % 4 else (214, 214, 214))
    for y in range(gh + 1):
        d.line([(0, y * Z), (gw * Z, y * Z)], fill=(236, 236, 236) if y % 4 else (214, 214, 214))
    for (ax, ay), label, col in anker:
        d.line([(ax * Z - 12, ay * Z), (ax * Z + 12, ay * Z)], fill=col, width=3)
        d.line([(ax * Z, ay * Z - 12), (ax * Z, ay * Z + 12)], fill=col, width=3)
        d.text((ax * Z + 8, ay * Z + 6), label, fill=col)
    d.rectangle([0, 0, gw * Z - 1, gh * Z - 1], outline=(220, 0, 0))
    d.text((6, 4), text, fill=(220, 0, 0))
    im.save(os.path.join(hier, 'ki', name))

ROT, BLAU, GRUEN = (220, 0, 0), (0, 90, 220), (0, 150, 60)
schablone('schablone-koerper.png', 80, 48, [((40, 3), 'Hals', ROT), ((66, 26), 'Schwanz', BLAU)], 'Koerper 80x48')
schablone('schablone-kopf.png', 60, 44, [((30, 40), 'Hals', ROT), ((13, 8), 'Ohr links', BLAU), ((47, 8), 'Ohr rechts', BLAU), ((30, 24), 'Gesicht', GRUEN)], 'Kopf 60x44')
schablone('schablone-kruse.png', 56, 20, [((28, 2), 'Hals', ROT)], 'Halskrause 56x20')
# Pose-Körper (27.11.): Größe (Raster), Hals-Ansatz, Schwanz-Ansatz
POSEN_KOERPER = {
    'laufen':    ((88, 48), (22, 8),  (76, 14)),
    'huepfen':   ((80, 52), (30, 8),  (68, 18)),
    'geduckt':   ((88, 44), (14, 22), (78, 8)),
    'eingerollt': ((88, 40), (24, 10), (66, 18)),
}
for n, ((gw, gh), hals, schwanz) in POSEN_KOERPER.items():
    schablone(f'schablone-koerper-{n}.png', gw, gh, [(hals, 'Hals', ROT), (schwanz, 'Schwanz', BLAU)], f'Koerper {n} {gw}x{gh}')
# Weitere Körper- und Kopfformen (27.11.): Variation der Silhouette
FORMEN_KOERPER = {  # Name: (Raster, Hals, Schwanz)
    'rundlich': ((72, 40), (36, 3), (60, 22)),
    'schlank':  ((76, 56), (36, 3), (62, 34)),
    'kraeftig': ((84, 50), (42, 3), (70, 28)),
}
for n, ((gw, gh), hals, schwanz) in FORMEN_KOERPER.items():
    schablone(f'schablone-koerper-{n}.png', gw, gh, [(hals, 'Hals', ROT), (schwanz, 'Schwanz', BLAU)], f'Koerper {n} {gw}x{gh}')
FORMEN_KOPF = {  # Name: (Raster, Hals, Ohr links, Ohr rechts, Gesicht)
    'klein-rund': ((52, 40), (26, 36), (11, 7), (41, 7), (26, 22)),
    'breit':      ((68, 44), (34, 40), (15, 8), (53, 8), (34, 24)),
}
for n, ((gw, gh), hals, ol, orr, g) in FORMEN_KOPF.items():
    schablone(f'schablone-kopf-{n}.png', gw, gh, [(hals, 'Hals', ROT), (ol, 'Ohr links', BLAU), (orr, 'Ohr rechts', BLAU), (g, 'Gesicht', GRUEN)], f'Kopf {n} {gw}x{gh}')
print('fertig')
