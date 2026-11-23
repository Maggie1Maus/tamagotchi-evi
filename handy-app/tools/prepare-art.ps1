param([string]$In, [string]$Out, [double]$Scale)
Add-Type -AssemblyName System.Drawing
if (-not ('ArtPrep' -as [type])) {
Add-Type -ReferencedAssemblies System.Drawing -TypeDefinition @'
using System; using System.Drawing; using System.Drawing.Drawing2D; using System.Drawing.Imaging; using System.Collections.Generic;
public static class ArtPrep {
  // Crops the figure (without the label below it), scales it to the given height,
  // keeps pure black and white, and makes the white around the figure transparent.
  public static string Run(string input, string output, double scale) {
    var src = new Bitmap(input); int w = src.Width, h = src.Height;
    Func<int,int,bool> ink = (x,y) => { var c = src.GetPixel(x,y); return c.R + c.G + c.B < 384; };
    var rowInk = new bool[h];
    for (int y=0;y<h;y++) for (int x=0;x<w;x+=2) if (ink(x,y)) { rowInk[y]=true; break; }
    // Ink rows grouped into blocks (gaps of up to 8 blank rows stay in one block).
    var blocks = new List<int[]>(); int start=-1, last=-1;
    for (int y=0;y<h;y++){ if(!rowInk[y]) continue; if(start<0){start=y;} else if(y-last>3){ blocks.Add(new[]{start,last}); start=y; } last=y; }
    if (start>=0) blocks.Add(new[]{start,last});
    // Label lines: short blocks at the bottom of the picture. Drop them.
    while (blocks.Count>1) { var b=blocks[blocks.Count-1]; if (b[1]-b[0] < 100 && b[0] > h*0.7) blocks.RemoveAt(blocks.Count-1); else break; }
    int top=blocks[0][0], bottom=blocks[blocks.Count-1][1];
    int left=w,right=0;
    for (int y=top;y<=bottom;y++) for(int x=0;x<w;x++) if(ink(x,y)){ if(x<left)left=x; if(x>right)right=x; }
    int cw = right-left+1, ch = bottom-top+1;
    int height = (int)Math.Round(ch * scale), ow = (int)Math.Round(cw * scale);
    var scaled = new Bitmap(ow, height);
    using (var g = Graphics.FromImage(scaled)) {
      g.Clear(Color.White); g.InterpolationMode = InterpolationMode.HighQualityBicubic; g.PixelOffsetMode = PixelOffsetMode.HighQuality;
      g.DrawImage(src, new Rectangle(0,0,ow,height), new Rectangle(left,top,cw,ch), GraphicsUnit.Pixel);
    }
    // 0 = outside (transparent), 1 = black, 2 = white inside
    var m = new int[ow,height];
    for (int y=0;y<height;y++) for(int x=0;x<ow;x++){ var c=scaled.GetPixel(x,y); m[x,y] = c.R+c.G+c.B < 3*140 ? 1 : 2; }
    // The outside is found with the outline thickened by 1 pixel, so a tiny gap in the outline
    // (one pixel wide) does not let the outside run into the figure. Then the white seam right
    // next to the outline is added to the outside. The black and white pixels stay as they are.
    var near = new bool[ow,height];
    for (int y=0;y<height;y++) for(int x=0;x<ow;x++)
      for (int yy=Math.Max(0,y-1); yy<=Math.Min(height-1,y+1); yy++) for (int xx=Math.Max(0,x-1); xx<=Math.Min(ow-1,x+1); xx++) if (m[xx,yy]==1) near[x,y]=true;
    var q = new Queue<int>();
    for (int y=0;y<height;y++) for(int x=0;x<ow;x++) if((x==0||y==0||x==ow-1||y==height-1)&&m[x,y]==2&&!near[x,y]){ m[x,y]=0; q.Enqueue(y*ow+x); }
    while(q.Count>0){ int p=q.Dequeue(), x=p%ow, y=p/ow; int[] dx={1,-1,0,0}, dy={0,0,1,-1};
      for(int k=0;k<4;k++){ int nx=x+dx[k], ny=y+dy[k]; if(nx>=0&&ny>=0&&nx<ow&&ny<height&&m[nx,ny]==2&&!near[nx,ny]){ m[nx,ny]=0; q.Enqueue(ny*ow+nx);} } }
    var seam = new List<int>();
    for (int y=0;y<height;y++) for(int x=0;x<ow;x++) {
      if (m[x,y]!=2) continue;
      bool edge = x==0||y==0||x==ow-1||y==height-1;
      for (int yy=Math.Max(0,y-1); yy<=Math.Min(height-1,y+1); yy++) for (int xx=Math.Max(0,x-1); xx<=Math.Min(ow-1,x+1); xx++) if (m[xx,yy]==0) edge=true;
      if (edge) seam.Add(y*ow+x);
    }
    foreach (int p in seam) m[p%ow, p/ow] = 0;
    var outBmp = new Bitmap(ow+2, height+2, PixelFormat.Format32bppArgb);
    for (int y=0;y<height+2;y++) for(int x=0;x<ow+2;x++) outBmp.SetPixel(x,y,Color.Transparent);
    for (int y=0;y<height;y++) for(int x=0;x<ow;x++) outBmp.SetPixel(x+1,y+1, m[x,y]==0 ? Color.Transparent : m[x,y]==1 ? Color.Black : Color.White);
    outBmp.Save(output, ImageFormat.Png);
    return (ow+2) + " " + (height+2);
  }
}
'@
}
[ArtPrep]::Run($In, $Out, $Scale)
