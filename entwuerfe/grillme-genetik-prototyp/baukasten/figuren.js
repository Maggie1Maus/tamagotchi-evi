// Geschätzte Merkmale sind mit ~ markiert. Alle Einträge können hier geändert werden.
window.FIGUREN = [
  ['evi','Evi','weiblich','Evi-Art','evi','evi','keins',2,'punkte','keins'],
  ['evi-m','Männliche Evi','maennlich','Evi-Art','evi-m','evi-m','maehne',2,'punkte','keins'],
  ['lotti','Lotti','weiblich','Rehkitz','lotti','lotti','keins',2,'flecken','keins'],
  ['ella','Ella','weiblich','Elefantenbaby','ella','ella','ruessel',2,'keins','keins'],
  ['gloeckchen','Glöckchen','weiblich','Hündin','gloeckchen','gloeckchen','keins',2,'keins','gloeckchen'],
  ['kiwi','Kiwi','weiblich','Dino','kiwi','kiwi','keins',2,'punkte','keins'],
  ['klecksi','Klecksi','weiblich','Dalmatinerwelpe','klecksi','klecksi','keins',1,'flecken','keins'],
  ['lumi','Lumi','weiblich','Glühwürmchen-Fee','lumi','lumi','fluegel',1,'keins','keins'],
  ['mia','Mia','weiblich','Kätzchen','mia','mia','keins',2,'helle-brust','schleife'],
  ['mimi','Mimi','weiblich','Schnecke','keins','keins','fuehler',2,'keins','keins'],
  ['moosi','Moosi','weiblich','Waldgeist','moosi','moosi','blaetter',3,'keins','keins'],
  ['spuki','Spuki','weiblich','Geisterkätzchen','keins','keins','keins',0,'keins','keins'],
  ['bit','Bit','maennlich','Roboterhund','bit','bit','antennen',3,'keins','keins'],
  ['blip','Blip','maennlich','Außerirdischer','blip','blip','antennen',2,'keins','keins'],
  ['blubber','Blubber','maennlich','Kleiner Drache','blubber','blubber','fluegel',2,'keins','keins'],
  ['funke','Funke','maennlich','Elektro-Eichhörnchen','funke','funke','keins',2,'keins','keins'],
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
