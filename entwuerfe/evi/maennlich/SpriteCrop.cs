using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;

public static class SpriteCrop {
  public static void Export(string source, int cols, int rows, int index, string destination, int targetW, int targetH) {
    using (var sheet = new Bitmap(source)) {
      int col=index%cols, row=index/cols;
      int x0=col*sheet.Width/cols, y0=row*sheet.Height/rows;
      int w=(col+1)*sheet.Width/cols-x0, h=(row+1)*sheet.Height/rows-y0;
      using(var cell=new Bitmap(w,h,PixelFormat.Format32bppArgb)) {
        using(var g=Graphics.FromImage(cell)) g.DrawImage(sheet,new Rectangle(0,0,w,h),new Rectangle(x0,y0,w,h),GraphicsUnit.Pixel);
        var seen=new bool[w*h]; var q=new int[w*h*4+2*w+2*h]; int head=0,tail=0;
        for(int x=0;x<w;x++){q[tail++]=x;q[tail++]=(h-1)*w+x;}
        for(int y=0;y<h;y++){q[tail++]=y*w;q[tail++]=y*w+w-1;}
        while(head<tail){int i=q[head++];if(i<0||i>=seen.Length||seen[i])continue;seen[i]=true;int x=i%w,y=i/w;Color c=cell.GetPixel(x,y);if(c.R<235||c.G<235||c.B<235)continue;cell.SetPixel(x,y,Color.Transparent);if(x>0)q[tail++]=i-1;if(x<w-1)q[tail++]=i+1;if(y>0)q[tail++]=i-w;if(y<h-1)q[tail++]=i+w;}
        int minX=w,minY=h,maxX=-1,maxY=-1;
        for(int y=0;y<h;y++)for(int x=0;x<w;x++)if(cell.GetPixel(x,y).A>0){minX=Math.Min(minX,x);maxX=Math.Max(maxX,x);minY=Math.Min(minY,y);maxY=Math.Max(maxY,y);}
        if(maxX<0)throw new Exception("Empty sprite: "+destination);
        int bw=maxX-minX+1,bh=maxY-minY+1;
        double scale=Math.Min((targetW-4.0)/bw,(targetH-4.0)/bh);
        int dw=Math.Max(1,(int)Math.Round(bw*scale)),dh=Math.Max(1,(int)Math.Round(bh*scale));
        int dx=(targetW-dw)/2,dy=targetH-dh-2;
        using(var output=new Bitmap(targetW,targetH,PixelFormat.Format32bppArgb)){
          using(var g=Graphics.FromImage(output)){g.Clear(Color.Transparent);g.InterpolationMode=InterpolationMode.NearestNeighbor;g.PixelOffsetMode=PixelOffsetMode.Half;g.DrawImage(cell,new Rectangle(dx,dy,dw,dh),new Rectangle(minX,minY,bw,bh),GraphicsUnit.Pixel);}
          for(int y=0;y<targetH;y++)for(int x=0;x<targetW;x++){Color c=output.GetPixel(x,y);if(c.A==0)continue;int v=(int)Math.Round(((c.R+c.G+c.B)/3.0)/17)*17;v=Math.Max(0,Math.Min(255,v));output.SetPixel(x,y,Color.FromArgb(255,v,v,v));}
          output.Save(destination,ImageFormat.Png);
        }
      }
    }
  }
}
