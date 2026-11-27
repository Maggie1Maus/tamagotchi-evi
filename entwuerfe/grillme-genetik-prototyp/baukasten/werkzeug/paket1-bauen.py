#!/usr/bin/env python3
"""Paket 1 bauen (Mia, Glöckchen, Lotti, Klecksi, Funke; Erwachsen, beide Geschlechter).

Python-Nachbau der Schritte C-E aus BuildParts.cs, nur für die neuen Teile.
Evis Basis und die Teile des Probelaufs werden NICHT verändert.
Aufruf (aus beliebigem Ordner):  python3 paket1-bauen.py
"""
import os, sys
import numpy as np
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from paket1_lib import *

FIGUREN = ['mia', 'gloeckchen', 'lotti', 'klecksi', 'funke']
SCHLAPP = {'gloeckchen', 'klecksi'}  # hängende Ohren: dürfen über die Wangen fallen


def maske_ohr(w, h):
    y, x = np.mgrid[0:h, 0:w]
    return (y < h * .34) | ((y < h * .48) & ((x < w * .38) | (x > w * .62)))

def maske_schlapp(w, h):
    y, x = np.mgrid[0:h, 0:w]
    return y < h * .64  # ganze obere Hälfte; Gesichtslinien werden unten ausgenommen

def maske_schwanz(w, h):
    y, x = np.mgrid[0:h, 0:w]
    return (y > h * .34) & (y < h * .98) & (x > np.where(y < h * .68, w * .72, w * .68))

def region(a, art, h):
    """Bereichswerte wie Region() in BuildParts.cs."""
    g = grau(a)
    out = np.zeros(g.shape, np.uint8)
    y = np.arange(a.shape[0])[:, None] * np.ones((1, a.shape[1]), int)
    r = np.full(g.shape, 51, np.uint8)
    if art == 'schwanz':
        r[(g >= 220) & (y < h * .72)] = 68
    if art == 'ohren':
        r[(g >= 52) & (g < 125) & (y < h * .49)] = 85
    r[g <= 70] = 17
    out[a[..., 3] > 0] = r[a[..., 3] > 0]
    return out

def karte(teil, art):
    h, w = teil.shape[:2]
    r = region(teil, art, h)
    k = leer(w, h)
    for v in np.unique(r[r > 0]):
        k[r == v] = (v, v, v, 255)
    return k

def entspeckeln(a, minimum):
    """Kleine abgetrennte Pixelgruppen entfernen (8er-Nachbarschaft)."""
    h, w = a.shape[:2]
    seen = np.zeros((h, w), bool)
    for sy in range(h):
        for sx in range(w):
            if seen[sy, sx] or a[sy, sx, 3] == 0:
                continue
            gruppe = [(sy, sx)]; seen[sy, sx] = True; i = 0
            while i < len(gruppe):
                y, x = gruppe[i]; i += 1
                for dy in (-1, 0, 1):
                    for dx in (-1, 0, 1):
                        ny, nx = y + dy, x + dx
                        if 0 <= ny < h and 0 <= nx < w and not seen[ny, nx] and a[ny, nx, 3] > 0:
                            seen[ny, nx] = True; gruppe.append((ny, nx))
            if len(gruppe) < minimum:
                for y, x in gruppe:
                    a[y, x] = 0
    return a

def teil_aus(variante, basis_ki, erlaubt, schwelle=34):
    """Schritt D/E: nur Pixel in der erlaubten Zone, die sich von der Basis unterscheiden."""
    a = basis_ki
    diff = (variante[..., 3] > 0) & ((a[..., 3] == 0) | (abs(variante[..., 0].astype(int) - a[..., 0]) >= schwelle))
    t = leer(variante.shape[1], variante.shape[0])
    m = diff & erlaubt
    t[m] = variante[m]
    return t


def bauen(sex):
    w = 114 if sex == 'weiblich' else 120
    h = 116
    dir_ = os.path.join(WURZEL, 'teile', sex, 'erwachsen')
    ki = os.path.join(HIER, 'ki')
    ai = runter(os.path.join(HIER, f'basis-{sex}-ki.png'), w, h)
    basis = lade_rgba(os.path.join(dir_, 'basis.png'))
    bkarte = lade_rgba(os.path.join(dir_, 'basis.bereiche.png'))
    gesicht = (bkarte[..., 0] == 34) & (bkarte[..., 3] > 0)
    for fig in FIGUREN:
        for art, erlaubt in (('ohren', maske_schlapp(w, h) if fig in SCHLAPP else maske_ohr(w, h)),
                             ('schwanz', maske_schwanz(w, h))):
            var = runter(os.path.join(ki, f'{sex}-{art}-{fig}.png'), w, h)
            if art == 'ohren' and fig in SCHLAPP:
                erlaubt = erlaubt & ~gesicht  # nie über Augen, Mund, Wangenlinie
            t = teil_aus(var, ai, erlaubt)
            if art == 'ohren':
                t = entspeckeln(t, 6)
            speichern(t, os.path.join(dir_, art, fig + '.png'))
            speichern(karte(t, art), os.path.join(dir_, art, fig + '.bereiche.png'))
            print(sex, art, fig, int((t[..., 3] > 0).sum()), 'Pixel')

    y, x = np.mgrid[0:h, 0:w]
    # Halsband: schmale Zone am Hals
    var = runter(os.path.join(ki, f'{sex}-zubehoer-halsband.png'), w, h)
    t = teil_aus(var, ai, (y > h * .46) & (y < h * .72), 34)
    t = entspeckeln(t, 4)
    speichern(t, os.path.join(dir_, 'zubehoer', 'halsband.png'))
    speichern(karte(t, 'zubehoer'), os.path.join(dir_, 'zubehoer', 'halsband.bereiche.png'))
    print(sex, 'halsband', int((t[..., 3] > 0).sum()))
    # Backenblitze: nur Wangen
    var = runter(os.path.join(ki, f'{sex}-besonderheit-backenblitze.png'), w, h)
    t = teil_aus(var, ai, (y > h * .30) & (y < h * .60), 34)
    speichern(t, os.path.join(dir_, 'besonderheit', 'backenblitze-vorn.png'))
    speichern(karte(t, 'besonderheit'), os.path.join(dir_, 'besonderheit', 'backenblitze-vorn.bereiche.png'))
    speichern(leer(w, h), os.path.join(dir_, 'besonderheit', 'backenblitze-hinten.png'))
    print(sex, 'backenblitze', int((t[..., 3] > 0).sum()))

    # Muster (nur auf Fell = Bereich 51)
    fell = (bkarte[..., 0] == 51) & (bkarte[..., 3] > 0) & (basis[..., 3] > 0)
    female = sex == 'weiblich'
    muster = {}
    # kitz-tupfen: Reihen heller 2x2-Tupfen auf Flanke und Rücken (versetzt)
    m = np.zeros((h, w), bool)
    for ry, zeile in enumerate(range(74, 104, 5)):
        for cx in range(12, w - 8, 6):
            cx2 = cx + (3 if ry % 2 else 0)
            m[zeile:zeile + 2, cx2:cx2 + 2] = True
    zone = (y > 72) & (y < 104)
    muster['kitz-tupfen'] = (m & zone & fell, 255)
    # flecken (Dalmatiner): unregelmäßig, kräftig, auch Kopf-Rand und Flanken
    rng = np.random.RandomState(11 if female else 12)
    m = np.zeros((h, w), bool)
    zentren = [(29, 87, 5.5, 4.5), (35, 96, 3.5, 3.2), (82, 85, 4.8, 5.2), (76, 95, 3.6, 3.0),
               (24, 78, 3.0, 3.4), (88, 99, 3.0, 2.6), (42, 70, 2.4, 2.2), (66, 74, 2.2, 2.4)] if female else \
              [(31, 83, 5.5, 4.5), (37, 92, 3.5, 3.2), (93, 82, 4.8, 5.2), (86, 93, 3.6, 3.0),
               (26, 74, 3.0, 3.4), (96, 97, 3.0, 2.6), (47, 68, 2.4, 2.2), (72, 71, 2.2, 2.4)]
    for k, (cx, cy, rx, ry) in enumerate(zentren):
        warp = .22 * np.sin(x * .83 + y * .51 + k * 2.3)
        m |= (((x - cx) / rx) ** 2 + ((y - cy) / ry) ** 2 + warp) < 1
    muster['flecken'] = (m & fell & (y > 66), 255)
    # augenfleck: dunkel um das rechte Auge (Betrachter), Augenlinien (Bereich 34) bleiben
    ex, ey = (64, 52) if female else (77, 50)
    m = (((x - ex) / 9.5) ** 2 + ((y - ey) / 8.5) ** 2) <= 1
    muster['augenfleck'] = (m & fell & (y > 38) & (y < 66), 255)
    for name, (mm, st) in muster.items():
        t = leer(w, h)
        t[mm] = (st, st, st, 255)
        speichern(t, os.path.join(dir_, 'muster', name + '.png'))
        k = leer(w, h)
        k[mm] = (51, 51, 51, 255)
        speichern(k, os.path.join(dir_, 'muster', name + '.bereiche.png'))
        print(sex, 'muster', name, int(mm.sum()))


if __name__ == '__main__':
    bauen('weiblich')
    bauen('maennlich')
