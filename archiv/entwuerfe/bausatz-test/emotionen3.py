#!/usr/bin/env python3
"""Gefühle (Gruppe A der Spielposen) auf der 3/4-Basis: Gesicht + Effekt per Skript.
Aufruf: python3 emotionen3.py -> pruefbogen-emotionen-basis3.png
"""
import numpy as np
from PIL import Image, ImageDraw
import basis3
from basis3 import W, H, HIER
from formen import baue_form, geometrie

Y, X = np.mgrid[0:H, 0:W]
def ell(cx, cy, rx, ry): return ((X - cx) / rx) ** 2 + ((Y - cy) / ry) ** 2 <= 1
def rand(m):
    d = m.copy(); d[1:] |= m[:-1]; d[:-1] |= m[1:]; d[:, 1:] |= m[:, :-1]; d[:, :-1] |= m[:, 1:]; return d & ~m
def setze(c, m, v):
    c.g[m] = v; c.a[m] = True
def zeichne(c, m, fuell=238):
    setze(c, rand(m), 34); setze(c, m, fuell)
def tropfen(c, cx, cy, h=6):
    t = np.zeros((H, W), bool)
    for k in range(h): t |= ell(cx, cy - k, 0.5 + 2.2 * (1 - k / h), 0.9)
    t |= ell(cx, cy, 2.6, 2.6)
    zeichne(c, t)

def schweiss(c, g): tropfen(c, g['mx'] + 24, g['my'] - 16)
def traene(c, g): tropfen(c, g['mx'] - 11, g['my'] + 7, 5)
def sabber(c, g):
    x0, y0 = g['mx'] + 6, g['my'] + 6
    m = (X >= x0) & (X <= x0 + 1) & (Y >= y0) & (Y <= y0 + 6) | ell(x0 + 0.5, y0 + 7, 2.2, 2.6)
    zeichne(c, m)
def schmutz(c, g):
    m = np.zeros((H, W), bool)
    for dx, dy, r in [(-18, -10, 3.4), (19, 2, 2.6), (6, -18, 2.2)]: m |= ell(g['mx'] + dx, g['my'] + dy, r, r * .85)
    for cx, cy, r in [(40, 90, 3.6), (58, 98, 3), (72, 84, 2.6)]: m |= ell(cx, cy, r, r * .85)
    m &= c.a & (c.g > 100)
    setze(c, m, 102)
def zzz(c, g):
    for x0, y0, s in [(g['mx'] + 28, g['my'] - 22, 5), (g['mx'] + 33, g['my'] - 30, 6)]:
        for (ax, ay, bx, by) in [(x0, y0, x0 + s, y0), (x0 + s, y0, x0, y0 + s), (x0, y0 + s, x0 + s, y0 + s)]:
            n = max(abs(bx - ax), abs(by - ay)) + 1
            for t in np.linspace(0, 1, n):
                px, py = int(round(ax + (bx - ax) * t)), int(round(ay + (by - ay) * t))
                if 0 <= px < W and 0 <= py < H: c.g[py, px] = 34; c.a[py, px] = True

EMOTIONEN = [
    ('ruhe', 'freundlich', [], ''), ('glücklich', 'lachend', [], ''), ('hungrig', 'ueberrascht', [sabber], 'Gesicht fehlt'),
    ('traurig', 'traurig', [traene], ''), ('müde', 'muede', [], ''), ('krank', 'muede', [schweiss], 'Gesicht fehlt'),
    ('schmutzig', 'traurig', [schmutz], 'Gesicht fehlt'), ('beleidigt', 'wuetend', [], ''), ('staunen', 'ueberrascht', [], ''),
    ('singen', 'lachend', [], 'Mund offen fehlt'), ('gähnen', 'muede', [], 'Gesicht fehlt'),
]
if __name__ == '__main__':
    z, cols = 3, 4
    rows = (len(EMOTIONEN) + cols - 1) // cols
    out = Image.new('RGB', (cols * W * z, rows * (H * z + 16)), 'white'); d = ImageDraw.Draw(out)
    for k, (n, g, eff, hin) in enumerate(EMOTIONEN):
        geo = geometrie('sitzend', '3-4')
        c = baue_form('sitzend', '3-4', ohren='spitz', schwanz='buschig', gesicht=g)
        for e in eff: e(c, geo)
        x, y = (k % cols) * W * z, (k // cols) * (H * z + 16)
        d.text((x + 4, y + 2), n + (f'  [{hin}]' if hin else ''), fill='black')
        out.paste(c.bild(255, z), (x, y + 16))
    out.save(HIER / 'pruefbogen-emotionen-basis3.png'); print(out.size)
