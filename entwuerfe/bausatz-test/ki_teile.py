"""Lädt freistehende Graustufen-Teile aus ki/teile."""
from functools import lru_cache
from pathlib import Path

import numpy as np
from PIL import Image

ORDNER = Path(__file__).resolve().parent / 'ki' / 'teile'
RASTER = 8
ROLLEN = (34, 119, 170, 204, 238, 255)
SCHABLONEN = {
    'ohr': ((24, 32), (12, 29)),
    'ohr-haengend': ((24, 32), (12, 2)),
    'gesicht': ((40, 28), (20, 18)),
    'schwanz': ((32, 40), (3, 29)),
    # 3/4-Basis (Leinwand 112 x 112), Ansatz jeweils der Hals; weitere Ansätze siehe basis3_layout.py
    'koerper': ((80, 48), (40, 3)),
    'koerper-laufen': ((88, 48), (22, 8)),
    'koerper-huepfen': ((80, 52), (30, 8)),
    'koerper-geduckt': ((88, 44), (14, 22)),
    'koerper-eingerollt': ((88, 40), (24, 10)),
    'koerper-rundlich': ((72, 40), (36, 3)),
    'koerper-schlank': ((76, 56), (36, 3)),
    'koerper-kraeftig': ((84, 50), (42, 3)),
    'kopf-klein-rund': ((52, 40), (26, 36)),
    'kopf-breit': ((68, 44), (34, 40)),
    'merkmal': ((48, 32), (6, 26)),
    'merkmal-maehne': ((88, 68), (44, 38)),
    'merkmal-stacheln': ((48, 32), (6, 26)),
    'merkmal-panzer': ((56, 44), (6, 40)),
    'merkmal-fluegel': ((56, 56), (6, 50)),
    'merkmal-fluegel-fee': ((56, 56), (6, 50)),
    'merkmal-fluegel-drache': ((56, 56), (6, 50)),
    'aufsatz-schleife': ((24, 20), (12, 16)),
    'aufsatz-halsband': ((44, 14), (22, 3)),
    'aufsatz-augenmaske': ((52, 16), (26, 8)),
    'aufsatz-backenblitz': ((12, 14), (6, 7)),
    'aufsatz': ((24, 20), (12, 16)),
    'koerper-baby': ((56, 34), (28, 3)),
    'koerper-teenager': ((72, 44), (36, 3)),
    'koerper-senior': ((76, 46), (34, 8)),
    'kopf-baby': ((56, 44), (28, 40)),
    'kopf-teenager': ((58, 42), (29, 38)),
    'kopf-senior': ((58, 44), (29, 40)),
    'koerper-rundlich-baby': ((50, 28), (25, 3)),
    'koerper-rundlich-teenager': ((64, 36), (32, 3)),
    'koerper-rundlich-senior': ((68, 38), (30, 7)),
    'koerper-schlank-baby': ((54, 40), (26, 3)),
    'koerper-schlank-teenager': ((68, 50), (33, 3)),
    'koerper-schlank-senior': ((72, 52), (32, 8)),
    'koerper-kraeftig-baby': ((58, 36), (29, 3)),
    'koerper-kraeftig-teenager': ((76, 46), (38, 3)),
    'koerper-kraeftig-senior': ((80, 48), (36, 8)),
    'kopf-klein-rund-baby': ((48, 40), (24, 36)),
    'kopf-klein-rund-teenager': ((50, 38), (25, 34)),
    'kopf-klein-rund-senior': ((52, 40), (26, 36)),
    'kopf-breit-baby': ((60, 44), (30, 40)),
    'kopf-breit-teenager': ((64, 42), (32, 38)),
    'kopf-breit-senior': ((68, 44), (34, 40)),
    'koerper-laufen-baby': ((62, 34), (15, 6)),
    'koerper-huepfen-baby': ((56, 36), (21, 6)),
    'koerper-geduckt-baby': ((62, 30), (10, 15)),
    'koerper-eingerollt-baby': ((62, 28), (17, 7)),
    'koerper-laufen-teenager': ((80, 44), (20, 7)),
    'koerper-huepfen-teenager': ((72, 48), (27, 7)),
    'koerper-geduckt-teenager': ((80, 40), (13, 20)),
    'koerper-eingerollt-teenager': ((80, 36), (22, 9)),
    'koerper-laufen-senior': ((84, 46), (21, 8)),
    'koerper-huepfen-senior': ((76, 50), (29, 8)),
    'koerper-eingerollt-senior': ((84, 38), (23, 10)),
    'kopf': ((60, 44), (30, 40)),
}


@lru_cache(maxsize=None)
def lade_teil(art, name):
    dateien = sorted(ORDNER.glob(f'{art}-{name}-[123].png'))
    if not dateien:
        return None
    datei = dateien[0]
    (breite, hoehe), (ax, ay) = SCHABLONEN.get(f'{art}-{name}', SCHABLONEN[art])
    with Image.open(datei) as bild:
        if bild.size != (breite * RASTER, hoehe * RASTER):
            raise ValueError(f'{datei.name}: falsche Bildgröße {bild.size}')
        pixel = np.asarray(bild.convert('RGBA'))
    raster = pixel[::RASTER, ::RASTER, 0].copy()
    if np.any(pixel[..., 3] != 255) or not np.all(np.isin(raster, ROLLEN)):
        raise ValueError(f'{datei.name}: unerlaubte Graustufe oder Transparenz')
    erwartet = raster.repeat(RASTER, axis=0).repeat(RASTER, axis=1)
    if not np.all(pixel[..., :3] == erwartet[..., None]):
        raise ValueError(f'{datei.name}: Farbabweichung oder unsauberes 8×8-Raster')
    # Augen, Brauen, Nase und Mund sind bewusst voneinander getrennt.
    if art in ('gesicht', 'merkmal', 'aufsatz'):
        if np.all(raster == 255):
            raise ValueError(f'{datei.name}: Gesicht ist leer')
        # Einzelne Streupixel (ohne Nachbarn, auch diagonal) entfernen.
        belegt = (raster != 255).astype(int)
        rand = np.pad(belegt, 1)
        nachbarn = sum(rand[1 + dy:1 + dy + belegt.shape[0], 1 + dx:1 + dx + belegt.shape[1]]
                       for dy in (-1, 0, 1) for dx in (-1, 0, 1) if (dx, dy) != (0, 0))
        raster[(belegt == 1) & (nachbarn == 0)] = 255
        return raster, (ax, ay)
    if raster[ay, ax] == 255:
        raise ValueError(f'{datei.name}: Ansatzpunkt liegt außerhalb des Teils')

    # Nur die mit dem Ansatz verbundene Form übernehmen.
    verbunden = np.zeros(raster.shape, dtype=bool)
    offen = [(ax, ay)]
    verbunden[ay, ax] = True
    while offen:
        x, y = offen.pop()
        for nx, ny in ((x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)):
            if 0 <= nx < breite and 0 <= ny < hoehe and not verbunden[ny, nx] and raster[ny, nx] != 255:
                verbunden[ny, nx] = True
                offen.append((nx, ny))
    raster[~verbunden] = 255
    return raster, (ax, ay)
