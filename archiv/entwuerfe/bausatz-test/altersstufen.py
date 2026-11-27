#!/usr/bin/env python3
"""Dasselbe Tier in vier Lebensphasen. -> pruefbogen-altersstufen.png"""
from PIL import Image, ImageDraw
from formen import baue_form
from basis3 import W, H, HIER

STUFEN = [('Baby', 'baby', 'baby'), ('Teenager', 'teenager', 'teenager'), ('Erwachsen', 'sitzend', '3-4'), ('Senior', 'senior', 'senior')]
TIERE = [('spitz', 'buschig', 'freundlich', 187, None), ('haengend', 'puschel', 'lachend', 238, 'flecken'), ('lang', 'duenn', 'freundlich', 153, None)]
if __name__ == '__main__':
    z = 3
    out = Image.new('RGB', (len(STUFEN) * W * z, len(TIERE) * (H * z + 14)), 'white'); d = ImageDraw.Draw(out)
    for j, (o, s, g, f, m) in enumerate(TIERE):
        for i, (name, k, h) in enumerate(STUFEN):
            c = baue_form(k, h, ohren=o, schwanz=s, gesicht=g, fell=f, muster=m)
            x, y = i * W * z, j * (H * z + 14)
            d.text((x + 4, y + 1), name, fill='black'); out.paste(c.bild(255, z), (x, y + 14))
    out.save(HIER / 'pruefbogen-altersstufen.png'); print(out.size)
