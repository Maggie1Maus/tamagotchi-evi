#!/usr/bin/env python3
"""Setzt die 3/4-Basis (Körper, Kopf, Halskrause + Ohren, Schwanz, Gesicht) auf einer 112 x 112 Leinwand zusammen.
Rollen: 34 Umriss, 119 innen, 170 Schatten, 204 Fell, 238 Licht, 255 Hintergrund. Fellton verschiebt Fell/Schatten/Licht.
Aufruf: python3 basis3.py  ->  pruefbogen-basis3.png
"""
import os
from pathlib import Path
import numpy as np
from PIL import Image, ImageDraw
import basis3_layout as L

HIER = Path(__file__).resolve().parent
TEILE = HIER / 'ki' / 'teile'
Z = 8
W, H = L.LEINWAND

def lade(art, name, nr):
    p = TEILE / f'{art}-{name}-{nr}.png'
    a = np.array(Image.open(p).convert('RGB'))[::Z, ::Z, 0]
    return a

class Leinwand:
    def __init__(self):
        self.g = np.zeros((H, W), np.int32); self.a = np.zeros((H, W), bool)
    def teil(self, raster, ansatz, ziel, rollen, spiegeln=False, hinter=None):
        ay, ax = ansatz[1], ansatz[0]
        ys, xs = np.nonzero(raster != 255)
        for y, x in zip(ys, xs):
            v = raster[y, x]
            ton = rollen.get(int(v), int(v))
            px, py = x - ax + ziel[0], y - ay + ziel[1]
            if spiegeln: px = W - 1 - px
            if 0 <= px < W and 0 <= py < H:
                self.g[py, px] = ton; self.a[py, px] = True
    def bild(self, hg=255, z=4):
        a = np.full((H, W, 3), hg, np.uint8)
        a[self.a] = np.stack([np.clip(self.g[self.a], 0, 255)] * 3, 1)
        return Image.fromarray(a).resize((W * z, H * z), Image.NEAREST)

def rollen(fell=204, kranz=238):
    d = fell - 204
    return {34: 34, 119: 119 + d // 2, 170: 170 + d, 204: 204 + d, 238: 238 + d}

def baue(koerper=3, kopf=1, kruse=2, ohren=None, schwanz=None, gesicht=None, fell=204, kranz=238):
    from ki_teile import lade_teil
    c = Leinwand(); r = rollen(fell)
    if schwanz:
        t = lade_teil('schwanz', schwanz)
        if t is not None: c.teil(t[0], t[1], L.SCHWANZ_ZIEL, r)
    c.teil(lade('koerper', 'sitzend', koerper), (40, 3), L.KOERPER_ZIEL, r)
    if ohren:
        t = lade_teil('ohr', ohren)
        if t is not None:
            zl, zr = (L.OHR_SEITE_LINKS_ZIEL, L.OHR_SEITE_RECHTS_ZIEL) if ohren == 'haengend' else (L.OHR_LINKS_ZIEL, L.OHR_RECHTS_ZIEL)
            c.teil(t[0], t[1], zl, r)
            c.teil(t[0], t[1], (W - 1 - zr[0], zr[1]), r, spiegeln=True)
    c.teil(lade('kopf', '3-4', kopf), (30, 40), L.KOPF_ZIEL, r)
    c.teil(lade('kruse', 'flauschig', kruse), (28, 2), L.KRUSE_ZIEL, {34: 34, 238: kranz, 204: kranz - 34, 170: kranz - 68})
    if gesicht:
        t = lade_teil('gesicht', gesicht)
        if t is not None:
            gt = {34: 17, 119: 51, 170: 102, 204: 153, 238: 255}
            c.teil(t[0], t[1], L.GESICHT_ZIEL, gt)
    return c

if __name__ == '__main__':
    z = 3
    kombis = [(k, h, 2) for k in (1, 2, 3) for h in (1, 2, 3)]
    out = Image.new('RGB', (3 * W * z, 3 * (H * z + 14)), 'white'); d = ImageDraw.Draw(out)
    for i, (k, h, kr) in enumerate(kombis):
        c = baue(k, h, kr, ohren='spitz', schwanz='buschig', gesicht='freundlich')
        x, y = (i % 3) * W * z, (i // 3) * (H * z + 14)
        d.text((x + 4, y + 1), f'Körper {k}, Kopf {h}', fill='black'); out.paste(c.bild(255, z), (x, y + 14))
    out.save(HIER / 'pruefbogen-basis3.png'); print(out.size)

def bogen_varianten():
    z = 3
    v = [('spitz', 'buschig', 204, 'freundlich'), ('rund', 'puschel', 170, 'lachend'), ('lang', 'duenn', 238, 'ueberrascht'),
         ('haengend', 'puschel', 187, 'traurig'), ('buschel', 'blitz', 221, 'wuetend'), ('spitz', 'duenn', 136, 'muede'),
         ('rund', 'buschig', 255, 'freundlich'), ('lang', 'blitz', 153, 'lachend')]
    kr = [238, 238, 238, 238, 255, 238, 255, 221]
    out = Image.new('RGB', (4 * W * z, 2 * (H * z + 14)), 'white'); d = ImageDraw.Draw(out)
    for i, (o, s, f, g) in enumerate(v):
        c = baue(2, 2, 2, ohren=o, schwanz=s, gesicht=g, fell=f, kranz=kr[i])
        x, y = (i % 4) * W * z, (i // 4) * (H * z + 14)
        d.text((x + 4, y + 1), f'Ohr {o}, Schwanz {s}, Fell {f}', fill='black'); out.paste(c.bild(255, z), (x, y + 14))
    out.save(HIER / 'pruefbogen-basis3-varianten.png'); print(out.size)

if __name__ == '__main__':
    bogen_varianten()
