# Schritt 10: Die Handy-App auf dem Waveshare-Board

Wunsch der Nutzerin vom 23. November 2026: **genau das Spiel der Handy-App**, alles an denselben Stellen, nur ohne den Rahmen um das Tamagotchi.

**Beschlüsse vom 23. November 2026:**
- **Bedienung über drei Bildschirm-Drittel:** links tippen = linke Taste, Mitte = mittlere Taste, rechts = rechte Taste der App. Später übernehmen die echten Retro-Gamer-Tasten.
- **Uhrzeit nach dem Einschalten einstellen**, solange die Pufferbatterie der Uhr fehlt.
- **In Etappen:** 1. Zimmer, Balken, Evi mit Lebensphasen, Menü, Tagesablauf → 2. die drei Spiele → 3. Name ändern und Hilfe → 4. Evis Momente, Begrüßung, Tageslaune.

## Etappe 1

- **Zimmer** wie in der App, pixelgenau: von Microsoft Edge aus `handy-app/dist/index.html` abfotografiert (320 × 224, also 1:1). Das Fenster zeigt Tag, Dämmerung oder Nacht mit Mond und Sternen nach der echten Sonne (`himmel.h` = `sky.js`).
- **Evi** mit allen Zeichnungen: Ei, Baby, Teenager, Erwachsen, Oma, je mit glücklich, hungrig, traurig, müde, krank, schmutzig, schlafend, im Maßstab der App, auf dem Teppich (Fußpunkt 167/180). Atmen (2 Pixel), Ei wackelt, Schatten. Nach dem Tod das Grab aus der App.
- **Balken oben** (Napf, Herz, Glanz mit Prozent; schwarz hinterlegt ab 30 oder weniger), **Zeile unten** (Name, Phase, Münzen und Evis Satz), **Menü rechts** mit Symbolen, **Wunschblase**, **zZz-Kasten**, **Reaktionszeichen** (♥ ♪ ✦ +) und die Hüpf-, Kau- und Wackelbewegung.
- **Spielregeln** vollständig aus `pet.js` übernommen (`pet.h`): Lebenslauf, Abnahme je Phase, Schlaf 23–7 Uhr, Licht aus, Gute Nacht mit Geschichte, Aufwecken, Mittagsschlaf, Mahlzeiten, Naschen mit Bauchweh, Wünsche, Medizin, Krankheit, Tod, neues Ei, Münzen.
- **Menü** wie in der App: Füttern (oder Frühstück, Mittagessen, Abendessen), Naschen, Spielen, Saubermachen, Medizin, Gute Nacht, Aufwecken, Licht aus/an, Neues Ei, Ton an/aus. Neu für das Board: **Uhr stellen**.
- **Töne** aus der App (Schlüpfen, Entwicklung, Achtung, Wunsch, Münze …).
- Das **allererste Ei** schlüpft wie in der App nach 1 Minute, jedes neue Ei nach 30 Minuten.

Die Etappen 2 bis 4 sind seit dem 23. November 2026 dabei, siehe unten.

**Anders als in der App, wegen des Geräts:**
- **Seit 24. November 2026 bleibt der Bildschirm an** (Wunsch der Nutzerin). **RESET** schaltet ihn aus; dann macht ihn der erste Druck oder Tipper nur wieder an. Bei Ereignissen (Wert niedrig, krank, Entwicklung, neuer Wunsch) und bei Besuch geht er von selbst an. Bis dahin ging die Beleuchtung nach 1 Minute ohne Berührung aus; `DUNKEL_NACH_MS` in `evi-handy.ino` (0 = nie) stellt das bei Bedarf wieder ein.
- **Gespeichert** wird bei jeder Pflege, bei Ereignissen und sonst alle 30 Minuten (Flash schonen). Die Uhr rechnet die Zeit nach, auch wenn das Board aus war.
- Die Schrift ist die eingebaute 5×7-Pixelschrift (mit Umlauten). Symbole und Zeichnungen sind aus der App übernommen.

## Etappen 2 bis 4 (23. November 2026, Wunsch der Nutzerin: „die Spiele und der Rest, der fehlt“)

Vorschau aus der Nachbildung am PC (Schrift dort nur Ersatz): [vorschau-etappe-2-4.png](vorschau-etappe-2-4.png)

**Etappe 2: die drei Spiele** (`spiele.ino`, wie `app.js`): je 5 Runden, jeder Erfolg 2 Münzen, Regeln aus `pet.h` (`spielBeenden`), Wünsche „Ballfangen!“ usw. werden erfüllt. Während des Spiels: weißer Grund mit Bodenlinie, keine Balken oben, Spielstand in der Zeile unten.

| Spiel | Tasten am Gerät | Touch (wie die App) |
|---|---|---|
| Ballfangen | A oder Steuerkreuz fängt; B oder START beendet | links oder Mitte fängt, rechts beendet |
| Links oder rechts? | ◀ oder ▶ raten; B oder START beendet | links/rechts raten, Mitte beendet |
| Melodie merken | ◀ linkes Feld, A (oder ▲▼) Mitte, ▶ rechtes Feld; B oder START bricht ab | die drei Drittel |

Ein falscher Ton beendet „Melodie merken“, geschaffte Runden zählen. Abbrechen gibt keine Münzen („Spiel abgebrochen“). Im Spiel wird alle 40 ms neu gezeichnet, damit der Ball flüssig läuft; der Ball läuft nach echter Zeit.

**Etappe 3: Name ändern und Hilfe** (`name-hilfe.ino`), statt der Dialoge der App eigene Bildschirme:
- **Name ändern:** Buchstabentafel A–Z, Ä Ö Ü ß, Umschalten (Aa), Bindestrich, Leerzeichen, Löschen, Zurück, OK. Steuerkreuz wählt, A tippt, START speichert, B bricht ab; oder auf die Tafel tippen. Nach dem ersten Buchstaben geht es klein weiter, nach Leerzeichen und Bindestrich wieder groß. Höchstens 20 Bytes (Platz im Spielstand).
- **Hilfe:** die Texte der App, angepasst an den Retro Gamer (Tasten, Uhr, Spielstand; ohne Handy-Hinweise), 7 Seiten. ◀▶ oder ▲▼ blättern, B oder START schließt; Touch: links zurück, Mitte schließen, rechts weiter.

**Etappe 4: Evis eigene Momente** (`momente.ino`, wie `moments.js` und die Momente in `app.js`), zuerst nur für die erwachsene Evi, seit 24. November 2026 in jeder Phase außer dem Ei (siehe oben):
- **Kleine Momente** alle 5–10 Minuten: herumlaufen und hüpfen, aus dem Fenster schauen (Sonne oder Mond in der Gedankenwolke), an etwas denken, summen (♪ ♫), gähnen, sich im Kreis drehen. Hungrig, traurig oder schmutzig denkt sie nur an Essen, Spielen oder Baden; müde gähnt sie.
- **Besuch** ein- bis zweimal am Tag zwischen 9 und 21 Uhr (wartet bis 22 Uhr): Schmetterling oder Vogel, im Dunkeln eine Sternschnuppe, mit Ton. Gleiche Zeiten wie in der App (Streuwert aus Geburtszeit und Datum).
- **Begrüßung** mit Hüpfen und „Guten Morgen!“, „Da bist du ja endlich!“, „Guten Abend!“ oder „Hallo!“ …; beim ersten Gruß des Tages die **Tageslaune** (verspielt, verschmust, verfressen, verträumt), die auch die Momente beeinflusst. Unten steht die Laune alle 10 Sekunden im Wechsel.
- Evis acht Zeichnungen der Momente (laufen, fenster, denken, singen, gaehnen, drehen, huepfen, staunen) 1:1 aus `handy-app/dist/img`, nur verkleinert (Maßstab 0,38) und schwarz-weiß: `momente.h`, erzeugt mit `tools/make-momente.mjs`. Nach rechts läuft sie gespiegelt, wie in der App.

**Anders als in der App, wegen des Geräts:**
- Kleine Momente nur bei hellem Bildschirm. **Besuch schaltet den Bildschirm ein.** Während eines Moments, eines Spiels und Gute Nacht bleibt der Bildschirm an.
- Begrüßt wird nach dem Einschalten des Boards und wenn man den Bildschirm nach mindestens 3 Minuten Dunkelheit weckt (statt „App wieder geöffnet“), und morgens beim Aufwachen, wenn der Bildschirm an ist.
- Wann der letzte Besuch war und ob die Tageslaune heute schon gesagt wurde, steht nur im Arbeitsspeicher; der Spielstand (`speicher.h`) bleibt unverändert. Nach einem Neustart kann darum ein Besuch vom selben Tag noch einmal kommen.
- Die Zeichen ▲▼◀▶ und „ “ sind in `text.h` ergänzt (Zeichen der 5×7-Schrift).

**Neue Dateien:** `spiele.ino`, `momente.ino`, `name-hilfe.ino` (die Arduino IDE zeigt sie als Reiter und setzt sie hinter `evi-handy.ino`), `extras.h` (Typen und Zustand für Spiele und Momente), `momente.h` (Bilder). Geändert: `evi-handy.ino`, `ton.h` (Töne „Besuch“ und „Zwitschern“, Einzelton), `text.h`.

**Geprüft (23. November 2026, Cloud):** mit `tools/pc-nachbildung/pruefen.sh` ohne Fehler und Warnungen übersetzt; in der Nachbildung laufen alle drei Spiele bis zum Ende (Münzen, Meldung), alle neun Momente enden mit Evi zurück auf dem Teppich, Besuch schaltet den dunklen Bildschirm ein und der Bildschirm bleibt bis zum Ende an, Begrüßung beim Aufwecken, Name tippen, Hilfe blättern. Bildschirmbilder angesehen. **Noch nicht in der Arduino IDE übersetzt und noch nicht auf dem Board.**

## Bildschirm bleibt an, RESET schaltet ihn (24. November 2026)

Wunsch der Nutzerin: Der Bildschirm geht nicht mehr nach 1 Minute aus, sondern bleibt an; RESET (hatte noch keine Aufgabe) schaltet ihn aus. `DUNKEL_NACH_MS = 0` in `evi-handy.ino`, `bildschirmAus()` beendet dabei ein Spiel (abgebrochen), Gute Nacht und einen Moment. Wieder an: jede Taste oder jeder Tipper (der erste Druck macht nur an, nach mindestens 3 Minuten begrüßt Evi), ein Ereignis oder Besuch. In der Hilfe ergänzt. Geprüft in der Nachbildung (`ablauf.cpp`, mit nachgebildetem MCP23017): bleibt nach 5 Minuten an, RESET aus und wieder an, Begrüßung, RESET im Spiel, Besuch schaltet ein. **Am Board bestätigt (24. November 2026, laut Nutzerin): RESET funktioniert.**

**Achtung Batterien:** Die Hintergrundbeleuchtung braucht den meisten Strom. Dauernd an leeren 4 × AAA deutlich schneller als mit Abschalten nach 1 Minute; wie lange sie halten, ist noch nicht gemessen. Mit Batterien also RESET zum Ausschalten nutzen, oder `DUNKEL_NACH_MS` wieder setzen.

## Momente in jeder Lebensphase (24. November 2026)

Wie Version 0.12 der App: Die Momente, der Besuch, die Begrüßung und die Tageslaune gibt es jetzt als Baby, Teenager, Erwachsene und Oma (nicht als Ei), jeweils mit Evis eigenen acht Zeichnungen der Phase. `momente.h` (mit `tools/make-momente.mjs` aus `handy-app/dist/img/<phase>-*.png`, Maßstab je Phase wie `ART_SCALE` in der App) enthält `MOMENT_BILD[Phase][8]`; `momenteFuer(Phase)` in `momente.ino` entscheidet, in welchen Phasen. Geprüft in der PC-Nachbildung: Baby (gehen, Fenster, Begrüßung), Teenager (Schmetterling, summen), Oma (drehen, Vogel) zeigen ihre eigenen Zeichnungen, jedes Bild auf dem nachgebildeten Display stimmt. **Am Board noch nicht ausprobiert.**

**Besuch mit den Bildern der Nutzerin (24. November 2026):** Schmetterling (32 × 26) und Vogel (23 × 20) wie in der App aus `handy-app/dist/besuch/` (`momente.h`: `BESUCH_FALTER`, `BESUCH_VOGEL`, innen weiß, Linien schwarz). Der Vogel wird gespiegelt und nur im Fenster gezeichnet (`zeichneBildBeschnitten`), das Fensterkreuz liegt davor; Sitzplatz 37/39 im Fenster. In der Nachbildung angesehen, am Board noch nicht.

## Flüssiger Ball: nur Geändertes senden (24. November 2026)

Laut Nutzerin lief alles, nur der Ball beim Ballfangen ruckelte. Grund: `zeigen()` schickte jedes Mal das ganze Bild (320 × 240 Punkte, etwa 35 ms), der Ball sprang also in großen Sätzen. Jetzt merkt sich `bildschirm.h`, was schon auf dem Display steht, und schickt nur die geänderten Zeilenblöcke (Ausschnitt per Spalten-/Zeilenfenster des ST7789). Beim Ball sind das im Mittel etwa 600 Bytes statt 153.600 pro Bild. Im Spiel wird jetzt etwa alle 12 ms gezeichnet (vorher 40 ms). Davon profitieren auch Menü, Momente und alle anderen Bildschirme.

Dabei behoben: Nach der letzten Ballrunde ließ sich der Ball bis zum Spielende noch einmal fangen („6 von 5 gefangen“).

Geprüft in der PC-Nachbildung, jetzt mit nachgebildetem Display: In 3913 Bildern (Spiele, Momente, Name, Hilfe) stand auf dem Display genau das Bild des Programms, ohne falschen Punkt. **Am Board bestätigt (24. November 2026, laut Nutzerin): Der Ball läuft flüssig.**

## Tasten reagieren schneller (24. November 2026)

Laut Nutzerin fühlte sich das Steuerkreuz schwerfällig an. Mechanisch wird die Gummimatte über den Lötpunkten von L, R und DOWN eingeschnitten (Nutzerin, geplant). Im Programm (`tasten.h`):
- Ein Druck zählt **sofort** beim ersten Lesen (vorher erst nach zwei gleichen Lesungen im Abstand von 8 ms). Entprellt wird nur noch das Loslassen: 20 ms ohne Druck, damit ein flackernder Kohlekontakt nicht doppelt zählt.
- Der MCP23017 **merkt sich jede Änderung** (Interrupt bei Änderung, Register INTF/INTCAP), ohne dass der INT-Ausgang angeschlossen ist. Ein kurzes Antippen zwischen zwei Abfragen geht nicht mehr verloren. Grenze: Werden zwei verschiedene Tasten innerhalb weniger ms angetippt und beide vor der nächsten Abfrage losgelassen, zählt nur die erste.
- Abfrage etwa alle 3 ms (vorher 8 ms). Festhalten wiederholt nur, solange die Taste wirklich gedrückt ist.

Geprüft mit einem nachgebildeten MCP23017 (`tools/pc-nachbildung/tasten.cpp`): 10-ms-Tipper während 40 ms Zeichnen wird erkannt (die alte Abfrage verpasste ihn), Halten zählt einmal, Steuerkreuz wiederholt, Flackern zählt nicht doppelt, zwei schnelle Tipper zählen zweimal. **Am Board ausprobiert (24. November 2026, laut Nutzerin): Die Tasten reagieren jetzt schnell und funktionieren „richtig gut“; die Gummimatte muss nicht eingeschnitten werden.**

## Evi spürt Bewegung, „Wie war dein Tag?“, zwei neue Spiele (24. November 2026)

Wunsch der Nutzerin: Bewegungen spüren (Gassi gehen, schütteln, kippen, aufs Gesicht legen), „Wie war dein Tag?“ und neue Spiele, aber nicht Verstecken oder Ball bringen. **Nur auf dem Board**, die Handy-App bleibt unverändert (Wunsch der Nutzerin).

**Lagesensor** (`lage.h`): der QMI8658 auf dem Board (I²C 0x6B oder 0x6A, an Wire1), nur Beschleunigung, ±4 g, 50 Messungen pro Sekunde. Die Reihenfolge der Bytes wird beim Start erkannt (in Ruhe misst er 1 g). Wie der Chip auf der Platine liegt, **lernt Evi beim Benutzen**: Bei jedem Tastendruck und Tipper wird das Gerät mit dem Bildschirm nach oben gehalten. Nach drei ruhigen Bedienungen kennt sie die Bildschirmrichtung, die Oberkante, sobald das Gerät dabei schräg gehalten wird. Kippen und Aufs-Gesicht-Legen wirken erst danach; Schritte und Schütteln sofort. Nach jedem Neustart lernt sie es neu (nur im Arbeitsspeicher).

| Bewegung | Erkennung | Was Evi tut (`bewegung.ino`) |
|---|---|---|
| **Gassi gehen** | Schritte im Takt (6 hintereinander, je 0,25–1,5 s Abstand); Ende nach 60 s ohne Schritt | Bei hellem Bildschirm trabt sie mit (Bild „laufen“), unten „Gassi gehen … 123 Schritte“. Ab 100 Schritten nach dem Spaziergang: „Das war schön draußen!“, +10 Stimmung (+5 je volle 1000 Schritte, höchstens +25), 1 Münze je 500 Schritte (höchstens 10), Sättigung und Sauberkeit je −5 (Regel `gassiBeenden` in `pet.h`). Nur wenn sie wach und gesund ist. Endet der Spaziergang bei dunklem Bildschirm, erzählt sie es beim Einschalten. Unten steht im Wechsel „Heute … Schritte zusammen“. |
| **Schütteln** | 4 Stöße über 1,8 g in 1,2 s | „Hui! Alles dreht sich!“, sie wackelt (Bild „drehen“), dann „Mir ist ganz schwindelig …“; beim dritten Mal in 2 Minuten „Bitte nicht mehr schütteln!“. Schlafend: „Hmpf … Evi schläft.“ Krank: „Mir geht es nicht gut …“. Das Ei wackelt. Keine Folgen für die Werte. |
| **Kippen** | etwa 20° zur Seite für 0,3 s | „Huch! Der Ball rollt!“ Ein Ball rollt zur tieferen Seite, Evi rennt hinterher. Liegt das Gerät 3 s gerade, holt sie ihn („Hab ihn!“) und geht zurück. |
| **Aufs Gesicht legen** | Bildschirm 1,5 s nach unten | Bildschirm aus (wie RESET); nachts schläft Evi und das Licht geht aus (mit Lichtbonus). Wieder umgedreht: Bildschirm an; ab 3 Minuten Begrüßung, ab 20 s „Gähn … das war ein schönes Nickerchen!“. Mit RESET ausgeschaltet bleibt er aus. |

**„Wie war dein Tag?“** Zwischen 18 und 23 Uhr fragt Evi einmal von selbst (statt eines kleinen Moments, mit dem Wunsch-Ton); das Menü zeigt die Antworten Gut, So lala, Nicht so gut. Gut: Jubel „Juhu! Das freut mich!“. So lala: „Morgen wird es bestimmt schöner!“. Nicht so gut: Evi tröstet („Oh nein … Komm, ich kuschel mit dir.“), singt dir ein Lied und sagt „Hab dich lieb! ♥“. B schließt ohne Antwort („Erzählst du es mir später?“); bis zur Antwort steht die Frage im Menü. Keine Folgen für die Werte; gemerkt nur im Arbeitsspeicher.

**Neue Spiele** (`spiele.ino`, je 5 Runden, 2 Münzen pro Erfolg wie die anderen; sie erfüllen keine Wünsche, die bleiben wie in der App):
- **Kekse fangen:** Kekse fallen von oben, Evi fängt sie mit dem Mund. Laufen mit ◀ ▶ (festhalten läuft weiter, Touch: links/rechts) oder durch **Kippen**. Jeder Keks fällt 20 % schneller. B (Touch: Mitte) beendet.
- **Schnick, Schnack, Schnuck:** ◀ Schere, A Stein, ▶ Papier (Touch: die drei Drittel). Evi hüpft dreimal („Schnick … Schnack … Schnuck!“) und zeigt ihre Wahl in einer Blase. Gleichstand zählt nicht, dann noch einmal.
- Schere, Stein, Papier und die Wolken für „So lala“/„Nicht so gut“ sind mit Linien gezeichnet (`zeichneZeichen` in `bewegung.ino`), kein Bild aus der App.

Hilfe um „Evi spürt Bewegung“ und „Wie war dein Tag?“ ergänzt, „Spielen“ nennt fünf Spiele.

**Geprüft in der PC-Nachbildung** (`tools/pc-nachbildung/bewegung.cpp`, mit nachgebildetem Lagesensor): Achsen gelernt; 4 Minuten Gehen = 439 von etwa 441 Schritten, Evi trabt mit, danach Stimmung +8,7 (abzüglich Abnahme) und Meldung; Zittern zählt nicht; Schütteln wach und schlafend; Kippen nach rechts und links, Ball und Evi folgen, Rückkehr; Aufs-Gesicht mit Nickerchen, nachts Licht aus mit Bonus, mit RESET bleibt er aus; Frage am Abend, Schließen ohne Antwort, Antwort aus dem Menü, Trösten; Kekse fangen mit Kippen 5 von 5; Schnick, Schnack, Schnuck bis zum Ende. Ohne Warnungen übersetzt, Display ohne falschen Punkt. **Am Board noch nicht ausprobiert:** ob der Sensor antwortet und wie empfindlich die Schwellen in der Hand sind. Die Zahlen stehen oben in `lage.h` (Namensraum `L`).

## Echte Retro-Gamer-Tasten (23. November 2026)

Seit Schritt 11 hängen die Tasten des Retro Gamers über den MCP23017 am Board (`tasten.h`). Belegung nach Wunsch der Nutzerin:

| Taste | im Spiel | beim Uhr-Stellen |
|---|---|---|
| Steuerkreuz hoch / runter | Auswahl im Menü bewegen (Festhalten blättert) | Wert ändern (Festhalten zählt schnell) |
| A | Menü öffnen / OK | nächstes Feld, nach der Minute fertig |
| B | zurück / Menü schließen | Feld zurück |
| START | Menü öffnen oder schließen | nächstes Feld |
| Steuerkreuz links / rechts | in den Spielen (Raten, Melodie) | Feld zurück / weiter |
| RESET | Bildschirm aus (ein laufendes Spiel wird abgebrochen); ist er aus, macht jede Taste ihn wieder an | – |

- Der erste Druck bei dunklem Bildschirm macht ihn nur an, wie beim Tippen. Jede Taste überspringt „Gute Nacht“.
- Die Hinweise unten zeigen „▲▼ wählen · A OK · B zurück“ und „A öffnet das Menü“, sobald der MCP23017 gefunden ist; sonst die alten Touch-Hinweise.
- Touch über die drei Drittel funktioniert weiterhin. Ohne MCP23017 läuft alles wie vorher; das Programm sucht ihn alle 2 Sekunden.
- Entprellt: Ein Druck zählt, wenn zwei Lesungen im Abstand von 8 ms gleich sind.
- Geprüft (23. November 2026, Cloud): mit g++ und Ersatz-Headern ohne Fehler und Warnungen. **Noch nicht in der Arduino IDE übersetzt und noch nicht auf dem Board.**

## Linker Bildrand im Retro Gamer (23. November 2026)

Am Gerät sind die ersten 8 Bildspalten links nicht zu sehen. `RAND_LINKS = 8` in `bildschirm.h`: Die drei Balken oben teilen sich jetzt die Breite ab x = 8, Name/Phase/Münzen unten beginnen bei x = 15, der Menühinweis ist im sichtbaren Teil mittig. Das Zimmer bleibt unverändert (links 8 Punkte Hintergrund verdeckt). Geprüft mit g++ und Ersatz-Headern; noch nicht in der IDE übersetzt.

## Aufspielen

1. In der Arduino IDE `software/schritt-10/evi-handy/evi-handy.ino` öffnen, Board **Raspberry Pi Pico 2**, hochladen.
2. Beim ersten Start fragt Evi nach **Datum und Uhrzeit**: links = weniger, rechts = mehr (festhalten zählt schnell), Mitte = nächstes Feld, nach der Minute „Fertig“. Vorgeschlagen wird die Uhrzeit vom Hochladen.
3. Nach RESET läuft die Uhr weiter. Nach dem Ausstecken muss sie neu gestellt werden, bis die Pufferbatterie da ist.

Der Spielstand von Schritt 9 wird nicht übernommen: Es beginnt ein neues Ei.

## Werkzeuge

- `tools/make-assets.ps1`: erzeugt `evi-handy/assets.h` (Zimmer, Symbole, Zeichnungen) aus der Handy-App. Braucht Microsoft Edge, kein Python, kein Node. Nach Änderungen an der App oder neuen Zeichnungen einfach erneut ausführen:
  `powershell -ExecutionPolicy Bypass -File software\schritt-10\tools\make-assets.ps1`
- `tools/preview-assets.ps1`: zeigt den Inhalt von `assets.h` als PNG.
- `tools/bildschirmfoto.ps1`: holt über USB ein Bildschirmfoto vom laufenden Board (Zeichen `S` über die serielle Schnittstelle). Seriellen Monitor der IDE vorher schließen.
- `tools/make-momente.mjs`: erzeugt `evi-handy/momente.h` (Evis Zeichnungen der Momente, Symbole der Gedankenwolke, Noten). Läuft in der Cloud mit Node und Playwright; auf dem Haupt-PC nicht nötig.
- `tools/pc-nachbildung/`: Prüfung am PC ohne Arduino IDE (siehe dortige README).

## Dateien

- `evi-handy.ino`: Bildschirm, Menü, Tasten-Drittel, Ereignisse, Gute-Nacht-Ablauf, Uhr stellen.
- `spiele.ino`, `momente.ino`, `name-hilfe.ino`, `extras.h`, `momente.h`: Etappen 2 bis 4 (siehe oben). `tasten.h`: echte Tasten über den MCP23017.
- `lage.h`, `bewegung.ino`: Lagesensor und was Evi bei Bewegung tut, „Wie war dein Tag?“ (seit 24.11.2026).
- `pet.h`: Spielregeln aus `pet.js`. `zeit.h`: Datum und deutsche Ortszeit mit Sommerzeit. `himmel.h`: Sonnenstand aus `sky.js`.
- `uhr.h`: Uhr PCF85063 (I²C 0x51). `speicher.h`: Spielstand mit Prüfsumme. `text.h`: Schrift mit Umlauten. `ton.h`: Töne der App.
- `board.h`, `touch.h`, `bildschirm.h`: aus Schritt 8/9.

## Geprüft (23. November 2026)

- Für „Raspberry Pi Pico 2“ ohne Fehler und Warnungen übersetzt: 219 KB Programm, 19 KB Arbeitsspeicher.
- `assets.h` als Vorschaubild angesehen: Zimmer in drei Himmeln, alle Zeichnungen, Grab.
- **Auf dem Board (23. November 2026):** Bildschirmfoto über USB (COM8) zeigt Zimmer mit Nachthimmel, Ei mit Schatten auf dem Teppich, Balken (80/80/100 %) und die Zeile „Evi Ei 0 … Schlüpft in ca. 1 Min.“ an denselben Stellen wie in der App. Menü und Pflege noch nicht rückgemeldet.
