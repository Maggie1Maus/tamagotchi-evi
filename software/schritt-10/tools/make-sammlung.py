"""Die Fundstückbilder nur zuschneiden, verkleinern und in 1-Bit-Masken umrechnen."""
from pathlib import Path
from PIL import Image, ImageChops, ImageDraw, ImageFilter, ImageOps
import sys

root = Path(__file__).resolve().parents[3]
lines = ['// Erzeugt von tools/make-sammlung.py. Neue Fundstückbilder im Stil Gemütlich & schlicht.',
         '#pragma once', '#include "assets.h"', '#include "sammlung.h"', '']
names = ['stein', 'blatt', 'feder', 'zapfen', 'micky', 'muschel', 'drache', 'triforce', 'fossil', 'junimo']
for name in names:
    image = Image.open(root / 'entwuerfe/sammlung' / (name + '.png')).convert('RGBA')
    source_size = image.size
    alpha = image.getchannel('A').point(lambda a: 255 if a > 127 else 0)
    image = image.crop(alpha.getbbox())
    image.thumbnail((60, 60), Image.Resampling.LANCZOS)
    w, h = image.size
    # Neue Bilder einmal als kleine, strikt schwarz-weiße Spielgrafiken ablegen. Die
    # ursprünglichen Generierungen bleiben separat erhalten. Danach sind die PNGs selbst
    # die pixelgenauen Quellen; Header lassen sich ohne weitere Bildbearbeitung erzeugen.
    if '--prepare' in sys.argv and (max(source_size) > 60 or name not in names[:4]):
        # Manche Generierungen liefern schwarze Konturen, aber ein durchsichtiges Inneres.
        # Nur die Deckmaske schließen und innen weiß füllen; die schwarzen Linien unverändert
        # lassen. Ein Pixel Abstand beim Suchen des Außenraums fängt winzige Konturlücken ab.
        original_alpha = image.getchannel('A').point(lambda a: 255 if a > 127 else 0)
        padded = ImageOps.expand(original_alpha, 3, fill=0).filter(ImageFilter.MaxFilter(3))
        ImageDraw.floodfill(padded, (0, 0), 128)
        filled = padded.point(lambda a: 0 if a == 128 else 255).filter(ImageFilter.MinFilter(3))
        inside = ImageChops.lighter(original_alpha, filled.crop((3, 3, w + 3, h + 3)))
        prepared = Image.new('RGBA', (w, h), (255, 255, 255, 0))
        for y in range(h):
            for x in range(w):
                r, g, b, a = image.getpixel((x, y))
                if a > 127:
                    c = 0 if r * 299 + g * 587 + b * 114 < 140000 else 255
                    prepared.putpixel((x, y), (c, c, c, 255))
                elif inside.getpixel((x, y)):
                    prepared.putpixel((x, y), (255, 255, 255, 255))
        image = prepared
        image.save(root / 'entwuerfe/sammlung' / (name + '.png'), optimize=True)
    pixels = image.load()
    for suffix in ['L', 'M']:
        data = []
        for y in range(h):
            for start in range(0, w, 8):
                byte = 0
                for bit in range(8):
                    x = start + bit
                    if x >= w:
                        continue
                    r, g, b, a = pixels[x, y]
                    if a > 127 and (suffix == 'M' or (r * 299 + g * 587 + b * 114) < 140000):
                        byte |= 128 >> bit
                data.append(byte)
        lines.append(f'static const uint8_t FUND_{name.upper()}_{suffix}[] = {{')
        for i in range(0, len(data), 20):
            lines.append('  ' + ','.join(f'0x{v:02x}' for v in data[i:i+20]) + ',')
        lines.append('};')
    lines.append(f'static const Bild FUND_ART_{name.upper()} = {{{w}, {h}, 0, 0, FUND_{name.upper()}_L, FUND_{name.upper()}_M}};')
lines.append('static const Bild* const FUND_BILD[] = {' + ', '.join('&FUND_ART_' + n.upper() for n in names) + '};')
lines.append('static_assert(sizeof(FUND_BILD) / sizeof(FUND_BILD[0]) == FUNDARTEN, "Jede Fundart braucht ein Bild");')
(root / 'software/schritt-10/evi-handy/fundbilder.h').write_text('\n'.join(lines) + '\n')
