# Tamagotchi-Projekt – Stand zum Weiterarbeiten

Stand: 18. November 2026

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

- **Entschieden am 20. November 2026:** Das Waveshare RP2350-Touch-LCD-2.8 wird verwendet. Farbe bleibt möglich, Betrieb mit 4 × AAA über einen Pololu-5-V-Wandler, das Display schläft und wird per Knopf oder Touch geweckt. Die Einkaufsliste steht in der [Hardware-Datei](Tamagotchi_Hardware_Cube.md#einkaufsliste-bei-berrybase-stand-20-november-2026). **Laut Nutzerin bestellt** (BerryBase: Waveshare-Board, MCP23017, Kabel, Schrumpfschlauch; Botland: Pololu S9V11F5, Dioden-Set). Die Pufferbatterie für die Uhr war nicht lieferbar und folgt später; bis dahin soll das Spiel nach einem Batteriewechsel die Uhrzeit abfragen. Das Display wird im Querformat (320 × 240) im Retro Gamer eingebaut, die alte Platine bleibt als Tastenplatine.
- Bevorzugt wird das **Waveshare RP2350-Touch-LCD-2.8** mit Farbe und Fingertouch. Es hat **57,60 × 43,20 mm** sichtbare Bildfläche und misst außen **73,06 × 50,54 mm**. Der recherchierte Preis bei BerryBase beträgt **25,90 €**. Die Passform im Retro Gamer und die Laufzeit bei Daueranzeige müssen noch geprüft werden. [Displayauswahl](Tamagotchi_Hardware_Cube.md#displayauswahl-vom-17-november-2026)
- Bei freiem UART lassen sich bis zu vier physische Spieltasten direkt am Waveshare anschließen. Für die sieben Eingänge von Steuerkreuz, A, B und Start ist der MCP23017 als kleine I²C-Erweiterung vorgeschlagen. Touch kann als zusätzliche Bedienung dienen.
- Bei der nächsten Zerlegung sind die Befestigung, der Anschluss und der freie Platz hinter dem Originaldisplay zu klären. Auf einem vorhandenen Foto ist die Platine unter dem angehobenen Display bereits teilweise zu sehen. [Fotos, Messpunkte und Umbauplan](Tamagotchi_Hardware_Cube.md#umbauversuch-mit-dem-vorhandenen-billig-game-boy)

## Wo wir stehen

- Der Funduino Cube, Lötzubehör, Lötzinn und die Lötstation sind laut Nutzerin bestellt. **Alles ist angekommen:** die Funduino-Lieferung (Cube, Zange, Spitzenreiniger, Pinzetten, Entlötpumpe) am 19. November 2026, Lötstation Zhongdi ZD-99 und Lötzinn (BerryBase) am 20. November 2026. **Cube für Evi fertig gelötet (20. November 2026):** Nano, Display, Taster, Lautsprecher, die drei LEDs, Fotowiderstand und die Widerstände sind angelötet. Noch offen und für Evi nicht nötig: Bewegungsmelder, Ultraschall, LED-Ring, Servo-Leiste, Infrarot-Empfänger, Potentiometer, MPU-6050, DHT11, Extern-Anschlüsse. Am 20. November 2026 im Bauhaus gekauft: Löthilfe CFH LA 240 („dritte Hand“). **Pinbelegung des Cube** am 20. November 2026 aus der offiziellen Lötanleitung abgelesen, siehe [Hardware-Datei](Tamagotchi_Hardware_Cube.md#pinbelegung-des-cube). Der Taster hängt an D9 und meldet gedrückt HIGH. **Neu: `software/schritt-05`** mit einem Cube-Test und Evi für den echten Cube (Taster D9, Töne an D7, LEDs als Hungeranzeige, Licht aus = Evi schläft). **Evi läuft auf dem echten Cube (20. November 2026, laut Nutzerin):** `evi-cube` mit der Arduino IDE aufgespielt; Anzeige, Taster, Hunger-LEDs, Hungerton und Fütter-Melodie funktionieren. Der Cube-Test wurde übersprungen. Noch nicht geprüft: Einschlafen bei Dunkelheit (Fotowiderstand) und die Lichtwerte hell/dunkel.
- Die Anforderungen stehen in `Tamagotchi_Anforderungen.md`, die Hardwareplanung in `Tamagotchi_Hardware_Cube.md`.
- Die Software liegt unter `software/`. Die Ordner `schritt-01` bis `schritt-05` bewahren die bisherigen Entwicklungsstände jeweils getrennt auf.
- Der aktuelle Stand für den echten Cube ist `software/schritt-05`. Der letzte Simulator-Stand ist `software/schritt-04`. Dort liegen der Programmcode, der virtuelle Aufbau (`diagram.json`), die benötigte Bibliothek (`libraries.txt`), eine Vorschau und eine Anleitung zum Wiederherstellen in Wokwi.
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

Vom Stand `software/schritt-05` weiterarbeiten. `evi-cube` läuft bereits auf dem Cube. Offen: Schlafenszeit bei Dunkelheit prüfen; falls sie nicht auslöst, mit `cube-test` die Licht-Werte hell und dunkel ablesen und `DARK_BELOW`/`BRIGHT_ABOVE` anpassen. Danach bieten sich an: Evis Zeichnungen verkleinert auf das 128 × 64-Display (vorher Vorschau zeigen), Spielstand im EEPROM, Spielregeln vom Testtempo auf echte Zeiten umstellen.
