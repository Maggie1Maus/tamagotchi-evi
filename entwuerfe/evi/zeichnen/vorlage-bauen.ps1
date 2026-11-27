# Baut die Procreate-Vorlagen aus Evis Original (nur skaliert und aufgehellt, nichts neu gezeichnet).
Add-Type -AssemblyName System.Drawing
$ordner = $PSScriptRoot
$quelle = Join-Path $ordner '..\evi-aktuell-referenz.png'
$src = New-Object System.Drawing.Bitmap(((Resolve-Path $quelle).Path))
$LW = 2048
$faktor = 3                       # Evi 3x vergroessert (820x640 -> 2460 waere zu gross, deshalb zugeschnitten)
# Evi-Inhalt zuschneiden (weisser Rand weg)
$minx=$src.Width;$miny=$src.Height;$maxx=0;$maxy=0
for($y=0;$y -lt $src.Height;$y++){for($x=0;$x -lt $src.Width;$x++){
 if($src.GetPixel($x,$y).R -lt 128){ if($x -lt $minx){$minx=$x}; if($x -gt $maxx){$maxx=$x}; if($y -lt $miny){$miny=$y}; if($y -gt $maxy){$maxy=$y} }}}
$w=$maxx-$minx+1;$h=$maxy-$miny+1
$s=[math]::Floor(1500/[math]::Max($w,$h)*100)/100
$dw=[int]($w*$s);$dh=[int]($h*$s)
$ox=[int](($LW-$dw)/2);$oy=[int](($LW-$dh)/2)

function Neu($mitGitter,$datei){
  $bmp = New-Object System.Drawing.Bitmap($LW,$LW)
  $zg = [System.Drawing.Graphics]::FromImage($bmp)
  $zg.Clear([System.Drawing.Color]::White)
  if($mitGitter){
    $p = New-Object System.Drawing.Pen([System.Drawing.Color]::FromArgb(255,225,235,245),2)
    for($i=0;$i -le $LW;$i+=128){ $zg.DrawLine($p,$i,0,$i,$LW); $zg.DrawLine($p,0,$i,$LW,$i) }
  }
  # Evi als hellblaue Unterlage (Linien blass, damit eigene schwarze Linien gut sichtbar sind)
  $cm = New-Object System.Drawing.Imaging.ColorMatrix
  $cm.Matrix33 = 0.30
  $ia = New-Object System.Drawing.Imaging.ImageAttributes
  $ia.SetColorMatrix($cm)
  $zg.InterpolationMode='NearestNeighbor'; $zg.PixelOffsetMode='Half'
  $ziel = New-Object System.Drawing.Rectangle($ox,$oy,$dw,$dh)
  $zg.DrawImage($src,$ziel,$minx,$miny,$w,$h,[System.Drawing.GraphicsUnit]::Pixel,$ia)
  $zg.Dispose()
  $bmp.Save((Join-Path $ordner $datei),[System.Drawing.Imaging.ImageFormat]::Png)
  $bmp.Dispose()
}
Neu $false 'Vorlage-Evi.png'
Neu $true  'Vorlage-Evi-mit-Raster.png'
"fertig: $dw x $dh bei ($ox,$oy)"
