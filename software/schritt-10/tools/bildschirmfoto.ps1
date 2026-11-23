# Holt ein Bildschirmfoto vom Waveshare-Board über USB und speichert es als PNG.
# Das Board muss evi-handy laufen haben. Die Arduino IDE darf den Port nicht belegen
# (seriellen Monitor schließen).
# Aufruf: powershell -ExecutionPolicy Bypass -File software\schritt-10\tools\bildschirmfoto.ps1 [-Port COM5] [-Datei foto.png]
param([string]$Port = '', [string]$Datei = (Join-Path (Get-Location) 'bildschirmfoto.png'))
Add-Type -AssemblyName System.Drawing
if (-not $Port) {
  $ports = [System.IO.Ports.SerialPort]::GetPortNames()
  if ($ports.Count -eq 0) { throw 'Kein COM-Port gefunden. Ist das Board eingesteckt?' }
  $Port = $ports[-1]
}
$sp = New-Object System.IO.Ports.SerialPort $Port, 115200
$sp.ReadTimeout = 4000
$sp.DtrEnable = $true
$sp.Open()
try {
  Start-Sleep -Milliseconds 300
  $sp.DiscardInBuffer()
  $sp.Write('S')
  do { $zeile = $sp.ReadLine().Trim() } until ($zeile -like 'BILD*')
  $daten = New-Object byte[] 9600
  $gelesen = 0
  while ($gelesen -lt 9600) { $gelesen += $sp.Read($daten, $gelesen, 9600 - $gelesen) }
} finally { $sp.Close() }
$bmp = New-Object Drawing.Bitmap 640, 480
for ($y = 0; $y -lt 240; $y++) { for ($x = 0; $x -lt 320; $x++) {
  $weiss = $daten[$y * 40 + [Math]::Floor($x / 8)] -band (0x80 -shr ($x % 8))
  $c = if ($weiss) { [Drawing.Color]::White } else { [Drawing.Color]::Black }
  $bmp.SetPixel(2 * $x, 2 * $y, $c); $bmp.SetPixel(2 * $x + 1, 2 * $y, $c)
  $bmp.SetPixel(2 * $x, 2 * $y + 1, $c); $bmp.SetPixel(2 * $x + 1, 2 * $y + 1, $c)
} }
$bmp.Save($Datei); $bmp.Dispose()
"Gespeichert: $Datei (Port $Port)"
