# Baut weitere Vorlagen aus Basis und Bereichskarte (Pixel weglassen/glaetten, nichts neu zeichnen)
Add-Type -AssemblyName System.Drawing
$T=(Resolve-Path (Join-Path $PSScriptRoot '..\..\grillme-genetik-prototyp\baukasten\teile\weiblich\erwachsen')).Path
$basis=New-Object System.Drawing.Bitmap("$T\basis.png")
$ber=New-Object System.Drawing.Bitmap("$T\basis.bereiche.png")
$fell=[System.Drawing.Color]::FromArgb(255,214,214,214)
function Ist68($x,$y){ if($x -lt 0 -or $y -lt 0 -or $x -ge 114 -or $y -ge 116){return $false}; $m=$ber.GetPixel($x,$y); return ($m.A -gt 0 -and $m.R -eq 68) }
function Bau($datei,$ohneGesicht,$ohneKranz){
  $b=New-Object System.Drawing.Bitmap(114,116)
  for($y=0;$y -lt 116;$y++){for($x=0;$x -lt 114;$x++){
    $p=$basis.GetPixel($x,$y); if($p.A -eq 0){continue}
    $m=$ber.GetPixel($x,$y)
    if($ohneGesicht -and $x -ge 31 -and $x -le 72 -and $y -ge 48 -and $y -le 69){ $b.SetPixel($x,$y,$fell); continue }
    if($ohneKranz){
      $nah=$false; for($dy=-1;$dy -le 1;$dy++){for($dx=-1;$dx -le 1;$dx++){ if(Ist68 ($x+$dx) ($y+$dy)){$nah=$true} }}
      if($nah -and $y -gt 70){ continue }   # Kranz und seine Linie weg (nur unterhalb des Kopfes)
    }
    $b.SetPixel($x,$y,$p)}}
  $bmp=New-Object System.Drawing.Bitmap(2048,2048)
  $zg=[System.Drawing.Graphics]::FromImage($bmp);$zg.Clear([System.Drawing.Color]::White)
  $cm=New-Object System.Drawing.Imaging.ColorMatrix;$cm.Matrix33=0.40
  $ia=New-Object System.Drawing.Imaging.ImageAttributes;$ia.SetColorMatrix($cm)
  $zg.InterpolationMode='NearestNeighbor';$zg.PixelOffsetMode='Half'
  $zg.DrawImage($b,(New-Object System.Drawing.Rectangle(340,328,(114*12),(116*12))),0,0,114,116,[System.Drawing.GraphicsUnit]::Pixel,$ia)
  $zg.Dispose();$bmp.Save((Join-Path $PSScriptRoot $datei),[System.Drawing.Imaging.ImageFormat]::Png)
}
Bau 'Vorlage-ohne-Kranz.png' $false $true
Bau 'Vorlage-ohne-Gesicht.png' $true $false
Bau 'Vorlage-Silhouette.png' $true $true
