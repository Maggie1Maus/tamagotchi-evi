#!/usr/bin/env python3
"""Python-Fassung von teile-aktualisieren.ps1: packt alle PNGs als Base64 in teile-daten.js."""
import base64, json, os, zipfile
hier = os.path.dirname(os.path.abspath(__file__))
repo = os.path.abspath(os.path.join(hier, '..', '..', '..'))
daten = {}
def bild(kennung, pfad=None, rohdaten=None):
    if rohdaten is None:
        if not os.path.exists(pfad):
            return
        rohdaten = open(pfad, 'rb').read()
    daten[kennung] = 'data:image/png;base64,' + base64.b64encode(rohdaten).decode()
td = os.path.join(hier, 'teile')
for ordner, _, dateien in os.walk(td):
    for f in sorted(dateien):
        if f.endswith('.png'):
            p = os.path.join(ordner, f)
            bild('teile/' + os.path.relpath(p, td).replace(os.sep, '/'), p)
zp = os.path.join(repo, 'entwuerfe', 'evi', 'graustufen-vorschau', 'Evi-Graustufen.zip')
with zipfile.ZipFile(zp) as z:
    for alter, phase in dict(baby='02-Baby', teenager='03-Teenager', erwachsen='04-Erwachsen', senior='05-Oma').items():
        n = f'Sprites/{phase}/gluecklich.png'
        if n in z.namelist():
            bild(f'original/evi/{alter}', rohdaten=z.read(n))
park = os.path.join(repo, 'entwuerfe', 'neue-tamagotchis', 'graustufen')
for figur in sorted(os.listdir(park)):
    if os.path.isdir(os.path.join(park, figur)):
        for alter in ('baby', 'teenager', 'erwachsen', 'senior'):
            bild(f'original/{figur}/{alter}', os.path.join(park, figur, f'{alter}-gluecklich.png'))
mann = os.path.join(repo, 'entwuerfe', 'evi', 'maennlich', 'Sprites')
for alter, phase in dict(baby='02-Baby', teenager='03-Teenager', erwachsen='04-Erwachsen', senior='05-Opa').items():
    bild(f'original/evi-m/{alter}', os.path.join(mann, phase, 'gluecklich.png'))
with open(os.path.join(hier, 'teile-daten.js'), 'w', encoding='utf-8', newline='\n') as f:
    f.write('window.TEILE_DATEN = ' + json.dumps(daten, separators=(',', ':')) + ';\n')
print(f'teile-daten.js aktualisiert: {len(daten)} Bilder.')
