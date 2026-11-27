#!/usr/bin/env python3
"""Sitzende Figuren aus beliebigen Körper- und Kopfformen (Variation der Silhouette).
Platzierung: Boden bei y = 110, Körper-Hals mittig bei x = 56, Kopf-Hals 4 Pixel über dem Körper-Hals.
Aufruf: python3 formen.py -> pruefbogen-formen.png (4 x 4 Grundformen)
"""
from PIL import Image, ImageDraw
import numpy as np
import basis3_layout as L
from basis3 import Leinwand, rollen, muster_maske, W, H, HIER, TEILE, Z
from ki_teile import lade_teil

BODEN = 110
KOPF_TIEFER = {}   # Platz für kopfspezifische Absenkung (der spitze Kopf wurde am 27.11. entfernt)

def datei(art, form, bevorzugt=None):
    if art == 'koerper' and form == 'sitzend': return TEILE / 'koerper-sitzend-2.png'
    if art == 'kopf' and form == '3-4': return TEILE / 'kopf-3-4-2.png'
    d = sorted(TEILE.glob(f'{art}-{form}-[123].png'))
    return d[0]

def raster(p): return np.array(Image.open(p).convert('RGB'))[::Z, ::Z, 0]

def baue_form(koerper='sitzend', kopf='3-4', kruse=2, ohren='spitz', schwanz='buschig', gesicht='freundlich',
              fell=204, kranz=238, muster=None, kopf_ueber=None):
    kf, hf = L.KOERPER_FORMEN[koerper], L.KOPF_FORMEN[kopf]
    if kopf_ueber is None:
        kopf_ueber = {'schlank': 1}.get(koerper, 4)   # hohe Körper: Kopf tiefer, damit lange Ohren nicht abgeschnitten werden
        kopf_ueber -= KOPF_TIEFER.get(kopf, 0)
    gw, gh = kf['groesse']; hx, hy = kf['hals']
    zx, zy = 56, BODEN - (gh - 1) + hy                  # Körper-Hals auf der Leinwand
    kx, ky = 56, zy - kopf_ueber                         # Kopf-Hals
    c = Leinwand(); r = rollen(fell)
    if schwanz:
        t = lade_teil('schwanz', schwanz)
        if t is not None:
            c.teil(t[0], t[1], (zx + kf['schwanz'][0] - hx, zy + kf['schwanz'][1] - hy), r)
    c.teil(raster(datei('koerper', koerper)), (hx, hy), (zx, zy), r)
    khx, khy = hf['hals']
    if ohren:
        t = lade_teil('ohr', ohren)
        if t is not None:
            if ohren == 'haengend':
                bw = hf['groesse'][0] // 2
                ol, orr = (-(bw - 5), -24), (bw - 4, -24)
            else:
                ol = (hf['ohr_l'][0] - khx, hf['ohr_l'][1] - khy); orr = (hf['ohr_r'][0] - khx, hf['ohr_r'][1] - khy)
            c.teil(t[0], t[1], (kx + ol[0], ky + ol[1]), r)
            c.teil(t[0], t[1], (W - 1 - (kx + orr[0]), ky + orr[1]), r, spiegeln=True)
    c.teil(raster(datei('kopf', kopf)), (khx, khy), (kx, ky), r)
    if muster:
        m, d = muster_maske(muster)
        ok = m & c.a & (c.g >= 85) & (c.g < 250)
        c.g[ok] = np.clip(np.round((c.g[ok] + d) / 17) * 17, 0, 255)
    c.teil(raster(TEILE / f'kruse-flauschig-{kruse}.png'), (28, 2), (kx, ky), {34: 34, 238: kranz, 204: kranz - 34, 170: kranz - 68})
    if gesicht:
        t = lade_teil('gesicht', gesicht)
        if t is not None:
            c.teil(t[0], t[1], (kx + hf['gesicht'][0] - khx, ky + hf['gesicht'][1] - khy), {34: 17, 119: 51, 170: 102, 204: 153, 238: 255})
    return c

KOERPER = ['sitzend', 'rundlich', 'schlank', 'kraeftig']
KOEPFE = ['3-4', 'klein-rund', 'breit']
if __name__ == '__main__':
    z = 2
    out = Image.new('RGB', (3 * W * z, 4 * (H * z + 14)), 'white'); d = ImageDraw.Draw(out)
    for j, k in enumerate(KOERPER):
        for i, h in enumerate(KOEPFE):
            c = baue_form(k, h)
            x, y = i * W * z, j * (H * z + 14)
            d.text((x + 4, y + 1), f'Körper {k} / Kopf {h}', fill='black'); out.paste(c.bild(255, z), (x, y + 14))
    out.save(HIER / 'pruefbogen-formen.png'); print(out.size)
