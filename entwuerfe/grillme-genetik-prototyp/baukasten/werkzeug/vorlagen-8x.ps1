$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$hier=$PSScriptRoot
$repo=(Resolve-Path (Join-Path $hier '..\..\..\..')).Path
$quellen=@{
  'evi-weiblich'=(Join-Path $hier 'evi-weiblich-original.png')
  'evi-maennlich'=(Join-Path $hier 'evi-maennlich-original.png')
}
foreach($name in @('leo','linus','groschen','mia','gloeckchen','lotti','klecksi','funke')){$quellen[$name]=Join-Path $repo "entwuerfe\neue-tamagotchis\graustufen\$name\erwachsen-gluecklich.png"}
foreach($name in $quellen.Keys){
  $src=[Drawing.Bitmap]::new($quellen[$name])
  try{
    $out=[Drawing.Bitmap]::new($src.Width*8,$src.Height*8,[Drawing.Imaging.PixelFormat]::Format24bppRgb)
    try{
      $g=[Drawing.Graphics]::FromImage($out)
      try{$g.Clear([Drawing.Color]::White);$g.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::NearestNeighbor;$g.PixelOffsetMode=[Drawing.Drawing2D.PixelOffsetMode]::Half;$g.DrawImage($src,[Drawing.Rectangle]::new(0,0,$out.Width,$out.Height),[Drawing.Rectangle]::new(0,0,$src.Width,$src.Height),[Drawing.GraphicsUnit]::Pixel)}finally{$g.Dispose()}
      $out.Save((Join-Path $hier "$name-8x.png"),[Drawing.Imaging.ImageFormat]::Png)
    }finally{$out.Dispose()}
  }finally{$src.Dispose()}
}
