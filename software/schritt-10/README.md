# Schritt 10: Die Handy-App auf dem Waveshare-Board

Wunsch der Nutzerin vom 23. November 2026: **genau das Spiel der Handy-App**, alles an denselben Stellen, nur ohne den Rahmen um das Tamagotchi.

**Beschlüsse vom 23. November 2026:**
- **Bedienung über drei Bildschirm-Drittel:** links tippen = linke Taste, Mitte = mittlere Taste, rechts = rechte Taste der App. Später übernehmen die echten Retro-Gamer-Tasten.
- **Uhrzeit nach dem Einschalten einstellen**, solange die Pufferbatterie der Uhr fehlt.
- **In Etappen:** 1. Zimmer, Balken, Evi mit Lebensphasen, Menü, Tagesablauf → 2. die drei Spiele → 3. Name ändern und Hilfe → 4. Evis Momente, Begrüßung, Tageslaune.

## Etappe 1 (dieser Stand)

- **Zimmer** wie in der App, pixelgenau: von Microsoft Edge aus `handy-app/dist/index.html` abfotografiert (320 × 224, also 1:1). Das Fenster zeigt Tag, Dämmerung oder Nacht mit Mond und Sternen nach der echten Sonne (`himmel.h` = `sky.js`).
- **Evi** mit allen Zeichnungen: Ei, Baby, Teenager, Erwachsen, Oma, je mit glücklich, hungrig, traurig, müde, krank, schmutzig, schlafend, im Maßstab der App, auf dem Teppich (Fußpunkt 167/180). Atmen (2 Pixel), Ei wackelt, Schatten. Nach dem Tod das Grab aus der App.
- **Balken oben** (Napf, Herz, Glanz mit Prozent; schwarz hinterlegt ab 30 oder weniger), **Zeile unten** (Name, Phase, Münzen und Evis Satz), **Menü rechts** mit Symbolen, **Wunschblase**, **zZz-Kasten**, **Reaktionszeichen** (♥ ♪ ✦ +) und die Hüpf-, Kau- und Wackelbewegung.
- **Spielregeln** vollständig aus `pet.js` übernommen (`pet.h`): Lebenslauf, Abnahme je Phase, Schlaf 23–7 Uhr, Licht aus, Gute Nacht mit Geschichte, Aufwecken, Mittagsschlaf, Mahlzeiten, Naschen mit Bauchweh, Wünsche, Medizin, Krankheit, Tod, neues Ei, Münzen.
- **Menü** wie in der App: Füttern (oder Frühstück, Mittagessen, Abendessen), Naschen, Spielen, Saubermachen, Medizin, Gute Nacht, Aufwecken, Licht aus/an, Neues Ei, Ton an/aus. Neu für das Board: **Uhr stellen**.
- **Töne** aus der App (Schlüpfen, Entwicklung, Achtung, Wunsch, Münze …).
- Das **allererste Ei** schlüpft wie in der App nach 1 Minute, jedes neue Ei nach 30 Minuten.

**Noch nicht dabei:** Die drei Spiele (das Menü zeigt sie, Auswählen sagt „kommen in Etappe 2“), Name ändern, Hilfe, Evis eigene Momente, Begrüßung, Tageslaune.

**Anders als in der App, wegen des Geräts:**
- Die Beleuchtung geht nach **1 Minute** ohne Berührung aus. Der erste Tipper schaltet sie nur wieder an. Bei Ereignissen (Wert niedrig, krank, Entwicklung, neuer Wunsch) geht sie von selbst an.
- **Gespeichert** wird bei jeder Pflege, bei Ereignissen und sonst alle 30 Minuten (Flash schonen). Die Uhr rechnet die Zeit nach, auch wenn das Board aus war.
- Die Schrift ist die eingebaute 5×7-Pixelschrift (mit Umlauten). Symbole und Zeichnungen sind aus der App übernommen.

## Echte Retro-Gamer-Tasten (23. November 2026)

Seit Schritt 11 hängen die Tasten des Retro Gamers über den MCP23017 am Board (`tasten.h`). Belegung nach Wunsch der Nutzerin:

| Taste | im Spiel | beim Uhr-Stellen |
|---|---|---|
| Steuerkreuz hoch / runter | Auswahl im Menü bewegen (Festhalten blättert) | Wert ändern (Festhalten zählt schnell) |
| A | Menü öffnen / OK | nächstes Feld, nach der Minute fertig |
| B | zurück / Menü schließen | Feld zurück |
| START | Menü öffnen oder schließen | nächstes Feld |
| Steuerkreuz links / rechts | noch keine Aufgabe (später für „Links oder rechts?“) | Feld zurück / weiter |
| RESET | noch keine Aufgabe | – |

- Der erste Druck bei dunklem Bildschirm macht ihn nur an, wie beim Tippen. Jede Taste überspringt „Gute Nacht“.
- Die Hinweise unten zeigen „▲▼ wählen · A OK · B zurück“ und „A öffnet das Menü“, sobald der MCP23017 gefunden ist; sonst die alten Touch-Hinweise.
- Touch über die drei Drittel funktioniert weiterhin. Ohne MCP23017 läuft alles wie vorher; das Programm sucht ihn alle 2 Sekunden.
- Entprellt: Ein Druck zählt, wenn zwei Lesungen im Abstand von 8 ms gleich sind.
- Geprüft (23. November 2026, Cloud): mit g++ und Ersatz-Headern ohne Fehler und Warnungen. **Noch nicht in der Arduino IDE übersetzt und noch nicht auf dem Board.**

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

## Dateien

- `evi-handy.ino`: Bildschirm, Menü, Tasten-Drittel, Ereignisse, Gute-Nacht-Ablauf, Uhr stellen.
- `pet.h`: Spielregeln aus `pet.js`. `zeit.h`: Datum und deutsche Ortszeit mit Sommerzeit. `himmel.h`: Sonnenstand aus `sky.js`.
- `uhr.h`: Uhr PCF85063 (I²C 0x51). `speicher.h`: Spielstand mit Prüfsumme. `text.h`: Schrift mit Umlauten. `ton.h`: Töne der App.
- `board.h`, `touch.h`, `bildschirm.h`: aus Schritt 8/9.

## Geprüft (23. November 2026)

- Für „Raspberry Pi Pico 2“ ohne Fehler und Warnungen übersetzt: 219 KB Programm, 19 KB Arbeitsspeicher.
- `assets.h` als Vorschaubild angesehen: Zimmer in drei Himmeln, alle Zeichnungen, Grab.
- **Auf dem Board (23. November 2026):** Bildschirmfoto über USB (COM8) zeigt Zimmer mit Nachthimmel, Ei mit Schatten auf dem Teppich, Balken (80/80/100 %) und die Zeile „Evi Ei 0 … Schlüpft in ca. 1 Min.“ an denselben Stellen wie in der App. Menü und Pflege noch nicht rückgemeldet.
