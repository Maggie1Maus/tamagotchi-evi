using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO;

public static class ProofSheet {
  static string root;
  static string Dir(string sex){return Path.Combine(root,"teile",sex,"erwachsen");}
  static Bitmap Load(string path){return File.Exists(path)?new Bitmap(path):null;}
  static Bitmap Layer(string sex,string key){return Load(Path.Combine(Dir(sex),key.Replace('/',Path.DirectorySeparatorChar)));}
  static Bitmap Colorize(Bitmap image,string path,int ton,int kranz){
    var outp=new Bitmap(image.Width,image.Height,PixelFormat.Format32bppArgb);
    using(var g=Graphics.FromImage(outp))g.DrawImage(image,0,0);
    string mapPath=path.Replace(".png",".bereiche.png");
    using(var map=Load(mapPath))if(map!=null){
      for(int y=0;y<outp.Height;y++)for(int x=0;x<outp.Width;x++){
        Color c=outp.GetPixel(x,y);if(c.A==0)continue;int r=map.GetPixel(x,y).R,v=c.R;
        if(r==51||r==119||r==85)v+=(2-ton)*17;
        if(r==68)v-=kranz*34;
        if(r==85)v=Math.Min(v,204);
        if(v!=c.R){v=Math.Max(0,Math.Min(255,(int)Math.Round(v/17.0)*17));outp.SetPixel(x,y,Color.FromArgb(255,v,v,v));}
      }
    }
    return outp;
  }
  static void Draw(Graphics g,string sex,string key,int ton,int kranz){
    string path=Path.Combine(Dir(sex),key.Replace('/',Path.DirectorySeparatorChar));
    using(var src=Load(path))if(src!=null)using(var c=Colorize(src,path,ton,kranz))g.DrawImage(c,0,0);
  }
  static Bitmap Compose(string sex,string ears,string tail,bool mane,int ton,int kranz,string structure,string accessory,string pattern){
    using(var basis=Layer(sex,"basis.png")){
      var outp=new Bitmap(basis.Width,basis.Height,PixelFormat.Format32bppArgb);
      using(var g=Graphics.FromImage(outp)){
        string own=sex=="weiblich"?"evi":"evi-m";
        if(tail!=null)Draw(g,sex,"schwanz/"+tail+".png",ton,kranz);
        Draw(g,sex,"basis.png",ton,kranz);
        Draw(g,sex,"struktur/"+(structure??"punkte")+".png",ton,kranz);
        if(tail==null)Draw(g,sex,"schwanz/"+own+".png",ton,kranz);
        Draw(g,sex,"ohren/"+(ears??own)+".png",ton,kranz);
        if(mane)Draw(g,sex,"besonderheit/leo-vorn.png",ton,kranz);
        if(accessory!=null)Draw(g,sex,"zubehoer/"+accessory+".png",ton,kranz);
      }
      if(pattern!=null)using(var mask=Layer(sex,"muster/"+pattern+".png"))using(var region=Layer(sex,"basis.bereiche.png"))if(mask!=null&&region!=null){
        for(int y=0;y<outp.Height;y++)for(int x=0;x<outp.Width;x++)if(mask.GetPixel(x,y).A>0&&region.GetPixel(x,y).R==51){Color c=outp.GetPixel(x,y);if(c.A>0){int v=Math.Min(255,(int)Math.Round((c.R+34)/17.0)*17);outp.SetPixel(x,y,Color.FromArgb(255,v,v,v));}}
      }
      return outp;
    }
  }
  static Bitmap RegionPreview(string sex){using(var src=Layer(sex,"basis.bereiche.png")){
    var dst=new Bitmap(src.Width,src.Height,PixelFormat.Format32bppArgb);
    for(int y=0;y<src.Height;y++)for(int x=0;x<src.Width;x++){
      Color c=src.GetPixel(x,y);if(c.A==0)continue;
      Color p=c.R==17?Color.Black:c.R==34?Color.Crimson:c.R==51?Color.SteelBlue:c.R==68?Color.Gold:c.R==85?Color.MediumPurple:c.R==102?Color.LimeGreen:Color.Orange;
      dst.SetPixel(x,y,p);
    }
    return dst;
  }}
  static void Panel(Graphics g,Bitmap image,string label,int col,int row,Color back){
    int pw=390,ph=385,x=col*pw,y=row*ph;
    using(var b=new SolidBrush(back))g.FillRectangle(b,x+4,y+32,pw-8,ph-37);
    using(var f=new Font("Arial",15,FontStyle.Bold))using(var brush=new SolidBrush(Color.Black))g.DrawString(label,f,brush,x+8,y+4);
    g.InterpolationMode=InterpolationMode.NearestNeighbor;g.PixelOffsetMode=PixelOffsetMode.Half;
    int dx=x+(pw-image.Width*3)/2,dy=y+38+(ph-50-image.Height*3)/2;
    g.DrawImage(image,new Rectangle(dx,dy,image.Width*3,image.Height*3),new Rectangle(0,0,image.Width,image.Height),GraphicsUnit.Pixel);
  }
  static void Row(Graphics g,int row,string sex,bool dark){
    string title=sex=="weiblich"?"Evi":"Männliche Evi";Color bg=dark?Color.FromArgb(75,75,75):Color.White;
    using(var b=Layer(sex,"basis.png"))Panel(g,b,title+" Basis"+(dark?" dunkel":" weiß"),0,row,bg);
    using(var x=Compose(sex,null,null,false,2,0,"punkte",null,null))Panel(g,x,"Eigene Teile",1,row,bg);
    using(var x=Compose(sex,"leo","leo",true,2,0,"punkte",null,null))Panel(g,x,"Leo",2,row,bg);
    using(var x=Compose(sex,"linus","linus",false,2,0,"punkte",null,null))Panel(g,x,"Linus",3,row,bg);
    using(var x=Compose(sex,"groschen","groschen",false,2,0,"punkte",null,"ringe"))Panel(g,x,"Groschen",4,row,bg);
  }
  static string Check(string sex){
    using(var made=Compose(sex,null,null,false,2,0,"punkte",null,null))
    using(var original=Load(Path.Combine(root,"werkzeug","evi-"+sex+"-original.png"))){
      int mismatch=0,body=0,behindTail=0;
      for(int y=0;y<made.Height;y++)for(int x=0;x<made.Width;x++){
        Color a=made.GetPixel(x,y),b=original.GetPixel(x,y);
        int av=a.A==0?255:a.R,bv=b.A==0?255:b.R;
        if(av!=bv){mismatch++;if(x>made.Width*.68&&y>made.Height*.34)behindTail++;else body++;}
      }
      return sex+": eigene Teile gegen Original: "+mismatch+" abweichende Pixel (davon "+behindTail+" im Schwanzbereich, "+body+" sonst) von "+(made.Width*made.Height)+".\n";
    }
  }
  public static void Run(string directory){
    root=directory;
    using(var sheet=new Bitmap(1950,385*11,PixelFormat.Format24bppRgb))using(var g=Graphics.FromImage(sheet)){
      g.Clear(Color.FromArgb(230,230,230));
      Row(g,0,"weiblich",false);Row(g,1,"weiblich",true);Row(g,2,"maennlich",false);Row(g,3,"maennlich",true);
      for(int i=0;i<5;i++)using(var x=Compose("weiblich",null,null,false,i,0,"punkte",null,null))Panel(g,x,"Fellton "+i,i,4,Color.White);
      for(int i=0;i<4;i++)using(var x=Compose("weiblich",null,null,false,2,i,"punkte",null,null))Panel(g,x,"Kranzton "+i,i,5,Color.White);
      using(var x=RegionPreview("weiblich"))Panel(g,x,"Bereichskarte",4,5,Color.White);
      string[] structures={"punkte","glatt","gesprenkelt","gestrichelt","wuschelig"};
      for(int i=0;i<5;i++)using(var x=Compose("weiblich",null,null,false,2,0,structures[i],null,null))Panel(g,x,structures[i],i,6,Color.White);
      using(var x=Compose("weiblich",null,null,false,2,0,"punkte","schleife",null))Panel(g,x,"Schleife",0,7,Color.White);
      using(var x=Compose("weiblich",null,null,false,2,0,"punkte","gloeckchen",null))Panel(g,x,"Glöckchen",1,7,Color.White);
      using(var x=Compose("maennlich",null,null,false,2,0,"punkte","schleife",null))Panel(g,x,"Junge + Schleife",2,7,Color.White);
      using(var x=Compose("maennlich",null,null,false,2,0,"punkte","gloeckchen",null))Panel(g,x,"Junge + Glöckchen",3,7,Color.White);
      using(var x=RegionPreview("maennlich"))Panel(g,x,"Bereichskarte Junge",4,7,Color.White);
      using(var x=Compose("weiblich","leo","groschen",true,3,1,"wuschelig","schleife","flecken"))Panel(g,x,"Kind: Leo/Groschen",0,8,Color.White);
      using(var x=Compose("maennlich","linus","leo",false,1,2,"glatt","gloeckchen",null))Panel(g,x,"Kind: Linus/Leo",1,8,Color.White);
      using(var x=Compose("weiblich","groschen","linus",false,4,3,"gestrichelt",null,"maske"))Panel(g,x,"Kind: Groschen/Linus",2,8,Color.White);
      using(var x=Compose("weiblich",null,null,false,2,0,"punkte",null,null))Panel(g,x,"Originalvergleich Evi",0,9,Color.White);
      using(var x=Load(Path.Combine(root,"werkzeug","evi-weiblich-original.png")))Panel(g,x,"Original Evi",1,9,Color.White);
      using(var x=Compose("maennlich",null,null,false,2,0,"punkte",null,null))Panel(g,x,"Originalvergleich Junge",2,9,Color.White);
      using(var x=Load(Path.Combine(root,"werkzeug","evi-maennlich-original.png")))Panel(g,x,"Original Junge",3,9,Color.White);
      using(var x=Compose("weiblich","leo","leo",true,2,0,"punkte",null,null))Panel(g,x,"Leo + Mähne (Detail)",0,10,Color.FromArgb(75,75,75));
      using(var x=Compose("maennlich","groschen","groschen",false,2,0,"punkte",null,"ringe"))Panel(g,x,"Groschen (Detail)",1,10,Color.FromArgb(75,75,75));
      foreach(var pair in new[]{new[]{"leo","2"},new[]{"linus","3"},new[]{"groschen","4"}}){
        string path=Path.GetFullPath(Path.Combine(root,"..","..","neue-tamagotchis","graustufen",pair[0],"erwachsen-gluecklich.png"));
        using(var original=Load(path))if(original!=null)Panel(g,original,"Parkvorlage "+pair[0],int.Parse(pair[1]),10,Color.White);
      }
      sheet.Save(Path.Combine(root,"werkzeug","pruefbogen-erwachsen.png"),ImageFormat.Png);
    }
    File.WriteAllText(Path.Combine(root,"werkzeug","pixelvergleich.txt"),Check("weiblich")+Check("maennlich"));
  }
}
