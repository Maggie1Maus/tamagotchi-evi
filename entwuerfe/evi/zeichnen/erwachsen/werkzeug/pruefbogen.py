#!/usr/bin/env python3
"""Baut einen Pruefbogen mit mehreren Kombinationen aus den vorhandenen Teilen (ergebnis/pruefbogen-*.png)."""
import sys, os, numpy as np
from PIL import Image, ImageDraw
import zusammensetzen as z

K = dict  # kurz
FIG = [
 ('Evi',                    K(gesicht='gesicht-neutral',  ohr='ohr-buschel', schwanz='schwanz-buschel')),
 ('Evi gluecklich',         K(gesicht='gesicht-gluecklich', ohr='ohr-buschel', schwanz='schwanz-buschel')),
 ('Evi mit Schlappohren',   K(gesicht='gesicht-neutral',  ohr='ohr-schlapp', schwanz='schwanz-buschel')),
 ('Evi, duenner Schwanz',   K(gesicht='gesicht-neutral',  ohr='ohr-buschel', schwanz='schwanz-duenn')),
 ('Klecksi',                K(gesicht='gesicht-neutral',  ohr='ohr-schlapp-flecken', schwanz='schwanz-duenn-flecken', muster_koerper='muster-flecken-koerper', muster_gesicht='muster-flecken-gesicht')),
 ('Klecksi traurig',        K(gesicht='gesicht-traurig',  ohr='ohr-schlapp-flecken', schwanz='schwanz-duenn-flecken', muster_koerper='muster-flecken-koerper', muster_gesicht='muster-flecken-gesicht')),
 ('Buschelohr + Flecken',   K(gesicht='gesicht-staunen',  ohr='ohr-buschel', schwanz='schwanz-duenn-flecken', muster_koerper='muster-flecken-koerper')),
 ('Schlappohr, dunkler',    K(gesicht='gesicht-muede',    ohr='ohr-schlapp', schwanz='schwanz-buschel', muster_gesicht='muster-flecken-gesicht', fell=170.)),
]
name = sys.argv[1] if len(sys.argv) > 1 else 'pruefbogen-1'
bilder = []
for t, kw in FIG:
    print(t, flush=True); bilder.append(z.bauen(**kw))
# gemeinsamer Massstab: Umriss aller Figuren
xs0 = ys0 = 1e9; xs1 = ys1 = 0
for g, a in bilder:
    ys, xs = np.nonzero(a * 255 > 60); xs0 = min(xs0, xs.min()); xs1 = max(xs1, xs.max()); ys0 = min(ys0, ys.min()); ys1 = max(ys1, ys.max())
box = (xs0, ys0, xs1, ys1)
Z = 4; Wd, Ht = 114, 116
bogen = Image.new('RGB', (4 * (Wd * Z + 20) + 20, 2 * (Ht * Z + 50) + 20), (255, 255, 255))
d = ImageDraw.Draw(bogen)
for i, ((t, kw), (g, a)) in enumerate(zip(FIG, bilder)):
    pix = z.pixel(g, a, box)
    slug = t.lower().replace(' + ', '-').replace(', ', '-').replace(' ', '-')
    for a_, b_ in (('ü', 'ue'), ('ä', 'ae'), ('ö', 'oe')): slug = slug.replace(a_, b_)
    os.makedirs(os.path.join(z.W, 'ergebnis', name), exist_ok=True)
    pix.save(os.path.join(z.W, 'ergebnis', name, slug + '-pixel.png'))
    pix.resize((Wd * 8, Ht * 8), Image.NEAREST).save(os.path.join(z.W, 'ergebnis', name, slug + '-pixel-8x.png'))
    sp = pix.resize((Wd * Z, Ht * Z), Image.NEAREST)
    bg = Image.new('RGBA', sp.size, (255, 255, 255, 255)); bg.alpha_composite(sp)
    x = 20 + (i % 4) * (Wd * Z + 20); y = 20 + (i // 4) * (Ht * Z + 50)
    bogen.paste(bg.convert('RGB'), (x, y)); d.text((x, y + Ht * Z + 8), t, fill=(0, 0, 0))
out = os.path.join(z.W, 'ergebnis', name + '.png'); bogen.save(out); print(out)
# Grossansicht (Originalaufloesung) der ersten vier
