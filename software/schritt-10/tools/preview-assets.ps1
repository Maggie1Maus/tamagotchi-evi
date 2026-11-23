# Zeigt den Inhalt von evi-handy/assets.h als PNG, um die Umrechnung zu pruefen.
# Aufruf: powershell -ExecutionPolicy Bypass -File software\schritt-10\tools\preview-assets.ps1 AUSGABE.png
param([string]$Out = (Join-Path $env:TEMP 'assets-vorschau.png'))
Add-Type -AssemblyName System.Drawing
$text = [IO.File]::ReadAllText((Join-Path $PSScriptRoot '..\evi-handy\assets.h'))
$arrays = @{}
foreach ($m in [regex]::Matches($text, 'static const uint8_t (\w+)\[\] = \{([^}]*)\}')) {
  $arrays[$m.Groups[1].Value] = [byte[]]([regex]::Matches($m.Groups[2].Value, '0x([0-9a-f]{2})') | ForEach-Object { [Convert]::ToByte($_.Groups[1].Value, 16) })
}
$bilder = @{}
foreach ($m in [regex]::Matches($text, 'static const Bild (\w+) = \{(-?\d+), (-?\d+), (-?\d+), (-?\d+), (\w+), (\w+)\}')) {
  $bilder[$m.Groups[1].Value] = @{ w = [int]$m.Groups[2].Value; h = [int]$m.Groups[3].Value; l = $m.Groups[6].Value; m = $m.Groups[7].Value }
}
$bmp = New-Object Drawing.Bitmap 980, 900
$g = [Drawing.Graphics]::FromImage($bmp); $g.Clear([Drawing.Color]::FromArgb(200, 200, 200)); $g.Dispose()
function Draw-Bits($data, [int]$w, [int]$h, [int]$px, [int]$py, [bool]$maskOnly = $false, $maskData = $null) {
  $bpr = [Math]::Ceiling($w / 8)
  for ($y = 0; $y -lt $h; $y++) { for ($x = 0; $x -lt $w; $x++) {
    $on = $data[$y * $bpr + [Math]::Floor($x / 8)] -band (0x80 -shr ($x % 8))
    if ($maskData) {
      $in = $maskData[$y * $bpr + [Math]::Floor($x / 8)] -band (0x80 -shr ($x % 8))
      if ($on) { $bmp.SetPixel($px + $x, $py + $y, [Drawing.Color]::Black) } elseif ($in) { $bmp.SetPixel($px + $x, $py + $y, [Drawing.Color]::White) }
    } else {
      $bmp.SetPixel($px + $x, $py + $y, $(if ($on) { [Drawing.Color]::Black } else { [Drawing.Color]::White }))
    }
  } }
}
$x = 0
foreach ($s in 'ZIMMER_TAG', 'ZIMMER_DAEMMERUNG', 'ZIMMER_NACHT') { Draw-Bits $arrays[$s] 320 224 $x 0; $x += 330 }
# Evi auf dem Teppich des Tag-Zimmers (Fusspunkt 167/180)
foreach ($pair in @(@('ART_ERWACHSEN_GLUECKLICH', 0), @('ART_BABY_GLUECKLICH', 330), @('ART_EI_BASE', 660))) {
  $b = $bilder[$pair[0]]
  Draw-Bits $arrays['ZIMMER_TAG'] 320 224 $pair[1] 234
  Draw-Bits $arrays[$b.l] $b.w $b.h ($pair[1] + 167 - [Math]::Round($b.w / 2)) (234 + 180 - $b.h) $false $arrays[$b.m]
}
# Alle Zeichnungen und Symbole
$x = 0; $y = 470; $rowH = 0
foreach ($name in ($bilder.Keys | Sort-Object)) {
  $b = $bilder[$name]
  if ($x + $b.w -gt 980) { $x = 0; $y += $rowH + 4; $rowH = 0 }
  if ($y + $b.h -gt 900) { break }
  Draw-Bits $arrays[$b.l] $b.w $b.h $x $y $false $(if ($b.m -ne 'nullptr') { $arrays[$b.m] } else { $null })
  $x += $b.w + 4; if ($b.h -gt $rowH) { $rowH = $b.h }
}
$bmp.Save($Out); $bmp.Dispose(); $Out
