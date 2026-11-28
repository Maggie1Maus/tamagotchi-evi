# Setzt die Teile zusammen (Ansicht frontal) und faerbt sie in Graustufen. Ergebnis: ..\ergebnis\Evi-neu-grau.png
# Aufruf: Rechtsklick -> Mit PowerShell ausfuehren. Andere Teile: Dateinamen als Parameter angeben,
# zum Beispiel  .\zusammensetzen.ps1 -Gesicht gluecklich -Ohr buschel -Hals schleife
param([string]$Gesicht='neutral',[string]$Ohr='buschel',[string]$Schwanz='buschel',[string]$Kopf='rund',[string]$Koerper='sitzend',[string]$Hals='krause-evi',[string]$Ansicht='frontal',[string]$Ausgabe='Evi-neu-grau.png')
Add-Type -AssemblyName System.Drawing
Add-Type -Path (Join-Path $PSScriptRoot 'zusammensetzen.cs') -ReferencedAssemblies System.Drawing
$w=(Resolve-Path (Join-Path $PSScriptRoot '..')).Path
[Zus]::Run("$w\koepfe\$Ansicht\kopf-$Kopf.png","$w\koerper\$Ansicht\koerper-$Koerper.png","$w\halsschmuck\$Ansicht\$Hals.png","$w\gesichter\$Ansicht\gesicht-$Gesicht.png","$w\ohren\$Ansicht\ohr-$Ohr.png","$w\schwaenze\$Ansicht\schwanz-$Schwanz.png","$w\ergebnis\$Ausgabe")
