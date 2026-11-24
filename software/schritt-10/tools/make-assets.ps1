# Erzeugt evi-handy/assets.h: Zimmer, Symbole und Evis Zeichnungen fuers Waveshare-Board,
# alles aus der Handy-App uebernommen.
#
#  - Zimmer: das SVG aus handy-app/dist/index.html, von Microsoft Edge (ohne Fenster)
#    in Originalgroesse 320 x 224 abfotografiert, einmal mit hellem, gepunktetem und
#    schwarzem Fenster (Tag, Daemmerung, Nacht).
#  - Symbole: die Icons der App (Napf, Herz, Glanz, Muenze, Menue) und die Reaktionszeichen.
#  - Evis Zeichnungen: handy-app/dist/img/*.png im Massstab der App (ART_SCALE in app.js).
#    Die Beleidigt-Posen kommen direkt aus entwuerfe/evi/emotionen.
#    Zu jedem Bild gibt es eine Maske (innen weiss, aussen durchsichtig).
#
# Aufruf aus dem Projektordner:
#   powershell -ExecutionPolicy Bypass -File software\schritt-10\tools\make-assets.ps1
Add-Type -AssemblyName System.Drawing
$ErrorActionPreference = 'Stop'

$root = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
$dist = Join-Path $root 'handy-app\dist'
$outDir = Join-Path $PSScriptRoot '..\evi-handy'
$work = Join-Path $env:TEMP 'evi-assets'
New-Item -ItemType Directory -Force $work, $outDir | Out-Null
$edge = @("${env:ProgramFiles(x86)}\Microsoft\Edge\Application\msedge.exe", "$env:ProgramFiles\Microsoft\Edge\Application\msedge.exe") |
  Where-Object { Test-Path $_ } | Select-Object -First 1
if (-not $edge) { throw 'Microsoft Edge nicht gefunden.' }

function Get-Shot([string]$html, [string]$name, [int]$w, [int]$h) {
  $file = Join-Path $work "$name.html"
  $png = Join-Path $work "$name.png"
  [IO.File]::WriteAllText($file, $html, (New-Object Text.UTF8Encoding($false)))
  if (Test-Path $png) { Remove-Item $png }
  $url = 'file:///' + ($file -replace '\\', '/')
  $p = Start-Process -FilePath $edge -Wait -PassThru -WindowStyle Hidden -ArgumentList @(
    '--headless=new', '--disable-gpu', '--hide-scrollbars', '--force-device-scale-factor=1',
    "--window-size=$w,$h", "--user-data-dir=$work\profile", "--screenshot=$png", $url)
  if (-not (Test-Path $png)) { throw "Bildschirmfoto fehlgeschlagen: $name" }
  return [Drawing.Bitmap]::FromFile($png)
}

# Bits einer Flaeche: Zeile fuer Zeile, links im hoechsten Bit. $test(x, y) -> $true = gesetzt.
function Get-Bits([int]$w, [int]$h, [scriptblock]$test) {
  $bytes = New-Object System.Collections.Generic.List[byte]
  for ($y = 0; $y -lt $h; $y++) {
    for ($bx = 0; $bx -lt $w; $bx += 8) {
      $b = 0
      for ($bit = 0; $bit -lt 8; $bit++) {
        if ($bx + $bit -lt $w -and (& $test ($bx + $bit) $y)) { $b = $b -bor (0x80 -shr $bit) }
      }
      $bytes.Add([byte]$b)
    }
  }
  return , $bytes
}

$lines = New-Object System.Collections.Generic.List[string]
function Add-Array([string]$cname, $bytes) {
  $lines.Add("static const uint8_t $cname[] = {")
  for ($i = 0; $i -lt $bytes.Count; $i += 20) {
    $end = [Math]::Min($i + 19, $bytes.Count - 1)
    $lines.Add('  ' + (($bytes[$i..$end] | ForEach-Object { '0x{0:x2}' -f $_ }) -join ',') + ',')
  }
  $lines.Add('};')
}

$lines.Add('// Erzeugt von software/schritt-10/tools/make-assets.ps1 - nicht von Hand aendern.')
$lines.Add('// Zimmer, Symbole und Zeichnungen aus handy-app/dist. Beleidigt-Posen aus entwuerfe/evi/emotionen.')
$lines.Add('// 1 Bit pro Punkt, Zeile fuer Zeile, linkes Pixel im hoechsten Bit. 1 = schwarz.')
$lines.Add('#pragma once')
$lines.Add('#include <stdint.h>')
$lines.Add('struct Bild { int16_t w, h, ox, oy; const uint8_t* linien; const uint8_t* maske; };')
$lines.Add('')

# ---------- Zimmer ----------
$index = [IO.File]::ReadAllText((Join-Path $dist 'index.html'))
$libStart = $index.IndexOf('<svg class="svg-library"'); $libEnd = $index.IndexOf('</svg>', $libStart) + 6
$library = $index.Substring($libStart, $libEnd - $libStart)
$roomStart = $index.IndexOf('<svg class="room-scene"'); $roomEnd = $index.IndexOf('</svg>', $roomStart) + 6
$room = $index.Substring($roomStart, $roomEnd - $roomStart)
$hide = '#evi-pos,#butterfly,#sleep-z,.minigame,.game-only,.night-only,#shooting-star,#bird,.reaction-symbols,.hum-notes,.svg-library{display:none}'
$skies = [ordered]@{
  TAG = '#window-sky{fill:#fff}.night-stars{display:none}'
  DAEMMERUNG = '#window-sky{fill:url(#dither)}.night-stars{display:none}'
  NACHT = '#window-sky{fill:#000}.night-stars{display:inline}'
}
foreach ($sky in $skies.Keys) {
  $html = "<!doctype html><html><head><style>body{margin:0;background:#fff}.room-scene{display:block;width:320px;height:224px}$hide$($skies[$sky])</style></head><body>$library$room</body></html>"
  $bmp = Get-Shot $html "zimmer-$sky" 320 224
  $bits = Get-Bits 320 224 { param($x, $y) $c = $bmp.GetPixel($x, $y); (0.299 * $c.R + 0.587 * $c.G + 0.114 * $c.B) -lt 128 }
  $bmp.Dispose()
  Add-Array "ZIMMER_$sky" $bits
}
$lines.Add('')

# ---------- Symbole ----------
# [Name, Symbol-ID oder Text, Groesse, Strichstaerke]; Text = Zeichen in system-ui wie in der App
$icons = @(
  @('NAPF', 'i-food', 14, 1.8), @('HERZ', 'i-heart', 14, 1.8), @('GLANZ', 'i-clean', 14, 1.8),
  @('MUENZE', 'i-coin', 10, 2.4),
  @('M_FUTTER', 'i-food', 14, 1.8), @('M_KEKS', 'i-cookie', 14, 1.8), @('M_BALL', 'i-ball', 14, 1.8),
  @('M_PUTZEN', 'i-clean', 14, 1.8), @('M_MEDIZIN', 'i-pill', 14, 1.8), @('M_LICHT', 'i-bulb', 14, 1.8),
  @('M_SONNE', 'i-sun', 14, 1.8), @('M_MOND', 'i-moon', 14, 1.8), @('M_EI', 'i-egg', 14, 1.8),
  @('M_STIFT', 'i-edit', 14, 1.8), @('M_TON', 'i-sound', 14, 1.8), @('M_HILFE', 'i-help', 14, 1.8),
  @('M_PFEILE', 'i-arrows', 14, 1.8), @('M_NOTE', 'i-note', 14, 1.8), @('M_HERZ', 'i-heart', 14, 1.8),
  @('W_BALL', 'i-ball', 15, 1.8), @('W_PFEILE', 'i-arrows', 15, 1.8), @('W_NOTE', 'i-note', 15, 1.8),
  @('W_KEKS', 'i-cookie', 15, 1.8), @('W_PUTZEN', 'i-clean', 15, 1.8), @('W_FUTTER', 'i-food', 15, 1.8),
  @('W_HERZ', 'i-heart', 15, 1.8),
  @('R_HERZ', 'text:&#x2665;', 21, 0), @('R_NOTE', 'text:&#x266A;', 21, 0), @('R_STERN', 'text:&#x2726;', 21, 0),
  @('R_PLUS', 'text:+', 21, 0), @('R_STERN_KLEIN', 'text:&#x2726;', 13, 0)
)
$cell = 32
$parts = foreach ($i in 0..($icons.Count - 1)) {
  $ic = $icons[$i]; $x = ($i % 10) * $cell; $y = [Math]::Floor($i / 10) * $cell
  if ($ic[1].StartsWith('text:')) {
    "<div style=`"position:absolute;left:${x}px;top:${y}px;width:${cell}px;height:${cell}px;font:$($ic[2])px/1 system-ui,sans-serif;color:#000`">$($ic[1].Substring(5))</div>"
  } else {
    "<svg style=`"position:absolute;left:${x}px;top:${y}px;width:$($ic[2])px;height:$($ic[2])px;fill:none;stroke:#000;stroke-width:$($ic[3])px;stroke-linecap:round;stroke-linejoin:round`"><use href=`"#$($ic[1])`"/></svg>"
  }
}
$html = "<!doctype html><html><head><style>body{margin:0;background:#fff}.svg-library{display:none}</style></head><body>$library$($parts -join '')</body></html>"
$atlas = Get-Shot $html 'symbole' 320 ([Math]::Ceiling($icons.Count / 10) * $cell)
foreach ($i in 0..($icons.Count - 1)) {
  $ic = $icons[$i]; $cx = ($i % 10) * $cell; $cy = [Math]::Floor($i / 10) * $cell
  $dark = { param($x, $y) $c = $atlas.GetPixel($cx + $x, $cy + $y); (0.299 * $c.R + 0.587 * $c.G + 0.114 * $c.B) -lt 150 }
  # Auf die Tinte zuschneiden
  $minX = $cell; $minY = $cell; $maxX = -1; $maxY = -1
  for ($y = 0; $y -lt $cell; $y++) { for ($x = 0; $x -lt $cell; $x++) { if (& $dark $x $y) {
    if ($x -lt $minX) { $minX = $x }; if ($x -gt $maxX) { $maxX = $x }; if ($y -lt $minY) { $minY = $y }; if ($y -gt $maxY) { $maxY = $y } } } }
  $w = $maxX - $minX + 1; $h = $maxY - $minY + 1
  $bits = Get-Bits $w $h { param($x, $y) & $dark ($x + $minX) ($y + $minY) }
  Add-Array "SYM_$($ic[0])_BITS" $bits
  $lines.Add("static const Bild SYM_$($ic[0]) = {$w, $h, 0, 0, SYM_$($ic[0])_BITS, nullptr};")
}
$atlas.Dispose()
$lines.Add('')

# ---------- Evis Zeichnungen ----------
$artJs = [IO.File]::ReadAllText((Join-Path $dist 'art.js'))
$scale = @{ egg = 0.232; baby = 0.344; teen = 0.372; adult = 0.38; senior = 0.4 }
$moods = 'base', 'gluecklich', 'hungrig', 'traurig', 'muede', 'krank', 'schmutzig', 'schlafend'
$stageNames = @{ egg = 'EI'; baby = 'BABY'; teen = 'TEEN'; adult = 'ERWACHSEN'; senior = 'OMA' }
$table = @{}

function Convert-Art([string]$path, [double]$s, [double]$angle, [string]$cname) {
  $img = [Drawing.Bitmap]::FromFile($path)
  $w = [int][Math]::Round($img.Width * $s); $h = [int][Math]::Round($img.Height * $s)
  $pad = if ($angle -ne 0) { 8 } else { 0 }
  $cw = $w + 2 * $pad; $ch = $h + $pad
  $small = New-Object Drawing.Bitmap $cw, $ch, ([Drawing.Imaging.PixelFormat]::Format32bppArgb)
  $g = [Drawing.Graphics]::FromImage($small)
  $g.Clear([Drawing.Color]::Transparent)
  $g.InterpolationMode = [Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
  $g.PixelOffsetMode = [Drawing.Drawing2D.PixelOffsetMode]::HighQuality
  if ($angle -ne 0) {
    # Um den Fusspunkt (unten Mitte) drehen, wie rotate() in der App
    $g.TranslateTransform($cw / 2, $ch); $g.RotateTransform($angle); $g.TranslateTransform(-$w / 2, -$h)
    $g.DrawImage($img, 0, 0, $w, $h)
  } else {
    $g.DrawImage($img, 0, 0, $w, $h)
  }
  $g.Dispose(); $img.Dispose()
  $linien = Get-Bits $cw $ch { param($x, $y) $c = $small.GetPixel($x, $y); $c.A -gt 127 -and (0.299 * $c.R + 0.587 * $c.G + 0.114 * $c.B) -lt 140 }
  $maske = Get-Bits $cw $ch { param($x, $y) $small.GetPixel($x, $y).A -gt 127 }
  $small.Dispose()
  Add-Array "${cname}_L" $linien
  Add-Array "${cname}_M" $maske
  $ox = -[int][Math]::Round($cw / 2); $oy = -$ch
  $lines.Add("static const Bild $cname = {$cw, $ch, $ox, $oy, ${cname}_L, ${cname}_M};")
}

foreach ($stage in 'egg', 'baby', 'teen', 'adult', 'senior') {
  foreach ($mood in $moods) {
    if ($artJs -notmatch "img/$stage-$mood\.png") { continue }
    $cname = "ART_$($stageNames[$stage])_$($mood.ToUpper())"
    Convert-Art (Join-Path $dist "img\$stage-$mood.png") $scale[$stage] 0 $cname
    $table["$stage/$mood"] = $cname
  }
}
# Das Ei wackelt: um 5 Grad nach links und rechts gedreht
Convert-Art (Join-Path $dist 'img\egg-base.png') $scale.egg 5 'ART_EI_RECHTS'
Convert-Art (Join-Path $dist 'img\egg-base.png') $scale.egg -5 'ART_EI_LINKS'

# Das Grab: die Pixelgrafik aus evi-sprite.js der App, genau wie dort gezeichnet
# (Szenenkoordinaten: translate(91 54) scale(2)). Grauer Grund = aussen.
$spriteUrl = 'file:///' + ((Join-Path $dist 'evi-sprite.js') -replace '\\', '/')
$html = @"
<!doctype html><html><head><style>body{margin:0;background:#808080}svg{display:block}</style></head><body>
<svg width="320" height="224" viewBox="0 0 320 224" shape-rendering="crispEdges"><g transform="translate(91 54) scale(2)"><path id="w" fill="#fff"/><path id="b" fill="#000"/></g></svg>
<script type="module">import { drawEvi } from '$spriteUrl'; const s = drawEvi({ stage: 'dead' });
document.getElementById('w').setAttribute('d', s.white); document.getElementById('b').setAttribute('d', s.black);</script></body></html>
"@
$file = Join-Path $work 'grab.html'; $png = Join-Path $work 'grab.png'
[IO.File]::WriteAllText($file, $html, (New-Object Text.UTF8Encoding($false)))
if (Test-Path $png) { Remove-Item $png }
Start-Process -FilePath $edge -Wait -WindowStyle Hidden -ArgumentList @('--headless=new', '--disable-gpu', '--hide-scrollbars',
  '--force-device-scale-factor=1', '--allow-file-access-from-files', '--virtual-time-budget=3000', '--window-size=320,224',
  "--user-data-dir=$work\profile", "--screenshot=$png", ('file:///' + ($file -replace '\\', '/')))
$grab = [Drawing.Bitmap]::FromFile($png)
$isIn = { param($x, $y) $c = $grab.GetPixel($x, $y); -not ($c.R -gt 100 -and $c.R -lt 160 -and $c.G -gt 100 -and $c.G -lt 160) }
$minX = 320; $minY = 224; $maxX = -1; $maxY = -1
for ($y = 0; $y -lt 224; $y++) { for ($x = 0; $x -lt 320; $x++) { if (& $isIn $x $y) {
  if ($x -lt $minX) { $minX = $x }; if ($x -gt $maxX) { $maxX = $x }; if ($y -lt $minY) { $minY = $y }; if ($y -gt $maxY) { $maxY = $y } } } }
if ($maxX -lt 0) { throw 'Grab nicht gezeichnet (Modul nicht geladen?)' }
$gw = $maxX - $minX + 1; $gh = $maxY - $minY + 1
Add-Array 'ART_GRAB_L' (Get-Bits $gw $gh { param($x, $y) $c = $grab.GetPixel($x + $minX, $y + $minY); $c.R -lt 60 })
Add-Array 'ART_GRAB_M' (Get-Bits $gw $gh { param($x, $y) & $isIn ($x + $minX) ($y + $minY) })
# Lage wie in der App: linke obere Ecke relativ zum Fusspunkt 167/180
$lines.Add("static const Bild ART_GRAB = {$gw, $gh, $($minX - 167), $($minY - 180), ART_GRAB_L, ART_GRAB_M};")
$grab.Dispose()

# Tabelle [Phase][Stimmung], nullptr = keine Zeichnung (dann Ersatz wie in der App)
$lines.Add('')
$lines.Add('// Reihenfolge der Stimmungen: ' + ($moods -join ', '))
$lines.Add('static const Bild* const ART[5][8] = {')
foreach ($stage in 'egg', 'baby', 'teen', 'adult', 'senior') {
  $row = foreach ($mood in $moods) { if ($table["$stage/$mood"]) { '&' + $table["$stage/$mood"] } else { 'nullptr' } }
  $lines.Add("  {$($row -join ', ')},  // $stage")
}
$lines.Add('};')

# ---------- Beleidigt ----------
$lines.Add('')
$lines.Add('// Beleidigt: Vorlagen aus entwuerfe/evi/emotionen, in der Reihenfolge von Phase.')
$prep = Join-Path $root 'handy-app\tools\prepare-art.ps1'
$emotionen = Join-Path $root 'entwuerfe\evi\emotionen'
$phasenOrdner = [ordered]@{ baby = 'baby'; teen = 'teenager'; adult = 'erwachsen'; senior = 'oma' }
$beleidigt = @('nullptr')  # Ei
foreach ($stage in $phasenOrdner.Keys) {
  $phase = $phasenOrdner[$stage]
  $datei = "evi-$phase-beleidigt.png"
  $vorlage = Join-Path (Join-Path $emotionen $phase) $datei
  $png = Join-Path $work $datei
  & $prep -In $vorlage -Out $png -Scale 0.31 | Out-Null
  $cname = "ART_$($stageNames[$stage])_BELEIDIGT"
  Convert-Art $png $scale[$stage] 0 $cname
  $beleidigt += "&$cname"
}
$lines.Add("static const Bild* const ART_BELEIDIGT[5] = {$($beleidigt -join ', ')};")

$out = [IO.Path]::GetFullPath((Join-Path $outDir 'assets.h'))
[IO.File]::WriteAllText($out, ($lines -join "`n") + "`n", (New-Object Text.UTF8Encoding($false)))
"assets.h geschrieben: $([Math]::Round((Get-Item $out).Length / 1KB)) KB Text"
