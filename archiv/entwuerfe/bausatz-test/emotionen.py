#!/usr/bin/env python3
"""Emotionen und Posen aus dem Spiel auf der aktuellen (runden) Basis: Gesichter + Zusatzeffekte per Skript.

Zusatzeffekte (Schweißtropfen, Träne, Sabber, Schmutz, Zzz) sind kleine Pixelformen, keine KI-Teile.
Aufruf: python3 emotionen.py -> pruefbogen-emotionen.png
"""
import os
import numpy as np
from PIL import Image, ImageDraw
import posen
from bausatz2 import X, Y, W, H, ellipse, polygon, linie, leer, rauschen, LINIE

def tropfen(b, cx, cy, h=6):
    m = polygon([(cx, cy - h), (cx - 2, cy), (cx + 2, cy)]) | ellipse(cx, cy, 2.4, 2.4)
    rand = (m | np.roll(m, 1, 0) | np.roll(m, -1, 0) | np.roll(m, 1, 1) | np.roll(m, -1, 1)) & ~m
    b.setze(rand, 34); b.setze(m, 238)

def schweiss(b, p):
    kx, ky = p['kopf'][:2]; tropfen(b, int(kx + 20), int(ky - 12))
def traene(b, p):
    kx, ky = p['kopf'][:2]
    tropfen(b, int(kx - 13), int(ky + 12), 5)
def sabber(b, p):
    gx, gy = p['gesicht']
    m = (X >= gx + 5) & (X <= gx + 6) & (Y >= gy + 9) & (Y <= gy + 15)
    m |= ellipse(gx + 5.5, gy + 16, 2, 2.4)
    rand = (m | np.roll(m, 1, 1) | np.roll(m, -1, 1) | np.roll(m, -1, 0)) & ~m
    b.setze(rand, 34); b.setze(m, 238)
def schmutz(b, p):
    kx, ky = p['kopf'][:2]
    mm = leer()
    for cx, cy, r in [(kx - 14, ky - 8, 3.2), (kx + 15, ky + 4, 2.5), (kx + 4, ky - 16, 2), (35, 76, 3.5), (60, 70, 3), (48, 84, 2.4)]:
        mm |= ellipse(cx, cy, r, r * .85)
    b.setze(mm & b.a & ~(b.g == 34) & ~(b.g == 17), np.where(rauschen(5) < 30, 102, 85))
def zzz(b, p):
    for k, (x0, y0, s) in enumerate([(76, 46, 5), (81, 37, 6), (87, 27, 7)]):
        b.setze(linie([(x0, y0), (x0 + s, y0), (x0, y0 + s), (x0 + s, y0 + s)], 1), 34)

# (Name im Spiel, Pose, Gesicht, Effekte, Hinweis)
EMOTIONEN = [
    ('ruhe', 'stehen', 'freundlich', [], ''),
    ('glücklich', 'stehen', 'lachend', [], ''),
    ('hungrig', 'stehen', 'ueberrascht', [sabber], 'eigenes Gesicht fehlt'),
    ('traurig', 'stehen', 'traurig', [traene], ''),
    ('müde', 'stehen', 'muede', [], ''),
    ('krank', 'stehen', 'muede', [schweiss], 'eigenes Gesicht fehlt'),
    ('schmutzig', 'stehen', 'traurig', [schmutz], ''),
    ('beleidigt', 'stehen', 'wuetend', [], ''),
    ('staunen', 'stehen', 'ueberrascht', [], ''),
    ('singen', 'huepfen', 'lachend', [], 'Mund offen fehlt'),
    ('gähnen', 'stehen', 'muede', [], 'Gesicht fehlt'),
    ('schlafend', 'schlafen', 'muede', [zzz], ''),
]

if __name__ == '__main__':
    hier = os.path.dirname(os.path.abspath(__file__))
    z, cols = 3, 4
    rows = (len(EMOTIONEN) + cols - 1) // cols
    out = Image.new('RGB', (cols * W * z, rows * (H * z + 16)), 'white'); d = ImageDraw.Draw(out)
    for k, (n, pose, g, eff, hin) in enumerate(EMOTIONEN):
        b = posen.tiere_pose(pose, 'spitz', 'buschig', 204, gesicht=g)
        for e in eff: e(b, posen.POSEN[pose])
        x, y = (k % cols) * W * z, (k // cols) * (H * z + 16)
        d.text((x + 4, y + 2), n + (f'  [{hin}]' if hin else ''), fill='black')
        out.paste(b.bild(255, z), (x, y + 16))
    out.save(os.path.join(hier, 'pruefbogen-emotionen.png')); print(out.size)
