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
  static bool Mane(int x,int y,int w,int h){return y>h*.12&&y<h*.72&&(x<w*.37||x>w*.63||y<h*.30||y>h*.60);}
  static bool Bow(int x,int y,int w,int h){return x>w*.52&&x<w*.85&&y>h*.23&&y<h*.47;}
  static bool Bell(int x,int y,int w,int h){return x>w*.38&&x<w*.62&&y>h*.59&&y<h*.78;}
  static bool Allowed(string kind,int x,int y,int w,int h){switch(kind){case "ohren":return EarPart(x,y,w,h);case "schwanz":return Tail(x,y,w,h);case "maehne":return Mane(x,y,w,h);case "schleife":return Bow(x,y,w,h);case "gloeckchen":return Bell(x,y,w,h);default:return false;}}
  static Bitmap Blank(int w,int h){return new Bitmap(w,h,PixelFormat.Format32bppArgb);}
  static void Save(Bitmap b,string path){Directory.CreateDirectory(Path.GetDirectoryName(path));b.Save(path,ImageFormat.Png);}
  static bool Equal(Color a,Color b){return a.A==b.A&&(a.A==0||a.R==b.R);}
  static int Region(Color c,int x,int y,int w,int h,string kind){
    if(c.A==0)return 0;int v=Gray(c);
    if(kind=="ohren"&&v>=52&&v<125&&y<h*.49)return 85;
    if(v<=70)return 17;
    if(kind=="schwanz"&&v>=220&&y<h*.72)return 68;
    if(kind=="struktur")return 119;
    return 51;
  }
  static void SaveMap(Bitmap src,string path,string kind){using(var map=Blank(src.Width,src.Height)){for(int y=0;y<src.Height;y++)for(int x=0;x<src.Width;x++){int r=Region(src.GetPixel(x,y),x,y,src.Width,src.Height,kind);if(r>0)map.SetPixel(x,y,Color.FromArgb(255,r,r,r));}Save(map,path);}}
  static bool[,] FloodLight(Bitmap src,int sx,int sy){
    int w=src.Width,h=src.Height;var seen=new bool[w,h];var qx=new int[w*h];var qy=new int[w*h];int first=0,last=0;
    if(sx<0||sy<0||sx>=w||sy>=h||src.GetPixel(sx,sy).A==0||Gray(src.GetPixel(sx,sy))<125)return seen;
    seen[sx,sy]=true;qx[last]=sx;qy[last++]=sy;
    while(first<last){int px=qx[first],py=qy[first++];foreach(Point n in new[]{new Point(px-1,py),new Point(px+1,py),new Point(px,py-1),new Point(px,py+1)}){
      if(n.X<0||n.Y<0||n.X>=w||n.Y>=h||seen[n.X,n.Y])continue;Color c=src.GetPixel(n.X,n.Y);
      if(c.A>0&&Gray(c)>=125){seen[n.X,n.Y]=true;qx[last]=n.X;qy[last++]=n.Y;}
    }}return seen;
  }
  static Bitmap BaseMap(Bitmap src,string sex,bool[,] tip){
    int w=src.Width,h=src.Height;bool female=sex=="weiblich";
    bool[,] ruff=FloodLight(src,female?50:60,female?80:74);
    var map=Blank(w,h);
    for(int y=0;y<h;y++)for(int x=0;x<w;x++){
      Color c=src.GetPixel(x,y);if(c.A==0)continue;int v=Gray(c),r;
      bool inRuff=ruff[x,y]&&(female?y>=72&&y<=96:(y>=68||(y>=56&&(x<31||x>88)))&&y<=90);
      if(inRuff||tip[x,y])r=68;
      else if(y>h*.88&&v>=190)r=102;
      else if(v<=85){
        bool face=female?(x>24&&x<73&&y>43&&y<68):(x>32&&x<83&&y>42&&y<64);
        r=face?34:17;
      }else if((female?x>42&&x<69&&y>57&&y<69:x>48&&x<73&&y>53&&y<67)&&v>215)r=34;
      else r=51;
      map.SetPixel(x,y,Color.FromArgb(255,r,r,r));
    }return map;
  }
  static bool Fur(Bitmap map,int x,int y){return x>=0&&y>=0&&x<map.Width&&y<map.Height&&map.GetPixel(x,y).R==51&&map.GetPixel(x,y).A>0;}
  static void Textures(Bitmap basis,Bitmap map,string dir){
    int w=basis.Width,h=basis.Height;string sd=Path.Combine(dir,"struktur");
    using(var points=Blank(w,h))using(var smooth=Blank(w,h)){
      for(int y=0;y<h;y++)for(int x=0;x<w;x++)smooth.SetPixel(x,y,basis.GetPixel(x,y));
      for(int y=2;y<h-2;y++)for(int x=2;x<w-2;x++)if(Fur(map,x,y)){
        Color c=basis.GetPixel(x,y);int[] near=new int[8];int n=0;
        foreach(int dy in new[]{-2,0,2})foreach(int dx in new[]{-2,0,2}){if(dx==0&&dy==0)continue;Color q=basis.GetPixel(x+dx,y+dy);if(q.A>0&&Gray(q)>80)near[n++]=Gray(q);}
        if(n<5)continue;Array.Sort(near,0,n);int med=near[n/2];
        if(med-Gray(c)>=20&&Gray(c)>90){points.SetPixel(x,y,c);int v=Math.Min(255,(int)Math.Round(med/17.0)*17);smooth.SetPixel(x,y,Color.FromArgb(255,v,v,v));}
      }
      for(int y=0;y<h;y++)for(int x=0;x<w;x++)basis.SetPixel(x,y,smooth.GetPixel(x,y));
      Save(points,Path.Combine(sd,"punkte.png"));SaveMap(points,Path.Combine(sd,"punkte.bereiche.png"),"struktur");
      using(var empty=Blank(w,h)){Save(empty,Path.Combine(sd,"glatt.png"));SaveMap(empty,Path.Combine(sd,"glatt.bereiche.png"),"struktur");}
      foreach(string type in new[]{"gesprenkelt","gestrichelt","wuschelig"})using(var tex=Blank(w,h)){
        for(int y=0;y<h;y++)for(int x=0;x<w;x++)if(Fur(map,x,y)&&(y<44||y>70)){
          int v=Gray(basis.GetPixel(x,y));bool mark=false;
          if(type=="gesprenkelt")mark=(x%7<3&&y%6<3)||((x+4)%13<2&&(y+3)%11<2);
          if(type=="gestrichelt")mark=y%7<2&&x%10<6;
          bool edge=!Fur(map,x-1,y)||!Fur(map,x+1,y)||!Fur(map,x,y-1)||!Fur(map,x,y+1);
          if(type=="wuschelig")mark=(edge&&(x+y)%6<4)||(y%8<2&&(x+y)%12<4);
          if(mark){int delta=type=="wuschelig"?(edge?-68:68):type=="gestrichelt"?-102:-85;int nv=Math.Max(0,Math.Min(255,(int)Math.Round((v+delta)/17.0)*17));tex.SetPixel(x,y,Color.FromArgb(255,nv,nv,nv));}
        }
        Save(tex,Path.Combine(sd,type+".png"));SaveMap(tex,Path.Combine(sd,type+".bereiche.png"),"struktur");
      }
    }
  }
  static void Patterns(Bitmap basis,Bitmap map,string sex,string dir){
    int w=basis.Width,h=basis.Height;string folder=Path.Combine(dir,"muster");
    foreach(string type in new[]{"flecken","ringe","helle-brust","maske","punkte"})using(var mask=Blank(w,h)){
      bool female=sex=="weiblich";double cx=w*.50,cy=female?103:96;
      for(int y=0;y<h;y++)for(int x=0;x<w;x++)if(Fur(map,x,y)&&y>(female?93:86)){
        bool mark=false;
        double dx=x-cx,dy=y-cy;
        if(type=="flecken")mark=Ellipse(x,y,w*.29,cy-3,5.4,4.0)||Ellipse(x,y,w*.70,cy+1,6.2,4.5)||Ellipse(x,y,w*.52,cy-5,3.6,2.8);
        if(type=="ringe")mark=(EllipseRing(x,y,w*.27,cy,8,6)||EllipseRing(x,y,w*.75,cy,8,6))&&Math.Abs(dx)>w*.15;
        if(type=="helle-brust")mark=Ellipse(x,y,cx,cy-1,w*.18,9);
        if(type=="maske")mark=Ellipse(x,y,w*.29,cy-3,w*.13,7)||Ellipse(x,y,w*.71,cy-3,w*.13,7);
        if(type=="punkte")mark=((x%8<2)&&(y%7<2));
        if(mark)mask.SetPixel(x,y,Color.FromArgb(255,255,255,255));
      }
      Save(mask,Path.Combine(folder,type+".png"));
      SaveMap(mask,Path.Combine(folder,type+".bereiche.png"),"muster");
    }
  }
  static bool Ellipse(int x,int y,double cx,double cy,double rx,double ry){double a=(x-cx)/rx,b=(y-cy)/ry;return a*a+b*b<=1;}
  static bool EllipseRing(int x,int y,double cx,double cy,double rx,double ry){double a=(x-cx)/rx,b=(y-cy)/ry,t=a*a+b*b;return t<=1&&t>=.45;}
  static void Stamp(Bitmap dst,int left,int top,string[] rows){
    for(int y=0;y<rows.Length;y++)for(int x=0;x<rows[y].Length;x++){
      int v=rows[y][x]=='#'?34:rows[y][x]=='m'?153:rows[y][x]=='l'?221:rows[y][x]=='w'?255:-1;
      if(v>=0&&left+x>=0&&top+y>=0&&left+x<dst.Width&&top+y<dst.Height)dst.SetPixel(left+x,top+y,Color.FromArgb(255,v,v,v));
    }
  }
  static void CleanAccessory(Bitmap part,string sex,string kind){
    using(var g=Graphics.FromImage(part))g.Clear(Color.Transparent);
    if(kind=="schleife")Stamp(part,sex=="weiblich"?65:79,sex=="weiblich"?31:32,new[]{
      "...##.....##...", "..#ll#...#ll#..", ".#llll#.#llll#.", "#llllll#llllll#", "#llllmm#mmllll#",
      ".#llll###llll#.", "..#ll#mmm#ll#..", "...##.#m#.##...", "......#m#......", ".....#mm#......", ".....#m#......."
    });
    if(kind=="gloeckchen")Stamp(part,sex=="weiblich"?50:54,sex=="weiblich"?75:72,new[]{
      ".....###.....", ".....#l#.....", "....#lll#....", "...#lllll#...", "..#lllwlll#..", ".#lllllllll#.",
      ".#lllllllll#.", "#lllllllllll#", "#############", ".#mmmmmmmmm#.", "..##mmmmm##..", ".....###.....", "......#......"
    });
  }
  static void SaveTailMap(Bitmap tail,bool[,] tip,string path){using(var map=Blank(tail.Width,tail.Height)){
    for(int y=0;y<tail.Height;y++)for(int x=0;x<tail.Width;x++){
      Color c=tail.GetPixel(x,y);if(c.A==0)continue;int r=tip[x,y]?(Gray(c)<=70?17:68):Region(c,x,y,tail.Width,tail.Height,"schwanz");
      map.SetPixel(x,y,Color.FromArgb(255,r,r,r));
    }Save(map,path);
  }}
  static void RemoveSpecks(Bitmap image,int minimum){
    int w=image.Width,h=image.Height;var visited=new bool[w,h];var qx=new int[w*h];var qy=new int[w*h];
    for(int sy=0;sy<h;sy++)for(int sx=0;sx<w;sx++)if(!visited[sx,sy]&&image.GetPixel(sx,sy).A>0){
      int first=0,last=0;visited[sx,sy]=true;qx[last]=sx;qy[last++]=sy;
      while(first<last){int x=qx[first],y=qy[first++];for(int dy=-1;dy<=1;dy++)for(int dx=-1;dx<=1;dx++){
        int nx=x+dx,ny=y+dy;if(nx<0||ny<0||nx>=w||ny>=h||visited[nx,ny]||image.GetPixel(nx,ny).A==0)continue;
        visited[nx,ny]=true;qx[last]=nx;qy[last++]=ny;
      }}
      if(last<minimum)for(int i=0;i<last;i++)image.SetPixel(qx[i],qy[i],Color.Transparent);
    }
  }
  static void AddMaleManeChin(Bitmap part,Bitmap original){
    for(int x=27;x<=93;x++){
      int top=(int)Math.Round(60+10*(1-Math.Abs(x-60)/33.0));
      int bottom=top+2+((x%11<3)?2:0);
      for(int y=top;y<=bottom;y++){
        Color under=original.GetPixel(x,y);if(under.A==0||Gray(under)<205)continue;
        int v=y==top?170:y==bottom&&x%11<3?102:153;part.SetPixel(x,y,Color.FromArgb(255,v,v,v));
      }
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
      bool[,] tip=FloodLight(orig,sex=="weiblich"?95:107,sex=="weiblich"?53:57);
      using(var region=BaseMap(basis,sex,tip)){
        Textures(basis,region,dir);
        Patterns(basis,region,sex,dir);
        Save(basis,Path.Combine(dir,"basis.png"));Save(region,Path.Combine(dir,"basis.bereiche.png"));
      }
      string own=sex=="weiblich"?"evi":"evi-m";
      Save(ownEars,Path.Combine(dir,"ohren",own+".png"));SaveMap(ownEars,Path.Combine(dir,"ohren",own+".bereiche.png"),"ohren");
      Save(ownTail,Path.Combine(dir,"schwanz",own+".png"));SaveTailMap(ownTail,tip,Path.Combine(dir,"schwanz",own+".bereiche.png"));
      foreach(string fig in new[]{"leo","linus","groschen"})foreach(string kind in new[]{"ohren","schwanz"}){
        using(var variant=Down(Path.Combine(ki,sex+"-"+kind+"-"+fig+".png"),w,h))using(var part=Blank(w,h)){
          for(int y=0;y<h;y++)for(int x=0;x<w;x++)if(Allowed(kind,x,y,w,h)){
            Color v=variant.GetPixel(x,y),a=ai.GetPixel(x,y);if(v.A>0&&(a.A==0||Math.Abs(v.R-a.R)>=34))part.SetPixel(x,y,v);
          }
          string p=Path.Combine(dir,kind,fig+".png");Save(part,p);SaveMap(part,Path.Combine(dir,kind,fig+".bereiche.png"),kind);
        }
      }
      foreach(string kind in new[]{"maehne","schleife","gloeckchen"}){
        string source=kind=="maehne"?sex+"-maehne-leo-v2":sex+"-zubehoer-"+kind+"-v2";
        string folder=kind=="maehne"?"besonderheit":"zubehoer";
        string filename=kind=="maehne"?"leo-vorn":kind;
        using(var variant=Down(Path.Combine(ki,source+".png"),w,h))using(var part=Blank(w,h)){
          for(int y=0;y<h;y++)for(int x=0;x<w;x++)if(Allowed(kind,x,y,w,h)){
            if(kind=="schleife"&&!(sex=="weiblich"?x>=62&&x<=83&&y>=30&&y<=49:x>=76&&x<=98&&y>=32&&y<=52))continue;
            if(kind=="gloeckchen"&&!(sex=="weiblich"?x>=43&&x<=67&&y>=70&&y<=89:x>=48&&x<=72&&y>=65&&y<=89))continue;
            Color v=variant.GetPixel(x,y),a=kind=="maehne"?ai.GetPixel(x,y):orig.GetPixel(x,y);
            if(v.A>0&&v.R<235&&(a.A==0||Math.Abs(v.R-a.R)>=34)){
              if(kind!="maehne"||a.A==0||(y>h*.60&&y<h*.72&&x>w*.30&&x<w*.70))part.SetPixel(x,y,v);
            }
          }
          if(kind!="maehne")CleanAccessory(part,sex,kind);
          else {RemoveSpecks(part,8);if(sex=="maennlich")AddMaleManeChin(part,orig);}
          Save(part,Path.Combine(dir,folder,filename+".png"));SaveMap(part,Path.Combine(dir,folder,filename+".bereiche.png"),folder);
        }
      }
    }
  }
  public static void Run(string root){BuildSex(root,"weiblich");BuildSex(root,"maennlich");}
}
