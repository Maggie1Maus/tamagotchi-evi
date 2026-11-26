#!/usr/bin/env python3
"""Packt die Dorfbilder (entwuerfe/dorf/weihnachten/dorf-weihnachten.zip) für die Firmware: 15 Ansichten
(Tag/Dämmerung/Nacht × Sonne/Sonne+Wolken/bewölkt/Regen/Schnee), je 320 × 174, 16 Graustufen, ohne Maske.
Ausgabe: evi-handy/dorfbilder.h. Die Bilder werden 1:1 übernommen. Aufruf: python3 tools/make-dorf-grau.py"""

from io import BytesIO
from pathlib import Path
from zipfile import ZipFile

from PIL import Image

STEP = Path(__file__).resolve().parents[1]
ZIP = STEP.parents[1] / "entwuerfe/dorf/weihnachten/dorf-weihnachten.zip"
AUS = STEP / "evi-handy/dorfbilder.h"
ZEITEN = ["tag", "daemmerung", "nacht"]
WETTER = ["sonne", "sonne-wolken", "bewoelkt", "regen", "schnee"]   # Reihenfolge wie enum Wetter (WT_SONNE, WT_SONNE_WOLKEN, WT_REGEN, WT_BEWOELKT, WT_SCHNEE) siehe unten
BREITE, HOEHE = 320, 174
# Reihenfolge im Programm: WT_SONNE, WT_SONNE_WOLKEN, WT_REGEN, WT_BEWOELKT, WT_SCHNEE
WETTER_PROGRAMM = ["sonne", "sonne-wolken", "regen", "bewoelkt", "schnee"]


def stufen(daten):
    with Image.open(BytesIO(daten)) as bild:
        if bild.format != "PNG" or bild.size != (BREITE, HOEHE):
            raise ValueError("erwartet PNG 320 x 174")
        grau = bild.convert("L")
        punkte = list(grau.get_flattened_data() if hasattr(grau, "get_flattened_data") else grau.getdata())
    if any(p % 17 for p in punkte):
        raise ValueError("erwartet 16 Graustufen")
    return [p // 17 for p in punkte]


def packen(s):
    daten = bytearray((len(s) + 1) // 2)
    for i, v in enumerate(s):
        daten[i // 2] |= v << 4 if i % 2 == 0 else v
    return daten


def main():
    zeilen = ["// Erzeugt von tools/make-dorf-grau.py aus entwuerfe/dorf/weihnachten/dorf-weihnachten.zip. Nicht von Hand ändern.",
              "// Dorf zur Weihnachtszeit: 15 Ansichten, 320 x 174, 16 Graustufen (halbbytes, erste Spalte oben links hoch).", "#pragma once", ""]
    namen = {}
    with ZipFile(ZIP) as z:
        for zeit in ZEITEN:
            for wetter in WETTER_PROGRAMM:
                name = f"{zeit}-{wetter}.png"
                s = stufen(z.read(name))
                bezeichner = f"DORF_WEIH_{zeit.upper()}_{wetter.upper().replace('-', '_')}"
                namen[(zeit, wetter)] = bezeichner
                daten = packen(s)
                zeilen.append(f"static constexpr uint8_t {bezeichner}_GRAU[] = {{")
                for i in range(0, len(daten), 32):
                    zeilen.append("  " + ",".join(f"0x{b:02x}" for b in daten[i:i + 32]) + ",")
                zeilen.append("};")
                zeilen.append(f"static constexpr ZimmerBild {bezeichner} = {{{BREITE}, {HOEHE}, {bezeichner}_GRAU, nullptr}};")
    zeilen.append("// [Himmel: Tag, Dämmerung, Nacht][Wetter: Sonne, Sonne+Wolken, Regen, bewölkt, Schnee] (Reihenfolge wie enum Himmel und enum Wetter)")
    zeilen.append("static constexpr const ZimmerBild* DORF_WEIHNACHT[3][5] = {")
    for zeit in ZEITEN:
        zeilen.append("  {" + ", ".join("&" + namen[(zeit, w)] for w in WETTER_PROGRAMM) + "},")
    zeilen.append("};")
    AUS.write_text("\n".join(zeilen) + "\n", encoding="utf-8")
    print("geschrieben:", AUS, AUS.stat().st_size, "Byte")


if __name__ == "__main__":
    main()
