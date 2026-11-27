// Geschätzte Merkmale sind mit ~ markiert. Alle Einträge können hier geändert werden.
window.FIGUREN = [
  ['evi','Evi','weiblich','Evi-Art','evi','evi','keins',2,'keins','keins'],
  ['evi-m','Männliche Evi','maennlich','Evi-Art','evi-m','evi-m','keins',2,'keins','keins'],
  ['lotti','Lotti','weiblich','Rehkitz','lotti','lotti','keins',2,'kitz-tupfen','keins'],
  ['ella','Ella','weiblich','Elefantenbaby','ella','ella','ruessel',2,'keins','keins'],
  ['gloeckchen','Glöckchen','weiblich','Hündin','gloeckchen','gloeckchen','keins',2,'keins','gloeckchen'],
  ['kiwi','Kiwi','weiblich','Dino','kiwi','kiwi','keins',2,'punkte','keins'],
  ['klecksi','Klecksi','weiblich','Dalmatinerwelpe','klecksi','klecksi','keins',1,'flecken','halsband'],
  ['lumi','Lumi','weiblich','Glühwürmchen-Fee','lumi','lumi','fluegel',1,'keins','keins'],
  ['mia','Mia','weiblich','Kätzchen','mia','mia','keins',2,'helle-brust','schleife'],
  ['mimi','Mimi','weiblich','Schnecke','keins','keins','fuehler',2,'keins','keins'],
  ['moosi','Moosi','weiblich','Waldgeist','moosi','moosi','blaetter',3,'keins','keins'],
  ['spuki','Spuki','weiblich','Geisterkätzchen','keins','keins','keins',0,'keins','keins'],
  ['bit','Bit','maennlich','Roboterhund','bit','bit','antennen',3,'keins','keins'],
  ['blip','Blip','maennlich','Außerirdischer','blip','blip','antennen',2,'keins','keins'],
  ['blubber','Blubber','maennlich','Kleiner Drache','blubber','blubber','fluegel',2,'keins','keins'],
  ['funke','Funke','maennlich','Elektro-Eichhörnchen','funke','funke','backenblitze',2,'keins','keins'],
  ['groschen','Groschen','maennlich','Waschbär','groschen','groschen','keins',3,'ringe','keins'],
  ['leo','Leo','maennlich','Löwenjunges','leo','leo','maehne',2,'keins','keins'],
  ['linus','Linus','maennlich','Hase','linus','linus','keins',1,'keins','keins'],
  ['mampf','Mampf','maennlich','Hamster','mampf','mampf','keins',2,'keins','keins'],
  ['rolli','Rolli','maennlich','Igel','rolli','rolli','stacheln',3,'keins','keins'],
  ['tetro','Tetro','maennlich','Schildkröte','tetro','tetro','panzer',2,'keins','keins']
].map(([id,name,geschlecht,tierart,ohren,schwanz,besonderheit,ton,muster,zubehoer]) => ({
  id,name,geschlecht,tierart,ohren,schwanz,besonderheit,ton,muster,zubehoer,
  geschaetzt: id === 'evi' ? [] : ['ohren','schwanz','besonderheit','ton','muster','zubehoer']
}));

// Kranzton 0=weiß, 1=hellgrau, 2=mittelgrau, 3=dunkelgrau.
// Bei allen Parkfiguren sind Kranzton und Struktur vorläufig geschätzt (~).
const KRANZ_UND_STRUKTUR = {
  evi:[0,'punkte'], 'evi-m':[0,'punkte'],
  lotti:[1,'gesprenkelt'], ella:[1,'glatt'], gloeckchen:[0,'glatt'],
  kiwi:[1,'gesprenkelt'], klecksi:[0,'gesprenkelt'], lumi:[0,'glatt'],
  mia:[0,'glatt'], mimi:[1,'glatt'], moosi:[2,'wuschelig'], spuki:[0,'glatt'],
  bit:[2,'glatt'], blip:[1,'glatt'], blubber:[2,'glatt'], funke:[1,'gestrichelt'],
  groschen:[1,'gesprenkelt'], leo:[2,'wuschelig'], linus:[0,'glatt'],
  mampf:[1,'glatt'], rolli:[2,'gestrichelt'], tetro:[2,'glatt']
};
for (const figur of window.FIGUREN) {
  [figur.kranzton,figur.struktur] = KRANZ_UND_STRUKTUR[figur.id];
  if (figur.id !== 'evi' && figur.id !== 'evi-m') figur.geschaetzt.push('kranzton','struktur');
}
