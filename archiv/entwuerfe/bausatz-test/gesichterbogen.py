#!/usr/bin/env python3
"""Zeigt alle Gesichtsteile nebeneinander, vergrößert und in Originalgröße."""
from pathlib import Path

from PIL import Image, ImageDraw

from bausatz2 import H, W, tiere
from ki_teile import lade_teil

AUSDRUECKE = {
    'freundlich': 'Freundlich',
    'lachend': 'Lachend',
    'ueberrascht': 'Überrascht',
    'traurig': 'Traurig',
    'wuetend': 'Wütend',
    'muede': 'Müde',
}


def pruefbogen():
    breite, hoehe = 3 * W + 16, 2 * H + 24
    bogen = Image.new('L', (3 * breite, 2 * hoehe), 255)
    text = ImageDraw.Draw(bogen)
    for i, (name, label) in enumerate(AUSDRUECKE.items()):
        if lade_teil('gesicht', name) is None:
            raise FileNotFoundError(f'Gesichtsteil fehlt: {name}')
        x, y = i % 3 * breite, i // 3 * hoehe
        text.text((x + 8, y + 4), label, fill=17)
        bogen.paste(tiere(gesicht=name).bild(z=2), (x + 8, y + 20))
        bogen.paste(tiere(gesicht=name, f=136, bauch=187).bild(z=1), (x + 2 * W + 8, y + H + 20))
    return bogen


if __name__ == '__main__':
    pruefbogen().save(Path(__file__).with_name('pruefbogen-gesichter.png'))
