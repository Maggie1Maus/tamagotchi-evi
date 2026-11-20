"""Erzeugt evi-cube/evi_art.h aus Evis Zeichnungen.

Quelle sind die schon zugeschnittenen Bilder der Handy-App
(handy-app/dist/img/adult-*.png, erzeugt von handy-app/tools/build-art.ps1).
Nichts wird neu gezeichnet: Alle Bilder werden mit demselben Massstab
verkleinert, und jeder Punkt wird an oder aus. Die schwarzen Linien der
Zeichnung leuchten auf dem OLED, alles andere bleibt dunkel.

Aufruf aus dem Projektordner:  python software/schritt-07/tools/make-art.py
Braucht Python 3 mit Pillow (pip install pillow).
"""
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parents[1] / 'evi-cube' / 'evi_art.h'
NAMES = ['gluecklich', 'hungrig', 'traurig', 'schmutzig', 'schlafend']
TARGET_HEIGHT = 64   # das hoechste Bild fuellt genau die Displayhoehe
THRESHOLD = 140      # dunkler als dieser Grauwert = Linie = leuchtet


def convert(img, scale):
    w, h = round(img.width * scale), round(img.height * scale)
    white = Image.new('RGBA', img.size, (255, 255, 255, 255))
    lum = Image.alpha_composite(white, img).convert('L').resize((w, h), Image.BOX)
    alpha = img.split()[3].resize((w, h), Image.BOX)
    L, A = lum.load(), alpha.load()
    return w, h, [[A[x, y] > 127 and L[x, y] < THRESHOLD for x in range(w)] for y in range(h)]


def xbm_bytes(w, h, lit):
    data = []
    for y in range(h):
        for bx in range(0, w, 8):
            byte = 0
            for bit in range(8):
                if bx + bit < w and lit[y][bx + bit]:
                    byte |= 1 << bit   # XBM: linkes Pixel im niedrigsten Bit
            data.append(byte)
    return data


def main():
    images = [Image.open(ROOT / 'handy-app' / 'dist' / 'img' / f'adult-{n}.png').convert('RGBA')
              for n in NAMES]
    scale = TARGET_HEIGHT / max(i.height for i in images)
    lines = ['// Erzeugt von software/schritt-07/tools/make-art.py - nicht von Hand aendern.',
             '// Evis Zeichnungen (erwachsen), nur gleichmaessig verkleinert und in an/aus',
             '// umgerechnet. Format XBM fuer U8g2 drawXBMP, im Flash-Speicher.',
             '#ifndef TAMAGOTCHI_EVI_ART_H', '#define TAMAGOTCHI_EVI_ART_H', '',
             '#include <stdint.h>', '#include <avr/pgmspace.h>', '']
    total = 0
    for name, img in zip(NAMES, images):
        w, h, lit = convert(img, scale)
        data = xbm_bytes(w, h, lit)
        total += len(data)
        upper = name.upper()
        lines.append(f'const uint8_t ART_{upper}_W = {w};')
        lines.append(f'const uint8_t ART_{upper}_H = {h};')
        lines.append(f'const uint8_t ART_{upper}[] PROGMEM = {{')
        for i in range(0, len(data), 16):
            lines.append('    ' + ', '.join(f'0x{b:02x}' for b in data[i:i + 16]) + ',')
        lines.append('};')
        lines.append('')
    lines.append('#endif')
    OUT.write_text('\n'.join(lines) + '\n', encoding='utf-8')
    print(f'{OUT.name}: {len(NAMES)} Bilder, {total} Bytes, Massstab {scale:.4f}')


if __name__ == '__main__':
    main()
