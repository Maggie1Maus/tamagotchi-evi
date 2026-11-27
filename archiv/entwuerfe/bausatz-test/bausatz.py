#!/usr/bin/env python3
"""Bausatz-Test: runde neue Basis, Teile per Skript erzeugt (kein KI-Bild, keine Evi-Pixel).

Jedes Teil ist nur eine FORM (Maske). Füllung, Schattierung und sauberer Umriss
(1 Pixel, dunkelgrau) entstehen automatisch, deshalb passen alle Teile zusammen.
Teile hängen an festen Ansatzpunkten (ANKER) der Basis.
Aufruf: python3 bausatz.py  ->  pruefbogen-test.png
"""
import numpy as np
from PIL import Image, ImageDraw
import os

W = H = 64
CX = 31.5                      # Mittelachse (Spiegelung)
LINIE = 34                     # Umrissgrau
ANKER = {                      # Ansatzpunkte (x, y) der linken Seite; rechts wird gespiegelt
    'ohr':   (21, 17),         # oben links am Kopf
    'ohr-seite': (16, 27),     # seitlich am Kopf (Schlappohren)
    'schwanz': (43, 49),       # hinten rechts am Körper
}

def leer():
    return np.zeros((H, W), bool)

def ellipse(cx, cy, rx, ry):
    y, x = np.mgrid[0:H, 0:W]
    return ((x - cx) / rx) ** 2 + ((y - cy) / ry) ** 2 <= 1

def polygon(punkte):
    im = Image.new('L', (W, H), 0)
    ImageDraw.Draw(im).polygon([(int(round(px)), int(round(py))) for px, py in punkte], fill=255)
    return np.array(im) > 0

def linie(punkte, dicke=2):
    im = Image.new('L', (W, H), 0)
    ImageDraw.Draw(im).line([(int(round(px)), int(round(py))) for px, py in punkte], fill=255, width=dicke)
    return np.array(im) > 0

def spiegel(m):
    return m[:, ::-1]

def umriss(m):
    d = m.copy()
    d[1:, :] |= m[:-1, :]; d[:-1, :] |= m[1:, :]; d[:, 1:] |= m[:, :-1]; d[:, :-1] |= m[:, 1:]
    return d & ~m

class Bild:
    """Graustufen-Leinwand mit Alpha (16 Stufen)."""
    def __init__(self):
        self.g = np.zeros((H, W), np.uint8)
        self.a = np.zeros((H, W), bool)
    def setze(self, maske, wert):
        self.g[maske] = wert; self.a[maske] = True
    def form(self, maske, fuell, schatten=True, linie=LINIE):
        """Fläche + Umriss + leichte Schattierung unten rechts."""
        o = umriss(maske)
        self.setze(o, linie)
        self.setze(maske, fuell)
        if schatten:
            y, x = np.mgrid[0:H, 0:W]
            ys, xs = np.nonzero(maske)
            if len(ys):
                cy, cx = ys.mean(), xs.mean()
                innen = maske & ~erodiere(maske)
                dunkel = innen & (((x - cx) + (y - cy)) > 3)
                self.setze(dunkel, max(0, fuell - 34))
    def bild(self, hintergrund=255, z=4):
        a = np.full((H, W, 3), hintergrund, np.uint8)
        a[self.a] = np.stack([self.g[self.a]] * 3, 1)
        return Image.fromarray(a).resize((W * z, H * z), Image.NEAREST)

def erodiere(m):
    e = m.copy()
    e[1:, :] &= m[:-1, :]; e[:-1, :] &= m[1:, :]; e[:, 1:] &= m[:, :-1]; e[:, :-1] &= m[:, 1:]
    e[0, :] = e[-1, :] = False; e[:, 0] = e[:, -1] = False
    return e

# ---------- Teile (linke Seite; Koordinaten relativ zum Anker) ----------
def ohr(art, fell):
    ax, ay = ANKER['ohr']
    seite = ANKER['ohr-seite']
    if art == 'spitz':
        a = polygon([(ax - 5, ay + 4), (ax - 4, ay - 9), (ax + 5, ay + 2)])
        i = polygon([(ax - 3, ay + 2), (ax - 3, ay - 5), (ax + 2, ay + 1)])
    elif art == 'rund':
        a = ellipse(ax - 1, ay - 2, 5.5, 5.5); i = ellipse(ax - 1, ay - 2, 3, 3)
    elif art == 'lang':
        a = ellipse(ax - 2, ay - 8, 3.6, 10.5); i = ellipse(ax - 2, ay - 8, 1.6, 8)
    elif art == 'haengend':
        sx, sy = seite
        a = ellipse(sx - 3, sy + 5, 4.5, 9.5); i = leer()
    elif art == 'buschel':
        a = polygon([(ax - 5, ay + 4), (ax - 4, ay - 8), (ax + 5, ay + 2)])
        a |= polygon([(ax - 5, ay - 8), (ax - 4, ay - 11), (ax - 3, ay - 8)]) | polygon([(ax - 3, ay - 9), (ax - 2, ay - 12), (ax - 1, ay - 8)])
        i = polygon([(ax - 3, ay + 2), (ax - 3, ay - 4), (ax + 2, ay + 1)])
    else:
        return None
    return a, i

def schwanz(art):
    ax, ay = ANKER['schwanz']
    spitze = leer()
    if art == 'puschel':
        m = ellipse(ax + 6, ay - 1, 5, 5); spitze = leer()
    elif art == 'buschig':
        m = ellipse(ax + 8, ay - 5, 7, 12)
        y, x = np.mgrid[0:H, 0:W]
        spitze = m & (y < ay - 11)
    elif art == 'duenn':
        m = linie([(ax, ay + 3), (ax + 7, ay + 3), (ax + 11, ay - 2), (ax + 10, ay - 8)], 2) | ellipse(ax + 10, ay - 9, 2.5, 2.5)
        spitze = ellipse(ax + 10, ay - 9, 2.5, 2.5)
    elif art == 'blitz':
        m = polygon([(ax, ay + 2), (ax + 6, ay - 3), (ax + 4, ay - 4), (ax + 11, ay - 12), (ax + 9, ay - 6), (ax + 12, ay - 7), (ax + 5, ay + 3)])
        spitze = leer()
    else:
        return None, None
    return m, spitze

# ---------- Basis ----------
def basis_masken():
    kopf = ellipse(CX, 26, 17, 15)
    koerper = ellipse(CX, 47, 14, 12)
    fuesse = ellipse(24, 58, 5, 3) | ellipse(39, 58, 5, 3)
    return kopf, koerper, fuesse

def tiere(ohren='spitz', schwanz_art='puschel', fell=204, bauch=240, spitze=250, ohr_innen=119, muster='keins', gesicht='froh'):
    b = Bild()
    kopf, koerper, fuesse = basis_masken()
    # Schwanz (hinten)
    sm, sp = schwanz(schwanz_art)
    if sm is not None:
        b.form(sm, fell)
        if sp.any(): b.setze(sp & erodiere(sm), spitze)
    # Ohren (hinter dem Kopf)
    o = ohr(ohren, fell)
    if o is not None:
        for seite in (False, True):
            a, i = o
            a = spiegel(a) if seite else a
            i = spiegel(i) if seite else i
            b.form(a, fell)
            if i.any(): b.setze(i & erodiere(a), ohr_innen)
    # Füße, Körper, Kopf
    b.form(fuesse, fell)
    b.form(koerper, fell)
    bauchm = ellipse(CX, 49, 8, 8) & koerper
    b.setze(bauchm & erodiere(koerper), bauch)
    b.form(kopf, fell)
    # Muster auf dem Fell
    y, x = np.mgrid[0:H, 0:W]
    if muster == 'flecken':
        m = leer()
        for cx, cy, r in [(40, 18, 3), (24, 44, 3), (40, 48, 3.5), (30, 54, 2.2), (45, 40, 2)]:
            m |= ellipse(cx, cy, r, r)
        b.setze(m & koerper & ~bauchm | (m & kopf & (y < 22)), max(0, fell - 85))
    elif muster == 'streifen':
        m = ((y - x // 2) % 6 < 2) & (koerper | (kopf & (y < 18))) & ~bauchm
        b.setze(m, max(0, fell - 51))
    # Gesicht (immer gleich, nur Basis)
    for ex in (24, 37):
        b.setze(ellipse(ex + .5, 27, 2.2, 2.8), 17)
        b.setze(np.isin(x, [ex, ex + 1]) & (y == 26) , 255)
    b.setze(linie([(29, 32), (31, 34), (34, 34), (36, 32)], 1), 34) if gesicht == 'froh' else None
    b.setze(ellipse(19.5, 32, 2.5, 1.6) | ellipse(44.5, 32, 2.5, 1.6), 153)
    return b

def sheet():
    ohren = ['spitz', 'rund', 'lang', 'haengend', 'buschel']
    schw = ['puschel', 'buschig', 'duenn', 'blitz']
    teile = []
    # Zeile 1: Katalog der Teile (auf Basis ohne weitere Teile)
    z1 = [tiere(o, 'puschel') for o in ohren] + [tiere('rund', s) for s in schw]
    # Zeile 2: Kombinationen mit Fellton und Muster
    z2 = [tiere('spitz', 'buschig', 204, muster='flecken'), tiere('lang', 'puschel', 238, muster='keins'),
          tiere('haengend', 'duenn', 187, muster='flecken'), tiere('rund', 'blitz', 170, muster='streifen'),
          tiere('buschel', 'buschig', 153, spitze=238), tiere('spitz', 'duenn', 221, muster='streifen'),
          tiere('lang', 'buschig', 136, bauch=221), tiere('haengend', 'puschel', 255, muster='flecken'),
          tiere('rund', 'puschel', 119, bauch=187), tiere('buschel', 'blitz', 204)]
    z = 4
    zeilen = []
    for reihe, hg in ((z1, 255), (z2, 255), (z2[:5], 90)):
        bilder = [t.bild(hg, z) for t in reihe]
        zeilen.append(bilder)
    breite = max(len(r) for r in zeilen) * W * z
    out = Image.new('RGB', (breite, H * z * len(zeilen) + 60), 'white')
    d = ImageDraw.Draw(out)
    titel = ['Teile: 5 Ohren (rund, Puschelschwanz) und 4 Schwänze (runde Ohren)', 'Zehn Kombinationen mit Fellton, Bauch, Muster', 'Auf dunklem Grund (Kontrolle der Umrisse)']
    y0 = 0
    for k, reihe in enumerate(zeilen):
        d.text((6, y0 + 3), titel[k], fill='black'); y0 += 16
        for i, im in enumerate(reihe):
            out.paste(im, (i * W * z, y0))
        y0 += H * z
    return out, z2

if __name__ == '__main__':
    out, z2 = sheet()
    hier = os.path.dirname(os.path.abspath(__file__))
    # Originalgröße unten anhängen
    klein = Image.new('RGB', (out.width, 76), 'white')
    d = ImageDraw.Draw(klein)
    d.text((6, 2), 'Originalgröße (1x, 64 x 64):', fill='black')
    for i, t in enumerate(z2):
        klein.paste(t.bild(255, 1), (6 + i * 68, 12))
    ges = Image.new('RGB', (out.width, out.height + 76), 'white')
    ges.paste(out, (0, 0)); ges.paste(klein, (0, out.height))
    ges.save(os.path.join(hier, 'pruefbogen-test.png'))
    print('pruefbogen-test.png', ges.size)
