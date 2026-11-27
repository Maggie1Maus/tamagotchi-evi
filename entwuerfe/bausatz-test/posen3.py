#!/usr/bin/env python3
"""Posen auf der 3/4-Basis: Körper je Haltung + gleicher Kopf, gleiche Ohren/Schwänze/Gesichter.
Aufruf: python3 posen3.py -> pruefbogen-posen-basis3.png
"""
import glob
from PIL import Image, ImageDraw
import numpy as np
import basis3_layout as L
from basis3 import Leinwand, lade, rollen, W, H, HIER, TEILE, Z
from ki_teile import lade_teil
import emotionen3

def koerper_datei(pose, nr=None):
    if nr is not None:
        return TEILE / f'koerper-{pose}-{nr}.png'
    dateien = sorted(TEILE.glob(f'koerper-{pose}-[123].png'))
    # sitzend: Empfehlung 2
    if pose == 'sitzend': return TEILE / 'koerper-sitzend-2.png'
    return dateien[0] if dateien else None

def baue_pose(pose='sitzend', kopf=2, kruse=2, ohren='spitz', schwanz='buschig', gesicht='freundlich', fell=204, kranz=238, muster=None, nr=None):
    p = L.POSEN[pose]
    c = Leinwand(); r = rollen(fell)
    hx, hy = p['hals']; zx, zy = p['hals_ziel']
    kx, ky = p['kopf_ziel']
    koerper = np.array(Image.open(koerper_datei(pose, nr)).convert('RGB'))[::Z, ::Z, 0]
    if schwanz:
        t = lade_teil('schwanz', schwanz)
        if t is not None:
            sx, sy = p['schwanz']
            c.teil(t[0], t[1], (zx + sx - hx + p.get('schwanz_dx', 0), zy + sy - hy), r)
    c.teil(koerper, (hx, hy), (zx, zy), r)
    if ohren:
        t = lade_teil('ohr', ohren)
        if t is not None:
            if ohren == 'haengend': ol, orr = L.KOPF_SEITE_LINKS, L.KOPF_SEITE_RECHTS
            else: ol, orr = L.KOPF_OHR_LINKS, L.KOPF_OHR_RECHTS
            c.teil(t[0], t[1], (kx + ol[0], ky + ol[1]), r)
            c.teil(t[0], t[1], (W - 1 - (kx + orr[0]), ky + orr[1]), r, spiegeln=True)
    c.teil(lade('kopf', '3-4', kopf), (30, 40), (kx, ky), r)
    if muster:
        m, d = __import__('basis3').muster_maske(muster)
        ok = m & c.a & (c.g >= 85) & (c.g < 250)
        c.g[ok] = np.clip(np.round((c.g[ok] + d) / 17) * 17, 0, 255)
    c.teil(lade('kruse', 'flauschig', kruse), (28, 2), (kx, ky), {34: 34, 238: kranz, 204: kranz - 34, 170: kranz - 68})
    if gesicht:
        t = lade_teil('gesicht', gesicht)
        if t is not None:
            c.teil(t[0], t[1], (kx + L.KOPF_GESICHT[0], ky + L.KOPF_GESICHT[1]), {34: 17, 119: 51, 170: 102, 204: 153, 238: 255})
    return c

POSEN = [('sitzend', 'freundlich'), ('laufen', 'lachend'), ('huepfen', 'lachend'), ('geduckt', 'ueberrascht'), ('eingerollt', 'muede')]
FIGUREN = [('spitz', 'buschig', 187, 238, None), ('haengend', 'puschel', 238, 255, 'flecken'), ('lang', 'duenn', 153, 221, None)]
if __name__ == '__main__':
    z = 3
    out = Image.new('RGB', (len(POSEN) * W * z, len(FIGUREN) * (H * z + 14)), 'white'); d = ImageDraw.Draw(out)
    for j, (o, s, f, kz, m) in enumerate(FIGUREN):
        for i, (pose, g) in enumerate(POSEN):
            c = baue_pose(pose, ohren=o, schwanz=s, gesicht=g, fell=f, kranz=kz, muster=m)
            if pose == 'eingerollt': emotionen3.zzz(c)
            x, y = i * W * z, j * (H * z + 14)
            d.text((x + 4, y + 1), pose, fill='black'); out.paste(c.bild(255, z), (x, y + 14))
    out.save(HIER / 'pruefbogen-posen-basis3.png'); print(out.size)
