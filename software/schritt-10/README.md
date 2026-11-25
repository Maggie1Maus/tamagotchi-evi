# Schritt 10: Die Handy-App auf dem Waveshare-Board

Wunsch der Nutzerin vom 23. November 2026: **genau das Spiel der Handy-App**, alles an denselben Stellen, nur ohne den Rahmen um das Tamagotchi.

**Beschlüsse vom 23. November 2026:**
- **Bedienung über drei Bildschirm-Drittel:** links tippen = linke Taste, Mitte = mittlere Taste, rechts = rechte Taste der App. Später übernehmen die echten Retro-Gamer-Tasten.
- **Uhrzeit nach dem Einschalten einstellen**, solange die Pufferbatterie der Uhr fehlt.
- **In Etappen:** 1. Zimmer, Balken, Evi mit Lebensphasen, Menü, Tagesablauf → 2. die drei Spiele → 3. Name ändern und Hilfe → 4. Evis Momente, Begrüßung, Tageslaune.

## Öffentlicher Parkeingang

- „In den Park“ steht im Zimmermenü, wenn Evi wach ist. Mit Touch oder Steuerkreuz und A wählt man den Besuch, die Stelle am Kiesweg oder die Stelle am Wiesenrand. Evi läuft selbst dorthin. B, START oder „Nach Hause“ führen ins Zimmer.
- Lotti begrüßt Evi. An einer Fundstelle schnuppert Evi zuerst. Mit A oder einem zweiten Tipper untersucht sie die Stelle und zeigt einen gefundenen Gegenstand. Pro Ortsdatum gibt es einen Fund. Fundort und Datum stehen im Sideboard. Wetter und Jahreszeit werden mitgespeichert.
- Besuch im Park (seit 25.11.2026): Eine der 20 Figuren aus `entwuerfe/neue-tamagotchis/graustufen/` in Evis Lebensphase. Sie wechselt alle 4 Stunden (`parkBesucher`, Lotti etwa jedes dritte Mal). Beim Ankommen sagt sie einen von 50 zufälligen Sätzen. Bilder: `python software/schritt-10/tools/make-besucher-grau.py` → `evi-handy/besucherbilder.h`.
- Der Park verwendet `jahreszeitBei`, `himmelBei` und `wetterBei` wie das Zimmerfenster. `python software/schritt-10/tools/make-park-grau.py` erzeugt `evi-handy/parkbilder.h` aus den Park-PNGs unter `entwuerfe/park/entwurf-1/` und Evis Zeichnungen aus `Evi-Graustufen.zip`.

## Zimmer, Möbel und Fensterausblick in Graustufen (25. November 2026)

- `tools/make-zimmer-grau.py` liest die PNGs aus `entwuerfe/zimmer/graustufen/spielgrafiken/` und erzeugt `evi-handy/zimmergrau.h`. Graustufen und Alpha bleiben getrennt gespeichert. Das nutzt alle 16 Stufen der Anzeige.
- Wand und Dielen sind heller. Die Zimmerlampe bestimmt die Helligkeit der Einrichtung, die Tageszeit bestimmt den Ausblick. Die Möbel nutzen ihre eigenen Deckmasken. Evi schläft hinter dem vorderen Bettrand. Der Rand folgt der Zimmerbeleuchtung.
- Die 66 × 53 Pixel große Fensterfläche beginnt bei (78, 45). Darauf liegen Himmel, Himmelszeichen, Wolken, Hügel, Baum und die saisonalen Ebenen. Der Winterbaum hat kahle Äste mit dezentem Astschnee bei Schneefall. Regen und Schnee laufen in je 16 Phasen hinter dem Holzrahmen.
- Für den Raspberry Pi Pico 2 übersetzt. Die PC-Nachbildung prüft Einrichtung, Bettüberdeckung, Wetter und 411 gerenderte Fensteransichten pixelgenau. Die Prüfung auf dem physischen Waveshare-Display steht aus.

## Graustufen-Anzeige und Evis Graustufen-Bilder (25. November 2026)

Wunsch der Nutzerin: Die neuen Graustufen-Zeichnungen von Evi (alle Lebensphasen, Emotionen, Alltagsposen, Garten, Ei) sollen ins Spiel. Auf die Rückfrage hat sie **Graustufen im Spiel** gewählt; die Regel „nur Schwarz und Weiß“ gilt für das Waveshare nicht mehr.
- **Anzeige:** `bild` ist jetzt eine 8-Bit-Fläche (`GrauCanvas` in `bildschirm.h`, abgeleitet von `GFXcanvas8`); alles bisherige Zeichnen geht unverändert weiter (Farbe 0 = schwarz, Farbe 1 = weiß, dazwischen Werte 2–254). Zur Anzeige werden 16 Stufen als graue RGB565-Farben geschickt; `zeigen()` sendet weiter nur Geändertes (jetzt Punkt für Punkt verglichen). `bild.bits()` liefert einen Schwarz-Weiß-Auszug (für `S` über USB und die PC-Prüfprogramme), `bild.stufe(x, y)` die Stufe. Speicher: 77 KB Bild + 77 KB Vergleichsbild.
- **Evis Bilder:** 82 Graustufen-Sprites aus `Evi-Graustufen.zip` (Ei, Baby, Teenager, Erwachsene, Oma: Ruhe, Emotionen, Beleidigt, Momente, Garten-Posen), 1:1 und gleich groß wie die alten Bilder. `tools/make-evi-grau.py` erzeugt `evigrau.h` (4 Bit je Punkt, 15 Stufen + durchsichtig, 400 KB). Jedes Graustufen-Bild gehört zu einem alten Schwarz-Weiß-Bild; das alte bleibt für Treffer und Verdeckung. `grau.h` zeichnet sie; beim Verkleinern (Tiefe) wird gemittelt. Fehlt zu einem Bild das Gegenstück, wird es wie bisher schwarz-weiß gezeichnet.
- Zum Zeitpunkt dieses ersten Umbaus lagen die neuen Zimmeransichten als vollständige Szenen mit festen Möbeln vor. Die anschließende Umstellung auf Einzelbilder steht im Abschnitt zum Zimmer.
- Bildaufträge für die Fundstücke im neuen Stil: `entwuerfe/sammlung/bildauftraege-graustufen.md`.
- Geprüft: alle elf PC-Prüfprogramme ohne Fehler und Warnungen, Display pixelgenau (16 Stufen); neue Prüfungen für Graustufen bei Evi, alle 82 Zuordnungen, Zimmer bleibt Schwarz-Weiß. **Echte Pico-2-Übersetzung nicht möglich, am Board noch nicht ausprobiert.** Darauf achten: Übersetzt `GFXcanvas8` mit den überschriebenen Funktionen? Größe des Programms (etwa 0,9 MB mehr), Geschwindigkeit des Zeichnens, Aussehen auf dem echten Bildschirm.

## Großer Weihnachtsbaum, Evi schläft im Tierbett (25. November 2026)

Wünsche der Nutzerin nach dem Test von Zimmer V2 und Einrichtungsmodus („funktioniert ganz gut“, auch der Gang zum Sideboard):
- **Weihnachtsbaum viel größer:** steht wie bisher rechts auf dem Boden (Fuß y = 173), reicht jetzt bis unter die Balkenleiste (145 statt 100 Punkte hoch, 84 breit, fast Originalgröße der Zeichnung). Konstanten `XBAUM_*` in `einrichtung.ino`; Zeichnen und Verdeckung nutzen dieselben Werte. Das Kerzentablett steht dadurch zum Teil hinter dem Baum; es wird seitdem direkt mit dem Sideboard gezeichnet, damit es nicht vor dem Baum erscheint (Hinweis der Nutzerin).
- **Evi schläft im Tierbett** (`bett.ino`, Zustand in `extras.h`): Schläft sie ein (nachts oder Mittagsschlaf), läuft sie bei hellem Bildschirm zum Bett, bei dunklem liegt sie gleich dort. Gezeichnet wird ihr vorhandenes Schlafbild in der Größe, die zur Tiefe des Betts passt; danach wird der vordere Rand des Korbs (ab Zeile `BETT_RAND`) noch einmal darübergelegt, so liegt sie im Korb. Die Schlafbilder aller Altersstufen sind innen ganz deckend (geprüft: keine Lücken in der Maske). Beim Aufwachen läuft sie zurück auf den Teppich. Ausgeschlafener wird sie dadurch nicht. Ist das Bett im Inventar, schläft sie wie bisher auf dem Teppich. Während sie hin- oder zurückläuft, beginnen keine Momente.
- **Evis Größe gemessen** (vorne auf dem Teppich, Bildschirm 240 Punkte hoch): Baby 54 (22 %), Teenager 64 (27 %), Erwachsene 68 (28 %), Oma 64 (27 %). Grillme-Ziel für die Erwachsene: etwa 30 % (72 Punkte). Noch nicht geändert.
- Geprüft: alle elf PC-Prüfprogramme ohne Fehler; `einrichtung.cpp` prüft Baumhöhe, Gang ins Bett, Lage im Bett, Rand davor, Rückweg, dunkel gleich im Bett, Bett im Inventar. **Am Board noch nicht ausprobiert.**

## Tieferer Schlaf, Touch im Dunkeln aus, Gassi entfernt (25. November 2026, zweite Fassung)

Gemessen mit der ersten Fassung (unten): Bildschirm an etwa 120 mA, Bildschirm aus 30–47 mA (drei AAA, Multimeter über den leeren vierten Platz). Wunsch der Nutzerin daraufhin: Gassi gehen ganz entfernen, den Chip tiefer schlafen lassen, im Dunkeln Touch grundsätzlich aus.

- **Gassi gehen ganz entfernt:** Schrittzählen, Mittraben, Spaziergang-Belohnung (`gassiBeenden` in `pet.h`), Funde beim Gassigehen (`fundBeimGassi`), die Zeilen „Gassi gehen …“ und „Heute … Schritte zusammen“. Auch das Erkennen von Schütteln ist weg (es diente nur dazu, Schütteln nicht als Gehen zu zählen). Ältere Funde vom Spaziergang bleiben in der Sammlung und zeigen weiter „Vom gemeinsamen Spaziergang mitgebracht.“. Neue Funde kommen nur noch aus dem Garten; dort sind alle zehn Arten erreichbar. Spielstandformat unverändert.
- **Dunkel wacht der Chip nur noch zu jedem Spielschritt auf** (etwa alle 120 ms statt alle 20 ms): Evis Uhr, Momente, Fragen und Tasten laufen weiter. Der Lagesensor misst dabei einmal (`LAGE_DUNKEL_MS`); das Glätten rechnet mit dem echten Abstand, so geht der Bildschirm nach dem Umdrehen weiter nach etwa einer halben Sekunde an. Solange ein Ton spielt, höchstens 20 ms Pause.
- **Dunkel ist Touch aus:** Der Touch-Chip wird im Reset gehalten (`touchSchlafen`/`touchAufwachen` in `touch.h`) und braucht so kaum Strom. Wecken mit jeder Taste, beim Umdrehen oder bei Ereignissen. Nach dem Aufwachen ist Touch nach 150 ms wieder da. Fehlen die Tasten (MCP23017 nicht gefunden), bleibt Touch an, sonst ließe sich Evi nicht wecken. Abschalten: `SCHLAF_TOUCH` in `schlaf.h`.
- `P` im Seriellen Monitor zeigt jetzt auch `touch=an/aus`. Dunkel sollten es über 98 % Schlaf sein.
- Geprüft in der PC-Nachbildung: alle zehn Prüfprogramme ohne Fehler und Warnungen. `schlaf.cpp`: dunkel 83 Runden in 10 s, 99,6 % geschlafen, Lagesensor und Spielschritte je etwa 8-mal pro Sekunde; Touch-Chip im Reset, Antippen weckt nicht, Taste weckt; während eines Tons höchstens 20 ms Pause; aufs Gesicht gelegt, 20 s später umgedreht: Bildschirm nach etwa 0,5 s an. `kuscheln.cpp`: dunkel weckt Antippen nicht, ohne Tasten schon. `bewegung.cpp`: Gehen mit Evi in der Hand löst nichts mehr aus. `sideboard.cpp`: jede Fundart ist im Garten erreichbar.
- **Am Board noch nicht ausprobiert.** Wie viel es spart, zeigt erst die nächste Messung.

## Schlafmodus und Helligkeit 50 % (25. November 2026)

Anlass: Vier neue AAA waren nach weniger als einem Tag leer, obwohl der Bildschirm meist mit RESET aus war. „Bildschirm aus“ schaltete bis dahin nur das Licht ab; der Chip lief ohne Pause mit vollem Takt weiter. Wunsch der Nutzerin: Schlafmodus einbauen, Helligkeit auf 50 %. Neue Datei `schlaf.h`, Anschlüsse in `evi-handy.ino`, `bildschirm.h`, `ton.h`.

| Was | Wann | Abschalten, falls am Board etwas nicht stimmt |
|---|---|---|
| **Helligkeit 50 %** statt 80 % | immer | `HELL_PROZENT` in `evi-handy.ino` |
| **Chip schläft zwischen den Abfragen** (`delay()` wartet beim RP2350 schlafend) | dunkel: bis zur nächsten Messung des Lagesensors, höchstens 20 ms (seit der zweiten Fassung oben: bis zum nächsten Spielschritt, 120 ms); hell: 2 ms je Runde. Nicht in Spielen und nicht, solange ein Finger auf dem Bildschirm liegt | `SCHLAF_CHIP` in `schlaf.h` |
| **Langsamer Takt** 48 statt 150 MHz | nach 3 s dunkel und still. Bildschirm an oder ein Ton: sofort wieder voll | `SCHLAF_TAKT` in `schlaf.h` |
| **Bildschirm-Chip schläft** (ST7789: Anzeige aus 0x28, Schlafen 0x10; Aufwachen 0x11, 5 ms, Anzeige an 0x29) | solange der Bildschirm aus ist. Zwischen Schlafen und Aufwachen mindestens 120 ms (Datenblatt). Nach dem Aufwachen wird einmal das ganze Bild geschickt | `LCD_SCHLAF` in `bildschirm.h` |
| **Ton-Chip nur bei Tönen an** (I²S läuft nur, solange ein Ton spielt, plus 2 s Nachlauf) | immer | `TON_PAUSE_AUS` in `ton.h` (falls es beim Piepen knackt oder der Anfang fehlt) |

- Was weiter geht, auch im Dunkeln: Tasten (der MCP23017 merkt sich jeden Druck), Antippen (seit der zweiten Fassung nicht mehr), Lagesensor 50-mal pro Sekunde (Gassi zählte weiter; seit der zweiten Fassung seltener und ohne Gassi), Evis Uhr und Spielregeln, Piepen bei Fragen und Besuch. Der erste Druck oder Tipper weckt wie bisher nur den Bildschirm.
- **`P` im Seriellen Monitor** zeigt den Stand: `STROM bildschirm=aus licht=0% lcd=schlaeft ton=aus takt=48MHz geschlafen=… von … ms (…%)`. Der Anteil zählt seit der letzten Abfrage. Dunkel sollten es über 90 % sein, hell mehr als die Hälfte.
- Grobe Schätzung, nicht gemessen: dunkel vorher etwa 50 mA, jetzt hoffentlich etwa 5–15 mA. Bildschirm an mit 50 % vielleicht 60–90 mA. Was die `PWR`-LED und die übrigen Bauteile des Boards brauchen, lässt sich per Programm nicht ändern. **Messen:** Multimeter im mA-Bereich in Reihe zwischen Batteriefach und Pololu, einmal hell, einmal dunkel (nach mindestens 5 s).
- Geprüft in der PC-Nachbildung (`tools/pc-nachbildung/schlaf.cpp`, bei der dort beim Schlafen auch die Zeit vergeht): Helligkeit 127 von 255; hell 2-ms-Pausen und Lagesensor weiter 47-mal pro Sekunde; mit Finger und im Spiel keine Pausen; dunkel schläft der Chip 95 % der Zeit, Lagesensor weiter etwa 48-mal pro Sekunde, nach 3 s langsamer Takt; Ton im Dunkeln mit vollem Takt, Ton-Chip danach aus; Wecken mit Taste und Antippen (Takt, Bildschirm-Chip, Licht, Bild ohne falschen Punkt); zweimal RESET schnell hintereinander hält die 120 ms ein; Gassi im Dunkeln zählt 35 Schritte in 20 s. Alle zehn Prüfprogramme ohne Fehler und ohne Warnungen.
- **Nicht geprüft:** Echte Pico-2-Übersetzung war in dieser Cloud-Sitzung nicht möglich (Arduino-Werkzeuge nicht ladbar). Benutzt werden `set_sys_clock_khz` und `clock_get_hz` aus dem Pico-SDK (`hardware/clocks.h`) und `i2s.end()`. Am Board ausprobieren: Startet Evi? Geht der Bildschirm mit RESET aus und wieder an, und ist das Bild danach richtig? Piept sie (ohne Knacken)? Läuft der Ball nach dem Aufwachen so flüssig wie vorher? Zählt Gassi bei dunklem Bildschirm?

## Freundschaft, Fragen und Plaudern (25. November 2026)

Ergebnis der Fragerunde mit der Nutzerin. Neue Datei `freunde.ino`. Zustand in `extras.h` (`FreundStand freund`, `gespraech`), Anschlüsse in `evi-handy.ino`, `momente.ino`, `kuscheln.ino`, `launen.ino`, `spiele.ino`. Die Texte stehen alle oben in `freunde.ino`; die Nutzerin möchte sie **nicht vorher lesen**, sondern sich überraschen lassen.

- **Fragen:** Zu festen, täglich wechselnden Zeiten fragt Evi etwas (Baby 4-mal ab 9/12/15/18 Uhr + bis zu 3 h, Teenager 2-mal ab 9/15 Uhr + bis zu 6 h, Erwachsen/Oma einmal zwischen 10 und 20 Uhr). Nur wenn der Bildschirm an ist, sie wach ist und gerade nichts anderes passiert. Mit Piep. Die Frage steht 5 Minuten in der Blase; unten „Eine Frage an dich · A antwortet“. **A oder START** zeigt die Antworten, B schließt sie wieder (die Frage bleibt stehen). Nach der Antwort verrät Evi ihre eigene, bei Gleichheit „Wir sind gleich!“. **100 Fragen, je 25 pro Altersstufe** (`FRAGEN`; neue nur hinten anfügen, die Nummern stehen im Spielstand, höchstens `FRAGEN_MAX` = 128). Eine Evi stellt in ihrem Leben etwa 15 davon, zufällig ausgewählt.
- **Plaudern** (Menü, wach, nicht als Ei): Evi erzählt ihre Antwort auf eine Frage ihrer Lebensphase, eine von **100 Meinungen** (`MEINUNGEN`, je zwei Fassungen, pro Evi ausgewürfelt, freigeschaltet je Freundschaftsstufe) oder erinnert sich an deine Antwort (ab Stufe 2). Am Geburtstag und am Tag davor oft davon.
- **Jede Evi ist anders:** Ihre Antworten und Meinungen kommen aus `streuwert(geboren …)`, ohne Speicher. Deine Antworten, die Freundschaft und alles Übrige gehören zur jeweiligen Evi und beginnen beim nächsten Ei neu. **Für immer:** Name `NUTZER_NAME` = „Maggie“ und der Geburtstag.
- **Geburtstag:** Einmal fragt Evi „Wann hast du Geburtstag, Maggie?“: Tag und Monat mit ▲▼ ändern, ◀▶ (L/R) wechselt das Feld, A speichert, B fragt in einer Stunde noch mal. Gespeichert nur auf dem Board, **nicht im Repository** (das Projekt ist öffentlich). Am Geburtstag ab 7 Uhr, sobald du da bist: Geburtstagslied, Hüpfen, Herzen, 10 Münzen.
- **Freundschaft** (verborgen, 0–100): + Antwort 4, gleiche Antwort 2, „Weißt du noch?“ richtig 6, Plaudern 1 (3-mal am Tag), Pflege/Kuscheln 0,5, Spiel 1, erfüllter Launenwunsch 2, Geburtstag 5; höchstens **12 am Tag**. − beleidigt 4, krank 8, ein Balken unter 30 je volle Stunde (wach) 2.
- **Stufen** ab 0 / 6 / 18 / 36 / 70: 1 schüchtern (knappe Begrüßung „Oh … hallo.“), 2 bekannt (erzählt, was sie nicht mag, erinnert sich an deine Antworten), 3 vertraut (hüpft dir bei der Begrüßung entgegen, kleine Geheimnisse), 4 Freunde (nennt dich Maggie, einmal am Tag ein kleines Geschenk mit 3 Münzen, „Weißt du noch?“ statt mancher Fragen), 5 beste Freunde (eigene Begrüßung, Träume beim Plaudern).
- **Beste Freunde:** ab 70 Punkten, frühestens als Erwachsene, und **nur, wenn du in den letzten 2 Minuten bedient hast** (sonst wartet der Moment): kleine Fanfare, „Maggie … darf ich dir was sagen?“, Hüpfen, viele Herzen, „Wir sind jetzt beste Freunde! ♥“. Unter 62 Punkten ist es vorbei: Sie sagt traurig etwa „Ich dachte, wir wären beste Freunde …“ (wenn du wieder da bist), und der Moment muss neu verdient werden.
- Mit täglich 12 Punkten: etwa 12 als Baby, 36 am Ende als Teenager, beste Freunde frühestens am 3. Tag als Erwachsene.
- Speicher: `FreundStand` liegt im Flash ab `FREUND_ADRESSE` = 3584 (`speicher.h`), getrennt vom Spielstand, mit eigener Prüfsumme. Version 2 (Platz für 128 Fragen); ein Stand der Version 1 (32 Fragen) wird beim Start mit Geburtstag, Freundschaft und Antworten übernommen. Der Spielstand selbst ist unverändert. Die Zahl der Fragen und Meinungen prüft je ein `static_assert`. Eine vorhandene Evi beginnt beim ersten Start mit Freundschaft 0 (schüchtern).
- Sprechblasen haben jetzt bis zu 4 Zeilen und wachsen nach oben.
- Zu jedem Anlass fünf zufällige Sätze.

**Geprüft:** echte Übersetzung für Pico 2 ohne neue Warnungen; alle Sätze passen rechnerisch in die Blase (15 Zeichen, 4 Zeilen). PC-Nachbildung nicht gelaufen (kein g++ auf dem Haupt-PC), es gibt noch keine eigene Prüfung für `freunde.ino`. **Am Board noch nicht ausprobiert.**

## Launenwünsche mit Folgen, Licht aus nach dem Aufwecken (25. November 2026)

Wunsch der Nutzerin: Wünsche und Launen sollen **Folgen** haben, und zu jedem Anlass soll es **fünf verschiedene Sätze** geben, die zufällig kommen. Neue Datei `launen.ino`; Anschlüsse in `kuscheln.ino`, `momente.ino`, `spiele.ino`, `evi-handy.ino`, Zustand in `extras.h`, Zahlen in `pet.h`.

| Tageslaune | Wunsch (Gedankenwolke) | Erfüllen |
|---|---|---|
| verspielt | „Spielst du mit mir?“ u. a. (Ball) | im Menü ein Spiel starten; sie bedankt sich nach dem Spiel |
| verschmust | „Kuschelst du mit mir?“ u. a. (Herz) | Evi antippen |
| verfressen | „Ich will einen Keks!“ u. a. (Keks) | Naschen oder Füttern |
| verträumt | „Schaust du mit mir raus?“ u. a. | sie geht ans Fenster; Evi antippen |

- Jeder dritte kleine Moment ist der Wunsch der heutigen Laune (vorher nur der Kuschelwunsch). **Nur, wenn in den letzten 10 Minuten jemand eine Taste gedrückt oder getippt hat** – liegt Evi unbeachtet herum, wünscht sie sich nichts und wird nicht beleidigt. Mit Ton.
- Sie wartet **45 Sekunden**. Unten steht, was zu tun ist („Möchte spielen · Menü: Spielen“).
- **Erfüllt:** +10 Stimmung, Münzton, einer von fünf Sätzen („Genau das wollte ich! ♥“ …).
- **Nicht erfüllt:** −5 Stimmung, trauriges Bild, einer von fünf Sätzen („Hmpf … dann eben nicht.“ …).
- **Zwei verpasste Wünsche hintereinander: 15 Minuten beleidigt.** Evi zeigt von Anfang an ihre Schmollpose mit verschränkten Vorderpfoten. Über ihr steht eine kleine Regenwolke, unten „Ist beleidigt · Keks oder dreimal streicheln“. Spiele lehnt sie ab („Spiel doch alleine!“ …), Kuscheln wirkt nicht („Lass mich!“ …), keine kleinen Momente, statt fröhlicher Begrüßung nur „Hmpf.“. **Versöhnen:** Naschen oder Füttern, oder dreimal antippen. Danach („Na gut … ♥“ …) ist alles wie vorher; nach 15 Minuten verzeiht sie auch von selbst.
- Schläft sie ein, wird krank, oder spielst du gerade, bist im Garten oder am Sideboard, endet ein offener Wunsch ohne Folgen.
- Alles nur im Arbeitsspeicher, der Spielstand bleibt unverändert. Zahlen: `WUNSCH_WARTEN`, `JEMAND_DA` (oben in `launen.ino`), `WUNSCH_VERPASST_LAUNE`, `BELEIDIGT_DAUER` (`pet.h`). Die Sätze stehen oben in `launen.ino`.
- Die vier Vorlagen für Baby, Teenager, Erwachsene und Oma liegen unter `entwuerfe/evi/emotionen/`. `tools/make-assets.ps1` schneidet die Beschriftung ab und erzeugt die Bilder mit transparentem Rand im jeweiligen Maßstab für das Display.

**Licht aus nach dem Aufwecken** (Wunsch der Nutzerin): Ist Evi nachts geweckt, steht im Menü „Licht aus“. Damit schläft sie sofort wieder ein (mit Lichtbonus, einmal pro Nacht), statt die 5 Minuten abzuwarten. Ohne Bedienung schläft sie wie bisher nach 5 Minuten von selbst wieder ein.

**Geprüft:** echte Übersetzung für Pico 2 (arduino-pico, `arduino-cli` aus der Arduino IDE) ohne neue Warnungen. `tools/pc-nachbildung/kuscheln.cpp` an das neue Verhalten angepasst (verpasst, beleidigt, kein Spiel, versöhnen), konnte auf dem Haupt-PC aber nicht laufen (kein g++). **Am Board noch nicht ausprobiert.**

## Kuscheln, kleine Uhrzeit, trüber Regenhimmel, kürzeres Menü (24. November 2026)

Wünsche der Nutzerin, nur auf dem Board:
- **Menü:** „Uhr stellen“, „Hilfe“ und „Name ändern“ sind raus (`name-hilfe.ino` gelöscht, spart etwa 20 KB Arbeitsspeicher). Die Uhr wird weiterhin beim Einschalten gestellt, wenn sie nicht läuft (ohne Pufferbatterie nach dem Ausstecken); dieser Bildschirm geht mit Tasten und mit Touch.
- **Uhrzeit** klein oben an der Wand zwischen Fenster und Evi (`zeichneUhrzeit`, `UHRZEIT_X/Y` in `evi-handy.ino`), mit weißem Feld dahinter, damit sie auch bei Licht aus lesbar bleibt. Dort ist in jeder Jahreszeit und an Weihnachten Platz. Nicht im Spiel und nicht in der Sammlung.
- **Regen:** Am Tag ist der Himmel bei Regen trüb, gepunktet wie in der Dämmerung. Die schwarze Regenwolke und der fallende Regen bleiben; die Regenstriche haben vor dem gepunkteten Himmel links und rechts einen hellen Rand, sonst sähe man sie kaum (`fensterZeichnen` in `umgebung.ino`). Nachts bleibt alles wie vorher.
- **Touch ist keine Taste mehr.** Evi antippen = **kuscheln**, mit dem Finger über sie streichen = weiter kuscheln (`kuscheln.ino`). Daneben tippen tut nichts. Nur wenn der MCP23017 mit den Tasten nicht gefunden wird (Kabel lose), gelten als **Notlösung** die drei Drittel wie früher, damit Evi bedienbar bleibt. Der erste Tipper auf den dunklen Bildschirm macht ihn nur an.

| Evi ist … | Antippen |
|---|---|
| wach (auch krank, hungrig, traurig) | Sie strahlt (glückliches Bild), wiegt sich, Herzen steigen auf, sie sagt etwas Liebes („Hihi! ♥“, „Hab dich lieb! ♥“; hungrig „Schön! Aber ich hab Hunger …“). Unten: „Evi wird gekuschelt ♥“. **+5 Stimmung**, an verschmusten Tagen **+8**, höchstens einmal pro Minute (Regel `kuscheln` in `pet.h`). |
| mitten in einem kleinen Moment (Spaziergang, Fenster …) | Der Moment hört auf, Evi bleibt beim Kuscheln stehen, wo sie ist, und geht danach zum Teppich zurück. |
| bei Besuch, Gassi, rollendem Ball, beim Trösten | Nur Herzen (und die Stimmung), das Ereignis läuft weiter. |
| schlafend | „Evi kuschelt sich im Schlaf an dich.“, ein Herz; sie wacht nicht auf, keine Wirkung. |
| Ei | Das Ei wackelt: „Das Ei ist ganz warm.“ |
| gestorben, Menü offen, im Spiel, bei Gute Nacht, Sammlung | nichts |

**Tageslaune „verschmust“ hat jetzt eine Bedeutung:** Kuscheln bringt mehr (+8 statt +5), und Evi möchte oft von selbst gekuschelt werden (**Kuschelwunsch**, jeder dritte kleine Moment; an anderen Tagen etwa jeder zwölfte): Sie fragt „Kuschelst du mit mir?“, dann steht ein Herz in ihrer Gedankenwolke, unten „Möchte kuscheln · tipp sie an ♥“. Wer sie in diesen etwa 13 Sekunden antippt, erfüllt den Wunsch: **+10 Stimmung**, Münzton, „Juhu! Ich hab dich lieb! ♥“, mehr Herzen. Sonst sagt sie „Später vielleicht! ♥“ – ohne Folgen. Nicht, solange ein anderer Wunsch in der Blase steht. Unten steht an verschmusten Tagen „Heute bin ich verschmust · tipp mich an!“, sonst im Wechsel mit „A öffnet das Menü“ auch „Tipp mich an zum Kuscheln ♥“.

**Prüfen am Board:** Seriellen Monitor öffnen (115200 Baud) und `T` senden: Das Board meldet den Touch-Chip, wo der letzte Tipper war und wo Evi gerade steht. Liegt der Tipper deutlich neben Evi, obwohl man sie getroffen hat, stimmt die Touch-Richtung nicht (`TOUCH_SPIEGELN_X/Y` in `touch.h`). Bisher war nur die waagerechte Richtung durch die drei Drittel bestätigt.

**Geprüft in der PC-Nachbildung** (`tools/pc-nachbildung/kuscheln.cpp`, mit nachgebildetem Touch-Chip CST328): Menü ohne die drei Punkte; Tippen neben Evi tut nichts; ohne Tasten öffnet die Mitte das Menü (Notlösung); Antippen kuschelt (+5, Satz, Herzen), zweites Antippen in derselben Minute ohne Wirkung, nach einer Minute wieder; 4 s Streicheln hält das Kuscheln an; verschmust +8; Kuschelwunsch (unter 300 Momenten verschmust 107-mal, verspielt 20-mal), erfüllt +10, unbeantwortet ohne Folgen; am Fenster unterbrochen und zurückgegangen; Schmetterling läuft weiter; Schlaf, Ei, Grab, Menü offen, Spiel; Wecken des dunklen Bildschirms kuschelt nicht; Uhrzeit und Regenhimmel. Alle anderen Prüfungen weiterhin ohne Fehler, ohne Warnungen übersetzt. **Die echte Übersetzung für Pico 2 war in dieser Sitzung nicht möglich** (Arduino-Werkzeuge ließen sich nicht laden); in der Arduino IDE übersetzen und am Board ausprobieren.

## Etappe 1

### Ergänzung vom 24. November: Baby deckend, Regen und Schnee bewegt

Die weiße Deckmaske des laufenden Babys ist repariert: Bauch und Beine verdecken den Boden auch beim gespiegelten Laufen. Größe und schwarze Zeichnung bleiben gleich. Nur `ART_BABY_LAUFEN_M` ist geändert; das Werkzeug `tools/make-momente.mjs` berücksichtigt die Füllung auch beim Neuerzeugen. Ohne Browser: `node software/schritt-10/tools/make-momente.mjs --repair-running-mask`.

Regen und Schnee werden etwa alle 40 ms gezeichnet, unabhängig vom 120-ms-Takt der Spielregeln. Schnee schwingt leicht seitlich. Nachts fallen helle Tropfen/Flocken vor dem dunklen Himmel. Baum, Hügel und Fensterkreuz verdecken den Niederschlag; bei ausgeschaltetem Bildschirm wird nichts gezeichnet. Schnee weiterhin nur im Winter. In der PC-Nachbildung und mit einer echten Pico-2-Übersetzung geprüft; am Board noch ausprobieren.

- **Zimmer** wie in der App, pixelgenau: von Microsoft Edge aus `handy-app/dist/index.html` abfotografiert (320 × 224, also 1:1). Das Fenster zeigt Tag, Dämmerung oder Nacht mit Mond und Sternen nach der echten Sonne (`himmel.h` = `sky.js`).
- **Evi** mit allen Zeichnungen: Ei, Baby, Teenager, Erwachsen, Oma, je mit glücklich, hungrig, traurig, müde, krank, schmutzig, schlafend, im Maßstab der App, auf dem Teppich (Fußpunkt 167/180). Atmen (2 Pixel), Ei wackelt, Schatten. Nach dem Tod das Grab aus der App.
- **Balken oben** (Napf, Herz, Glanz mit Prozent; schwarz hinterlegt ab 30 oder weniger), **Zeile unten** (Name, Phase, Münzen und Evis Satz), **Menü rechts** mit Symbolen, **Wunschblase**, **zZz-Kasten**, **Reaktionszeichen** (♥ ♪ ✦ +) und die Hüpf-, Kau- und Wackelbewegung.
- **Spielregeln** vollständig aus `pet.js` übernommen (`pet.h`): Lebenslauf, Abnahme je Phase, Schlaf 23–7 Uhr, Licht aus, Gute Nacht mit Geschichte, Aufwecken, Mittagsschlaf, Mahlzeiten, Naschen mit Bauchweh, Wünsche, Medizin, Krankheit, Tod, neues Ei, Münzen.
- **Menü** wie in der App: Füttern (oder Frühstück, Mittagessen, Abendessen), Naschen, Spielen, Saubermachen, Medizin, Gute Nacht, Aufwecken, Licht aus/an, Neues Ei, Ton an/aus. Neu für das Board: **Uhr stellen** (seit 24.11.2026 wieder aus dem Menü genommen, wie Name ändern und Hilfe).
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

**Etappe 3: Name ändern und Hilfe** (`name-hilfe.ino`), statt der Dialoge der App eigene Bildschirme. **Seit 24.11.2026 auf Wunsch der Nutzerin entfernt** (braucht sie nicht); so war es gebaut:
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

**Achtung Batterien:** Die Hintergrundbeleuchtung braucht den meisten Strom. Dauernd an leeren 4 × AAA deutlich schneller als mit Abschalten nach 1 Minute; wie lange sie halten, ist noch nicht gemessen. Mit Batterien also RESET zum Ausschalten nutzen, oder `DUNKEL_NACH_MS` wieder setzen. Am 25. November 2026 waren vier neue AAA nach weniger als einem Tag leer; seitdem gibt es den Schlafmodus (siehe oben).

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

## Schütteln entfernt (24. November 2026)

Wunsch der Nutzerin nach dem Aufspielen („funktioniert“): Das Wackeln stört. Evi reagiert nicht mehr auf Schütteln oder Wackeln: kein Schwindel-Moment, keine Sätze („Hui! Alles dreht sich!“, „Hmpf …“, „Bitte nicht schütteln“), das Ei wackelt dabei nicht mehr. Entfernt: `geschuettelt()`, `M_SCHWINDEL`, `A_WACKELN`, die Hilfe dazu. `lage.h` erkennt Schütteln nur noch intern, damit es nicht als Gehen zählt. Gassi gehen, Kippen und Aufs-Gesicht-Legen bleiben. In der Nachbildung geprüft: Wackeln und Schütteln lösen nichts aus.

## Sterne und Kerzen nach den Bildern der Nutzerin (24. November 2026)

Die Nutzerin hat `entwuerfe/umgebung/fenster-sterne.png` und `kerzentablett.png` hochgeladen (durchsichtiger Hintergrund). Sie ersetzen Claudes Platzhalter, aufbereitet wie der Weihnachtsbaum (nur verkleinert und schwarz-weiß, `make-umgebung.py`): Sternengirlande über dem Fenster, die hängenden Sterne außen neben dem Rahmen; Kerzentablett auf dem Sideboard, die Kerzen stehen vor dem unteren Teil des Fensters. Größe nach Wunsch der Nutzerin etwas kleiner als zuerst: beide 1 : 19 (Girlande 102 × 33, Tablett 57 × 41 Punkte; vorher 1 : 16 und 1 : 15). In der Nachbildung am Tag und in der Nacht angesehen, am Board noch nicht.

## Nach dem ersten Test am Board: Bewegung ruhiger, Weihnachten größer (24. November 2026)

Rückmeldung der Nutzerin (am Board ausprobiert): In der Hand stillgehalten und beim Kippen zählte Evi Schritte, beim Wackeln passierte nichts, beim Ablegen auf den Tisch wurde ihr schwindelig. Weihnachtsdeko viel zu klein, der Baum darf nicht auf dem Tisch stehen.
- **Bewegung neu erkannt** (`lage.h`): Gemessen wird nur noch die Bewegung ohne Schwerkraft. **Schritte** zählen erst nach **10 gleichmäßigen** Schritten (je 0,33–1,1 s, jeder Abstand höchstens 35 % anders als der vorige, Ausschlag über 0,18 g), und dreht sich das Gerät dabei um mehr als etwa 20°, fängt die Zählung neu an. **Schütteln/Wackeln:** 5 Stöße über 0,7 g, je mindestens 90 ms auseinander, in 1,5 s; ein Aufsetzen auf den Tisch gibt nur ein, zwei Stöße. **Kippen** ab etwa 27° für 0,5 s und nur, wenn das Gerät sonst ruhig ist.
- **Maßstab des Sensors** wird beim Stillliegen gelernt (Messung in Ruhe = 1 g). Falls der Sensor auf dem Board anders eingestellt ist als erwartet, stimmen die Schwellen trotzdem.
- **Prüfen am Board:** Seriellen Monitor der Arduino IDE öffnen (115200 Baud) und `L` senden. Das Board meldet Sensor, Byte-Reihenfolge, gelernten Maßstab, Messwerte, gelernte Achsen, Unruhe, Neigung und Schrittzählung.
- **Weihnachten:** Der Baum steht auf dem Boden (150 Punkte hoch), der Tisch ist dann weg (Tisch, Regal und Pflanze sind jetzt alle eigene Ebenen in `umgebung.h`). Sterne am Fenster und Kerzentablett doppelt so groß. Die Zeichnungen der Nutzerin für Sterne und Kerzen sind noch nicht im Repository; bis dahin die von Claude gezeichneten.
- Geprüft in der PC-Nachbildung (`bewegung.cpp`, neue Fälle wie am Board): 2 Minuten in der zitternden Hand, 1 Minute hin und her kippen, fünfmal auf den Tisch legen – keine Schritte, kein Schwindel; Wackeln mit ±1 g, 3-mal pro Sekunde – Schwindel; Sensor im doppelten Maßstab – nach 8 s Ruhe gelernt, dann alles wie sonst. Gehen 4 Minuten: 442 Schritte.

## Neues Zimmer, Jahreszeiten, Wetter und Weihnachtsdeko (24. November 2026)

Wunsch der Nutzerin: die beschlossene Gestaltung „Gemütlich & schlicht“ aus `entwuerfe/umgebung/` einbauen, nur auf dem Board.
- **Bilder:** `tools/make-umgebung.py` erzeugt `evi-handy/umgebung.h` aus den Entwürfen. Nur ausgeschnitten, verkleinert (Möbel 1 : 5,25, Napf 1 : 10,5, Pflanze 1 : 7, Weihnachtsbaum auf 119 Punkte Höhe) und in Schwarz/Weiß umgerechnet. Von Claude gezeichnet, weil es kein Bild dafür gibt: Dielenboden (nach dem Muster „Boden“). Sterne am Fensterrahmen und Kerzentablett seit dem 24.11.2026 nach den Bildern der Nutzerin (siehe oben). Fenster, Teppich und Tapete kommen aus dem bisherigen Zimmer.
- **Zimmer:** Sideboard links (statt der Kommode), Tisch rechts mit der Topfpflanze darauf, Wandregal oben rechts, Napf mit Pfote vorn rechts, Dielenboden. Evi, Teppich und Fenster stehen wie vorher.
- **Jahreszeiten** (Baum und Hügel vor dem Fenster, aus `fenster-jahreszeiten-referenz.png`): Frühling ab 1. Februar, Sommer ab 1. Juni, Herbst ab 21. September, Winter ab 1. November (deutsches Datum). Die vier Scheiben der Vorlage liegen einzeln auf den Scheiben des Spielfensters; dessen Kreuz liegt davor. Nachts werden die Umrisse zum Himmel hin hell gezeichnet, sonst verschwänden die Äste im schwarzen Himmel. Flächen, die ganz von Linien umschlossen sind (unter der Krone bis zu den Hügeln, zwischen Ästen und Rahmen), sind in `HIMMEL_EXTRA` in `make-umgebung.py` von Hand als Himmel markiert; seit dem Hinweis der Nutzerin vom 24.11.2026 auch bei Frühling und Sommer (vorher nachts weiß).
- **Wetter**, ohne Internet: Sonne, Sonne mit Wolken, Regen (eine Regenwolke, schräge Striche fallen), bewölkt (zwei Wolken), Schnee (Wolke, Schneesterne fallen) — **Schnee nur im Winter**. Es wechselt alle 3 Stunden, zufällig je nach Jahreszeit, aber für jeden Tag fest (Streuwert aus Datum und Uhrzeit, nach einem Neustart gleich). Gewichte (Sonne, Sonne+Wolken, Regen, bewölkt, Schnee): Frühling 3/3/3/2/0, Sommer 6/3/2/1/0, Herbst 2/3/4/4/0, Winter 2/2/2/3/4 (`WETTER_GEWICHT` in `umgebung.ino`). Zu sehen am Tag und in der Dämmerung; nachts bleibt der Himmel mit Mond und Sternen wie bisher. „Aus dem Fenster schauen“ sagt jetzt, was draußen ist („Schaut dem Regen zu“, „Oh, es schneit!“ …). Liegender Schnee ist nicht eigens dargestellt; der Winterbaum hat Schnee auf den Ästen wie im Entwurf.
- **Weihnachten vom 18. November bis einschließlich 5. Januar:** Der Weihnachtsbaum steht statt Wandregal und Pflanze (seit dem Test am Board auf dem Boden, der Tisch ist dann weg, siehe oben), Sterne hängen am Fensterrahmen, ein Kerzentablett steht auf dem Sideboard. Für Sterne und Kerzen gilt derselbe Zeitraum wie für den Baum. Ab 6. Januar sind Regal und Pflanze zurück.
- **Geprüft in der PC-Nachbildung** (`tools/pc-nachbildung/umgebung.cpp`): alle Datumsgrenzen; ein Jahr Wetter (kein Schnee außerhalb des Winters, Wechsel nur alle 3 Stunden); Bilder aller 17 Kombinationen am Tag, jede Jahreszeit nachts, Dämmerung, Weihnachten Tag und Nacht angesehen. **Am Board noch nicht ausprobiert.**

## Evi spürt Bewegung, „Wie war dein Tag?“, zwei neue Spiele (24. November 2026)

Wunsch der Nutzerin: Bewegungen spüren (Gassi gehen, schütteln, kippen, aufs Gesicht legen), „Wie war dein Tag?“ und neue Spiele, aber nicht Verstecken oder Ball bringen. **Nur auf dem Board**, die Handy-App bleibt unverändert (Wunsch der Nutzerin).

**Lagesensor** (`lage.h`): der QMI8658 auf dem Board (I²C 0x6B oder 0x6A, an Wire1), nur Beschleunigung, ±4 g, 50 Messungen pro Sekunde. Die Reihenfolge der Bytes wird beim Start erkannt (in Ruhe misst er 1 g). Wie der Chip auf der Platine liegt, **lernt Evi beim Benutzen**: Bei jedem Tastendruck und Tipper wird das Gerät mit dem Bildschirm nach oben gehalten. Nach drei ruhigen Bedienungen kennt sie die Bildschirmrichtung, die Oberkante, sobald das Gerät dabei schräg gehalten wird. Kippen und Aufs-Gesicht-Legen wirken erst danach; Schritte und Schütteln sofort. Nach jedem Neustart lernt sie es neu (nur im Arbeitsspeicher).

| Bewegung | Erkennung | Was Evi tut (`bewegung.ino`) |
|---|---|---|
| **Gassi gehen** (am 25.11.2026 ganz entfernt, siehe oben) | Schritte im Takt (6 hintereinander, je 0,25–1,5 s Abstand); Ende nach 60 s ohne Schritt | Bei hellem Bildschirm trabt sie mit (Bild „laufen“), unten „Gassi gehen … 123 Schritte“. Ab 100 Schritten nach dem Spaziergang: „Das war schön draußen!“, +10 Stimmung (+5 je volle 1000 Schritte, höchstens +25), 1 Münze je 500 Schritte (höchstens 10), Sättigung und Sauberkeit je −5 (Regel `gassiBeenden` in `pet.h`). Nur wenn sie wach und gesund ist. Endet der Spaziergang bei dunklem Bildschirm, erzählt sie es beim Einschalten. Unten steht im Wechsel „Heute … Schritte zusammen“. |
| **Schütteln** (Reaktion am 24.11., Erkennung am 25.11.2026 entfernt) | 4 Stöße über 1,8 g in 1,2 s | „Hui! Alles dreht sich!“, sie wackelt (Bild „drehen“), dann „Mir ist ganz schwindelig …“; beim dritten Mal in 2 Minuten „Bitte nicht mehr schütteln!“. Schlafend: „Hmpf … Evi schläft.“ Krank: „Mir geht es nicht gut …“. Das Ei wackelt. Keine Folgen für die Werte. |
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
- Touch über die drei Drittel funktioniert seit 24.11.2026 nur noch als Notlösung, wenn der MCP23017 fehlt; sonst ist Touch zum Kuscheln da (siehe oben). Das Programm sucht ihn alle 2 Sekunden.
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

- `tools/make-assets.ps1` erzeugt `evi-handy/assets.h` aus der Handy-App und den vier Beleidigt-Zeichnungen unter `entwuerfe/evi/emotionen/`. Braucht Microsoft Edge. Nach Änderungen an den Quellen erneut ausführen.
  `powershell -ExecutionPolicy Bypass -File software\schritt-10\tools\make-assets.ps1`
- `tools/preview-assets.ps1`: zeigt den Inhalt von `assets.h` als PNG.
- `tools/bildschirmfoto.ps1`: holt über USB ein Bildschirmfoto vom laufenden Board (Zeichen `S` über die serielle Schnittstelle). Seriellen Monitor der IDE vorher schließen.
- `tools/make-umgebung.py`: erzeugt `evi-handy/umgebung.h` (Zimmer, Jahreszeiten, Wetter, Weihnachten) aus `entwuerfe/umgebung/`. Braucht Python mit Pillow und numpy (Cloud).
- `tools/make-momente.mjs`: erzeugt `evi-handy/momente.h` (Evis Zeichnungen der Momente, Symbole der Gedankenwolke, Noten). Läuft in der Cloud mit Node und Playwright; auf dem Haupt-PC nicht nötig.
- `tools/pc-nachbildung/`: Prüfung am PC ohne Arduino IDE (siehe dortige README).

## Dateien

- `evi-handy.ino`: Bildschirm, Menü, Tasten-Drittel, Ereignisse, Gute-Nacht-Ablauf, Uhr stellen.
- `spiele.ino`, `momente.ino`, `extras.h`, `momente.h`: Etappen 2 und 4 (siehe oben). `tasten.h`: echte Tasten über den MCP23017.
- `kuscheln.ino`: Evi antippen oder streicheln, Kuschelwunsch (seit 24.11.2026).
- `lage.h`, `bewegung.ino`: Lagesensor und was Evi bei Bewegung tut, „Wie war dein Tag?“ (seit 24.11.2026).
- `umgebung.ino`, `umgebung.h`: neues Zimmer, Jahreszeiten, Wetter und Weihnachtsdeko (seit 24.11.2026; `tools/make-umgebung.py`).
- `pet.h`: Spielregeln aus `pet.js`. `zeit.h`: Datum und deutsche Ortszeit mit Sommerzeit. `himmel.h`: Sonnenstand aus `sky.js`.
- `uhr.h`: Uhr PCF85063 (I²C 0x51). `speicher.h`: Spielstand mit Prüfsumme. `text.h`: Schrift mit Umlauten. `ton.h`: Töne der App.
- `schlaf.h`: Schlafmodus, damit die Batterien länger halten (seit 25.11.2026).
- `board.h`, `touch.h`, `bildschirm.h`: aus Schritt 8/9.

## Geprüft (23. November 2026)

- Für „Raspberry Pi Pico 2“ ohne Fehler und Warnungen übersetzt: 219 KB Programm, 19 KB Arbeitsspeicher.
- `assets.h` als Vorschaubild angesehen: Zimmer in drei Himmeln, alle Zeichnungen, Grab.
- **Auf dem Board (23. November 2026):** Bildschirmfoto über USB (COM8) zeigt Zimmer mit Nachthimmel, Ei mit Schatten auf dem Teppich, Balken (80/80/100 %) und die Zeile „Evi Ei 0 … Schlüpft in ca. 1 Min.“ an denselben Stellen wie in der App. Menü und Pflege noch nicht rückgemeldet.

## Sammlung im Sideboard (24. November 2026)

Über **Sideboard öffnen** im Menü: Evi läuft etwa eine Sekunde nach links, steht kurz von hinten vor dem Sideboard und öffnet eine eigene Sammlungsansicht. Das Zimmer verschwindet. Ein Fundstück pro Seite, mit Bild, Name, Fundort, Funddatum und ursprünglicher Generation. Ältere Funde heißen beim nächsten Ei „Erbstück“. Nichts wird auf das Sideboard gestellt; Kerzen bleiben dort.

- Tasten: ◀/▶ (auch ▲/▼) blättern, B/A/START schließen. Touch: links zurückblättern, Mitte schließen, rechts weiterblättern. RESET schließt und schaltet aus.
- Beim Schließen läuft Evi zum Teppich zurück. Wenn sie schläft, ein Ei oder gestorben ist, kann man die Sammlung direkt öffnen, ohne sie zu wecken oder ihre Phase zu ändern.
- Bereits beim **Gassigehen** Fundstücke finden (Wunsch der Nutzerin): ab 100 Schritten erster Fund sicher, danach 40 % je Spaziergang, höchstens eines pro echtem Tag. Inzwischen zehn verfügbare Arten, siehe den Abschnitt zu den neuen Fundstückbildern. Die Werte stehen in `sammlung.h`.
- 256 Einträge, kein automatisches Löschen. Spielstand Version 2 enthält die Sammlung; alte Spielstände werden mit Namen, Münzen, Bedürfnissen und Ton übernommen. Die Sammlung bleibt bei einem neuen Ei erhalten.
- Neue Fundstückbilder mit Bildgenerierung erstellt, nur zugeschnitten, verkleinert und in Schwarz-Weiß umgerechnet (`tools/make-sammlung.py`). Evis vorhandene Zeichnungen werden wiederverwendet.
- Persönlichkeit und Park-Funktionen bleiben offen.

Prüfung: PC-Nachbildung einschließlich Sideboard-Weg, Rückansicht, Blättern, Zurückgehen, RESET, Schlaf, Fundgrenzen, Generationen und Übernahme eines alten Spielstands; echte Übersetzung für Pico 2. Am Board noch ausprobieren.

## Neue Fundstückbilder und Fundarten (24. November 2026)

Der Fundkatalog ist von vier auf zehn verfügbare Arten erweitert. Neu: Micky-Knopf, Arielles Muschel, eigener kleiner Gartendrache, Triforce-Anhänger, Animal-Crossing-Spiralfossil und Junimo-Figur. Sie können bereits beim Gassigehen gefunden und im Sideboard angesehen werden. Alle Fundregeln bleiben gleich, Gegenstandsart und Tag werden weiterhin gespeichert. Die alten Fund-IDs 0–3 und das Spielstandformat bleiben unverändert.

Bilder: `entwuerfe/sammlung/`; Erzeugung des Headers mit `python software/schritt-10/tools/make-sammlung.py`. Namen und Bilder werden beim Kompilieren auf dieselbe Anzahl geprüft. Die neuen Bilder sind außen transparent und innen deckend weiß. Im PC-Test wird jede Art angezeigt, gespeichert/geladen und durch ein Jahr Spaziergänge auf Erreichbarkeit geprüft.

Gewünscht sind elf Motive. Stitch ist noch offen, da die Bildgenerierung abgelehnt wurde; es gibt dafür keinen Platzhalter in der zufälligen Fundauswahl. Der eigene Gartendrache entstand nach dem ebenfalls abgelehnten Spyro-Bild und der anschließenden Erlaubnis der Nutzerin, einen ähnlichen Drachen zu gestalten. Er ist ein eigener Entwurf und wird nicht als Spyro beschriftet.

## Gemeinsamer Garten (24. November 2026)

Im Zimmermenü **In den Garten** wählen (wach, ab Baby bis Oma). Zimmer und Sideboard bleiben übersichtlich; der Garten ist eine eigene Ansicht. Baum, Busch und Blumenbeet sind die drei Suchstellen. Links/rechts oder hoch/runter wählen räumlich die Stelle, **A** schickt Evi hin. Sie läuft selbst, schnuppert und wartet mit **?**. Erst **A** oder Antippen dieser Stelle hilft ihr beim Untersuchen. Ein Fund bleibt mit Evis Zeigepose sichtbar, bis A gedrückt wird. B/START beziehungsweise unten rechts antippen führt heim. Ohne erkannte Tasten: links nächste Stelle, Mitte bestätigen, rechts heim. RESET beendet den Garten und schaltet das Display aus. Zur Schlafenszeit geht Evi heim; Bedürfnisse und Uhr laufen weiter.

Startregeln: jede Stelle einmal pro echtem Ortsdatum; mindestens eine von drei Stellen ist täglich ergiebig, andere mit 20 %. Die benutzten Stellen werden auch bei erfolgloser Suche gespeichert. Kein Sammelset-Ziel. Alle zehn bestehenden Arten können vorkommen. Zusätzlich bleiben echte Spaziergänge (ab 100 Schritten; erster ausreichender Spaziergang sicher, danach 40 %, höchstens ein Fund am Tag). Gartenfunde verbrauchen dieses Spaziergangslimit nicht. Maximal 256 Einträge, niemals ältere Funde überschreiben.

Ein Gartenfund merkt Gegenstandsart, Ortsdatum, genaue Stelle, Jahreszeit, damaliges Spielwetter und bestehende Generation. Sideboard zeigt Bild, Name, Herkunft, Datum und einen Erinnerungssatz aus diesen gespeicherten Daten. Bei echten Spaziergängen ist das reale Wetter unbekannt; es wird kein gemessenes Wetter behauptet. Spielstand **Version 3** übernimmt Version 1 und 2 samt Namen, Bedürfnissen, Münzen, Ton und vorhandenen Funden. Keine neue Verpaarungs- oder Generationenmechanik.

Garten und Fenster verwenden dieselben Funktionen für Jahreszeit, 3-Stunden-Spielwetter und Sonnenzeiten. Vier Varianten von Baum, Busch und Beet; Sonne, Wolken, fallender Regen, langsam pendelnder Schnee, nachts Mond/Sterne und helle Konturen. Sterne am Zaun, Kranz an der Tür und Schneemann vom 18. November bis einschließlich 5. Januar wie die Zimmerdeko. Gartenbilder einzeln unter `entwuerfe/garten/`, Gesamtansichten dort unter `vorschauen/`. Zwölf neue Evi-Posen unter `entwuerfe/evi/garten/`: schnuppern, mit einer Pfote untersuchen, stolz zeigen; jede Pose für Baby, Teenager, Erwachsene und Oma. Die bisherigen Zeichnungen bleiben unverändert. Fundbilder sind getrennte Ebenen auf den Pfoten. Alle neuen PNGs strikt schwarz-weiß, außen transparent, Körper innen deckend.

`python3 software/schritt-10/tools/make-garten.py` erzeugt `gartenbilder.h` reproduzierbar aus den PNGs. Die vollständigen Bildaufträge stehen in `entwuerfe/garten/bildauftraege.json`. `tools/pc-nachbildung/garten.cpp` prüft Suche, Touch/Tasten, Lebensphasen, Persistenz/Migration, Tageslimits, Wetterbewegung und Bildschirm aus. Gesamte PC-Prüfung und echte Pico-2-Übersetzung erfolgreich; am echten Board noch ausprobieren. Stand der Übersetzung: 392880 Bytes Programm (9 %), 33288 Bytes globale Variablen (6 %), Spielstand 3288 Bytes im 4096-Byte-EEPROM-Bereich. Der RAM-Wert enthält keine Laufzeitbelegung von Stack/Heap.


## Garten-Gestaltung überarbeitet und bestätigt (24. November 2026)

Nach der Rückmeldung zu uneinheitlichen Strichen und zusammengewürfelten Elementen hat die Nutzerin den neuen Gesamtentwurf ausdrücklich bestätigt. Dieser ist jetzt die Grundlage der Spielansicht: fünf zusammenhängende saisonale Gartenebenen (`entwuerfe/garten/v2/`), zusätzlich 17 einzeln verwendbare Objekte (`v2/einzelobjekte/`). Der Garten wird als gestaltete Umgebung gezeichnet. Der bestätigte Sommerentwurf liefert die Sommerkonturen direkt; Jahreszeiten und Weihnachtsfassung erhalten die Perspektive und die Anordnung. Frühere Dateien bleiben nachvollziehbar erhalten.

Evi steht kleiner auf der freien Wiese, alle Altersstufen und Suchposen bleiben verfügbar. Beetsuche rechts vom Weg. Der Himmel ist freigestellt für Tageslicht, ruhige Wolkenschichten, Regen/Schnee und Nacht; keine aufgesetzten Wetterzeichen. Der Niederschlag bewegt sich auch über der offenen Wiese hinter Evi. Deckmasken verhindern, dass der Himmel durch Rasen, Schnee und Zaun scheint. Die Wetterberechnung entspricht weiterhin dem Fenster. Weihnachten bleibt vom 18. November bis einschließlich 5. Januar, mit Kranz, kleinem Schneemann und runder Lichterkette.

Generator: `python3 software/schritt-10/tools/make-garten-szene.py` erzeugt `gartenlandschaft.h` aus den kleinen PNG-Quellen. Erstaufbereitung unterstützt `--prepare <Szenenmanifest>` und `--prepare-objects <Objektmanifest>`. Die Bildaufträge stehen bei den Quellen. `make-garten.py` bleibt für die unveränderten Evi-Posen/älteren Quellen erhalten. Screenshots des tatsächlich gerenderten Spiels unter `v2/vorschauen/`. Keine Änderung des Spielstandformats oder der Fundregeln.

Prüfung dieser Fassung: gesamte PC-Nachbildung mit acht Prüfprogrammen erfolgreich und ohne Warnungen; neue Prüfungen für freie Himmelsmaske, deckenden Garten, jahreszeitliche Gesamtebenen, Weihnachtsgrenzen, ruhigen Sonnenhimmel und Niederschlag tags/nachts. Echte Pico-2-Übersetzung erfolgreich: **444328 Bytes Programm (10 %)**, **33288 Bytes globale Variablen (6 %, ohne Stack/Heap)**. Spielstand unverändert 3288 Bytes. Am echten Waveshare noch prüfen: Bildwirkung, Geschwindigkeit, Touch und Ton.


## Kompaktes Zimmer und Raumtiefe (24. November 2026)

Die Nutzerin hat den ersten, kompakteren Zimmerentwurf und die Umsetzung freigegeben. Neues Zimmer aus `entwuerfe/zimmer/kompakt/` mit Weihnachtsfassung im bisherigen Zeitraum 18.11.–5.1. Das Fenster zeigt die tatsächlich gerenderte Gartenansicht mit derselben Jahreszeit und demselben Wetter. Vogel und Sternschnuppe liegen weiter hinter Rahmen/Kreuz.

`raumtiefe.h/.ino` zeichnet Evi in Zimmer und Garten aus derselben Bodenposition: 65 % hinten bis 100 % vorne, kontinuierlich, von den Füßen aus skaliert. Die Altersstufe bestimmt die Grundgröße, die vorhandene Zeichnung die Pose. Figur bleibt innen weiß deckend, auch gespiegelt. Vordergrundstempel maskieren nur Evi; dabei bleiben die Originalpixel der gesamten Umgebungszeichnung sichtbar. Verdeckte Körperteile werden beim Touch nicht getroffen.

Zimmer: kleiner Spaziergang mit Tiefe, zum Fenster seitlich um das Sideboard, anschließend zurück zum Teppich. Kuscheln lässt Evi an ihrem aktuellen Platz. Sideboard nutzt denselben Größenwechsel. Garten: seitlich über die freie Wiese und danach in die Tiefe zu Baum/Busch/Beet. Zur Untersuchung kleiner; nachher kommt Evi wieder vor und zeigt den Fund. Mit A vollständig auf die Wiese zurück, B/START heim. RESET und Schlaf räumen die Szenenpositionen auf. Minispiele verwenden weiter ihre eigenen Koordinaten und Größen. Bedürfnisse, Fundregeln und Spielstandformat Version 3 unverändert.

Generator `tools/make-zimmer-tiefe.py`; Details und Prüfplan in `Raumtiefe_und_Zimmer.md`. Neues Prüfprogramm `tools/pc-nachbildung/raumtiefe.cpp` zusätzlich zu den acht bestehenden Programmen. Am echten Board noch ausprobieren: Bildwirkung, flüssiges Laufen, Touch auf kleinen sichtbaren Figurteilen, Ton und Geschwindigkeit.

Prüfung Raumtiefe: alle neun PC-Prüfprogramme ohne Fehler/Warnungen und echte Pico-2-Übersetzung erfolgreich. Programm 486360 Bytes (11 %), globale Variablen 33380 Bytes (6 %, ohne Stack/Heap). Spielstandformat unverändert. Am echten Board noch testen.

## Neues Zimmer und Einrichten – 25. November 2026

Die bestätigte V2-Grafikgrundlage mit vollständigen saisonalen Bäumen und 51 Fensterzuständen ist eingebaut. Menü → **Zimmer einrichten**: Startmöbel mit Steuerkreuz verschieben, A bestätigen, B/START abbrechen; rechts in der Auswahlliste legt ein Möbel ins Inventar. RESET verwirft eine offene Vorschau. Drei Tiefenzonen und freie Interaktionswege werden geprüft. Weihnachten bleibt erhalten. Möbel/Inventar werden separat ab EEPROM 3328 gespeichert.

Siehe [Einrichtung_und_Grillme.md](../../Einrichtung_und_Grillme.md) für Bedienung, Grenzen und Datenformat. Der Grafik-Header lässt sich mit `python3 software/schritt-10/tools/make-zimmer-v2.py` reproduzieren. Die PC-Nachbildung enthält jetzt zusätzlich `einrichtung.cpp` und den unabhängigen Pixelvergleich aller 51 Fensterausblicke. Für diesen Stand stehen echte Pico-2-Übersetzung und physischer Boardtest noch aus.
