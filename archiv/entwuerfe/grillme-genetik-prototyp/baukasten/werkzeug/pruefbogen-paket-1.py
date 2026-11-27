#!/usr/bin/env python3
"""Prüfbogen für Paket 1 -> pruefbogen-paket-1.png (Nachbau der Ebenenlogik aus app.js)."""
import os, sys, random
import numpy as np
from PIL import Image, ImageDraw
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from paket1_lib import *

TON = {'mia': 2, 'gloeckchen': 2, 'lotti': 2, 'klecksi': 1, 'funke': 2, 'evi': 2}
NAMEN = {'mia': 'Mia', 'gloeckchen': 'Glöckchen', 'lotti': 'Lotti', 'klecksi': 'Klecksi', 'funke': 'Funke', 'evi': 'Evi'}
KR = [255, 136, 119, 102]

def teil(sex, art, name):
    p = os.path.join(WURZEL, 'teile', sex, 'erwachsen', art, name + '.png')
    return lade_rgba(p) if os.path.exists(p) else None

def faerben(im, karte, ton, kranz):
    """Wie colored() in app.js."""
    o = im.copy()
    if karte is None:
        return o
    g = o[..., 0].astype(int)
    r = karte[..., 0].astype(int)
    a = o[..., 3]
    # Kranzton auf transparenten Kranzpixeln
    luecke = (a == 0) & (karte[..., 3] > 0) & (r == 68) & (kranz > 0)
    nxt = g.copy()
    m = np.isin(r, (51, 119, 85)); nxt[m] = g[m] + (2 - ton) * 17
    m = r == 68; nxt[m] = g[m] - (255 - KR[kranz])
    m = r == 85; nxt[m] = np.minimum(nxt[m], 204)
    nxt = np.clip(np.round(nxt / 17.0) * 17, 0, 255).astype(np.uint8)
    ch = (a > 0) & (nxt != g)
    o[ch, 0] = o[ch, 1] = o[ch, 2] = nxt[ch]
    o[luecke] = (KR[kranz], KR[kranz], KR[kranz], 255)
    return o

def lege(dst, src):
    m = src[..., 3] > 0
    dst[m] = src[m]

def figur(sex, ohren='evi', schwanz='evi', besond='keins', ton=2, kranz=0, muster='keins', zub='keins', struktur='punkte'):
    eigen = 'evi' if sex == 'weiblich' else 'evi-m'
    ohren = eigen if ohren == 'evi' else ohren
    schwanz = eigen if schwanz == 'evi' else schwanz
    basis = teil(sex, '.', 'basis') if False else lade_rgba(os.path.join(WURZEL, 'teile', sex, 'erwachsen', 'basis.png'))
    bk = lade_rgba(os.path.join(WURZEL, 'teile', sex, 'erwachsen', 'basis.bereiche.png'))
    h, w = basis.shape[:2]
    c = np.zeros((h, w, 4), np.uint8)
    def tint(art, name):
        im = teil(sex, art, name)
        if im is None:
            return None
        k = teil(sex, art, name + '.bereiche')
        return faerben(im, k, ton, kranz)
    if schwanz != eigen:
        t = tint('schwanz', schwanz)
        if t is not None: lege(c, t)
    lege(c, faerben(basis, bk, ton, kranz))
    s = tint('struktur', struktur)
    if s is not None: lege(c, s)
    if muster != 'keins':
        mk = teil(sex, 'muster', muster)
        if mk is not None:
            g = grau(c)
            shift = 68 if muster in ('kitz-tupfen', 'helle-brust') else -68
            ok = (mk[..., 3] > 0) & (c[..., 3] > 0) & (bk[..., 0] == 51)
            nv = np.clip(np.round((g + shift * mk[..., 0].astype(int) / 255) / 17.0) * 17, 0, 255).astype(np.uint8)
            c[ok, 0] = c[ok, 1] = c[ok, 2] = nv[ok]
    t = tint('ohren', ohren)
    if t is not None: lege(c, t)
    if schwanz == eigen:
        t = tint('schwanz', schwanz)
        if t is not None: lege(c, t)
    if besond != 'keins':
        t = teil(sex, 'besonderheit', besond + '-vorn')
        if t is not None: lege(c, t)
    if zub != 'keins':
        t = teil(sex, 'zubehoer', zub)
        if t is not None: lege(c, t)
    return c

def bild(c, bg, z=3):
    h, w = c.shape[:2]
    im = Image.new('RGB', (w, h), bg)
    im.paste(Image.fromarray(c), (0, 0), Image.fromarray(c))
    return im.resize((w * z, h * z), Image.NEAREST)

def park(fig, z=3):
    p = os.path.join(WURZEL, '..', '..', 'neue-tamagotchis', 'graustufen', fig, 'erwachsen-gluecklich.png')
    a = lade_rgba(p)
    im = Image.new('RGB', (a.shape[1], a.shape[0]), 'white')
    im.paste(Image.fromarray(a), (0, 0), Image.fromarray(a))
    return im.resize((a.shape[1] * z, a.shape[0] * z), Image.NEAREST)

FIG = ['mia', 'gloeckchen', 'lotti', 'klecksi', 'funke']
BESOND = {'funke': 'backenblitze'}
MUSTER = {'lotti': 'kitz-tupfen', 'klecksi': 'flecken'}
ZUB = {'klecksi': 'halsband'}

def zeile(ims, titel, breite, hoehe, z_label=14):
    """ims: Liste von (Bild, Beschriftung). Gibt ein Bild mit Titel zurück."""
    abst = 8
    W = sum(i.width for i, _ in ims) + abst * (len(ims) + 1)
    H = max(i.height for i, _ in ims) + 2 * z_label + 14
    out = Image.new('RGB', (W, H), 'white')
    d = ImageDraw.Draw(out)
    d.text((abst, 2), titel, fill='black')
    x = abst
    for i, t in ims:
        out.paste(i, (x, z_label + 4))
        d.text((x, z_label + 6 + i.height), t, fill='black')
        x += i.width + abst
    return out

zeilen = []
for f in FIG:
    ims = []
    for sex, kürzel in (('weiblich', 'Mädchen'), ('maennlich', 'Junge')):
        c = figur(sex, ohren=f, schwanz=f, besond=BESOND.get(f, 'keins'), ton=TON[f], muster=MUSTER.get(f, 'keins'), zub=ZUB.get(f, 'keins'))
        ims.append((bild(c, 'white'), f'{kürzel}, weiß'))
        ims.append((bild(c, (90, 90, 90)), f'{kürzel}, dunkelgrau'))
    ims.append((park(f), 'Parkbild'))
    zeilen.append(zeile(ims, NAMEN[f], 0, 0))

# neue Muster und Zubehör (auf Evis Teilen, Mädchen und Junge)
ims = []
for sex, k in (('weiblich', 'w'), ('maennlich', 'm')):
    for titel, kw in (('kitz-tupfen', dict(muster='kitz-tupfen')), ('flecken (Klecksi)', dict(muster='flecken')),
                      ('augenfleck', dict(muster='augenfleck')), ('halsband', dict(zub='halsband')),
                      ('backenblitze', dict(besond='backenblitze'))):
        ims.append((bild(figur(sex, **kw), 'white', 2), f'{titel} ({k})'))
zeilen.append(zeile(ims, 'Neue Muster und neues Zubehör (auf Evi-Basis)', 0, 0))

# sechs zufällige Kinder
random.seed(27)
kinder = [('weiblich', dict(ohren='funke', schwanz='evi', besond='backenblitze'), 'Evi × Funke (w)'),
          ('maennlich', dict(ohren='mia', schwanz='evi-m'), 'männl. Evi × Mia'),
          ('maennlich', dict(ohren='evi-m', schwanz='gloeckchen', zub='halsband'), 'männl. Evi × Glöckchen'),
          ('maennlich', dict(ohren='lotti', schwanz='lotti', muster='kitz-tupfen'), 'männl. Evi × Lotti'),
          ('maennlich', dict(ohren='klecksi', schwanz='funke', muster='augenfleck', zub='halsband'), 'männl. Evi × Klecksi/Funke'),
          ('weiblich', dict(ohren='lotti', schwanz='funke', besond='backenblitze', muster='flecken', ton=1), 'Lotti/Funke-Mix (w)')]
ims = [(bild(figur(s, **kw), 'white', 2), t) for s, kw, t in kinder]
zeilen.append(zeile(ims, 'Sechs Beispielkinder', 0, 0))

# Kranzton 0–3 für Lotti und Funke
ims = []
for f in ('lotti', 'funke'):
    for kr in range(4):
        ims.append((bild(figur('weiblich', ohren=f, schwanz=f, kranz=kr, besond=BESOND.get(f, 'keins')), 'white', 2), f'{NAMEN[f]} Kranzton {kr}'))
zeilen.append(zeile(ims, 'Kranzton 0–3 (Lotti und Funke, Mädchen)', 0, 0))

W = max(z.width for z in zeilen)
H = sum(z.height for z in zeilen)
bogen = Image.new('RGB', (W, H), 'white')
y = 0
for z in zeilen:
    bogen.paste(z, (0, y)); y += z.height
ziel = os.path.join(HIER, 'pruefbogen-paket-1.png')
bogen.save(ziel)
print(ziel, bogen.size)
