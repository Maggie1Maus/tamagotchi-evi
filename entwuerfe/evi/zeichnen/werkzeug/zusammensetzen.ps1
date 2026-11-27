# Setzt die sechs Ebenen aus ..\basis zusammen und faerbt sie in Graustufen (Ergebnis in ..\ergebnis).
# Aufruf: Rechtsklick -> Mit PowerShell ausfuehren
Add-Type -AssemblyName System.Drawing
Add-Type -Path (Join-Path $PSScriptRoot 'zusammensetzen.cs') -ReferencedAssemblies System.Drawing
$basis=(Resolve-Path (Join-Path $PSScriptRoot '..\basis')).Path
[Zus]::Run($basis,(Join-Path $PSScriptRoot '..\ergebnis\Evi-neu-grau.png'))
