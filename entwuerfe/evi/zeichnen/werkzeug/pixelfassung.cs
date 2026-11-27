using System;using System.Drawing;using System.Drawing.Imaging;
public static class Pix{
 const int N=2048;
 public static string Run(string eingabe,string spriteOut,string vorschauOut){
  var b=new Bitmap(eingabe);var d=b.LockBits(new Rectangle(0,0,N,N),ImageLockMode.ReadOnly,PixelFormat.Format32bppArgb);
  var buf=new byte[N*N*4];System.Runtime.InteropServices.Marshal.Copy(d.Scan0,buf,0,buf.Length);b.UnlockBits(d);
  int minx=N,miny=N,maxx=0,maxy=0;
  for(int y=0;y<N;y++)for(int x=0;x<N;x++)if(buf[(y*N+x)*4+3]>60){if(x<minx)minx=x;if(x>maxx)maxx=x;if(y<miny)miny=y;if(y>maxy)maxy=y;}
  int W=114,H=116;double w=maxx-minx+1,h=maxy-miny+1;
  double cell=Math.Max(h/(H-2),w/(W-2));                       // je 1 Zelle Rand
  double ox=minx-(W*cell-w)/2.0, oy=maxy+1-H*cell+cell;        // Unterkante 1 Zelle ueber Rand
  var sp=new Bitmap(W,H,PixelFormat.Format32bppArgb);
  for(int cy=0;cy<H;cy++)for(int cx=0;cx<W;cx++){
   int x0=(int)Math.Floor(ox+cx*cell),x1=(int)Math.Floor(ox+(cx+1)*cell),y0=(int)Math.Floor(oy+cy*cell),y1=(int)Math.Floor(oy+(cy+1)*cell);
   if(x1<=x0)x1=x0+1;if(y1<=y0)y1=y0+1;
   int tot=0,op=0,dark=0;double sum=0;
   for(int y=y0;y<y1;y++)for(int x=x0;x<x1;x++){tot++;if(x<0||y<0||x>=N||y>=N)continue;int i=(y*N+x)*4;int a=buf[i+3];if(a<128)continue;op++;int g=buf[i];if(g<=70)dark++;else sum+=g;}
   if(op*2<tot)continue;                                          // mehr als halb leer -> transparent
   int v;
   if(dark*100>=tot*30) v=51;                                     // Linie/dunkle Flaeche
   else if(op-dark>0) v=(int)Math.Round(sum/(op-dark)/17.0)*17;   // Fuellung, auf 16 Stufen gerundet
   else v=51;
   if(v>255)v=255;sp.SetPixel(cx,cy,Color.FromArgb(255,v,v,v));}
  sp.Save(spriteOut,ImageFormat.Png);
  var big=new Bitmap(W*8,H*8);var g2=Graphics.FromImage(big);g2.Clear(Color.White);g2.InterpolationMode=System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;g2.PixelOffsetMode=System.Drawing.Drawing2D.PixelOffsetMode.Half;g2.DrawImage(sp,0,0,W*8,H*8);g2.Dispose();big.Save(vorschauOut,ImageFormat.Png);
  return "Zelle "+cell.ToString("0.00")+" px, Figur "+w+"x"+h;}
}
