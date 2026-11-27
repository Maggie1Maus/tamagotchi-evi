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
    if art == 'gesicht':
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
