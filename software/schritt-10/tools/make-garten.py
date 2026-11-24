"""Freigestellte Garten- und Evi-PNGs in deckende 1-Bit-Bilder umrechnen.

Normal: Header pixelgenau aus den kleinen Repository-PNGs erzeugen.
Ersteinrichtung: --prepare <JSON-Manifest> (Originalpfade, Maße und Bildaufträge).
Bestehende Evi-Zeichnungen werden niemals überschrieben.
"""
from pathlib import Path
from PIL import Image, ImageOps, ImageFilter, ImageDraw, ImageChops
import json, sys
root = Path(__file__).resolve().parents[3]
evi = root / 'entwuerfe/evi/garten'
garden = root / 'entwuerfe/garten'
phases = ['baby', 'teen', 'adult', 'senior']
poses = ['schnuppern', 'untersuchen', 'zeigen']
seasons = ['fruehling', 'sommer', 'herbst', 'winter']
if '--prepare' in sys.argv:
    specs = json.loads(Path(sys.argv[sys.argv.index('--prepare') + 1]).read_text())
    for spec in specs:
        folder = evi if spec['kind'] == 'evi' else garden
        folder.mkdir(parents=True, exist_ok=True)
        image = Image.open(spec['path']).convert('RGBA')
        alpha = image.getchannel('A').point(lambda a: 255 if a > 127 else 0)
        image = image.crop(alpha.getbbox())
        image.thumbnail((spec.get('width', 110), spec['height']), Image.Resampling.LANCZOS)
        w, h = image.size
        alpha = image.getchannel('A').point(lambda a: 255 if a > 127 else 0)
        # Weiße Innenflächen erhalten, winzige Konturlücken nur für die Deckmaske schließen.
        padded = ImageOps.expand(alpha, 3, fill=0).filter(ImageFilter.MaxFilter(3))
        ImageDraw.floodfill(padded, (0, 0), 128)
        filled = padded.point(lambda a: 0 if a == 128 else 255).filter(ImageFilter.MinFilter(3))
        mask = ImageChops.lighter(alpha, filled.crop((3, 3, w+3, h+3)))
        prepared = Image.new('RGBA', (w, h), (255, 255, 255, 0))
        for y in range(h):
            for x in range(w):
                r, g, b, a = image.getpixel((x, y))
                if mask.getpixel((x, y)):
                    c = 0 if a > 127 and r*299 + g*587 + b*114 < 140000 else 255
                    prepared.putpixel((x, y), (c, c, c, 255))
        prepared.save(folder / (spec['key'] + '.png'), optimize=True)
    # Bildaufträge ohne flüchtige Originalpfade im Repository festhalten.
    (garden / 'bildauftraege.json').write_text(json.dumps([{k:v for k,v in s.items() if k not in ('path','reference')} for s in specs], ensure_ascii=False, indent=2)+'\n')
lines = ['// Erzeugt von tools/make-garten.py aus entwuerfe/garten und entwuerfe/evi/garten.', '#pragma once', '#include "assets.h"', '']
assets = [(evi, p+'-'+q) for p in phases for q in poses]
assets += [(garden, p+'-'+s) for p in ['baum','busch','beet'] for s in seasons]
assets += [(garden, p) for p in ['zaun','gartentuer','lichterkette','kranz','schneemann']]
for folder, key in assets:
    image = Image.open(folder / (key+'.png')).convert('RGBA')
    w,h = image.size
    name = 'GA_'+key.replace('-','_').upper()
    for suffix in ['L','M']:
        data = []
        for y in range(h):
            for start in range(0,w,8):
                byte = 0
                for bit in range(8):
                    if start+bit >= w: continue
                    r,g,b,a = image.getpixel((start+bit,y))
                    if a > 127 and (suffix == 'M' or r < 128): byte |= 128 >> bit
                data.append(byte)
        lines.append('static const uint8_t '+name+'_'+suffix+'[] = {')
        lines += ['  '+','.join(f'0x{v:02x}' for v in data[i:i+20])+',' for i in range(0,len(data),20)]
        lines.append('};')
    lines.append(f'static const Bild {name} = {{{w}, {h}, 0, 0, {name}_L, {name}_M}};')
lines.append('static const Bild* const GARTEN_EVI[5][3] = {\n  {nullptr, nullptr, nullptr},')
for p in phases: lines.append('  {'+', '.join('&GA_'+(p+'_'+q).upper() for q in poses)+'},')
lines.append('};')
for p in ['baum','busch','beet']:
    lines.append('static const Bild* const GARTEN_'+p.upper()+'[4] = {'+', '.join('&GA_'+(p+'_'+s).upper() for s in seasons)+'};')
(root/'software/schritt-10/evi-handy/gartenbilder.h').write_text('\n'.join(lines)+'\n')
print('29 Einzelbilder und Garten-Header bereit.')
