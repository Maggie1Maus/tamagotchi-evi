#!/usr/bin/env python3
"""Packt die bestätigten Park-PNGs und ihre Himmelsmasken für die Firmware."""

from collections import deque
from io import BytesIO
from pathlib import Path
from zipfile import ZipFile

from PIL import Image


STEP = Path(__file__).resolve().parents[1]
QUELLE = STEP.parents[1] / "entwuerfe/park/entwurf-2"
ALT = STEP.parents[1] / "entwuerfe/park/entwurf-1"
BREITE, HOEHE = 320, 174
HIMMEL_H = 74


def lesen(datei, groesse, quelle=QUELLE):
    with Image.open(quelle / datei) as bild:
        if bild.format != "PNG" or bild.size != groesse:
            raise ValueError(f"{datei}: erwartet PNG {groesse}")
        rgba = bild.convert("RGBA")
        punkte = list(rgba.get_flattened_data() if hasattr(rgba, "get_flattened_data") else rgba.getdata())
    if any(r != g or r != b or r % 17 or a not in (0, 255) for r, g, b, a in punkte):
        raise ValueError(f"{datei}: erwartet 16 Graustufen und Alpha 0/255")
    return punkte


def maske_packen(deckung):
    daten = bytearray((len(deckung) + 7) // 8)
    for i, aktiv in enumerate(deckung):
        if aktiv:
            daten[i // 8] |= 128 >> (i % 8)
    return daten


def himmel(punkte, winter=False):
    # Helle Flächen, die vom Bildrand aus erreichbar sind. Konturen schützen Äste und Baumkronen.
    offen = [r >= (187 if i // BREITE < 56 else 204)
             for i, (r, _, _, _) in enumerate(punkte[:BREITE * HIMMEL_H])]
    if winter:
        # Weiße Schneekanten liegen unmittelbar über dunklen Ästen. Sie sind
        # Landschaft und dürfen beim Wechsel zum schwarzen Himmel nicht verschwinden.
        for y in range(HIMMEL_H - 2):
            for x in range(32, 156):
                i = y * BREITE + x
                if punkte[i][0] >= 221 and any(
                        punkte[(y + dy) * BREITE + x][0] < 136 for dy in (1, 2)):
                    offen[i] = False
    sichtbar = bytearray(len(offen))
    wartend = deque()

    def aufnehmen(x, y):
        if 0 <= x < BREITE and 0 <= y < HIMMEL_H:
            i = y * BREITE + x
            if offen[i] and not sichtbar[i]:
                sichtbar[i] = 1
                wartend.append((x, y))

    for x in range(BREITE):
        # Die Sommerkrone ragt oben aus dem Bild. Helle Kronenpixel am
        # oberen Rand sind keine Startpunkte für die Himmelssuche.
        if x < 32 or x >= 156:
            aufnehmen(x, 0)
    for y in range(1, HIMMEL_H):
        aufnehmen(0, y)
        aufnehmen(BREITE - 1, y)
    while wartend:
        x, y = wartend.popleft()
        aufnehmen(x - 1, y)
        aufnehmen(x + 1, y)
        aufnehmen(x, y - 1)
        aufnehmen(x, y + 1)
    return sichtbar


def erzeugen():
    out = ["// Erzeugt von tools/make-park-grau.py.", "#pragma once", ""]

    def tabelle(name, daten):
        out.append(f"static constexpr uint8_t {name}[] = {{")
        for i in range(0, len(daten), 24):
            out.append("  " + ",".join(f"0x{b:02x}" for b in daten[i:i + 24]) + ",")
        out.append("};")

    def export(name, punkte, w, h):
        deckung = [a == 255 for _, _, _, a in punkte]
        grau = bytearray((len(punkte) + 1) // 2)
        for i, (r, _, _, a) in enumerate(punkte):
            if a:
                grau[i // 2] |= (r // 17) << (0 if i % 2 else 4)
        tabelle(f"{name}_GRAU", grau)
        alpha = "nullptr"
        if not all(deckung):
            alpha = f"{name}_MASKE"
            tabelle(alpha, maske_packen(deckung))
        out.append(f"static constexpr ZimmerBild {name} = {{{w}, {h}, {name}_GRAU, {alpha}}};")

    namen = ("FRUEHLING", "SOMMER", "HERBST", "WINTER")
    for saison in namen:
        pfad = f"park-{saison.lower()}.png"
        punkte = lesen(pfad, (BREITE, HOEHE))
        if any(a != 255 for _, _, _, a in punkte):
            raise ValueError(f"{pfad}: Parkhintergrund muss deckend sein")
        export(f"PARK_{saison}", punkte, BREITE, HOEHE)
        tabelle(f"PARK_HIMMEL_{saison}", maske_packen(himmel(punkte, saison == "WINTER")))
    for motiv in ("LATERNE", "STERN"):
        with Image.open(QUELLE / f"{motiv.lower()}.png") as im:
            breite, hoehe = im.size
        punkte = lesen(f"{motiv.lower()}.png", (breite, hoehe))
        export(f"PARK_{motiv}", punkte, breite, hoehe)
        if motiv == "LATERNE":
            # Die hellen Glasscheiben im oberen Gehäuse, ohne Metallrahmen.
            glas = [a == 255 and r >= 187 and .21 <= (i % breite) / breite <= .82
                    and .145 <= (i // breite) / hoehe <= .255
                    for i, (r, _, _, a) in enumerate(punkte)]
            tabelle("PARK_LATERNENGLAS", maske_packen(glas))
    export("PARK_LOTTI", lesen("lotti-graustufen.png", (33, 54), ALT), 33, 54)
    evi_zip = STEP.parents[1] / "entwuerfe/evi/graustufen-vorschau/Evi-Graustufen.zip"
    phasen = ("02-Baby", "03-Teenager", "04-Erwachsen", "05-Oma")
    evi = []
    with ZipFile(evi_zip) as archiv:
        for phase in phasen:
            posen = []
            for pose in ("schnuppern", "untersuchen", "zeigen"):
                name = f"PARK_EVI_{phase[3:].upper()}_{pose.upper()}"
                with Image.open(BytesIO(archiv.read(f"Sprites/{phase}/garten-{pose}.png"))) as bild:
                    w, h = bild.size
                    grau = bild.convert("LA")
                    punkte = []
                    for g, alpha in (grau.get_flattened_data() if hasattr(grau, "get_flattened_data") else grau.getdata()):
                        wert = min(255, ((g + 8) // 17) * 17)
                        punkte.append((wert, wert, wert, 255 if alpha >= 128 else 0))
                export(name, punkte, w, h)
                posen.append("&" + name)
            evi.append("  {" + ", ".join(posen) + "},")
    out.extend(["static constexpr const ZimmerBild* PARK_EVI[][3] = {", *evi, "};"])
    out.append("static constexpr const ZimmerBild* PARK_BODEN[] = {"
               + ", ".join(f"&PARK_{name}" for name in namen) + "};")
    out.append("static constexpr const uint8_t* PARK_HIMMEL[] = {"
               + ", ".join(f"PARK_HIMMEL_{name}" for name in namen) + "};")
    out.append(f"static constexpr int PARK_HIMMEL_H = {HIMMEL_H};")
    (STEP / "evi-handy/parkbilder.h").write_text("\n".join(out) + "\n", encoding="utf-8", newline="\n")


if __name__ == "__main__":
    erzeugen()
