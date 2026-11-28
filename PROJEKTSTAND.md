# Tamagotchi-Projekt – Stand zum Weiterarbeiten

## Aufräumen: Archiv (27. November 2026)
- Neuer Ordner `archiv/` (siehe `archiv/README.md`). Aussortiertes wird dort abgelegt statt gelöscht; die Pfade spiegeln die alten.
- Archiviert: alte Evi-Fassung `emotionen/evi-*.png`, lose Vorschaubilder und Bildprompts in `entwuerfe/evi/`, Ohr-Tests in `zeichnen/`, doppelte Vorschauen in `graustufen-vorschau/`, männliche Blätter (`maennlich/quellen`), die vier alten Pixel-Evi-Basisbilder, die lokale Evi-Emotionen-ZIP sowie die **Handy-App** (Code, Tests, Icons, `build-art.ps1`, `prepare_besuch.py`) mit ihren Sicherungen.
- In `handy-app/` bleibt, was der Waveshare-Bau noch liest (siehe `CLAUDE.md`). Die Vorschau „handy-app“ in `.claude/launch.json` entfällt. Evis fertige Bilder in `handy-app/dist/img/` bleiben; zum Neubauen `build-art.ps1` samt Basisbildern aus dem Archiv zurückholen.

## Evi neu aus eigenen Ebenen (27. November 2026)

- **Ordner nach Altersstufen (27.11.):** `entwuerfe/evi/zeichnen/` hat jetzt `erwachsen/` (alles bisherige: basis, vorlagen, ergebnis, werkzeug, gesichter …), dazu leere, gleich aufgebaute `baby/`, `teenager/`, `senior/`. Pfade unten gelten für `erwachsen/`.
- **Teile-Struktur (27.11.):** Kein `basis/` mehr. Pro Stufe: `koepfe/`, `gesichter/`, `ohren/`, `halsschmuck/` (Krause, Schleife, Halsband …) jeweils mit Ansichten `frontal/ seite/ schnueffeln/ hinten/`, dazu `koerper/`, `schwaenze/`, `muster/` (**ebenfalls mit Ansichtsordnern**, Haltung im Dateinamen, z. B. `koerper-sitzend`), `posen/` (Textdatei je Pose mit Teile-Liste). Evis Grundteile sind jetzt `kopf-rund`, `ohr-buschel`, `schwanz-buschel`, `koerper-sitzend`, `krause-evi`, `gesicht-neutral`. `zusammensetzen.ps1` hat Parameter (-Gesicht, -Ohr, -Hals …) und liest diese Pfade; **noch nicht auf Windows getestet**.
- **Klecksi-Teile (28.11.):** Von der Nutzerin auf Evis Vorlage gezeichnet und einsortiert: `ohr-schlapp`, `ohr-schlapp-flecken` (beide Ohren in einer Datei), `schwanz-duenn`, `schwanz-duenn-flecken`, `muster/frontal/muster-flecken-gesicht`, `muster/frontal/muster-flecken-koerper`. Rezept: `erwachsen/posen/klecksi-sitzen-frontal.txt`. Alle Teile-Ordner haben die Ansichtsordner (frontal, seite, schnueffeln, hinten). **`zusammensetzen.ps1` kennt Muster und Ohren mit beiden Seiten noch nicht** (spiegelt das Ohr immer) – noch nicht gebaut. Neue Gesichter der Nutzerin (neutral, glücklich, beleidigt …) liegen in `erwachsen/gesichter/`.

- Die Nutzerin zeichnet Evi jetzt selbst auf dem iPad (Adobe Fresco) in sechs Ebenen: `basis/Evi-neue-vorlage-` + `kopf`, `koerper`, `kranz`, `gesicht`, `ohr`, `schwanz` (2048 × 2048, transparent, in `entwuerfe/evi/zeichnen/`). Weiße Flächen (Krause) sind Absicht: sie decken Linien dahinter ab.
- `zusammensetzen.ps1` (+ `.cs`) baut daraus `Evi-neu-grau.png` (Fellgrau, Schattierungen, Ohr gespiegelt); `pixelfassung.ps1` rechnet es auf 114 × 116 Pixel, 16 Graustufen herunter (`Evi-neu-pixel.png`). Die Lücke zwischen den Vorderbeinen wird nach ihrer Größe erkannt und dunkel gefärbt (Körper-Ebene stark ändern → prüfen).
- Anleitung und Vorlagen zum Zeichnen: `entwuerfe/evi/zeichnen/ANLEITUNG.md`. Ordner in `zeichnen/`: `basis/` (sechs Grundebenen), `vorlagen/`, `ergebnis/` (Evi-neu-grau, Evi-neu-pixel), `werkzeug/` (zusammensetzen.ps1, pixelfassung.ps1), `gesichter/ ohren/ schwaenze/ muster/ koerper/` für neue Teile (Namensregeln in der Anleitung). Die Ohr-Zeichnungen `ohr-buschel-maggie*.png` waren frühere Tests (jetzt in `archiv/entwuerfe/evi/zeichnen/`).
- Noch nicht auf dem Waveshare eingebaut. Nächster Schritt offen: Gesichter für die Emotionen zeichnen lassen, Altersstufen, Ohren und Schwänze für die Kreuzung mit den 20 Tamagotchis.

## Bauteile Paket 1 gebaut (27. November 2026) – wartet auf Freigabe

- Aus den KI-Bildern in `archiv/entwuerfe/grillme-genetik-prototyp/baukasten/werkzeug/ki/` sind Ohren und Schwänze von Mia, Glöckchen, Lotti, Klecksi und Funke (Erwachsen, beide Geschlechter) entstanden, dazu `halsband`, `backenblitze`, `kitz-tupfen`, `augenfleck` und neue `flecken` für Klecksi. [Prüfbogen](archiv/entwuerfe/grillme-genetik-prototyp/baukasten/werkzeug/pruefbogen-paket-1.png). Bauskripte jetzt auch in Python (`paket1-bauen.py`, `pruefbogen-paket-1.py`, `teile-aktualisieren.py`). Evis Basis und die Probelauf-Teile sind unverändert (nur `flecken` wurde für Klecksi neu gerechnet).
- Zu prüfen: Klecksis dunkles Ohr hat eine helle Lücke (helle Ohr-Pixel gleichen dem Fell und fallen weg), Funkes Ohren sind klein, die Mädchen-Muster `kitz-tupfen` und `flecken` sind wegen der kleinen Fellfläche sparsam, der rechte Jungenkranz zeigt weiter den bekannten Rand aus der freigegebenen Basis.
- **Rückmeldung der Nutzerin (27.11.):** Blitze zu klein/verpixelt; Klecksi soll Flecken auch am Körper haben; Ohren sitzen nicht richtig am Kopf und sind pixelig. Erledigt: Blitze jetzt sauber gesetzt (5×6 Pixel, mehr Platz ist auf der Wange nicht), Klecksis `flecken` über den ganzen Körper. **Offen:** Ohren brauchen neue KI-Bilder, Aufträge dafür unter „Zweite Runde Ohren“ in `PROMPT-paket-1-bildauftraege.md` (Funke, Klecksi, Glöckchen, Lotti).
- Keine weiteren Figuren oder Altersstufen, bis die Nutzerin Paket 1 freigibt.

## Eigene Bauteile zeichnen auf dem iPad (27. November 2026)

- Wunsch der Nutzerin: Die KI-Zeichnungen der letzten Runden gefallen ihr nicht; sie möchte die Bauteile (Ohren, Gesichtsmerkmale, Schwänze, Fell-Muster) selbst mit iPad, Apple Pencil und Procreate zeichnen. Die 20 ursprünglichen Tamagotchis bleiben und werden nur mit Evi gekreuzt.
- Fertig: [Anleitung](entwuerfe/evi/zeichnen/ANLEITUNG.md) und zwei Vorlagen (`Vorlage-Evi.png`, `Vorlage-Evi-mit-Raster.png`, 2048 × 2048, Evi blass als Unterlage; gebaut mit `archiv/entwuerfe/evi/zeichnen/vorlage-bauen.ps1` aus der Graustufen-Evi `archiv/entwuerfe/grillme-genetik-prototyp/baukasten/werkzeug/evi-weiblich-original.png`). Abgabe als PSD in `entwuerfe/evi/zeichnen/abgabe/`.
- Nächster Schritt: Die Nutzerin zeichnet zuerst zwei Ohren, einen Schwanz und ein Streifenmuster. Danach Ebenen auseinanderschneiden (Werkzeug für PSD fehlt noch, Python ist auf dem PC nicht installiert), spiegeln, Graustufen und Fellfarben setzen und kombinieren.

## Bauteil-Probelauf: Korrekturrunde (27. November 2026)

- Der [neue Prüfbogen](archiv/entwuerfe/grillme-genetik-prototyp/baukasten/werkzeug/pruefbogen-erwachsen.png) nutzt Mähne A nur für Leo-Jungen; B ist entfernt. Die hellen Schwanzspitzen haben konturgenaue Kranzmasken ohne graues Fell, und die Kontrastkorrektur greift nur bei dunklen, fast gleichen Tönen. Der weiße Rest am rechten Jungenkranz wurde als Lücke im Kranz geschlossen. Evis eigene Teile ergeben bei beiden Geschlechtern weiterhin 0 abweichende Pixel zum Original.
- **Freigegeben von der Nutzerin (27. November 2026).** Die anschließende Kleinkorrektur ist im neuen Prüfbogen umgesetzt: einzelne helle Pixel am rechten Jungenkranz und an den Schultern sind in der Kranzmaske, die wuschelige Struktur ist von der Stirnmitte entfernt. Leo-Jungen bekommen Mähne A, Leo-Mädchen keine Mähne (Löwin); ein heller Kranz auf hellem Fell ist erlaubt. Auf Wunsch der Nutzerin werden derzeit keine neuen Figuren oder Altersstufen ergänzt.
- Älterer Hinweis (vor der Freigabe): Evis Stirnspitzen sind in Mähne A noch sichtbar. Fellstrukturen und Flecken sind bei Originalgröße fein, Linus' Schwanz wird teilweise vom Körper verdeckt. Keine weiteren Figuren oder Altersstufen ergänzen, bis die Nutzerin den Probelauf freigibt.

## Männliche Evi und neuer Familien-Baukasten (26. November 2026)

- Die Nutzerin hat das männliche Evi-Charakterblatt freigegeben. Unter [maennlich/README.md](entwuerfe/evi/maennlich/README.md) liegen 80 Graustufen-Sprites für Baby, Teenager, Erwachsener und Opa. Der beleidigte Opa wurde auf vier Pfoten korrigiert. Die Fenster- und Gartenposen zeigen nur die Figur.
- Der neue [Familien-Baukasten](archiv/entwuerfe/grillme-genetik-prototyp/baukasten/README.md) läuft als eigenständige `index.html` mit eingebetteten Bildern, Elternwahl nach Geschlecht, Merkmalsauswahl, Generationen und Liste der noch fehlenden deckungsgleichen Teile. Die vorhandenen Kreuzungsvorschauen bleiben bestehen.
- Für Erwachsene gibt es jetzt einen [Bauteil-Probelauf](archiv/entwuerfe/grillme-genetik-prototyp/baukasten/README.md) für beide Geschlechter mit Leo-, Linus- und Groschen-Ohren und -Schwänzen, Mähne, Bereichskarten, Fell- und Kranztönen, Strukturen und Zubehör. Der [Prüfbogen](archiv/entwuerfe/grillme-genetik-prototyp/baukasten/werkzeug/pruefbogen-erwachsen.png) zeigt die Ergebnisse; Evis eigene Teile ergeben wieder exakt das Original (0 abweichende Pixel). Leos Mähne und Groschens Schwanzansatz brauchen optische Korrektur und Freigabe. Weitere Figuren und Altersstufen bleiben bis dahin offen. Der interaktive Browsertest von `file://` wurde durch die Browser-Sicherheitsrichtlinie verweigert.

## Grillme-Genetik: korrigierte Ohrvorschau (26. November 2026)

- Die [isolierte Vorschau](archiv/entwuerfe/grillme-genetik-prototyp/README.md) zeigt Evis aktuelle Graustufen-Ohren auf Mia, Lotti und Glöckchen. Jede Altersstufe hat eigene Konturen und Ansatzpunkte. Die Originale stehen direkt darunter.
- Augen, Schnauze, Schwanz und Bewegungsposen brauchen noch eigene Freistellungen. Vor der Ausweitung auf weitere Figuren steht die optische Freigabe der Vorschau.

## Dorfbild eingebaut (26. November 2026)

- Die Nutzerin hat die **Weihnachts-Dorfbilder** hochgeladen (`entwuerfe/dorf/weihnachten/dorf-weihnachten.zip`, 15 PNGs 320 × 174: Tag/Dämmerung/Nacht × Sonne, Sonne+Wolken, bewölkt, Regen, Schnee). `tools/make-dorf-grau.py` packt sie 1:1 nach `evi-handy/dorfbilder.h` (`DORF_WEIHNACHT[Himmel][Wetter]`, 16 Graustufen, rund 420 KB Programmspeicher).
- **Ablauf:** Evi läuft den Kiesweg entlang zu den Häusern. Dort zeigt der Bildschirm **das Dorfbild** mit Evi vorne links, **keine Navigation im Dorf**. **A** (oder das Bild antippen) öffnet die Liste **„Wen besuchst du?“** rechts im Bild: alle, die man aus dem Park kennt, mit „zu Hause“, „im Park“ oder „wohnt bei dir“; Pfeile wählen, A (oder die Zeile zweimal antippen) besucht, B schließt die Liste, B im Dorfbild geht zurück in den Park. Unbekannte stehen nicht in der Liste.
- **Außerhalb der Weihnachtszeit (18.11.–5.1.)** gibt es noch kein Dorfbild; dort dient der Park (Evi am Dorfeingang) als Hintergrund. **Für die anderen Jahreszeiten fehlen noch Dorfbilder** (Frühling, Sommer, Herbst, Winter ohne Weihnachtsschmuck): bei Bedarf bei der Nutzerin anfragen und über das Skript einbauen.
- Test `dorf.cpp` prüft u. a. Pixel für Pixel alle Dorfbilder zur Weihnachtszeit.

## Neue Genetikvorschau mit gemeinsamer Kopfform (26. November 2026)

- Der [interaktive Prototyp](archiv/entwuerfe/grillme-genetik-prototyp/gemeinsame-kopfform/README.md) verwendet jetzt einen Evi-Entwurf, Lotti, Glöckchen und Mia als Ausgangsfiguren. Die gemeinsame Kopfform ist wieder runder. Die fünf Merkmale lassen sich in allen vier Altersstufen mit acht Emotionen und 13 Posen anschauen.

## Schritt 3 (26.11.2026): Verlieben, Klopfen, Partner, Ei, Baby, Oma im Zimmer

Neue Dateien `evi-handy/haushalt.h`, `haushalt.ino`. Gespeichert im EEPROM ab 2200 (`HaushaltStand`, mit Prüfsumme). Geändert: `park.h/.ino`, `dorf.ino`, `stammbaum.ino` (`stammEintragenKern`), `kuscheln.ino`, `evi-handy.ino`.

- **Verlieben** nur zwischen Mädchen und Junge (`gegengeschlecht`). Ein **erwachsenes Tamagotchi ohne Partner** wird nach einem Treffen im Park oder einem Besuch im Dorf gefragt „Soll X dein Partner werden?“, wenn die Figur das andere Geschlecht hat und **beste Freunde (Stufe 7)** sind. „Nein“ gilt für den Tag. Gleiche Geschlechter bleiben nur Freunde.
- **Klopfen:** In den letzten 24 Stunden des Erwachsenenlebens klopft es ohne Partner **alle zwei Stunden tagsüber (9, 11 … 19 Uhr)**: Klopf-Piepton, Bildschirm geht an, Frage „Soll X dein Partner werden?“ (weißer Hintergrund, beide Figuren). Bei „Nein“ kommt im nächsten Zeitfenster ein neuer Kandidat des anderen Geschlechts (alle sechs verschieden); letzte Klopfzeit eine Stunde vor Schluss.
- **Partner** zieht ein, lebt frei im Zimmer (Originalgröße, läuft, schaut aus dem Fenster, schläft nachts auf dem Teppich, Antippen = kurzer Satz), versorgt sich selbst. Er steht nie mehr im Park; im Dorf: „wohnt bei dir“. Sein Name (Namenscode) wird gespeichert.
- **Ei:** am **Morgen/Vormittag (8–11 Uhr) des letzten Erwachsenentags**; zieht der Partner später ein, 20 Minuten danach; bleibt weniger als 10 Minuten bis zum Ende, gibt es kein Ei und der Partner zieht wieder aus. Das Ei hüpft **eine Minute** im Zimmer, dann schlüpft es.
- **Schlüpfen:** Der Partner geht. Das **Baby ist das neue Tamagotchi** (Generation +1, Münzen bleiben, Sammlung leer, danach die Namenseingabe, Geschlecht zufällig). Das bisherige Tamagotchi kommt **mit seinen vollen 10 Lebenstagen in den Stammbaum** (Eltern des Babys: es und der Partner) und **lebt als Oma/Opa im Zimmer weiter** (ohne Balken, ohne Pflege; läuft herum, Antippen = Satz, schläft nachts auf dem Teppich).
- **Tod der Oma:** Meldung „ist friedlich eingeschlafen“, sie liegt **15 Minuten** im Zimmer (Schlaf-Pose). Antippen fragt „Begraben?“; „Begraben“ lässt sie sofort verschwinden, sonst ist sie nach 15 Minuten von selbst weg.
- **Notlösung:** Stirbt ein Tamagotchi früher (Vernachlässigung) oder findet sich kein Partner, bleibt **„Neues Ei“** im Menü (neues Ei ohne Namen, Elternteil nach Freundschaft). Dann zieht ein Partner wieder aus.
- Test `tools/pc-nachbildung/haushalt.cpp` (in `pruefen.sh`): Klopfzeiten, Kandidaten, Ja/Nein, Eizeit, Schlüpfen, Stammbaum, Oma, Begraben, Speichern, Junge/Mädchen, Notlösung. Alle Prüfungen fehlerfrei. **Am Board noch nicht ausprobiert:** besonders Klopfen (Zeitfenster), Laufen der Bewohner im Zimmer, Lage der Schaltflächen.
- Offen/Ideen: Partner und Oma interagieren mit weiteren Aktionen; Partner im Park nicht sichtbar, sein Haus im Dorf leer; Oma-Bild beim Tod unabhängig vom Wesen.

## Schritt 2 (26.11.2026): Das Dorf

- Neue Datei `evi-handy/dorf.ino`. Im Park gibt es ein viertes Ziel **„Zum Dorf“** (die Häuser am Ende des Kieswegs antippen oder mit dem Steuerkreuz wählen). Das Tamagotchi läuft bis zu den Häusern, dann öffnet sich die Auswahl: das Raster aller 20 Figuren (wie das Album); darunter steht, ob die gewählte Figur **zu Hause oder gerade im Park** ist. A (oder zweimal antippen) besucht.
- **Nur Bekannte** (mindestens ein Treffen im Park) lassen sich besuchen („Das Haus kennst du noch nicht.“, 3 Varianten). **Steht die Figur gerade im Park** (`parkBesucher`), ist sie nicht zu Hause: 8 Varianten wie „Klopf, klopf … Mampf ist unterwegs.“
- **Der Besuch** spielt im eigenen Zimmer **ohne Weihnachtsdeko** (`dorfHaus`-Schalter in `weihnachtsZeit`), Tamagotchi links, Besucher rechts in Originalgröße, hinter Möbeln verdeckt. Ablauf mit A: Begrüßung (Hausgruß oder Satz der Stufe) → zweiter Satz → Geschenk (nur beste Freunde, höchstens einmal am Tag je Figur, 1/3 Chance) → danach immer neue Sätze zum Plaudern; B zurück ins Dorf, B im Dorf zurück in den Park.
- **Jeder Besuch zählt als Treffen**, auch mehrmals am Tag (Wunsch der Nutzerin): so wächst die Stufe „beste Freunde“ (ab 7). Das Geschenk, das man beim Verlassen noch nicht gesehen hat, wird als Meldung gesagt.
- Test `tools/pc-nachbildung/dorf.cpp` (in `pruefen.sh`); alle Prüfungen fehlerfrei. Am Board noch nicht ausprobiert (Lage der antippbaren Fläche über den Häusern ansehen!).

## Schritt 1 (26.11.2026): Speicher umgebaut, Stammbaum scrollt

- **Fundstücke höchstens 50** (vorher 256), `SAMMLUNG_MAX` in `sammlung.h`. Jedes neue Tamagotchi beginnt mit leerer Sammlung (bei „Neues Ei“ und später beim Übernehmen des Babys). Spielstand Version 4 (816 Byte); Version 3 (256 Funde) und 2 werden beim Start übernommen, die letzten 50 Funde bleiben (Test `speicher.cpp`).
- **Stammbaum bis zu 50 Tamagotchis** (`STAMM_MAX`), neuer Platz im EEPROM ab 1100; Version 2 vom alten Platz 3904 wird übernommen. **Ansicht als lange Fläche von oben (älteste Familie) nach unten:** je Familie Mutter, Vater, darunter das Kind mit Name, Geschlecht, **Generation**, Wesen und Alter; Linien verbinden das Kind mit seiner Seite in der nächsten Familie. Scrollen mit Pfeiltasten (A = eine Familie), Finger ziehen oder ▲/▼ unten; Schieber am Rand; B schließt. Öffnet ganz unten beim jetzigen Tamagotchi.
- Frei im EEPROM: 816–1100, 2120–3328 (für Partner, Ei, Oma).

## Geplant: Verlieben, Partner, Ei und Generationenwechsel (Wunsch der Nutzerin vom 26. November 2026, noch nicht gebaut)

Erster Teil schon umgesetzt: **Das Geschlecht eines Babys wird ausgewürfelt** (nicht gewählt); die Namenseingabe zeigt „Es ist ein Mädchen/Junge!“.

Der Lebenslauf soll sich ändern (statt „Neues Ei“ im Menü):
- **Verlieben:** Nur Mädchen und Junge können sich verlieben und ein Baby bekommen. Zwei gleiche Geschlechter bleiben beste Freunde. Verstehen sich ein erwachsenes Tamagotchi und eine Parkfreund-Figur des anderen Geschlechts sehr gut, gibt es irgendwann die Auswahl „verlieben“. Sagt die Nutzerin ja, wird die Figur Partner und **zieht ins Haus ein**. Der Partner kümmert sich selbst um seine Bedürfnisse; die Nutzerin spielt nicht mit ihm.
- **Ei:** Am letzten Tag des erwachsenen Lebens legt das Tamagotchi ein Ei. Es hüpft etwa eine Minute und schlüpft dann. Mit dem Schlüpfen **verschwindet der Partner**; zu zweit sind dann das bisherige Tamagotchi (wird Oma) und das Baby.
- **Das Baby ist das neue Tamagotchi der Nutzerin** (sie kümmert sich um das Baby). Die Mutter bzw. der Vater wird Oma/Opa, die Nutzerin kümmert sich nicht mehr um sie/ihn. Das Baby bekommt ein zufälliges Geschlecht, und die Nutzerin gibt ihm einen Namen. Eltern im Stammbaum: das Tamagotchi davor und der Partner.
- **Antworten der Nutzerin (26.11.2026):**
  - Das bisherige Tamagotchi **bleibt im Haus und lebt seine Lebensphasen zu Ende**, die Nutzerin kümmert sich ab dem Schlüpfen aber nur noch um das Baby.
  - **Ohne Partner:** Am letzten erwachsenen Tag klopft es **alle zwei Stunden** (Piepton) an der Haustür. Ein Bildschirm zeigt das eigene und das andere Tamagotchi (weißer Hintergrund, keine neue Zeichnung) mit der Frage, ob es der neue Partner werden soll. Bei „Nein“ kommt zwei Stunden später ein neuer Kandidat, so lange, bis ein Partner gefunden ist. Findet sich keiner, beginnt eine neue Generation mit einem **neuen Ei**, wenn die Oma einschläft.
  - **Frühes Sterben** (z. B. Vernachlässigung): „Neues Ei“ bleibt als Notlösung.
  - **Zimmer mit mehreren:** Alle Figuren in **Originalgröße**, sie laufen frei herum, schauen aus dem Fenster und **schlafen auf dem Teppich**. Das gespielte Tamagotchi schläft im Körbchen. Man kann mit den anderen **interagieren**.
  - **Oma/Opa:** Schläft sie friedlich ein, gibt es eine kleine Meldung. Sie bleibt dann **15 Minuten im Zimmer liegen**; mit ihr interagieren und **„begraben“** lässt sie sofort verschwinden.
  - Lebensphasen (Stand `pet.h`): Baby 1 Tag, Teenager 2 Tage, Erwachsen 5 Tage, Oma 2 Tage, zusammen 10 Tage. Die Nutzerin wollte die Längen wissen, um zu entscheiden, wann das Verlieben kommt.
- **Dorf (Wunsch der Nutzerin, 26.11.2026):** Im Park läuft Evi den Kiesweg entlang zu den Häusern („Dorf“). Dort wählt man, wen man besucht; nur Tamagotchis, die man im Park schon getroffen hat. Steht das Tamagotchi gerade im Park, kommt eine Meldung („nicht zu Hause“, mehrere Varianten). Das Haus ist ein Raum wie das eigene Zimmer ohne Weihnachtsdeko (keine neue Zeichnung). Man plaudert und geht wieder heim. **Jeder Besuch bringt Freundschaft** (auch mehrere am Tag), so erreicht man die Stufe „beste Freunde“.
- **Verlieben erst ab Stufe 7** (beste Freunde), nur Mädchen/Junge, nur für erwachsene Tamagotchis ohne Partner. Findet sich der Partner erst am Nachmittag des letzten Erwachsenentags, ist das so; das Tamagotchi lebt seine Lebensphase wie vorgesehen zu Ende.
- **Ei:** Mit Partner am Morgen/Vormittag des letzten Erwachsenentags; sonst gilt die Regel „kurz nach dem Einzug des Partners“. **Stammbaum:** wächst mit jedem Baby, zeigt die Generation an.
- **Speicher geprüft (26.11.2026):** Der Speicherbereich für Spielstände (EEPROM, 4096 Byte) ist nicht voll: Spielstand 0–3288 (davon 3092 Byte für bis zu 256 Funde), Zimmer 3328–3364, Freundschaft 3584–3800, Parkfreunde 3856–3904, Stammbaum 3904–4084, Merker 4095. **Frei sind etwa 330 Byte** in Lücken (3288–3328, 3364–3584, 3800–3856, 4084–4095). Das Programm (Flash, 4 MB) ist ein anderer Speicher: zuletzt gemessen 31 % (1301480 Byte) und 21 % Variablen; hier ist keine echte Pico-Übersetzung möglich, das zeigt die Arduino IDE nach dem Übersetzen.
- Noch offen: wie viele Generationen der Stammbaum speichern kann (siehe Speicher: etwa 10), welche Interaktionen es mit den anderen Tamagotchis gibt (vorerst nur die besprochenen).

## Namen: Tamagotchis benennen, wechselnde Namen der Parkfreunde – 26. November 2026

Auf Wunsch der Nutzerin, nur Waveshare. Neu: `evi-handy/namen.h`, `namen.ino`. Geändert: `stammbaum.h/.ino`, `park.h/.ino`, `parkfreunde.ino`, `pet.h`, `freunde.ino`, `evi-handy.ino`.

- **Die Tiere heißen allgemein „Tamagotchi“**, das allererste hieß Evi und behält den Namen. Texte ohne Namen sagen „Tamagotchi“ (z. B. „Tamagotchi ruht gerade.“).
- **Jedes neue Baby bekommt von der Nutzerin einen Namen.** „Neues Ei“ erzeugt ein Tamagotchi ohne Namen. Sobald es geschlüpft ist, öffnet sich die Namenseingabe (Park-Phase `PARK_NAME`): Buchstabenfeld A–Z, Ä Ö Ü ß mit Tasten (Steuerkreuz, A tippt, B löscht, START fertig) oder Touch, dazu Knöpfe „Löschen“, „Vorschlag“ (zufälliger Name aus der Liste), „Mädchen/Junge“ und „Fertig“. Höchstens 11 Buchstaben (und 19 Bytes wegen der Umlaute), erster Buchstabe groß, der Rest klein. Ohne Namen geht „Fertig“ nicht; schaltet man den Bildschirm aus, kommt die Eingabe wieder, bis ein Name da ist.
- **Tamagotchis haben ein Geschlecht** (Mädchen/Junge), das beim Benennen gewählt wird; Evi ist ein Mädchen. Es steht im Stammbaum.
- **50 Jungen- und 50 Mädchennamen** (`NAMEN_M`, `NAMEN_W`, u. a. Alf, Paula). Die 20 Parkfiguren sind je 10 weiblich/männlich (Maske in `namen.h`). **Ab dem ersten benannten Baby** (`stamm.neueNamenAb`) heißen die Figuren neu, innerhalb eines Tamagotchi-Lebens fest (alle Lebensphasen gleich), alle verschieden, nie wie das Tamagotchi selbst. Beim nächsten Tamagotchi wandert jede Figur in ein anderes Zehnerfenster der Liste, nach 5 Leben sind alle 50+50 Namen einmal dran. **Das jetzige Tamagotchi behält die alten Namen** (Lotti, Mampf …). Keine Tierart im Album/Park angezeigt (Wunsch der Nutzerin: man sieht ja das Bild).
- **Stammbaum:** Das Tamagotchi davor ist Mutter oder Vater (je nach Geschlecht), der andere Elternteil ist die Parkfreund-Figur des anderen Geschlechts mit den meisten Treffen. **Ihr Name wird mitgespeichert**, weil er sich ändert (`StammEintrag.elternName`, Namenscode). Mutter steht links, Vater rechts. Speicher: Version 2 (Version 1 wird verworfen; der Stammbaum war erst kurz vorher entstanden).
- Prüfungen: `tools/pc-nachbildung/namen.cpp` (in `pruefen.sh`): Listen, Eingabe per Touch und Tasten, Grenzen, Vorschlag, Geschlecht, Rotation über 5 Tamagotchis, keine Figur heißt wie das Tamagotchi, Stammbaum mit Junge und Mädchen. Alle Prüfungen fehlerfrei. Am Board noch nicht ausprobiert.

## Stammbaum, 20 Wesen-Folgen und Geschenke der Parkfreunde – 26. November 2026

Auf Wunsch der Nutzerin, nur Waveshare, ohne neue Bilder. Neu: `evi-handy/stammbaum.h`, `stammbaum.ino`. Geändert: `pet.h`, `park.ino`, `park.h`, `parkfreunde.ino/.h`, `freunde.ino`, `momente.ino`, `launen.ino`, `evi-handy.ino`.

- **Stammbaum** (Menü „Stammbaum“, auch beim Ei und bei verstorbener Evi): Wenn mit „Neues Ei“ eine neue Evi beginnt, kommt die verstorbene in den Baum: Name, Wesen, wie alt sie wurde (Tage) und ihr Bild in der Lebensphase, in der sie gegangen ist (meist Oma). Ansicht: oben Mutter und Vater, Linien nach unten, darunter die gewählte Evi; unten Name · Wesen · Alter. Links/rechts (Pfeile oder Antippen) blättert zur Mutter bzw. zum Kind, B oder „Zurück“ schließt. Läuft als Park-Phase `PARK_STAMM`. Gespeichert im EEPROM ab 3904 (letzte 8 verstorbene Evis, ältere fallen heraus; die jetzige Evi steht immer am Ende).
- **Mutter** = die Evi davor. **Vater** = die männliche Parkfreund-Figur (Bit, Blip, Blubber, Funke, Groschen, Leo, Linus, Mampf, Rolli, Tetro), die die Mutter am häufigsten getroffen hat; ohne Treffen „unbekannt“. Das ist eine Festlegung von Claude; die Nutzerin hat nicht gesagt, wer Vater sein soll. Die allererste Evi hat unbekannte Eltern.
- **20 neue Folgen des Wesens** (Regeln in `pet.h`, Rest verteilt): Naschkatze: Hunger 15 % schneller, eine Nascherei mehr am Tag, Naschen +15 statt +10 Laune, mehr verfressene Tage. Sportlerin: Spiele 50 % mehr Münzen, 20 % schneller schmutzig, wacht eine Stunde früher auf, mehr verspielte Tage. Wasserscheu: Saubermachen kostet Laune (−5 statt +3), 20 % langsamer schmutzig. Langschläferin: schläft morgens eine Stunde länger, abends eine Stunde früher müde, Schlaf kostet 20 % weniger, Gute Nacht +15 statt +10 Laune, mehr verträumte Tage. Schmusekatze: Kuscheln 50 % stärker, mehr verschmuste Tage, nach zwei statt drei Streichlern versöhnt. Neugierige: eine Frage mehr am Tag, Plaudern wirkt 5- statt 3-mal am Tag. Dazu kommen die früheren Folgen (Freundschaft bei Naschen/Spielen/Kuscheln/Plaudern, wasserscheu mag Regen im Park nicht, passende Besucher). Lieblingslaune kommt an etwa 40 % der Tage (statt 25 %).
- **Geschenke der besten Freunde** (ab 7 Treffen, an etwa jedem dritten Tag, einmal pro Figur und Tag): nach der Begrüßung und dem zweiten Satz folgt auf A ein dritter Text mit dem Geschenk, je nach Wesen der Figur: Naschkatze Kekse (satt +25), Sportlerin tobt mit Evi (Laune +15), wasserscheu putzt Evi sauber (+30), Langschläferin ein Kissen (Laune +10), Schmusekatze eine Umarmung (Freundschaft mit dir +3), Neugierige 4–7 Münzen. Das Geschenk wird sofort gegeben; schließt man den Park vorher, steht es als Meldung.
- Prüfungen: neues Programm `tools/pc-nachbildung/stammbaum.cpp` (in `pruefen.sh`) prüft jede Folge, Geschenke, Stammbaum (Speichern, Verdrängen, Vater, Umlaute, alle Seiten zeichnen). Alle Prüfungen und der Fenstervergleich fehlerfrei. Am Board noch nicht ausprobiert.
- Weitere Ideen: weitere Geschenke (Fundstück), Stammbaum mit mehr als 8 Evis, Wesen vererben.

## Parkfreunde, zweiter Satz, Album und Evis Wesen – 26. November 2026

Auf Wunsch der Nutzerin (Ideen 1–4 aus dem Vorschlag), nur Waveshare, ohne neue Bilder. Neue Dateien `evi-handy/parkfreunde.h` und `parkfreunde.ino`; geändert: `park.ino`, `park.h`, `freunde.ino`, `evi-handy.ino`.

- **Freunde erinnern sich:** Jede der 20 Figuren zählt, wie oft Evi sie getroffen hat (höchstens einmal pro Tag und Figur, beim Begrüßen). Stufen: fremd (nie), bekannt (1–2), vertraut (3–6), beste Freunde (ab 7). Fremde bekommen immer einen Kennenlern-Satz, danach etwa jedes zweite Mal einen Satz der Stufe, sonst einen der 50 alten Sätze. Gespeichert im EEPROM ab 3856 (`ParkFreundStand`, mit Prüfsumme). **Die Treffen gehören zu einer Evi** (Feld `evi` = Zeitpunkt „geboren“, Wunsch der Nutzerin): Bei einer neuen Evi fängt alles wieder bei null an (`parkFreundPruefen`). Version 2; ein früher gespeicherter Stand der Version 1 wird dabei verworfen. Jedes Treffen gibt etwas Freundschaft mit dir (0,5, bei gleichem Wesen 2).
- **Zweiter Satz:** Nach der Begrüßung zeigt A noch einen zweiten Satz, dann erst geht es weiter. Er ist entweder über das Wetter/die Jahreszeit/Tageszeit (10 Lagen, je 3 Sätze), über Funde (heute gefunden / noch nicht gesucht), über die Figur selbst oder, bei gleichem Wesen, „wir zwei sind gleich“.
- **Album „Parkfreunde“** (Menü, unter „In den Park“): Raster aus allen 20 Figuren in halber Größe mit Evis Lebensphase. Unbekannte nur als Schatten, drei Punkte zeigen die Stufe. Unten Name, Art, Anzahl Treffen. Pfeile/Touch wählen, B oder „Zurück“ schließt. Läuft als Park-Phase `PARK_ALBUM`, deshalb gelten Schlafen, Bildschirm aus und Touch wie im Park.
- **Evis Wesen:** Jede Evi hat eins von sechs (Naschkatze, Sportlerin, wasserscheu, Langschläferin, Schmusekatze, Neugierige), aus ihrem Zeitpunkt „geboren“ ausgewürfelt, ändert sich nie, steht oben im Album. Jede Figur hat auch eins (Tabelle `FIGUR_WESEN`, nach den „Wesen“ in `entwuerfe/neue-tamagotchis/vorschlaege.md`). Folgen: Naschkatze freut sich über Naschen (mehr Freundschaft), Sportlerin über Spiele (doppelt), Schmusekatze über Kuscheln (doppelt), Neugierige über Plaudern (doppelt) und neue Bekannte; wasserscheue Evis mögen Saubermachen nicht (keine Freundschaft dafür). **Wasserscheue Evis gehen bei Regen nicht in den Park** („Evi mag bei Regen nicht raus.“) und gehen heim, wenn es dort anfängt zu regnen (`parkZuNass`); das Album geht auch bei Regen. Evi kommentiert das manchmal („Brr … muss das Wasser sein?“) und verrät ihr Wesen beim Plaudern. Im Park kommt von den 19 anderen Figuren jedes dritte Mal eine mit gleichem Wesen; Lotti weiter etwa jedes dritte Mal.
- Weitere Ideen dazu: weitere Folgen (Langschläferin schläft morgens länger), Geschenke von besten Park-Freunden, eigene Sätze je Figur.
- Neues PC-Prüfprogramm `tools/pc-nachbildung/parkfreunde.cpp` (in `pruefen.sh`): Satzlängen, Wesen, Treffer-Zählung, Speichern/Laden, Besucherwahl, Folgen, Album in allen Lebensphasen. Alle Prüfungen und der Fenstervergleich fehlerfrei. Am Board noch nicht ausprobiert.

## Uhr zurückgestellt: Dauer-Melodie behoben, Geburtstag neu fragen – 26. November 2026

- **Bug:** Nach Vor- und Zurückstellen der Uhr (Jahreszeiten anschauen) kam alle 10 Sekunden die Entwicklungsmelodie mit „Evi ist jetzt ein Teenager!“. Ursache: Der Spielstand („aktualisiert“) blieb auf dem Zukunftsdatum stehen; `melde()` verglich die Phase dort (erwachsen) mit der Phase jetzt (Teenager). Außerdem rechnete Evi bis zum Erreichen des Zukunftsdatums nicht weiter.
- **Fix:** `weiterrechnen()` in `pet.h` setzt den Stand bei zurückgestellter Uhr auf jetzt und rechnet ab da normal weiter. `melde()` und `setup()` werten einen Stand in der Zukunft nicht mehr als Phasenwechsel. Bereits durchgerechnete Zukunftstage (Hunger, Sauberkeit) bleiben, das Alter stimmt immer (aus dem Schlüpfzeitpunkt).
- Beim ersten Start nach dem Aufspielen kann noch einmal der Bildschirm „Datum und Uhrzeit“ kommen, weil der gespeicherte Stand in der Zukunft liegt – einfach das heutige Datum bestätigen.
- **Geburtstag neu fragen** (Nutzerin hatte sich vertippt): Merker-Byte ganz hinten im EEPROM (`GEB_NEU_ADRESSE`, `GEB_NEU_FRAGEN` in `freunde.ino`). Ist er nicht gleich `GEB_NEU_FRAGEN`, vergisst Evi einmal den Geburtstag (Freundschaft und Antworten bleiben) und fragt bei nächster Gelegenheit neu. Für ein weiteres Neufragen `GEB_NEU_FRAGEN` um eins erhöhen.
- Neues PC-Prüfprogramm `tools/pc-nachbildung/zeitzurueck.cpp` (in `pruefen.sh`): ohne Fix 3 Fehler, mit Fix alles in Ordnung; alle übrigen Prüfungen und der 51-Fenster-Vergleich ebenfalls fehlerfrei. Am Board noch nicht geprüft.

## Aktuelle Korrektur: Fensterbaum nachts ohne Leuchtkontur (26. November 2026)

- Nach dem Aufspielen störte die Nutzerin die weiße Umrandung des Nachtbaums. Diese Gestaltung ist durch eine matte dunkelgraue Nachtpalette ersetzt; die frühere Regel mit weißem Rand gilt nicht mehr.
- Alle vier Nachtbäume werden mit `tools/make-fenster-nacht.py` aus den vorhandenen Tagesbäumen erzeugt. Keine Kantenaufhellung, Baum höchstens 51/255, winterlicher Schnee höchstens 85/255. Schwarzer Nachthimmel sowie die Tages- und Dämmerungsansichten bleiben erhalten.
- Die vier PNG-Nachtebenen, `zimmergrau.h`, sämtliche Nachtvorschauen, Übersichten und das ZIP-Paket werden gemeinsam aktualisiert. Die Firmware verwendet die korrigierten Grafiken direkt.
- Arbeitsumgebung wieder verfügbar: native C++-Übersetzung mit Warnungen, vorhandene Zimmer-/Parkprüfungen, neue Prüfung gegen leuchtende Nachtkonturen sowie 51 Kombinationen und 411 Animationsansichten erfolgreich. Physischer Boardtest der neuen Nachtkorrektur steht aus.

## Heller Fensterausblick – 26. November 2026

- Die Nutzerin hat nach dem Test am Board einen freundlicheren Fensterausblick und einen helleren Tagesbaum gewünscht, die Tageslicht-Vorschau bestätigt und das vollständige Grafikpaket samt Codeanbindung zum Hochladen freigegeben.
- Neue Fensterebenen und alle 51 Kombinationen unter `entwuerfe/zimmer/fenster-hell-2026-11-26/`. Sonne fast weiß, Landschaft und Baum heller; Wolken/Regen abgestuft dunkler; Dämmerung ohne Mond und mit hellem Baum; Nachthimmel schwarz. Schnee ist fest im Winterbaum enthalten, auch bei Sonne; Schneefall nur im Winter.
- In `zimmergrau.h` und `einrichtung.ino` eingebunden; Generator und unabhängiger PNG-Vergleich angepasst. Der komplette aktuelle Ordner `software/schritt-10/evi-handy` bleibt der aufzuspielende Stand.
- Native PC-Prüfungen bestanden vor dem Umgebungsausfall. Nach Wiederherstellung Datenvergleich aller 51 Kombinationen und 411 Animationsansichten zwischen PNG-Ebenen und gepacktem Header erfolgreich. Neue native Übersetzung nach der Wiederherstellung sowie physischer Boardtest stehen noch aus. Details und reproduzierbare Prüfkommandos in der README des Grafikpakets.

## Parkhorizont mit Häusern – Gestaltung vom 26. November 2026

- Die bestehende Parkansicht wird um **genau drei kleine Häuschen am fernen Ende des Kieswegs** ergänzt: eines links, zwei rechts. Die zwei rechten Häuser stehen leicht hintereinander und dürfen sich überlappen; keines schwebt, alle stehen sichtbar auf dem Boden. Sie stehen nah am Weg und ersetzen weder den Weg noch die bestehenden Bäume und Sträucher am Horizont.
- Die Häuser sind zunächst **reine Hintergrundkulisse**. Sie werden im Park nicht angetippt oder einzelnen Freunden per Auswahl zugeordnet. Die bestehenden Parkziele und die derzeitige Besuchsfigur im Park bleiben davon unabhängig.
- Die Hausgrafiken sollen sich an Jahreszeit und Tageszeit anpassen: im Winter etwas Schnee auf den Dächern, nachts einzelne erleuchtete Fenster; die Landschaft bleibt ruhig und lesbar. Eine konkrete Ausgestaltung für alle Zustände wird mit den fertigen Parkgrafiken abgestimmt.
- **Für später vorgemerkt, jetzt nicht umsetzen:** Evi könnte den Kiesweg entlanggehen; danach öffnet sich ein Besuchsmenü mit Freunden wie Lotti. Die Wahl erfolgt im Menü und nicht durch Antippen eines Hauses. Heute wird nur die Ansicht des Parks mit den Häusern gestaltet.
- Gestaltungsprozess: Vorschau gezeigt, Häuser näher an den Weg gerückt, schwebendes Haus korrigiert und die beiden dünnen Hintergrundbäume entfernt. Anschließend beauftragte die Nutzerin die vollständige Ausarbeitung für Jahreszeiten, Tageszeiten und Wetter sowie leuchtende Fenster bei Nacht, Dämmerung und Regen. Der darauf aufbauende Stand ist unten dokumentiert.


Stand: 26. November 2026

## Parkansichten für Jahreszeiten, Tageszeiten und Wetter (26. November 2026)

- **Korrektur nach weiterer Rückmeldung:** Die zunächst zu kleinen Strich-Laternen sind durch freigestellte Graustufenzeichnungen mit Glas, Dach und schattiertem Mast ersetzt. Perspektivische Höhen 72/52/34 Pixel; deutlich breitere und hellere Lichtflächen auf dem Kiesweg. Fünfzackiger hängender Weihnachtsstern als separate Grafik, winterliche Schneedecke auf den Laternendächern. Schaltregeln bleiben Nacht, Dämmerung und Regen; Sterne während der bestehenden Weihnachtszeit. Positionen so angepasst, dass die entfernte Laterne neben der Baumkrone sichtbar bleibt und die vordere den Schneemann nicht überdeckt.
- Die zuvor noch offenen Wünsche sind jetzt in den Quellen umgesetzt: größerer dreiteiliger Schneemann, kleine kahle Büsche bei den Winterhäusern, dichteres Herbstlaub besonders unter dem Baum, Pilze statt Blumen. Alle 51 Ansichten wurden erneut exportiert und geprüft. **Die Nutzerin hat die überarbeitete Vorschau am 26. November 2026 bestätigt und das Hochladen auf GitHub ausdrücklich freigegeben.** Veröffentlichung auf `main` nach dem bestehenden Projektablauf; die frühere automatische Ablehnung ist durch diese neue ausdrückliche Freigabe geklärt.

- Nach den Korrekturen der Häuseranordnung und dem Entfernen der beiden dünnen Hintergrundbäume wurde die vollständige Ausarbeitung beauftragt. Neue Quellen und 320 × 174 große Grundbilder unter `entwuerfe/park/entwurf-2/`; die frühere Version bleibt als Entwurf erhalten.
- Häuser bleiben reine Kulisse. Ihre drei Fenster sind eine eigene Lichtebene und leuchten nachts, bei Dämmerung und bei Regen, auch am Tag. Die drei separaten Parklaternen nutzen dieselbe Einschaltregel mit breiten, hellen Lichtflächen auf dem Boden und dezent gepunktetem Rand. Hängende Sterne während der bestehenden Weihnachtszeit (18. November bis 5. Januar).
- Vier Jahreszeiten mit Blüten, Sommerkrone, dichterem Herbstlaub und Pilzen beziehungsweise Schnee auf Ästen, Baumfuß, Sträuchern, Bank und Dächern sowie größerem Schneemann. Keine Fußspuren. Graustufen beibehalten, Tageshimmel aufgehellt, Nachthimmel schwarz; Regen und Schneefall weiter animiert, Schneefall nur im Winter.
- Eingebaut in die Waveshare-Parkdarstellung: `make-park-grau.py`, `parkbilder.h`, `park.ino`. 51 zulässige Kombinationen werden mit dem echten Firmware-Zeichencode exportiert, siehe `entwuerfe/park/entwurf-2/README.md`. Die Hausbesuchslogik bleibt eine spätere Aufgabe.
- PC-Prüfung: Übersetzung mit Warnungen, bestehende Zimmer-/Park-/Fundprüfungen sowie alle 51 Parkansichten erfolgreich. Noch keine Prüfung auf dem physischen Waveshare-Board.

## Wechselnder Besuch im Park (25. November 2026)

- Wunsch der Nutzerin: Die 20 Graustufen-Figuren gefallen ihr sehr gut. Im Park steht jetzt nicht mehr immer Lotti, sondern wechselnder Besuch. Er bleibt jeweils 4 Stunden derselbe und wechselt nicht bei jedem Parkbesuch. Lotti als Evis Freundin kommt etwa jedes dritte Mal, sonst eine der 19 anderen Figuren. Der Besuch hat immer dieselbe Lebensphase wie Evi.
- 50 Begrüßungssätze erscheinen zufällig, nie zweimal derselbe hintereinander (`PARK_GRUSS_TEXT` in `park.ino`). Höchstens 40 Zeichen, damit der Name und der Satz in eine Zeile passen.
- Die Bilder werden 1:1 übernommen, nur zugeschnitten und wie Lotti um 10 % vergrößert: `software/schritt-10/tools/make-besucher-grau.py` → `evi-handy/besucherbilder.h`. Läuft ohne Pillow, auch mit dem Python aus dem Arduino-Paket (`pngklein.py`). Evi bleibt links vor breiten Figuren stehen.
- Echte Pico-2-Übersetzung ohne Fehler: 1301480 Bytes Programm (31 %), globale Variablen 115080 Bytes (21 %). Vorschau aller 80 Figuren im Park: `entwuerfe/park/entwurf-1/besuch-vorschau.png`. Die PC-Nachbildung (`pruefen.sh`) konnte auf dem Haupt-PC nicht laufen (kein g++). Am Board noch prüfen.

## Graustufen-Zeichnungen für 20 andere Tamagotchis

- Unter `entwuerfe/neue-tamagotchis/graustufen/` liegen 80 einzelne Graustufen-PNGs. Jede Figur hat die Pose „Glücklich“ als Baby, Teenager, erwachsene und ältere Figur (Oma oder Opa). Die PNGs sind 72 × 64 Pixel groß, haben eine gemeinsame Standlinie und verwenden die 16 Graustufen des Spiels mit binärer Transparenz.
- Die bestehenden Figurenentwürfe und Evis Alltagsposen dienten als Vorlagen. Lottis freigegebene Erwachsenenzeichnung aus dem Park ist mittig in die gemeinsame Bildfläche eingesetzt. Zwei Übersichten zeigen die Lebensphasen nebeneinander.

## Öffentlicher Parkeingang mit Lotti und Fundstücken

- Im Waveshare-Menü führt „In den Park“ zum bestätigten Graustufenentwurf. Touch oder Steuerkreuz und A wählen Lotti, den Kiesweg oder den Wiesenrand. Evi läuft selbst zu ihrem Ziel. Bei Lotti folgt eine Begrüßung. An einer Fundstelle schnuppert Evi, und mit A oder einem weiteren Tipper schaut sie nach.
- Der erste Parkfund ist ein Stein am Kiesweg. Danach gibt es pro Ortsdatum an einer von zwei Stellen einen Fund aus dem vorhandenen Sortiment. Zwei aufeinanderfolgende Funde unterscheiden sich. Fundort, Datum, Wetter und Jahreszeit werden im Sideboard gespeichert. Vorhandene Garten- und Spaziergangsfunde bleiben lesbar.
- Der Ausblick im Park nutzt dieselben Regeln für Tageszeit, Jahreszeit und Wetter wie das Zimmerfenster. Die Parkbilder liegen in `entwuerfe/park/entwurf-1/`. `software/schritt-10/tools/make-park-grau.py` erzeugt die Firmwaregrafiken einschließlich Evis vorhandener Such- und Fundposen.
- Seit 25.11.2026 werden Evi und Lotti im Park 10 % größer gezeichnet (`PARK_GROESSE_Z/N` in `park.ino`). Lottis Tippfläche und Zielpfeil wachsen mit. Übersetzt fehlerfrei, am Board noch nicht angesehen.

## Zimmer, Möbel und Wetter in Graustufen (25. November 2026)

- Nach dem Test der Nutzerin am Waveshare wurden Wand, Fußleiste und Dielen aufgehellt. Das Zimmer bleibt bei eingeschaltetem Licht den ganzen Tag gleich hell. „Licht aus“ dimmt den Innenraum einschließlich Möbeln und vorderem Bettrand. Die Uhr bleibt lesbar.
- Im Fenster wechseln Tageszeit und Wetter. Der Winterbaum hat kahle Äste, die bei Schneefall etwas Schnee tragen. Regen und Schnee laufen in je 16 Phasen hinter dem Rahmen. Die Wetteranzeige neben der Uhr wurde entfernt.
- Tierbett, Sideboard, Regal und Teppich sind im Einrichtungsmodus verschiebbar. Ihre PNG-Deckkraft steuert die Verdeckung. `software/schritt-10/tools/make-zimmer-grau.py` erzeugt `evi-handy/zimmergrau.h` aus 89 Zimmer-, Möbel- und Fenster-PNGs und speichert die 16 Graustufen und die Deckkraft getrennt.

## Graustufen für Fundstücke und Weihnachtsschmuck (25. November 2026)

- Spielgrafiken unter `entwuerfe/zimmer/graustufen/spielgrafiken/` geprüft. Die 109 PNGs verwenden 16 Grautöne und Alpha 0/255 in den Größen aus `liste.txt`.
- Eingebaut (1:1, `tools/make-spiel-grau.py` → `evi-handy/spielgrau.h`, 25637 Byte): die 10 Fundstücke, großer Weihnachtsbaum, Kerzentablett, Fenstersterne. Größen stimmen mit den Schwarz-Weiß-Bildern überein, die weiter für Treffer/Verdeckung gelten. Wie bei Evi nur 15 Stufen (Weiß und Hellgrau zusammengelegt).

## Garten pausiert (25. November 2026)

- Wunsch der Nutzerin: Der Garten wird erstmal nicht gebraucht. Menüpunkt „In den Garten“ und alle Garten-Abfragen sind aus der Waveshare-Firmware entfernt (`evi-handy.ino`, Kuscheln, Bett, Bewegung, Freunde, Launen). Der Garten-Code und die Garten-Bilder liegen unverändert in `software/schritt-10/evi-handy/pausiert-garten/` (wird von Arduino nicht mitübersetzt) und können später zurückgeholt werden. Alte Garten-Funde im Sideboard und der Spielstand bleiben unverändert. Handy-App nicht angefasst.
- Noch nicht geprüft: echte Pico-2-Übersetzung (hier nicht möglich). Die Generatoren `make-garten*.py` schreiben noch nach `evi-handy/`; beim Zurückholen die Dateien wieder an den alten Ort schieben.

## Nachthimmel vollständig schwarz (25. November 2026)

- Weiterer ausdrücklicher Wunsch der Nutzerin: Der Nachthimmel ist jetzt bei allen Wetterlagen eine geschlossene schwarze Fläche. Mond, Sterne und Niederschlag werden nachts weiß gezeichnet; Wolken erhalten helle Konturen. Der Baum bleibt schwarz mit weißem Rand. Die 17 Tages- und 17 Dämmerungsansichten bleiben pixelgleich zum vorigen Stand; Dämmerung weiterhin mit weißem Baum und ohne Mond.
- In der Waveshare-Firmware und im reproduzierbaren Grafikexport umgesetzt. Eigene Bildschirmprüfungen sichern alle 17 Nachtzustände auf durchgehend schwarzen freien Himmel sowie alle 16 Regen-/Schneephasen auf weiße sichtbare Striche/Flocken. Der aktuelle Vergleich liegt unter `entwuerfe/zimmer/grillme-2026-11-25/v2/board/fenster-tag-daemmerung-nacht.png`.
- Syntaxprüfung ohne Warnungen, die PC-Prüfprogramme für Einrichtung, Umgebung und Raumtiefe sowie der unabhängige Vergleich aller 51 Fensteransichten erfolgreich. Nach dem Zusammenführen der parallelen Akku-/Pin-Änderung diese Prüfungen und zusätzlich den Schlafmodus erneut erfolgreich geprüft. Echte Pico-2-Übersetzung und physischer Boardtest für diese Änderung stehen aus.

## Fensterausblick: Dämmerung und Nacht konkretisiert (25. November 2026)

- Ausdrücklicher Wunsch der Nutzerin: Dämmerung mit weißem Baum und dunkler Kontur, ohne Mond; nachts schwarzer Baum mit weißem Rand, Mond erst bei Nacht. Die vorhandene Baumform und die Tagdarstellung bleiben erhalten. Der ältere Fensterzeichner hatte den Mond auch bei Dämmerung eingeblendet; diese Bedingung ist ebenfalls korrigiert.
- Die Kontrastvarianten werden aus den vorhandenen Masken vorberechnet und nicht bei jedem Bild neu gesucht. `make-zimmer-v2.py` erzeugt auch die aktuellen 51 Fensterreferenzen. Direkter Vergleich unter `entwuerfe/zimmer/grillme-2026-11-25/v2/board/fenster-tag-daemmerung-nacht.png`.
- Geprüft: drei betroffene PC-Prüfprogramme ohne Fehler, Syntaxprüfung ohne Warnungen, alle 51 Fensterzustände pixelgenau gegen aktuelle PNG-Referenzen; die 17 Tagesansichten bleiben unverändert. Eigene Prüfungen sichern Baumfüllung/-rand aller vier Jahreszeiten sowie den fehlenden Dämmerungsmond auch im älteren Zeichner.
- Nach dem Zusammenführen mit den parallelen Stromspar-/Gassi-Änderungen zusätzlich alle elf PC-Prüfprogramme und den 51-Fenster-Vergleich erneut erfolgreich ausgeführt, ohne Warnungen.

## Graustufen: Anzeige umgebaut, Evi in Graustufen (25. November 2026)

- Neue Graustufen-Bilder der Nutzerin im Repository (`entwuerfe/evi/graustufen-vorschau/`, `entwuerfe/zimmer/graustufen/`). Nach Rückfrage entschieden: **Graustufen im Spiel**. Die alte Regel „nur Schwarz-Weiß“ gilt für das Waveshare nicht mehr (`CLAUDE.md` angepasst).
- Eingebaut: 8-Bit-Anzeige mit 16 Stufen und **alle 82 Evi-Bilder in Graustufen** (alle Lebensphasen, Emotionen, Momente, Garten-Posen, Ei). Alle elf PC-Prüfprogramme ohne Fehler. **Am Board noch nicht ausprobiert, echte Übersetzung hier nicht möglich.**
- **Noch Schwarz-Weiß** und offen: Zimmer/Fenster/Möbel (die neuen Zimmerbilder sind fertige Gesamtbilder mit eingebackenen Möbeln und vertragen sich nicht mit „Zimmer einrichten“; es fehlen Einzelbilder von Wand, Boden, Fenster und Möbeln), Fundstücke (Bildaufträge in `entwuerfe/sammlung/bildauftraege-graustufen.md`), Symbole.
- Die „Gegenstände“, von denen die Nutzerin sprach, liegen noch nicht im Repository.
- Einzelheiten in `software/schritt-10/README.md`.

## Großer Weihnachtsbaum, Evi schläft im Tierbett (25. November 2026)

- Laut Nutzerin funktionieren Einrichten und der Gang zum Sideboard am Board gut. Neue Wünsche, umgesetzt (nur Waveshare): Weihnachtsbaum vom Boden bis an den oberen Zimmerrand; Evi geht zum Schlafen ins Tierbett (vorhandenes Schlafbild, deckend, vorderer Korbrand davor, keine neue Zeichnung, keine Spielwirkung). Das Kerzentablett liegt jetzt richtig hinter dem Baum (stand zuerst davor). Evis Größe bleibt, wie sie ist (Entscheidung der Nutzerin). Einzelheiten in `software/schritt-10/README.md`. Alle elf PC-Prüfprogramme ohne Fehler. **Am Board noch nicht ausprobiert.**
- Evis Größe gemessen: Erwachsene 68 von 240 Punkten (28 %), Baby 22 %, Teenager und Oma 27 %. Ob sie größer werden soll, entscheidet die Nutzerin.
- Wunsch für später: mehr Gegenstände im Zimmer, auch an der Wand (z. B. kleine Bilder). Funde ins Regal geht zu Weihnachten nicht, weil dann statt des Regals der Baum steht.

## Schaltplan ausgewertet: Verstärker läuft immer, Akku-Schalter aus (25. November 2026)

- Die Nutzerin hat den Waveshare-Schaltplan geschickt. Ergebnis in [Stromverbrauch laut Schaltplan](Tamagotchi_Hardware_Cube.md#stromverbrauch-laut-schaltplan-25-november-2026): Der größte bekannte Dauerverbraucher ist vermutlich der **Lautsprecher-Verstärker APA2068**, der fest eingeschaltet ist (kein GPIO). Dazu Lade-Chip, Betriebs-LED und der Chip selbst.
- Per Programm geändert (Waveshare, nur Firmware): `BAT_EN` jetzt LOW (spart etwa 2,9 mA, der Akku-Schalter wird mit VBUS-Versorgung nicht gebraucht; `BAT_HALTEN` in `board.h`), Pull-down an den SD-Karten-Leitungen und der Akku-Taste aus (etwa 0,4 mA). Alle elf PC-Prüfprogramme ohne Fehler. **Am Board noch nicht ausprobiert.**
- Offen, Entscheidung der Nutzerin: Verstärker per Umlöten abschaltbar machen (Draht von R24/`SD` zu GP0).

## Tieferer Schlaf, Touch im Dunkeln aus, Gassi entfernt (25. November 2026, zweite Fassung)

- Nach der Messung (hell etwa 120 mA, dunkel 30–47 mA, schwankend) Wunsch der Nutzerin: Gassi gehen ganz entfernen, den Chip tiefer schlafen lassen, im Dunkeln Touch grundsätzlich aus. Umgesetzt, nur Waveshare:
  - **Gassi gehen ganz entfernt** (Schritte, Mittraben, Belohnung, Funde beim Spaziergang, Schrittzeilen; auch die Schüttel-Erkennung). Ältere Spaziergang-Funde bleiben. Neue Funde nur aus dem Garten, dort sind alle Arten erreichbar. Spielstand unverändert.
  - **Dunkel wacht der Chip nur noch alle 120 ms auf** (zu jedem Spielschritt) statt alle 20 ms. Der Lagesensor misst dann einmal; Umdrehen weckt weiter (nach etwa 0,5 s).
  - **Dunkel ist Touch aus** (Touch-Chip im Reset). Wecken mit Tasten, Umdrehen oder Ereignissen. Ohne gefundene Tasten bleibt Touch an.
- Alle PC-Prüfprogramme ohne Fehler und Warnungen, auch nach dem Zusammenführen mit dem Einrichtungsmodus (elf Programme und der Fenstervergleich). **Am Board noch nicht ausprobiert, Strom noch nicht gemessen.** Einzelheiten in `software/schritt-10/README.md`.
- **Am Board (25. November 2026, laut Nutzerin):** aufgespielt. `P` dunkel: `lcd=schlaeft touch=aus ton=aus takt=48MHz`, geschlafen 97 %. **Gemessen dunkel 43–50 mA** (gleiche Messweise, dieselben schon etwas verbrauchten Batterien). Also keine messbare Ersparnis gegenüber der ersten Fassung (30–47 mA). Der Chip schläft schon fast immer; der Rest kommt sehr wahrscheinlich von Bauteilen des Boards. Messwerte schwanken auch mit dem Ladezustand der Batterien (weniger Spannung, mehr Strom); künftig zusätzlich die Spannung der Batterien messen.
- Offen, falls es immer noch viel ist: Ein großer Teil des Stroms kommt vermutlich von Bauteilen des Boards, die das Programm nicht abschalten kann (`PWR`-LED, Spannungsregler, Lade-Chip, evtl. Verstärker). Der Schaltplan von Waveshare war aus der Cloud-Sitzung nicht abrufbar.

## Grillme-Zimmer V2 und erster Einrichtungsmodus (25. November 2026)

- Die Nutzerin hat die korrigierte Fenstervorschau mit vollständigem Bäumchen, Hügeln sowie großer Sonne und Wolke bestätigt und anschließend den Einbau sowie die Umsetzung weiterer klar beschlossener Grillme-Punkte und das Hochladen beauftragt. Nur Waveshare, entsprechend der geltenden Projektregel.
- Das geprüfte V2-Grafikpaket ist jetzt eingebaut: 51 Kombinationen, drei Tageszeiten, Regen/Schnee in getrennten Ebenen, Schneefall nur im Winter, ausschließlich Schwarz-Weiß. Fensterrahmen und Kreuz liegen davor. Originalzeichnungen von Evi, bisherige Bedürfnisse und Jahreszeitenregeln bleiben erhalten. Weihnachtsbaum, Fenstersterne und Kerzentablett bleiben im bisherigen Zeitraum erhalten.
- Neu im Menü: **Zimmer einrichten**. Tierbett, Sideboard, Regal und Teppich verschieben oder ins Inventar räumen; A bestätigt, B/START verwirft die Platzierungsvorschau, RESET verwirft und schaltet aus. Drei Tiefenzonen, geschützte vordere Spielfläche, Wandbelegung hinten und Verdeckung durch mittlere Möbel. Navigationsprüfung hält Fenster, Bett und Sideboard erreichbar. Evis Gänge folgen den aktuellen Möbelpositionen.
- Kataloggrundlage für Breiten-/Höhenbelegung und Ablagekapazität: Sideboard zwei Plätze, Kerzentablett benötigt beide. Endgültiges Slotraster, weitere Gegenstände, Käufe und Möbelwirkungen bleiben offen. Vier-Pixel-Schritte sind eine konfigurierbare Prototypentscheidung, kein neuer endgültiger Rasterbeschluss.
- Möbel/Inventar separat mit Version und Prüfsumme ab EEPROM 3328; vorhandene Spielstände und der Freundschafts-/Geburtstagsbereich ab 3584 bleiben erhalten. Weitere Entscheidungen zu öffentlichem Park, gekauftem Garten und romantischen Beziehungen sind dokumentiert, nicht mit erfundenen Preisen oder Regeln vorweggenommen.
- Prüfung: alle elf PC-Prüfprogramme ohne Fehler und Warnungen; zusätzlich alle 51 Firmware-Fenster pixelgenau gegen das bestätigte Grafikpaket verglichen. Einrichtung, Inventar, Abbrechen, RESET, freie Wege und getrennte EEPROM-Bereiche geprüft. Bildschirmbilder und Einbau-Prüfbericht liegen im V2-Entwurfsordner.
- Einzelheiten, Bedienung und Prüfung in [Einrichtung_und_Grillme.md](Einrichtung_und_Grillme.md). Echte Pico-2-Übersetzung und physischer Boardtest stehen aus; Arduino-Werkzeug hier nicht vorhanden.

## Schlafmodus eingebaut, Helligkeit 50 % (25. November 2026)

- Auf Wunsch der Nutzerin („Ja bau den Schlafmodus ein, Helligkeit auf 50 %“), nur Waveshare. Neue Datei `software/schritt-10/evi-handy/schlaf.h`. Der Chip schläft zwischen den Abfragen (dunkel bis zur nächsten Messung des Lagesensors, höchstens 20 ms; hell 2 ms). Dunkel läuft er nach 3 s mit 48 statt 150 MHz, und der Bildschirm-Chip schläft. Der Ton-Chip bekommt nur bei Tönen Takt. Tasten, Antippen, Gassi und Piepen gehen weiter. Jede Stelle ist einzeln abschaltbar. `P` im Seriellen Monitor zeigt, wie viel der Chip geschlafen hat. Einzelheiten in `software/schritt-10/README.md`.
- Geprüft in der PC-Nachbildung mit neuem `schlaf.cpp`: alle zehn Prüfprogramme ohne Fehler und Warnungen. Dabei wurde auch ein schon vorher fehlschlagender Launenwunsch-Test in `kuscheln.cpp` berichtigt (der Test hatte die offene Geburtstagsfrage und eine Zeitangabe falsch gesetzt, das Spiel selbst war richtig). **Echte Pico-2-Übersetzung in dieser Sitzung nicht möglich, am Board noch nicht ausprobiert.** Beim Aufspielen auf Fehlermeldungen achten (benutzt `set_sys_clock_khz`, `clock_get_hz`, `i2s.end()`).
- Erwartung (nicht gemessen): dunkel vorher etwa 50 mA, jetzt hoffentlich etwa 5–15 mA, hell mit 50 % etwa 60–90 mA. Strom messen, sobald die Nutzerin kann.
- **Am Board (25. November 2026, laut Nutzerin):** Übersetzt und aufgespielt. `P` im Seriellen Monitor (USB, COM8): hell `takt=150MHz lcd=wach ton=aus`, geschlafen 28 % (enthält den Start; Zeichnen kostet Rechenzeit). Dunkel nach RESET: `licht=0% lcd=schlaeft ton=aus takt=48MHz`, geschlafen 91 %. Der Serielle Monitor funktioniert auch mit 48 MHz. Wecken mit Taste und Touch, Ton und Ball laut Nutzerin in Ordnung.
- **Gemessen am 25. November 2026** (PCW01A, Bereich A⎓ 200m, drei AAA, Multimeter über den leeren vierten Platz, also bei etwa 4,5 V): **Bildschirm an etwa 120 mA** (beim Uhrstellen, 50 % Licht), **Bildschirm aus etwa 30 mA** (10 s nach RESET; bei einer zweiten Messung schwankte es zwischen 30 und 47 mA). Das sind etwa 0,54 W und 0,14 W. Mit vier frischen AAA (grob 5 Wh) reicht das nur dunkel für etwa 1,5 Tage, hell für etwa 9 Stunden. Weniger Ersparnis als erhofft (geschätzt waren 5–15 mA dunkel). Ein Wert von vor dem Schlafmodus wurde nicht gemessen. Messtipp: Spitzen festkleben, beim Uhrstellen fünfmal A drücken. Nächste Schritte noch offen (siehe Antwort an die Nutzerin): herausfinden, wohin die 30 mA gehen, tieferer Schlaf, Akku statt AAA.

## Batterien nach weniger als einem Tag leer (25. November 2026)

- Laut Nutzerin ging der Bildschirm aus und flackerte danach an und aus: Die vier AAA waren leer, obwohl der Bildschirm seit dem Vorabend meist mit RESET ausgeschaltet war. Die Batterien waren laut Nutzerin neu.
- Ursache laut Code: „Bildschirm aus“ schaltet nur die Hintergrundbeleuchtung ab. Der Chip läuft ohne Pause mit vollem Takt weiter und fragt ständig Touch, Lagesensor und Tasten ab. Bildschirmsteuerung, Touch-Chip, Verstärker und `PWR`-LED bleiben ebenfalls an. Grob geschätzt (nicht gemessen) braucht das Board so etwa 40–60 mA, mit Licht etwa 100–150 mA. Vier AAA reichen damit für ungefähr 10–15 Stunden, mit Licht für etwa 6–10 Stunden.
- Für eine Woche mit 4 × AAA dürfte das Gerät im Mittel nur etwa 5 mA brauchen. Dafür muss der Chip bei dunklem Bildschirm wirklich schlafen und durch Taste, Touch oder Uhr geweckt werden. **Eingebaut am 25. November 2026, siehe oben.** Den echten Strom mit dem Multimeter messen: mA-Bereich, in Reihe zwischen Batteriefach und Pololu, einmal mit Licht und einmal ohne.
- Flackern bei leeren Batterien: Das Board startet, die Spannung bricht ein, es startet neu. Sofort eine AAA herausnehmen, sonst kann der Spielstand beim Speichern beschädigt werden (nur eine Kopie mit Prüfsumme).

## Beschlossen: Freundschaft, Fragen und Plaudern (Fragerunde vom 25. November 2026)

Ergebnis einer Fragerunde mit der Nutzerin (nur Waveshare). Sie wünscht sich mehr als Pflege: Fortschritt (A), Gestalten (B), Sammeln (D), Gespräche (E). Grundgerüst ist eine **verborgene Freundschaft**, die nach und nach Dinge freischaltet. Die Texte schreibt Claude direkt; die Nutzerin will sie vorher **nicht** lesen (Überraschung).
- **Fragen:** Evi fragt von selbst mit Piep zu wechselnden Uhrzeiten, als Baby etwa 4-mal am Tag, als Teenager 2-mal, als Erwachsene und Oma einmal. Jede Frage bleibt etwa 5 Minuten stehen. Jede Altersstufe hat eigene Fragen (Baby kindlich, Teenager pubertär, Oma mit Oma-Interessen). Beantwortet wird per Auswahl, kein Freitext. Nicht beantwortet kostet nichts.
- **Plaudern** (Menü): Evi fragt nichts, sondern erzählt von sich, was sie mag, nicht mag und denkt, zufällig ausgewählt. Je höher die Freundschaft, desto persönlicher.
- **Jede Evi ist anders:** Vorlieben und Meinungen werden beim Schlüpfen ausgewürfelt (aus dem Schlüpfzeitpunkt, braucht keinen Speicher). Jede Evi lernt die Nutzerin neu kennen. **Bleibt für immer:** Name „Maggie“ (nicht „Maggie1Maus“) und der Geburtstag. Der Geburtstag wird einmal am Gerät per Steuerkreuz abgefragt und nur auf dem Board gespeichert, nicht im öffentlichen Repository.
- **Evi greift Antworten auf**, freut sich über Gemeinsamkeiten („Wir sind gleich!“), und das „Weißt du noch?“-Spiel fragt ab, was Evi erzählt hat.
- **Freundschaft:** verborgen, 5 Stufen (schüchtern, bekannt, vertraut, Freunde, beste Freunde), jede mit spürbaren Zeichen (Begrüßung, Name, Geschenke, Geheimnisse, Träume). Beste Freunde spätestens als Erwachsene erreichbar. Sinkt durch Beleidigtsein, lange leere Balken und Krankheit. **„Beste Freunde“** ist ein großer, nicht zu verpassender Moment, kann verloren gehen („Ich dachte, wir wären beste Freunde …“) und muss dann neu verdient werden.
- Je Anlass möglichst fünf zufällige Sätze. Später als eigene Vorhaben: Kunststücke, Zimmer gestalten, Album früherer Evis.
- **Eingebaut am 25. November 2026** in `software/schritt-10/evi-handy/freunde.ino` (100 Fragen, 100 Meinungen zum Plaudern, 5 Stufen, Geschenke, „Weißt du noch?“, beste Freunde, Geburtstagsfeier mit Lied). Einzelheiten und Zahlen in `software/schritt-10/README.md`. Für Pico 2 ohne neue Warnungen übersetzt, **am Board noch nicht ausprobiert**. Die vorhandene Evi startet schüchtern (Freundschaft 0). Der Geburtstag steht absichtlich nicht im Repository.

## Waveshare: Launenwünsche mit Folgen, Licht aus nach dem Aufwecken (25. November 2026)

- Wunsch der Nutzerin: Launen und Wünsche sollen Folgen haben, und zu jedem Anlass soll es fünf zufällige Sätze geben. Jede Tageslaune hat jetzt ihren Wunsch (verspielt: spielen, verschmust: kuscheln, verfressen: Keks, verträumt: zusammen aus dem Fenster schauen). Sie wartet 45 s. Erfüllt: +10. Verpasst: −5 und traurig. Zwei verpasste hintereinander: 15 Minuten beleidigt (keine Spiele, Kuscheln wirkt nicht, kleine Regenwolke). Versöhnen mit Keks/Essen oder dreimal streicheln. Wünsche kommen nur, wenn in den letzten 10 Minuten jemand bedient hat. Neue Datei `software/schritt-10/evi-handy/launen.ino`, Einzelheiten in `software/schritt-10/README.md`. Handy-App unverändert.
- Nachts geweckt: „Licht aus“ im Menü schickt Evi sofort wieder schlafen. Ohne Bedienung schläft sie wie bisher nach 5 Minuten wieder ein.
- Die Schmollpose mit verschränkten Vorderpfoten erscheint ab dem zweiten verpassten Wunsch. Eigene Zeichnungen für Baby, Teenager, Erwachsene und Oma unter `entwuerfe/evi/emotionen/`, eingebunden über `tools/make-assets.ps1`. Alle vier Display-Vorschauen geprüft und für Pico 2 ohne Warnungen übersetzt.
- Für Pico 2 ohne neue Warnungen übersetzt. Die PC-Nachbildung ist angepasst, konnte auf dem Haupt-PC aber nicht laufen. **Am Board noch nicht ausprobiert.**
- Später, als Teil 2 besprochen: eine feste Persönlichkeit pro Evi (z. B. wasserscheu, Langschläferin, Naschkatze, Sportlerin) mit Vorlieben und Abneigungen, die Folgen haben.

## Evi läuft mit Batterien (24. November 2026)

- Die Nutzerin hat bestätigt, dass Evi mit vier AAA im originalen Batteriefach startet und läuft. Am Ausgang des Pololu S9V11F5 wurden rund 5 V gemessen. Die Versorgung geht über die 1N5819-Diode an den zwölfpoligen Stecker des Waveshare. [Verdrahtung im Hardware-Aufbau](Tamagotchi_Hardware_Cube.md#batteriebetrieb-im-retro-gamer-24-november-2026)
- **Vor dem Anschluss an den PC eine AAA herausnehmen** und warten, bis die `PWR`-LED ausgeht. Für den Wechsel zurück zum Batteriebetrieb zuerst USB-C abziehen und dann die AAA wieder einsetzen.
- Als Nächstes die Module und Kabel im Gehäuse befestigen, den Lautsprecher platzieren und die Passform der Rückschale prüfen.

## Kompaktes Zimmer und allgemeine Raumtiefe eingebaut (24. November 2026)

- Nutzerin bevorzugt den gemütlicheren ersten Zimmerentwurf und hat die Umsetzung mit „Ok dann leg los“ beauftragt. Der kompakte Raum ist jetzt die Zimmeransicht (`entwuerfe/zimmer/kompakt/`); Proportionen und Möbelgrößen des ersten Entwurfs erhalten. Weihnachtsfassung passend zum selben Zimmer, Baum auf dem Boden statt rechter Tisch/Pflanze/Regal, Fenstersterne und Kerzentablett im bestehenden Zeitraum 18.11.–5.1.
- Gemeinsamer Renderer `raumtiefe.h/.ino` für Zimmer/Garten: Füße als Bodenanker, weiche Größenänderung von 65 % hinten bis 100 % vorne, altersabhängige Grundgrößen und ursprüngliche Posen/Zeichnungen erhalten. Vordergrundstempel lassen beim Verdecken die Originalpixel der Möbel/Beetkante stehen. Sichtbare Figurteile bleiben antippbar, verdeckte Teile reagieren nicht.
- Zimmer: zusätzliche Bodenposition und begehbare Wege, Fensterbesuch seitlich am Sideboard vorbei und dahinter. Kuscheln hält die aktuelle Tiefenposition, danach geht es zum vorderen Teppich zurück. Sideboardlauf skaliert mit. Minispiele behalten ihre separaten Spielkoordinaten.
- Garten: auf der freien Wiese zur Suchstelle, dann kleiner in die Tiefe zum Baum/Busch/Beet. Nach Untersuchen wieder herauskommen, Ergebnis weiter vorne zeigen, anschließend auf die freie Wiese zurück. Pfoten hinter der vorderen Beetkante verdeckt.
- Neuer Fensterausblick aus der echten Gartenansicht, damit Saison/Wetter/Tag-Nacht zusammenpassen. Besuch auf die neue Scheibe abgebildet, Rahmen/Kreuz bleiben davor. Keine Änderungen am Spielstandformat oder den Fundregeln; Handy-App unverändert.
- Details in [Raumtiefe_und_Zimmer.md](Raumtiefe_und_Zimmer.md), Generator `make-zimmer-tiefe.py`. Alle neun PC-Prüfprogramme ohne Fehler/Warnungen und echte Pico-2-Übersetzung erfolgreich: Programm 486360 Bytes (11 %), globale Variablen 33380 Bytes (6 %, ohne Stack/Heap). Am echten Board noch prüfen.

## Bestätigten Garten-Gesamtentwurf eingebaut (24. November 2026)

- Nutzerin fand die erste Gartenfassung lieblos/zusammengewürfelt (Strichstärken, isolierte Sonne/Wolke). Den neuen natürlichen Gesamtentwurf bestätigte sie anschließend ausdrücklich mit „Jaaaaaaaa 100%. Das sieht gut aus“. Diese Gestaltung ist jetzt in der Waveshare-Firmware eingebaut.
- Fünf zusammenhängende Gartenebenen (Frühling/Sommer/Herbst/Winter/Weihnachten) unter `entwuerfe/garten/v2/`. Dieselbe Perspektive, verbundene Beete, natürlicher Baum/Busch, Trittsteinweg, kleine runde Lichter. Zusätzlich 17 passende Einzelobjekte in `v2/einzelobjekte/`. Neue Szene wird nicht erneut aus lose nebeneinanderstehenden Symbolen zusammengesetzt.
- Ruhiger Himmel ohne Sonne-/Wolkensymbole; Wetter als atmosphärische Himmelsschichten, fallender Regen/Schnee im Himmel und auf der offenen Wiese, nachts Sterne und helle Konturen. Jahreszeiten/Wetter/Sonnenzeiten identisch zum Fenster; Weihnachten weiterhin 18.11.–5.1.
- Evi kleiner zur Gartenperspektive, Fundsuche beim Beet jetzt an der passenden Stelle. Alle zwölf Suchposen und alten Zeichnungen unverändert. Fundregeln, Metadaten und Version-3-Spielstand unverändert.
- Generator `make-garten-szene.py` → `gartenlandschaft.h`; gespeicherte Quellen, Bildaufträge und aktuelle Bildschirmansichten unter `entwuerfe/garten/v2/`. Erste Gartenfassung und bestätigter Entwurf bleiben zur Nachvollziehbarkeit erhalten. Handy-App unverändert. Acht PC-Prüfprogramme und echte Pico-2-Übersetzung erfolgreich: 444328 Bytes Programm (10 %), 33288 Bytes globale Variablen (6 %, ohne Stack/Heap). Spielstand 3288 Bytes unverändert. Am echten Board noch testen.

## Garten und Evi-Suchposen umgesetzt (24. November 2026)

- Garten als eigene Ansicht im Waveshare-Menü: eingezäunt, Gartentür, Sternenlichterkette, Baum, Busch, Beet. Vier saisonale Varianten der Pflanzen; dasselbe Wetter und Tag/Nacht wie im Fenster, bewegter Regen/Schnee auch nachts. Kranz und Schneemann im bisherigen Weihnachtszeitraum 18.11.–5.1. Einzelbilder und gerenderte Gesamtansichten unter `entwuerfe/garten/`.
- Zwölf neue Zeichnungen unter `entwuerfe/evi/garten/`: schnuppern, mit der Pfote untersuchen, Fund stolz zeigen, jeweils Baby/Teenager/Erwachsene/Oma. Nutzerin bat ausdrücklich um weitere Zeichnungen und Berücksichtigung der „Generationen“; umgesetzt für die bestehenden Lebensphasen. Alte Zeichnungen bleiben unverändert. Deckende Körper; Fund separat auf den Pfoten. Keine neue Verpaarungsmechanik.
- Gemeinsame Suche: Stelle wählen, A schickt Evi los, beim Fragezeichen A oder Stelle antippen zum Helfen, Ergebnis bleibt bis A; B/START heim. Je Stelle ein Versuch am Tag, mindestens eine von drei ergiebig, weitere 20 % als Startwerte. Echte Spaziergänge bleiben zusätzlich und haben ihr eigenes tägliches Fundlimit.
- Neue Funde mit Gegenstandsart, Datum, Quelle, Gartenstelle, damaliger Jahreszeit und Spielwetter. Erinnerungssatz im Sideboard aus gespeicherten Daten. Reales Spaziergangswetter bleibt unbekannt. Version-3-Spielstand übernimmt Version 1/2 verlustfrei, 3288 Bytes von 4096 EEPROM-Bytes.
- Gesamte PC-Prüfung inklusive neuem `garten.cpp` erfolgreich; echte Pico-2-Übersetzung: 392880 Bytes Programm (9 %), 33288 Bytes globale Variablen (6 %, ohne Stack/Heap). Am echten Board noch testen.
- Details und Startregeln in [Garten_und_Fundstuecke.md](Garten_und_Fundstuecke.md) und `software/schritt-10/README.md`. Generator `make-garten.py`, Bildaufträge unter `entwuerfe/garten/bildauftraege.json`. Handy-App unverändert. Stitch weiterhin offen.

## Fundstückbilder und Fundauswahl erweitert (24. November 2026)

- Nutzerin hat elf Motive und die kleine Figur statt des Eis für Spyro freigegeben; Bilder zeichnen, hochladen und in den Code integrieren.
- Neu erfolgreich gezeichnet und eingebaut: Micky-Knopf, Arielles Muschel, Triforce-Anhänger, Animal-Crossing-Spiralfossil, Junimo-Figur und ein eigenständiger kleiner Gartendrache. Bei Spyro und Stitch lehnte der Bildgenerator die Erzeugung ohne konkreten Grund ab. Nutzerin erlaubte danach einen ähnlichen Drachen-Nachbau; umgesetzt als eigener Gartendrache, nicht als Spyro. Stitch bleibt ohne Ersatz offen.
- Aktuell zehn verfügbare Fundarten, darunter die vier bisherigen unveränderten Bilder. Alle zehn sind beim bestehenden Gassigehen erreichbar und im Sideboard mit Name und Datum sichtbar. Die alten Fund-IDs bleiben unverändert; das Spielstandformat muss nicht geändert werden.
- Bilder in `entwuerfe/sammlung/`, vollständige Bildaufträge in `bildauftraege.json`. `make-sammlung.py` erzeugt `fundbilder.h`; die schwarzen Linien und weißen Innenflächen der neuen Figuren werden als getrennte 1-Bit-Flächen abgelegt.
- Zusätzliche Prüfungen: jede Art zeichnen, speichern/laden und über die Fundauswahl erreichen; die alten vier Bilder bleiben bytegleich. Gartenansicht und neue Garten-Suchabläufe sind inzwischen umgesetzt (siehe oben).

## Konzept: gemeinsamer Garten und zehn Fundstückarten (24. November 2026)

Neue Entscheidungen und offene Gestaltungsvorschläge stehen in [Garten_und_Fundstuecke.md](Garten_und_Fundstuecke.md). Gemeinsamer eingezäunter Garten statt Park, auch vom Sofa aus; zusätzliche Funde bei echten Spaziergängen. Gegenstandsart und Fundtag verbindlich festhalten, mit passenden Erinnerungssätzen. Gesamtansicht plus einzelne Gartenobjekte und Jahreszeiten vorsehen. Wetter muss im Garten sichtbar sein und zum Fensterausblick passen. Vier vorhandene Fundstücke sowie Micky-Knopf, Arielles Muschel und Stitch-Anhänger sind bestätigt. Allgemeine Gaming-Motive gestrichen; weitere Motive aus Spyro, Zelda, Animal Crossing und Stardew Valley besprechen, erst danach zeichnen. Mit allen vier Spielreihen wären es elf statt ungefähr zehn Fundstückarten; Umfang noch offen. Federkitzeln, Zapfenspielen und Sammelsets gestrichen; Generationen/Erbstücke vorerst zurückgestellt. **Historischer Konzeptstand; Garten und neue Evi-Zeichnungen sind inzwischen umgesetzt, siehe oben.**

## Waveshare: Kuscheln, kleine Uhrzeit, trüber Regenhimmel, kürzeres Menü (24. November 2026)

Wünsche der Nutzerin, nur auf dem Board (Einzelheiten in `software/schritt-10/README.md`):
- **Menü:** „Uhr stellen“, „Hilfe“ und „Name ändern“ entfernt (`name-hilfe.ino` gelöscht). Die Uhr fragt weiterhin beim Einschalten, wenn sie nicht läuft.
- **Uhrzeit** klein oben an der Wand zwischen Fenster und Evi, auch bei Licht aus lesbar.
- **Regen:** Himmel am Tag trüb gepunktet wie in der Dämmerung; schwarze Regenwolke und fallender Regen bleiben (Regenstriche mit hellem Rand, damit man sie sieht).
- **Touch = Kuscheln statt Tasten:** Evi antippen oder streicheln. Wach: glückliches Bild, wiegen, Herzen, ein lieber Satz, +5 Stimmung (verschmust +8), höchstens einmal pro Minute. Schlafend kuschelt sie sich nur an, das Ei wackelt. Bei Besuch und Gassi nur Herzen. Fehlen die Tasten (MCP23017 nicht gefunden), gelten als Notlösung die drei Drittel wie früher.
- **Tageslaune „verschmust“** bedeutet jetzt etwas: mehr Wirkung beim Kuscheln, und Evi fragt oft von selbst „Kuschelst du mit mir?“ (Kuschelwunsch, erfüllt +10). Die anderen Launen (verspielt, verfressen, verträumt) beeinflussen weiter nur, welche Momente und Gedanken öfter kommen.
- Prüfung: PC-Nachbildung mit neuem `kuscheln.cpp` (nachgebildeter Touch-Chip), alle Prüfungen ohne Fehler und Warnungen. **Echte Pico-2-Übersetzung diesmal nicht möglich** (Arduino-Werkzeuge in der Cloud nicht ladbar). Am Board ausprobieren; trifft das Antippen Evi nicht, im Seriellen Monitor `T` senden (zeigt Tipper und Evis Lage).
- Offen, nur als Idee: den anderen Tageslaunen ähnlich eine Bedeutung geben (verspielt fragt nach einem Spiel, verfressen nach einem Keks, verträumt schaut öfter aus dem Fenster).

## Waveshare: deckendes Baby und bewegter Niederschlag (24. November 2026)

- Rückmeldung der Nutzerin: Beim Laufen scheint der Dielenboden durch Evi. Die Baby-Laufzeichnung hatte einen durchsichtigen Bauch. Nur die Deckmaske in `momente.h` ist gefüllt, die schwarzen Linien und die Größe sind unverändert; auch beim gespiegelten Laufen nach rechts. Die Reparatur ist in `tools/make-momente.mjs` enthalten (`--repair-running-mask` funktioniert ohne Browser).
- Wunsch der Nutzerin: Regen und Schnee sollen draußen sichtbar fallen. Die vorhandene Bewegung wird jetzt etwa alle 40 ms statt nur alle 120 ms gezeichnet; Schnee pendelt sanft seitlich. Niederschlag ist auch nachts sichtbar (helle Punkte auf dunklem Himmel). Er bleibt auf die Fensterscheiben begrenzt, hinter Baum, Hügeln und Fensterkreuz. Schnee weiterhin nur im Winter; Spielregeln behalten ihren Takt.
- Prüfung: echte Übersetzung für Pico 2 mit arduino-pico 6.4.0; zusätzliche Prüfungen für Baby-Füllung in beiden Laufrichtungen, Wetterbewegung tags/nachts, Begrenzung aufs Fenster und Bildschirm aus. Am echten Board noch ausprobieren.

## Sammlung im Sideboard (24. November 2026)

- Die Nutzerin möchte Fundstücke von draußen und Erbstücke sammeln. Stil wie das Zimmer: **Gemütlich & schlicht**. Vier neue Fundstückbilder: glatter Stein, Blatt, Feder und Tannenzapfen (`entwuerfe/sammlung/`, verkleinert und schwarz-weiß in `fundbilder.h`, Werkzeug `make-sammlung.py`).
- Im Menü **Sideboard öffnen**: Evi läuft zum Sideboard und steht kurz mit ihrer vorhandenen Rückansicht davor. Danach eine eigene Ansicht statt des Zimmers. Je ein Fundstück mit Bild, Name, Fundort, Datum und Generation. ◀/▶ oder ▲/▼ blättern; B/A/START schließen, Touch: links zurück, Mitte schließen, rechts weiter. Beim Schließen läuft Evi zurück. Schlafende Evi, Ei und Grab bleiben beim Betrachten ungestört. RESET schließt und schaltet aus.
- Aktuell **keine Fundstücke oben auf dem Sideboard ausstellen**: Dort stehen die Kerzen. Die Sammlung wird im Sideboard aufbewahrt.
- Auf Wunsch schon beim Gassigehen: ab 100 Schritten erster Fund sicher, danach 40 % je ausreichendem Spaziergang, höchstens ein Fund pro Tag (Startwerte). Maximal 256 Einträge; volle Sammlung überschreibt nichts. Funddatum und Generation bleiben erhalten. Beim nächsten Ei werden ältere Gegenstände als Erbstücke angezeigt.
- Spielstand Version 2 speichert die Sammlung; vorhandene Version-1-Spielstände werden mit Name, Münzen, Bedürfnissen und Toneinstellung übernommen. Persönlichkeiten und Park-Funktionen bleiben für eine andere Sitzung offen.

## Waveshare: Schütteln entfernt (24. November 2026)

Laut Nutzerin aufgespielt und funktioniert. Auf ihren Wunsch reagiert Evi nicht mehr auf Wackeln/Schütteln (kein Schwindel, keine Sätze dazu). Gassi gehen, Kippen, Aufs-Gesicht-Legen, „Wie war dein Tag?“ und die neuen Spiele bleiben.

## Waveshare: nach dem ersten Test am Board (24. November 2026)

Rückmeldung der Nutzerin nach dem Aufspielen: Bewegung viel zu empfindlich (Schritte beim Stillhalten und Kippen, Schwindel beim Ablegen, Wackeln ohne Reaktion); Weihnachtsdeko zu klein; Baum nicht auf den Tisch.
- Bewegungserkennung neu und strenger, Maßstab des Sensors wird selbst gelernt; `L` im Seriellen Monitor zeigt die Sensorwerte. Einzelheiten in `software/schritt-10/README.md`.
- Weihnachten: Baum groß auf dem Boden, Tisch dann weg; Sterne und Kerzen doppelt so groß.
- **Bilder eingebaut (24. November 2026):** `entwuerfe/umgebung/fenster-sterne.png` und `kerzentablett.png` ersetzen auf dem Waveshare Claudes Platzhalter (über `tools/make-umgebung.py`): Girlande über dem Fenster, Kerzentablett groß auf dem Sideboard. Am Board noch nicht angesehen.
- Am Board erneut ausprobieren: Gassi gehen, Stillhalten, Kippen, Wackeln, Ablegen.

## Waveshare: neues Zimmer, Jahreszeiten, Wetter, Weihnachtsdeko (24. November 2026)

Wunsch der Nutzerin: Jahreszeiten wie in den Fensterentwürfen (Frühling ab 1. Februar, Sommer ab 1. Juni, Herbst ab 21. September, Winter ab 1. November), Weihnachtsdeko nach der Zeitangabe im Repo, die neuen Möbel; dazu Wetter zufällig je nach Jahreszeit, **Schnee nur im Winter**. Nur Waveshare. Einzelheiten in `software/schritt-10/README.md`.
- Möbel aus `entwuerfe/umgebung/` (Sideboard, Tisch mit Pflanze, Wandregal, Napf) und Dielenboden; Werkzeug `tools/make-umgebung.py` → `umgebung.h`.
- Wetter wechselt alle 3 Stunden, fest für jeden Tag; zu sehen am Tag und in der Dämmerung, nachts wie bisher Mond und Sterne.
- Weihnachten 18. November bis 5. Januar: Baum auf dem Tisch statt Regal und Pflanze, Sterne am Fenster, Kerzentablett auf dem Sideboard (derselbe Zeitraum für alles; die Nutzerin hatte ihn für Sterne und Kerzen noch nicht festgelegt).
- Fenstersterne und Kerzentablett waren zunächst klein selbst gezeichnete Platzhalter; seit 24. November 2026 sind die Bilder der Nutzerin eingebaut.
- Geprüft in der PC-Nachbildung (`umgebung.cpp`), Bilder angesehen. **Am Board noch nicht ausprobiert.**

## Waveshare: Evi spürt Bewegung, „Wie war dein Tag?“, zwei neue Spiele (24. November 2026)

**Beschluss der Nutzerin:** Die Handy-App wird **nicht mehr mitgeändert**, nur noch das Waveshare (außer sie fragt ausdrücklich; steht auch in `CLAUDE.md`).

Aus der Ideensammlung gewählt: Bewegungen (Gassi gehen, schütteln, kippen, aufs Gesicht legen), „Wie war dein Tag?“ und neue Spiele, die nicht Verstecken oder Ball bringen sind. Gemeinsame Rituale sonst vorerst nicht. Einzelheiten in `software/schritt-10/README.md`.
- **Lagesensor** QMI8658 (`lage.h`); Evi lernt beim Tastendrücken, wie das Gerät gehalten wird.
- **Gassi gehen:** Schritte zählen, Evi trabt mit; ab 100 Schritten +10 bis +25 Stimmung, 1 Münze je 500 Schritte, Sättigung und Sauberkeit je −5.
- **Schütteln:** Evi wird schwindelig (ohne Folgen). **Kippen:** Ein Ball rollt, Evi rennt hinterher. **Aufs Gesicht:** Bildschirm aus, nachts Licht aus; umgedreht wieder an mit Nickerchen-Gähnen oder Begrüßung.
- **„Wie war dein Tag?“** abends 18–23 Uhr einmal, Antworten Gut / So lala / Nicht so gut; bei „Nicht so gut“ tröstet Evi und singt.
- **Spiele:** Kekse fangen (◀ ▶ oder kippen) und Schnick, Schnack, Schnuck.
- **Touch durch die Plexiglasscheibe** funktioniert laut Nutzerin, mit etwas mehr Druck, auch für Menüauswahl.
- Geprüft in der PC-Nachbildung (`bewegung.cpp`, alle Prüfungen bestanden, ohne Warnungen). **Am Board noch nicht ausprobiert:** Sensor, Empfindlichkeit (Schwellen in `lage.h`), Schritte in der Tasche.

## Gestaltung: gemütlich & schlicht, Jahreszeiten, Wetter und Weihnachtsbaum (24. November 2026, beschlossen; Entwürfe hochgeladen)

- Verbindliche Gestaltungslinie für Evis Zuhause: **Variante A „Gemütlich & schlicht“**, passend zu Evis Schwarz-Weiß-Pixelstil. Tisch, Pflanze, Napf, Wandregal, Sideboard und Dielenboden als einzelne Elemente vorgesehen.
- Die vier Jahreszeiten am Baum vor dem Fenster sind bestätigt. **Fünf Wetterzustände**: Sonne, Sonne mit Wolken, Regen, bewölkt und Schneefall. Jede Jahreszeit ist mit jedem Wetter kombinierbar (20 Kombinationen), auch Winter mit Sonne oder Regen. Liegender Schnee und Schneefall sind unabhängig.
- Das Wetter wird simuliert, ohne Internet; echtes Wetter bleibt verworfen. Auswahl und Häufigkeit noch offen. Nacht mit Mond, Sternen und Sternschnuppen bleibt bestehen.
- **Weihnachtsbaum:** ersetzt jährlich ab **18. November** Wandregal und Pflanze; am **6. Januar** werden Regal und Pflanze wieder angezeigt. Damit sichtbar bis einschließlich 5. Januar, nach deutscher Ortszeit. Tischänderung nicht beschlossen.
- **Weitere Weihnachtsdeko ausgewählt:** Sterne am äußeren Fensterrahmen und ein Kerzentablett mit drei unterschiedlich hohen Kerzen, Tannenzweig und wenigen Beeren auf dem Sideboard. Der Ausblick bleibt frei. Zeitraum und Flackern noch offen; noch nicht eingebaut.
- Originale PNG-Vorschauen und Hinweise: [`entwuerfe/umgebung/README.md`](entwuerfe/umgebung/README.md). Dies ist ein Upload der Entwürfe und Planung; die neuen Möbel, Wetterzustände und der saisonale Tausch sind noch nicht im Spiel eingebaut.

## Ideen: Evi alltagsnah und interaktiver, ohne Internet (24. November 2026, Ideensammlung)

**Echtes Wetter: von der Nutzerin verworfen.** Der RP2350 auf dem Waveshare hat kein WLAN, das Board kann also kein Wetter abfragen. Nur in der Handy-App wäre es möglich (Wetterdienst, z. B. Open-Meteo); das wollte sie nicht ohne die echte Evi. Ein Wettersensor misst nur Tasche oder Zimmer und erkennt keinen Regen oder Schnee.

Stattdessen von Claude vorgeschlagen, alles ohne Internet, mit Uhr, Bewegungssensor (IMU) und Touch des Waveshare. **Weitere Ideen noch nicht beschlossen; Jahreszeiten und Weihnachtsbaum sind inzwischen wie oben beschrieben bestätigt:**
- **Kalender:** Adventskalender vom 1. bis 24. Dezember (ein Türchen am Tag, kleine Überraschung oder Deko fürs Zimmer), Feiertage (Weihnachtsbaum, Feuerwerk im Fenster an Silvester, Ostereier suchen, Kürbis an Halloween), Jahreszeiten am Baum vor dem Fenster (bunte Blätter, kahl, Blüten), Wochenende (länger schlafen), Geburtstag der Nutzerin.
- **Bewegung:** Gassi gehen (Schritte zählen, wenn Evi mit spazieren geht; auf dem Handy nur bei offener App), schütteln (Evi wird schwindelig), kippen (Ball rollt, Evi rennt hinterher), Gerät aufs Gesicht legen (Evi schläft ein).
- **Touch (nur Waveshare):** streicheln (seit 24.11.2026 als Kuscheln eingebaut), kitzeln, Evi folgt dem Finger.
- **Gemeinsame Rituale:** „Wie war dein Tag?“ am Abend (gut, so lala, schlecht; Evi freut sich mit oder tröstet), Zähneputzen zusammen mit 2-Minuten-Uhr, „Während du weg warst …“, Evi merkt sich, wann die Nutzerin meist vorbeischaut, und wartet dann am Fenster.
- **Spiele:** Verstecken, Ball bringen (aus der Sammlung vom 23. November).
- Claudes Empfehlung: zuerst Adventskalender (ab 1. Dezember), dann Gassi gehen und Hinlegen zum Schlafen, dann „Wie war dein Tag?“.

## Waveshare: Bildschirm bleibt an, RESET schaltet ihn (24. November 2026)

Wunsch der Nutzerin: Der Bildschirm des Boards geht nicht mehr nach 1 Minute aus. **RESET** (hatte keine Aufgabe) schaltet ihn aus und mit jedem Druck wieder an; Ereignisse und Besuch schalten ihn ebenfalls ein. Ein laufendes Spiel wird beim Ausschalten abgebrochen. **Am Board bestätigt (laut Nutzerin): RESET funktioniert, der Ball läuft flüssig, Momente gesehen (Spaziergang mit Hüpfen, Blick aus dem Fenster).** Stromverbrauch und Laufzeit mit vier AAA bei dauerhaft leuchtendem Bildschirm noch messen. Einstellbar über `DUNKEL_NACH_MS` in `software/schritt-10/evi-handy/evi-handy.ino`.

## Version 0.12: Evis Momente in jeder Lebensphase (24. November 2026, gebaut, noch nicht veröffentlicht)

Wunsch der Nutzerin: Evis neue Zeichnungen der Momente für **Baby, Teenager und Oma** (je 8: laufen, fenster, denken, singen, gaehnen, drehen, huepfen, staunen; `entwuerfe/evi/momente/`, hochgeladen am 24. November 2026) einbauen, „nicht mehr nur für die Erwachsene“.
- **Handy-App:** 24 Zeichnungen mit `handy-app/tools/prepare_art.py` aufbereitet (wie immer nur zugeschnitten, verkleinert, außen durchsichtig) und in `art.js` eingetragen. `MOMENTS.stages` ist jetzt Baby, Teenager, Erwachsen und Oma: Momente, Besuch, Begrüßung und Tageslaune gibt es in jeder Phase außer dem Ei. Hilfe angepasst, `build-art.ps1` verarbeitet die Momente jetzt für alle Phasen, Cache `kleines-zuhause-v12-momente`. 39 Prüfungen bestanden; im Browser mit vorgespulter Zeit gesehen: Baby hüpft und dreht sich, Teenager denkt, summt und dreht sich, Oma läuft.
- **Waveshare (Schritt 10):** `momente.h` enthält die Zeichnungen aller vier Phasen (im Maßstab der App), Momente, Begrüßung und Tageslaune ab Baby. In der PC-Nachbildung geprüft (Baby, Teenager, Oma mit eigenen Zeichnungen, Display ohne Abweichung), am Board noch nicht.
- **Besuch mit den Bildern der Nutzerin** (Wunsch: „auch für die echte Evi, nicht nur für die Handy-App“): `entwuerfe/evi/momente/schmetterling.png` (32 × 26) unverändert, `vogel.png` auf drei Viertel verkleinert (23 × 20), damit er in eine Fensterscheibe passt (`handy-app/tools/prepare_besuch.py` → `handy-app/dist/besuch/`). Weil es je nur ein Bild gibt, schwebt der Schmetterling statt zu flattern; der Vogel ist nach links gezeichnet und wird gespiegelt, damit er in Flugrichtung und beim Sitzen zu Evi schaut. In der App und auf dem Waveshare (`momente.h`: `BESUCH_FALTER`, `BESUCH_VOGEL`) eingebaut; App im Browser und Board in der Nachbildung angesehen.

## Version 0.11: Himmel, Tageslaune, mehr Besuch (23. November 2026, gebaut, noch nicht veröffentlicht)

Wunsch der Nutzerin: aus den Vorschlägen „mehr Besuch“ und „Tageslaune“; dazu soll es dunkel werden, wenn in Deutschland die Sonne untergeht, und das soll immer aktuell bleiben. Einzelheiten in `handy-app/README.md`.
- **Himmel im Fenster nach der echten Sonne:** hell, in der Dämmerung gepunktet, nachts schwarz mit Mond und Sternen. Neue Datei `handy-app/dist/sky.js` rechnet Sonnenauf- und -untergang für jeden Tag selbst aus, ohne Internet, mit Sommerzeit. Nichts muss nachgetragen werden; das funktioniert auch später auf dem Waveshare mit seiner Uhr. Ort: Mitte Deutschlands (in Aachen/Görlitz bis zu etwa 20 Minuten Unterschied). Geprüft gegen veröffentlichte Zeiten für Berlin, Frankfurt, München, Hamburg: höchstens 2 Minuten Abweichung. Das Zimmer selbst bleibt hell (Licht an), bis Evi schläft. Falls die Nutzerin ihre Stadt nennt, kann `SKY` darauf gestellt werden.
- **Mehr Besuch:** Neben dem Schmetterling landet ein Vogel auf dem Fensterkreuz (mit Zwitscherton). Im Dunkeln kommt stattdessen eine Sternschnuppe; Evi wünscht sich etwas, eine zweite fällt hinter die Hügel. Vogel und Sternschnuppe sind Pixelgrafik des Spiels; Evi nutzt die Zeichnungen „laufen“, „staunen“, „hüpfen“ und „denken“.
- **Tageslaune:** verspielt, verschmust, verfressen oder verträumt, fest für jeden Tag und jede Evi. Evi sagt sie bei der ersten Begrüßung des Tages; danach steht sie im Wechsel mit „Mitte öffnet das Menü“ unten in der Zeile. Je nach Laune kommen passende Momente öfter.
- **Guten Morgen** auch, wenn Evi um 7 Uhr bei offener App aufwacht.
- Geprüft: 39 Regelprüfungen (6 neue), auch in zwei anderen Zeitzonen. Im Browser mit deutscher Uhrzeit: Himmel um 12 Uhr hell, 16:40 Dämmerung, 18 Uhr Nacht, Übergang 16:55 → 17:05 bei offener App; Tageslaune nach der ersten Begrüßung und im Wechsel in der Zeile, nicht bei der zweiten Begrüßung; „Guten Morgen!“ beim Aufwachen; Vogel von Anfang bis Ende; Sternschnuppe mit Wunsch und zweiter Sternschnuppe. Keine Konsolenfehler.
- **Veröffentlichen** wie bei 0.9/0.10: von ChatGPT aus `git pull` und `handy-app/dist` unter dem bestehenden Projekt neu veröffentlichen.

## Version 0.10: Evi tut selbst etwas (23. November 2026, gebaut, noch nicht veröffentlicht)

Beschlüsse der Nutzerin vom 23. November 2026, zur Ideensammlung unten:
- Alle Ideen gefallen ihr; sie bleiben als Sammlung im Projektstand. Die **Schatzkiste noch nicht** umsetzen.
- Evis Bild darf **gespiegelt** werden.
- Häufigkeit: alle 5–10 Minuten etwas Kleines, 1–2 Mal am Tag etwas Besonderes. **Ton nur bei etwas Besonderem.**
- Umsetzen sollte Claude zuerst die zwei, drei einfachsten Sachen.
- Sie wünscht sich für das, was Evi tut, **eigene neue Zeichnungen**. Bis dahin werden die vorhandenen Bilder benutzt. **Erst einmal nur bei der erwachsenen Evi.**

Umgesetzt in 0.10, Einzelheiten in `handy-app/README.md`:
- **Begrüßung** beim Öffnen und nach mindestens 3 Minuten Abwesenheit: Evi hüpft und sagt zum Beispiel „Da bist du ja!“, „Guten Morgen!“ oder „Guten Abend!“.
- **Kleine Momente** alle 5–10 Minuten: Spaziergang, zum Fenster gehen, Gedankenwolke, summen, gähnen, im Kreis drehen. Kein Ton.
- **Besonderes** 1–2 Mal am Tag (9–21 Uhr, wartet bis 22 Uhr): Ein Schmetterling besucht Evi, mit Ton.
- Nur die erwachsene Evi, nur bei offener App, nicht im Schlaf und nicht bei Krankheit. Die Momente ändern keine Werte. Neue Datei `handy-app/dist/moments.js` ohne Browserabhängigkeit, damit sie später aufs Waveshare passt.
- **Bildaufträge** für 8 neue Zeichnungen der erwachsenen Evi: `entwuerfe/evi/momente/Bildauftraege.md` (laufen, hüpfen, gähnen, staunen, Fenster, denken, singen, drehen). Jede Zeichnung, die dort unter dem richtigen Namen liegt, übernimmt `build-art.ps1`, und das Spiel benutzt sie sofort. Die Erweiterung von `build-art.ps1` konnte in der Cloud nicht ausgeführt werden (kein PowerShell); beim ersten neuen Bild auf dem PC prüfen.
- Geprüft: 33 Regelprüfungen mit `npm test` (7 neue), auch in zwei anderen Zeitzonen. Im Browser mit vorgestellter Uhr bei 390 und 320 Pixeln: alle sechs kleinen Momente in 75 Minuten im richtigen Abstand, Schmetterling von Anfang bis Ende, Spiegeln beim Laufen nach rechts, Begrüßung („Guten Morgen!“, nach 5 Minuten ja, nach 1 Minute nein), Menü offen verhindert Momente, Füttern beendet einen Moment, Spielstart holt Evi sofort zurück, „Links oder rechts?“ funktioniert weiter, nachts nichts, Baby/Teenager/Oma ohne Momente, „Danke!“ nach Wunsch, „Bewegung reduzieren“. Mit Platzhalterbildern geprüft, dass neue Zeichnungen (laufen, hüpfen, gähnen) sofort benutzt werden. Keine Konsolenfehler.
- **Veröffentlichen** wie bei 0.9: von ChatGPT aus `git pull` und `handy-app/dist` unter dem bestehenden Projekt neu veröffentlichen.

**Version 0.10.1 (23. November 2026): die Zeichnungen der Nutzerin sind eingebaut.** Alle 8 Bilder aus `entwuerfe/evi/momente/` (laufen, hüpfen, gähnen, staunen, Fenster, denken, singen, drehen) sind 1:1 übernommen, nur zugeschnitten, verkleinert und außen durchsichtig. In der Cloud gibt es kein PowerShell. Deshalb gibt es jetzt `handy-app/tools/prepare_art.py`, einen Python-Nachbau von `prepare-art.ps1`. An den 7 Bildern der erwachsenen Evi geprüft: gleiche Größen, nur 0,03–0,05 % abweichende Randpixel. Bei „Drehen“ hat der Umriss eine 1-Pixel-Lücke am rechten Ohransatz, durch die das Innere durchsichtig wurde. Darum suchen beide Werkzeuge „außen“ jetzt mit einem um 1 Pixel abgedichteten Umriss; die Zeichnung bleibt unverändert. Derselbe Fehler steckte im bisherigen Bild „krank“ der erwachsenen Evi (Schwanz durchsichtig); dieses Bild ist neu erzeugt. Die anderen 31 Bilder bleiben unverändert (nur winzige Zwickel zwischen Körper und Schwanz wären sonst weiß statt durchsichtig). Die Änderung an `prepare-art.ps1` konnte in der Cloud nicht ausgeführt werden; beim nächsten Lauf auf dem PC prüfen. Im Browser geprüft: alle Momente zeigen die neuen Zeichnungen, Begrüßung mit „hüpfen“, Laufen nach rechts gespiegelt.

## Ideen: Evi tut selbst Dinge (23. November 2026, Ideensammlung)

Die Nutzerin hat Evi meist offen auf dem Handy und fände es süß, wenn Evi sie ab und zu mit etwas überrascht, das sie von selbst tut. Von Claude vorgeschlagen, Beschlüsse siehe Version 0.10:
- **Rahmen:** Die Zeichnungen bleiben 1:1. Bewegt wird nur das ganze Bild (laufen, hüpfen, wackeln, drehen), dazu Dinge um Evi herum (Blasen, Ball, Schmetterling, Fenster). Nur nette Überraschungen ohne Strafe, wie bei den Wünschen. Überraschungen hinterlassen Spuren, damit man nichts verpasst, wenn man nur ab und zu hinschaut.
- **Alltagsmomente** alle paar Minuten bei offener App: durchs Zimmer laufen, aus dem Fenster schauen, Gedankenblasen (Keks, Ball, Herz, Mond), summen mit Noten, gähnen, niesen, Schwanz jagen, kurzes Nickerchen.
- **Besuch im Zimmer:** Schmetterling, Vogel am Fenster, rollender Ball, Sternschnuppe nachts, Wetter im Fenster (Regen, Schnee, Regenbogen).
- **Evi bringt etwas:** Fundstücke (Münze, Blume, Feder, Stein, Blatt) mit „Für dich!“, gesammelt in einer Schatzkiste, manche selten. Evi malt Bilder, die danach an der Wand hängen. Passt zum geplanten Laden.
- **Spielen aus eigenem Antrieb:** Verstecken (raten mit links/Mitte/rechts, passt zu den drei Tasten und zum Waveshare), Evi bringt den Ball und fragt „Spielen?“.
- **Evi merkt, dass man da ist:** Begrüßung beim Öffnen („Da bist du ja!“, „Guten Morgen!“), „Während du weg warst …“, Schütteln über den Bewegungssensor (Handy; das Waveshare hat laut Händler eine IMU).
- **Eigener Kopf:** Tageslaune (verspielt, verschmust, verfressen, verträumt), später vererbbar; Lieblingsspiel und Lieblingskeks; Tagebuch mit einem Satz pro Abend als Grundlage fürs Familienalbum.
- **Besondere Tage:** Adventskalender vom 1. bis 24. Dezember, Schnee im Fenster, Weihnachten, Silvester, Entwicklungstag feiern.
- **Hilfreiche neue Zeichnungen (nur auf Wunsch):** Evi von hinten am Fenster, Evi springt, Evi trägt etwas im Maul, Evi malt.
- **Noch offen aus der Sammlung:** Schatzkiste (vorerst nicht), Bilder malen für die Wand, Verstecken (von Claude als nächstes größeres Spiel vorgeschlagen), Ball bringen, „Während du weg warst …“, Schütteln, Lieblingsspiel und Lieblingsessen, Tagebuch, Adventskalender ab 1. Dezember, Feiertage. Erledigt in 0.11: Tageslaune, Vogel, Sternschnuppe. Verworfen am 24. November 2026: echtes Wetter im Fenster (kein WLAN auf dem Waveshare).

## Ideen: Partner, Nachwuchs und neue Tamagotchis (21. November 2026, nur Brainstorming)

Die Nutzerin möchte Evi umfassend ausbauen, Richtung Partnerwahl und Nachwuchs. Besprochen, **noch nichts beschlossen:**
- Lebensbogen: Partner kennenlernen in den ersten Erwachsenentagen, Herzchen sammeln, ein Ei, Mama/Oma und Kind gemeinsam, Oma stirbt etwa dann, wenn das Kind erwachsen wird. Die jetzigen Lebenszeiten passen dafür fast ohne Änderung.
- Weitere Ideen: Besuch an der Tür oder Ausflug, Geschenke aus dem Laden fürs Date, Wesen/Persönlichkeit vererben, Familienalbum, Findel-Ei, wenn kein Partner gefunden wird.
- **Automatisch erzeugte Babybilder** (Bildgenerator im Spiel, eigener OpenAI-API-Schlüssel, Kosten pro Bild): von der Nutzerin **vorerst abgelehnt**. Ein Zusammensetzen aus Bildteilen geht nicht, weil Ohren, Schwanz und Haltung in jedem der 28 Evi-Bilder anders sind.
- **20 Vorschläge für neue Tamagotchis** (10 männlich als mögliche Partner, 10 weiblich), eigene Figuren mit deutlicher Anspielung auf sehr bekannte Spieleklassiker oder Disney-Filme. Auf Wunsch der Nutzerin ersetzt: Pummeluff-, Dragon-Quest- und Final-Fantasy-Anspielungen (kennt sie nicht oder mag sie nicht); dazu kamen Zelda-Waldgeist und Disney-Tiere. In `entwuerfe/neue-tamagotchis/vorschlaege.md` stehen die Bildaufträge und pro Figur eine Zeichnung im erwachsenen, glücklichen Zustand. Die 20 Schwarz-Weiß-PNGs verwenden Evis Zeichnung als Stilvorlage und liegen neben der Vorschlagsdatei. Die Auswahl der Figuren steht noch aus. Ausgewählte Zeichnungen später wie Evis Bilder 1:1 verwenden.

## Version 0.9: Evis Tag (21. November 2026, gebaut, noch nicht veröffentlicht)

Die Nutzerin möchte vor dem Laden zuerst Evis Tagesablauf ausbauen. Neue Ideen sollen über Wochen nach und nach dazukommen: zuerst in der Handy-App ausprobieren, später aufs Waveshare RP2350 übertragen (16 MB Flash, 520 KB RAM, dort ist genug Platz; der Cube ist mit Schritt 7 nahezu voll und bleibt Lerngerät).

Von der Nutzerin beschlossen und in 0.9 umgesetzt (Einzelheiten und Zahlen in `handy-app/README.md`):
- **Morgens müde:** Evi steht um 7 Uhr von selbst auf und ist bis 7:30 Uhr noch müde. Kein Wecken durch die Spielerin, weil Evi meist vorher wach ist.
- **Abendritual „Gute Nacht“:** von 22 bis 23 Uhr im Menü. Ablauf: Zähne putzen, Gute-Nacht-Geschichte (10 kurze Geschichten), zudecken, Licht aus. Evi schläft sofort, bekommt +10 Stimmung und den Lichtbonus. Am nächsten Morgen ist sie ausgeschlafen statt müde.
- **Mahlzeiten:** Frühstück 7–9, Mittagessen 11–14, Abendessen 17–20 Uhr. Das erste Füttern darin gibt +50 statt +40 Sättigung und +5 Stimmung extra. Eine verpasste Mahlzeit hat keine Folgen.
- **Naschen:** +10 Sättigung, +10 Stimmung. Bis zu 3 am Tag sind in Ordnung. Ab dem 4. gibt es 2 Stunden Bauchweh: Bild „krank“, die Stimmung sinkt schneller. Medizin hilft sofort. Bauchweh ist nie tödlich.
- **Wünsche:** 3–4 Mal am Tag zwischen 8 und 22 Uhr, als Sprechblase, jeweils 1 Stunde lang. Wird ein Wunsch erfüllt, gibt es +10 Stimmung und 3 Münzen. **Ein verpasster Wunsch hat keine Folgen** (ausdrücklicher Wunsch der Nutzerin).
- **Mittagsschlaf fürs Baby:** 12 bis 13 Uhr, Pflege gesperrt.
- **Aufwecken in der Nacht (0.9.1, Wunsch der Nutzerin):** Im Menü steht „Aufwecken“. Jeder Tastendruck hält Evi weitere 5 Minuten wach. Danach schläft sie im selben Licht wie vorher wieder ein. Das Aufwecken hat bewusst keine Folgen; das lässt sich später noch ändern.
- **Streicheln:** erst auf dem Waveshare mit Touch, nicht in der Handy-App.
- **Spielstand:** Spielstände von 0.7/0.8 werden übernommen. Liegt im Browser nur ein Spielstand von 0.4/0.5, bleibt diese Evi erhalten, statt mit einem Ei neu zu beginnen.

Geprüft: 26 Regelprüfungen mit `npm test`, auch in anderen Zeitzonen. Dazu kamen 36 Prüfungen in Chromium mit vorgestellter Uhrzeit, bei 390 und 320 Pixeln Breite: Übernahme eines 0.8-Spielstands, Frühstück, morgens müde, Gute Nacht mit Geschichte und Überspringen, ausgeschlafen am Morgen, Naschen bis zum Bauchweh, Medizin, Mittagsschlaf, Wunsch mit „Danke!“ und Münzen, Übernahme eines 0.5-Spielstands, neues Ei ohne Spielstand. Für das Aufwecken kamen 9 weitere hinzu: Menü, müdes Bild, Countdown, Tastendruck hält wach, Einschlafen mit Licht wieder aus. Keine Konsolenfehler.

**Veröffentlichen:** Die Nutzerin spielt über den ChatGPT-Link (`momos-kleines-zuhause…chatgpt.site`, Projekt in `handy-app/.openai/hosting.json`). Eine Claude-Cloud-Sitzung kann dorthin nicht veröffentlichen. Das muss von ChatGPT aus geschehen: `git pull` und dann `handy-app/dist` unter dem bestehenden Projekt neu veröffentlichen. Nur unter derselben Adresse bleibt der Spielstand im Browser erhalten. Welche Version dort bisher lief, ist nicht dokumentiert.

## Neue Handy-Version

- Die Nutzerin möchte das Tamagotchi zunächst auf dem Handy spielen und die spätere Übertragung auf das eigene tragbare Gerät vorbereiten. Vereinbart wurde eine Web-App als Einstieg; TestFlight ist dafür vorerst nicht nötig.
- Unter `handy-app/` liegt die spielbare Version 0.5 „Evis kleines Zuhause“. Die [veröffentlichte Handy-App](https://momos-kleines-zuhause.bushymount8.chatgpt.site) ist über den bisherigen privaten Link erreichbar und erfordert das berechtigte ChatGPT-Konto.
- Evi ist ein fuchsartiges Schwarz-Weiß-Tier mit rundem Gesicht, großen Augen, langen Ohren und buschigem Schwanz. Alte Spielstände mit dem Standardnamen „Momo“ zeigen beim Laden „Evi“. Individuell gewählte Namen haben Vorrang. Im Menü stehen Füttern, Spielen, Saubermachen und „Name ändern“ zur Verfügung.
- Darstellung am 17. November 2026 korrigiert: Die Nutzerin hat die geplante Schwarz-Weiß-Darstellung bekräftigt. Die erste farbige Handygestaltung entsprach dieser Richtung nicht. Version 0.2 verwendet direkt schwarz-weiß gezeichnete Grafik für Tier, Zimmer und Symbole sowie eine schwarz-weiße Bedienoberfläche. Keine Farbinformation und kein Graustufenfilter; Konturen und Muster unterscheiden die Objekte. Die Offline-Cache-Version wurde erhöht, das Spielstandformat blieb erhalten.
- Speicherung lokal in diesem Browser bzw. dieser installierten Web-App. Kein geräteübergreifender Abgleich. Browserdaten zu löschen kann den Stand entfernen. Tatsächlich verstrichene Zeit wird beim Öffnen nachgerechnet. Die Bedürfnisse sind vorläufige Testwerte; Tod, Schlaf, Entwicklung, Einrichtung und Generationen sind noch nicht implementiert. Möbel im Bild sind Kulisse.
- Spielregeln in `handy-app/dist/pet.js` sind von Darstellung, Touch-Eingabe, Uhr und Speicherung getrennt. Das ist eine Grundlage für eine spätere Portierung, kein Versprechen, JavaScript unverändert auf der noch offenen Hardware auszuführen. Cube-Dateien wurden nicht verändert.
- Alle acht automatisierten Prüfungen der Spielregeln und der Namensübernahme bestanden. Die Menübedienung wurde in Version 0.3 bei 320 und 390 Pixeln geprüft. Evis Darstellung und Name wurden bei 390 Pixeln geprüft und am privaten Link bestätigt. Installation und Offline-Start auf einem echten iPhone stehen noch zur Prüfung an.
- Anleitung und technische Aufteilung stehen in `handy-app/README.md`. `handy-app-schwarz-weiss.jpg` zeigt Version 0.2, `handy-app-vorschau.jpg` die frühere farbige Version. Der Unterordner hat ein eigenes Hosting-Repository.
- Die Nutzerin verwendet Chrome auf dem Handy. Als Nächstes: denselben Link in Chrome neu laden und die Darstellung prüfen; bei Bedarf über „Teilen → Zum Startbildschirm hinzufügen“ installieren.

## Version 0.8: Evis Emotionen (18. November 2026)

- Die Nutzerin hat 28 Emotionsbilder abgelegt: glücklich, hungrig, traurig, müde, krank, schmutzig und schlafend, jeweils für Baby, Teenager, Erwachsen und Oma (`entwuerfe/evi/emotionen/`). Sie sind 1:1 eingebaut. Damit zeigt auch die erwachsene Evi jetzt die Zeichnungen der Nutzerin statt der Pixel-Evi.
- Zuordnung zu den Spielzuständen: siehe Tabelle in `handy-app/README.md`. Neu ist „müde“ von 22 bis 23 Uhr, als Vorbote der Schlafenszeit.
- Werkzeug zum erneuten Einspielen: `handy-app/tools/build-art.ps1`.
- Geprüft: alle Bilder ohne Beschriftung; Erwachsen hungrig, Oma krank, Oma nach Medizin (Grundbild), Oma nach Füttern (glücklich), Teenager glücklich, Ei. Schlafend und müde wurden nicht live gesehen, weil die Uhrzeit beim Test nicht passte. Die Zuordnung ist im Code eindeutig.
- Unter dem Artefakt-Link veröffentlicht. Der Spielstand bleibt erhalten, es gibt keinen Neustart.

## Version 0.7: eigene Zeichnungen, Oma, drei Spiele (18. November 2026)

- Die Nutzerin hat Zeichnungen für Ei, Baby, Teenager und Oma in `entwuerfe/evi/` abgelegt. Ausdrücklicher Wunsch: **1:1 übernehmen, nicht verändern.** Ein erster Versuch, sie im Zeichenmodul nachzubauen, wurde abgelehnt, besonders das Baby. Jetzt zeigt das Spiel ihre Bilder unverändert, nur zugeschnitten, verkleinert und außen durchsichtig (`handy-app/dist/img/`). Für das Ei wurde `evi-ei-v2.png` gewählt, die neuere Fassung ohne Augen. Die erwachsene Evi bleibt die Pixel-Evi aus `evi-sprite.js`, weil es dafür keine Zeichnung gibt.
- Folge: Bei den gezeichneten Phasen bewegt sich das ganze Bild, es atmet, hüpft und wackelt. Gesichter wie Blinzeln, Hunger oder Schlaf können sich dort erst ändern, wenn es dafür gezeichnete Varianten gibt.
- Neue Lebensphase **Oma**: Nach 5 Tagen als Erwachsene ist Evi 2 Tage lang Oma und stirbt dann an Altersschwäche („ist friedlich eingeschlafen“).
- **Drei Spiele zur Auswahl** im Vollbild: Ballfangen, Links oder rechts?, Melodie merken. Je 5 Runden und 2 Münzen pro Erfolg.
- Ausgleich: Spielen gibt jetzt +15 plus 7 Stimmung pro Erfolg, höchstens +50. Vorher konnte man ein Baby kaum glücklich halten, ohne stündlich zu spielen.
- **Neustart:** neuer Speicherplatz `kleines-zuhause.pet.v2`. Beim ersten Öffnen erscheint ein Ei, das einmalig nach einer Minute schlüpft. Der alte Spielstand bleibt unangetastet im Browser liegen.
- Geprüft: 15 Regelprüfungen im Browser bestanden. Live geprüft: Ei schlüpft nach 1 Minute, alle drei Spiele mit Münzen (Melodie merken mit Spielende durch falschen Ton), Vollbild sowie Baby, Teenager und Oma im Zimmer.

## Spielablauf seit Version 0.6 (18. November 2026)

Von der Nutzerin festgelegt:
- **Lebenslauf:** Ei etwa 30 Minuten. Das Schlüpfen passiert auch in Abwesenheit, wer es verpasst, hat Pech. Danach Baby etwa 1 Tag mit Pflege alle 1–2 Stunden, Teenager etwa 2 Tage mit Pflege alle 2–3 Stunden, erwachsen etwa 5 Tage mit Pflege 3–4 Mal am Tag. Was danach passiert, ist offen; bis dahin bleibt das Tier erwachsen.
- **Vernachlässigung:** erst krank, dann Tod.
- **Schlaf:** Alle Tamagotchis gehen von selbst um 23 Uhr schlafen. Licht ausmachen gibt einen Stimmungsbonus.
- **Geld:** über ein kleines Minispiel.

Von Claude als Startwerte gesetzt, änderbar: Aufwachen um 7 Uhr; krank nach 4 Stunden mit einem Wert auf null; Tod nach 24 Stunden Krankheit ohne Medizin oder 30 Stunden Vernachlässigung; Ballfangspiel mit 5 Bällen und 2 Münzen pro Treffer; Münzen bleiben beim neuen Ei erhalten. Alle Zahlen stehen in `RULES` in `handy-app/dist/pet.js` und in `handy-app/README.md`.

Umsetzung: neue Zeichnungen für Ei (mit Wackeln und Riss), Baby, Teenager, krank und Grab; Nachtansicht mit ausgeschaltetem Licht; Medizin; Ton mit Abschaltung; Münzanzeige. Web-Apps können im geschlossenen Zustand keine Benachrichtigung senden. Ereignisse aus der Abwesenheit werden beim Öffnen gemeldet, auf dem Gerät soll später der Summer piepen.

Geprüft: 13 neue Regelprüfungen in `handy-app/tests/pet.test.mjs`. Sie liefen im Browser über einen Ersatz für Node, nicht mit `npm test`. Außerdem live geprüft: Schlüpfen, Menü bei Krankheit, Medizin, Ballspiel (4 von 5 gefangen = 8 Münzen), Grab, Nachtansicht. Noch nicht auf dem Handy getestet. **Nächster Schritt:** Möbel-Laden, damit die Münzen einen Zweck bekommen.

## Neue Evi seit Version 0.5 (18. November 2026)

- Die Nutzerin fand die bisherige Evi hässlich und wünschte sich etwas deutlich Evoli-Artiges. Gewählt wurden: feine Pixelkunst, weiterhin schwarz-weiß, riesige Ohren, flauschige Halskrause, großer buschiger Schwanz, große glänzende Augen und mehrere Bewegungen.
- Evi ist ein eigener Entwurf im Evoli-Stil, keine Kopie der Pokémon-Figur. Braunes Fell erscheint als Punktraster, helles Fell (Krause, Schwanzspitze) weiß.
- Nachbesserung auf Wunsch der Nutzerin: größerer, süßerer Mund (offenes Lächeln mit kleiner Zunge, größere Nase) und nur noch halb so viele Fell-Punkte.
- Neu: `handy-app/dist/evi-sprite.js` erzeugt die Pixelbilder aus einer Pose. `app.js` animiert damit Atmen, Blinzeln, Ohrenzucken, Schwanzwedeln, Stimmungsgesichter (zufrieden, hungrig, traurig, schmutzig) und die Reaktionen auf Füttern, Spielen und Putzen.
- Lokal im Browser bei 375 und 320 Pixeln geprüft; keine Konsolenfehler. Auf dem Rechner ist kein Node.js installiert. Deshalb liefen `npm test` und `node --check` nicht. `pet.js` wurde nicht verändert. Version 0.5 ist zusätzlich als privates Claude-Artefakt veröffentlicht: https://claude.ai/artifact/CvEr23tAtpFftDp4Q6a7g3 (Anmeldung bei claude.ai nötig). Dafür wurden alle Dateien in eine einzige Seite gepackt. Dort gibt es keinen Offline-Modus und keine Installation auf dem Startbildschirm, und der Spielstand ist getrennt vom alten Link. Version 0.5 wurde am 18. November 2026 auch unter dem bisherigen privaten ChatGPT-Link erfolgreich veröffentlicht. Alle acht automatisierten Prüfungen der Spielregeln sowie die Syntaxprüfungen für app.js, evi-sprite.js und sw.js bestanden vor der Veröffentlichung. Das App-Symbol (`icon.svg`, PNGs) zeigt noch das alte Gesicht.

## Handy-Oberfläche seit Version 0.3

- Die Hauptansicht erprobt das rechteckige Geräteformat 7:5. Die bisherigen Überschriften sind entfallen. Sättigung, Stimmung und Sauberkeit stehen direkt im Spielbild.
- Füttern, Spielen und Saubermachen sind über das Menü mit markierter Auswahl erreichbar. Die drei unbeschrifteten Touch-Tasten simulieren die Gerätebedienung. Links wechselt die Auswahl, die Mitte öffnet das Menü und bestätigt, rechts schließt es.

## Retro-Gamer-Umbau

- **Entschieden am 20. November 2026:** Das Waveshare RP2350-Touch-LCD-2.8 wird verwendet. Farbe bleibt möglich, Betrieb mit 4 × AAA über einen Pololu-5-V-Wandler, das Display schläft und wird per Knopf oder Touch geweckt. Die Einkaufsliste steht in der [Hardware-Datei](Tamagotchi_Hardware_Cube.md#einkaufsliste-bei-berrybase-stand-20-november-2026). **Laut Nutzerin bestellt** (BerryBase: Waveshare-Board, MCP23017, Kabel, Schrumpfschlauch; Botland: Pololu S9V11F5, Dioden-Set). **BerryBase-Lieferung mit dem Waveshare-Board am 23. November 2026 angekommen** (Foto der Nutzerin: Waveshare-Board im Karton, MCP23017 mit Stiftleisten, JST-SH-Kabel 200 mm, Jumperkabel, Schrumpfschlauch, Multimeter PCWork PCW01A; kein USB-C-Kabel). **Board läuft (23. November 2026, Foto der Nutzerin):** am USB-C-Kabel startet das Vorführprogramm ab Werk, eine Malfläche: Bildschirm hell und farbig, Touch zeichnet Punkte und Striche. **Neu: `software/schritt-08/waveshare-test`** (23. November 2026): erster eigener Test fürs Waveshare-Board – Bildschirm quer 320 × 240 schwarz-weiß, Malen per Touch, Ton über I²S. Anschlüsse aus Waveshares Demo V2. Erkennt den Touch-Chip selbst (CST3530 ab Oktober 2026, sonst CST328). Board-Paket arduino-pico 6.4.0 auf dem Haupt-PC installiert; der Sketch ließ sich für „Raspberry Pi Pico 2“ ohne Warnungen übersetzen. **Am 23. November 2026 auf das Board geladen, laut Nutzerin steht „Hallo Evi!“ da.** Stolpersteine: Die IDE hing beim ersten Start nach der Installation eine Weile; zuerst war „Raspberry Pi Pico“ statt „Pico 2“ gewählt; der erste Upload braucht die BOOT-Taste. Laut Nutzerin funktioniert alles: Bild richtig herum mit USB rechts, Malpunkt genau unter dem Finger, Touch-Chip **CST328 (V1)**, Melodie und Knöpfe gehen. Ton war recht laut, Lautstärke auf 700 gesenkt (noch nicht angehört). **Neu: `software/schritt-09/evi-waveshare`** (23. November 2026, Wunsch der Nutzerin): Evi auf dem Waveshare-Board. Regeln, Spielstand-Aufbau und Melodien wie auf dem Cube (`pet.h`, `save.h` unverändert), Evis fünf Zeichnungen der erwachsenen Evi (170 Pixel hoch, schwarze Linien auf Weiß), Touch-Knöpfe Essen, Spiel, Putzen und Licht (Licht aus = Evi schläft, Bild dunkel). Bildschirm geht nach 1 Minute aus, Tipper weckt. Spielstand im Flash (EEPROM-Bibliothek), Zeit ohne Strom zählt als Pause. Neue Evi: Finger beim Einschalten auf den Bildschirm. Bilder mit `software/schritt-09/tools/make-art.ps1` (PowerShell, auf dem Haupt-PC gibt es kein Python). Ohne Warnungen übersetzt. **Läuft auf dem Board (23. November 2026, Foto der Nutzerin):** glückliche Evi, Balken und vier Knöpfe werden richtig angezeigt. Laut Nutzerin funktioniert alles: Essen, Spiel, Putzen mit Melodie und Bewegung, Licht/Schlafen, Bildschirm aus nach 1 Minute und Wecken, Spielstand nach Ausstecken, Knöpfe gut zu treffen. Hintergrund ist in echt weiß (auf Fotos hellblau). **Neu: `software/schritt-10/evi-handy`** (23. November 2026, Wunsch der Nutzerin: genau das Spiel der Handy-App, alles an denselben Stellen, ohne den Rahmen). **Beschlüsse:** Bedienung über drei Bildschirm-Drittel (wie die drei Tasten der App, später echte Tasten), Uhrzeit nach dem Einschalten per Touch stellen (bis die Pufferbatterie da ist), Umsetzung in Etappen: 1. Zimmer, Balken, Evi mit allen Lebensphasen, Menü, Tagesablauf; 2. die drei Spiele; 3. Name ändern, Hilfe; 4. Momente, Begrüßung, Tageslaune. **Etappe 1 geschrieben:** Spielregeln vollständig aus `pet.js` (`pet.h`), Zimmer pixelgenau von Edge aus der App abfotografiert (`tools/make-assets.ps1`), alle Zeichnungen im Maßstab der App, Himmel nach der Sonne, Uhr PCF85063, Bildschirmfoto über USB (`tools/bildschirmfoto.ps1`). Ohne Warnungen übersetzt. **Läuft auf dem Board (23. November 2026):** Bildschirmfoto über USB zeigt Zimmer mit Nachthimmel, Ei auf dem Teppich, Balken und Zeile wie in der App. Menü und Pflege noch nicht rückgemeldet. Anleitung: `software/schritt-10/README.md`. Anleitung in `software/schritt-08/README.md`. **Botland-Lieferung am 23. November 2026 angekommen** (Wandler und Dioden-Set mit 1N5819, 1N5822 und weiteren Gleichrichterdioden; für die Batterie ist die 1N5819 vorgesehen). Die Pufferbatterie für die Uhr war nicht lieferbar und folgt später; bis dahin soll das Spiel nach einem Batteriewechsel die Uhrzeit abfragen. Das Display wird im Querformat (320 × 240) im Retro Gamer eingebaut, die alte Platine bleibt als Tastenplatine.
- Bevorzugt wird das **Waveshare RP2350-Touch-LCD-2.8** mit Farbe und Fingertouch. Es hat **57,60 × 43,20 mm** sichtbare Bildfläche und misst außen **73,06 × 50,54 mm**. Der recherchierte Preis bei BerryBase beträgt **25,90 €**. Die Passform im Retro Gamer und die Laufzeit bei Daueranzeige müssen noch geprüft werden. [Displayauswahl](Tamagotchi_Hardware_Cube.md#displayauswahl-vom-17-november-2026)
- **23. November 2026: Einbau begonnen.** Plastik an der Front weggeschnitten, Batterie- und Lautsprecherdrähte von der alten Platine abgelötet, alte Platine oberhalb der Schraublöcher durchgeschnitten. **Waveshare-Board und Tastenplatine passen laut Nutzerin perfekt.** Lautsprecher wird als Letztes entschieden (Waveshare-Lautsprecher passt nicht an die alte Stelle, der alte ist festgeschmolzen). Alle 8 Tasten und Minus sind mit bunten Drähten an der Tastenplatine angelötet (Lötplan `retro-gamer-loetplan.jpg`); Drähte am MCP23017 angelötet (Modul auf der Rückseite der Tastenplatine, Kabel Adafruit 4401 zum I²C-Stecker). **`software/schritt-11/tasten-test` läuft: Die echten Tasten funktionieren (23. November 2026, laut Nutzerin).** **Tasten in Evi eingebaut** (`schritt-10/evi-handy/tasten.h`, Belegung nach Wunsch der Nutzerin: Steuerkreuz hoch/runter wählt, A = OK, B = zurück, START = Menü; noch nicht in der IDE übersetzt). Nächste Schritte: am Board ausprobieren, Modul festkleben, Drähte verlegen, USB-C-Öffnung, Stromversorgung. Rückschale schließt noch nicht ganz (Batteriefach), später. Offene Schritte und Fotos: [Hardware-Datei](Tamagotchi_Hardware_Cube.md#einbauversuch-mit-dem-waveshare-board-23-november-2026-foto-der-nutzerin).
- **23. November 2026: Schritt 10 vollständig (Etappen 2–4), Wunsch der Nutzerin „die Spiele und der Rest“:** die drei Spiele (Ballfangen, Links oder rechts?, Melodie merken) mit echten Tasten und Touch, Name ändern (Buchstabentafel) und Hilfe (7 Seiten, an den Retro Gamer angepasst), Evis eigene Momente als Erwachsene (laufen, Fenster, denken, summen, gähnen, drehen) mit ihren acht Zeichnungen 1:1, Besuch (Schmetterling, Vogel, Sternschnuppe; schaltet den Bildschirm ein), Begrüßung beim Einschalten und Aufwecken, Tageslaune. In der Cloud mit einer PC-Nachbildung geprüft (übersetzt ohne Warnungen, alle Abläufe und Bilder angesehen: `software/schritt-10/vorschau-etappe-2-4.png`). **Am Board laut Nutzerin (24. November 2026): alles in Ordnung, nur Ballfangen ruckelt.** Ursache: Jedes Bild wurde komplett gesendet (35 ms). Jetzt sendet `zeigen()` nur geänderte Stellen (Ball: etwa 600 statt 153.600 Bytes pro Bild), im Spiel wird alle 12 ms gezeichnet; in der Nachbildung mit nachgebildetem Display geprüft, am Board noch nicht. Falls es trotzdem ruckelt, käme ein anderes Spiel in Frage (Idee der Nutzerin). Einzelheiten: `software/schritt-10/README.md`. **Steuerkreuz schwerfällig (24. November 2026):** `tasten.h` zählt einen Druck jetzt sofort, der MCP23017 merkt sich jede Änderung (INTF/INTCAP), Abfrage alle 3 ms; mit nachgebildetem MCP23017 geprüft, am Board noch nicht. **Am Board bestätigt (24. November 2026, laut Nutzerin): Die Tasten reagieren schnell und funktionieren „richtig gut“. Die Gummimatte muss nicht eingeschnitten werden.**
- Bei freiem UART lassen sich bis zu vier physische Spieltasten direkt am Waveshare anschließen. Für die sieben Eingänge von Steuerkreuz, A, B und Start ist der MCP23017 als kleine I²C-Erweiterung vorgeschlagen. Touch kann als zusätzliche Bedienung dienen.
- Bei der nächsten Zerlegung sind die Befestigung, der Anschluss und der freie Platz hinter dem Originaldisplay zu klären. Auf einem vorhandenen Foto ist die Platine unter dem angehobenen Display bereits teilweise zu sehen. [Fotos, Messpunkte und Umbauplan](Tamagotchi_Hardware_Cube.md#umbauversuch-mit-dem-vorhandenen-billig-game-boy)

## Wo wir stehen

- Der Funduino Cube, Lötzubehör, Lötzinn und die Lötstation sind laut Nutzerin bestellt. **Alles ist angekommen:** die Funduino-Lieferung (Cube, Zange, Spitzenreiniger, Pinzetten, Entlötpumpe) am 19. November 2026, Lötstation Zhongdi ZD-99 und Lötzinn (BerryBase) am 20. November 2026. **Cube für Evi fertig gelötet (20. November 2026):** Nano, Display, Taster, Lautsprecher, die drei LEDs, Fotowiderstand und die Widerstände sind angelötet. Noch offen und für Evi nicht nötig: Bewegungsmelder, Ultraschall, LED-Ring, Servo-Leiste, Infrarot-Empfänger, Potentiometer, MPU-6050, DHT11, Extern-Anschlüsse. Am 20. November 2026 im Bauhaus gekauft: Löthilfe CFH LA 240 („dritte Hand“). **Pinbelegung des Cube** am 20. November 2026 aus der offiziellen Lötanleitung abgelesen, siehe [Hardware-Datei](Tamagotchi_Hardware_Cube.md#pinbelegung-des-cube). Der Taster hängt an D9 und meldet gedrückt HIGH. **Neu: `software/schritt-05`** mit einem Cube-Test und Evi für den echten Cube (Taster D9, Töne an D7, LEDs als Hungeranzeige, Licht aus = Evi schläft). **Evi läuft auf dem echten Cube (20. November 2026, laut Nutzerin):** `evi-cube` mit der Arduino IDE aufgespielt; Anzeige, Taster, Hunger-LEDs, Hungerton und Fütter-Melodie funktionieren. Auch das Einschlafen bei Dunkelheit funktioniert laut Nutzerin. Der Cube-Test wurde übersprungen. **Neu: `software/schritt-06`** mit echtem Hunger-Tempo wie bei der erwachsenen Evi in der Handy-App (20 Punkte pro Stunde, im Schlaf 15 %, Füttern +40, Start 80). Am PC geprüft, noch nicht auf dem Cube. **Neu: `software/schritt-07`** (20. November 2026, Wunsch der Nutzerin: Spielstand, ihre Zeichnungen, mehr Bedürfnisse): Evis Zeichnungen der erwachsenen Evi (glücklich, hungrig, traurig, schmutzig, schlafend) ohne Zimmer auf dem Display, nur verkleinert, Linien leuchten; Satt, Laune und Sauber mit Füttern, Spielen und Putzen wie in der Handy-App; Spielstand im EEPROM (16 Plätze reihum), neue Evi durch Taster beim Einstecken. Am PC geprüft (54 Regelprüfungen, 21 Prüfungen in einer Nachbildung mit Vorschaubildern). **Am 21. November 2026 auf dem echten Cube getestet, laut Nutzerin funktioniert alles:** Zeichnungen, Menü, LEDs, Fotosensor. Ob der Spielstand eine Nacht ohne Strom übersteht, ist noch nicht ausprobiert.
- Die Anforderungen stehen in `Tamagotchi_Anforderungen.md`, die Hardwareplanung in `Tamagotchi_Hardware_Cube.md`.
- Die Software liegt unter `software/`. Die Ordner `schritt-01` bis `schritt-07` bewahren die bisherigen Entwicklungsstände jeweils getrennt auf.
- Der aktuelle Stand für den echten Cube ist `software/schritt-07`. Der letzte Simulator-Stand ist `software/schritt-04`. Dort liegen der Programmcode, der virtuelle Aufbau (`diagram.json`), die benötigte Bibliothek (`libraries.txt`), eine Vorschau und eine Anleitung zum Wiederherstellen in Wokwi.
- Der aktuelle Stand wurde im Wokwi-Simulator kompiliert und ausprobiert. Zusätzlich bestanden 32 Verhaltensprüfungen ohne Fehler. Das ist noch keine Prüfung auf dem echten Cube.

## Aktuelle Bedienung

- Einmal kurz: Weiter.
- Zweimal schnell: Zurück.
- Etwa 0,7 Sekunden halten: Bestätigen.
- Zuhause öffnet Weiter oder Bestätigen die Aktionen. Dort lassen sich Winken und Füttern auswählen. Zurück führt nach Hause.
- Ein einzelner kurzer Druck wird erst nach ungefähr 300 Millisekunden wirksam, damit ein Doppelklick erkannt werden kann.

Knopferkennung, Menü und Tierregeln sind getrennt. Die drei Befehle können auf dem späteren Gerät über einzelne Knöpfe oder ein Steuerkreuz mit weiteren Tasten ausgelöst werden. Die konkrete Belegung hängt von der Hardware ab.

## Wichtig zum Speichern

Die Dateien in diesem Projektordner sind lokal gespeichert. Das momentan geöffnete Wokwi-Projekt hat noch keine dauerhaft gespeicherte Onlineadresse. Es muss zum Weiterarbeiten nicht offen bleiben: Die Anleitung in `software/schritt-04/README.md` beschreibt, wie die Simulation aus den lokalen Dateien wiederhergestellt wird. Ein Neustart der Simulation setzt den aktuellen Tierzustand zurück; ein gespeicherter Spielstand ist noch nicht umgesetzt.

## Beim nächsten Mal am Cube

Vom Stand `software/schritt-07` weiterarbeiten. Er läuft auf dem Cube. Offen: bestätigen, dass der Spielstand nach dem Ausstecken über Nacht erhalten bleibt.

**MPU-6050 entfernt (21. November 2026):** Das Modul war verdreht eingelötet. Entlöten mit der Pumpe klappte nicht; das Modul wurde geopfert, die Pins gekürzt und mit etwas Kraft herausgezogen. Für Evi wird das MPU nicht gebraucht. Danach wieder eingesteckt: laut Nutzerin geht es Evi gut, alles funktioniert einwandfrei. Falls später gewünscht: neues GY-521-Modul (etwa 3–5 €); vorher prüfen, ob die Löcher noch durchgehend leiten. Für künftiges Entlöten Entlötlitze besorgen. Mögliche nächste Ideen: kleines Spiel statt einfachem Spielen, Lebensphasen mit den Baby-, Teenager- und Oma-Zeichnungen (Speicher prüfen), Krankheit.
