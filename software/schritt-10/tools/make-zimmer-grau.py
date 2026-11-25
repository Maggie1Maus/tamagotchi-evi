#!/usr/bin/env python3
"""Exportiert die Spiel-PNGs ohne Größen- oder Helligkeitsänderung als Zimmergrafiken."""

from pathlib import Path

from PIL import Image


STEP = Path(__file__).resolve().parents[1]
SOURCE = STEP.parents[1] / "entwuerfe/zimmer/graustufen/spielgrafiken"
OUTPUT = STEP / "evi-handy/zimmergrau.h"
ZEIT = ("tag", "daemmerung", "nacht")
WETTER = ("klar", "leicht-bewoelkt", "regen", "bedeckt", "schneefall")
JAHRESZEIT = ("fruehling", "sommer", "herbst", "winter")
GLAS = (66, 53)


def lesen(pfad, groesse):
    with Image.open(SOURCE / (pfad + ".png")) as bild:
        if bild.format != "PNG" or bild.size != groesse:
            raise ValueError(f"{pfad}: {bild.format} {bild.size}, erwartet PNG {groesse}")
        rgba = bild.convert("RGBA")
        punkte = list(rgba.get_flattened_data() if hasattr(rgba, "get_flattened_data") else rgba.getdata())
    if any(r != g or r != b or r % 17 or a not in (0, 255) for r, g, b, a in punkte):
        raise ValueError(f"{pfad}: erwartet 16 Graustufen mit Alpha 0/255")
    return punkte


def tabelle(name, zeilen):
    return [f"static constexpr const ZimmerBild* {name}[][{len(zeilen[0])}] = {{",
            *("  {" + ", ".join("&" + eintrag for eintrag in zeile) + "}," for zeile in zeilen),
            "};"]


def erzeugen():
    ausgabe = [
        "// Erzeugt von tools/make-zimmer-grau.py aus den Spiel-PNGs.",
        "// 4 Bit je Graustufe, 1 Bit Deckkraft. Alle Zeilen liegen ohne Füllbits hintereinander.",
        "#pragma once",
        "",
    ]
    bilder = {}

    def export(pfad, groesse, name=None):
        name = name or "ZG_" + pfad.split("/")[-1].replace("-", "_").upper()
        punkte = lesen(pfad, groesse)
        if name in bilder:
            raise ValueError(f"Doppelter Bildname: {name}")
        bilder[name] = punkte
        deckend = all(p[3] == 255 for p in punkte)
        if pfad.startswith(("zimmer/", "fenster/himmel-")) and not deckend:
            raise ValueError(f"{pfad}: Hintergrund muss vollständig deckend sein")
        grau = bytearray((len(punkte) + 1) // 2)
        maske = bytearray((len(punkte) + 7) // 8)
        for i, (g, _, _, alpha) in enumerate(punkte):
            if alpha:
                grau[i // 2] |= (g // 17) << (0 if i % 2 else 4)
                maske[i // 8] |= 128 >> (i % 8)
        ausgabe.append(f"// {pfad}.png")
        for suffix, daten in (("GRAU", grau), ("MASKE", maske)):
            if suffix == "MASKE" and deckend:
                break
            ausgabe.append(f"static constexpr uint8_t {name}_{suffix}[] = {{")
            for i in range(0, len(daten), 24):
                ausgabe.append("  " + ",".join(f"0x{b:02x}" for b in daten[i:i + 24]) + ",")
            ausgabe.append("};")
        deckung = "nullptr" if deckend else name + "_MASKE"
        w, h = groesse
        ausgabe.append(f"static constexpr ZimmerBild {name} = {{{w}, {h}, {name}_GRAU, {deckung}}};")
        ausgabe.append("")
        return name

    raum = [export("zimmer/wand-boden" + ende, (320, 174))
            for ende in ("", "-daemmerung", "-nacht")]
    for name, groesse in (
        ("tierbett", (81, 45)), ("tierbett-vorderrand", (81, 14)), ("sideboard", (102, 51)),
        ("regal", (93, 20)), ("teppich", (130, 36)), ("fensterrahmen", (81, 69)),
    ):
        export("moebel/" + name, groesse)

    himmel = [[export(f"fenster/himmel-{zeit}-{wetter}", GLAS)
               for wetter in WETTER] for zeit in ZEIT]
    zeichen = [export(f"fenster/himmelszeichen-{zeit}", GLAS) for zeit in ZEIT]
    wolken = [[export(f"fenster/wolken-{wetter}-{zeit}", GLAS)
               for wetter in WETTER] for zeit in ZEIT]
    baeume = [[export(f"fenster/baum-{saison}{ende}", GLAS) for saison in JAHRESZEIT]
              for ende in ("", "-hell", "-dunkel")]
    for name in ("huegel", "laub-herbst", "schnee-boden", "schnee-aeste"):
        export("fenster/" + name, GLAS)
    regen = [export(f"fenster/regen-{i}", GLAS) for i in range(16)]
    schnee = [export(f"fenster/schnee-{i}", GLAS) for i in range(16)]
    for name, groesse in (
        ("herbstblatt", (5, 6)), ("wetter-sonne", (22, 22)), ("wetter-sonne-wolke", (26, 24)),
        ("wetter-wolke", (27, 21)), ("wetter-regenwolke", (27, 17)), ("mond", (16, 17)),
    ):
        export("fenster/" + name, groesse)

    if bilder["ZG_TIERBETT"][31 * 81:] != bilder["ZG_TIERBETT_VORDERRAND"]:
        raise ValueError("Der Vorderrand muss dem Tierbett ab der Zeile 31 entsprechen")
    for y in range(53):
        for x in range(66):
            alpha = bilder["ZG_FENSTERRAHMEN"][(y + 7) * 81 + x + 7][3]
            if bool(alpha) != (31 <= x < 35 or 26 <= y < 30):
                raise ValueError("Der Fensterrahmen passt nicht zur Glasfläche (7, 7)")

    for name, eintraege in (("ZG_RAUM", raum), ("ZG_ZEICHEN", zeichen),
                            ("ZG_REGEN", regen), ("ZG_SCHNEE", schnee)):
        ausgabe.append(f"static constexpr const ZimmerBild* {name}[] = {{"
                       + ", ".join("&" + eintrag for eintrag in eintraege) + "};")
    for name, zeilen in (("ZG_HIMMEL", himmel), ("ZG_WOLKEN", wolken), ("ZG_BAEUME", baeume)):
        ausgabe.extend(tabelle(name, zeilen))

    OUTPUT.write_text("\n".join(ausgabe) + "\n", encoding="utf-8", newline="\n")
    print(f"{len(bilder)} Bilder mit 16 Graustufen und separater Transparenz.")


if __name__ == "__main__":
    erzeugen()
