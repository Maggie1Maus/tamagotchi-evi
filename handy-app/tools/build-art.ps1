# Prepares all of the owner's drawings at one scale per life stage and writes dist/art.js.
$prep = Join-Path $PSScriptRoot 'prepare-art.ps1'
# Run from anywhere: powershell -ExecutionPolicy Bypass -File handy-app\tools\build-art.ps1
$root = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path
$src = Join-Path $root 'entwuerfe\evi'
$out = Join-Path $root 'handy-app\dist\img'
Get-ChildItem $out -Filter *.png | Remove-Item -Confirm:$false
$scale = 0.31
$moods = 'gluecklich','hungrig','traurig','muede','krank','schmutzig','schlafend'
# Drawings for Evi's own moments (version 0.10 grown-up, since 0.12 also baby, teenager, grandma).
# Optional: each one is used as soon as entwuerfe\evi\momente\<prefix>-<name>.png exists.
$momentArt = 'laufen','fenster','denken','singen','gaehnen','drehen','huepfen','staunen'
$stages = [ordered]@{
  egg    = @{ dir = $null;        prefix = $null;            base = 'evi-ei-v2.png' }
  baby   = @{ dir = 'baby';       prefix = 'evi-baby';       base = 'evi-baby.png' }
  teen   = @{ dir = 'teenager';   prefix = 'evi-teenager';   base = 'evi-teenager.png' }
  adult  = @{ dir = 'erwachsen';  prefix = 'evi-erwachsen';  base = $null }
  senior = @{ dir = 'oma';        prefix = 'evi-oma';        base = 'evi-oma.png' }
}
$js = @('// Generated from entwuerfe/evi/ by build-art.ps1: the drawings, unchanged except cropped,',
        '// scaled to one size per stage and made transparent outside. [file, width, height] in pixels.',
        'export const ART = {')
foreach ($stage in $stages.Keys) {
  $info = $stages[$stage]
  $entries = @()
  $files = @()
  if ($info.base) { $files += ,@('base', (Join-Path $src $info.base)) }
  if ($info.dir) { foreach ($m in $moods) { $files += ,@($m, (Join-Path $src "emotionen\$($info.dir)\$($info.prefix)-$m.png")) } }
  if ($info.prefix) {
    foreach ($m in $momentArt) {
      $file = Join-Path $src "momente\$($info.prefix)-$m.png"
      if (Test-Path $file) { $files += ,@($m, $file) }
    }
  }
  foreach ($f in $files) {
    $name = "$stage-$($f[0]).png"
    $size = (& $prep -In $f[1] -Out (Join-Path $out $name) -Scale $scale).Split(' ')
    $entries += "    $($f[0]): ['img/$name', $($size[0]), $($size[1])]"
  }
  $js += "  ${stage}: {"
  $js += ($entries -join ",`n")
  $js += '  },'
}
$js[-1] = '  }'
$js += '};'

[System.IO.File]::WriteAllText((Join-Path $root 'handy-app\dist\art.js'), ($js -join "`n"))
Get-Content (Join-Path $root 'handy-app\dist\art.js')
