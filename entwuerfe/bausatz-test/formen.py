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
KOPF_UEBER_STANDARD = -3     # Kopf-Hals liegt so viele Pixel ÜBER dem Körper-Hals (negativ = darunter)
KRUSE_TIEFER_STANDARD = 3   # Halskrause sitzt so viele Pixel unter dem Kopf-Hals: der Kopf sitzt am Hals, nicht auf der Krause
PANZER_WINKEL = 30       # Grad, nach rechts unten gekippt
PANZER_VERSATZ = (-14, 8)  # Verschiebung gegenüber dem Rückenansatz
KRUSE_TIEFER = {}
KOPF_TIEFER = {}   # Platz für kopfspezifische Absenkung (der spitze Kopf wurde am 27.11. entfernt)

def datei(art, form, bevorzugt=None):
    if art == 'koerper' and form == 'sitzend': return TEILE / 'koerper-sitzend-2.png'
    if art == 'kopf' and form == '3-4': return TEILE / 'kopf-3-4-2.png'
    d = sorted(TEILE.glob(f'{art}-{form}-[123].png'))
    return d[0]

def raster(p): return np.array(Image.open(p).convert('RGB'))[::Z, ::Z, 0]

def baue_form(koerper='sitzend', kopf='3-4', kruse=2, ohren='spitz', schwanz='buschig', gesicht='freundlich',
              fell=204, kranz=238, muster=None, kopf_ueber=None, merkmale=(), kruse_tiefer=None, kruse_hinten=True):
    kf, hf = L.KOERPER_FORMEN[koerper], L.KOPF_FORMEN[kopf]
    khx, khy = hf['hals']
    if kopf_ueber is None:
        kopf_ueber = KOPF_UEBER_STANDARD - KOPF_TIEFER.get(kopf, 0)   # negativ: Kopf sitzt im Körperhals, Kinn berührt die Krause
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
    if 'maehne' in merkmale: merk('maehne', kopfmitte)                              # hinter dem Kopf
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
    if kruse_tiefer is None: kruse_tiefer = KRUSE_TIEFER.get(kopf, KRUSE_TIEFER_STANDARD)
    def zeichne_kruse():
        c.teil(raster(TEILE / f'kruse-flauschig-{kruse}.png'), (28, 2), (kx, ky + kruse_tiefer), {34: 34, 238: kranz, 204: kranz - 34, 170: kranz - 68})
    if kruse_hinten: zeichne_kruse()                                                # Krause hinter dem Kinn: der Kopf sitzt am Hals
    c.teil(raster(datei('kopf', kopf)), (khx, khy), (kx, ky), r)
    if muster:
        m, d = muster_maske(muster)
        ok = m & c.a & (c.g >= 85) & (c.g < 250)
        c.g[ok] = np.clip(np.round((c.g[ok] + d) / 17) * 17, 0, 255)
    if not kruse_hinten: zeichne_kruse()
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
