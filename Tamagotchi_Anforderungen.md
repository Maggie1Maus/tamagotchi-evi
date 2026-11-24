# Tamagotchi-Projekt – Anforderungen und offene Entscheidungen

Stand: 24. November 2026  
Das Waveshare RP2350-Touch-LCD-2.8 ist der Favorit für die Erprobung. Beim Retro Gamer stehen die Maße und Anschlüsse hinter dem Originaldisplay zur Prüfung an. Cube, Lötzubehör, Lötzinn und Zhongdi ZD-99 sind laut Nutzerin bestellt. Die Funduino-Lieferung ist am 19. November 2026 angekommen, Lötzinn und Zhongdi ZD-99 am 20. November 2026; der Aufbau steht noch aus.

## 1. Ziel und Rahmen

- Ein eigenes, selbstgebautes Tamagotchi als persönliches Projekt entwickeln.
- Ein virtuelles Haustier begleiten, versorgen und mit ihm interagieren.
- Über mehrere Generationen spielen und dabei dauerhaft etwas aufbauen.
- Die Software selbst programmieren; Softwareentwicklung ist bereits vertraut.
- Elektronik und Löten dürfen Teil des Lernens sein. Der Aufbau soll für eine Elektronik-Einsteigerin verständlich und mit überschaubarem Lernaufwand machbar sein.
- Spielerlebnis und Geräteanforderungen wurden zunächst auf Überblicksebene gesammelt. Aktuell haben Hardware, Zubehör und Beschaffung Vorrang; die genaue Software und die Spielregeln werden danach geplant.
- Erster praktischer Schritt: Funduino Cube als Lötbausatz aufbauen und darauf eine kleine eigene Tamagotchi-Anwendung ausprobieren. Ein späterer Hardwarewechsel ist akzeptiert.
- Die langfristigen Geräteanforderungen gelten für das spätere tragbare Gerät. Der erste Cube-Prototyp darf davon bewusst abweichen; Einzelheiten stehen in „Tamagotchi_Hardware_Cube.md“.
- Beim Brainstorming bewusst auf der Überblicksebene bleiben und bei zu viel technischem Detail wieder zu den Anforderungen zurückkehren.

## 2. Haustier und Interaktion

- Ein sichtbares virtuelles Haustier mit grundlegenden Tamagotchi-Funktionen.
- Versorgung und Interaktion, insbesondere Füttern und Saubermachen.
- Bedürfnisse und Zustände wie Hunger, Verschmutzung und Unzufriedenheit.
- Akustische Hinweise, wenn das Tier Aufmerksamkeit benötigt, beispielsweise bei Hunger oder Verschmutzung.
- Eigene Persönlichkeiten der Tiere, die sich spürbar auf das Spiel auswirken.
- Die konkreten Persönlichkeiten, Auswirkungen und weiteren Aktionen werden später festgelegt.

## 3. Echtzeit und Tagesablauf

- Das Spiel berücksichtigt tatsächlich vergangene Zeit.
- Bedürfnisse verändern sich mit der Zeit.
- Entwicklung und Lebensereignisse können nach bestimmten Zeitspannen eintreten.
- Schlafenszeiten richten sich nach der Uhrzeit und können vom Lebensalter abhängen.
- Die genannten Zeiten waren Beispiele, keine festgelegten Spielregeln:
  - Hunger nach etwa drei Stunden.
  - Nachwuchs nach etwa fünf Tagen.
  - Schlafenszeit für ein Kind etwa um 20 Uhr, für ein erwachsenes Tier etwa um 23 Uhr.
- Genaue Zeitspannen, Alterungsdauer und Tagesabläufe bleiben offen.

## 4. Partnersuche, Nachwuchs und Generationen

- Eine Partnersuche innerhalb des Spiels ermöglichen.
- Virtuelle Partner zufällig im Spiel kennenlernen bzw. auswählen lassen.
- Keine Verbindung mit einem zweiten Tamagotchi-Gerät erforderlich oder vorgesehen.
- Nachwuchs kann Merkmale der Eltern übernehmen und ihnen im Aussehen ähneln.
- Die genaue Vererbung und die Gestaltung der Partner bleiben offen.
- Ein erwachsenes Tier kann Nachwuchs bekommen.
- Es gibt eine gemeinsame Phase mit Elternteil und Kind; beide sollen gleichzeitig auf dem Display zu sehen sein.
- Das ältere Tier wird alt und kann sterben.
- Anschließend wird mit dem Nachwuchs weitergespielt.
- Sterben ist grundsätzlich Teil des gewünschten Spiels. Weitere Todesursachen und ihre Regeln sind noch nicht festgelegt.

## 5. Zuhause, Welt und dauerhafter Fortschritt

- Das Tier hat ein Zuhause.
- Es kann das Zuhause verlassen und Ausflüge unternehmen.
- Eine kleine erkundbare Welt bzw. das Herumlaufen in ihr ist als Wunschidee aufgenommen; Umfang und Darstellung bleiben offen.
- Im Spiel Geld verdienen.
- Gegenstände bzw. schöne Dinge kaufen.
- Bereits in Version 1 soll das Zuhause eingerichtet werden können. Dinge und zusätzliche Räume sollen im Spiel freischaltbar sein.
- Das Tier soll Möbel und Spielzeug sichtbar benutzen und mit Einrichtungsgegenständen interagieren.
- Einrichtung soll auch spielerische Vorteile bieten: Ein besseres Bett könnte die Erholung verbessern, neues Spielzeug die Zufriedenheit länger erhalten. Die konkreten Wirkungen sind noch offen.
- Zuhause, gekaufte Gegenstände und Geld bleiben beim Generationenwechsel erhalten.
- So entsteht Fortschritt über das Leben eines einzelnen Tieres hinaus.
- Konkrete Orte, Verdienstmöglichkeiten, Kaufgegenstände, Raumzahl, Preise und Freischaltbedingungen werden später ausgearbeitet.

## 6. Betreuung bei Abwesenheit

- Eine Betreuungsfunktion ist grundsätzlich gewünscht.
- Sie soll Zeiten unterstützen, in denen keine Zeit für die Versorgung ist, beispielsweise während der Arbeit.
- Wie sie funktioniert, was sie übernimmt und welche Folgen sie für Zeitablauf und Bedürfnisse hat, bleibt offen.

## 7. Gerät und Bedienung

| Bereich | Festgehaltener Wunsch |
| --- | --- |
| Bedienung | Mindestens drei physische Knöpfe. Für ein eigenes Gehäuse sind unbeschriftete Knöpfe vorgesehen. Beim Umbau des Billig-Game-Boys kommen die vorhandenen Tasten und das Steuerkreuz infrage. |
| Touch | Kapazitiver Fingertouch ist als zusätzliche Bedienung möglich. |
| Menü | Pflegeaktionen werden im Display ausgewählt. Die aktuelle Auswahl ist sichtbar markiert. |
| Tierstatus | Sättigung, Stimmung und Sauberkeit sind direkt in die Spielanzeige integriert. |
| Größe | Möglichst hosentaschentauglich; maximal ungefähr Game-Boy-Color-Größe. |
| Displayform | Quadratisch als kompakte Option. Eine rechteckige Spielfläche kommt ebenfalls infrage. |
| Sichtbare Displayfläche | Ungefähr 3 × 3 cm als kompakte Option. Auch 7 cm Breite × 5 cm Höhe sind ausdrücklich möglich. |
| Ungeeignete Proportion | Etwa 3 cm breit und nur 1,5 cm hoch ist zu flach. |
| Farben | Die Nutzerin hat Schwarz-Weiß für Tier und Spielwelt am 17. November 2026 bekräftigt. Am 20. November 2026 festgelegt: Das Display muss farbfähig sein, damit Farbe später möglich bleibt. |
| Sichtbarkeit | Geändert am 20. November 2026: Evi muss nicht dauerhaft sichtbar sein. Das Display darf ausgehen und wird per Knopf oder Touch geweckt. |
| Bewegung | Flüssige Bildbewegungen haben bei der Displayauswahl Vorrang gegenüber der besprochenen E-Paper-Lösung mit langsameren Bildwechseln. Eine konkrete Bildrate ist noch nicht festgelegt. |
| Beleuchtung | Keine Hintergrundbeleuchtung erforderlich; eine Lösung mit Beleuchtung ist bei Bedarf ebenfalls akzeptabel. |
| Ton | Geräusche und Hinweise bei Bedürfnissen oder Problemen. |
| Stummschaltung | Töne sollen abschaltbar sein. |

Für die nächste Überarbeitung soll die Handy-App die Darstellung und das Menü des späteren Geräts erproben. Die bisherigen Überschriften außerhalb des Spiels entfallen in der Hauptansicht. Mindestens drei neutrale Touch-Tasten können die Hardwareknöpfe nachbilden.

**Entschieden am 20. November 2026:** Das spätere Gerät wird mit dem **Waveshare RP2350-Touch-LCD-2.8** gebaut, betrieben mit AAA-Batterien über einen 5-V-Wandler. Begründung, Folgen und Einkaufsliste stehen in der [Hardware-Datei](Tamagotchi_Hardware_Cube.md#entscheidung-vom-20-november-2026-waveshare-rp2350-touch-lcd-28). Das Sharp Memory LCD ist verworfen, weil es keine Farbe kann.

Die Nutzerin bevorzugt das **Waveshare RP2350-Touch-LCD-2.8** wegen Farbe und Fingertouch. Die sichtbare Fläche im Querformat beträgt rund **5,76 × 4,32 cm**. Für die Daueranzeige mit Batterien wurde auch ein reflektives Memory-LCD betrachtet. Die Bildrate, die Laufzeit und der endgültige Einbau sind noch zu prüfen. [Verglichene Displays](Tamagotchi_Hardware_Cube.md#displayauswahl-vom-17-november-2026)

Der vorhandene Retro Gamer (Art.-Nr. 3307) ist ein möglicher Gehäusespender. Bei der nächsten Zerlegung sollen insbesondere die Rückseite des Originaldisplays, seine Befestigung, das Flachbandkabel und der freie Platz geprüft werden. [Umbauplan](Tamagotchi_Hardware_Cube.md#umbauversuch-mit-dem-vorhandenen-billig-game-boy)

## 8. Stromversorgung und Datenerhalt

- Batteriebetrieb ist die aktuell gewünschte Richtung; ein Akku ist nicht als Voraussetzung festgelegt.
- Der Batterietyp bleibt offen. Besprochen wurden Knopfzellen und normale AAA-Batterien.
- Ein Batteriewechsel etwa einmal pro Woche wäre akzeptabel; längere Laufzeit wäre ebenfalls willkommen.
- Das ist eine gewünschte Größenordnung, noch keine technisch bestätigte Laufzeit.
- Der Spielstand soll beim Batteriewechsel erhalten bleiben.
- Der Umgang mit Uhrzeit und verstrichener Zeit während eines Batteriewechsels muss berücksichtigt werden; die genaue Lösung ist offen.
- Die Batterieentscheidung wird später zusammen mit Display, Gerätegröße und Energiebedarf getroffen.

## 9. Entwicklung, Kosten und Aufbau

- Die eigene Software soll unkompliziert vom Computer auf das Gerät übertragen und aktualisiert werden können.
- Für den Einstieg ist der Funduino Cube mit Nano R3 und Übertragung eigener Programme über USB vorgesehen. Die konkrete Entwicklungsumgebung und Softwarestruktur sind noch offen; die Plattform des späteren Geräts ebenfalls.
- Das Budget ist flexibel; es darf etwas kosten, soll aber nicht in den Tausenderbereich gehen.
- Für die erste Bestellung wurde keine feste Kostenobergrenze gesetzt. Der allgemeine Wunsch, nicht in den Tausenderbereich zu gehen, bleibt bestehen.
- Bei der späteren Kostenschätzung sollen Bauteile, Gehäuse und gegebenenfalls benötigtes Werkzeug berücksichtigt werden.
- Der Elektronikaufbau soll für eine Einsteigerin lernbar bleiben.
- Am 16. November 2026 hat die Nutzerin die Bestellung bestätigt: Cube, Zange, Lötspitzenreiniger, Pinzettenset und Entlötpumpe bei Funduino; Goobay-Elektronik-Lötzinn (20 g, 0,8 mm) und Zhongdi ZD-99-Lötstation bei BerryBase. Die genauen Artikel stehen in „Tamagotchi_Hardware_Cube.md“.
- Ein älterer Lötkolben ist bereits vorhanden. Für den Einstieg wurde nun zusätzlich die Zhongdi ZD-99 bestellt; der Neukauf ist damit entschieden. Die Station ist am 20. November 2026 angekommen; die praktische Erprobung steht noch aus.

## 10. Ergänzende Vorschläge – noch nicht ausdrücklich beschlossen

Diese Punkte wurden vorgeschlagen, aber noch nicht einzeln bestätigt:

- Geschlossenes Gehäuse mit gut zugänglichem Batteriefach.
- Warnung bei schwacher Batterie.
- Gut dokumentierte Bauteile als Auswahlkriterium.

## 11. Offene Entscheidungen im Überblick

- Display entschieden (Waveshare RP2350-Touch-LCD-2.8). Offen sind noch die Passform im Retro Gamer, der gemessene Ruhestrom und die Pufferbatterie für die Uhr.
- Batterieart, Anzahl der Batterien und tatsächliche Laufzeit.
- Elektronikplattform des späteren tragbaren Geräts; für den Cube-Einstieg ist USB-Programmierung vorgesehen.
- Gehäuseform, genaue Abmessungen und Anordnung von Display und Knöpfen.
- Eignung von Gehäuse, Originaldisplay, Knöpfen und Steuerkreuz des Retro Gamer. Zu klären sind besonders die Maße und Anschlüsse hinter dem Display.
- Genaue Menüführung und Tastenbelegung der Handy-App und des Geräts.
- Verhalten von Uhrzeit und Spiel während eines Batteriewechsels.
- Ausgestaltung der Betreuungsfunktion.
- Persönlichkeiten und ihre Auswirkungen.
- Zeitspannen für Bedürfnisse, Entwicklung, Nachwuchs und Lebensdauer.
- Weitere Todesursachen und Verhalten bei Tod ohne Nachwuchs.
- Vererbung von Aussehen und gegebenenfalls weiteren Merkmalen.
- Umfang der Welt, Ausflüge, Geldverdienst und Einkaufsmöglichkeiten.
- Raumzahl, Preise, Freischaltbedingungen und genaue Wirkungen der Einrichtung.
- Genauer Umfang und Umsetzung der ersten spielbaren Version, insbesondere auf der begrenzten Cube-Hardware.
- Vorbereitung des Lötarbeitsplatzes und Prüfung der gelieferten Ausstattung. Die bestellten Artikel sind in der Hardware-Datei festgehalten.

## 12. Aktuell vereinbartes Vorgehen

- Aktualisierung am 18. November 2026, Spielablauf festgelegt: Ei etwa 30 Minuten, schlüpft auch in Abwesenheit. Baby etwa 1 Tag mit Pflege alle 1–2 Stunden, Teenager etwa 2 Tage mit Pflege alle 2–3 Stunden, erwachsen etwa 5 Tage mit 3–4 Mal Pflege am Tag; danach vorerst weiter erwachsen. Vernachlässigung führt erst zu Krankheit, dann zum Tod. Alle Tiere schlafen ab 23 Uhr; Licht aus gibt einen Stimmungsbonus. Geld verdient man über ein Minispiel. Die frühere Beispielidee einer Schlafenszeit um 20 Uhr für Kinder ist damit ersetzt. Umgesetzt in Handy-App 0.6; Einzelheiten und Startwerte in `PROJEKTSTAND.md`.

- Aktualisierung am 17. November 2026: Als Einstieg für das spätere tragbare Spiel wurde eine eigenständige Handy-Web-App vereinbart und unter `handy-app/` begonnen. Erste Spielprobe: ein Tier, ein Zimmer, Füttern, Spielen und Saubermachen, drei Bedürfnisse, lokale Speicherung und Nachberechnung echter Zeit. Die Regeln sind von der Browser-Anbindung getrennt; die konkrete spätere Hardware und damit die Sprache der Geräteversion bleiben offen. Dieser kleine Einstieg ersetzt nicht den bereits gewünschten Umfang von Version 1 mit Einrichtung und zusätzlichen Räumen. Cube-Übungen bleiben separat erhalten. Details, privater Spiel-Link und Prüfumfang stehen in `PROJEKTSTAND.md` und `handy-app/README.md`.
- Softwarestand unter `software/schritt-04`: Einzelklick = Weiter, Doppelklick = Zurück, Halten = Bestätigen. Ein Einzelklick wartet etwa 300 Millisekunden auf einen zweiten Klick. Die Anwendung startet zuhause; Zurück verlässt die Aktionsansicht. Knopferkennung, allgemeine Steuerbefehle, Menü und Tierlogik sind getrennt, damit später drei einzelne Knöpfe dieselben Befehle liefern können. 32 Verhaltensprüfungen sowie die Bedienung per Maus und Leertaste wurden im Simulator bestanden; die echte Hardware und ihre spätere Drei-Knopf-Anbindung sind noch nicht geprüft.
- Weiterentwicklung unter `software/schritt-03`: Winken und Füttern sind jetzt getrennte Aktionen in einem im Browser geprüften Menü. Kurzes Drücken wechselt die Auswahl, Halten für ungefähr 0,7 Sekunden führt sie einmal aus. Die Auswahl bleibt danach erhalten. Das funktioniert mit dem virtuellen Taster und der Leertaste; Hunger und Testtempo bleiben wie in Schritt 2. Die beiden früheren Schritte bleiben separat erhalten.
- Softwarefortschritt: Unter `software/schritt-02` gibt es inzwischen eine im Browser geprüfte Fütterprobe mit Sättigungsanzeige und hungrigem Gesicht. Vorläufige Testwerte: Start 60 Prozent, ein Punkt weniger pro simulierter Sekunde, 25 Punkte pro Fütterung, hungrig bei höchstens 30 Prozent. Diese Werte sind keine endgültigen Spielregeln. Die Grenzen bei null und 100 Prozent sind berücksichtigt; Tod und Speicherung sind noch nicht umgesetzt. `software/schritt-01` bleibt als getrennte Tastenprobe erhalten.
- Aktualisierung am 16. November 2026: Die Nutzerin möchte die Wartezeit bis zur Lieferung für die Software nutzen und schrittweise anfangen. Unter `software/schritt-01` wurde eine erste Nano-/OLED-Simulation mit Tieranzeige und Begrüßung per Tastendruck vorbereitet und in Wokwi ausprobiert. Die Anschlüsse sind vorläufig; der Abgleich mit dem echten Cube steht aus. Als nächster kleiner Lernschritt bietet sich Füttern mit einem Sättigungswert an, noch ohne festgelegte weitere Spielregeln.
- Hardware und Zubehör zuerst besprechen und beschaffen; danach die Software und das genaue Spielerlebnis ausarbeiten.
- Beide Hardwarestufen dürfen bereits gemeinsam betrachtet werden. Die bestätigten Bestellungen umfassen zunächst den Cube, Lötzubehör, Lötzinn und die neue Lötstation.
- Nach der Lieferung: Lieferumfang prüfen, Arbeitsplatz vorbereiten, Cube nach Anleitung aufbauen und anschließend Display, Taster, Ton und USB-Programmierung testen.
- Der Cube dient zum Lötenlernen und zum Ausprobieren eines kleinen Spiels am USB-Anschluss. Ein Tier, einfache Animationen, Bedürfnisse, Aktionen und Ton wurden als erster Umfang vorgeschlagen, aber noch nicht im Detail beschlossen.
- Für Version 1 des geplanten Spiels sind das Einrichten des Zuhauses, freischaltbare Gegenstände und zusätzliche Räume beschlossen. Das Tier soll mit Möbeln und Spielzeug sichtbar interagieren; Einrichtung soll spielerische Vorteile haben. Der kleine Cube-Test kann zunächst nur einen Teil davon umsetzen.
- Beim Festlegen des konkreten Umfangs und der grafischen Darstellung sind die Speichergrenzen der gewählten Hardware zu berücksichtigen.
- Nachwuchs und Generationen bleiben Ziele für spätere Versionen und gehören nicht zum beschlossenen Umfang von Version 1.
- Mindestens drei physische Knöpfe, die endgültige Displaygröße, Batterielaufzeit und Uhrzeit beim Batteriewechsel sind auf der späteren Hardware zu prüfen.
- Die kritische Prüfung hat die Eignung des Cube für diesen begrenzten Einstieg bestätigt; praktische Funktionstests stehen noch aus.
- Die genannten Teile für das spätere Gerät sind bisher Kandidaten, keine bestätigte Einkaufsliste.

## 13. Gestaltung von Evis Zuhause – beschlossen am 24. November 2026

### Verbindliche Gestaltungslinie: „Gemütlich & schlicht“

- Die Bildvorschläge der Variante A („Gemütlich & schlicht“) geben grundsätzlich die gestalterische Richtung für das Zuhause und künftige Einrichtungsgegenstände vor.
- Möbel und Umgebung passen zu Evis Schwarz-Weiß-Pixelstil: klare abgestufte Konturen, überwiegend weiße Flächen und sparsame Punkte oder Schraffuren für Struktur.
- Gemütliche, einfache Formen und zurückhaltende Details verwenden; die Elemente müssen auch auf dem kleinen Display gut erkennbar bleiben.
- Tisch, separate Topfpflanze, Napf, Wandregal, Sideboard und Boden als einzelne, kombinierbare Elemente gestalten.
- Variante A zeigt einen schlichten Holztisch, eine Blattpflanze im Topf, einen Napf mit Pfotenmotiv, ein Bücherregal mit kleiner Pflanze, ein Holzsideboard und einen Dielenboden. Diese Vorschläge dienen als gestalterische Referenz.

### Fenster und Jahreszeiten

- Vor dem Fenster steht draußen ein kleiner Laubbaum, an dem sich die Jahreszeit ablesen lässt.
- Die bereits gezeigten vier Jahreszeitenentwürfe sind gestalterisch bestätigt.
- Frühling: Blüten und frische, noch lockere Belaubung.
- Sommer: eine volle, dichte Laubkrone.
- Herbst: weniger Laub, sichtbare Äste, fallende Blätter und Laub am Boden.
- Winter: kahle Äste und optional liegender Schnee als Teil der Umgebung.
- Alle Jahreszeiten müssen auch ohne Farbe voneinander unterscheidbar sein.

### Wetter unabhängig von der Jahreszeit

- Fünf Wetterzustände vorsehen: Sonne, Sonne mit Wolken, Regen, nur Wolken (bewölkt) und Schneefall.
- Jede der vier Jahreszeiten muss mit jedem der fünf Wetterzustände kombinierbar sein: insgesamt 20 Kombinationen.
- Beispielsweise sind Winter mit Sonne und Winter mit Regen ausdrücklich möglich. Auch Schneefall ist grundsätzlich mit jeder Jahreszeit kombinierbar.
- Das Wetter darf die jahreszeitliche Darstellung des Baums nicht automatisch ersetzen: Ein blühender Baum bleibt bei Regen oder Schneefall ein Frühlingsbaum.
- Liegender Schnee und gerade fallender Schnee sind getrennte Darstellungen. Winter bedeutet nicht automatisch Schneefall; Schneefall bedeutet nicht automatisch eine vollständig schneebedeckte Umgebung.
- Regen durch erkennbare Regenstriche und Schneefall durch klar unterscheidbare fallende Flocken darstellen, jeweils draußen im Fensterblick.
- Jahreszeitliche Landschaft und Wetter als unabhängig kombinierbare Grafikelemente planen; dafür sind keine 20 vollständig getrennten Hintergrundbilder erforderlich.
- Die Wetterdarstellung soll ohne Internet funktionieren. Reale Wetterdaten wurden bereits verworfen; die genaue Auswahl und Häufigkeit des simulierten Wetters bleiben offen.
- Die bestehende Nachtdarstellung mit Mond, Sternen und Sternschnuppen bleibt unverändert. Die neuen Wettervorschläge beziehen sich auf die Tagesansicht.

Die Entwürfe und diese Ergänzung sind Gestaltungsvorgaben. Eine Integration der neuen Grafiken oder Wetterfunktionen in das Spiel ist damit noch nicht erfolgt.

### Weihnachtsbaum – saisonale Einrichtung

- Ein Weihnachtsbaum im Stil „Gemütlich & schlicht“ ersetzt jährlich ab dem 18. November das Wandregal und die Topfpflanze.
- Am 6. Januar verschwinden der Weihnachtsbaum und seine saisonale Darstellung wieder; Regal und Pflanze kehren zurück. Sichtbarer Zeitraum: 18. November bis einschließlich 5. Januar, jeweils nach deutscher Ortszeit.
- Nur Regal und Pflanze werden ersetzt; eine Änderung des Tischs wurde nicht beschlossen.
- Schwarz-Weiß-Pixelgrafik mit Stern, wenigen Kugeln und einer schlichten Girlande. Entwurf: `entwuerfe/umgebung/weihnachtsbaum.png`.
- Die Datumsregel ist geplant; in diesem Schritt werden nur Entwürfe und Planung hochgeladen.

Diese Liste hält Wünsche und Entscheidungen fest. Sie ist noch kein Schaltplan, keine Bauteilempfehlung und keine detaillierte Software-Spezifikation.
