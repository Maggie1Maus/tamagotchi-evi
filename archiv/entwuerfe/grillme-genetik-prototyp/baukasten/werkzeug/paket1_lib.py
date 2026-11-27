"""Gemeinsame Hilfen für Paket 1 (Python-Nachbau der Schritte aus BuildParts.cs)."""
import numpy as np
from PIL import Image
import os
HIER = os.path.dirname(os.path.abspath(__file__))
WURZEL = os.path.dirname(HIER)

def lade_rgba(pfad):
    return np.array(Image.open(pfad).convert('RGBA'))

def grau(a):
    return (a[..., 0].astype(int) + a[..., 1] + a[..., 2]) // 3

def runter(pfad, w, h):
    """Wie Down() in BuildParts.cs: Median über 3x3 am Blockmittelpunkt, auf 17er-Stufen runden, hell -> transparent."""
    src = np.array(Image.open(pfad).convert('RGB'))
    g = (src[..., 0].astype(int) + src[..., 1] + src[..., 2]) // 3
    H, W = g.shape
    out = np.zeros((h, w, 4), np.uint8)
    ys = ((np.arange(h) + 0.5) * H / h).astype(int)
    xs = ((np.arange(w) + 0.5) * W / w).astype(int)
    for y in range(h):
        for x in range(w):
            vals = []
            for oy in (-1, 0, 1):
                for ox in (-1, 0, 1):
                    vals.append(g[min(H - 1, max(0, ys[y] + oy)), min(W - 1, max(0, xs[x] + ox))])
            v = sorted(vals)[4]
            if v > 247:
                continue
            q = min(255, int(round(v / 17.0)) * 17)
            out[y, x] = (q, q, q, 255)
    return out

def speichern(a, pfad):
    os.makedirs(os.path.dirname(pfad), exist_ok=True)
    Image.fromarray(a.astype(np.uint8), 'RGBA').save(pfad)

def leer(w, h):
    return np.zeros((h, w, 4), np.uint8)
