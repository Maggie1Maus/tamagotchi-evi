#!/usr/bin/env python3
"""Erzeugt evi-handy/evigrau.h aus den Graustufen-Sprites der Nutzerin.

Quelle: entwuerfe/evi/graustufen-vorschau/Evi-Graustufen.zip (Ordner Sprites/, Bilder mit Alpha).
Jeder Sprite wird 1:1 übernommen (nur 15 Graustufen, durchsichtig = eigener Wert), nichts wird neu gezeichnet.
Ablage im Programm: 4 Bit je Punkt, zwei Punkte je Byte (oberes Halbbyte zuerst). Wert 0–14 = Grau (0 schwarz,
14 weiß), 15 = durchsichtig. Jeder Sprite gehört zu einem vorhandenen Schwarz-Weiß-Bild (gleiche Größe), das
weiter für Trefferprüfung und Verdeckung gebraucht wird.
Aufruf: python3 software/schritt-10/tools/make-evi-grau.py
"""
import io, re, sys, zipfile, pathlib
from PIL import Image

HIER = pathlib.Path(__file__).resolve().parent
REPO = HIER.parents[2]
ZIP = REPO / "entwuerfe/evi/graustufen-vorschau/Evi-Graustufen.zip"
NEU = REPO / "entwuerfe/evi/graustufen-neu"   # neue Bilder aus Evis eigenen Teilen: haben Vorrang vor der ZIP
SKETCH = HIER.parent / "evi-handy"
AUS = SKETCH / "evigrau.h"

PHASEN = {"02-Baby": ("BABY", "BABY", "BABY"), "03-Teenager": ("TEEN", "TEEN", "TEEN"),
          "04-Erwachsen": ("ERWACHSEN", "ADULT", "ERWACHSEN"), "05-Oma": ("OMA", "SENIOR", "OMA")}
ZUSTAND = {"ruhe": "BASE", "gluecklich": "GLUECKLICH", "hungrig": "HUNGRIG", "traurig": "TRAURIG", "muede": "MUEDE",
           "krank": "KRANK", "schmutzig": "SCHMUTZIG", "schlafend": "SCHLAFEND", "beleidigt": "BELEIDIGT",
           "denken": "DENKEN", "drehen": "DREHEN", "fenster": "FENSTER", "gaehnen": "GAEHNEN", "huepfen": "HUEPFEN",
           "laufen": "LAUFEN", "singen": "SINGEN", "staunen": "STAUNEN"}
GARTEN = {"garten-schnuppern": "SCHNUPPERN", "garten-untersuchen": "UNTERSUCHEN", "garten-zeigen": "ZEIGEN"}

# Größen der vorhandenen Schwarz-Weiß-Bilder aus den Programmdateien lesen
groessen = {}
for datei in ("assets.h", "momente.h", "pausiert-garten/gartenbilder.h"):
    for m in re.finditer(r"static const Bild (\w+) = \{(\d+), (\d+),", (SKETCH / datei).read_text(encoding="utf-8")):
        groessen[m.group(1)] = (int(m.group(2)), int(m.group(3)))

def alt_name(ordner, datei):
    if ordner == "01-Ei":
        return {"ruhe": "ART_EI_BASE", "wackeln-links": "ART_EI_LINKS", "wackeln-rechts": "ART_EI_RECHTS"}.get(datei)
    art, garten, _ = PHASEN[ordner]
    if datei in ZUSTAND:
        return f"ART_{art}_{ZUSTAND[datei]}"
    if datei in GARTEN:
        return f"GA_{garten}_{GARTEN[datei]}"
    return None

z = zipfile.ZipFile(ZIP)
eintraege, fehler, neue_bilder = [], 0, []
alle = set(z.namelist())
if NEU.exists():       # Bilder, die es nur als neue Zeichnung gibt (zum Beispiel ruhe der erwachsenen Evi)
    alle |= {p.relative_to(NEU).as_posix() for p in NEU.rglob("*.png")}
for name in sorted(alle):
    m = re.fullmatch(r"Sprites/([^/]+)/([^/]+)\.png", name)
    if not m:
        continue
    alt = alt_name(m.group(1), m.group(2))
    if alt is None or alt not in groessen:
        print("kein altes Bild für", name, alt); fehler += 1; continue
    neu = NEU / name
    if neu.exists():
        im = Image.open(neu).convert("LA"); neue_bilder.append(name)
        if alt in groessen and im.size != groessen[alt]: pass
    else:
        im = Image.open(io.BytesIO(z.read(name))).convert("LA")
    if im.size != groessen[alt]:
        print("Größe passt nicht:", name, im.size, "alt", groessen[alt]); fehler += 1; continue
    w, h = im.size
    px = list(im.getdata())
    stufen = []
    for g, a in px:
        stufen.append(15 if a < 128 else min(14, round(g / 255 * 14)))
    if len(stufen) % 2: stufen.append(15)
    daten = bytes((stufen[i] << 4) | stufen[i + 1] for i in range(0, len(stufen), 2))
    eintraege.append((alt, w, h, daten, name))
if fehler:
    sys.exit(f"{fehler} Fehler, nichts geschrieben")

out = ["// Graustufen-Bilder für Evi (erzeugt von tools/make-evi-grau.py aus Evi-Graustufen.zip, nicht von Hand ändern).",
       "// Jedes Bild gehört zu einem Schwarz-Weiß-Bild gleicher Größe; 4 Bit je Punkt, 15 = durchsichtig.",
       "#pragma once", ""]
for alt, w, h, daten, name in eintraege:
    out.append(f"// {name}")
    out.append(f"static const uint8_t G_{alt}[] = {{")
    for i in range(0, len(daten), 24):
        out.append("  " + ",".join(f"0x{b:02x}" for b in daten[i:i + 24]) + ",")
    out.append("};")
out.append("")
out.append("struct GrauZuordnung { const Bild* alt; const uint8_t* daten; };")
out.append("static const GrauZuordnung GRAU_TABELLE[] = {")
garten_aktiv = (SKETCH / "gartenbilder.h").exists()   # Garten ist pausiert (pausiert-garten/): dann nicht in die Tabelle
anzahl = 0
for alt, *_ in eintraege:
    if alt.startswith("GA_") and not garten_aktiv:
        continue
    out.append(f"  {{&{alt}, G_{alt}}},")
    anzahl += 1
out.append("};")
out.append("static const int GRAU_ANZAHL = sizeof(GRAU_TABELLE) / sizeof(GRAU_TABELLE[0]);")
AUS.write_text("\n".join(out) + "\n", encoding="utf-8")
print(len(neue_bilder), "davon neu aus graustufen-neu:", ", ".join(n.split("/")[-1][:-4] for n in neue_bilder))
print(len(eintraege), "Bilder,", sum(len(e[3]) for e in eintraege), "Byte ->", AUS)
