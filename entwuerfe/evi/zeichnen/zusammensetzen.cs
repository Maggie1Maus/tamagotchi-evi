using System;using System.Collections.Generic;using System.Drawing;using System.Drawing.Imaging;using System.IO;
public static class Zus{
 const int N=2048;
 class L{public float[] g=new float[N*N];public float[] a=new float[N*N];}
 static void Read(string f,out byte[] lum,out byte[] al){
  var b=new Bitmap(f);var d=b.LockBits(new Rectangle(0,0,N,N),ImageLockMode.ReadOnly,PixelFormat.Format32bppArgb);
  var buf=new byte[N*N*4];System.Runtime.InteropServices.Marshal.Copy(d.Scan0,buf,0,buf.Length);b.UnlockBits(d);
  lum=new byte[N*N];al=new byte[N*N];
  for(int i=0;i<N*N;i++){al[i]=buf[i*4+3];lum[i]=(byte)((buf[i*4]+buf[i*4+1]+buf[i*4+2])/3);} }
 static bool[] Dil(bool[] m,int r){ // quadratisch, getrennt
  var t=new bool[N*N];var o=new bool[N*N];
  for(int y=0;y<N;y++){for(int x=0;x<N;x++){
    // einfache Variante
    bool v=false;int x0=Math.Max(0,x-r),x1=Math.Min(N-1,x+r);for(int k=x0;k<=x1;k++)if(m[y*N+k]){v=true;break;}t[y*N+x]=v;}}
  for(int x=0;x<N;x++)for(int y=0;y<N;y++){bool v=false;int y0=Math.Max(0,y-r),y1=Math.Min(N-1,y+r);for(int k=y0;k<=y1;k++)if(t[k*N+x]){v=true;break;}o[y*N+x]=v;}
  return o;}
 // Ergebnis: Label je Pixel (0 aussen/Linie, >=1 innen) ; Groessen und Schwerpunkt
 static int[] Label(bool[] lineD,out List<int> size,out List<double> cy){
  var lab=new int[N*N];size=new List<int>{0};cy=new List<double>{0};
  var q=new int[N*N];
  // aussen = Label -1 via Flut vom Rand
  var outside=new bool[N*N];int h=0,t=0;
  for(int i=0;i<N;i++){int[] ps={i,(N-1)*N+i,i*N,i*N+N-1};foreach(int p in ps)if(!lineD[p]&&!outside[p]){outside[p]=true;q[t++]=p;}}
  while(h<t){int p=q[h++];int x=p%N,y=p/N;
   if(x>0&&!lineD[p-1]&&!outside[p-1]){outside[p-1]=true;q[t++]=p-1;}
   if(x<N-1&&!lineD[p+1]&&!outside[p+1]){outside[p+1]=true;q[t++]=p+1;}
   if(y>0&&!lineD[p-N]&&!outside[p-N]){outside[p-N]=true;q[t++]=p-N;}
   if(y<N-1&&!lineD[p+N]&&!outside[p+N]){outside[p+N]=true;q[t++]=p+N;}}
  int id=0;
  for(int s=0;s<N*N;s++){ if(lineD[s]||outside[s]||lab[s]!=0)continue; id++;h=0;t=0;q[t++]=s;lab[s]=id;long sy=0;int cnt=0;
   while(h<t){int p=q[h++];int x=p%N,y=p/N;cnt++;sy+=y;
    if(x>0&&!lineD[p-1]&&!outside[p-1]&&lab[p-1]==0){lab[p-1]=id;q[t++]=p-1;}
    if(x<N-1&&!lineD[p+1]&&!outside[p+1]&&lab[p+1]==0){lab[p+1]=id;q[t++]=p+1;}
    if(y>0&&!lineD[p-N]&&!outside[p-N]&&lab[p-N]==0){lab[p-N]=id;q[t++]=p-N;}
    if(y<N-1&&!lineD[p+N]&&!outside[p+N]&&lab[p+N]==0){lab[p+N]=id;q[t++]=p+N;}}
   size.Add(cnt);cy.Add((double)sy/cnt);}
  return lab;}
 static void Grow(int[] lab,int r){for(int k=0;k<r;k++){var n=(int[])lab.Clone();
  for(int y=1;y<N-1;y++)for(int x=1;x<N-1;x++){int p=y*N+x;if(lab[p]!=0)continue;int v=lab[p-1];if(v==0)v=lab[p+1];if(v==0)v=lab[p-N];if(v==0)v=lab[p+N];if(v==0)v=lab[p-N-1];if(v==0)v=lab[p-N+1];if(v==0)v=lab[p+N-1];if(v==0)v=lab[p+N+1];n[p]=v;}
  Array.Copy(n,lab,lab.Length);}}
 // Ebene bauen. farbe(id,size,cy,rangNachGroesse) liefert Fuellgrau (<0 = nicht fuellen)
 static L Build(string f,int r,float linie,Func<int,int,double,int,float> farbe,float abdeckung,float weissFarbe){
  byte[] lum,al;Read(f,out lum,out al);var l=new L();
  var line=new bool[N*N];for(int i=0;i<N*N;i++)line[i]=al[i]>=90&&lum[i]<140;
  var ld=Dil(line,r);List<int> size;List<double> cy;var lab=Label(ld,out size,out cy);
  Grow(lab,r);
  // Rang nach Groesse
  var order=new List<int>();for(int i=1;i<size.Count;i++)order.Add(i);order.Sort((a,b)=>size[b].CompareTo(size[a]));
  var rank=new int[size.Count];for(int i=0;i<order.Count;i++)rank[order[i]]=i;
  var fc=new float[size.Count];for(int i=1;i<size.Count;i++)fc[i]=(size[i]<40)?-1:farbe(i,size[i],cy[i],rank[i]);
  for(int p=0;p<N*N;p++){
   float g=0,a=0;
   if(lab[p]>0&&fc[lab[p]]>=0){g=fc[lab[p]];a=1;}
   else if(abdeckung>=0&&al[p]>0&&!(lum[p]<140)){g=abdeckung;a=1;}
   // weisse Eigenfarbe (Glanzlichter) behalten
   if(weissFarbe>=0&&al[p]>200&&lum[p]>=235){g=weissFarbe;a=1;}
   if(lum[p]<140&&al[p]>0){float la=al[p]/255f;float na=la+a*(1-la);g=(linie*la+g*a*(1-la))/na;a=na;}
   l.g[p]=g;l.a[p]=a;}
  return l;}
 static void Over(float[] g,float[] a,L l,bool mirror,int axis){
  for(int y=0;y<N;y++)for(int x=0;x<N;x++){int sx=mirror?(2*axis-x):x;if(sx<0||sx>=N)continue;int s=y*N+sx;float la=l.a[s];if(la<=0)continue;int p=y*N+x;
   float oa=la+a[p]*(1-la);g[p]=(l.g[s]*la+g[p]*a[p]*(1-la))/oa;a[p]=oa;}}
 public static string Run(string dir,string ausgabe){
  Func<string,string> P=n=>Path.Combine(dir,n);
  var g=new float[N*N];var a=new float[N*N];
  float LIN=51,FELL=204,KOPF=221,KRAUSE=238,INN=85;
  var schwanz=Build(P("Evi-neue-vorlage-schwanz.png"),4,LIN,(id,sz,cy,rk)=>rk==0?FELL:(rk==1?255f:FELL),-1,-1);
  var ohr=Build(P("Evi-neue-vorlage-ohr.png"),4,LIN,(id,sz,cy,rk)=>rk==0?FELL:(rk==1?INN:FELL),-1,-1);
  var rumpf=Build(P("Evi-neue-blankovorlage.png"),6,LIN,(id,sz,cy,rk)=>rk==0?KOPF:FELL,-1,-1);
  var krause=Build(P("Evi-neue-vorlage-krause.png"),4,LIN,(id,sz,cy,rk)=>-1f,KRAUSE,-1);
  var gesicht=Build(P("Evi-neue-vorlage-gesicht.png"),3,LIN,(id,sz,cy,rk)=>cy>1080?INN:LIN,-1,255);
  var haare=Build(P("Evi-neue-vorlage-kopfhaare.png"),3,LIN,(id,sz,cy,rk)=>KOPF,KOPF,-1);
  // Spiegelachse: Kopfmitte aus Rumpf (obere Haelfte)
  int minx=N,maxx=0;for(int y=700;y<1150;y++)for(int x=0;x<N;x++)if(rumpf.a[y*N+x]>0.5){if(x<minx)minx=x;if(x>maxx)maxx=x;}
  int axis=(minx+maxx)/2;
  Over(g,a,schwanz,false,0);Over(g,a,ohr,false,0);Over(g,a,ohr,true,axis);
  Over(g,a,rumpf,false,0);Over(g,a,krause,false,0);Over(g,a,gesicht,false,0);Over(g,a,haare,false,0);
  var bmp=new Bitmap(N,N,PixelFormat.Format32bppArgb);
  var d=bmp.LockBits(new Rectangle(0,0,N,N),ImageLockMode.WriteOnly,PixelFormat.Format32bppArgb);
  var buf=new byte[N*N*4];for(int i=0;i<N*N;i++){byte v=(byte)Math.Max(0,Math.Min(255,Math.Round(g[i])));buf[i*4]=v;buf[i*4+1]=v;buf[i*4+2]=v;buf[i*4+3]=(byte)Math.Round(a[i]*255);}
  System.Runtime.InteropServices.Marshal.Copy(buf,0,d.Scan0,buf.Length);bmp.UnlockBits(d);bmp.Save(ausgabe,ImageFormat.Png);
  return "Achse "+axis+" Kopf "+minx+".."+maxx;}
}
