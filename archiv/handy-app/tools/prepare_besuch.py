"""Bereitet die Besuchsbilder der Nutzerin auf (entwuerfe/evi/momente/schmetterling.png, vogel.png).

Beide sind schon reine Pixelbilder (schwarz, weiß, durchsichtig). Der Schmetterling wird 1:1
übernommen. Der Vogel wird auf drei Viertel verkleinert, damit er in eine Fensterscheibe passt
(Mittelwert je Fläche, dann wieder rein schwarz, weiß oder durchsichtig). Sonst wird nichts geändert.

    python3 handy-app/tools/prepare_besuch.py      (aus dem Projektordner)
Ergebnis: handy-app/dist/besuch/schmetterling.png und vogel.png
"""
import os
from PIL import Image

HIER = os.path.dirname(os.path.abspath(__file__))
QUELLE = os.path.join(HIER, '..', '..', 'entwuerfe', 'evi', 'momente')
ZIEL = os.path.join(HIER, '..', 'dist', 'besuch')
GROESSE = {'schmetterling.png': 1.0, 'vogel.png': 0.75}


def aufbereiten(name, faktor):
    src = Image.open(os.path.join(QUELLE, name)).convert('LA')
    w, h = src.size
    nw, nh = round(w * faktor), round(h * faktor)
    hell = Image.new('L', (w, h), 255)
    hell.paste(src.getchannel('L'), (0, 0), src.getchannel('A'))
    deckend = src.getchannel('A')
    if (nw, nh) != (w, h):
        hell = hell.resize((nw, nh), Image.BOX)
        deckend = deckend.resize((nw, nh), Image.BOX)
    aus = Image.new('LA', (nw, nh), (255, 0))
    for y in range(nh):
        for x in range(nw):
            if deckend.getpixel((x, y)) >= 128:
                aus.putpixel((x, y), (0 if hell.getpixel((x, y)) < 140 else 255, 255))
    aus.save(os.path.join(ZIEL, name))
    return nw, nh


os.makedirs(ZIEL, exist_ok=True)
for name, faktor in GROESSE.items():
    print(name, *aufbereiten(name, faktor))
