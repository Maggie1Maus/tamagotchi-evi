# Vorlage fuer Ohren: nur Kopf und Koerper (ohne Evis Ohren und Schwanz), blass, gleiche Position wie Vorlage-Evi.png
Add-Type -AssemblyName System.Drawing
$T=Join-Path $PSScriptRoot '..\..\grillme-genetik-prototyp\baukasten\teile\weiblich\erwachsen'
$basis=New-Object System.Drawing.Bitmap((Resolve-Path (Join-Path $T 'basis.png')).Path)
$bmp=New-Object System.Drawing.Bitmap(2048,2048)
$zg=[System.Drawing.Graphics]::FromImage($bmp);$zg.Clear([System.Drawing.Color]::White)
$cm=New-Object System.Drawing.Imaging.ColorMatrix;$cm.Matrix33=0.40
$ia=New-Object System.Drawing.Imaging.ImageAttributes;$ia.SetColorMatrix($cm)
$zg.InterpolationMode='NearestNeighbor';$zg.PixelOffsetMode='Half'
$zg.DrawImage($basis,(New-Object System.Drawing.Rectangle(340,328,(114*12),(116*12))),0,0,114,116,[System.Drawing.GraphicsUnit]::Pixel,$ia)
$zg.Dispose()
$bmp.Save((Join-Path $PSScriptRoot 'Vorlage-ohne-Ohren.png'),[System.Drawing.Imaging.ImageFormat]::Png)
