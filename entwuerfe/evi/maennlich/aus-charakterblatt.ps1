$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing
$src = Join-Path $PSScriptRoot 'charakterblatt-runde-1.png'
$bild = [Drawing.Bitmap]::new($src)
try {
  $phasen = @(
    @('02-Baby',70,265,390,365,99,89),
    @('03-Teenager',530,155,465,480,86,107),
    @('04-Erwachsen',1030,105,500,535,120,116),
    @('05-Opa',1570,105,590,535,121,110)
  )
  foreach ($p in $phasen) {
    $ordner = Join-Path $PSScriptRoot ('Sprites\'+$p[0])
    New-Item -ItemType Directory -Force $ordner | Out-Null
    $crop = [Drawing.Bitmap]::new([int]$p[3],[int]$p[4],[Drawing.Imaging.PixelFormat]::Format32bppArgb)
    try {
      $g=[Drawing.Graphics]::FromImage($crop)
      try { $g.DrawImage($bild,[Drawing.Rectangle]::new(0,0,[int]$p[3],[int]$p[4]),[Drawing.Rectangle]::new([int]$p[1],[int]$p[2],[int]$p[3],[int]$p[4]),[Drawing.GraphicsUnit]::Pixel) }
      finally { $g.Dispose() }
      # Nur die von außen erreichbare weiße Fläche wird transparent.
      $w=$crop.Width; $h=$crop.Height
      $seen=[bool[]]::new($w*$h); $queue=[Collections.Generic.Queue[int]]::new()
      for($x=0;$x -lt $w;$x++){ $queue.Enqueue($x); $queue.Enqueue(($h-1)*$w+$x) }
      for($y=0;$y -lt $h;$y++){ $queue.Enqueue($y*$w); $queue.Enqueue($y*$w+$w-1) }
      while($queue.Count -gt 0){
        $i=$queue.Dequeue(); if($i -lt 0 -or $i -ge $seen.Length -or $seen[$i]){continue}
        $seen[$i]=$true; $x=$i%$w; $y=[int][Math]::Floor($i/$w)
        $c=$crop.GetPixel($x,$y)
        if($c.R -lt 242 -or $c.G -lt 242 -or $c.B -lt 242){continue}
        $crop.SetPixel($x,$y,[Drawing.Color]::Transparent)
        if($x -gt 0){$queue.Enqueue($i-1)}
        if($x -lt $w-1){$queue.Enqueue($i+1)}
        if($y -gt 0){$queue.Enqueue($i-$w)}
        if($y -lt $h-1){$queue.Enqueue($i+$w)}
      }
      $out=[Drawing.Bitmap]::new([int]$p[5],[int]$p[6],[Drawing.Imaging.PixelFormat]::Format32bppArgb)
      try {
        $g=[Drawing.Graphics]::FromImage($out)
        try {$g.Clear([Drawing.Color]::Transparent);$g.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::NearestNeighbor;$g.PixelOffsetMode=[Drawing.Drawing2D.PixelOffsetMode]::Half;$g.DrawImage($crop,[Drawing.Rectangle]::new(0,0,$out.Width,$out.Height),[Drawing.Rectangle]::new(0,0,$w,$h),[Drawing.GraphicsUnit]::Pixel)}
        finally {$g.Dispose()}
        for($y=0;$y -lt $out.Height;$y++){for($x=0;$x -lt $out.Width;$x++){$c=$out.GetPixel($x,$y);if($c.A -eq 0){continue};$v=[int]([Math]::Round((($c.R+$c.G+$c.B)/3)/17)*17);$out.SetPixel($x,$y,[Drawing.Color]::FromArgb(255,$v,$v,$v))}}
        $out.Save((Join-Path $ordner 'gluecklich.png'),[Drawing.Imaging.ImageFormat]::Png)
      } finally {$out.Dispose()}
    } finally {$crop.Dispose()}
  }
} finally {$bild.Dispose()}
