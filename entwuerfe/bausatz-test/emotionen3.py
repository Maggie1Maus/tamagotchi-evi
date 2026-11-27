#!/usr/bin/env python3
"""Gefühle (Gruppe A der Spielposen) auf der 3/4-Basis: Gesicht + Effekt per Skript.
Aufruf: python3 emotionen3.py -> pruefbogen-emotionen-basis3.png
"""
import numpy as np
from PIL import Image, ImageDraw
import basis3
from basis3 import W, H, HIER, baue

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

def schweiss(c): tropfen(c, 82, 34)
def traene(c): tropfen(c, 45, 58, 5)
def sabber(c):
    m = (X >= 62) & (X <= 63) & (Y >= 58) & (Y <= 64) | ell(62.5, 65, 2.2, 2.6)
    zeichne(c, m)
def schmutz(c):
    m = np.zeros((H, W), bool)
    for cx, cy, r in [(40, 36, 3.4), (74, 44, 2.6), (60, 28, 2.2), (30, 90, 3.6), (52, 96, 3), (72, 80, 2.6)]: m |= ell(cx, cy, r, r * .85)
    m &= c.a & (c.g > 100)
    setze(c, m, 102)
def zzz(c):
    for x0, y0, s in [(86, 30, 5), (90, 22, 6), (94, 12, 7)]:
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
        c = baue(2, 2, ohren='spitz', schwanz='buschig', gesicht=g)
        for e in eff: e(c)
        x, y = (k % cols) * W * z, (k // cols) * (H * z + 16)
        d.text((x + 4, y + 2), n + (f'  [{hin}]' if hin else ''), fill='black')
        out.paste(c.bild(255, z), (x, y + 16))
    out.save(HIER / 'pruefbogen-emotionen-basis3.png'); print(out.size)
