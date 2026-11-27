$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing
$dllDir=Split-Path ([Drawing.Bitmap].Assembly.Location)
Add-Type -Path (Join-Path $PSScriptRoot 'SpriteCrop.cs') -ReferencedAssemblies @([Drawing.Bitmap].Assembly.Location,(Join-Path $dllDir 'System.Private.Windows.GdiPlus.dll'),(Join-Path $dllDir 'System.Private.Windows.Core.dll'),(Join-Path $dllDir 'System.Drawing.Primitives.dll'),(Join-Path $PSHOME 'System.Collections.dll'),(Join-Path $PSHOME 'System.Runtime.dll'),[string].Assembly.Location)
$sizes = @{
  baby=@('02-Baby',99,89); teenager=@('03-Teenager',86,107)
  erwachsen=@('04-Erwachsen',120,116); opa=@('05-Opa',121,110)
}
$emotionen=@('ruhe','gluecklich','hungrig','traurig','muede','krank','schmutzig','schlafend','beleidigt')
$posen=@('laufen','fenster','denken','singen','gaehnen','drehen','huepfen','staunen','garten-schnuppern','garten-untersuchen','garten-zeigen')
foreach($alter in @('baby','teenager','erwachsen','opa')){
  $dir=Join-Path $PSScriptRoot ('Sprites\'+$sizes[$alter][0]);New-Item -ItemType Directory -Force $dir | Out-Null
  foreach($typ in @('emotionen','posen')){
    $quelle=Join-Path $PSScriptRoot "quellen\$typ-$alter.png"
    $namen=if($typ -eq 'emotionen'){$emotionen}else{$posen}
    $cols=if($typ -eq 'emotionen'){3}else{4}
    for($i=0;$i -lt $namen.Count;$i++){
      if($namen[$i] -eq 'gluecklich'){continue}
      [SpriteCrop]::Export($quelle,$cols,3,$i,(Join-Path $dir ($namen[$i]+'.png')),[int]$sizes[$alter][1],[int]$sizes[$alter][2])
    }
  }
}
[SpriteCrop]::Export((Join-Path $PSScriptRoot 'quellen\opa-beleidigt-korrigiert.png'),1,1,0,(Join-Path $PSScriptRoot 'Sprites\05-Opa\beleidigt.png'),121,110)
Write-Host 'Männliche Evi: 80 Einzelbilder vorhanden.'
