#!/usr/bin/env python3
"""Pose-Test: dieselben Teile (Ohren, Schwanz, Gesicht, Fell, Muster) in mehreren Posen.

Jede Pose beschreibt nur die Formen der Basis (Kopf, Körper, Arme, Füße) und die ANSATZPUNKTE
für Ohren, Schwanz und Gesicht. Die Teile selbst bleiben unverändert und wandern mit.
Aufruf: python3 posen.py  ->  pruefbogen-posen.png
"""
import os
import numpy as np
from PIL import Image, ImageDraw
import bausatz2 as b
from bausatz2 import W, H, X, Y, ellipse, erode, rauschen, polygon, linie, leer, Bild, LINIE, GESICHTSTOENE
from ki_teile import lade_teil

POSEN = {
    # kopf=(x,y,rx,ry) koerper=(x,y,rx,ry) arme/fuesse = Liste (x,y,rx,ry)
    'stehen': dict(
        kopf=(47.5, 40, 26, 23), koerper=(47.5, 71, 21, 18),
        arme=[(26, 70, 5, 8), (69, 70, 5, 8)], fuesse=[(36, 88, 8, 4.5), (59, 88, 8, 4.5)],
        ohr_l=(31, 25), ohr_r=(64, 25), ohr_seite_l=(24, 40), ohr_seite_r=(71, 40),
        schwanz=(70, 78), gesicht=(48, 48), gesicht_name='freundlich'),
    'huepfen': dict(  # Arme hoch, Füße auseinander, alles etwas höher
        kopf=(47.5, 33, 26, 23), koerper=(47.5, 62, 20, 17),
        arme=[(19, 42, 4.5, 9), (76, 42, 4.5, 9)], fuesse=[(33, 82, 8, 4.5), (62, 82, 8, 4.5)],
        ohr_l=(31, 18), ohr_r=(64, 18), ohr_seite_l=(24, 33), ohr_seite_r=(71, 33),
        schwanz=(69, 68), gesicht=(48, 41), gesicht_name='lachend'),
    'schlafen': dict(  # liegt zusammengerollt, Kopf tief, Pfoten vor dem Kopf
        kopf=(47.5, 58, 26, 22), koerper=(47.5, 78, 31, 13),
        arme=[(35, 79, 7, 4.5), (60, 79, 7, 4.5)], fuesse=[],
        ohr_l=(31, 43), ohr_r=(64, 43), ohr_seite_l=(24, 58), ohr_seite_r=(71, 58),
        schwanz=(67, 84), gesicht=(48, 66), gesicht_name='muede'),
    'neugierig': dict(  # Kopf schief: Kopf nach rechts, Ohren verschieden hoch
        kopf=(51, 40, 26, 23), koerper=(47.5, 71, 21, 18),
        arme=[(26, 70, 5, 8), (69, 70, 5, 8)], fuesse=[(36, 88, 8, 4.5), (59, 88, 8, 4.5)],
        ohr_l=(35, 27), ohr_r=(69, 23), ohr_seite_l=(28, 42), ohr_seite_r=(75, 38),
        schwanz=(70, 78), gesicht=(52, 48), gesicht_name='ueberrascht'),
}

def tiere_pose(pose='stehen', ohren='spitz', schw='puschel', f=204, bauch=238, spitze=250, innen=119, muster='keins', gesicht=None):
    p = POSEN[pose]
    g = gesicht or p['gesicht_name']
    bld = Bild()
    kx, ky, krx, kry = p['kopf']
    kopf = ellipse(kx, ky, krx, kry)
    koerper = ellipse(*p['koerper'])
    arme = leer()
    for a in p['arme']: arme |= ellipse(*a)
    fuesse = leer()
    for a in p['fuesse']: fuesse |= ellipse(*a)
    # Schwanz
    schwanznaht = ohrnaht = None
    st = lade_teil('schwanz', schw)
    if st is not None:
        licht = spitze if schw in ('buschig', 'duenn') else None
        schwanznaht = bld.ki_teil(st, p['schwanz'], f, innen, licht=licht)
    # Ohren (links normal, rechts gespiegelt; Ansatz je Seite einzeln, damit der Kopf geneigt sein kann)
    ot = lade_teil('ohr', ohren)
    if ot is not None:
        seit = ohren == 'haengend'
        zl = p['ohr_seite_l' if seit else 'ohr_l']; zr = p['ohr_seite_r' if seit else 'ohr_r']
        ohrnaht = bld.ki_teil(ot, zl, f, innen)
        ohrnaht |= bld.ki_teil(ot, (W - 1 - zr[0], zr[1]), f, innen, gespiegelt=True)
    # Füße, Körper, Bauch, Arme, Kopf
    if fuesse.any():
        bld.fell(fuesse, f, tuften=False, salz=41)
        for fx, fy, _, _ in p['fuesse']:
            for dx in (-3, 0, 3):
                bld.setze((X == int(fx + dx)) & (Y >= int(fy) - 1) & (Y <= int(fy)), LINIE + 17)
    bld.fell(koerper, f, salz=51, naht=schwanznaht)
    bx, by = p['koerper'][0], p['koerper'][1]
    zack = 1.0 + 0.10 * np.abs(np.sin((X - bx) * 0.55))
    bm = ((((X - bx) / (13 * zack)) ** 2 + ((Y - (by + 3)) / (min(12, p['koerper'][3] - 3) * zack)) ** 2) <= 1) & koerper & (Y > by - 9)
    if pose == 'schlafen':
        bm = leer()
    bld.setze(bm & erode(koerper), np.where(rauschen(17) < 8, bauch - 17, bauch))
    bld.fell(arme, f, tuften=False, salz=61)
    bld.fell(kopf, f, salz=71, naht=ohrnaht)
    # Muster
    if muster == 'flecken':
        mm = leer()
        for cx, cy, r in [(kx + 12, ky - 14, 5), (34, p['koerper'][1] + 1, 4.5), (60, p['koerper'][1] + 5, 5), (kx - 17, ky + 15, 3)]:
            mm |= ellipse(cx, cy, r, r * 0.9)
        bld.setze(mm & (koerper | kopf) & ~bm & erode(kopf | koerper), np.where(rauschen(3) < 20, f - 85 + 17, f - 85))
    elif muster == 'streifen':
        mm = ((Y - X // 2) % 9 < 3) & (koerper | (kopf & (Y < ky - 12))) & ~bm
        bld.setze(mm & erode(koerper | kopf), f - 51)
    # Wangen, Gesicht (Teil), Schnurrhaare: alles relativ zum Kopf
    for wx in (kx - 21.5, kx + 21.5):
        bld.setze(ellipse(wx, ky + 9, 5, 3.2) & ((X + Y) % 2 == 0), 170)
    gt = lade_teil('gesicht', g)
    if gt is not None:
        bld.setze_teil(gt, p['gesicht'], GESICHTSTOENE)
    for dy0, dy in ((8, -2), (10, 0), (12, 2)):
        y0 = ky + dy0
        # Schnurrhaare liegen hinter den Armen (nicht über Armen oder Körper zeichnen)
        bld.setze(linie([(kx - 26, y0), (kx - 34, y0 + dy)], 1) & ~arme & ~koerper, 119)
        bld.setze(linie([(kx + 26, y0), (kx + 34, y0 + dy)], 1) & ~arme & ~koerper, 119)
    return bld

if __name__ == '__main__':
    hier = os.path.dirname(os.path.abspath(__file__))
    z = 3
    zeilen = [
        ('spitz', 'buschig', 204, 'keins'), ('haengend', 'duenn', 187, 'flecken'), ('buschel', 'blitz', 221, 'keins'),
        ('rund', 'puschel', 153, 'streifen'), ('lang', 'buschig', 238, 'keins')]
    posen = list(POSEN)
    out = Image.new('RGB', (len(posen) * W * z, 18 + len(zeilen) * H * z), 'white')
    d = ImageDraw.Draw(out)
    for i, pn in enumerate(posen):
        d.text((i * W * z + 6, 3), pn, fill='black')
    for r, (o, s, f, m) in enumerate(zeilen):
        for i, pn in enumerate(posen):
            out.paste(tiere_pose(pn, o, s, f, muster=m).bild(255, z), (i * W * z, 18 + r * H * z))
    out.save(os.path.join(hier, 'pruefbogen-posen.png'))
    print(out.size)
