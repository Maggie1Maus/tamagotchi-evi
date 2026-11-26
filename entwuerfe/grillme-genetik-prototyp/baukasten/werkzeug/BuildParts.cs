using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;

public static class BuildParts {
  static int Gray(Color c) { return (c.R+c.G+c.B)/3; }
  static Color Quant(Color c) { if(c.A<128||Gray(c)>247)return Color.Transparent;int v=(int)Math.Round(Gray(c)/17.0)*17;return Color.FromArgb(255,v,v,v); }
  static Bitmap Down(string path,int w,int h) {
    using(var src=new Bitmap(path)){
      var dst=new Bitmap(w,h,PixelFormat.Format32bppArgb);
      for(int y=0;y<h;y++)for(int x=0;x<w;x++){
        int sx=(int)((x+0.5)*src.Width/w),sy=(int)((y+0.5)*src.Height/h);
        int[] vals=new int[9];int n=0;
        for(int oy=-1;oy<=1;oy++)for(int ox=-1;ox<=1;ox++){
          int px=Math.Max(0,Math.Min(src.Width-1,sx+ox)),py=Math.Max(0,Math.Min(src.Height-1,sy+oy));
          vals[n++]=Gray(src.GetPixel(px,py));
        }
        Array.Sort(vals);int v=vals[4];
        dst.SetPixel(x,y,v>247?Color.Transparent:Color.FromArgb(255,Math.Min(255,(int)Math.Round(v/17.0)*17),Math.Min(255,(int)Math.Round(v/17.0)*17),Math.Min(255,(int)Math.Round(v/17.0)*17)));
      }
      return dst;
    }
  }
  static bool Ear(int x,int y,int w,int h){return y<h*.44 && (x<w*.44 || x>w*.56);}
  static bool EarPart(int x,int y,int w,int h){return y<h*.34||(y<h*.48&&(x<w*.38||x>w*.62));}
  static bool Tail(int x,int y,int w,int h){return y>h*.34 && y<h*.98 && x>w*(y<h*.68?.72:.68);}
  static bool Mane(int x,int y,int w,int h){return y>h*.13&&y<h*.83&&(x<w*.30||x>w*.70||y<h*.32||y>h*.67);}
  static bool Bow(int x,int y,int w,int h){return x>w*.53&&x<w*.86&&y>h*.58&&y<h*.77;}
  static bool Bell(int x,int y,int w,int h){return x>w*.42&&x<w*.60&&y>h*.61&&y<h*.78;}
  static bool Allowed(string kind,int x,int y,int w,int h){switch(kind){case "ohren":return EarPart(x,y,w,h);case "schwanz":return Tail(x,y,w,h);case "maehne":return Mane(x,y,w,h);case "schleife":return Bow(x,y,w,h);case "gloeckchen":return Bell(x,y,w,h);default:return false;}}
  static Bitmap Blank(int w,int h){return new Bitmap(w,h,PixelFormat.Format32bppArgb);}
  static void Save(Bitmap b,string path){Directory.CreateDirectory(Path.GetDirectoryName(path));b.Save(path,ImageFormat.Png);}
  static bool Equal(Color a,Color b){return a.A==b.A&&(a.A==0||a.R==b.R);}
  static int Region(Color c,int x,int y,int w,int h,string kind){
    if(c.A==0)return 0;int v=Gray(c);
    if(kind=="ohren"&&v<105&&x>w*.12&&x<w*.88&&y<h*.36)return 85;
    if(v<=51){if(kind=="basis"&&x>w*.29&&x<w*.72&&y>h*.3&&y<h*.68)return 34;return 17;}
    if(kind=="basis"){
      if(y>h*.87&&v>=204)return 102;
      if(y>h*.64&&y<h*.89&&v>=204)return 68;
      if(x>w*.28&&x<w*.72&&y>h*.30&&y<h*.68&&v>=221)return 34;
    }
    if(kind=="schwanz"&&v>=221)return 68;
    if(kind=="basis"&&v>=221&&y>h*.6)return 68;
    if(kind=="struktur")return 119;
    return 51;
  }
  static void SaveMap(Bitmap src,string path,string kind){using(var map=Blank(src.Width,src.Height)){for(int y=0;y<src.Height;y++)for(int x=0;x<src.Width;x++){int r=Region(src.GetPixel(x,y),x,y,src.Width,src.Height,kind);if(r>0)map.SetPixel(x,y,Color.FromArgb(255,r,r,r));}Save(map,path);}}
  static bool Fur(Bitmap b,int x,int y){return x>=0&&y>=0&&x<b.Width&&y<b.Height&&Region(b.GetPixel(x,y),x,y,b.Width,b.Height,"basis")==51;}
  static void Textures(Bitmap basis,string dir){
    int w=basis.Width,h=basis.Height;string sd=Path.Combine(dir,"struktur");
    using(var points=Blank(w,h))using(var smooth=Blank(w,h)){
      for(int y=0;y<h;y++)for(int x=0;x<w;x++)smooth.SetPixel(x,y,basis.GetPixel(x,y));
      for(int y=2;y<h-2;y++)for(int x=2;x<w-2;x++)if(Fur(basis,x,y)){
        Color c=basis.GetPixel(x,y);int[] near=new int[8];int n=0;
        foreach(int dy in new[]{-2,0,2})foreach(int dx in new[]{-2,0,2}){if(dx==0&&dy==0)continue;Color q=basis.GetPixel(x+dx,y+dy);if(q.A>0&&Gray(q)>80)near[n++]=Gray(q);}
        if(n<5)continue;Array.Sort(near,0,n);int med=near[n/2];
        if(med-Gray(c)>=20&&Gray(c)>90){points.SetPixel(x,y,c);int v=Math.Min(255,(int)Math.Round(med/17.0)*17);smooth.SetPixel(x,y,Color.FromArgb(255,v,v,v));}
      }
      for(int y=0;y<h;y++)for(int x=0;x<w;x++)basis.SetPixel(x,y,smooth.GetPixel(x,y));
      Save(points,Path.Combine(sd,"punkte.png"));SaveMap(points,Path.Combine(sd,"punkte.bereiche.png"),"struktur");
      using(var empty=Blank(w,h)){Save(empty,Path.Combine(sd,"glatt.png"));SaveMap(empty,Path.Combine(sd,"glatt.bereiche.png"),"struktur");}
      foreach(string type in new[]{"gesprenkelt","gestrichelt","wuschelig"})using(var tex=Blank(w,h)){
        for(int y=0;y<h;y++)for(int x=0;x<w;x++)if(Fur(basis,x,y)){
          int v=Gray(basis.GetPixel(x,y));bool mark=false;
          if(type=="gesprenkelt")mark=((x*17+y*11)%37==0)||((x*13+y*7)%43==1);
          if(type=="gestrichelt")mark=(y%9==3&&x%11<2);
          if(type=="wuschelig")mark=((x+y)%7==0)&&(!Fur(basis,x-1,y)||!Fur(basis,x+1,y)||!Fur(basis,x,y-1));
          if(mark){int nv=Math.Max(0,Math.Min(255,(int)Math.Round((v+(type=="wuschelig"?34:-34))/17.0)*17));tex.SetPixel(x,y,Color.FromArgb(255,nv,nv,nv));}
        }
        Save(tex,Path.Combine(sd,type+".png"));SaveMap(tex,Path.Combine(sd,type+".bereiche.png"),"struktur");
      }
    }
  }
  static void Patterns(Bitmap basis,string dir){
    int w=basis.Width,h=basis.Height;string folder=Path.Combine(dir,"muster");
    foreach(string type in new[]{"flecken","ringe","helle-brust","maske","punkte"})using(var mask=Blank(w,h)){
      for(int y=0;y<h;y++)for(int x=0;x<w;x++)if(Fur(basis,x,y)){
        bool mark=false;
        if(type=="flecken")mark=((x/7+y/9)%5==0)&&((x%7-3)*(x%7-3)+(y%9-4)*(y%9-4)<12);
        if(type=="ringe")mark=y>h*.68&&((x+y/3)%12<3);
        if(type=="helle-brust")mark=y>h*.72&&x>w*.38&&x<w*.62;
        if(type=="maske")mark=y>h*.35&&y<h*.55&&((x>w*.2&&x<w*.43)||(x>w*.57&&x<w*.8));
        if(type=="punkte")mark=(x*13+y*17)%47==0;
        if(mark)mask.SetPixel(x,y,Color.FromArgb(255,255,255,255));
      }
      Save(mask,Path.Combine(folder,type+".png"));
      SaveMap(mask,Path.Combine(folder,type+".bereiche.png"),"muster");
    }
  }
  static void BuildSex(string root,string sex){
    string tool=Path.Combine(root,"werkzeug"),dir=Path.Combine(root,"teile",sex,"erwachsen"),ki=Path.Combine(tool,"ki");
    using(var orig=new Bitmap(Path.Combine(tool,"evi-"+sex+"-original.png")))
    using(var ai=Down(Path.Combine(tool,"basis-"+sex+"-ki.png"),orig.Width,orig.Height))
    using(var basis=Blank(orig.Width,orig.Height))
    using(var ownEars=Blank(orig.Width,orig.Height))
    using(var ownTail=Blank(orig.Width,orig.Height)){
      int w=orig.Width,h=orig.Height;
      for(int y=0;y<h;y++)for(int x=0;x<w;x++){
        Color o=orig.GetPixel(x,y),a=ai.GetPixel(x,y);
        bool e=Ear(x,y,w,h),t=Tail(x,y,w,h);
        Color b=(e||t)?a:o;
        // Keep the original silhouette: own layers can restore each changed pixel exactly.
        if(o.A==0)b=Color.Transparent;
        basis.SetPixel(x,y,b);
        if(!Equal(o,b)&&o.A>0){if(e)ownEars.SetPixel(x,y,o);else if(t)ownTail.SetPixel(x,y,o);}
      }
      Textures(basis,dir);
      Patterns(basis,dir);
      Save(basis,Path.Combine(dir,"basis.png"));SaveMap(basis,Path.Combine(dir,"basis.bereiche.png"),"basis");
      string own=sex=="weiblich"?"evi":"evi-m";
      Save(ownEars,Path.Combine(dir,"ohren",own+".png"));SaveMap(ownEars,Path.Combine(dir,"ohren",own+".bereiche.png"),"ohren");
      Save(ownTail,Path.Combine(dir,"schwanz",own+".png"));SaveMap(ownTail,Path.Combine(dir,"schwanz",own+".bereiche.png"),"schwanz");
      foreach(string fig in new[]{"leo","linus","groschen"})foreach(string kind in new[]{"ohren","schwanz"}){
        using(var variant=Down(Path.Combine(ki,sex+"-"+kind+"-"+fig+".png"),w,h))using(var part=Blank(w,h)){
          for(int y=0;y<h;y++)for(int x=0;x<w;x++)if(Allowed(kind,x,y,w,h)){
            Color v=variant.GetPixel(x,y),a=ai.GetPixel(x,y);if(v.A>0&&(a.A==0||Math.Abs(v.R-a.R)>=34))part.SetPixel(x,y,v);
          }
          string p=Path.Combine(dir,kind,fig+".png");Save(part,p);SaveMap(part,Path.Combine(dir,kind,fig+".bereiche.png"),kind);
        }
      }
      foreach(string kind in new[]{"maehne","schleife","gloeckchen"}){
        string source=kind=="maehne"?sex+"-maehne-leo":sex+"-zubehoer-"+kind;
        string folder=kind=="maehne"?"besonderheit":"zubehoer";
        string filename=kind=="maehne"?"leo-vorn":kind;
        using(var variant=Down(Path.Combine(ki,source+".png"),w,h))using(var part=Blank(w,h)){
          for(int y=0;y<h;y++)for(int x=0;x<w;x++)if(Allowed(kind,x,y,w,h)){
            Color v=variant.GetPixel(x,y),a=ai.GetPixel(x,y);if(v.A>0&&(a.A==0||Math.Abs(v.R-a.R)>=34)){
              if(kind!="maehne"||a.A==0||x<w*.22||x>w*.78||y>h*.67)part.SetPixel(x,y,v);
            }
          }
          Save(part,Path.Combine(dir,folder,filename+".png"));SaveMap(part,Path.Combine(dir,folder,filename+".bereiche.png"),folder);
        }
      }
    }
  }
  public static void Run(string root){BuildSex(root,"weiblich");BuildSex(root,"maennlich");}
}
