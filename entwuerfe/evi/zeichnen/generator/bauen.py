#!/usr/bin/env python3
"""Baut generator/index.html aus dem, was in den Ordnern liegt.
Aufruf:  python3 bauen.py          (aus dem Ordner generator/ oder von ueberall)
- Durchsucht <stufe>/{koepfe,koerper,ohren,schwaenze,halsschmuck,gesichter,muster}/<ansicht>/*.png
- Faerbt und beschattet jedes Teil wie zusammensetzen.py (gegen den ersten Kopf/Koerper/Halsschmuck als Standard)
- Liest <stufe>/posen/<name>-<haltung>-<ansicht>.txt: Teile, die dort stehen, gehoeren zu diesem Tamagotchi
- Schreibt eine einzige index.html (Bilder eingebettet), per Doppelklick im Browser zu oeffnen."""
import os, sys, io, re, json, base64
import numpy as np
from PIL import Image
HIER = os.path.dirname(os.path.abspath(__file__))
ZEICHNEN = os.path.dirname(HIER)
sys.path.insert(0, os.path.join(ZEICHNEN, 'erwachsen', 'werkzeug'))
import zusammensetzen as z
N = z.N; AUF = 1024                      # Teile werden auf 1024 x 1024 verkleinert eingebettet
ANSICHTEN = ['frontal', 'seite', 'schnueffeln', 'hinten']
TYPEN = ['koepfe', 'koerper', 'ohren', 'schwaenze', 'halsschmuck', 'kopfschmuck', 'gesichter', 'muster']
SLOT_RANG = {'schwanz': 0, 'ohr': 1, 'koerper': 2, 'muster-koerper': 3, 'kopf': 4, 'muster-gesicht': 5, 'halsschmuck': 6, 'gesicht': 7}

def pngs(ordner):
    if not os.path.isdir(ordner): return []
    return sorted(f[:-4] for f in os.listdir(ordner) if f.lower().endswith('.png'))

def klein(g, a, box=None):
    v = np.clip(np.round(g), 0, 255).astype(np.uint8)
    im = Image.fromarray(np.dstack([v, v, v, np.round(a * 255).astype(np.uint8)]), 'RGBA').resize((AUF, AUF), Image.LANCZOS)
    b = io.BytesIO(); im.save(b, 'PNG', optimize=True)
    return 'data:image/png;base64,' + base64.b64encode(b.getvalue()).decode()

def umriss(a):
    ys, xs = np.nonzero(a * 255 > 60)
    return (int(xs.min()), int(ys.min()), int(xs.max()), int(ys.max())) if len(xs) else None

def poseDateien(stufe, ansicht):
    """Tamagotchi-Rezepte aus posen/*.txt: {name: {slot: dateiname}}"""
    r = {}
    ordner = os.path.join(ZEICHNEN, stufe, 'posen')
    if not os.path.isdir(ordner): return r
    for f in sorted(os.listdir(ordner)):
        m = re.match(r'(.+?)-[a-z]+-(frontal|seite|schnueffeln|hinten)\.txt$', f)
        if not m or m.group(2) != ansicht: continue
        rez = {}
        for zeile in open(os.path.join(ordner, f), encoding='utf-8'):
            zeile = zeile.split('#')[0].strip()
            if '=' not in zeile: continue
            k, v = [x.strip() for x in zeile.split('=', 1)]
            mm = re.search(r'([^/\s]+)\.png', v)
            if mm: rez[k] = mm.group(1)
        r[m.group(1).capitalize()] = rez
    return r

def bake(stufe, ansicht):
    P = lambda t, n: os.path.join(ZEICHNEN, stufe, t, ansicht, n + '.png')
    L = {t: pngs(os.path.join(ZEICHNEN, stufe, t, ansicht)) for t in TYPEN}
    if not (L['koepfe'] and L['koerper']): return None
    fell, kopfton = z.FELL, z.FELL + 17
    kopf0 = z.bauen_ebene(P('koepfe', L['koepfe'][0]), 6, lambda rk: kopfton if rk == 0 else -1.)
    kb0 = z.bauen_ebene(P('koerper', L['koerper'][0]), 5, lambda rk: fell, zu=[(814, 1145, 1096, 1134)])
    ys, xs = np.nonzero(kopf0.cover)
    cx, cy, R = (xs.min() + xs.max()) / 2, (ys.min() + ys.max()) / 2, (xs.max() - xs.min()) / 2
    axis = int(round(cx))
    yy, xx = np.mgrid[0:N, 0:N]; tt = ((xx - cx) + (yy - cy)) / (2 * R)
    dH = z.dt(kopf0.cover)
    kr0 = z.bauen_ebene(P('halsschmuck', L['halsschmuck'][0]), 4, lambda rk: -1., abdeckung=z.KRAUSE) if L['halsschmuck'] else None
    dK = z.dt(kr0.cover) if kr0 else None
    dB = z.dt(kb0.cover)
    teile = []; boxen = []
    def add(typ, name, g, a, slot, mitUmriss=True):
        teil = dict(typ=typ, name=name.split('__')[0], datei=name, slot=slot, bild=klein(g, a),
                    schicht='vorne' if name.endswith('__vorne') else ('hinten' if name.endswith('__hinten') else 'mitte'))
        teile.append(teil)
        if mitUmriss:
            b = umriss(a)
            if b: boxen.append(b)
        print('  ', typ, name, flush=True)
    for n in L['koepfe']:
        kp = z.bauen_ebene(P('koepfe', n), 6, lambda rk: kopfton if rk == 0 else -1.)
        kp.g = np.where(kp.shad, kp.g - 16 * tt, kp.g).astype(np.float32); z.rand_(kp, 24, 30)
        add('kopf', n, kp.g, kp.a, 'kopf')
    for n in L['koerper']:
        kb = z.bauen_ebene(P('koerper', n), 5, lambda rk: fell, zu=[(814, 1145, 1096, 1134)])
        z.rand_(kb, 20, 26); z.schatten(kb, dH, 28, 55, 1)
        if dK is not None: z.schatten(kb, dK, 26, 45, 1)
        add('koerper', n, kb.g, kb.a, 'koerper')
    for n in L['ohren']:
        lo, ao = z.lesen(P('ohren', n)); cols = np.nonzero(ao > 60)[1]
        beide = len(cols) and cols.min() < axis - 60 and cols.max() > axis + 60
        oh = z.bauen_ebene(P('ohren', n), 4, (lambda rk: fell) if beide else (lambda rk: fell if rk != 1 else z.INN), abdeckung=fell if beide else None)
        z.rand_(oh, 20, 24); z.schatten(oh, dH, 22, 45, 1)
        g, a = oh.g, oh.a
        if not beide:
            g0 = np.zeros((N, N), np.float32); a0 = np.zeros((N, N), np.float32)
            g0, a0 = z.drauf(g0, a0, oh); g, a = z.drauf(g0, a0, oh, True, axis)
        add('ohr', n, g, a, 'ohr')
    for n in L['schwaenze']:
        sw = z.bauen_ebene(P('schwaenze', n), 4, lambda rk: fell if rk != 1 else (255. if 'buschel' in n else fell))
        z.rand_(sw, 18, 30); z.schatten(sw, dB, 30, 75, 0.2)
        add('schwanz', n, sw.g, sw.a, 'schwanz')
    for n in L['halsschmuck']:
        kr = z.bauen_ebene(P('halsschmuck', n), 4, lambda rk: -1., abdeckung=z.KRAUSE)
        z.rand_(kr, 9, 18); z.schatten(kr, dH, 32, 55, 1)
        add('halsschmuck', n, kr.g, kr.a, 'halsschmuck')
    for n in L['kopfschmuck']:
        ks = z.bauen_ebene(P('kopfschmuck', n), 4, lambda rk: fell)
        z.rand_(ks, 14, 16); z.schatten(ks, dH, 20, 35, 1)
        add('kopfschmuck', n, ks.g, ks.a, 'kopfschmuck')
    for n in L['muster']:
        m = z.eigen(P('muster', n), z.LIN)
        gesicht = 'gesicht' in n
        m.a = m.a * (kopf0.cover if gesicht else (kb0.cover | (kb0.a > 0)))
        add('muster', n, m.g, m.a, 'muster-gesicht' if gesicht else 'muster-koerper', mitUmriss=False)
    for n in L['gesichter']:
        ge = z.eigen(P('gesichter', n)); add('gesicht', n, ge.g, ge.a, 'gesicht', mitUmriss=False)
    box = [min(b[0] for b in boxen), min(b[1] for b in boxen), max(b[2] for b in boxen), max(b[3] for b in boxen)]
    return dict(teile=teile, box=box, achse=axis)

def main():
    daten = {}
    for stufe in sorted(os.listdir(ZEICHNEN)):
        if not os.path.isdir(os.path.join(ZEICHNEN, stufe, 'koepfe')): continue
        for ansicht in ANSICHTEN:
            print(stufe, ansicht, flush=True)
            b = bake(stufe, ansicht)
            if not b: continue
            b['tamagotchis'] = poseDateien(stufe, ansicht)
            daten.setdefault(stufe, {})[ansicht] = b
    vorlage = open(os.path.join(HIER, 'vorlage.html'), encoding='utf-8').read()
    seite = vorlage.replace('/*DATEN*/null', json.dumps(daten, ensure_ascii=False))
    out = os.path.join(HIER, 'index.html')
    open(out, 'w', encoding='utf-8').write(seite)
    print('geschrieben:', out, round(len(seite) / 1e6, 1), 'MB')

if __name__ == '__main__': main()
