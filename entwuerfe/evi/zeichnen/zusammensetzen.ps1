# Setzt die sechs Evi-neue-... Ebenen zusammen und faerbt sie in Graustufen (16er-Stufen).
# Aufruf: Rechtsklick -> Mit PowerShell ausfuehren (im selben Ordner wie die PNG-Ebenen).
Add-Type -AssemblyName System.Drawing
Add-Type -Path (Join-Path $PSScriptRoot 'zusammensetzen.cs') -ReferencedAssemblies System.Drawing
[Zus]::Run($PSScriptRoot,(Join-Path $PSScriptRoot 'Evi-neu-grau.png'))
