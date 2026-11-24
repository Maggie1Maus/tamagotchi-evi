# Gemeinsamer Garten und Fundstücke
Stand: 24. November 2026

## Überarbeitete Gestaltung, von der Nutzerin bestätigt

Die erste Gartenfassung wurde als lieblos und zusammengewürfelt abgelehnt. Der neue Gesamtentwurf unter `entwuerfe/garten/neuentwurf/` wurde anschließend mit „Jaaaaaaaa 100%. Das sieht gut aus“ bestätigt und ist jetzt in der Firmware eingebaut.

Die Darstellung nutzt fünf zusammenhängende Gartenebenen unter `entwuerfe/garten/v2/`: Frühling, Sommer, Herbst, Winter und Weihnachten. Einheitliche Perspektive und Linienführung; verbundene Steinränder, natürlicher Baum links, Busch rechts, hinteres Blumenbeet, geschwungene Trittsteine, zurückhaltende runde Lichter. Keine einzeln aufgesetzten Sonne-/Wolkenbilder. Das Wetter verändert die Atmosphäre im freigestellten Himmel; Regen und Schnee fallen im Himmel und auf der offenen Wiese, bevor Evi darüber gezeichnet wird. Nachts wenige Sterne und schmale helle Silhouettenkanten. Die 17 zusätzlich abgeleiteten Einzelobjekte liegen unter `v2/einzelobjekte/` und werden nicht wieder als unterschiedlich große Symbole zum Garten zusammengesetzt. Die Lichter ergänzen den Winterzaun außerhalb der Weihnachtszeit.

Evi ist im Garten passend zur räumlichen Tiefe etwas kleiner; ihre Zeichnungen selbst bleiben unverändert. Die Blumenbeet-Suchposition liegt jetzt vor dem tatsächlich rechts vom Weg befindlichen Beet. Suchablauf, Funddatum, Erinnerungssätze und Spielstandformat bleiben erhalten. Der Weihnachtszeitraum bleibt 18. November bis einschließlich 5. Januar.

`software/schritt-10/tools/make-garten-szene.py` erzeugt `gartenlandschaft.h` aus den gespeicherten kleinen PNGs. Der freigegebene Sommerentwurf liefert direkt die Sommerkonturen. Alle Quellen sind schwarz-weiß, Gartenflächen deckend, der Himmel transparent. Bildaufträge sind bei den Quellen dokumentiert. Die früheren Einzelzeichnungen unter `entwuerfe/garten/` bleiben als frühere Fassung erhalten. Aktuelle Bildschirmansichten: `entwuerfe/garten/v2/vorschauen/`.

## Eingebaut auf dem Waveshare

Der gemeinsame, eingezäunte Garten ersetzt den Park. Er ist über „In den Garten“ im Zimmermenü auch vom Sofa aus erreichbar. Stil: gemütlich & schlicht, schwarz-weiß, Lichterketten, Gartentür, Baum, Busch und Blumenbeet. Eigene Ansicht, damit der Zimmerbildschirm frei bleibt. Die Handy-App bleibt unverändert.

- Links/rechts oder hoch/runter wählt eine der drei Suchstellen; A schickt Evi dorthin. Evi bewegt sich selbst.
- Schnuppern → Fragezeichen wartet auf Hilfe → A oder Stelle antippen → mit der Pfote untersuchen → Fund stolz zeigen. Kein Zeitdruck; A bestätigt, B/START geht heim. Ohne erkannte Tasten bleiben die drei Touch-Drittel die Notbedienung.
- Jede Stelle einmal pro echtem Ortsdatum; täglich mindestens eine ergiebige Stelle, weitere mit 20 % (anpassbare Startwerte). Versuche bleiben nach Neustart erhalten. Kein Sammelset-Ziel und kein Farmen derselben Stelle durch erneutes Öffnen.
- Echte Spaziergänge ermöglichen zusätzlich Funde: ab 100 Schritten erster ausreichender Spaziergang sicher, danach 40 %, höchstens ein Fund pro Tag. Garten und Spaziergang haben getrennte Limits.
- Speicherung: Gegenstandsart, echtes Funddatum ohne Uhrzeit, Garten/Spaziergang, genaue Gartenstelle, damalige Jahreszeit und damaliges Spielwetter. Die bestehende Generation wird weiter mitgespeichert. Maximal 256 Einträge, keine Überschreibung.
- Das Sideboard zeigt Name, Bild, Herkunft, Datum und einen Erinnerungssatz. Nichts auf die Kerzenfläche stellen.
- Beispiele: „Unter unserem Gartenbaum gefunden“, „An einem Regentag im Garten entdeckt“, „Zwischen den Herbstblättern gefunden“, „Vom gemeinsamen Spaziergang mitgebracht“. Das tatsächliche Wetter bei echten Spaziergängen kennt das Gerät nicht und erfindet es nicht.
- Spielstand Version 3 übernimmt Version 1 und 2 mit allen bisherigen Daten. Alte Funde erhalten keine nachträglich erfundene Gartenstelle oder Wetterlage.

## Bilder, Jahreszeiten und Wetter

Zwölf neue Evi-Zeichnungen: **schnuppern**, **mit einer Pfote untersuchen**, **stolz einen Fund zeigen** für **Baby, Teenager, Erwachsene und Oma**. Mit „Generationen“ sind in dieser Umsetzung diese bereits vorhandenen Lebensphasen berücksichtigt; zukünftige Partner oder eine Verpaarungsmechanik sind damit nicht festgelegt. Neue Dateien unter `entwuerfe/evi/garten/`, bestehende Evi-Zeichnungen bleiben unverändert. Alle Körper weiß deckend, nur außen transparent. Der Fund wird als separates Bild über die Pfoten gelegt, damit jeder Fund zu jeder Altersstufe passt.

Erste Fassung: siebzehn einzelne Gartenbilder unter `entwuerfe/garten/`: Baum, Busch und Beet je viermal; Zaun, Gartentür, Sternenlichterkette, Kranz und Schneemann. Die vollständigen Bildaufträge sind daneben als JSON dokumentiert. Gesamtansichten des tatsächlich gerenderten Gartens in `vorschauen/`.

- Frühling: Blüten und junge Blätter.
- Sommer: dichtes Laub und blühendes Beet.
- Herbst: dünneres Laub und gefallene Blätter.
- Winter: kahle Zweige mit Schnee, verschneites Beet.
- Jahreszeit, Wetter und Sonnenzeiten stammen aus denselben Funktionen wie der Fensterausblick. Sonne, Sonne/Wolken, Regen, bedeckt und Winterschnee werden dargestellt; Regen und Schnee bewegen sich auch nachts.
- Weihnachtsgestaltung als erste Umsetzung: Kranz an der Gartentür, Sternenlichterkette am Zaun, kleiner Schneemann beim Beet. Zeitraum wie Zimmerdeko: **18. November bis einschließlich 5. Januar**, ab 6. Januar wieder ohne Weihnachtsdeko.

## Fundstückarten

Ziel war ungefähr zehn Arten. Mit allen gewünschten Motiven waren es elf; zehn sind aktuell vorhanden, Stitch bleibt offen.

| Gegenstand | Stand |
|---|---|
| Glatter Stein, kleines Blatt, weiche Feder, Tannenzapfen | Bisherige bestätigte Bilder unverändert |
| Micky-Knopf | Gezeichnet und eingebaut |
| Arielles Muschel-Anhänger | Gezeichnet und eingebaut |
| Kleiner Gartendrache | Eigene Figur, nach abgelehnter Spyro-Generierung und Erlaubnis für ähnlichen Drachen; wird nicht als Spyro ausgegeben |
| Triforce-Anhänger (Zelda) | Gezeichnet und eingebaut |
| Spiralfossil (Animal Crossing) | Gezeichnet und eingebaut |
| Junimo-Figur (Stardew Valley) | Gezeichnet und eingebaut |
| Stitch-Anhänger | Bestätigt, Bildgenerator hat die Erstellung abgelehnt, ohne Ersatz offen |

Alle zehn vorhandenen Arten sind im Garten und beim echten Gassigehen erreichbar, im Sideboard einsehbar und nach Neustart erhalten. Fundbilder unter `entwuerfe/sammlung/`.

## Zurückgestellt oder gestrichen

Kein Federkitzeln, Zapfenspielen oder Vervollständigen von Sammelsets. Persönlichkeit und umfangreiche Generationen-/Erbstückmechanik bleiben für eine spätere Sitzung. Neue Posen für bestehende Lebensphasen setzen das nicht voraus. Die bereits vorhandene Generationenzuordnung und das Weitergeben der Sammlung bleiben erhalten.

## Prüfung

Gesamte PC-Nachbildung einschließlich Garten, bestehender Spiele, Touch, Tasten, Wetter, Sideboard und Migration alter Spielstände erfolgreich. Auch echte Arduino-Übersetzung für Pico 2 erfolgreich. Ton, echte Bedienung und Geschwindigkeit am Waveshare noch ausprobieren. Bedienung und technische Details in `software/schritt-10/README.md`.
