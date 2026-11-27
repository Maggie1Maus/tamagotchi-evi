$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$dllDir=Split-Path ([Drawing.Bitmap].Assembly.Location)
$refs=@([Drawing.Bitmap].Assembly.Location,(Join-Path $dllDir 'System.Private.Windows.GdiPlus.dll'),(Join-Path $dllDir 'System.Private.Windows.Core.dll'),(Join-Path $dllDir 'System.Drawing.Primitives.dll'),(Join-Path $PSHOME 'System.Collections.dll'),(Join-Path $PSHOME 'System.Runtime.dll'),[string].Assembly.Location)
Add-Type -Path (Join-Path $PSScriptRoot 'ProofSheet.cs') -ReferencedAssemblies $refs
[ProofSheet]::Run((Resolve-Path (Join-Path $PSScriptRoot '..')).Path)
