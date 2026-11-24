"""Die freigegebene Gartenszene als gemeinsame 1-Bit-Ebene für alle Jahreszeiten.
Normal: Header aus den kleinen PNGs erzeugen. Ersteinrichtung: --prepare <Manifest>.
Einzelbilder bleiben zusätzlich verwendbar; die Szene wird nicht aus Symbolen neu aufgebaut.
"""
from pathlib import Path
from PIL import Image, ImageChops, ImageOps, ImageFilter, ImageDraw
import json, sys
root = Path(__file__).resolve().parents[3]
folder = root/'entwuerfe/garten/v2'
folder.mkdir(exist_ok=True)
seasons = ['fruehling','sommer','herbst','winter','weihnachten']
if '--prepare' in sys.argv:
    specs = json.loads(Path(sys.argv[sys.argv.index('--prepare')+1]).read_text())
    for s in specs:
        image = Image.open(s['path']).convert('RGBA').resize((320,174),Image.Resampling.LANCZOS)
        # Sommerlinien stammen direkt aus dem freigegebenen Entwurf; die Freistellung liefert nur die Himmelsmaske.
        source = Image.open(root/'entwuerfe/garten/neuentwurf/gesamtentwurf-sommer.png').convert('RGBA').resize((320,174),Image.Resampling.LANCZOS) if s['key']=='sommer' else image
        prepared = Image.new('RGBA',(320,174),(255,255,255,0))
        for y in range(174):
            for x in range(320):
                r,g,b,a = source.getpixel((x,y))
                ink = a>64 and r*299+g*587+b*114 < (210000 if s['key']=='sommer' else 180000)
                # Die Generierung entfernt teils auch weiße Rasen-/Schneeflächen. Unterhalb der
                # Zaunoberkante ist alles Garten: Deckmaske schließen, ohne Konturen zu verändern.
                opaque = y>=72 or image.getpixel((x,y))[3]>96 or ink
                if opaque:
                    c = 0 if ink else 255
                    prepared.putpixel((x,y),(c,c,c,255))
        prepared.save(folder/(s['key']+'.png'),optimize=True)
    old = json.loads((folder/'bildauftraege.json').read_text()) if (folder/'bildauftraege.json').exists() else []
    bykey = {s['key']:s for s in old}
    bykey.update({s['key']:{k:v for k,v in s.items() if k!='path'} for s in specs})
    (folder/'bildauftraege.json').write_text(json.dumps(list(bykey.values()),ensure_ascii=False,indent=2)+'\n')
if '--prepare-objects' in sys.argv:
    specs = json.loads(Path(sys.argv[sys.argv.index('--prepare-objects')+1]).read_text())
    objects=folder/'einzelobjekte'; objects.mkdir(exist_ok=True)
    for spec in specs:
        image=Image.open(spec['path']).convert('RGBA')
        alpha=image.getchannel('A').point(lambda a:255 if a>96 else 0)
        image=image.crop(alpha.getbbox())
        image.thumbnail((spec['width'],spec['height']),Image.Resampling.LANCZOS)
        w,h=image.size
        alpha=image.getchannel('A').point(lambda a:255 if a>96 else 0)
        # Nur die Deckmaske geschlossener Objekt-Innenflächen vervollständigen.
        padded=ImageOps.expand(alpha,3,fill=0).filter(ImageFilter.MaxFilter(3))
        ImageDraw.floodfill(padded,(0,0),128)
        filled=padded.point(lambda a:0 if a==128 else 255).filter(ImageFilter.MinFilter(3))
        mask=ImageChops.lighter(alpha,filled.crop((3,3,w+3,h+3)))
        out=Image.new('RGBA',(w,h),(255,255,255,0))
        for y in range(h):
            for x in range(w):
                r,g,b,a=image.getpixel((x,y))
                if mask.getpixel((x,y)):
                    c=0 if a>64 and r*299+g*587+b*114<180000 else 255
                    out.putpixel((x,y),(c,c,c,255))
        out.save(objects/(spec['key']+'.png'),optimize=True)
    (objects/'bildauftraege.json').write_text(json.dumps([{k:v for k,v in sp.items() if k not in ('path','reference')} for sp in specs],ensure_ascii=False,indent=2)+'\n')
lines=['// Erzeugt von tools/make-garten-szene.py; freigegebener Garten als zusammenhängende Ebene.','#pragma once','#include "assets.h"','']
for key in seasons+['lichter']:
    p=folder/'einzelobjekte/lichterkette.png' if key=='lichter' else folder/(key+'.png')
    if not p.exists(): continue
    image=Image.open(p).convert('RGBA'); w,h=image.size
    name='LANDSCHAFT_'+key.upper()
    for suffix in ['L','M']:
        data=[]
        for y in range(h):
            for start in range(0,w,8):
                byte=0
                for bit in range(8):
                    if start+bit>=w: continue
                    r,g,b,a=image.getpixel((start+bit,y))
                    if a>127 and (suffix=='M' or r<128): byte|=128>>bit
                data.append(byte)
        lines.append('static const uint8_t '+name+'_'+suffix+'[] = {')
        lines+=['  '+','.join(f'0x{v:02x}' for v in data[i:i+20])+',' for i in range(0,len(data),20)]
        lines.append('};')
    lines.append(f'static const Bild {name} = {{{w}, {h}, 0, 27, {name}_L, {name}_M}};')
lines.append('static const Bild* const GARTEN_LANDSCHAFT[4] = {'+', '.join('&LANDSCHAFT_'+s.upper() for s in seasons[:4])+'};')
(root/'software/schritt-10/evi-handy/gartenlandschaft.h').write_text('\n'.join(lines)+'\n')
print('Gartenszenen in 1 Bit vorbereitet.')
