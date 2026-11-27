param($ohr,$out)
Add-Type -AssemblyName System.Drawing
$T='C:\Users\Maggi\Documents\ChatGPT\Tamagotchi Projekt 2\entwuerfe\grillme-genetik-prototyp\baukasten\teile\weiblich\erwachsen'
$src=New-Object System.Drawing.Bitmap($ohr)
$basis=New-Object System.Drawing.Bitmap("$T\basis.png")
$evOhr=New-Object System.Drawing.Bitmap("$T\ohren\evi.png")
$schw=New-Object System.Drawing.Bitmap("$T\schwanz\evi.png")
$N=2048/12|% {[int]$_}  # Zellen je Zeile (Raster Ursprung 4,4, 12 px)
$cols=170;$rows=170
# 1) schwarze Zellen
$blk=New-Object 'bool[,]' $rows,$cols
for($r=0;$r -lt $rows;$r++){for($c=0;$c -lt $cols;$c++){
 $n=0;for($y=4+12*$r;$y -lt 16+12*$r -and $y -lt 2048;$y+=2){for($x=4+12*$c;$x -lt 16+12*$c -and $x -lt 2048;$x+=2){$p=$src.GetPixel($x,$y);if($p.A -gt 128 -and $p.R -lt 140){$n++}}}
 if($n -ge 5){$blk[$r,$c]=$true}}}
# 2) Fuellen von aussen (4er-Nachbarschaft)
$lab=New-Object 'int[,]' $rows,$cols   # 0 unbekannt, 1 aussen
$q=New-Object System.Collections.Queue
$q.Enqueue(@(0,0));$lab[0,0]=1
while($q.Count){$t=$q.Dequeue();$r=$t[0];$c=$t[1]
 foreach($d in @(@(1,0),@(-1,0),@(0,1),@(0,-1))){$nr=$r+$d[0];$nc=$c+$d[1]
  if($nr -ge 0 -and $nr -lt $rows -and $nc -ge 0 -and $nc -lt $cols -and -not $blk[$nr,$nc] -and $lab[$nr,$nc] -eq 0){$lab[$nr,$nc]=1;$q.Enqueue(@($nr,$nc))}}}
# 3) Innenflaechen in Komponenten
$comp=New-Object 'int[,]' $rows,$cols;$sizes=@{};$id=1
for($r=0;$r -lt $rows;$r++){for($c=0;$c -lt $cols;$c++){
 if(-not $blk[$r,$c] -and $lab[$r,$c] -eq 0 -and $comp[$r,$c] -eq 0){
  $id++;$cnt=0;$q.Enqueue(@($r,$c));$comp[$r,$c]=$id
  while($q.Count){$t=$q.Dequeue();$cnt++;$rr=$t[0];$cc=$t[1]
   foreach($d in @(@(1,0),@(-1,0),@(0,1),@(0,-1))){$nr=$rr+$d[0];$nc=$cc+$d[1]
    if($nr -ge 0 -and $nr -lt $rows -and $nc -ge 0 -and $nc -lt $cols -and -not $blk[$nr,$nc] -and $lab[$nr,$nc] -eq 0 -and $comp[$nr,$nc] -eq 0){$comp[$nr,$nc]=$id;$q.Enqueue(@($nr,$nc))}}}
  $sizes[$id]=$cnt}}}
"Innenflaechen: " + (($sizes.GetEnumerator()|%{"$($_.Key)=$($_.Value)"}) -join ', ')
$dunkel=($sizes.GetEnumerator()|sort Value|select -first 1).Key   # kleinste = Ohrmuschel
# 4) Ohrbild im Spritemass (114x116): Zelle (r,c) -> Sprite (c-28, r-27)
$W=114;$H=116
$ear=New-Object System.Drawing.Bitmap($W,$H)
$cOut=[System.Drawing.Color]::FromArgb(255,49,49,49);$cFur=[System.Drawing.Color]::FromArgb(255,212,212,212);$cIn=[System.Drawing.Color]::FromArgb(255,92,92,92)
for($r=0;$r -lt $rows;$r++){for($c=0;$c -lt $cols;$c++){
 $sx=$c-28;$sy=$r-27;if($sx -lt 0 -or $sx -ge $W -or $sy -lt 0 -or $sy -ge $H){continue}
 if($blk[$r,$c]){$ear.SetPixel($sx,$sy,$cOut)}elseif($comp[$r,$c] -eq $dunkel){$ear.SetPixel($sx,$sy,$cIn)}elseif($comp[$r,$c] -gt 1){$ear.SetPixel($sx,$sy,$cFur)}}}
# Spiegelachse aus Evis Ohren
$minL=999;$maxR=-1;for($y=0;$y -lt $H;$y++){for($x=0;$x -lt $W;$x++){if($evOhr.GetPixel($x,$y).A -gt 128){if($x -lt $minL){$minL=$x};if($x -gt $maxR){$maxR=$x}}}}
"Ohr-Spanne $minL..$maxR"
$res=New-Object System.Drawing.Bitmap($W,$H)
$g=[System.Drawing.Graphics]::FromImage($res);$g.Clear([System.Drawing.Color]::White)
$g.DrawImage($schw,0,0);$g.DrawImage($basis,0,0)
$g.DrawImage($ear,0,0)
$m=New-Object System.Drawing.Bitmap($W,$H)
for($y=0;$y -lt $H;$y++){for($x=0;$x -lt $W;$x++){$p=$ear.GetPixel($x,$y);if($p.A -gt 0){$nx=$minL+$maxR-$x;if($nx -ge 0 -and $nx -lt $W){$m.SetPixel($nx,$y,$p)}}}}
$g.DrawImage($m,0,0);$g.Dispose()
$big=New-Object System.Drawing.Bitmap(($W*8),($H*8));$g2=[System.Drawing.Graphics]::FromImage($big)
$g2.InterpolationMode='NearestNeighbor';$g2.PixelOffsetMode='Half';$g2.DrawImage($res,0,0,($W*8),($H*8));$g2.Dispose()
$big.Save($out,[System.Drawing.Imaging.ImageFormat]::Png)
