#!/usr/bin/env python3
"""Erzeugt evi-handy/zimmergrau.h: Zimmer, Möbel, Fensterrahmen und alle Fensterebenen in Graustufen.

Quelle: entwuerfe/zimmer/graustufen/spielgrafiken/ (Bilder der Nutzerin, 1:1, nichts neu gezeichnet).
Format wie evigrau.h: 4 Bit je Punkt, zwei Punkte je Byte (oberes Halbbyte zuerst), 0-14 Grau, 15 = durchsichtig.
Zusätzlich ZIMMER_SCHEIBE: 1 Bit je Punkt, 1 = Glas (durchsichtiger Bereich des neuen Fensterrahmens).
Aufruf: python3 software/schritt-10/tools/make-zimmer-grau.py
"""
import pathlib, sys
from PIL import Image
HIER = pathlib.Path(__file__).resolve().parent
Q = HIER.parents[2] / "entwuerfe/zimmer/graustufen/spielgrafiken"
AUS = HIER.parent / "evi-handy/zimmergrau.h"

def laden(datei, groesse):
    im = Image.open(Q / (datei + ".png")).convert("LA")
    if im.size != groesse:
        sys.exit(f"Größe passt nicht: {datei} {im.size} statt {groesse}")
    return im

def packen(im):
    st = [15 if a < 128 else min(14, round(g / 255 * 14)) for g, a in im.getdata()]
    if len(st) % 2: st.append(15)
    return bytes((st[i] << 4) | st[i + 1] for i in range(0, len(st), 2))

arrays = []   # (name, kommentar, bytes)
def dazu(name, datei, groesse):
    arrays.append((name, datei, packen(laden(datei, groesse))))
    return name

F = (66, 53)
HZ = ["tag", "daemmerung", "nacht"]
WE = ["klar", "leicht-bewoelkt", "regen", "bedeckt", "schneefall"]     # Reihenfolge wie enum Wetter
JZ = ["fruehling", "sommer", "herbst", "winter"]
BAUMEND = {"tag": "", "daemmerung": "-hell", "nacht": "-dunkel"}
def n(s): return s.upper().replace("-", "_")

raum = [dazu("RAUM_" + n(h), "zimmer/wand-boden" + {"tag": "", "daemmerung": "-daemmerung", "nacht": "-nacht"}[h], (320, 174)) for h in HZ]
himmel = [[dazu(f"HIMMEL_{n(h)}_{n(w)}", f"fenster/himmel-{h}-{w}", F) for w in WE] for h in HZ]
zeichen = [dazu("ZEICHEN_" + n(h), "fenster/himmelszeichen-" + h, F) for h in HZ]
wolken = [[dazu(f"WOLKEN_{n(h)}_{n(w)}", f"fenster/wolken-{w}-{h}", F) for w in WE] for h in HZ]
huegel = dazu("HUEGEL", "fenster/huegel", F)
baum = [[dazu(f"BAUM_{n(h)}_{n(j)}", f"fenster/baum-{j}{BAUMEND[h]}", F) for j in JZ] for h in HZ]
laub = dazu("LAUB", "fenster/laub-herbst", F)
sboden = dazu("SCHNEE_BODEN", "fenster/schnee-boden", F)
saeste = dazu("SCHNEE_AESTE", "fenster/schnee-aeste", F)
regen = [dazu(f"REGEN_{i}", f"fenster/regen-{i}", F) for i in range(16)]
schnee = [dazu(f"SCHNEE_{i}", f"fenster/schnee-{i}", F) for i in range(16)]
blatt = dazu("BLATT", "fenster/herbstblatt", (5, 6))
moebel = [("EVI_TIERBETT", "tierbett", (81, 45)), ("EVI_SIDEBOARD", "sideboard", (102, 51)), ("EVI_REGAL", "regal", (93, 20)),
          ("EVI_TEPPICH", "teppich", (130, 36)), ("EVI_FENSTER_RAHMEN", "fensterrahmen", (81, 69))]
for name, datei, gr in moebel:
    dazu("M_" + name, "moebel/" + datei, gr)

# Glas = durchsichtiger Teil des Rahmens im Fensterbereich (7, 7), 66 x 53
rahmen = Image.open(Q / "moebel/fensterrahmen.png").convert("RGBA")
zeilen = []
for y in range(53):
    bits = [1 if rahmen.getpixel((x + 7, y + 7))[3] < 128 else 0 for x in range(66)]
    bits += [0] * 6
    zeilen.append(bytes(sum(b << (7 - k) for k, b in enumerate(bits[i:i + 8])) for i in range(0, 72, 8)))
scheibe = b"".join(zeilen)

o = ["// Graustufen für Zimmer, Möbel, Fensterrahmen und Fensterebenen (erzeugt von tools/make-zimmer-grau.py, nicht von Hand ändern).",
     "// 4 Bit je Punkt, 15 = durchsichtig. Fensterebenen sind 66 x 53 und liegen bei (ZF_X, ZF_Y).", "#pragma once", ""]
for name, datei, d in arrays:
    o += [f"// {datei}.png", f"static const uint8_t ZG_{name}[] = {{"]
    o += ["  " + ",".join(f"0x{b:02x}" for b in d[i:i + 24]) + "," for i in range(0, len(d), 24)] + ["};"]
o += ["// 1 Bit je Punkt, 9 Byte je Zeile, 1 = Glas", "static const uint8_t ZIMMER_SCHEIBE[] = {"]
o += ["  " + ",".join(f"0x{b:02x}" for b in scheibe[i:i + 9]) + "," for i in range(0, len(scheibe), 9)] + ["};", ""]
def tab2(name, t): return f"static const uint8_t* const {name}[{len(t)}][{len(t[0])}] = {{" + ",".join("{" + ",".join("ZG_" + x for x in r) + "}" for r in t) + "};"
o.append("static const uint8_t* const ZG_RAUM[3] = {" + ",".join("ZG_" + x for x in raum) + "};")
o.append(tab2("ZG_HIMMEL", himmel)); o.append("static const uint8_t* const ZG_ZEICHEN[3] = {" + ",".join("ZG_" + x for x in zeichen) + "};")
o.append(tab2("ZG_WOLKEN", wolken)); o.append(tab2("ZG_BAUM", baum))
o.append("static const uint8_t* const ZG_REGEN[16] = {" + ",".join("ZG_" + x for x in regen) + "};")
o.append("static const uint8_t* const ZG_SCHNEE[16] = {" + ",".join("ZG_" + x for x in schnee) + "};")
o.append("struct ZimmerGrauZuordnung { const EviGrafik* alt; const uint8_t* daten; };")
o.append("static const ZimmerGrauZuordnung ZIMMER_GRAU[] = {" + ",".join(f"{{&{nm}, ZG_M_{nm}}}" for nm, *_ in moebel) + "};")
o.append(f"static const int ZIMMER_GRAU_ANZAHL = {len(moebel)};")
AUS.write_text("\n".join(o) + "\n", encoding="utf-8")
print(len(arrays), "Bilder,", sum(len(a[2]) for a in arrays), "Byte ->", AUS)
