#!/usr/bin/env python3
"""Kleine Aufsätze: Mia, Glöckchen, Klecksi, Mampf, Groschen, Funke. -> pruefbogen-aufsaetze.png"""
from PIL import Image, ImageDraw
from formen import baue_form
from basis3 import W, H, HIER

F = [  # Name, Körper, Kopf, Ohren, Schwanz, Gesicht, Fell, Muster, Aufsätze
 ('Mia: Schleife', 'sitzend', '3-4', 'spitz', 'duenn', 'freundlich', 238, None, ('schleife',)),
 ('Glöckchen: Glöckchen', 'rundlich', '3-4', 'haengend', 'puschel', 'lachend', 238, None, ('gloeckchen',)),
 ('Klecksi: Halsband', 'schlank', 'breit', 'haengend', 'duenn', 'lachend', 255, 'flecken', ('halsband',)),
 ('Groschen: Augenmaske, Ringelschwanz', 'rundlich', 'breit', 'rund', 'geringelt', 'freundlich', 153, None, ('augenmaske',)),
 ('Funke: Backenblitze', 'schlank', '3-4', 'buschel', 'buschig', 'lachend', 204, None, ('backenblitz',)),
]
if __name__ == '__main__':
    z, cols = 3, 3
    out = Image.new('RGB', (cols * W * z, 2 * (H * z + 14)), 'white'); d = ImageDraw.Draw(out)
    for i, (n, k, h, o, s, g, f, m, a) in enumerate(F):
        c = baue_form(k, h, ohren=o, schwanz=s, gesicht=g, fell=f, muster=m, aufsaetze=a)
        x, y = (i % cols) * W * z, (i // cols) * (H * z + 14)
        d.text((x + 4, y + 1), n, fill='black'); out.paste(c.bild(255, z), (x, y + 14))
    out.save(HIER / 'pruefbogen-aufsaetze.png'); print(out.size)
