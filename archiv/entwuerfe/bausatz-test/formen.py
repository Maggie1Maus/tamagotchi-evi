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
MAEHNE_RAND = (10, 9)        # Die Mähne wird auf eine Ellipse um den Kopf beschnitten (Kopfbreite/2 + 10, Kopfhöhe/2 + 9)
KOPF_UEBER_STANDARD = -5     # Kopf-Hals liegt so viele Pixel ÜBER dem Körper-Hals (negativ = darunter)
PANZER_WINKEL = 30       # Grad, nach rechts unten gekippt
PANZER_VERSATZ = (-14, 8)  # Verschiebung gegenüber dem Rückenansatz
KOPF_TIEFER = {}   # Platz für kopfspezifische Absenkung (der spitze Kopf wurde am 27.11. entfernt)

def datei(art, form, bevorzugt=None):
    if art == 'koerper' and form == 'sitzend': return TEILE / 'koerper-sitzend-2.png'
    if art == 'kopf' and form == '3-4': return TEILE / 'kopf-3-4-2.png'
    d = sorted(TEILE.glob(f'{art}-{form}-[123].png'))
    return d[0]

def raster(p): return np.array(Image.open(p).convert('RGB'))[::Z, ::Z, 0]

def muster_relativ(art, kopfmaske, koerpermaske, kopfbox, mx, my):
    """Muster passend zu Kopf und Körper (nicht mehr feste Leinwandpositionen): Flecken, Streifen, Tupfen, Augenfleck."""
    y, x = np.mgrid[0:H, 0:W]
    hx0, hx1, hy0, hy1 = kopfbox
    gesichtszone = (np.abs(x - mx) <= 16) & (y >= my - 12) & (y <= my + 14)
    flaeche = (kopfmaske | koerpermaske) & ~gesichtszone
    if art == 'flecken':
        rng = np.random.RandomState(7)
        ys, xs = np.nonzero(koerpermaske & ~kopfmaske)
        m = np.zeros((H, W), bool)
        hoehe = ys.max() - ys.min() + 1 if len(ys) else 40
        skala = max(0.6, min(1.1, hoehe / 46))
        for k in range(8):
            i = rng.randint(len(ys)); rr = rng.uniform(2.4, 4.6) * skala
            m |= ((x - xs[i]) / rr) ** 2 + ((y - ys[i]) / (rr * .85)) ** 2 + .15 * np.sin(x * .9 + y * .6 + k) <= 1
        for fx, fy, rr in ((0.25, 0.22, 3.2), (0.8, 0.3, 2.8)):
            cx_, cy_ = hx0 + fx * (hx1 - hx0), hy0 + fy * (hy1 - hy0)
            m |= ((x - cx_) / rr) ** 2 + ((y - cy_) / (rr * .85)) ** 2 <= 1
        return m & flaeche, -68
    if art == 'streifen':
        return ((y - x // 2) % 9 < 3) & flaeche & ((y > my + 6) | koerpermaske), -51
    if art == 'tupfen':
        return (x % 7 < 2) & (y % 6 < 2) & (koerpermaske & ~kopfmaske), 34
    if art == 'augenfleck':
        return (((x - (mx + 11)) / 9) ** 2 + ((y - (my - 3)) / 8) ** 2 <= 1) & kopfmaske, -68
    return None, 0

def baue_form(koerper='sitzend', kopf='3-4', ohren='spitz', schwanz='buschig', gesicht='freundlich',
              fell=204, muster=None, kopf_ueber=None, merkmale=(), aufsaetze=()):
    kf, hf = L.KOERPER_FORMEN[koerper], L.KOPF_FORMEN[kopf]
    khx, khy = hf['hals']
    if kopf_ueber is None:
        kopf_ueber = KOPF_UEBER_STANDARD - KOPF_TIEFER.get(kopf, 0)   # negativ: Kopf sitzt im Körperhals
    gw, gh = kf['groesse']; hx, hy = kf['hals']
    zx, zy = 56, BODEN - (gh - 1) + hy                  # Körper-Hals auf der Leinwand
    kx, ky = 56, zy - kopf_ueber                         # Kopf-Hals
    c = Leinwand(); r = rollen(fell)
    kr = raster(datei('koerper', koerper))
    spalte = min(hx + L.RUECKEN_VERSATZ, kr.shape[1] - 1)
    oben = int(np.nonzero(kr[:, spalte] != 255)[0].min())
    rueck = (zx + spalte - hx, zy + oben - hy)            # Rückenansatz auf der Leinwand
    def merk(name, ziel, rl=None):
        t = lade_teil('merkmal', name)
        if t is not None: c.teil(t[0], t[1], ziel, rl or r)
    # Körper-Silhouette auf der Leinwand (für Stacheln und Panzer)
    maske = np.zeros((H, W), bool)
    ys_, xs_ = np.nonzero(kr != 255)
    maske[np.clip(ys_ - hy + zy, 0, H - 1), np.clip(xs_ - hx + zx, 0, W - 1)] = True
    oberkante = np.array([(int(np.nonzero(kr[:, x] != 255)[0].min()) if (kr[:, x] != 255).any() else -1) for x in range(kr.shape[1])])
    def stacheln():
        """Stachelband folgt der Oberkante von Kopf + Körper, vom Scheitel bis zum Hinterteil (liegt hinter beiden)."""
        t = lade_teil('merkmal', 'stacheln')
        if t is None: return
        ra, (ax, ay) = t
        uni = maske.copy()
        kraster = raster(datei('kopf', kopf))
        yk, xk = np.nonzero(kraster != 255)
        uni[np.clip(yk - khy + ky, 0, H - 1), np.clip(xk - khx + kx, 0, W - 1)] = True
        start = kx - 12 - ax                      # Spalte des Teils px=0
        for px in range(ra.shape[1]):
            cx = start + px
            if cx < 0 or cx >= W or not uni[:, cx].any(): continue
            ty = int(np.nonzero(uni[:, cx])[0].min())
            zeilen = np.nonzero(ra[:, px] != 255)[0]
            if len(zeilen) == 0: continue
            unten = int(zeilen.max())
            for py in zeilen:
                cy = ty + 1 - unten + py
                if 0 <= cy < H:
                    c.g[cy, cx] = r.get(int(ra[py, px]), int(ra[py, px])); c.a[cy, cx] = True
    def panzer():
        """Panzer liegt schräg auf dem Rücken (um PANZER_WINKEL nach rechts unten gekippt, folgt der Rückenlinie)."""
        t = lade_teil('merkmal', 'panzer')
        if t is None: return
        ra, (ax, ay) = t
        bild = Image.fromarray(ra.astype(np.uint8), 'L').rotate(-PANZER_WINKEL, resample=Image.NEAREST, expand=True, fillcolor=255)
        rr = np.array(bild)
        # Rotierte Position des Ansatzpunktes (links unten) bestimmen
        h0, w0 = ra.shape; h1, w1 = rr.shape
        px, py = ax - w0 / 2, ay - h0 / 2
        a_ = np.deg2rad(PANZER_WINKEL)
        qx, qy = px * np.cos(a_) - py * np.sin(a_) + w1 / 2, px * np.sin(a_) + py * np.cos(a_) + h1 / 2
        ys2, xs2 = np.nonzero(rr != 255)
        ox, oy = rueck[0] + PANZER_VERSATZ[0], rueck[1] + PANZER_VERSATZ[1]
        for y2, x2 in zip(ys2, xs2):
            cx, cy = int(round(x2 - qx + ox)), int(round(y2 - qy + oy))
            if 0 <= cx < W and 0 <= cy < H:
                c.g[cy, cx] = r.get(int(rr[y2, x2]), int(rr[y2, x2])); c.a[cy, cx] = True
    for m in merkmale:
        if m == 'stacheln': stacheln()
        elif m in ('fluegel-fee', 'fluegel-drache'): merk(m, rueck)   # hinter dem Körper
    if schwanz:
        t = lade_teil('schwanz', schwanz)
        if t is not None:
            c.teil(t[0], t[1], (zx + kf['schwanz'][0] - hx, zy + kf['schwanz'][1] - hy), r)
    c.teil(kr, (hx, hy), (zx, zy), r)
    if 'panzer' in merkmale: panzer()                                              # vor dem Körper, auf die Körperform beschnitten
    khx, khy = hf['hals']
    kopfmitte = (kx + hf['gesicht'][0] - khx, ky + hf['gesicht'][1] - khy)
    khraster = raster(datei('kopf', kopf))
    ykh, xkh = np.nonzero(khraster != 255)
    kopfmaske = np.zeros((H, W), bool)
    kopfmaske[np.clip(ykh - khy + ky, 0, H - 1), np.clip(xkh - khx + kx, 0, W - 1)] = True
    hx0, hx1 = int(xkh.min() - khx + kx), int(xkh.max() - khx + kx)
    hy0, hy1 = int(ykh.min() - khy + ky), int(ykh.max() - khy + ky)
    if 'maehne' in merkmale:                                                         # hinter dem Kopf, auf Kopfgröße beschnitten
        t = lade_teil('merkmal', 'maehne')
        if t is not None:
            ra, (ax_, ay_) = t
            ccx, ccy = (hx0 + hx1) / 2, (hy0 + hy1) / 2
            rx, ry = (hx1 - hx0) / 2 + MAEHNE_RAND[0], (hy1 - hy0) / 2 + MAEHNE_RAND[1]
            ys5, xs5 = np.nonzero(ra != 255)
            for y5, x5 in zip(ys5, xs5):
                cx, cy = x5 - ax_ + kopfmitte[0], y5 - ay_ + kopfmitte[1]
                if 0 <= cx < W and 0 <= cy < H and ((cx - ccx) / rx) ** 2 + ((cy - ccy) / ry) ** 2 <= 1 and cy <= hy1 + 4:
                    if ra[y5, x5] == 34 and ((cx - ccx) / (rx - 3)) ** 2 + ((cy - ccy) / (ry - 3)) ** 2 <= 0.8: continue   # innere Umrisslinie der Mähne weglassen
                    c.g[cy, cx] = r.get(int(ra[y5, x5]), int(ra[y5, x5])); c.a[cy, cx] = True
            # Lücke zwischen Kopf und Mähnenring mit Mähnenfell füllen (sonst bleibt bei kleinen Köpfen ein leerer Ring)
            yy, xx = np.mgrid[0:H, 0:W]
            innen = (((xx - ccx) / (rx - 4)) ** 2 + ((yy - ccy) / (ry - 4)) ** 2 <= 1) & (yy <= hy1 + 3) & ~c.a
            c.g[innen] = np.where((xx[innen] + yy[innen]) % 2 == 0, r.get(204, 204), r.get(170, 170)); c.a[innen] = True
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
    geo = geometrie(koerper, kopf, kopf_ueber)
    mx, my = geo['mx'], geo['my']
    if muster:
        m, d = muster_relativ(muster, kopfmaske, maske, (hx0, hx1, hy0, hy1), mx, my)
        ok = m & c.a & (c.g >= 85) & (c.g < 250)
        c.g[ok] = np.clip(np.round((c.g[ok] + d) / 17) * 17, 0, 255)
    ohr_r = (kx + hf['ohr_r'][0] - khx, ky + hf['ohr_r'][1] - khy)
    def aufsatz(name, ziel, spiegeln=False, rl=None):
        t = lade_teil('aufsatz', name)
        if t is not None: c.teil(t[0], t[1], ziel, rl or {34: 34, 119: 119, 170: 170, 204: 204, 238: 238}, spiegeln=spiegeln)
    def aufsatz_im_kopf(name, ziel, spiegeln=False):
        t = lade_teil('aufsatz', name)
        if t is None: return
        tmp = Leinwand(); tmp.teil(t[0], t[1], ziel, {34: 34, 119: 119, 170: 170, 204: 204, 238: 238}, spiegeln=spiegeln)
        ok = tmp.a & kopfmaske
        c.g[ok] = tmp.g[ok]; c.a[ok] = True
    if 'augenmaske' in aufsaetze: aufsatz_im_kopf('augenmaske', (mx, my - 4))
    if gesicht:
        t = lade_teil('gesicht', gesicht)
        if t is not None:
            c.teil(t[0], t[1], (kx + hf['gesicht'][0] - khx, ky + hf['gesicht'][1] - khy), {34: 17, 119: 51, 170: 102, 204: 153, 238: 255})
    if 'backenblitz' in aufsaetze:
        aufsatz_im_kopf('backenblitz', (mx - 17, my + 6)); aufsatz_im_kopf('backenblitz', (W - 1 - (mx + 17), my + 6), spiegeln=True)
    # Unterkante des Kopfes (Kinn) an einer Spalte der Leinwand
    kraster2 = raster(datei('kopf', kopf))
    def kinn(x):
        sp = x - kx + khx
        if 0 <= sp < kraster2.shape[1] and (kraster2[:, sp] != 255).any():
            return ky + int(np.nonzero(kraster2[:, sp] != 255)[0].max()) - khy
        return ky + 3
    if 'halsband' in aufsaetze:
        t = lade_teil('aufsatz', 'halsband')
        if t is not None:
            ra, (ax_, ay_) = t
            ziel = (kx - 3, kinn(kx - 3) - 1)
            ys3, xs3 = np.nonzero(ra != 255)
            for y3, x3 in zip(ys3, xs3):
                cx, cy = x3 - ax_ + ziel[0], y3 - ay_ + ziel[1]
                # nur dort zeichnen, wo ein Körper (Hals, Schultern) ist: das Band endet am Halsrand
                if 0 <= cx < W and 0 <= cy < H and maske[cy, cx]:
                    c.g[cy, cx] = int(ra[y3, x3]); c.a[cy, cx] = True
    if 'schleife' in aufsaetze: aufsatz('schleife', (ohr_r[0] - 2, ohr_r[1] + 1))
    return c

def geometrie(koerper='sitzend', kopf='3-4', kopf_ueber=None):
    """Wichtige Punkte auf der Leinwand: Kopf-Hals (kx, ky) und Gesichtsmitte (mx, my)."""
    kf, hf = L.KOERPER_FORMEN[koerper], L.KOPF_FORMEN[kopf]
    if kopf_ueber is None: kopf_ueber = KOPF_UEBER_STANDARD - KOPF_TIEFER.get(kopf, 0)
    zy = BODEN - (kf['groesse'][1] - 1) + kf['hals'][1]
    kx, ky = 56, zy - kopf_ueber
    return dict(kx=kx, ky=ky, mx=kx + hf['gesicht'][0] - hf['hals'][0], my=ky + hf['gesicht'][1] - hf['hals'][1])

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
