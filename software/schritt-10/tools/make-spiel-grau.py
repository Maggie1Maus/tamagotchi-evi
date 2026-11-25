#!/usr/bin/env python3
"""Erzeugt evi-handy/spielgrau.h: Graustufen für Fundstücke und Weihnachtsschmuck.

Quelle: entwuerfe/zimmer/graustufen/spielgrafiken/ (Bilder der Nutzerin, 1:1, nichts neu gezeichnet).
Gleiches Format wie evigrau.h (4 Bit je Punkt, 15 = durchsichtig, 0-14 Grau). Jedes Bild muss genau die Größe
des vorhandenen Schwarz-Weiß-Bildes haben, das weiter für Treffer/Verdeckung zuständig ist.
Aufruf: python3 software/schritt-10/tools/make-spiel-grau.py
"""
import re, sys, pathlib
from PIL import Image
HIER = pathlib.Path(__file__).resolve().parent
QUELLE = HIER.parents[2] / "entwuerfe/zimmer/graustufen/spielgrafiken"
SKETCH = HIER.parent / "evi-handy"
ZUORDNUNG = {
    "funde/stein": "FUND_ART_STEIN", "funde/blatt": "FUND_ART_BLATT", "funde/feder": "FUND_ART_FEDER",
    "funde/zapfen": "FUND_ART_ZAPFEN", "funde/micky": "FUND_ART_MICKY", "funde/muschel": "FUND_ART_MUSCHEL",
    "funde/drache": "FUND_ART_DRACHE", "funde/triforce": "FUND_ART_TRIFORCE", "funde/fossil": "FUND_ART_FOSSIL",
    "funde/junimo": "FUND_ART_JUNIMO",
    "deko/weihnachtsbaum": "WEIHNACHTSBAUM", "deko/kerzentablett": "KERZEN", "deko/fenstersterne": "FENSTERSTERNE",
}
groessen = {}
for datei in ("fundbilder.h", "umgebung.h"):
    for m in re.finditer(r"static const Bild (\w+) = \{(\d+), (\d+),", (SKETCH / datei).read_text(encoding="utf-8")):
        groessen[m.group(1)] = (int(m.group(2)), int(m.group(3)))
eintraege, fehler = [], 0
for datei, alt in ZUORDNUNG.items():
    im = Image.open(QUELLE / (datei + ".png")).convert("LA")
    if groessen.get(alt) != im.size:
        print("Größe passt nicht:", datei, im.size, "alt", groessen.get(alt)); fehler += 1; continue
    st = [15 if a < 128 else min(14, round(g / 255 * 14)) for g, a in im.getdata()]
    if len(st) % 2: st.append(15)
    eintraege.append((alt, bytes((st[i] << 4) | st[i + 1] for i in range(0, len(st), 2)), datei))
if fehler: sys.exit("Fehler, nichts geschrieben")
out = ["// Graustufen für Fundstücke und Weihnachtsschmuck (erzeugt von tools/make-spiel-grau.py, nicht von Hand ändern).",
       "// Format wie evigrau.h: 4 Bit je Punkt, 15 = durchsichtig.", "#pragma once", ""]
for alt, d, datei in eintraege:
    out += [f"// {datei}.png", f"static const uint8_t G_{alt}[] = {{"]
    out += ["  " + ",".join(f"0x{b:02x}" for b in d[i:i+24]) + "," for i in range(0, len(d), 24)] + ["};"]
out += ["", "static const GrauZuordnung SPIEL_GRAU_TABELLE[] = {"] + [f"  {{&{a}, G_{a}}}," for a, *_ in eintraege] + ["};",
        f"static const int SPIEL_GRAU_ANZAHL = {len(eintraege)};"]
(SKETCH / "spielgrau.h").write_text("\n".join(out) + "\n", encoding="utf-8")
print(len(eintraege), "Bilder,", sum(len(e[1]) for e in eintraege), "Byte")
