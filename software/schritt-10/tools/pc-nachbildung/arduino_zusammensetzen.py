# Bildet nach, wie die Arduino IDE einen Sketch mit mehreren .ino-Dateien übersetzt:
# Hauptdatei zuerst, dann die anderen .ino alphabetisch; Funktionsankündigungen (ohne Standardwerte)
# vor die erste Funktion. Aufruf: python3 -I arduino_zusammensetzen.py <sketchordner> <ausgabe.ino>
import re, sys, os
ordner, aus = sys.argv[1], sys.argv[2]
name = os.path.basename(os.path.normpath(ordner))
haupt = os.path.join(ordner, name + '.ino')
andere = sorted(f for f in os.listdir(ordner) if f.endswith('.ino') and f != name + '.ino')
text = open(haupt, encoding='utf-8').read()
for f in andere: text += '\n' + open(os.path.join(ordner, f), encoding='utf-8').read()
muster = re.compile(r'^((?:static |inline )*(?:const )?[A-Za-z_][\w:<>]*[ \*&]+[\*&]*(\w+)\s*\(([^()]*)\))\s*\{', re.M)
def ohne_standard(params):
    teile, tiefe, aktuell = [], 0, ''
    for ch in params:
        if ch in '([{': tiefe += 1
        if ch in ')]}': tiefe -= 1
        if ch == ',' and tiefe == 0: teile.append(aktuell); aktuell = ''
        else: aktuell += ch
    teile.append(aktuell)
    return ', '.join(p.split('=')[0].strip() for p in teile if p.strip())
ankuendigungen, erste = [], None
for m in muster.finditer(text):
    kopf = m.group(1)
    if m.group(2) in ('if', 'for', 'while', 'switch', 'return'): continue
    if erste is None: erste = m.start()
    vor = kopf[:kopf.index('(')]
    ankuendigungen.append(f'{vor}({ohne_standard(m.group(3))});')
text = text[:erste] + '\n'.join(ankuendigungen) + '\n' + text[erste:]
open(aus, 'w', encoding='utf-8').write(text)
print(f'{len(ankuendigungen)} Ankündigungen, Dateien: {[name + ".ino"] + andere}')
