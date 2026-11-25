#!/usr/bin/env python3
"""Packt die 80 Graustufen-Bilder der anderen Tamagotchis für den Park (besucherbilder.h).

Quelle: entwuerfe/neue-tamagotchis/graustufen/<name>/<phase>-gluecklich.png (72 x 64, 16 Graustufen,
Alpha 0/255, gemeinsame Standlinie in Zeile 61). Die Bilder werden 1:1 übernommen, nur auf den
gezeichneten Bereich zugeschnitten. Braucht kein Pillow:

  python3 software/schritt-10/tools/make-besucher-grau.py
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pngklein  # noqa: E402

STEP = Path(__file__).resolve().parents[1]
QUELLE = STEP.parents[1] / "entwuerfe/neue-tamagotchis/graustufen"
BREITE, HOEHE, STANDLINIE = 72, 64, 61
PHASEN = ("baby", "teenager", "erwachsen", "senior")
# Lotti zuerst: Sie ist Evis Freundin und am häufigsten im Park.
FIGUREN = (
    ("lotti", "Lotti"), ("bit", "Bit"), ("blip", "Blip"), ("blubber", "Blubber"), ("ella", "Ella"),
    ("funke", "Funke"), ("gloeckchen", "Glöckchen"), ("groschen", "Groschen"), ("kiwi", "Kiwi"),
    ("klecksi", "Klecksi"), ("leo", "Leo"), ("linus", "Linus"), ("lumi", "Lumi"), ("mampf", "Mampf"),
    ("mia", "Mia"), ("mimi", "Mimi"), ("moosi", "Moosi"), ("rolli", "Rolli"), ("spuki", "Spuki"),
    ("tetro", "Tetro"),
)


def erzeugen():
    out = ["// Erzeugt von tools/make-besucher-grau.py aus entwuerfe/neue-tamagotchis/graustufen/.",
           "#pragma once", "",
           "// Ausschnitt eines 72 x 64 großen Bildes; x/y = linke obere Ecke im Gesamtbild.",
           "struct BesuchBild { uint8_t x, y; ZimmerBild bild; };",
           f"constexpr int BESUCH_BREITE = {BREITE}, BESUCH_HOEHE = {HOEHE}, BESUCH_STANDLINIE = {STANDLINIE};",
           ""]

    def tabelle(name, daten):
        out.append(f"static constexpr uint8_t {name}[] = {{")
        for i in range(0, len(daten), 24):
            out.append("  " + ",".join(f"0x{b:02x}" for b in daten[i:i + 24]) + ",")
        out.append("};")

    zeilen = []
    for datei, _ in FIGUREN:
        eintraege = []
        for phase in PHASEN:
            pfad = QUELLE / datei / f"{phase}-gluecklich.png"
            w, h, punkte = pngklein.lesen(pfad)
            if (w, h) != (BREITE, HOEHE):
                raise ValueError(f"{pfad}: erwartet {BREITE} x {HOEHE}")
            if any(r != g or r != b or r % 17 or a not in (0, 255) for r, g, b, a in punkte):
                raise ValueError(f"{pfad}: erwartet 16 Graustufen und Alpha 0/255")
            belegt = [i for i, p in enumerate(punkte) if p[3]]
            x0, x1 = min(i % w for i in belegt), max(i % w for i in belegt)
            y0, y1 = min(i // w for i in belegt), max(i // w for i in belegt)
            if y1 != STANDLINIE:
                raise ValueError(f"{pfad}: Standlinie {y1} statt {STANDLINIE}")
            cw, ch = x1 - x0 + 1, y1 - y0 + 1
            aus = [punkte[(y0 + y) * w + x0 + x] for y in range(ch) for x in range(cw)]
            grau = bytearray((len(aus) + 1) // 2)
            maske = bytearray((len(aus) + 7) // 8)
            for i, (r, _, _, a) in enumerate(aus):
                if a:
                    grau[i // 2] |= (r // 17) << (0 if i % 2 else 4)
                    maske[i // 8] |= 128 >> (i % 8)
            name = f"BESUCH_{datei.upper()}_{phase.upper()}"
            tabelle(name + "_GRAU", grau)
            tabelle(name + "_MASKE", maske)
            eintraege.append(f"{{{x0}, {y0}, {{{cw}, {ch}, {name}_GRAU, {name}_MASKE}}}}")
        zeilen.append("  {" + ", ".join(eintraege) + "},")
    out.append(f"constexpr int BESUCHER = {len(FIGUREN)};")
    out.append("static constexpr BesuchBild BESUCH_BILD[BESUCHER][4] = {")
    out.extend(zeilen)
    out.append("};")
    out.append("const char* const BESUCH_NAME[BESUCHER] = {"
               + ", ".join(f'"{anzeige}"' for _, anzeige in FIGUREN) + "};")
    with open(STEP / "evi-handy/besucherbilder.h", "w", encoding="utf-8", newline="\n") as datei:
        datei.write("\n".join(out) + "\n")


if __name__ == "__main__":
    erzeugen()
