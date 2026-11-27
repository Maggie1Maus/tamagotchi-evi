param()
$ErrorActionPreference = 'Stop'
$hier = Split-Path -Parent $MyInvocation.MyCommand.Path
$repo = (Resolve-Path (Join-Path $hier '..\..\..')).Path
$daten = [ordered]@{}
function Add-Bild([string]$kennung, [string]$pfad) {
  if (Test-Path -LiteralPath $pfad) {
    $bytes = [IO.File]::ReadAllBytes($pfad)
    $daten[$kennung] = 'data:image/png;base64,' + [Convert]::ToBase64String($bytes)
  }
}
Get-ChildItem -LiteralPath (Join-Path $hier 'teile') -Filter '*.png' -File -Recurse | ForEach-Object {
  $rel = $_.FullName.Substring((Join-Path $hier 'teile').Length + 1).Replace('\','/')
  Add-Bild "teile/$rel" $_.FullName
}
$zipPfad = Join-Path $repo 'entwuerfe\evi\graustufen-vorschau\Evi-Graustufen.zip'
Add-Type -AssemblyName System.IO.Compression.FileSystem
$zip = [IO.Compression.ZipFile]::OpenRead($zipPfad)
try {
  $phasen = @{ baby='02-Baby'; teenager='03-Teenager'; erwachsen='04-Erwachsen'; senior='05-Oma' }
  foreach ($alter in $phasen.Keys) {
    $eintrag = $zip.GetEntry("Sprites/$($phasen[$alter])/gluecklich.png")
    if ($eintrag) {
      $stream = $eintrag.Open(); $speicher = New-Object IO.MemoryStream
      try { $stream.CopyTo($speicher); $daten["original/evi/$alter"] = 'data:image/png;base64,' + [Convert]::ToBase64String($speicher.ToArray()) }
      finally { $stream.Dispose(); $speicher.Dispose() }
    }
  }
} finally { $zip.Dispose() }
$park = Join-Path $repo 'entwuerfe\neue-tamagotchis\graustufen'
Get-ChildItem -LiteralPath $park -Directory | ForEach-Object {
  $figur = $_.Name
  foreach ($alter in @('baby','teenager','erwachsen','senior')) {
    Add-Bild "original/$figur/$alter" (Join-Path $_.FullName "$alter-gluecklich.png")
  }
}
$mann = Join-Path $repo 'entwuerfe\evi\maennlich\Sprites'
$mannAlter = @{ baby='02-Baby'; teenager='03-Teenager'; erwachsen='04-Erwachsen'; senior='05-Opa' }
foreach ($alter in $mannAlter.Keys) {
  Add-Bild "original/evi-m/$alter" (Join-Path $mann "$($mannAlter[$alter])\gluecklich.png")
}
$json = ConvertTo-Json -InputObject $daten -Depth 3 -Compress
[IO.File]::WriteAllText((Join-Path $hier 'teile-daten.js'), "window.TEILE_DATEN = $json;`n", [Text.UTF8Encoding]::new($false))
Write-Host "teile-daten.js aktualisiert: $($daten.Count) Bilder."
