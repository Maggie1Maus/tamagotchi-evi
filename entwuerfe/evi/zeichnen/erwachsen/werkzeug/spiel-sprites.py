#!/usr/bin/env python3
"""Baut die neuen Spielbilder der erwachsenen Evi aus den Teilen und legt sie nach
entwuerfe/evi/graustufen-neu/Sprites/04-Erwachsen/<bild>.png (Graustufen, Alpha).
Jedes Bild bekommt die Groesse des gleichnamigen alten Bildes im Spiel (Evi-Graustufen.zip), damit das Spiel
es ohne Aenderung nutzt. Massstab: wie die normale Evi (114 x 116), nur kleiner wenn die Figur sonst nicht
in die alte Flaeche passt. Bilder, die hier fehlen (schlafend, drehen, fenster, huepfen, laufen, garten-schnuppern,
garten-untersuchen), bleiben die alten aus der ZIP.
Aufruf: python3 spiel-sprites.py   (dauert etwa 2 Minuten)"""
import io, os, sys, zipfile, numpy as np
from PIL import Image
import zusammensetzen as z

REPO = os.path.abspath(os.path.join(z.W, '..', '..', '..', '..'))
ZIP = os.path.join(REPO, 'entwuerfe/evi/graustufen-vorschau/Evi-Graustufen.zip')
AUS = os.path.join(REPO, 'entwuerfe/evi/graustufen-neu/Sprites/04-Erwachsen')

OB, OH = 'ohr-buschel', 'ohr-buschel-haengend'
SB, SG = 'schwanz-buschel', 'schwanz-buschel-gesenkt'
K, KP, KV = 'koerper-sitzend', 'koerper-sitzend-ohne-pfote-links', 'koerper-sitzend-ohne-vorderpfoten'
# Bild: (Gesicht, Ohren, Schwanz, Koerper, Pfoten). Thermometer und Schmutzflecken fehlen noch.
REZEPTE = {
    'ruhe': ('neutral', OB, SB, K, None), 'gluecklich': ('gluecklich', OB, SB, K, None), 'staunen': ('staunen', OB, SB, K, None),
    'singen': ('singen', OH, SB, K, None), 'traurig': ('traurig', OH, SG, K, None),
    'muede': ('muede', OH, SG, K, None), 'krank': ('krank', OH, SG, K, None),
    'schmutzig': ('schmutzig', OH, SG, K, None),
    'hungrig': ('hungrig', OH, SG, KP, 'pfote-finger'), 'denken': ('denken', OB, SB, KP, 'pfote-finger'),
    'gaehnen': ('gaehnen', OH, SB, KP, 'pfote-gesicht'),
    'beleidigt': ('beleidigt', OH, SG, KV, 'arme-verschraenkt__aufkoerper'),
    'garten-zeigen': ('gluecklich', OB, SB, KV, 'pfoten-hoch'),
}

def main():
    nur = sys.argv[1:]
    zf = zipfile.ZipFile(ZIP)
    os.makedirs(AUS, exist_ok=True)
    g0, a0 = z.bauen('gesicht-neutral', OB, SB)
    cell0, _, _ = z.standard_raster(g0, a0)
    for name, (ge, oh, sw, ko, pf) in REZEPTE.items():
        if nur and name not in nur: continue
        g, a = z.bauen('gesicht-' + ge, oh, sw, koerper=ko, pfoten=pf)
        alt = 'gluecklich' if name == 'ruhe' else name            # ruhe gibt es in der ZIP nicht: Groesse von glücklich
        w, h = Image.open(io.BytesIO(zf.read(f'Sprites/04-Erwachsen/{alt}.png'))).size
        ys, xs = np.nonzero(a * 255 > 60); x0, x1, y0, y1 = xs.min(), xs.max() + 1, ys.min(), ys.max() + 1
        cell = max((x1 - x0) / w, (y1 - y0) / h, cell0)          # nie groesser als die normale Evi
        ox = (x0 + x1) / 2 - w * cell / 2; oy = y1 - h * cell     # mittig, Fuesse unten
        Image.fromarray(z.raster(g, a, cell, ox, oy, w, h), 'RGBA').save(os.path.join(AUS, name + '.png'))
        print(f'{name:14s} {w}x{h}  Massstab {cell0 / cell * 100:.0f}%', flush=True)

if __name__ == '__main__': main()
