"""Compare firmware framebuffer pixels with the approved independent graphics export."""
import io,sys,zipfile
from pathlib import Path
from PIL import Image
root=Path(__file__).resolve().parents[3]
images=Path(sys.argv[1])
seasons=['fruehling','sommer','herbst','winter']
times=['tag','daemmerung','nacht']
weather=['klar','leicht-bewoelkt','regen','bedeckt','schneefall']
with zipfile.ZipFile(root/'entwuerfe/zimmer/grillme-2026-11-25/v2/Evi_Grafikpaket_V2.zip') as z:
 prefix='evi-grafikpaket-v2-2026-11-25/'
 glass=Image.open(io.BytesIO(z.read(prefix+'bausteine/fenster-scheibenmaske/linien.png'))).convert('L')
 checked=0
 for j,season in enumerate(seasons):
  for h,time in enumerate(times):
   for w,rain in enumerate(weather):
    if rain=='schneefall' and season!='winter':continue
    actual=Image.open(images/f'v2-window-{j}-{h}-{w}.pbm').convert('L')
    expected=Image.open(io.BytesIO(z.read(prefix+f'fensterausblicke/{season}-{time}-{rain}.png'))).convert('L')
    for y in range(240):
     for x in range(320):
      inside=79<=x<145 and 46<=y<99 and glass.getpixel((x-79,y-46))==0
      value=expected.getpixel((x-79,y-46)) if inside else 255
      if actual.getpixel((x,y))!=value:raise AssertionError((season,time,rain,x,y))
    checked+=1
 print(f'OK: {checked} Firmware-Fensterausblicke stimmen pixelgenau mit den freigegebenen Exporten überein; Rahmen und Außenbereich unverändert.')
