#!/usr/bin/env python3
"""Bausatz-Test 2: größere runde Basis (96 x 96) mit Detail-Durchgang.

Wie bausatz.py (Teile sind Formen, Umriss automatisch), aber mit:
Fellpunkten, gerasterten Verläufen (Dithering), flauschigem Rand, Augen mit Glanzpunkten,
Nase, Mund mit Zunge, Schnurrhaaren, Wangen-Raster, Bauchfell mit Zackenrand, Pfotenzehen,
Ohrinnenfell und Schwanzfell.
Aufruf: python3 bausatz2.py -> pruefbogen-test2.png
"""
import numpy as np
from PIL import Image, ImageDraw
import os

from ki_teile import lade_teil

W = H = 96
CX = 47.5
LINIE = 34
ANKER = {'ohr': (31, 25), 'ohr-seite': (24, 40), 'schwanz': (65, 74)}
BAYER = np.array([[0, 2], [3, 1]]) / 4.0

Y, X = np.mgrid[0:H, 0:W]

def leer(): return np.zeros((H, W), bool)
def ellipse(cx, cy, rx, ry): return ((X - cx) / rx) ** 2 + ((Y - cy) / ry) ** 2 <= 1
def polygon(p):
    im = Image.new('L', (W, H), 0); ImageDraw.Draw(im).polygon([(int(round(a)), int(round(b))) for a, b in p], fill=255); return np.array(im) > 0
def linie(p, d=2):
    im = Image.new('L', (W, H), 0); ImageDraw.Draw(im).line([(int(round(a)), int(round(b))) for a, b in p], fill=255, width=d); return np.array(im) > 0
def spiegel(m): return m[:, ::-1]
def rauschen(salz=0):
    return ((X * 73856093) ^ (Y * 19349663) ^ (salz * 83492791)) % 100
def dilate(m):
    d = m.copy(); d[1:] |= m[:-1]; d[:-1] |= m[1:]; d[:, 1:] |= m[:, :-1]; d[:, :-1] |= m[:, 1:]; return d
def erode(m):
    e = m.copy(); e[1:] &= m[:-1]; e[:-1] &= m[1:]; e[:, 1:] &= m[:, :-1]; e[:, :-1] &= m[:, 1:]
    e[0] = e[-1] = False; e[:, 0] = e[:, -1] = False; return e
def umriss(m): return dilate(m) & ~m

def flauschig(m, dichte=26, salz=1, oben_frei=True):
    """Rand mit kleinen Fellbüscheln: einzelne Randpixel werden nach außen verlängert."""
    rand = umriss(m)
    zufall = rauschen(salz) < dichte
    if oben_frei:
        zufall &= ~(Y < (np.nonzero(m)[0].min() + 6))
    return m | (rand & zufall)

class Bild:
    def __init__(self):
        self.g = np.zeros((H, W), np.uint8); self.a = np.zeros((H, W), bool)
    def setze(self, m, v):
        self.g[m] = np.clip(v, 0, 255) if np.ndim(v) == 0 else np.clip(v, 0, 255)[m]; self.a[m] = True
    def fell(self, maske, fuell, tuften=True, salz=1, punkte=True, schatten=True, naht=None):
        m = flauschig(maske, salz=salz) if tuften else maske
        rand = umriss(m)
        self.setze(rand if naht is None else rand & ~naht, LINIE)
        ys, xs = np.nonzero(m)
        cy, cx = (ys.min() + ys.max()) / 2, (xs.min() + xs.max()) / 2
        hy, hx = max(1, (ys.max() - ys.min()) / 2), max(1, (xs.max() - xs.min()) / 2)
        s = 0.5 + 0.35 * (Y - cy) / hy + 0.22 * (X - cx) / hx          # 0 = hell (oben links), 1 = dunkel
        bay = BAYER[Y % 2, X % 2]
        stufe = np.floor(1.2 - 2.6 * s + bay * 0.9 + 0.1) if schatten else np.zeros((H, W))
        stufe = np.clip(stufe, -2, 1)
        v = fuell + 17 * stufe
        r = rauschen(salz + 5)
        if punkte:
            v = np.where(r < 5, v - 17, v); v = np.where((r >= 5) & (r < 8), v + 17, v)
        # Innenrand unten rechts etwas dunkler (Rundung)
        innen = m & ~erode(m)
        v = np.where(innen & ((X - cx) + (Y - cy) > 4), v - 17, v)
        self.setze(m, np.round(v / 17) * 17)
        return m
    def ki_teil(self, teil, ziel, fell, innen, gespiegelt=False, licht=None):
        raster, (ax, ay) = teil
        gx, gy = ziel
        rollen = {
            34: LINIE,
            119: innen,
            170: fell - 34,
            204: fell,
            238: fell + 34 if licht is None else licht,
        }
        fuellung = leer()
        for rolle, ton in rollen.items():
            y, x = np.nonzero(raster == rolle)
            x, y = x - ax + gx, y - ay + gy
            if gespiegelt:
                x = W - 1 - x
            sichtbar = (x >= 0) & (x < W) & (y >= 0) & (y < H)
            maske = leer()
            maske[y[sichtbar], x[sichtbar]] = True
            self.setze(maske, ton)
            if rolle != LINIE:
                fuellung |= maske
        return fuellung

    def bild(self, hg=255, z=4):
        a = np.full((H, W, 3), hg, np.uint8); a[self.a] = np.stack([self.g[self.a]] * 3, 1)
        return Image.fromarray(a).resize((W * z, H * z), Image.NEAREST)

# ---------- Teile ----------
def ohr(art):
    ax, ay = ANKER['ohr']; sx, sy = ANKER['ohr-seite']
    if art == 'spitz':
        a = polygon([(ax - 8, ay + 6), (ax - 6, ay - 14), (ax + 8, ay + 3)]); i = polygon([(ax - 5, ay + 3), (ax - 5, ay - 8), (ax + 3, ay + 2)])
    elif art == 'rund':
        a = ellipse(ax - 1, ay - 3, 8.5, 8.5); i = ellipse(ax - 1, ay - 3, 5, 5)
    elif art == 'lang':
        a = ellipse(ax - 3, ay - 12, 5.5, 16); i = ellipse(ax - 3, ay - 12, 2.8, 12)
    elif art == 'haengend':
        a = ellipse(sx - 4, sy + 8, 7, 14); i = leer()
    elif art == 'buschel':
        a = polygon([(ax - 8, ay + 6), (ax - 6, ay - 12), (ax + 8, ay + 3)])
        a |= polygon([(ax - 8, ay - 12), (ax - 6, ay - 17), (ax - 4, ay - 12)]) | polygon([(ax - 5, ay - 13), (ax - 3, ay - 18), (ax - 1, ay - 12)]) | polygon([(ax - 2, ay - 11), (ax, ay - 16), (ax + 1, ay - 10)])
        i = polygon([(ax - 5, ay + 3), (ax - 5, ay - 6), (ax + 3, ay + 2)])
    else: return None
    return a, i

def schwanz(art):
    ax, ay = ANKER['schwanz']; sp = leer()
    if art == 'puschel': m = ellipse(ax + 9, ay - 2, 8, 8)
    elif art == 'buschig':
        m = ellipse(ax + 12, ay - 8, 10, 18); sp = m & (Y < ay - 17)
    elif art == 'duenn':
        m = linie([(ax, ay + 4), (ax + 11, ay + 4), (ax + 17, ay - 3), (ax + 15, ay - 12)], 3) | ellipse(ax + 15, ay - 14, 4, 4); sp = ellipse(ax + 15, ay - 14, 4, 4)
    elif art == 'blitz':
        m = polygon([(ax, ay + 3), (ax + 9, ay - 4), (ax + 6, ay - 6), (ax + 17, ay - 19), (ax + 14, ay - 9), (ax + 18, ay - 11), (ax + 8, ay + 4)])
    else: return None, None
    return m, sp

def tiere(ohren='spitz', schw='puschel', f=204, bauch=238, spitze=250, innen=119, muster='keins', mund='offen'):
    b = Bild()
    kopf = ellipse(CX, 40, 26, 23); koerper = ellipse(CX, 71, 21, 18)
    arme = ellipse(26, 70, 5, 8) | ellipse(69, 70, 5, 8)
    fuesse = ellipse(36, 88, 8, 4.5) | ellipse(59, 88, 8, 4.5)
    schwanznaht = ohrnaht = None
    schwanzteil = lade_teil('schwanz', schw)
    if schwanzteil is not None:
        licht = spitze if schw in ('buschig', 'duenn') else None
        schwanznaht = b.ki_teil(schwanzteil, ANKER['schwanz'], f, innen, licht=licht)
    else:
        sm, sp = schwanz(schw)
        if sm is not None:
            m = b.fell(sm, f, salz=21)
            if sp.any():
                t = sp & erode(m); b.setze(t, np.where(rauschen(9) < 14, spitze - 17, spitze))
    ohrteil = lade_teil('ohr', ohren)
    if ohrteil is not None:
        ziel = ANKER['ohr-seite' if ohren == 'haengend' else 'ohr']
        ohrnaht = b.ki_teil(ohrteil, ziel, f, innen)
        ohrnaht |= b.ki_teil(ohrteil, ziel, f, innen, gespiegelt=True)
    else:
        o = ohr(ohren)
        if o is not None:
            for sx in (False, True):
                a, i = o
                a = spiegel(a) if sx else a; i = spiegel(i) if sx else i
                m = b.fell(a, f, tuften=False, salz=31 + sx)
                if i.any():
                    ii = i & erode(m)
                    b.setze(ii, np.where(rauschen(13) < 25, innen + 17, innen))
                    # Innenohrfell: helle Strichel
                    b.setze(ii & (((X + 2 * Y) % 7) == 0), innen + 34)
    b.fell(fuesse, f, tuften=False, salz=41)
    # Zehen
    for fx in (36, 59):
        for dx in (-3, 0, 3):
            b.setze((X == fx + dx) & (Y >= 87) & (Y <= 88), LINIE + 17)
    b.fell(koerper, f, salz=51, naht=schwanznaht)
    # Bauch mit Zackenrand (wie Evis Halskrause)
    zack = 1.0 + 0.10 * np.abs(np.sin((X - CX) * 0.55))
    bm = ((((X - CX) / (13 * zack)) ** 2 + ((Y - 74) / (12 * zack)) ** 2) <= 1) & koerper & (Y > 62)
    bi = bm & erode(koerper)
    b.setze(bi, np.where(rauschen(17) < 8, bauch - 17, bauch))
    b.fell(arme, f, tuften=False, salz=61)
    b.fell(kopf, f, salz=71, naht=ohrnaht)
    # Muster
    if muster == 'flecken':
        mm = leer()
        for cx, cy, r in [(60, 26, 5), (34, 72, 4.5), (60, 76, 5), (45, 84, 3), (67, 62, 3), (30, 55, 3)]:
            mm |= ellipse(cx, cy, r, r * 0.9)
        t = mm & (koerper | kopf) & ~bm & erode(kopf | koerper)
        b.setze(t, np.where(rauschen(3) < 20, f - 85 + 17, f - 85))
    elif muster == 'streifen':
        mm = ((Y - X // 2) % 9 < 3) & (koerper | (kopf & (Y < 28))) & ~bm
        b.setze(mm & erode(koerper | kopf), f - 51)
    # Gesicht
    for ex in (35, 59):
        b.setze(ellipse(ex + .5, 41, 4, 5.2), 17)
        b.setze(ellipse(ex - 0.5, 39, 1.4, 1.6), 255)            # großer Glanzpunkt
        b.setze((X == ex + 2) & (Y == 44), 238)                   # kleiner Glanzpunkt
        au = -1 if ex < CX else 1                                   # Wimpern am äußeren Augenrand
        for k in (0, 1):
            b.setze((X == int(ex + au * (5 + k))) & (Y == 37 - k), 17)
    # Wangen (gerastert)
    for wx in (26, 69):
        w = ellipse(wx, 49, 5, 3.2) & ((X + Y) % 2 == 0)
        b.setze(w, 170)
    # Nase und Mund
    b.setze(polygon([(46, 47), (50, 47), (48, 50)]), 51)
    if mund == 'offen':
        b.setze(ellipse(CX, 52, 6.5, 5) & (Y >= 52), 17)
        b.setze(ellipse(CX, 55.5, 3.6, 1.9) & (Y >= 54) & (Y <= 56), 153)   # Zunge
        b.setze(((X == 41) | (X == 54)) & (Y == 51), 34)          # Mundwinkel
    else:
        b.setze(linie([(42, 52), (45, 55), (50, 55), (53, 52)], 1), 34)
    # Schnurrhaare
    for y0, dy in ((48, -2), (50, 0), (52, 2)):
        b.setze(linie([(21, y0), (13, y0 + dy)], 1), 119)
        b.setze(linie([(74, y0), (82, y0 + dy)], 1), 119)
    return b

def sheet():
    from bausatz import tiere as alt
    ohren = ['spitz', 'rund', 'lang', 'haengend', 'buschel']; schw = ['puschel', 'buschig', 'duenn', 'blitz']
    z = 4
    z1 = [tiere(o, 'puschel') for o in ohren] + [tiere('rund', s) for s in schw]
    z2 = [tiere('spitz', 'buschig', 204, muster='flecken'), tiere('lang', 'puschel', 238), tiere('haengend', 'duenn', 187, muster='flecken'),
          tiere('rund', 'blitz', 170, muster='streifen'), tiere('buschel', 'buschig', 153, spitze=238), tiere('spitz', 'duenn', 221, muster='streifen', mund='zu'),
          tiere('lang', 'buschig', 136, bauch=221), tiere('haengend', 'puschel', 255, muster='flecken'), tiere('rund', 'puschel', 119, bauch=187), tiere('buschel', 'blitz', 204, mund='zu')]
    breite = 5 * W * z
    # Vergleich: alte Version (6x) neben neuer (4x), gleiche Bildschirmgröße
    out = Image.new('RGB', (breite, 40 + H * z * 3 + 40 + H * 2 + 20), 'white')
    d = ImageDraw.Draw(out)
    d.text((6, 4), 'Vergleich (gleiche Bildschirmgröße): links erste Version 64 px, rechts neue Version 96 px', fill='black')
    out.paste(alt('spitz', 'buschig').bild(255, 6), (6, 20))
    out.paste(tiere('spitz', 'buschig').bild(255, 4), (6 + 64 * 6 + 20, 20))
    out.paste(tiere('rund', 'puschel', muster='flecken').bild(255, 4), (6 + 64 * 6 + 20 + W * 4 + 10, 20))
    out.paste(tiere('lang', 'duenn', 238, mund='zu').bild(90, 4), (6 + 64 * 6 + 20 + 2 * (W * 4 + 10), 20))
    y0 = 40 + max(64 * 6, H * 4)
    d.text((6, y0 - 14), 'Alle Ohren und Schwänze, dazu zehn Kombinationen', fill='black')
    for i, t in enumerate(z1[:9]):
        out.paste(t.bild(255, 3), (6 + (i % 9) * W * 3 // 1 if False else 6 + i * (W * 2 + 4), y0 + 0)) if False else None
    return out, z1, z2, y0

if __name__ == '__main__':
    out, z1, z2, y0 = sheet()
    z = 3
    zeilen = [z1[:5], z1[5:], z2[:5], z2[5:]]
    gesamt = Image.new('RGB', (max(out.width, 5 * W * z), y0 + 20 + len(zeilen) * H * z + 40 + H), 'white')
    gesamt.paste(out.crop((0, 0, out.width, y0 + 4)), (0, 0))
    d = ImageDraw.Draw(gesamt)
    y = y0 + 4
    for k, r in enumerate(zeilen):
        for i, t in enumerate(r): gesamt.paste(t.bild(255, z), (i * W * z, y))
        y += H * z
    d.text((6, y + 2), 'Originalgröße 1x (96 x 96):', fill='black')
    for i, t in enumerate(z2): gesamt.paste(t.bild(255, 1), (6 + i * 100, y + 14))
    hier = os.path.dirname(os.path.abspath(__file__))
    gesamt.save(os.path.join(hier, 'pruefbogen-test2.png')); print(gesamt.size)
