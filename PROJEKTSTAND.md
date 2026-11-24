# Tamagotchi-Projekt – Stand zum Weiterarbeiten

Stand: 24. November 2026

## Waveshare: neues Zimmer, Jahreszeiten, Wetter, Weihnachtsdeko (24. November 2026)

Wunsch der Nutzerin: Jahreszeiten wie in den Fensterentwürfen (Frühling ab 1. Februar, Sommer ab 1. Juni, Herbst ab 21. September, Winter ab 1. November), Weihnachtsdeko nach der Zeitangabe im Repo, die neuen Möbel; dazu Wetter zufällig je nach Jahreszeit, **Schnee nur im Winter**. Nur Waveshare. Einzelheiten in `software/schritt-10/README.md`.
- Möbel aus `entwuerfe/umgebung/` (Sideboard, Tisch mit Pflanze, Wandregal, Napf) und Dielenboden; Werkzeug `tools/make-umgebung.py` → `umgebung.h`.
- Wetter wechselt alle 3 Stunden, fest für jeden Tag; zu sehen am Tag und in der Dämmerung, nachts wie bisher Mond und Sterne.
- Weihnachten 18. November bis 5. Januar: Baum auf dem Tisch statt Regal und Pflanze, Sterne am Fenster, Kerzentablett auf dem Sideboard (derselbe Zeitraum für alles; die Nutzerin hatte ihn für Sterne und Kerzen noch nicht festgelegt).
- Fenstersterne und Kerzentablett hat Claude klein selbst gezeichnet (kein Entwurfsbild vorhanden). Wenn die Nutzerin Bilder dafür möchte, lassen sie sich ersetzen.
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
- **Touch (nur Waveshare):** streicheln (schon geplant), kitzeln, Evi folgt dem Finger.
- **Gemeinsame Rituale:** „Wie war dein Tag?“ am Abend (gut, so lala, schlecht; Evi freut sich mit oder tröstet), Zähneputzen zusammen mit 2-Minuten-Uhr, „Während du weg warst …“, Evi merkt sich, wann die Nutzerin meist vorbeischaut, und wartet dann am Fenster.
- **Spiele:** Verstecken, Ball bringen (aus der Sammlung vom 23. November).
- Claudes Empfehlung: zuerst Adventskalender (ab 1. Dezember), dann Gassi gehen und Hinlegen zum Schlafen, dann „Wie war dein Tag?“.

## Waveshare: Bildschirm bleibt an, RESET schaltet ihn (24. November 2026)

Wunsch der Nutzerin: Der Bildschirm des Boards geht nicht mehr nach 1 Minute aus. **RESET** (hatte keine Aufgabe) schaltet ihn aus und mit jedem Druck wieder an; Ereignisse und Besuch schalten ihn ebenfalls ein. Ein laufendes Spiel wird beim Ausschalten abgebrochen. **Am Board bestätigt (laut Nutzerin): RESET funktioniert, der Ball läuft flüssig, Momente gesehen (Spaziergang mit Hüpfen, Blick aus dem Fenster).** **Offen:** Mit dauernd leuchtendem Bildschirm halten 4 × AAA deutlich kürzer; Verbrauch messen, wenn der Pololu-Wandler angeschlossen ist. Einstellbar über `DUNKEL_NACH_MS` in `software/schritt-10/evi-handy/evi-handy.ino`.

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
