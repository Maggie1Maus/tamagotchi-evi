# Erzeugt evi-waveshare/evi_art.h aus Evis Zeichnungen.
#
# Quelle sind die schon zugeschnittenen Bilder der Handy-App
# (handy-app/dist/img/adult-*.png, erzeugt von handy-app/tools/build-art.ps1).
# Nichts wird neu gezeichnet: Alle Bilder werden mit demselben Massstab
# verkleinert, und jeder Punkt wird schwarz (Linie) oder weiss.
#
# Aufruf aus dem Projektordner:
#   powershell -ExecutionPolicy Bypass -File software\schritt-09\tools\make-art.ps1
Add-Type -AssemblyName System.Drawing

$root = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
$src = Join-Path $root 'handy-app\dist\img'
$out = Join-Path $PSScriptRoot '..\evi-waveshare\evi_art.h'
$names = 'gluecklich', 'hungrig', 'traurig', 'schmutzig', 'schlafend'
$targetHeight = 170   # das hoechste Bild wird 170 Pixel hoch
$threshold = 140      # dunkler als dieser Grauwert = Linie

$images = @{}
foreach ($n in $names) { $images[$n] = [Drawing.Bitmap]::FromFile((Join-Path $src "adult-$n.png")) }
$maxH = ($images.Values | Measure-Object -Property Height -Maximum).Maximum
$scale = $targetHeight / $maxH

$lines = @(
  '// Erzeugt von software/schritt-09/tools/make-art.ps1 - nicht von Hand aendern.',
  '// Evis Zeichnungen (erwachsen), nur gleichmaessig verkleinert und in schwarz/weiss',
  '// umgerechnet. 1 Bit pro Punkt, Zeile fuer Zeile, linkes Pixel im hoechsten Bit',
  '// (Format von Adafruit GFX drawBitmap). Gesetztes Bit = Linie.',
  '#pragma once',
  '#include <stdint.h>',
  ''
)
$total = 0
foreach ($n in $names) {
  $img = $images[$n]
  $w = [int][Math]::Round($img.Width * $scale)
  $h = [int][Math]::Round($img.Height * $scale)
  # Auf weissem Grund verkleinern: Durchsichtiges wird weiss.
  $small = New-Object Drawing.Bitmap $w, $h
  $g = [Drawing.Graphics]::FromImage($small)
  $g.Clear([Drawing.Color]::White)
  $g.InterpolationMode = [Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
  $g.PixelOffsetMode = [Drawing.Drawing2D.PixelOffsetMode]::HighQuality
  $g.DrawImage($img, 0, 0, $w, $h)
  $g.Dispose()

  $bytes = New-Object System.Collections.Generic.List[string]
  for ($y = 0; $y -lt $h; $y++) {
    for ($bx = 0; $bx -lt $w; $bx += 8) {
      $byte = 0
      for ($bit = 0; $bit -lt 8; $bit++) {
        $x = $bx + $bit
        if ($x -lt $w) {
          $c = $small.GetPixel($x, $y)
          $lum = 0.299 * $c.R + 0.587 * $c.G + 0.114 * $c.B
          if ($lum -lt $threshold) { $byte = $byte -bor (0x80 -shr $bit) }
        }
      }
      $bytes.Add(('0x{0:x2}' -f $byte))
    }
  }
  $small.Dispose()
  $total += $bytes.Count
  $upper = $n.ToUpper()
  $lines += "const int ART_${upper}_W = $w;"
  $lines += "const int ART_${upper}_H = $h;"
  $lines += "const uint8_t ART_${upper}[] = {"
  for ($i = 0; $i -lt $bytes.Count; $i += 16) {
    $end = [Math]::Min($i + 15, $bytes.Count - 1)
    $lines += '    ' + (($bytes[$i..$end]) -join ', ') + ','
  }
  $lines += '};'
  $lines += ''
}
foreach ($img in $images.Values) { $img.Dispose() }
$enc = New-Object Text.UTF8Encoding($false)
New-Item -ItemType Directory -Force (Split-Path $out) | Out-Null
[IO.File]::WriteAllText([IO.Path]::GetFullPath($out), ($lines -join "`n") + "`n", $enc)
"evi_art.h: $($names.Count) Bilder, $total Bytes, Massstab $([Math]::Round($scale, 4))"
