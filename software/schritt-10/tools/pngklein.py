"""Sehr kleiner PNG-Leser ohne Pillow (8 Bit, RGBA/RGB/Grau/Grau+Alpha).

Auf dem Haupt-PC gibt es kein Python mit Pillow, nur das Python 3.7 aus dem
Arduino-Paket (rp2040/tools/pqt-python3). Damit laufen Werkzeuge, die nur
diesen Leser brauchen, auch dort.
"""

import struct
import zlib


def lesen(pfad):
    """Gibt (breite, hoehe, punkte) zurück; punkte ist eine Liste von (r, g, b, a)."""
    with open(pfad, "rb") as datei:
        daten = datei.read()
    if daten[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError(f"{pfad}: kein PNG")
    pos, idat = 8, b""
    w = h = farbtyp = None
    while pos < len(daten):
        laenge, art = struct.unpack(">I4s", daten[pos:pos + 8])
        inhalt = daten[pos + 8:pos + 8 + laenge]
        pos += 12 + laenge
        if art == b"IHDR":
            w, h, tiefe, farbtyp, _, _, zeilensprung = struct.unpack(">IIBBBBB", inhalt)
            if tiefe != 8 or zeilensprung or farbtyp not in (0, 2, 4, 6):
                raise ValueError(f"{pfad}: nur 8 Bit ohne Palette und ohne Zeilensprung")
        elif art == b"IDAT":
            idat += inhalt
        elif art == b"IEND":
            break
    kanaele = {0: 1, 2: 3, 4: 2, 6: 4}[farbtyp]
    roh = zlib.decompress(idat)
    zeile = w * kanaele
    vorher = bytearray(zeile)
    punkte = []
    for y in range(h):
        filt = roh[y * (zeile + 1)]
        akt = bytearray(roh[y * (zeile + 1) + 1:(y + 1) * (zeile + 1)])
        for i in range(zeile):
            a = akt[i - kanaele] if i >= kanaele else 0
            b = vorher[i]
            c = vorher[i - kanaele] if i >= kanaele else 0
            if filt == 1:
                akt[i] = (akt[i] + a) & 255
            elif filt == 2:
                akt[i] = (akt[i] + b) & 255
            elif filt == 3:
                akt[i] = (akt[i] + (a + b) // 2) & 255
            elif filt == 4:
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                akt[i] = (akt[i] + (a if pa <= pb and pa <= pc else b if pb <= pc else c)) & 255
        for x in range(w):
            k = akt[x * kanaele:(x + 1) * kanaele]
            if farbtyp == 0:
                punkte.append((k[0], k[0], k[0], 255))
            elif farbtyp == 4:
                punkte.append((k[0], k[0], k[0], k[1]))
            elif farbtyp == 2:
                punkte.append((k[0], k[1], k[2], 255))
            else:
                punkte.append(tuple(k))
        vorher = akt
    return w, h, punkte
