"""Vergleicht die Fensterfotos der PC-Nachbildung pixelgenau mit den Spiel-PNGs."""

from math import pi, sin
from pathlib import Path
import sys

from PIL import Image, ImageChops


QUELLE = Path(__file__).resolve().parents[3] / "entwuerfe/zimmer/graustufen/spielgrafiken"
FENSTER = Path(__file__).resolve().parents[3] / "entwuerfe/zimmer/fenster-hell-2026-11-26/ebenen"
JAHRESZEITEN = ("fruehling", "sommer", "herbst", "winter")
TAGESZEITEN = ("tag", "daemmerung", "nacht")
WETTER = ("klar", "leicht-bewoelkt", "regen", "bedeckt", "schneefall")


def pruefen(fotos):
    ebenen = {}

    def png(pfad):
        if pfad not in ebenen:
            datei = QUELLE / (pfad + ".png")
            if pfad.startswith("fenster/") and (FENSTER / (pfad[8:] + ".png")).exists():
                datei = FENSTER / (pfad[8:] + ".png")
            with Image.open(datei) as bild:
                ebenen[pfad] = bild.convert("RGBA")
        return ebenen[pfad]

    kombinationen = anzahl = 0
    for j, saison in enumerate(JAHRESZEITEN):
        for h, zeit in enumerate(TAGESZEITEN):
            for w, wetter in enumerate(WETTER):
                if wetter == "schneefall" and saison != "winter":
                    continue
                kombinationen += 1
                phasen = 16 if wetter in ("regen", "schneefall") or saison == "herbst" else 1
                for phase in range(phasen):
                    # Außen je ein Pixel, um Überzeichnen neben dem Rahmen zu erkennen.
                    fenster = Image.new("RGBA", (83, 71), "white")
                    glas = png(f"fenster/himmel-{zeit}-{wetter}").copy()
                    voreinander = []
                    if wetter in ("klar", "leicht-bewoelkt"):
                        voreinander.append(f"himmelszeichen-{zeit}")
                    voreinander.extend((f"wolken-{wetter}-{zeit}", f"huegel-{zeit}"))
                    if saison == "winter":
                        voreinander.append(f"schnee-boden-{zeit}")
                    voreinander.append(f"baum-{saison}" + ("", "-hell", "-dunkel")[h])
                    if saison == "herbst":
                        voreinander.append(f"laub-herbst-{zeit}")
                    for name in voreinander:
                        glas.alpha_composite(png("fenster/" + name))
                    if saison == "herbst":
                        pos = (19 + round(sin(phase * pi / 8) * 2), 28 + phase * 18 // 16)
                        glas.alpha_composite(png("fenster/herbstblatt"), pos)
                    if wetter in ("regen", "schneefall"):
                        animation = "regen" if wetter == "regen" else "schnee"
                        glas.alpha_composite(png(f"fenster/{animation}-{phase}"))
                    fenster.alpha_composite(glas, (8, 8))
                    fenster.alpha_composite(png("moebel/fensterrahmen"), (1, 1))
                    pfad = fotos / f"grau-fenster-{j}-{h}-{w}-{phase}.pgm"
                    with Image.open(pfad) as foto:
                        unterschied = ImageChops.difference(fenster.convert("L"), foto.convert("L")).getbbox()
                    if unterschied:
                        raise AssertionError(f"{pfad.name}: Abweichung im Bereich {unterschied}")
                    anzahl += 1
    print(f"OK: {kombinationen} Fensterkombinationen, {anzahl} Ansichten mit allen Animationsphasen.")


if __name__ == "__main__":
    pruefen(Path(sys.argv[1]))
