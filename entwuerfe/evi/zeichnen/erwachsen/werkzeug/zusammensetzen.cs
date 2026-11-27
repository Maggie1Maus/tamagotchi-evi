using System;using System.Collections.Generic;using System.Drawing;using System.Drawing.Imaging;using System.IO;
public static class Zus{
 const int N=2048;public static string Dbg="";
 public class L{public float[] g=new float[N*N];public float[] a=new float[N*N];public bool[] line=new bool[N*N];public bool[] shad=new bool[N*N];public bool[] cover=new bool[N*N];}
 static void Read(string f,out byte[] lum,out byte[] al){
  var b=new Bitmap(f);var d=b.LockBits(new Rectangle(0,0,N,N),ImageLockMode.ReadOnly,PixelFormat.Format32bppArgb);
  var buf=new byte[N*N*4];System.Runtime.InteropServices.Marshal.Copy(d.Scan0,buf,0,buf.Length);b.UnlockBits(d);
  lum=new byte[N*N];al=new byte[N*N];
  for(int i=0;i<N*N;i++){al[i]=buf[i*4+3];lum[i]=(byte)((buf[i*4]+buf[i*4+1]+buf[i*4+2])/3);} }
 static bool[] Dil(bool[] m,int r){
  var t=new bool[N*N];var o=new bool[N*N];
  for(int y=0;y<N;y++){for(int x=0;x<N;x++){bool v=false;int x0=Math.Max(0,x-r),x1=Math.Min(N-1,x+r);for(int k=x0;k<=x1;k++)if(m[y*N+k]){v=true;break;}t[y*N+x]=v;}}
  for(int x=0;x<N;x++)for(int y=0;y<N;y++){bool v=false;int y0=Math.Max(0,y-r),y1=Math.Min(N-1,y+r);for(int k=y0;k<=y1;k++)if(t[k*N+x]){v=true;break;}o[y*N+x]=v;}
  return o;}
 public static float[] DT(bool[] src){
  float INF=1e9f;var d=new float[N*N];for(int i=0;i<N*N;i++)d[i]=src[i]?0:INF;
  for(int y=0;y<N;y++)for(int x=0;x<N;x++){int p=y*N+x;float v=d[p];
   if(x>0&&d[p-1]+1<v)v=d[p-1]+1;if(y>0&&d[p-N]+1<v)v=d[p-N]+1;
   if(x>0&&y>0&&d[p-N-1]+1.414f<v)v=d[p-N-1]+1.414f;if(x<N-1&&y>0&&d[p-N+1]+1.414f<v)v=d[p-N+1]+1.414f;d[p]=v;}
  for(int y=N-1;y>=0;y--)for(int x=N-1;x>=0;x--){int p=y*N+x;float v=d[p];
   if(x<N-1&&d[p+1]+1<v)v=d[p+1]+1;if(y<N-1&&d[p+N]+1<v)v=d[p+N]+1;
   if(x<N-1&&y<N-1&&d[p+N+1]+1.414f<v)v=d[p+N+1]+1.414f;if(x>0&&y<N-1&&d[p+N-1]+1.414f<v)v=d[p+N-1]+1.414f;d[p]=v;}
  return d;}
 static int[] Label(bool[] lineD,out List<int> size){
  var lab=new int[N*N];size=new List<int>{0};var q=new int[N*N];var outside=new bool[N*N];int h=0,t=0;
  for(int i=0;i<N;i++){int[] ps={i,(N-1)*N+i,i*N,i*N+N-1};foreach(int p in ps)if(!lineD[p]&&!outside[p]){outside[p]=true;q[t++]=p;}}
  while(h<t){int p=q[h++];int x=p%N,y=p/N;
   if(x>0&&!lineD[p-1]&&!outside[p-1]){outside[p-1]=true;q[t++]=p-1;}
   if(x<N-1&&!lineD[p+1]&&!outside[p+1]){outside[p+1]=true;q[t++]=p+1;}
   if(y>0&&!lineD[p-N]&&!outside[p-N]){outside[p-N]=true;q[t++]=p-N;}
   if(y<N-1&&!lineD[p+N]&&!outside[p+N]){outside[p+N]=true;q[t++]=p+N;}}
  int id=0;
  for(int s=0;s<N*N;s++){ if(lineD[s]||outside[s]||lab[s]!=0)continue; id++;h=0;t=0;q[t++]=s;lab[s]=id;int cnt=0;
   while(h<t){int p=q[h++];int x=p%N,y=p/N;cnt++;
    if(x>0&&!lineD[p-1]&&!outside[p-1]&&lab[p-1]==0){lab[p-1]=id;q[t++]=p-1;}
    if(x<N-1&&!lineD[p+1]&&!outside[p+1]&&lab[p+1]==0){lab[p+1]=id;q[t++]=p+1;}
    if(y>0&&!lineD[p-N]&&!outside[p-N]&&lab[p-N]==0){lab[p-N]=id;q[t++]=p-N;}
    if(y<N-1&&!lineD[p+N]&&!outside[p+N]&&lab[p+N]==0){lab[p+N]=id;q[t++]=p+N;}}
   size.Add(cnt);}
  return lab;}
 static void Grow(int[] lab,int r){for(int k=0;k<r;k++){var n=(int[])lab.Clone();
  for(int y=1;y<N-1;y++)for(int x=1;x<N-1;x++){int p=y*N+x;if(lab[p]!=0)continue;int v=lab[p-1];if(v==0)v=lab[p+1];if(v==0)v=lab[p-N];if(v==0)v=lab[p+N];if(v==0)v=lab[p-N-1];if(v==0)v=lab[p-N+1];if(v==0)v=lab[p+N-1];if(v==0)v=lab[p+N+1];n[p]=v;}
  Array.Copy(n,lab,lab.Length);}}
 static void Seg(bool[] m,int x0,int y0,int x1,int y1,int r){int n=Math.Max(Math.Abs(x1-x0),Math.Abs(y1-y0));for(int i=0;i<=n;i++){int x=x0+(x1-x0)*i/n,y=y0+(y1-y0)*i/n;for(int dy=-r;dy<=r;dy++)for(int dx=-r;dx<=r;dx++){int xx=x+dx,yy=y+dy;if(xx>=0&&yy>=0&&xx<N&&yy<N)m[yy*N+xx]=true;}}}
 static L Build(string f,int r,float linie,Func<int,float> farbe,float abdeckung,int[][] zu){
  byte[] lum,al;Read(f,out lum,out al);var l=new L();
  var line=new bool[N*N];for(int i=0;i<N*N;i++)line[i]=al[i]>=90&&lum[i]<140;
  var flood=(bool[])line.Clone();if(zu!=null)foreach(var s in zu)Seg(flood,s[0],s[1],s[2],s[3],3);
  var ld=Dil(flood,r);List<int> size;var lab=Label(ld,out size);Grow(lab,r);
  var order=new List<int>();for(int i=1;i<size.Count;i++)order.Add(i);order.Sort((a,b)=>size[b].CompareTo(size[a]));
  var rank=new int[size.Count];for(int i=0;i<order.Count;i++)rank[order[i]]=i;
  if(f.Contains("koerper")){for(int i=0;i<Math.Min(order.Count,6);i++){long sx=0,sy=0;int c=0;for(int p=0;p<N*N;p++)if(lab[p]==order[i]){sx+=p%N;sy+=p/N;c++;}Dbg+="["+size[order[i]]+"@"+(sx/Math.Max(c,1))+","+(sy/Math.Max(c,1))+"]";}}var fc=new float[size.Count];for(int i=1;i<size.Count;i++)fc[i]=(size[i]<40)?-1:farbe(rank[i]);
  for(int p=0;p<N*N;p++){
   float g=0,a=0;bool fill=false;
   if(lab[p]>0&&fc[lab[p]]>=0){g=fc[lab[p]];a=1;fill=true;}
   else if(abdeckung>=0&&al[p]>0&&!(lum[p]<140)){g=abdeckung;a=1;fill=true;}
   l.shad[p]=fill&&g>=100;
   if(lum[p]<140&&al[p]>0){float la=al[p]/255f;float na=la+a*(1-la);g=(linie*la+g*a*(1-la))/na;a=na;l.line[p]=true;}
   l.g[p]=g;l.a[p]=a;l.cover[p]=a>0.5f;}
  return l;}
 static L Eigen(string f,float dunkel){
  byte[] lum,al;Read(f,out lum,out al);var l=new L();
  for(int p=0;p<N*N;p++){if(al[p]==0)continue;float v=lum[p];float g=v<70?dunkel:(float)(Math.Round(v/17.0)*17);l.g[p]=g;l.a[p]=al[p]/255f;l.cover[p]=al[p]>128;}
  return l;}
 static void Schatten(L l,float[] d,float amp,float s,float tipFaktor){
  for(int p=0;p<N*N;p++){if(!l.shad[p])continue;float k=l.g[p]>=250?tipFaktor:1f;if(d[p]>s*5)continue;l.g[p]-=amp*k*(float)Math.Exp(-d[p]/s);if(l.g[p]<110)l.g[p]=110;}}
 static void Rand(L l,float amp,float s){var d=DT(l.line);Schatten(l,d,amp,s,0.35f);}
 static void Over(float[] g,float[] a,L l,bool mirror,int axis){
  for(int y=0;y<N;y++)for(int x=0;x<N;x++){int sx=mirror?(2*axis-x):x;if(sx<0||sx>=N)continue;int s=y*N+sx;float la=l.a[s];if(la<=0)continue;int p=y*N+x;
   float oa=la+a[p]*(1-la);g[p]=(l.g[s]*la+g[p]*a[p]*(1-la))/oa;a[p]=oa;}}
 public static string Run(string kopfP,string koerperP,string krauseP,string gesichtP,string ohrP,string schwanzP,string ausgabe){
  float LIN=51,FELL=204,KOPF=221,KRAUSE=244,INN=85;
  var schwanz=Build(schwanzP,4,LIN,rk=>rk==0?FELL:(rk==1?255f:FELL),-1,null);
  var ohr=Build(ohrP,4,LIN,rk=>rk==0?FELL:(rk==1?INN:FELL),-1,null);
  var kopf=Build(kopfP,6,LIN,rk=>rk==0?KOPF:-1f,-1,null);
  var koerper=Build(koerperP,5,LIN,rk=>rk<=1?FELL:(rk==2?90f:240f),-1,new int[][]{new int[]{814,1145,1096,1134}});
  var kranz=Build(krauseP,4,LIN,rk=>-1f,KRAUSE,null);
  var gesicht=Eigen(gesichtP,34);
  int hx0=N,hx1=0,hy0=N,hy1=0;for(int p=0;p<N*N;p++)if(kopf.cover[p]){int x=p%N,y=p/N;if(x<hx0)hx0=x;if(x>hx1)hx1=x;if(y<hy0)hy0=y;if(y>hy1)hy1=y;}
  float cx=(hx0+hx1)/2f,cy=(hy0+hy1)/2f,R=(hx1-hx0)/2f;
  for(int p=0;p<N*N;p++)if(kopf.shad[p]){int x=p%N,y=p/N;float t=((x-cx)+(y-cy))/(2*R);kopf.g[p]-=16*t;}
  Rand(kopf,24,30);
  var dHead=DT(kopf.cover);var dKranz=DT(kranz.cover);var dKoerper=DT(koerper.cover);
  Rand(koerper,20,26);Schatten(koerper,dHead,28,55,1);Schatten(koerper,dKranz,26,45,1);
  Rand(ohr,20,24);Schatten(ohr,dHead,22,45,1);
  Rand(schwanz,18,30);Schatten(schwanz,dKoerper,30,75,0.2f);
  Rand(kranz,9,18);Schatten(kranz,dHead,32,55,1);
  int axis=(int)Math.Round(cx);
  var g=new float[N*N];var a=new float[N*N];
  Over(g,a,schwanz,false,0);Over(g,a,ohr,false,0);Over(g,a,ohr,true,axis);
  Over(g,a,koerper,false,0);Over(g,a,kopf,false,0);Over(g,a,kranz,false,0);Over(g,a,gesicht,false,0);
  var bmp=new Bitmap(N,N,PixelFormat.Format32bppArgb);
  var d=bmp.LockBits(new Rectangle(0,0,N,N),ImageLockMode.WriteOnly,PixelFormat.Format32bppArgb);
  var buf=new byte[N*N*4];for(int i=0;i<N*N;i++){byte v=(byte)Math.Max(0,Math.Min(255,Math.Round(g[i])));buf[i*4]=v;buf[i*4+1]=v;buf[i*4+2]=v;buf[i*4+3]=(byte)Math.Round(a[i]*255);}
  System.Runtime.InteropServices.Marshal.Copy(buf,0,d.Scan0,buf.Length);bmp.UnlockBits(d);bmp.Save(ausgabe,ImageFormat.Png);
  return "Achse "+axis+" "+Dbg;}
}
