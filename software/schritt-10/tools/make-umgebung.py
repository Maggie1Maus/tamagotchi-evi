#!/usr/bin/env python3
# Erzeugt evi-handy/umgebung.h: Evis neues Zimmer „Gemütlich & schlicht“ (Wunsch der Nutzerin vom 24.11.2026),
# die Jahreszeiten im Fenster, die Wetter-Bilder und die Weihnachtsdeko. Nur für das Board.
#
# Quelle sind die Entwürfe in entwuerfe/umgebung/ (beschlossene Gestaltung). Sie werden nur ausgeschnitten,
# verkleinert und in Schwarz/Weiß umgerechnet, nicht neu gezeichnet:
#  - moebel-gemuetlich-schlicht.png: Tisch, Pflanze, Napf, Wandregal, Sideboard (Maßstab 1 : 5,25, beim Napf 1 : 10,5)
#  - fenster-jahreszeiten-referenz.png: untere Reihe = Baum und Hügel der vier Jahreszeiten,
#    obere Reihe = Sonne, Sonne mit Wolke, Regenwolke, Wolke. Das Fensterkreuz der Vorlage wird weggelassen und
#    die vier Scheiben einzeln auf die Scheiben des Spielfensters gelegt (dessen Kreuz liegt davor).
#  - weihnachtsbaum.png (durchsichtiger Hintergrund)
#  - fenster-sterne.png, kerzentablett.png (durchsichtiger Hintergrund, seit 24.11.2026)
# Von Claude gezeichnet, weil es dafür kein Bild gibt: Dielenboden (nach dem Muster „Boden“ im Entwurf).
# Der Rest des Zimmers (Fenster, Teppich, Tapete) kommt aus dem bisherigen Zimmer in evi-handy/assets.h.
#
#   python3 software/schritt-10/tools/make-umgebung.py
# Braucht Python mit Pillow und numpy (in der Cloud vorhanden). Schreibt außerdem eine Vorschau nach /tmp.
import re, os, sys
import numpy as np
from PIL import Image

HIER = os.path.dirname(os.path.abspath(__file__))
WURZEL = os.path.normpath(os.path.join(HIER, '..', '..', '..'))
U = os.path.join(WURZEL, 'entwuerfe', 'umgebung')
SKETCH = os.path.join(HIER, '..', 'evi-handy')
B, H = 320, 224

def lade(name):
    return np.array(Image.open(os.path.join(U, name)).convert('L')) < 128

def trimmen(b):
    ys, xs = np.nonzero(b)
    return b[ys.min():ys.max() + 1, xs.min():xs.max() + 1]

def verkleinern(b, f, schwelle=0.35, fy=None):
    """Jeder neue Punkt ist schwarz, wenn genug vom Feld darunter schwarz ist."""
    fy = fy or f
    h, w = int(round(b.shape[0] / fy)), int(round(b.shape[1] / f))
    o = np.zeros((h, w), bool)
    for y in range(h):
        for x in range(w):
            blk = b[int(y * fy):max(int(y * fy) + 1, int((y + 1) * fy)), int(x * f):max(int(x * f) + 1, int((x + 1) * f))]
            o[y, x] = blk.size and blk.mean() > schwelle
    return o

def aussen(linien, saat=None):
    """Was von außen (oder von den Saatpunkten aus) erreichbar ist, ohne eine Linie zu kreuzen.
    Die Linien werden dafür um 1 Punkt abgedichtet, damit kleine Lücken nichts auslaufen lassen."""
    h, w = linien.shape
    dicht = linien.copy()
    dicht[:, 1:] |= linien[:, :-1]; dicht[1:, :] |= linien[:-1, :]
    weg = np.zeros((h, w), bool)
    stapel = saat if saat is not None else [(y, x) for y in range(h) for x in (0, w - 1)] + [(y, x) for x in range(w) for y in (0, h - 1)]
    stapel = [p for p in stapel if not dicht[p]]
    for p in stapel: weg[p] = True
    while stapel:
        y, x = stapel.pop()
        for ny, nx in ((y + 1, x), (y - 1, x), (y, x + 1), (y, x - 1)):
            if 0 <= ny < h and 0 <= nx < w and not weg[ny, nx] and not dicht[ny, nx]:
                weg[ny, nx] = True; stapel.append((ny, nx))
    # Die Abdichtung hat das Innere um 1 Punkt verkleinert: Außen darf nicht bis in die Linien reichen
    return weg & ~linien

def figur(linien):
    """Linien und Maske (deckend = alles innerhalb des Umrisses)."""
    return linien, ~aussen(linien)

def aus_text(zeilen):
    w = max(len(z) for z in zeilen)
    return np.array([[c == 'X' for c in z.ljust(w, '.')] for z in zeilen], bool)

# ---------- Bisheriges Zimmer aus assets.h ----------
assets = open(os.path.join(SKETCH, 'assets.h'), encoding='utf-8').read()
def feld(name, n):
    m = re.search(r'static const uint8_t %s\[\] = \{(.*?)\};' % name, assets, re.S)
    werte = [int(v, 16) for v in re.findall(r'0x[0-9a-fA-F]{2}', m.group(1))][:n]
    bits = np.unpackbits(np.array(werte, np.uint8)).reshape(H, B).astype(bool)
    return bits
alt_tag, alt_nacht = feld('ZIMMER_TAG', 40 * H), feld('ZIMMER_NACHT', 40 * H)

# ---------- Möbel ----------
m = lade('moebel-gemuetlich-schlicht.png')
def moebel(box, f):
    x0, y0, x1, y1 = box
    return figur(verkleinern(trimmen(m[y0:y1, x0:x1]), f))
TISCH = moebel((100, 160, 500, 510), 5.25)
PFLANZE = moebel((600, 160, 930, 510), 7.0)
NAPF = moebel((1080, 160, 1430, 510), 10.5)
REGAL = moebel((60, 630, 500, 930), 5.25)
SIDEBOARD = moebel((540, 630, 1000, 930), 5.25)

# ---------- Freigestellte Bilder der Nutzerin (durchsichtiger Hintergrund) ----------
def freigestellt(name, f):
    """Linien = dunkle deckende Punkte, Maske = deckende Punkte; verkleinert um f."""
    bild = np.array(Image.open(os.path.join(U, name)).convert('RGBA'))
    deck = bild[:, :, 3] > 128
    dunk = (bild[:, :, :3].mean(2) < 128) & deck
    ys, xs = np.nonzero(deck)
    ausschnitt = (slice(ys.min(), ys.max() + 1), slice(xs.min(), xs.max() + 1))
    lin = verkleinern(dunk[ausschnitt], f, 0.35)
    mas = verkleinern(deck[ausschnitt], f, 0.5)
    return lin, mas | lin

# ---------- Weihnachtsbaum ----------
wb = np.array(Image.open(os.path.join(U, 'weihnachtsbaum.png')).convert('RGBA'))
deckend = wb[:, :, 3] > 128
dunkel = (wb[:, :, :3].mean(2) < 128) & deckend
ys, xs = np.nonzero(deckend)
box = (slice(ys.min(), ys.max() + 1), slice(xs.min(), xs.max() + 1))
# Seit dem Test am Board (24.11.2026): Der Baum steht zu Weihnachten auf dem Boden statt auf dem Tisch
# (der Tisch ist dann weg) und ist größer: 150 Punkte hoch, vom Boden bis unter die Leiste oben.
f_baum = (ys.max() - ys.min() + 1) / 150.0
WBAUM_L = verkleinern(dunkel[box], f_baum, 0.35)
WBAUM = (WBAUM_L, ~aussen(WBAUM_L))

# ---------- Fenster: Scheiben der Vorlage auf die Scheiben des Spiels ----------
# Spielfenster (Scheibe x 33–94, y 49–117; Kreuz x 61–64, y 81–84)
GLAS = (33, 49, 62, 69)
SCHEIBEN_SPIEL = [(33, 49, 28, 32), (65, 49, 30, 32), (33, 85, 28, 33), (65, 85, 30, 33)]
r = lade('fenster-jahreszeiten-referenz.png')
def scheiben_vorlage(a, x0, x1):
    """Innere Scheiben eines Vorlagenfensters zwischen x0 und x1: findet Rahmen und Kreuz selbst."""
    def laeufe(v):
        out, drin = [], False
        for i, s in enumerate(v):
            if s > 0.6 and not drin: st, drin = i, True
            if s <= 0.6 and drin: out.append((st, i - 1)); drin = False
        if drin: out.append((st, len(v) - 1))
        return out
    y0, y1 = a
    c = r[y0:y1, x0:x1]
    sp = [(p + x0, q + x0) for p, q in laeufe(c.mean(0))]
    ze = [(p + y0, q + y0) for p, q in laeufe(c.mean(1))]
    links, kreuz_x, rechts = sp[1][1] + 1, sp[2], sp[3][0] - 1
    oben, unten = ze[1][1] + 1, ze[-2][0] - 1
    # die Reihe mit einer breiten Wolke (zu viel Schwarz) überspringen: Kreuz = schmale Linie in der Mitte
    mitte = [z for z in ze[2:-2] if z[1] - z[0] < 10]
    kreuz_y = mitte[0]
    return [(links, oben, kreuz_x[0] - 1, kreuz_y[0] - 1), (kreuz_x[1] + 1, oben, rechts, kreuz_y[0] - 1),
            (links, kreuz_y[1] + 1, kreuz_x[0] - 1, unten), (kreuz_x[1] + 1, kreuz_y[1] + 1, rechts, unten)]

def fensterbild(a, x0, x1):
    """Linien einer ganzen Scheibe (62 × 69), Scheibe für Scheibe übertragen; das Kreuz bleibt frei."""
    gl = np.zeros((GLAS[3], GLAS[2]), bool)
    for (vx0, vy0, vx1, vy1), (sx, sy, sw, sh) in zip(scheiben_vorlage(a, x0, x1), SCHEIBEN_SPIEL):
        teil = r[vy0:vy1 + 1, vx0:vx1 + 1]
        k = verkleinern(teil, teil.shape[1] / sw, 0.3, teil.shape[0] / sh)[:sh, :sw]
        gl[sy - GLAS[1]:sy - GLAS[1] + k.shape[0], sx - GLAS[0]:sx - GLAS[0] + k.shape[1]] = k
    return gl

FENSTER_X = [(55, 370), (430, 740), (800, 1110), (1170, 1480)]

def fuellen(frei, saat):
    """Flutfüllung über freie Punkte von den Saatpunkten aus."""
    h, w = frei.shape
    weg = np.zeros((h, w), bool)
    stapel = [p for p in saat if frei[p]]
    for p in stapel: weg[p] = True
    while stapel:
        y, x = stapel.pop()
        for ny, nx in ((y + 1, x), (y - 1, x), (y, x + 1), (y, x - 1)):
            if 0 <= ny < h and 0 <= nx < w and frei[ny, nx] and not weg[ny, nx]:
                weg[ny, nx] = True; stapel.append((ny, nx))
    return weg

def jahreszeit_fenster(x0, x1):
    """Linien und Maske einer Jahreszeit. Der Himmel wird in voller Auflösung der Vorlage gesucht (dort sind
    die Umrisse lückenlos), Scheibe für Scheibe: in den oberen Scheiben vom oberen Rand aus, in den unteren dort,
    wo direkt über dem Kreuz Himmel war. Dann wird alles wie die Linien verkleinert."""
    scheiben = scheiben_vorlage((630, 920), x0, x1)
    himmel_oben = {}
    lin = np.zeros((GLAS[3], GLAS[2]), bool)
    deckt = np.zeros((GLAS[3], GLAS[2]), bool)
    for n, ((vx0, vy0, vx1, vy1), (sx, sy, sw, sh)) in enumerate(zip(scheiben, SCHEIBEN_SPIEL)):
        teil = r[vy0:vy1 + 1, vx0:vx1 + 1]
        frei = ~teil
        if n < 2:
            saat = [(0, x) for x in range(teil.shape[1])]
        else:
            oben = himmel_oben[n - 2]            # Scheibe darüber: deren unterste Reihe
            saat = [(0, x) for x in range(min(teil.shape[1], oben.shape[1])) if oben[-1, x]]
        hi = fuellen(frei, saat)
        if n < 2: himmel_oben[n] = hi
        k = verkleinern(teil, teil.shape[1] / sw, 0.3, teil.shape[0] / sh)[:sh, :sw]
        d = verkleinern(~hi, teil.shape[1] / sw, 0.5, teil.shape[0] / sh)[:sh, :sw]
        if n < 2: k[0:3, :] = False; d[0:3, :] = False      # Reste des Vorlagenrahmens oben
        else: k[0, :] = k[1, :]; d[0, :] = d[1, :]          # Rest des Kreuzes: erste Reihe wie die zweite
        # einzelne senkrechte Striche am linken und rechten Scheibenrand (Reste von Rahmen und Kreuz) weg
        for rand, innen in ((0, 1), (sw - 1, sw - 2)):
            einzeln = k[:, rand] & ~k[:, innen]
            k[einzeln, rand] = False; d[einzeln, rand] = False
        lin[sy - GLAS[1]:sy - GLAS[1] + k.shape[0], sx - GLAS[0]:sx - GLAS[0] + k.shape[1]] = k
        deckt[sy - GLAS[1]:sy - GLAS[1] + d.shape[0], sx - GLAS[0]:sx - GLAS[0] + d.shape[1]] = d
    return lin, deckt | lin

# Himmel zwischen Ästen und Rahmen, den die Suche nicht erreicht (ganz von Linien umschlossen): von Hand
# gewählte Punkte (Reihe, Spalte in der Scheibe) und die Reihen, zwischen denen dort Himmel ist.
# Frühling und Sommer: unter der Krone links und rechts vom Stamm bis zu den Hügeln (ab der Unterkante der Krone).
HIMMEL_EXTRA = {0: ([(58, 40), (58, 56)], 54, 63), 1: ([(58, 40), (58, 56)], 55, 63),
                2: ([(29, 60), (38, 55), (55, 58)], 0, 61), 3: ([(29, 60), (38, 53), (50, 57)], 0, 56)}

JAHRESZEITEN = []
for i, (x0, x1) in enumerate(FENSTER_X):
    lin, mas = jahreszeit_fenster(x0, x1)
    if i in HIMMEL_EXTRA:
        punkte, von, bis = HIMMEL_EXTRA[i]
        frei = mas & ~lin
        frei[:von, :] = False
        frei[bis + 1:, :] = False
        mas = mas & ~fuellen(frei, punkte)
    JAHRESZEITEN.append((lin, mas))

# Wetter aus der oberen Reihe: nur die obere linke Scheibe (Sonne, Sonne mit Wolke, Regenwolke, Wolke)
WETTER = []
for i, (x0, x1) in enumerate(FENSTER_X):
    (vx0, vy0, vx1, vy1) = scheiben_vorlage((180, 470), x0, x1)[0]
    teil = r[vy0:vy1 + 1, vx0:vx1 + 1]
    k = verkleinern(teil, teil.shape[1] / 28, 0.3, teil.shape[0] / 32)
    if i == 2:   # Regenwolke: nur die Wolke, ohne die Tropfen darunter
        zeilen = k.any(1)
        ende = next(y for y in range(len(zeilen)) if zeilen[y] and not zeilen[y + 1] and y > 3)
        k[ende + 1:] = False
    k = trimmen(k)
    WETTER.append(figur(k))

# ---------- Weihnachtsdeko der Nutzerin (hochgeladen am 24.11.2026, ersetzt Claudes Platzhalter) ----------
# Sternengirlande über dem Fenster: 1 : 16 (122 × 39 Punkte), die hängenden Sterne außen neben dem Rahmen
FENSTERSTERNE = freigestellt('fenster-sterne.png', 16.0)
# Kerzentablett auf dem Sideboard: 1 : 15 (72 × 52 Punkte), fast so breit wie das Sideboard; die Kerzen
# stehen vor dem unteren Teil des Fensters
KERZEN = freigestellt('kerzentablett.png', 15.0)

# ---------- Grundzimmer ----------
z = np.zeros((H, B), bool)
for y in range(146):                                # Tapete: Punkte 2 × 2 im Raster 40 × 36, links und rechts
    for x in range(B):
        if (x < 108 or x >= 226) and x % 40 in (19, 20) and y % 36 in (17, 18): z[y, x] = True
z[147:150, :] = True; z[153, :] = True              # Fußleiste
# Dielenboden: Bretter 12 Punkte hoch, 74 und 54 Punkte lang, Fugen versetzt, ein paar Maserstriche (nach dem Entwurf „Boden“)
for reihe, y0 in enumerate(range(154, H, 12)):
    if y0 + 11 < H: z[y0 + 11, :] = True
    versatz = (reihe * 53) % 128
    for x in range(-128 + versatz, B, 128):
        for xf in (x, x + 74):
            if 0 <= xf < B: z[y0:min(H, y0 + 11), xf] = True
        for xm, ym, lang in ((x + 14, y0 + 4, 7), (x + 92, y0 + 7, 6)):
            for xx in range(xm, xm + lang):
                if 0 <= xx < B and ym < H: z[ym, xx] = True
# Fenster (Rahmen, Fensterbank) und Teppich bleiben wie bisher
z[40:131, 22:107] = alt_tag[40:131, 22:107]
for y in range(170, H):
    for x in range(100, 240):
        if ((x - 169) / 61.5) ** 2 + ((y - 190) / 17.5) ** 2 <= 1: z[y, x] = alt_tag[y, x]

def einsetzen(ziel, fig, x, y):
    lin, mas = fig
    h, w = lin.shape
    t = ziel[y:y + h, x:x + w]
    t[mas[:t.shape[0], :t.shape[1]]] = False
    t[lin[:t.shape[0], :t.shape[1]]] = True

# Lage im Zimmer (Unterkante wo nötig)
SIDEBOARD_X, SIDEBOARD_UNTEN = 14, 193
TISCH_X, TISCH_UNTEN = 244, 181
TISCH_OBEN = TISCH_UNTEN - TISCH[0].shape[0] + 11    # auf der Tischplatte
NAPF_X, NAPF_UNTEN = 262, 199
einsetzen(z, SIDEBOARD, SIDEBOARD_X, SIDEBOARD_UNTEN - SIDEBOARD[0].shape[0])
einsetzen(z, NAPF, NAPF_X, NAPF_UNTEN - NAPF[0].shape[0])
mitte_tisch = TISCH_X + TISCH[0].shape[1] // 2
lagen = {
    'TISCH': (TISCH, TISCH_X, TISCH_UNTEN - TISCH[0].shape[0]),
    'REGAL': (REGAL, 275 - REGAL[0].shape[1] // 2, 30),
    'PFLANZE': (PFLANZE, mitte_tisch - PFLANZE[0].shape[1] // 2, TISCH_OBEN - PFLANZE[0].shape[0]),
    'WEIHNACHTSBAUM': (WBAUM, 273 - WBAUM[0].shape[1] // 2, 179 - WBAUM[0].shape[0]),
    'KERZEN': (KERZEN, 53 - KERZEN[0].shape[1] // 2, SIDEBOARD_UNTEN - SIDEBOARD[0].shape[0] + 8 - KERZEN[0].shape[0]),
    'FENSTERSTERNE': (FENSTERSTERNE, 4, 28),
}

# ---------- Ausgabe ----------
def bytes_von(b):
    h, w = b.shape
    pad = np.zeros((h, (w + 7) // 8 * 8), bool); pad[:, :w] = b
    return list(np.packbits(pad.astype(np.uint8), axis=1).flatten())

zeilen = [
    '// Erzeugt von software/schritt-10/tools/make-umgebung.py - nicht von Hand ändern.',
    '// Evis Zimmer „Gemütlich & schlicht“, Jahreszeiten, Wetter und Weihnachtsdeko aus entwuerfe/umgebung/',
    '// (nur ausgeschnitten und verkleinert). 1 Bit pro Punkt, links im höchsten Bit; Linien 1 = schwarz,',
    '// Maske 1 = deckend (weiß). Bild steht in assets.h.',
    '#pragma once', '#include "assets.h"', '']
def feld_aus(name, werte):
    zeilen.append('static const uint8_t %s[] = {' % name)
    for i in range(0, len(werte), 20): zeilen.append('  ' + ','.join('0x%02x' % v for v in werte[i:i + 20]) + ',')
    zeilen.append('};')
def bild_aus(name, fig, ox=0, oy=0):
    lin, mas = fig
    feld_aus(name + '_LINIEN', bytes_von(lin)); feld_aus(name + '_MASKE', bytes_von(mas & ~lin | lin))
    zeilen.append('static const Bild %s = {%d, %d, %d, %d, %s_LINIEN, %s_MASKE};' % (name, lin.shape[1], lin.shape[0], ox, oy, name, name))

zeilen.append('// Grundzimmer ohne Tisch, Regal, Pflanze und Scheibe (die zeichnet fensterZeichnen in umgebung.ino), 320 × 224')
feld_aus('ZIMMER_GRUND', bytes_von(z))
for name, (fig, x, y) in lagen.items():
    zeilen.append('// %s: oben links bei %d/%d' % (name, x, y))
    bild_aus(name, fig, x, y)
for i, n in enumerate(['FRUEHLING', 'SOMMER', 'HERBST', 'WINTER']):
    bild_aus('FENSTER_' + n, JAHRESZEITEN[i], GLAS[0], GLAS[1])
for i, n in enumerate(['SONNE', 'SONNE_WOLKE', 'REGENWOLKE', 'WOLKE']):
    bild_aus('WETTER_' + n, WETTER[i])
zeilen.append('static const Bild* const FENSTER_JAHRESZEIT[4] = {&FENSTER_FRUEHLING, &FENSTER_SOMMER, &FENSTER_HERBST, &FENSTER_WINTER};')
open(os.path.join(SKETCH, 'umgebung.h'), 'w', encoding='utf-8').write('\n'.join(zeilen) + '\n')

# Vorschau: Grundzimmer mit allem
v = z.copy()
for name in ('TISCH', 'REGAL', 'PFLANZE', 'KERZEN'):
    fig, x, y = lagen[name]; einsetzen(v, fig, x, y)
gl = v[GLAS[1]:GLAS[1] + GLAS[3], GLAS[0]:GLAS[0] + GLAS[2]]; gl[:] = False
einsetzen(v, JAHRESZEITEN[0], GLAS[0], GLAS[1])
Image.fromarray(np.where(v, 0, 255).astype(np.uint8)).resize((B * 3, H * 3), Image.NEAREST).save(os.environ.get('VORSCHAU', '/tmp/umgebung-vorschau.png'))
for name, (fig, x, y) in lagen.items(): print(name, fig[0].shape, 'bei', x, y)
print('Jahreszeiten', [j[0].shape for j in JAHRESZEITEN], 'Wetter', [w[0].shape for w in WETTER])
print('umgebung.h geschrieben')
