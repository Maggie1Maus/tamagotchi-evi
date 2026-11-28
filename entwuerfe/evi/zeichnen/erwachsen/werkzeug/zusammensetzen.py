#!/usr/bin/env python3
"""Setzt Evi (und andere Tamagotchis) aus Teilen zusammen. Python-Fassung von zusammensetzen.cs
(gleiche Farben und Schatten), zusaetzlich: Muster, Ohren mit beiden Seiten.
Aufruf als Modul: from zusammensetzen import bauen, pixel
Benoetigt: numpy, scipy, pillow."""
import os, numpy as np
from PIL import Image
from scipy import ndimage as ndi

N = 2048
LIN, FELL, KOPF, KRAUSE, INN = 51., 204., 221., 244., 85.
W = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))

def lesen(f):
    a = np.array(Image.open(f).convert('RGBA'))
    return a[..., :3].mean(-1), a[..., 3]

class Ebene:
    def __init__(s):
        s.g = np.zeros((N, N), np.float32); s.a = np.zeros((N, N), np.float32)
        s.line = np.zeros((N, N), bool); s.shad = np.zeros((N, N), bool); s.cover = np.zeros((N, N), bool)

def dil(m, r): return ndi.maximum_filter(m.astype(np.uint8), size=2 * r + 1) > 0

def bauen_ebene(f, r, farbe, abdeckung=None, zu=None, fell=FELL):
    lum, al = lesen(f); l = Ebene()
    line = (al >= 90) & (lum < 140)
    flood = line.copy()
    for x0, y0, x1, y1 in (zu or []):
        n = max(abs(x1 - x0), abs(y1 - y0))
        for i in range(n + 1):
            x = x0 + (x1 - x0) * i // n; y = y0 + (y1 - y0) * i // n
            flood[max(0, y - 3):y + 4, max(0, x - 3):x + 4] = True
    ld = dil(flood, r)
    lab, n = ndi.label(~ld)
    rand = np.unique(np.concatenate([lab[0], lab[-1], lab[:, 0], lab[:, -1]]))
    lab[np.isin(lab, rand[rand > 0])] = 0
    ids = [i for i in np.unique(lab) if i > 0]
    size = {i: int((lab == i).sum()) for i in ids}
    order = sorted(ids, key=lambda i: -size[i])
    rank = {i: k for k, i in enumerate(order)}
    for _ in range(r):                         # Regionen bis an die Linie wachsen lassen
        gr = ndi.maximum_filter(lab, size=3)
        lab = np.where(lab == 0, gr, lab)
    g = np.zeros((N, N), np.float32); a = np.zeros((N, N), np.float32)
    fill = np.zeros((N, N), bool)
    for i in ids:
        if size[i] < 40: continue
        c = farbe(rank[i])
        if c < 0: continue
        m = lab == i; g[m] = c; a[m] = 1; fill |= m
    if abdeckung is not None:
        m = (al > 0) & ~(lum < 140) & ~fill
        g[m] = abdeckung; a[m] = 1; fill |= m
    l.shad = fill & (g >= 100)
    dk = (lum < 140) & (al > 0)
    la = al / 255.0
    na = la + a * (1 - la)
    with np.errstate(invalid='ignore', divide='ignore'):
        gl = np.where(na > 0, (LIN * la + g * a * (1 - la)) / na, 0)
    g = np.where(dk, gl, g); a = np.where(dk, na, a)
    l.line = dk; l.g = g.astype(np.float32); l.a = a.astype(np.float32); l.cover = a > 0.5
    return l

def eigen(f, dunkel=34.):
    lum, al = lesen(f); l = Ebene()
    g = np.where(lum < 70, dunkel, np.round(lum / 17.0) * 17)
    m = al > 0
    l.g = np.where(m, g, 0).astype(np.float32); l.a = (al / 255.0).astype(np.float32); l.cover = al > 128
    return l

def schatten(l, d, amp, s, tip=1.0):
    k = np.where(l.g >= 250, tip, 1.0)
    m = l.shad & (d <= s * 5)
    l.g = np.where(m, np.maximum(110, l.g - amp * k * np.exp(-d / s)), l.g).astype(np.float32)

def dt(m): return ndi.distance_transform_edt(~m).astype(np.float32)
def rand_(l, amp, s): schatten(l, dt(l.line), amp, s, 0.35)

def drauf(g, a, l, spiegeln=False, achse=0):
    L_g, L_a = l.g, l.a
    if spiegeln:
        sx = 2 * achse - np.arange(N); ok = (sx >= 0) & (sx < N); sx = np.clip(sx, 0, N - 1)
        L_g = np.where(ok[None, :], L_g[:, sx], 0); L_a = np.where(ok[None, :], L_a[:, sx], 0)
    oa = L_a + a * (1 - L_a)
    with np.errstate(invalid='ignore', divide='ignore'):
        ng = np.where(oa > 0, (L_g * L_a + g * a * (1 - L_a)) / oa, g)
    return np.where(L_a > 0, ng, g).astype(np.float32), np.where(L_a > 0, oa, a).astype(np.float32)

def muster_ebene(f):
    e = eigen(f, LIN); return e

def bauen(gesicht, ohr, schwanz, kopf='kopf-rund', koerper='koerper-sitzend', hals='krause-evi',
          muster_koerper=None, muster_gesicht=None, kopfschmuck=None, pfoten=None, ansicht='frontal', fell=FELL, ausgabe=None):
    """Alle Angaben sind Dateinamen ohne .png. Gibt (grau, alpha) zurueck, speichert RGBA wenn ausgabe gesetzt."""
    P = lambda t, a, n: os.path.join(W, t, a, n + '.png')
    kopfton = fell + 17
    sw = bauen_ebene(P('schwaenze', ansicht, schwanz), 4, lambda rk: fell if rk != 1 else (255. if 'buschel' in schwanz else fell), fell=fell)
    lo, ao = lesen(P('ohren', ansicht, ohr))
    beide = False
    kp = bauen_ebene(P('koepfe', ansicht, kopf), 6, lambda rk: kopfton if rk == 0 else -1.)
    kb = bauen_ebene(P('koerper', ansicht, koerper), 5, lambda rk: fell,  # neuer Koerper hat keine Luecke zwischen den Vorderbeinen mehr
                     zu=[(814, 1145, 1096, 1134)])
    kr = bauen_ebene(P('halsschmuck', ansicht, hals), 4, lambda rk: -1., abdeckung=KRAUSE)
    ge = eigen(P('gesichter', ansicht, gesicht))
    ys, xs = np.nonzero(kp.cover)
    cx, cy, R = (xs.min() + xs.max()) / 2, (ys.min() + ys.max()) / 2, (xs.max() - xs.min()) / 2
    axis = int(round(cx))
    cols = np.nonzero(ao > 60)[1]
    beide = cols.min() < axis - 60 and cols.max() > axis + 60
    if beide:
        oh = bauen_ebene(P('ohren', ansicht, ohr), 4, lambda rk: fell, abdeckung=fell)
    else:
        oh = bauen_ebene(P('ohren', ansicht, ohr), 4, lambda rk: fell if rk != 1 else INN)
    yy, xx = np.mgrid[0:N, 0:N]
    t = ((xx - cx) + (yy - cy)) / (2 * R)
    kp.g = np.where(kp.shad, kp.g - 16 * t, kp.g).astype(np.float32)
    rand_(kp, 24, 30)
    dH, dK, dB = dt(kp.cover), dt(kr.cover), dt(kb.cover)
    rand_(kb, 20, 26); schatten(kb, dH, 28, 55, 1); schatten(kb, dK, 26, 45, 1)
    rand_(oh, 20, 24); schatten(oh, dH, 22, 45, 1)
    rand_(sw, 18, 30); schatten(sw, dB, 30, 75, 0.2)
    rand_(kr, 9, 18); schatten(kr, dH, 32, 55, 1)
    # Reihenfolge von hinten nach vorn. Endet ein Dateiname auf __vorne oder __hinten,
    # liegt das Teil ganz vorn bzw. ganz hinten, egal was sonst die Regel waere.
    teile = [('schwanz', schwanz, sw, False), ('ohr', ohr, oh, False)]
    if not beide: teile.append(('ohr', ohr, oh, True))
    teile.append(('koerper', koerper, kb, False))
    if muster_koerper:
        m = muster_ebene(P('muster', ansicht, muster_koerper)); m.a = m.a * (kb.cover | (kb.a > 0))
        teile.append(('m', muster_koerper, m, False))
    teile.append(('kopf', kopf, kp, False))
    if muster_gesicht:
        m = muster_ebene(P('muster', ansicht, muster_gesicht)); m.a = m.a * kp.cover
        teile.append(('m', muster_gesicht, m, False))
    teile.append(('hals', hals, kr, False))
    if kopfschmuck:
        ks = bauen_ebene(P('kopfschmuck', ansicht, kopfschmuck), 4, lambda rk: fell)
        rand_(ks, 14, 16); schatten(ks, dH, 20, 35, 1)
        teile.append(('kopfschmuck', kopfschmuck, ks, False))
    teile.append(('gesicht', gesicht, ge, False))
    if pfoten:
        pf = bauen_ebene(P('pfoten', ansicht, pfoten), 4, lambda rk: fell)
        rand_(pf, 16, 20)
        teile.append(('pfoten', pfoten, pf, False))
    hinten = [t for t in teile if t[1].endswith('__hinten')]
    vorne = [t for t in teile if t[1].endswith('__vorne')]
    mitte = [t for t in teile if t not in hinten and t not in vorne]
    auf = [t for t in mitte if t[1].endswith('__aufkoerper')]      # direkt ueber dem Koerper, unter Kopf und Krause
    mitte = [t for t in mitte if t not in auf]
    i = next(k for k, t in enumerate(mitte) if t[0] == 'koerper') + 1
    mitte = mitte[:i] + auf + mitte[i:]
    g = np.zeros((N, N), np.float32); a = np.zeros((N, N), np.float32)
    for _, _, ebene, spiegeln in hinten + mitte + vorne:
        g, a = drauf(g, a, ebene, spiegeln, axis)
    if ausgabe:
        v = np.clip(np.round(g), 0, 255).astype(np.uint8)
        Image.fromarray(np.dstack([v, v, v, np.round(a * 255).astype(np.uint8)]), 'RGBA').save(ausgabe)
    return g, a

def pixel(g, a, box=None, Wd=114, Ht=116):
    """Rechnet auf Evis Pixelraster herunter (wie pixelfassung.cs). box=(minx,miny,maxx,maxy) fuer gleichen Massstab."""
    al = np.round(a * 255)
    if box is None:
        ys, xs = np.nonzero(al > 60); box = (xs.min(), ys.min(), xs.max(), ys.max())
    minx, miny, maxx, maxy = box
    w, h = maxx - minx + 1, maxy - miny + 1
    cell = max(h / (Ht - 2), w / (Wd - 2))
    ox = minx - (Wd * cell - w) / 2.0; oy = maxy + 1 - Ht * cell + cell
    out = np.zeros((Ht, Wd, 4), np.uint8)
    for cy in range(Ht):
        for cx in range(Wd):
            x0, x1 = int(np.floor(ox + cx * cell)), int(np.floor(ox + (cx + 1) * cell))
            y0, y1 = int(np.floor(oy + cy * cell)), int(np.floor(oy + (cy + 1) * cell))
            x1 = max(x1, x0 + 1); y1 = max(y1, y0 + 1)
            tot = (x1 - x0) * (y1 - y0)
            xa, xb, ya, yb = max(x0, 0), min(x1, N), max(y0, 0), min(y1, N)
            if xb <= xa or yb <= ya: continue
            A = al[ya:yb, xa:xb]; G = g[ya:yb, xa:xb]
            op = A >= 128; n_op = op.sum()
            if n_op * 2 < tot: continue
            dark = (op & (G <= 70)).sum()
            if dark * 100 >= tot * 30: v = 51
            elif n_op - dark > 0: v = int(round(G[op & (G > 70)].mean() / 17.0)) * 17
            else: v = 51
            v = min(v, 255); out[cy, cx] = (v, v, v, 255)
    return Image.fromarray(out, 'RGBA')


def raster(g, a, cell, ox, oy, Wd, Ht):
    """Wie pixel(), aber mit vorgegebenem Raster (gleicher Massstab fuer alle Posen)."""
    al = np.round(a * 255)
    out = np.zeros((Ht, Wd, 4), np.uint8)
    for cy in range(Ht):
        for cx in range(Wd):
            x0, x1 = int(np.floor(ox + cx * cell)), int(np.floor(ox + (cx + 1) * cell))
            y0, y1 = int(np.floor(oy + cy * cell)), int(np.floor(oy + (cy + 1) * cell))
            x1 = max(x1, x0 + 1); y1 = max(y1, y0 + 1)
            tot = (x1 - x0) * (y1 - y0)
            xa, xb, ya, yb = max(x0, 0), min(x1, N), max(y0, 0), min(y1, N)
            if xb <= xa or yb <= ya: continue
            A = al[ya:yb, xa:xb]; G = g[ya:yb, xa:xb]
            op = A >= 128; n_op = op.sum()
            if n_op * 2 < tot: continue
            dark = (op & (G <= 70)).sum()
            if dark * 100 >= tot * 30: v = 51
            elif n_op - dark > 0: v = int(round(G[op & (G > 70)].mean() / 17.0)) * 17
            else: v = 51
            v = min(v, 255); out[cy, cx] = (v, v, v, 255)
    return out

def standard_raster(g, a, Wd=114, Ht=116):
    """Cell/Ursprung der normalen Evi (wie pixel())."""
    al = np.round(a * 255); ys, xs = np.nonzero(al > 60)
    minx, miny, maxx, maxy = xs.min(), ys.min(), xs.max(), ys.max()
    w, h = maxx - minx + 1, maxy - miny + 1
    cell = max(h / (Ht - 2), w / (Wd - 2))
    return cell, minx - (Wd * cell - w) / 2.0, maxy + 1 - Ht * cell + cell
