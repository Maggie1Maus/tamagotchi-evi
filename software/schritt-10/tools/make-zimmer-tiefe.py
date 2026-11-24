"""Freigegebenes kompaktes Zimmer, saisonale Fassung und Tiefenstempel.
Die Stempel sind Spielfeld-Geometrie; beim Verdecken bleiben die Originalpixel der Gesamtszene stehen.
"""
from pathlib import Path
from PIL import Image,ImageDraw
import json,sys
root=Path(__file__).resolve().parents[3]
folder=root/'entwuerfe/zimmer/kompakt';folder.mkdir(exist_ok=True)
if '--prepare' in sys.argv:
    spec=json.loads(Path(sys.argv[sys.argv.index('--prepare')+1]).read_text())
    for key,path in spec['paths'].items():
        im=Image.open(path).convert('L').resize((320,174),Image.Resampling.LANCZOS).point(lambda p:255 if p>=190 else 0,mode='1')
        im.save(folder/(key+'.png'))
    (folder/'bildauftrag-weihnachten.txt').write_text(spec['prompt']+'\n')
lines=['// Erzeugt von tools/make-zimmer-tiefe.py. Kompakter erster Entwurf, Originalkonturen.','#pragma once','#include "assets.h"','']
def emit(name,im,bild=False):
    im=im.convert('1'); w,h=im.size
    # Das Bildfeld enthält schwarze Konturen; Stempel enthalten weiße, also aktive Punkte.
    raw=im.tobytes()
    if bild: raw=bytes(v^255 for v in raw)
    lines.append('static const uint8_t '+name+'_L[] = {')
    for i in range(0,len(raw),20): lines.append('  '+','.join(f'0x{v:02x}' for v in raw[i:i+20])+',')
    lines.append('};')
    if bild: lines.append(f'static const Bild {name} = {{{w},{h},0,27,{name}_L,nullptr}};')
for key in ['normal','weihnachten']: emit('ZIMMER_NEU_'+key.upper(),Image.open(folder/(key+'.png')),True)
# Koordinaten im echten Bildschirm (Szenenbild beginnt bei 27). Masken sind keine neuen Möbelbilder.
polys={
'SIDEBOARD':[[(32,119),(40,117),(106,117),(108,121),(106,158),(101,162),(97,160),(35,162),(32,158)],[(53,107),(76,107),(79,117),(53,117)]],
'TISCH':[[(220,126),(229,124),(288,128),(288,138),(220,136)],[(223,136),(230,136),(230,162),(223,160)],[(277,137),(284,137),(284,161),(277,162)],[(232,137),(237,137),(237,157),(232,157)],[(232,150),(281,150),(281,153),(232,153)],[(231,105),(235,93),(248,91),(255,91),(264,93),(270,102),(263,109),(261,126),(239,126),(237,110)]],
'BAUM':[[(249,67),(255,74),(254,80),(265,89),(260,95),(272,106),(267,113),(279,127),(275,134),(284,144),(256,147),(257,158),(241,158),(239,147),(216,144),(225,131),(220,128),(231,115),(225,108),(237,97),(232,92),(243,82),(244,74)]],
'NAPF':[[(257,179),(261,173),(282,173),(287,178),(290,187),(285,191),(258,191),(254,187)]],
'GARTEN_BEET':[[(174,132),(184,130),(196,132),(206,131),(218,132),(229,130),(244,131),(264,132),(264,140),(174,140)]],
'GARTEN_BAUM':[[(0,159),(20,155),(42,151),(64,145),(91,139),(92,144),(69,153),(49,161),(18,168),(0,170)]],
'GARTEN_BUSCH':[[(257,145),(265,141),(277,146),(289,146),(307,152),(319,156),(319,178),(280,166),(258,160)]]}
for name,ps in polys.items():
    im=Image.new('1',(320,174),0);d=ImageDraw.Draw(im)
    for poly in ps:d.polygon([(x,y-27) for x,y in poly],fill=1)
    emit('TIEFE_'+name,im)
# Ein Name pro Maske für direkten Zugriff.
lines += ['#define TIEFE_'+n+' TIEFE_'+n+'_L' for n in polys]
(root/'software/schritt-10/evi-handy/zimmerlandschaft.h').write_text('\n'.join(lines)+'\n')
(folder/'tiefenstempel.json').write_text(json.dumps(polys,indent=2)+'\n')
print('Zimmer und Tiefenstempel erzeugt.')
