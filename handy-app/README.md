# Kleines Zuhause – Handy-Prototyp 0.11

**Neu in 0.11: Himmel, Tageslaune, mehr Besuch.** Beschlossen mit der Besitzerin am 23. November 2026, wieder nur für die erwachsene Evi (außer dem Himmel).

- **Himmel nach der echten Sonne:** Das Fenster ist hell, solange in Deutschland die Sonne scheint, in der Dämmerung gepunktet (bis die Sonne 6° unter dem Horizont steht) und nachts schwarz mit Mond und Sternen. `dist/sky.js` rechnet Sonnenauf- und -untergang für jeden Tag selbst aus (Sonnenstandsgleichung, auf etwa 1–2 Minuten genau), ohne Internet und ohne Tabelle. Es muss nie etwas nachgetragen werden, auch die Sommerzeit stimmt von selbst. Ort: die Mitte Deutschlands (51,16° N, 10,45° O); in Aachen oder Görlitz geht die Sonne bis zu etwa 20 Minuten anders unter. Der Ort steht in `SKY` und lässt sich ändern. Die Schlafenszeit 23–7 Uhr bleibt, wie sie ist.
- **Mehr Besuch** bei den besonderen Momenten (1–2 am Tag): neben dem Schmetterling ein **Vogel**, der auf dem Fensterkreuz landet und zwitschert (eigener Ton). Evi kommt näher, staunt und hüpft, dann fliegt er davon („Tschüss, Vögelchen!“). Ist es draußen dunkel, kommt stattdessen eine **Sternschnuppe**: Evi staunt, wünscht sich etwas (Gedankenwolke mit Herz, Zeichnung „denken“), und eine zweite fällt hinter die Hügel („Noch eine!“). Im Winter kommt die Sternschnuppe also oft am Abend, im Sommer selten.
- **Tageslaune:** Jede Evi hat jeden Tag eine Laune: verspielt (öfter drehen, spazieren, an den Ball denken), verschmust (öfter Herz-Gedanken, Begrüßung immer mit mehr Hüpfern), verfressen (öfter an Kekse und Essen denken) oder verträumt (öfter Fenster, Gedankenwolke, summen). Sie steht für den ganzen Tag fest. Bei der ersten Begrüßung des Tages sagt Evi sie („Heute bin ich verspielt!“), danach steht sie alle 10 Sekunden im Wechsel mit „Mitte öffnet das Menü“ in der Zeile. Werte in `MOMENTS.tempers`.
- **Guten Morgen beim Aufwachen:** Ist die App um 7 Uhr offen, begrüßt Evi dich beim Aufwachen.

## Version 0.10

**Neu in 0.10: Evi tut selbst etwas.** Wunsch der Besitzerin vom 23. November 2026: Evi soll sie ab und zu überraschen. **Vorerst nur die erwachsene Evi.** Was wann passiert, steht in `MOMENTS` in `dist/moments.js`, die Darstellung in `dist/app.js`. Die Momente ändern keine Werte, Münzen oder Regeln.

- **Kleine Momente** alle 5 bis 10 Minuten, solange die App offen ist und Evi wach ist: ein kleiner Spaziergang, zum Fenster gehen und hinausschauen, eine Gedankenwolke (Keks, Ball, Herz, Lied, Essen), summen mit Noten, gähnen, im Kreis drehen. Nie zweimal dasselbe hintereinander. Ist Evi hungrig, traurig oder schmutzig, denkt sie nur an Essen, Spielen oder Baden. Abends ab 22 Uhr und morgens noch müde gähnt sie nur. Kein Ton.
- **Etwas Besonderes** ein- oder zweimal am Tag zwischen 9 und 21 Uhr, mit Ton: Ein Schmetterling flattert herein, kreist um Evi, landet auf ihrem Kopf („Hihi, das kitzelt!“) und fliegt davon, Evi läuft hinterher und hüpft. Die Zeiten stehen für jeden Tag und jede Evi fest. Der Moment wartet bis 22 Uhr, bis die App offen ist. Wurden beide verpasst, kommt nur einer.
- **Begrüßung** beim Öffnen und nach mindestens 3 Minuten Abwesenheit: Evi hüpft und sagt „Da bist du ja!“, „Hallo!“ oder „Schön, dass du da bist!“. Beim ersten Besuch des Tages bis 11 Uhr „Guten Morgen!“, nach mehr als 4 Stunden „Da bist du ja endlich!“ (beide mit mehr Hüpfern), abends ab 18 Uhr „Guten Abend!“. Keine Begrüßung, wenn sie schläft oder krank ist.
- **Bewegung:** Evis Zeichnungen bleiben unverändert. Das ganze Bild wird verschoben, angehoben und beim Laufen nach rechts gespiegelt (von der Besitzerin erlaubt). Gedankenwolke, Noten und Schmetterling sind Pixelgrafik des Spiels.
- **Eigene Zeichnungen für die Momente (seit 0.10.1 alle 8 eingebaut):** laufen (Spaziergang, zum Fenster, hinter dem Schmetterling her; nach rechts gespiegelt), hüpfen (Begrüßung, Hüpfer), gähnen, staunen (Schmetterling kreist und landet), Fenster (von hinten), denken (Gedankenwolke), singen (summen), drehen (im Kreis). Vorlagen und Bildaufträge in `../entwuerfe/evi/momente/`. `tools/build-art.ps1` übernimmt jede vorhandene Datei `evi-erwachsen-<name>.png` aus diesem Ordner. Fehlt eine, zeigt das Spiel das passende Emotionsbild (gähnen = „müde“, Schmetterling = „glücklich“).
- **Unterbrechen:** Menü offen, Spiel, Gute Nacht oder ein Dialog verhindern neue Momente. Pflege beendet einen Moment, Evi läuft zurück auf ihren Teppich. Bei Spielstart, Gute Nacht, Einschlafen, Krankheit oder wenn die App in den Hintergrund geht, ist sie sofort wieder auf dem Teppich.
- **Gespeichert** wird nur, wann zuletzt etwas Besonderes war (`kleines-zuhause.moments`), getrennt vom Spielstand.
- Bei „Bewegung reduzieren“ in den Systemeinstellungen läuft und dreht Evi sich nicht, und der Schmetterling sitzt nur kurz auf ihrem Kopf.

## Version 0.9

**Neu in 0.9: Evis Tag.** Beschlossen mit der Besitzerin am 21. November 2026. Alle Zahlen stehen in `RULES` in `dist/pet.js`.

- **Morgens müde:** Evi steht um 7 Uhr auf und ist bis 7:30 Uhr noch müde. Ab 22 Uhr ist sie wieder müde.
- **Gute Nacht (Abendritual):** Von 22 bis 23 Uhr gibt es im Menü „Gute Nacht“. Der Ablauf dauert etwa 15 Sekunden: Zähne putzen, eine von 10 kurzen Gute-Nacht-Geschichten, zudecken, Licht aus. Jede Taste überspringt ihn. Evi schläft danach sofort und bekommt +10 Stimmung, dazu den Lichtbonus von +10. Am nächsten Morgen ist sie ausgeschlafen: Statt „müde“ zeigt sie das glückliche Bild.
- **Mahlzeiten:** Frühstück 7–9, Mittagessen 11–14, Abendessen 17–20 Uhr. Das erste Füttern in diesem Zeitraum heißt im Menü wie die Mahlzeit und gibt +50 Sättigung (statt +40) und +8 Stimmung (statt +3). Eine verpasste Mahlzeit hat keine Folgen.
- **Naschen:** neuer Menüpunkt. Ein Keks gibt +10 Sättigung und +10 Stimmung. Bis zu 3 Kekse am Tag sind in Ordnung. Ab dem 4. hat Evi 2 Stunden Bauchweh: Bild „krank“, die Stimmung sinkt 1,5-mal so schnell, kein Naschen mehr. Medizin hilft sofort. Am Bauchweh stirbt sie nicht. Um Mitternacht beginnt die Zählung neu.
- **Wünsche:** 3–4 Mal am Tag zwischen 8 und 22 Uhr wünscht sich Evi etwas: eines der drei Spiele, Naschen, Baden oder Essen (zur Essenszeit die passende Mahlzeit). Eine Sprechblase zeigt den Wunsch eine Stunde lang. Wird er erfüllt, gibt es +10 Stimmung und 3 Münzen, und die Blase sagt „Danke!“. Ein Wunsch lässt sich auch dann erfüllen, wenn der Wert schon voll ist. Ein verpasster Wunsch hat keine Folgen. Während Schlaf, Krankheit und Bauchweh wünscht sie sich nichts. Die Wunschzeiten stehen für jeden Tag und jede Evi fest, deshalb gelten sie auch bei geschlossener App.
- **Aufwecken (0.9.1):** Schläft Evi nachts, gibt es im Menü „Aufwecken“. Danach ist sie wach und müde, und das Menü ist wie am Tag. Die Bedürfnisse sinken im Wach-Tempo. Jeder Tastendruck hält sie weitere 5 Minuten wach. Danach schläft sie im selben Licht wie vorher wieder ein. Ein zweites Abendritual in derselben Nacht gibt es nicht. Das Aufwecken hat sonst keine Folgen.
- **Mittagsschlaf:** Das Baby schläft von 12 bis 13 Uhr. Die Bedürfnisse sinken dann wie nachts bei ausgeschaltetem Licht, Pflege ist gesperrt.
- **Spielstand:** Der Spielstand von 0.7/0.8 wird übernommen und bekommt die neuen Angaben dazu (Speicherformat 3). Gibt es im Browser nur einen Spielstand von 0.4/0.5, bleibt diese Evi erhalten, statt mit einem neuen Ei zu beginnen.

## Version 0.8

**Neu in 0.8:** Die Emotionen der Besitzerin sind eingebaut, je 7 Zustände für Baby, Teenager, Erwachsen und Oma, wieder 1:1 übernommen. Welches Bild erscheint:

| Zustand im Spiel | Bild |
| --- | --- |
| Alle Werte ab 60, oder kurz nach Füttern, Spielen, Putzen, Schlüpfen und Entwicklung | glücklich |
| Kein Wert niedrig, aber nicht alle ab 60 | Grundzeichnung (bei Erwachsen: glücklich) |
| Sättigung am niedrigsten und ≤ 30 | hungrig |
| Stimmung am niedrigsten und ≤ 30 | traurig |
| Sauberkeit am niedrigsten und ≤ 30 | schmutzig |
| Krank | krank |
| 22–23 Uhr, wenn sonst alles in Ordnung ist | müde |
| 23–7 Uhr | schlafend |

Die Bilder entstehen mit `tools/build-art.ps1` aus `../entwuerfe/evi/`. Ohne PowerShell (zum Beispiel in der Cloud) macht `python3 tools/prepare_art.py EIN.png AUS.png 0.31` dasselbe für ein einzelnes Bild; den Eintrag in `dist/art.js` dann von Hand ergänzen. Seit 0.10.1 wird „außen“ mit einem um 1 Pixel abgedichteten Umriss gesucht, damit eine winzige Lücke im Umriss das Innere nicht durchsichtig macht (vorher betroffen: der Schwanz der erwachsenen Evi bei „krank“). Das Skript schneidet aus, entfernt die Beschriftung, verkleinert alle Bilder einer Lebensphase im selben Maßstab und schreibt `dist/img/*.png` und `dist/art.js`. Neue oder geänderte Zeichnungen: Dateien gleich benennen und das Skript erneut ausführen. Die Pixel-Evi aus `evi-sprite.js` wird nur noch für das Grab gebraucht und dient als Ersatz, falls ein Bild fehlt.

## Version 0.7

**Neu in 0.7:** Ei, Baby, Teenager und Oma sind die Zeichnungen der Besitzerin aus `../entwuerfe/evi/`. Sie sind unverändert übernommen, nur zugeschnitten, verkleinert und außen durchsichtig gemacht (`dist/img/`). Die erwachsene Evi zeichnet weiterhin `evi-sprite.js`. Neu ist auch die Oma-Phase von 2 Tagen mit Tod durch Alter. „Spielen“ öffnet eine Auswahl aus drei Spielen im Vollbild: Ballfangen, Links oder rechts?, Melodie merken. Die Spielstände liegen unter dem neuen Schlüssel `kleines-zuhause.pet.v2`. Damit beginnt das Spiel neu mit einem Ei, das einmalig nach einer Minute schlüpft.

Eigenständiger Einstieg für die spätere tragbare Tamagotchi-Version. Tier, Zimmer, Bedienung und App-Symbole verwenden Schwarz und Weiß. Konturen, Muster und Gesichtsausdrücke müssen ohne Farbinformation funktionieren; die Grafiken sind direkt entsprechend gezeichnet, ohne nachträglichen Graustufenfilter. Die Cube-Übungen unter `software/` bleiben separat erhalten.

## Lokal öffnen

Im Ordner `handy-app` mit installiertem Node.js: `npm start`. Anschließend `http://127.0.0.1:4173` öffnen. ES-Module und Offline-Speicherung brauchen einen Webserver; `index.html` nicht nur per Doppelklick öffnen. Der lokale Vorschau-Server ist nur am Computer erreichbar. Der veröffentlichte HTTPS-Link ist für das Handy vorgesehen.

## Spielen

- Die Schwarz-Weiß-Spielszene im Seitenverhältnis 7:5 zeigt Evi und den Tierstatus. Napf, Herz und Glanzsymbol stehen für Sättigung, Stimmung und Sauberkeit.
- Evi (seit 0.5) ist ein eigenes, Evoli-artiges Pixeltier, gezeichnet als 82 × 64-Pixel-Grafik. Sie hat riesige Ohren mit schwarzer Innenseite, große glänzende Augen, eine flauschige helle Halskrause und einen buschigen Schwanz mit heller Spitze. Braunes Fell wird als Punktraster dargestellt, helles Fell rein weiß. Das Gesicht bleibt ohne Raster, damit es gut lesbar ist.
- Bewegungen: Evi atmet, blinzelt, zuckt mit den Ohren und wedelt mit dem Schwanz, wenn sie zufrieden ist. Bei Hunger hat sie besorgte Brauen und einen offenen Mund. Ist sie traurig, hängen Ohren und Schwanz herab. Ist sie schmutzig, hat sie müde Augen, Flecken im Fell und Geruchswölkchen. Beim Füttern kaut sie, beim Spielen hüpft sie, beim Putzen wackelt sie. Bei „Bewegung reduzieren“ in den Systemeinstellungen bleiben nur die Gesichtsausdrücke.
- Die mittlere der drei Tasten öffnet das Spielmenü und bestätigt die markierte Auswahl. Links wechselst du zum nächsten Eintrag, rechts schließt du das Menü.
- Das Menü passt sich an. Tagsüber gibt es Füttern (zur Essenszeit als Frühstück, Mittagessen oder Abendessen), Naschen, Spielen und Saubermachen, bei Krankheit oder Bauchweh zusätzlich Medizin, ab 22 Uhr „Gute Nacht“. Nachts gibt es Licht aus/an. Nach dem Tod gibt es „Neues Ei“. Immer verfügbar sind Name ändern, Ton an/aus und Hilfe. Der bisherige Standardname „Momo“ wechselt beim Öffnen zu „Evi“. Ein eigener Name hat Vorrang.
- **Lebenslauf:** Ei 30 Minuten → Baby 1 Tag → Teenager 2 Tage → erwachsen 5 Tage → Oma 2 Tage → stirbt an Altersschwäche. Bei der Oma sinkt die Sättigung um 18 statt 20 pro Stunde. Das Ei schlüpft auch ohne Spielerin. Ist die App offen, wackelt das Ei, bekommt einen Riss, und Evi schlüpft live mit Ton.
- **Pflegetakt:** Abnahme pro Stunde im Wachzustand (Sättigung/Stimmung/Sauberkeit): Baby 45/35/30, Teenager 28/22/18, erwachsen 20/15/12. Ein voller Wert fällt damit nach etwa 1,5 / 2,5 / 3,5 Stunden auf „niedrig“ (30).
- **Aktionen:** Füttern +40 Sättigung. Saubermachen +60 Sauberkeit. Medizin heilt, kostet aber 5 Stimmung. Bei voller Anzeige passiert nichts.
- **Spiele:** Jedes Spiel hat 5 Runden, und jeder Erfolg bringt 2 Münzen. Die Stimmung steigt um 15 plus 7 pro Erfolg, also höchstens um 50. Sättigung −8, Sauberkeit −5.
  - **Ballfangen:** Drück Mitte oder links, wenn der Ball im Feld ist. Rechts bricht ab. Der Ball wird von Runde zu Runde schneller.
  - **Links oder rechts?:** Rate mit links oder rechts, wohin Evi springt. Die Mitte bricht ab.
  - **Melodie merken:** Die Tonfolge beginnt mit 2 Tönen und wird jede Runde einen Ton länger. Ein falscher Ton beendet das Spiel, geschaffte Runden zählen.
- **Tagesablauf (seit 0.9):** siehe oben – müde Zeiten, Gute Nacht, Mahlzeiten, Naschen, Wünsche, Mittagsschlaf fürs Baby.
- **Schlaf:** 23 bis 7 Uhr, für alle Lebensphasen. Die Bedürfnisse sinken dann nur mit 15 % des Tagestempos. Licht aus gibt einmal pro Nacht +10 Stimmung, und die Stimmung sinkt kaum. Bleibt das Licht an, sinkt sie mit 60 %. Morgens geht das Licht von selbst an.
- **Krankheit und Tod:** Steht ein Wert insgesamt 4 Stunden auf null, wird Evi krank: Stimmung sinkt 1,5-mal so schnell, Spielen geht nicht. Ohne Medizin stirbt sie nach 24 Stunden Krankheit, bei Dauervernachlässigung nach 30 Stunden. Danach startet „Neues Ei“ die nächste Generation; Münzen bleiben erhalten.
- **Ton:** Signale bei Schlüpfen, Entwicklung, Krankheit, Tod und wenn ein Wert niedrig wird; abschaltbar. Web-Apps können keine Benachrichtigung senden, solange sie geschlossen sind – Ereignisse in der Abwesenheit werden beim nächsten Öffnen kurz gemeldet.
- Tatsächlich vergangene Zeit wird beim Öffnen minutengenau nachgerechnet, einschließlich Nacht, Entwicklung und Krankheit. Keine laufende Ausführung im Hintergrund nötig.
- Spielstände aus 0.4/0.5 werden übernommen; das Tier gilt dann als erwachsen.
- Automatisches Speichern nur auf diesem Gerät, in diesem Browser bzw. dieser installierten Web-App. Kein Cloud-Abgleich. Das Entfernen von Browserdaten kann den Stand löschen. Unlesbare bzw. unbekannte Spielstände werden nicht überschrieben; in diesem Fehlerfall läuft nur eine ungespeicherte Sitzung.
- Nach vollständig erfolgreichem Laden der Offline-Dateien auch offline nutzbar. Eine Hosting-Anmeldung und Updates benötigen Internet. Keine Benachrichtigungen.
- Möbel im Bild sind vorläufige Kulisse. Kauf, Einrichtung und weitere Räume folgen als Nächstes, dann haben die Münzen einen Zweck. Partnersuche und Vererbung kommen später.

## Auf dem Handy

Öffne auf dem iPhone den veröffentlichten Link in Chrome oder Safari und wähle „Teilen → Zum Home-Bildschirm“. Aktiviere „Als Web-App öffnen“, wenn diese Option angezeigt wird. Melde dich bei Bedarf mit dem berechtigten Konto an. Auf Android findest du „Installieren“ oder „Zum Startbildschirm hinzufügen“ im Browsermenü. Die Anleitung steht im Spielmenü unter „Hilfe“.

## Vorbereitung auf das spätere Gerät

`dist/pet.js` enthält ausschließlich Zustand und Spielregeln. `dist/moments.js` entscheidet ebenso ohne Browser, wann Evi etwas von selbst tut, und `dist/sky.js` rechnet den Himmel aus. Zeit und Befehle werden als Parameter übergeben; zurück kommt ein neuer Zustand. Kein DOM, kein `localStorage`, keine Timer und keine Grafik in diesem Modul. `dist/app.js` verbindet die Regeln mit Browser-Uhr, Speicherung und Oberfläche und steuert Evis Bewegungen. `dist/evi-sprite.js` zeichnet Evi: Eine Pose (Augen, Mund, Ohren- und Schwanzwinkel, Atmung, Schmutz) wird zu einem Pixelraster aus Schwarz und Weiß. Das Modul hat keine Abhängigkeit vom Browser. Damit lassen sich die Einzelbilder später als Bitmaps für das Gerät exportieren, zum Beispiel für die 320 × 240 Pixel des Waveshare. Die Anzeige benötigt keine externen Dateien oder Schriftanbieter.

Die spätere Hardware ist noch offen. JavaScript ist daher kein Versprechen unveränderter Übernahme auf einen Mikrocontroller. Zustandsmodell, Regeln und Verhaltensprüfungen dienen als klare Vorlage für einen gegebenenfalls nötigen C++-Port. Anzeige, Tasten, Uhr und Speicher brauchen eigene Hardwareadapter. Die drei Pflegeaktionen sind Spielbefehle. Die Cube-Testregeln werden nicht automatisch verändert.

Die drei Touch-Tasten erproben die Bedienung des späteren Geräts. Ein Steuerkreuz und zusätzliche Tasten kommen beim Umbau des vorhandenen Retro Gamer infrage.

## Prüfen und ändern

`npm test` prüft Lebensphasen, Schlüpfen, Pflegetakt, Schlaf und Lichtbonus, Krankheit, Tod und neues Ei, Ballspiel-Münzen, Wertebereiche, rückwärts verstellte Uhr, gespeicherte Daten, Übernahme alter Spielstände und Namen, die Oma-Phase mit Tod durch Alter sowie seit 0.9 müde Zeiten, Abendritual, Mahlzeiten, Naschen und Bauchweh, Mittagsschlaf, Wünsche, Aufwecken in der Nacht und die Übernahme von 0.8-Spielständen (26 Prüfungen). `tests/moments.test.mjs` prüft seit 0.10 Evis eigene Momente: nur Erwachsen, Abstand 5–10 Minuten, Auswahl je nach Zustand ohne Wiederholung, Zeiten und Warten der besonderen Momente sowie die Begrüßung (7 Prüfungen), seit 0.11 auch Tageslaune und Besuch (3 weitere). `tests/sky.test.mjs` vergleicht Sonnenauf- und -untergang mit veröffentlichten Zeiten für Berlin, Frankfurt, München und Hamburg (höchstens 3 Minuten Abweichung) und prüft Tag, Dämmerung und Nacht im Winter und im Sommer (3 Prüfungen). Zusammen 39. `node --check dist/app.js` und `node --check dist/sw.js` prüfen die Syntax. `node make-icons.mjs` erzeugt die PNG-App-Symbole ohne Zusatzpakete.

Bei Offline-Änderungen die Cache-Version in `dist/sw.js` erhöhen. Die `.openai/hosting.json` ordnet diesen Unterordner seiner privaten Site zu. Andere Projektdateien werden nicht mit hochgeladen.
