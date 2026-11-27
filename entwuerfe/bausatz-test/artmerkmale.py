#!/usr/bin/env python3
"""Artmerkmale: Leo, Rolli, Tetro, Lumi, Blubber, Bit, Blip, Ella mit ihren Merkmalen. -> pruefbogen-artmerkmale.png"""
from PIL import Image, ImageDraw
from formen import baue_form
from basis3 import W, H, HIER

F = [  # Name, Körper, Kopf, Ohren, Schwanz, Gesicht, Fell, Kranz, Merkmale
 ('Leo', 'kraeftig', 'breit', 'rund', 'duenn', 'freundlich', 204, 136, ('maehne',)),
 ('Rolli', 'rundlich', '3-4', None, 'puschel', 'freundlich', 136, 153, ('stacheln',)),
 ('Tetro', 'kraeftig', '3-4', None, 'duenn', 'muede', 119, 136, ('panzer',)),
 ('Lumi', 'schlank', 'klein-rund', 'buschel', 'puschel', 'lachend', 221, 255, ('fluegel-fee',)),
 ('Blubber', 'kraeftig', 'klein-rund', 'rund', 'blitz', 'wuetend', 170, 119, ('fluegel-drache',)),
 ('Bit', 'kraeftig', 'breit', 'antenne', 'blitz', 'freundlich', 119, 170, ()),
 ('Blip', 'sitzend', 'klein-rund', 'antenne', 'duenn', 'ueberrascht', 204, 238, ()),
 ('Ella', 'kraeftig', 'breit', 'haengend', 'duenn', 'freundlich', 187, 221, ('ruessel',)),
]
if __name__ == '__main__':
    z, cols = 3, 4
    out = Image.new('RGB', (cols * W * z, 2 * (H * z + 14)), 'white'); d = ImageDraw.Draw(out)
    for i, (n, k, h, o, s, g, f, kz, m) in enumerate(F):
        c = baue_form(k, h, 2, ohren=o, schwanz=s, gesicht=g, fell=f, kranz=kz, merkmale=m)
        x, y = (i % cols) * W * z, (i // cols) * (H * z + 14)
        d.text((x + 4, y + 1), f'{n}: {", ".join(m) or ("Antennen" if o == "antenne" else "")}', fill='black')
        out.paste(c.bild(255, z), (x, y + 14))
    out.save(HIER / 'pruefbogen-artmerkmale.png'); print(out.size)
