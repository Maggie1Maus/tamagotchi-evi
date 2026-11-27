#!/usr/bin/env python3
"""Dasselbe Tier in vier Lebensphasen, für alle Körper- und Kopfformen. -> pruefbogen-altersformen.png"""
from PIL import Image, ImageDraw
from formen import baue_form
from basis3 import W, H, HIER

ALTER = ['baby', 'teenager', 'erwachsen', 'senior']

def koerper_key(form, alter):
    if form == 'sitzend':
        return {'baby': 'baby', 'teenager': 'teenager', 'erwachsen': 'sitzend', 'senior': 'senior'}[alter]
    return form if alter == 'erwachsen' else f'{form}-{alter}'

def kopf_key(form, alter):
    if form == '3-4':
        return {'baby': 'baby', 'teenager': 'teenager', 'erwachsen': '3-4', 'senior': 'senior'}[alter]
    return form if alter == 'erwachsen' else f'{form}-{alter}'

PAARE = [('sitzend', '3-4'), ('rundlich', 'klein-rund'), ('rundlich', 'breit'), ('schlank', 'klein-rund'), ('schlank', 'breit'), ('kraeftig', 'breit')]
if __name__ == '__main__':
    z = 3
    out = Image.new('RGB', (4 * W * z, len(PAARE) * (H * z + 14)), 'white'); d = ImageDraw.Draw(out)
    for j, (kf, hf) in enumerate(PAARE):
        for i, a in enumerate(ALTER):
            c = baue_form(koerper_key(kf, a), kopf_key(hf, a), ohren='rund' if j % 2 else 'spitz', schwanz='duenn' if j % 2 else 'buschig', gesicht='freundlich', fell=204)
            x, y = i * W * z, j * (H * z + 14)
            d.text((x + 4, y + 1), f'{kf} / {hf}: {a}', fill='black'); out.paste(c.bild(255, z), (x, y + 14))
    out.save(HIER / 'pruefbogen-altersformen.png'); print(out.size)
