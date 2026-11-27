# Rechnet ..\ergebnis\Evi-neu-grau.png auf Evis Pixelraster (114 x 116, 16 Graustufen) herunter.
Add-Type -AssemblyName System.Drawing
Add-Type -Path (Join-Path $PSScriptRoot 'pixelfassung.cs') -ReferencedAssemblies System.Drawing
$erg=(Resolve-Path (Join-Path $PSScriptRoot '..\ergebnis')).Path
[Pix]::Run((Join-Path $erg 'Evi-neu-grau.png'),(Join-Path $erg 'Evi-neu-pixel.png'),(Join-Path $erg 'Evi-neu-pixel-8x.png'))
