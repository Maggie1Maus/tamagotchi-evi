# Rechnet Evi-neu-grau.png auf Evis Pixelraster (114 x 116, 16 Graustufen) herunter.
Add-Type -AssemblyName System.Drawing
Add-Type -Path (Join-Path $PSScriptRoot 'pixelfassung.cs') -ReferencedAssemblies System.Drawing
[Pix]::Run((Join-Path $PSScriptRoot 'Evi-neu-grau.png'),(Join-Path $PSScriptRoot 'Evi-neu-pixel.png'),(Join-Path $PSScriptRoot 'Evi-neu-pixel-8x.png'))
