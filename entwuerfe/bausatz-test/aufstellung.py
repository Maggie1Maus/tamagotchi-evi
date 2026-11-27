#!/usr/bin/env python3
"""Variationstest: alle 21 Figuren als Bausatz-Rezept (mit den bisherigen Teilen). -> pruefbogen-aufstellung.png"""
from PIL import Image, ImageDraw
from basis3 import baue, W, H, HIER

# name: (körper, kopf, krause, ohren, schwanz, gesicht, fell, kranz, muster, fehlt)
F = {
 'Evi':      (2, 2, 2, 'spitz', 'buschig', 'freundlich', 187, 238, None, ''),
 'Lotti':    (1, 1, 1, 'spitz', 'puschel', 'freundlich', 170, 255, 'tupfen', 'Rehohren, Stummelschwanz'),
 'Ella':     (3, 3, 3, 'haengend', 'duenn', 'freundlich', 187, 221, None, 'Rüssel'),
 'Glöckchen':(2, 1, 2, 'haengend', 'puschel', 'lachend', 238, 255, None, 'Glöckchen'),
 'Kiwi':     (3, 2, 1, None, 'blitz', 'freundlich', 153, 221, 'tupfen', 'Dino-Zacken'),
 'Klecksi':  (1, 3, 2, 'haengend', 'duenn', 'lachend', 255, 255, 'flecken', 'Halsband'),
 'Lumi':     (2, 1, 3, 'buschel', 'puschel', 'lachend', 221, 255, None, 'Flügel'),
 'Mia':      (2, 2, 2, 'spitz', 'duenn', 'freundlich', 238, 255, None, 'Schleife'),
 'Mimi':     (3, 3, 1, None, None, 'muede', 204, 238, None, 'Schnecke: anderes Tier'),
 'Moosi':    (1, 2, 3, 'rund', 'puschel', 'freundlich', 136, 153, 'streifen', 'Blätter'),
 'Spuki':    (2, 3, 2, None, None, 'ueberrascht', 255, 255, None, 'Geist: schwebend'),
 'Bit':      (3, 1, 1, 'lang', 'blitz', 'freundlich', 119, 170, None, 'Antennen, Roboter'),
 'Blip':     (1, 3, 3, None, 'duenn', 'ueberrascht', 204, 238, None, 'Antennen'),
 'Blubber':  (2, 1, 2, 'rund', 'blitz', 'wuetend', 170, 119, None, 'Flügel, Drachenkopf'),
 'Funke':    (3, 2, 3, 'buschel', 'buschig', 'lachend', 204, 255, None, 'Backenblitze'),
 'Groschen': (1, 1, 1, 'rund', 'buschig', 'freundlich', 153, 204, 'streifen', 'Augenmaske, Ringe'),
 'Leo':      (2, 3, 2, 'rund', 'duenn', 'freundlich', 204, 136, None, 'Mähne, Quaste'),
 'Linus':    (3, 2, 1, 'lang', 'puschel', 'freundlich', 255, 255, None, ''),
 'Mampf':    (1, 1, 3, 'rund', 'puschel', 'lachend', 221, 238, None, 'Backentaschen'),
 'Rolli':    (2, 3, 1, None, 'puschel', 'freundlich', 136, 153, None, 'Stacheln'),
 'Tetro':    (3, 1, 2, None, 'duenn', 'muede', 119, 136, None, 'Panzer'),
}
if __name__ == '__main__':
    z, cols = 2, 7
    rows = (len(F) + cols - 1) // cols
    out = Image.new('RGB', (cols * W * z, rows * (H * z + 30)), 'white'); d = ImageDraw.Draw(out)
    for i, (n, (k, h, kr, o, s, g, f, kz, m, fehlt)) in enumerate(F.items()):
        c = baue(k, h, kr, ohren=o, schwanz=s, gesicht=g, fell=f, kranz=kz, muster=m)
        x, y = (i % cols) * W * z, (i // cols) * (H * z + 30)
        d.text((x + 4, y + 1), n, fill='black')
        if fehlt: d.text((x + 4, y + 13), 'fehlt: ' + fehlt, fill=(160, 0, 0))
        out.paste(c.bild(255, z), (x, y + 28))
    out.save(HIER / 'pruefbogen-aufstellung.png'); print(out.size)
