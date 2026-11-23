# Tamagotchi-Projekt – Hardware-Einstieg mit dem Funduino Cube

Stand: 17. November 2026  
Status: Cube, Lötzubehör, Lötzinn und Zhongdi ZD-99 am 16. November 2026 laut Nutzerin bestellt. **Alles ist laut Nutzerin angekommen:** Funduino (Cube und Zubehör) am 19. November 2026, Lötzinn und Lötstation (BerryBase) am 20. November 2026. Montage und Funktionstests stehen noch aus.

Diese Zusammenfassung ergänzt die [Projektanforderungen](Tamagotchi_Anforderungen.md). Sie beschreibt den ersten Prototyp auf dem Funduino Cube und in Abschnitt 10 den möglichen Umbau des vorhandenen Billig-Game-Boys.

## 1. Gewünschter Einstieg

- Zunächst auf vorhandener, dokumentierter Hardware eine kleine eigene Tamagotchi-Anwendung entwickeln.
- Ein späterer Wechsel auf andere Hardware ist ausdrücklich in Ordnung.
- Der erste Aufbau muss noch nicht das endgültige Format, Display oder die gewünschte Batterielaufzeit erreichen.
- Löten und Elektronik dürfen Teil des Lernens sein. Ein älterer Lötkolben ist bereits vorhanden; zusätzlich wurde die Zhongdi ZD-99-Lötstation bestellt.
- Außer dem älteren Lötkolben war zunächst kein Lötzubehör vorhanden. Die inzwischen bestellten Artikel sind in Abschnitt 8 festgehalten.
- Die erste Bestellung ist laut Nutzerin erfolgt. Als Nächstes stehen Lieferung, Aufbau und Hardwareprüfung an; die genaue Software und die Spielregeln werden anschließend geplant.
- Für die erste Bestellung besteht keine feste Kostenobergrenze; die allgemeine Budgetvorstellung aus der Anforderungsliste bleibt bestehen.
- Entscheidend ist, dass auf dem ersten Board bereits eine kleine Anwendung umgesetzt werden kann.

## 2. Besprochener Bausatz

**Funduino Cube Lötübung – Lern-Board für Arduino, mit Anleitung**  
Artikelnummer: **F23110296**

[Produktseite bei Funduino](https://funduino.com/products/funduino-cube-lotubung-inkl-anleitung)

Der Link kam von einem Kollegen. Seine Argumente waren die Ersparnis gegenüber einer fertig aufgebauten Platine, Spaß beim Löten und anschließend eine fertige, gut ansprechbare Platine.

Laut Produktbeschreibung und Cube-Dokumentation gehören unter anderem dazu:

- Funduino Nano R3 als programmierbarer Mikrocontroller.
- OLED-Display mit 0,96 Zoll Diagonale und 128 × 64 Pixeln.
- Ein Spieltaster und ein akustischer Signalgeber.
- Weitere Bauteile für Lernprojekte, beispielsweise LEDs, Sensoren und ein Servomotor.
- Ein Mini-USB-Kabel mit 100 cm Länge und Anleitungen zum Aufbau und zur Programmierung. Ein zusätzliches Kabel wird für diesen Aufbau zunächst nicht benötigt, sofern am Computer ein passender Anschluss vorhanden ist.
- Ein digitales Arbeitsbuch mit 13 Lektionen und mehr als 180 Seiten.

Eigene Programme lassen sich nach dem Aufbau über USB übertragen. Die Arduino-Entwicklungsumgebung und textbasierte Programmierung sind vorgesehen; die konkrete Arbeitsumgebung wurde noch nicht festgelegt.

Der Bausatz erfordert eigene Lötarbeiten; auch Display, Taster und Signalgeber werden nach Anleitung montiert. Laut offizieller Lötanleitung gehört ein MPU-6050-Modul zu den zu montierenden Bauteilen. Die allgemeine Komponentenübersicht enthält teilweise abweichende Sensorbezeichnungen; maßgeblich sind die Anleitung und der tatsächlich gelieferte Bausatz.

Lötzinn ist im aufgeführten Lieferumfang nicht genannt und wurde separat bei BerryBase bestellt. Das digitale Arbeitsbuch gehört dazu; ein zusätzliches gedrucktes Arbeitsbuch ist für den Einstieg nicht erforderlich.

## 3. Ergebnis der Bewertung

**Der Cube wurde als sinnvoller Einstieg für einen ersten kleinen Tamagotchi-Prototyp empfohlen.**

Die Plattform verbindet Lötpraxis mit einem dokumentierten Aufbau. Display, Eingabe und Tonausgabe sind bereits vorgesehen. Damit erscheint eine einfache spielbare Anwendung machbar.

Die Empfehlung bezieht sich auf eine erste Entwicklungsplattform. Eine vollständige Umsetzung aller langfristigen Spielideen auf diesem Board wurde nicht zugesichert und noch nicht getestet.

Die erneute kritische Prüfung anhand beider Projektdateien hat diese Einschätzung bestätigt: Für Lötenlernen und einen kleinen Prototyp am USB-Anschluss ist die Auswahl stimmig. Die endgültige Bedienung, die Hosentaschengröße und die angestrebte Batterielaufzeit lassen sich damit noch nicht prüfen oder zusagen. Ein fertiger Cube wäre eine Alternative, wenn die eigene Montage entfallen soll; aktuell ist aber der Lötbausatz ausgewählt.

### Einordnung des Kostenarguments

- Der Lötbausatz ist günstiger als die fertig aufgebaute Cube-Variante.
- Nach erfolgreicher Montage erhält man einen selbst aufgebauten Cube.
- Daraus folgt noch keine bestätigte Ersparnis für das gesamte Tamagotchi-Projekt: Zusätzliche Teile und spätere Hardware können weitere Kosten verursachen.
- „Dasselbe Ergebnis“ trifft auf den Vergleich mit einem fertig aufgebauten Cube zu. Die spätere Tamagotchi-Hardware kann andere Eigenschaften benötigen.

## 4. Vorgeschlagener erster Prototyp

Folgender Umfang wurde als realistischer Ausgangspunkt vorgeschlagen:

- Ein selbst gezeichnetes Tier auf dem Display.
- Eine einfache Animation.
- Hunger und Zufriedenheit, die sich während des Betriebs verändern.
- Ein kleines Menü zum Füttern, Spielen und Saubermachen.
- Unterschiedliche Gesichtsausdrücke je nach Zustand.
- Ein Piepton, wenn das Tier Aufmerksamkeit benötigt.

Der genaue Umfang ist noch nicht verbindlich festgelegt und wird an den verfügbaren Speicher angepasst.

**Vorschlag für die vorläufige Bedienung mit einem Taster:**

- Kurz drücken: nächsten Menüpunkt auswählen.
- Lange drücken: Auswahl bestätigen.

Diese Bedienung ist ein Vorschlag für den Prototyp. Für das spätere Gerät sind mindestens drei physische Knöpfe vorgesehen.

## 5. Technische Grenzen und offene Punkte

| Bereich | Einordnung |
| --- | --- |
| Display | Das kleine OLED ist flacher als die beiden erwogenen Anzeigeformate von ungefähr 3 × 3 cm und 7 × 5 cm. Es eignet sich für erste Versuche. |
| Displaytechnik | Der Cube verwendet ein OLED. Für das endgültige Gerät wurde ein Schwarz-Weiß-LCD als mögliche Richtung besprochen; das konkrete Modell bleibt offen. |
| Knöpfe | Ein Spieltaster ist vorhanden. Für die spätere Bedienung mit mindestens drei Knöpfen braucht es zusätzliche Taster oder andere Hardware. |
| Speicher | Der klassische Nano besitzt 2 KB Arbeitsspeicher und 32 KB Flash-Speicher; ein Teil des Flash-Speichers wird vom Bootloader belegt. Ein üblicher Vollbildpuffer für 128 × 64 Schwarz-Weiß-Pixel belegt bereits 1 KB. Grafiken und Spielumfang müssen sparsam umgesetzt werden; andere Pufferverfahren sind möglich. |
| Größerer Spielumfang | Welt, Einrichtung, verschiedene Tierdarstellungen und Generationen können weitere Hardware sinnvoll machen. Die tatsächliche Grenze hängt von der Umsetzung ab. |
| Stromversorgung | Für die ersten Versuche ist USB-Betrieb vorgesehen. Die spätere Batterieversorgung ist damit noch nicht gelöst. |
| Laufzeit | Die gewünschte Größenordnung von ungefähr einer Woche Batteriebetrieb ist noch nicht technisch bestätigt. |
| Spielstand | Der klassische Nano besitzt 1 KB nichtflüchtigen EEPROM-Speicher. Für einen kleinen Spielstand kann dieser genügen; ein separates Speichermodul wird für den Anfang nicht benötigt. Speicherhäufigkeit und Verhalten bei plötzlichem Stromverlust sind noch auszuarbeiten. |
| Uhrzeit | Die bisher beschriebene Cube-Ausstattung enthält keine batteriegepufferte Echtzeituhr. Verstrichene Zeit während des USB-Betriebs und die Uhrzeit über einen Stromausfall hinweg sind unterschiedliche Aufgaben. Die Lösung für Batteriewechsel und Stromunterbrechungen bleibt offen. |

## 6. Software für einen späteren Hardwarewechsel

Als technische Vorgehensweise wurde vorgeschlagen, die Spiellogik von der Hardware-Ansteuerung zu trennen:

- **Spiellogik:** Bedürfnisse, Aktionen, Entwicklung und Spielregeln.
- **Hardware-Ansteuerung:** Display, Knöpfe, Ton, Zeitquelle und Speichern.

So kann ein möglichst großer Teil der Spiellogik beim späteren Hardwarewechsel erhalten bleiben. Die Ansteuerung und gegebenenfalls die Darstellung müssen angepasst werden. Vollständige Wiederverwendung ohne Änderungen wurde nicht zugesichert.

Unter `software/schritt-04` liefert die Knopferkennung die Befehle Weiter, Bestätigen und Zurück. Diese Befehle können später auch von einzelnen Tasten oder von einem Steuerkreuz mit weiteren Tasten kommen. Der Simulator verwendet eine vorläufige Tasterbelegung an D2. Die tatsächlichen Anschlüsse und die Funktionsweise am Cube sind vor der Übertragung zu prüfen.

### Pinbelegung des Cube

Am 20. November 2026 von der Platinenbeschriftung in der [offiziellen Lötanleitung V2](https://funduinoshop.com/media/pdf/eb/80/f6/Loetanleitung-Funduino-Cube-DE-V2.pdf) abgelesen (Seite „Referenz: Die unbestückte Platine“). Am echten Cube noch nicht nachgemessen.

| Nr. in der Anleitung | Bauteil | Bezeichnung auf der Platine | Nano-Pin | Hinweis |
| --- | --- | --- | --- | --- |
| 0 | Nano R3 | A1, zwei Buchsenleisten | – | USB zeigt zur oberen Kante. |
| 1 | LED 1 rot, LED 2 gelb, LED 3 grün | D1–D3 mit R1 200 Ω, R2 200 Ω, R3 330 Ω | **D12, D11, D10** | Kurzes Bein (flache Seite) ins eckige Loch neben „GND“. |
| 2 | Taster | SW1 mit R4 10 kΩ | **D9** | R4 zieht nach GND, der Taster verbindet mit 5 V. **Gedrückt = HIGH.** D9 kann keinen externen Interrupt. `software/schritt-04` nimmt noch D2 mit `INPUT_PULLUP` und Interrupt an und muss angepasst werden. |
| 3 | Bewegungsmelder (PIR) | U1 | D8 | |
| 4 | Lautsprecher | BZ1 | **D7** | „+“ zum eckigen Loch bei D7. |
| 5 | Ultraschall HC-SR04 | U2 | D5, D6 | Reihenfolge auf der Platine: GND, D5, D6, 5V. |
| 6 | LED-Ring WS2812B, 12 LEDs | U3 | D4 | Laut Anleitung der schwierigste Schritt, zuletzt löten. |
| 7 | Servo-Stiftleiste | – | D3 | |
| 8 | Infrarot-Empfänger VS1838B | U4 | D2 | |
| 9 | Fotowiderstand (LDR) | R5, dazu R6 10 kΩ | A7 | |
| 10 | Potentiometer | RV1 | A6 | |
| 11 | OLED 128 × 64, SSD1306 | U5 | A4 (SDA), A5 (SCL) | I²C. |
| 12 | MPU-6050 | U7 | A4, A5 | I²C, gleiche Leitung wie das OLED. Am 21. November 2026 verdreht eingelötet und wieder entfernt; Platz derzeit leer. |
| 13 | DHT11 | U6 mit R7 1,2 kΩ | A3 | |
| 14 | Extern 1 / 2 / 3 | Buchsen- und Stiftleisten | I²C (A4, A5) / Seriell (D0, D1) / A1, A2 | An Extern 1 könnte später der MCP23017 zum Testen der Retro-Gamer-Tasten. |

Für Evi auf dem Cube wichtig: OLED, Taster, Lautsprecher. Nützliche Extras: LEDs als Hinweis „Evi braucht dich“, Fotowiderstand für „Licht aus = Schlafenszeit“, Potentiometer als Drehrad für das Menü.

## 7. Kaufmöglichkeiten und bisherige Suche in Kassel

Aktueller Stand: Cube und ausgewähltes Zubehör wurden laut Nutzerin bei Funduino bestellt. Die folgenden lokalen Kaufhinweise dokumentieren die frühere Recherche; sie sind keine noch offene Voraussetzung für den Einstieg.

Recherche in diesem Gespräch, Stand 16. November 2026:

- Für genau diesen Cube-Lötbausatz wurde kein bestätigter Ladenverkauf oder lokaler Lagerbestand in Kassel gefunden.
- Das ist kein Nachweis, dass kein Händler ihn beschaffen kann.
- Als belegte Bestellmöglichkeit wurde die oben verlinkte Funduino-Produktseite gefunden.
- Dort wurden bei der Recherche **29,99 € plus Versand** und **1–3 Werktage Lieferzeit** angezeigt. Preise und Verfügbarkeit können sich ändern.

### Mögliche lokale Anfrage

**NT-Elektronik**

- Website: https://www.nt-elektronik.de/
- Adresse laut eigener Website: Wolfsschlucht 19, 34117 Kassel.
- Telefon: 0561 / 2020858.
- Ob der Laden Funduino führt oder den Cube beschaffen kann, ist **unbestätigt**.
- Vor einer Fahrt telefonisch nach „Funduino Cube Lötübung, Artikelnummer F23110296“ fragen.

Ältere Branchenverzeichnisse nannten eine andere Adresse; für diese Zusammenfassung wurde die Angabe der eigenen Website verwendet.

### Kaufland-Angebot

Ein gefundenes Kaufland-Angebot war ein Online-Marktplatzangebot vom Verkäufer Funduino. Es belegt keine Verfügbarkeit in einer Kasseler Kaufland-Filiale.

## 8. Bestellte Artikel und nachgeschlagene Produktnamen

Die Nutzerin hat am **16. November 2026** in diesem Projektchat bestätigt, den Cube, das Funduino-Zubehör, Lötzinn und einen neuen Lötkolben bestellt zu haben. Das Lötkolbenmodell **Zhongdi ZD-99** wurde auf Nachfrage ausdrücklich bestätigt. Die Produktnamen und die bisherigen Warenkorbpreise wurden im Chat „Warenkorb prüfen“ nachgeschlagen. Bestellbestätigungen und Rechnungen wurden nicht eingesehen; Versandkosten und tatsächliche Endbeträge sind deshalb noch nicht dokumentiert. Lieferstand laut Nutzerin: **Funduino-Lieferung am 19. November 2026 angekommen, BerryBase-Lieferung (Lötzinn und Zhongdi ZD-99) am 20. November 2026 angekommen.** Damit ist die Bestellung vom 16. November vollständig da; ob jedes Teil im Paket war, wurde nicht einzeln geprüft.

### Funduino

Bestellt: jeweils ein Artikel beziehungsweise ein Set gemäß der zuvor festgelegten Einkaufsliste. **Angekommen am 19. November 2026** (laut Nutzerin).

| Ausgewählter Artikel | Preis laut Warenkorb-Chat | Zweck |
| --- | --- | --- |
| Funduino Cube Lötübung – Lern-Board für Arduino, mit Anleitung; F23110296 | 29,99 € | Bausatz für den Einstieg. |
| Filamentzange – Kneifzange für 3D-Druck, gehärteter Stahl; F23107297 | 2,13 € | Für das Kürzen der Bauteildrähte im ersten Cube-Aufbau vorgesehen; Einordnung unten. |
| Lötspitzenreiniger – Messingschwamm, Spiralwolle, trocken | 4,70 € | Reinigen der Lötkolbenspitze. |
| ESD Pinzette Set – antistatisch, 7 Stück, für Modellbau | 2,39 € | Kleine Bauteile greifen und positionieren. |
| CMT-136 Entlötpumpe – manuell, antistatische Spitze | 2,61 € | Überschüssiges Lötzinn für Korrekturen entfernen. |

**Bisher dokumentierte Artikelsumme Funduino: 41,82 € zuzüglich Versand**, nach dem damaligen Warenkorbstand; kein geprüfter Rechnungsbetrag.

Nachprüfung zur Zange: Die aktuelle Funduino-Produktseite beschreibt hauptsächlich das Schneiden von Kunststofffilament. Auf der älteren offiziellen Funduinoshop-Seite wird jedoch dieselbe Artikelnummer **F23107297** ausdrücklich zum Schneiden von Filament und elektronischen Komponenten angeboten. Damit ist die günstige Zange für den ersten Cube-Aufbau eine plausible Auswahl; praktische Tests und genaue Angaben zu Drahtmaterial beziehungsweise maximalem Drahtdurchmesser fehlen. Die frühere Aussage aus dem anderen Chat über Kupferdraht bis 1 mm ist durch diese Produktseiten nicht belegt. Die zwischenzeitlich empfohlene Knipex Electronic Super Knips 78 61 125 für 20,90 € ist eine passende Alternative, aber für diesen Einstieg kein notwendiger Kauf. Empfehlung nach erneuter Prüfung: die bereits ausgewählte Funduino-Zange für den Anfang beibehalten.

### BerryBase

- **1 × Goobay Lötzinn bleifrei, ø 0,8 mm, No-Clean-Flussmittel, Kolophoniumkern 2,5 %, 217 °C; 20 g; Artikelnummer 77421.**
- Preis laut Warenkorb-Chat und Produktseite: **7,90 € zuzüglich Versand**.
- Es handelt sich um Elektroniklot mit Flussmittelseele für die Lötverbindungen. Verzinnter Kupferdraht ist kein Ersatz dafür. Der bei Funduino gefundene „PCB-Lötdraht“ hatte widersprüchliche Beschreibungen und wurde nicht als Lötzinn ausgewählt.
- Bestellung bei BerryBase von der Nutzerin bestätigt. Im anderen Chat wurden die 20 g für den ersten Cube-Aufbau als ausreichend eingeschätzt; eine Aufstockung auf 50 g war nicht vorgesehen.
- **Angekommen am 20. November 2026** (laut Nutzerin).

### Neue Lötstation: Zhongdi ZD-99 – angekommen

- **Zhongdi ZD-99 Analoge Lötstation, Temperaturregelung 150–450 °C, Lötspitzenwechsel, 58 W, 230 V; Artikelnummer ZD-99-58W.**
- Bei BerryBase bestellt; Modell und Bestellung von der Nutzerin ausdrücklich bestätigt. Die frühere offene Neukaufentscheidung ist damit erledigt.
- Bisher dokumentierter Preis: **18,90 € zuzüglich Versand**. Laut bisheriger Produktbeschreibung sind Lötkolben und Halterung enthalten; nach Lieferung den tatsächlichen Lieferumfang prüfen.
- **Angekommen am 20. November 2026** (laut Nutzerin). Lieferumfang und ein erster Funktionstest (Aufheizen) sind noch nicht geprüft.
- Ein älterer Lötkolben ist weiterhin vorhanden, sein Modell ist nicht bekannt. Seine Prüfung ist keine Voraussetzung mehr für die Beschaffung der neuen Station.
- Ein zusätzlicher separater Lötkolbenständer wurde ausdrücklich nicht gewünscht und nicht hinzugefügt.
- Eine hitzefeste Arbeitsfläche und geeignete Belüftung für die Lötarbeiten vorsehen. Der tatsächliche Arbeitsplatz wurde noch nicht geprüft.

Damit beträgt die **Artikelsumme nach den dokumentierten Einzelpreisen 68,62 €**: Funduino 41,82 €, BerryBase-Lötzinn 7,90 € und Lötstation 18,90 €. Die tatsächlichen Rechnungsbeträge einschließlich Versand sind noch nicht abgeglichen.

## 9. Was für den Cube notwendig oder optional ist

- Notwendig für die eigene Montage: geeigneter Lötkolben, Elektronik-Lötzinn, ein geeignetes Werkzeug zum Kürzen der Drahtenden sowie Ablage und Spitzenreinigung.
- Ein spezieller Spitzenreiniger ist keine technische Voraussetzung: Die Cube-Lötanleitung zeigt auch die Reinigung mit einem feuchten geeigneten Schwamm. Der Messingschwamm wurde hier aus Komfortgründen mitbestellt.
- Pinzetten und Entlötpumpe sind hilfreiche Ergänzungen, keine Voraussetzung für die Funktionsweise des Cube; beide wurden bewusst in die Bestellung aufgenommen.
- Ein normales Digitalmultimeter mit Messspitzen und Durchgangsprüfung ist für Fehlersuche und Spannungsmessungen hilfreich, aber für den ersten Aufbau nicht zwingend. Es gehört nicht zu den bisher bestätigten Bestellungen. Die Durchgangsprüfung erfolgt an der stromlosen Schaltung.
- Ein USB-Spannungs-/Strommesser ersetzt dieses Multimeter nicht bei der Prüfung einzelner Verbindungen.
- Kein zusätzliches MPU-6050-Modul für den ersten Prototyp kaufen. Unabhängig von der Sensorvariante im gelieferten Cube ist Bewegungserkennung für den vorgeschlagenen ersten Spielumfang nicht erforderlich.
- Die angebotenen ESP32- und Raspberry-Pi-Pico-Adapter sind für einen späteren Wechsel des Mikrocontrollers gedacht. Mit dem mitgelieferten Nano sind sie für den Einstieg nicht nötig; ein Adapter allein enthält nicht automatisch den entsprechenden Mikrocontroller.
- Zusätzliche Displays, Taster, Lautsprecher, Steckbrett, Batterieteile und Echtzeituhr sind für den zunächst beschriebenen USB-Prototyp nicht erforderlich.
- Die offizielle Lötanleitung beschreibt den LED-Ring als anspruchsvolleren Schritt und empfiehlt ihn für Anfänger zuletzt. Der Aufbau soll entsprechend schrittweise erfolgen.

### Einkaufsliste Bauhaus Kassel, Holländische Str. 203a (Stand 20. November 2026)

Bestand auf bauhaus.info unter „Reservieren & Abholen in Kassel“ am 20. November 2026 nachgesehen. Bestände ändern sich. **Gekauft am 20. November 2026:** die Löthilfe LA 240. Die übrigen Artikel sind nur Vorschläge. Eine Telefonleitung als Versuchsdraht wurde besprochen und von der Nutzerin weggelassen.

| Artikel | Prod.-Nr. | Preis | In Kassel | Wozu |
| --- | --- | --- | --- | --- |
| [CFH Löthilfe LA 240](https://www.bauhaus.info/loetzubehoer/cfh-loethilfe-la-240/p/30525555) („dritte Hand“, 2 Krokodilklemmen, ohne Lupe) | 30525555 | 7,50 € | **gekauft** | Hält Platine oder Drähte beim Löten, z. B. am Pololu-Wandler und an der Tastenplatine. |
| [Profi Depot Digital-Multimeter MM 2500](https://www.bauhaus.info/multimeter/profi-depot-digital-multimeter-mm-2500/p/22183127) | 22183127 | 24,90 € | 13 Stück | Nur falls bei BerryBase kein Multimeter mitbestellt wurde. Durchgangsprüfung (Piepser-Symbol) auf der Verpackung prüfen; auf der Website nicht bestätigt. |
| [CFH Entlötlitze EL 235](https://www.bauhaus.info/entloetlitzen/cfh-entloetlitze-el-235/p/30527773) | 30527773 | 4,95 € | vorrätig | Saugt überschüssiges Zinn auf, z. B. bei Zinnbrücken zwischen Nano-Pins; ergänzt die Entlötpumpe. |
| [Robbyrob Isopropanol 99,9 %, 0,5 l mit Sprühkopf](https://www.bauhaus.info/profi-reinigungsmittel/robbyrob-reinigungsmittel-isopropanol-999-/p/33018151) | 33018151 | 7,95 € | vorrätig | Optional: Flussmittelreste von der Platine putzen (mit alter Zahnbürste). |
| [Alpha Tools Digitaler Messschieber](https://www.bauhaus.info/schieblehren-messschieber/alpha-tools-digitaler-messschieber/p/20497503) | 20497503 | 15,95 € | 16 Stück | Optional: Maße im Retro Gamer (Displayfenster, freie Höhe) und später die Bauhöhe des Waveshare. |

Unterlage: Eine Fliese wurde von der Nutzerin abgelehnt. Eine Silikon-Lötmatte war in Kassel nicht vorrätig. Für den Anfang reicht ein altes Holzbrett; eine Silikon-Lötmatte kann bei der nächsten BerryBase-Bestellung mitkommen.

Nicht empfohlen: Abisolierzangen im Bauhaus beginnen bei 0,2 mm². Die dünnen Jumperkabel (AWG 28, etwa 0,08 mm²) sind dafür zu dünn.

## 10. Bisherige Kandidaten für das spätere tragbare Gerät

### Displayauswahl vom 17. November 2026

Die Nutzerin bevorzugt das **Waveshare RP2350-Touch-LCD-2.8** (BerryBase **WS-30706**, recherchierter Preis **25,90 €**) wegen Farbe und kapazitivem Fingertouch. Das Board vereint Display, Mikrocontroller und Echtzeituhr. Ein Lautsprecher liegt bei. Die Hintergrundbeleuchtung lässt sich dimmen und braucht auch bei einem stehenden Bild Strom. Die Passform im Retro Gamer und die Batterielaufzeit bei dauerhaft sichtbarem Bild sind offen. [BerryBase](https://www.berrybase.de/waveshare-rp2350-kapazitive-2-8-ips-touchscreen-dev-board-240x320-imu-rtc-16mb-flash-audio) · [Waveshare-Dokumentation](https://www.waveshare.com/wiki/RP2350-Touch-LCD-2.8)

| Modell | Sichtbare Fläche (B × H) | Außenmaß (B × H) | Anzeige und Bedienung |
| --- | --- | --- | --- |
| Waveshare 1,54″ (18079) | 27,72 × 27,72 mm | 50 × 35 mm | Farbe, ohne Touch. [Waveshare](https://www.waveshare.com/wiki/1.54inch_LCD_Module) |
| Waveshare 1,54″ Touch (34370) | 27,72 × 27,72 mm | 40,2 × 40,2 mm | Farbe, Fingertouch. [Waveshare](https://docs.waveshare.com/1.54inch_Touch_LCD_Module) |
| Waveshare 2,4″ (18366) | 48,96 × 36,72 mm | 70,50 × 43,30 mm | Farbe, ohne Touch. [Welectron](https://www.welectron.com/Waveshare-18366-24inch-LCD-Module) |
| **Waveshare RP2350-Touch-LCD-2.8 (30706)** | **57,60 × 43,20 mm** | **73,06 × 50,54 mm** | **Farbe, 320 × 240 Pixel im Querformat, Fingertouch.** [Waveshare](https://www.waveshare.com/wiki/RP2350-Touch-LCD-2.8) · [BerryBase](https://www.berrybase.de/waveshare-rp2350-kapazitive-2-8-ips-touchscreen-dev-board-240x320-imu-rtc-16mb-flash-audio) |
| Adafruit Sharp Memory LCD 2,7″ (4694) | 58,8 × 35,3 mm | 63,50 × 55,88 mm | Schwarz auf Grau, ohne Touch, bei Umgebungslicht ohne Hintergrundbeleuchtung sichtbar. [Adafruit](https://www.adafruit.com/product/4694) · [Platinenzeichnung](https://github.com/adafruit/Adafruit-Sharp-Memory-Display-PCBs/blob/master/Adafruit%202.7in%20Sharp%20Memory%20Display.brd) |
| Waveshare 3,2″ Touch LCD (D) (16498) | 64,8 × 48,6 mm | 94,97 × 62,24 mm | Farbe, Touch per Druck, große Platine. [Waveshare](https://www.waveshare.com/product/3.2inch-320x240-touch-lcd-d.htm) |

### Entscheidung vom 20. November 2026: Waveshare RP2350-Touch-LCD-2.8

Die Nutzerin hat sich für das **Waveshare RP2350-Touch-LCD-2.8** entschieden. Ausschlaggebend waren diese Vorgaben:

- **Farbe soll möglich bleiben.** Evi und die Spielwelt bleiben vorerst schwarz-weiß. Farbe für Möbel, Laden oder neue Zeichnungen soll später ohne Hardwarewechsel möglich sein.
- **Betrieb mit Batterien**, möglichst AAA, kein Akku als Voraussetzung.
- **Evi muss nicht dauerhaft sichtbar sein.** Das Display darf ausgehen und wird per Knopf oder Touch geweckt. Damit entfällt die Anforderung „dauerhaft sichtbar“ aus der Anforderungsliste.
- Evi wird meist bei Tageslicht angeschaut.

Verworfen wurde das **Sharp Memory LCD 2,7″ (Adafruit 4694)**. Es passt sehr gut zu Evis reiner Schwarz-Weiß-Pixelkunst und wäre bei dauerhaft sichtbarem Bild sparsamer. Farbe ist damit aber nicht möglich.

**Folgen für den Aufbau:**

- **Laufzeit:** Die Hintergrundbeleuchtung verbraucht den meisten Strom. Bei ausgeschaltetem Display und schlafendem Chip ist eine Woche mit 4 AAA-Batterien nach grober Abschätzung erreichbar. Waveshare nennt keine Ruhestromwerte. Der Ruhestrom des ganzen Boards muss deshalb gemessen werden.
- **Stromversorgung:** Laut Waveshare gibt es nur USB-C und einen MX1.25-Anschluss für einen 3,7-V-Lithium-Akku mit Ladeschaltung. AAA-Batterien dürfen **nicht** an den Akkuanschluss. Vorgesehen ist: 4 × AAA → Wandler auf 5 V → aufgeschnittenes USB-Kabel → USB-C-Buchse des Boards. 4 × AAA liefern je nach Batterieart und Ladezustand etwa 3,6 bis 6,4 V. Direkt an USB wären frische Alkali-Batterien zu hoch. Der Wandler muss einen **sehr niedrigen Eigenverbrauch** haben, weil das Gerät im Mittel höchstens etwa 5 mA brauchen darf. Vorgesehen ist der Pololu S9V11F5 (unter 0,2 mA). Die Pololu-S13V-Reihe scheidet mit 10–20 mA aus.
- **Uhr beim Batteriewechsel:** Die Echtzeituhr ist ein PCF85063. Laut Waveshare gibt es einen Anschluss für eine **wiederaufladbare** Pufferbatterie. Bauform und Stecker sind nicht dokumentiert. Die Pufferbatterie wird erst nach Lieferung anhand des Boards ausgewählt. Bis dahin beim Batteriewechsel kurz USB anschließen.
- **Lieferumfang laut Waveshare:** Board, Lautsprecher 8 Ω / 2 W, 2 × SH1.0-Kabel 4-polig, 1 × SH1.0-Kabel 12-polig. Akku nicht enthalten.

### Einkaufsliste bei BerryBase (Stand 20. November 2026)

**Bestellstatus 20. November 2026:** Laut Nutzerin sind die Artikel der BerryBase- und der Botland-Bestellung unten bestellt, außer der Pufferbatterie. Ob Multimeter und USB-C-Kabel dabei waren (nur „falls nicht vorhanden“), wurde nicht angegeben. Bestellbestätigungen wurden nicht eingesehen. **Botland-Lieferung am 23. November 2026 angekommen** (Foto der Nutzerin): Pololu S9V11F5 und das Dioden-Set. **BerryBase-Lieferung mit dem Waveshare-Board am 23. November 2026 angekommen** (Foto der Nutzerin): Waveshare RP2350-Touch-LCD-2.8 (SKU 30706, Karton noch zu), Adafruit MCP23017 mit beiliegenden Stiftleisten, Adafruit JST-SH-4-Pin-Kabel **200 mm (Adafruit 4401)** statt der geplanten 100 mm (passt genauso), Jumperkabel Male-Male 40 Adern, Schrumpfschlauch-Set, Multimeter PCWork PCW01A. Ein USB-C-Kabel ist nicht auf dem Foto, also wohl nicht mitbestellt; ein vorhandenes Datenkabel verwenden. **Erster Test am 23. November 2026 bestanden (Foto):** Am USB-C startet das Werks-Vorführprogramm, eine Malfläche mit hellblauem Hintergrund und Farbfeldern. Bild und Touch funktionieren.

Am 20. November 2026 kritisch überarbeitet. Die erste Fassung enthielt zu viel Testzubehör und einen ungeeigneten Wandler.

**Grundgedanke:** Zum Start braucht es nur das Board. Display, Touch, Lautsprecher und Uhr sind darauf, die Spielentwicklung kann sofort per USB beginnen. Alles für Batterie und Tasten kommt in eine zweite Bestellung, sobald das Board da ist und der Retro Gamer zerlegt wurde. Vorher sind Pufferbatterie, Ruhestrom und Tastenverschaltung ohnehin nicht bekannt.

**Bestellung 1 bei BerryBase (jetzt):**

| Anzahl | Artikel | Preis | Nötig? |
| --- | --- | --- | --- |
| 1 | [Waveshare RP2350-Touch-LCD-2.8, WS-30706](https://www.berrybase.de/waveshare-rp2350-kapazitive-2-8-ips-touchscreen-dev-board-240x320-imu-rtc-16mb-flash-audio) | 25,90 € | Ja. |
| 1 | [Multimeter PCWork PCW01A](https://www.berrybase.de/en/pcwork-pcw01a-digitales-multimeter) | 12,90 € | Ja, falls noch keins vorhanden. Für Tastenkontakte (Durchgang) und Strommessung. Messleitungen liegen bei. |
| 1 | [BlackCotton USB-A auf USB-C, 1 m](https://www.berrybase.de/blackcotton-usb-a-usb-c-kabel-usb-2.0-schwarz-1-0m) | 1,90 € | Nur falls zu Hause kein USB-C-Datenkabel passend zum Computer vorhanden ist. |
| 1 | [Adafruit MCP23017, ADA5346](https://www.berrybase.de/adafruit-mcp23017-i2c-gpio-expander-breakout) | 5,90 € | Ja. Die Nutzerin will alle 7 Retro-Gamer-Tasten nutzen (Steuerkreuz, A, B, Start); Reset kann als achte Taste dazukommen. |
| 1 | [Jumperkabel Male-Male, 40 Adern, trennbar, 20 cm](https://www.berrybase.de/en/40pin-jumper-dupont-kabel-male-male-trennbar-laenge-0-20-m) | etwa 1,60–1,80 € | Ja. Als dünner, farbiger Draht (AWG 28) für Tasten, MCP23017 und Wandler. Stecker abschneiden, Enden abisolieren. Kein passender Draht vorhanden. Lagerangaben widersprüchlich; sonst das [Set mit 3 Kabeln](https://www.berrybase.de/en/40pin-jumper-dupont-cable-set-1x-f-f-m-m-f-m-each-20cm) für 4,90 €. |
| 1 | [Schrumpfschlauch-Set 100-teilig, bunt](https://www.berrybase.de/en/heat-shrink-tubing-set-100-pieces-colourful) | etwa 1,70–2,80 € | Ja. Isoliert die Lötstellen an Wandler und Drähten. Mit Feuerzeug oder Heißluft schrumpfen. Aus Bestellung 2 vorgezogen. |
| 1 | [Adafruit JST-SH 4-Pin-Kabel (STEMMA QT/Qwiic), ADA4399](https://www.berrybase.de/adafruit-jst-sh-4-pin-kabel-1-mm-pitch-fuer-stemma-qt-und-qwiic-i2c-gnd-sda-scl-3-3-v), 100 mm | ab 1,30 € | Empfohlen. Verbindet MCP23017 und Waveshare ohne Löten. Laut Waveshare-Grafik hat der I²C-Stecker die Reihenfolge GND, 3V3, SDA, SCL wie bei Qwiic. |

**Bestellung bei Botland (jetzt):**

| Anzahl | Artikel | Preis | Nötig? |
| --- | --- | --- | --- |
| 1 | [Pololu S9V11F5, 5 V Step-Up/Step-Down](https://botland.de/aufwarts-abwartswandler/8738-s9v11f5-aufwarts-abwartswandler-5-v-15-a-pololu-2836-5904422305796.html) | 8,90 € mit MwSt., Versand ab 4,69 € | Ja, für den Betrieb mit 4 × AAA. Am 20. November 2026 im Browser geprüft: „Verfügbar, wir versenden heute“, Zahlung mit Klarna und PayPal, 30 Tage Rückgaberecht. |
| 1 | [Gleichrichterdioden-Set, 200 Stück, justPi](https://botland.de/gleichrichterdioden/23836-gleichrichterdioden-set-verschiedene-typen-200-stueck-justpi-5904422384258.html) | 7,90 € | Ja, für die Schottky-Diode der Batterieeinspeisung. Enthält laut Botland 1N5819 und 1N5822. Einzelne Schottky-Dioden führen weder BerryBase noch Botland. **Geliefert am 23. November 2026** in einzeln beschrifteten Tütchen. Auf dem Foto zu sehen: 1N5819 (Schottky, 1 A / 40 V, 20 Stück) und 1N5822 (Schottky, 3 A / 40 V, 10 Stück). Laut Nutzerin außerdem Tütchen mit 1 A / 1000 V und 3 A / 1000 V; das sind normale Gleichrichterdioden (vermutlich 1N4007 und 1N5408, Typnummern noch nicht abgelesen), für Evi nicht nötig. Vorschlag für die Batterieeinspeisung: **1N5819** (klein, 1 A reicht für das Board). Der helle Ring auf der Diode markiert die Kathode, sie kommt an VBus. |

**Einspeisestelle für die Batterie:** Laut [Waveshare-Pinbelegung](https://www.waveshare.com/wiki/RP2350-Touch-LCD-2.8) führt der 12-polige Stecker **VBus (5 V)** und GND. Dort speist der Pololu-Wandler über die Schottky-Diode ein (Anode am Wandler, Kathode an VBus). Die USB-C-Buchse bleibt frei zum Programmieren; die Diode verhindert, dass USB-Strom in den Wandler zurückfließt.

**Botland und Waveshare (Websuche am 23. November 2026, Seite selbst nicht geöffnet):** Botland führt Waveshare, auch kleine Displays, z. B. 1,28" rund mit Touch (Waveshare 24155, 18,90 €, laut Seite keine Nachlieferung geplant), 1,54" 240 × 240 (18079), 1,69" 240 × 280 (24382) und Boards mit eingebautem Display (RP2040-LCD-0.96, RP2040 mit 1,28" rundem Display 24580). Unser RP2350-Touch-LCD-2.8 (30706) wurde dort nicht gefunden. Reine Displays brauchen einen eigenen Mikrocontroller, Boards mit Display nicht.

EXP-Tech verkauft laut Nutzerin nur an Unternehmen und scheidet aus. RobotShop EU ließ sich wegen einer Bot-Prüfung nicht prüfen.

**Zum Multimeter:** Ein Mikroampere-Bereich ist laut Produktseite nicht bestätigt, aber auch nicht nötig. Für eine Woche mit 4 × AAA darf das Gerät im Mittel höchstens etwa 5 mA brauchen. Ein Milliampere-Bereich reicht dafür aus.

**Bestellung 2 (nach Lieferung des Boards und Zerlegung des Retro Gamer):**

Am 20. November 2026 entschieden: Wandler und Dioden-Set kommen jetzt mit (Botland). Die Pufferbatterie wird später bestellt; sie war laut Nutzerin bei BerryBase nicht auf Lager. Bis dahin fragt das Spiel nach einem Batteriewechsel die Uhrzeit ab.

- [RTC-Akku für Raspberry Pi 5 (ML2020, wiederaufladbar)](https://www.berrybase.de/en/rtc-battery-rechargeable-battery-for-raspberry-pi-5), BerryBase, etwa 5,70 €, wenn wieder lieferbar. Wiederaufladbare Lithium-Mangan-Zelle mit 2-poligem JST-Stecker. Ob der Stecker in die RTC-Buchse des Waveshare passt, dann am Board prüfen; sonst anlöten. Vor dem Einstecken die Polung mit dem Multimeter prüfen.
- Tasten nur, falls die Kontakte des Retro Gamer nicht weiterverwendet werden können.

Zur Pufferbatterie: Sie hält nur die Uhr am Laufen, während die AAA-Batterien gewechselt werden, und wird im Betrieb automatisch nachgeladen. Der Spielstand liegt im Flash-Speicher und braucht sie nicht.

**Gestrichen, mit Grund:**

| Artikel | Grund |
| --- | --- |
| Pololu S13V10F5 / S13V20F5 | Laut Pololu 10–20 mA Eigenverbrauch. Allein das würde 4 × AAA in wenigen Tagen leeren. S13V10F5 war außerdem nicht lieferbar. |
| SparkFun-Qwiic-Adapterkabel | Nur für Händler bestellbar. Nicht nötig: Dem Board liegen zwei 4-polige SH1.0-Kabel bei, die man notfalls aufschneidet und anlötet. |
| Adafruit JST-SH-Kabel ADA4399 | Aus demselben Grund verzichtbar. |
| Kurzhubtaster-Sortiment | Nur Ersatz für Retro-Gamer-Tasten. Zum Testen reicht Touch. |
| Breadboard und Dupont-Kabel | Das Board hat kleine SH1.0-Stecker, keine Breadboard-Stifte. Bringt hier wenig. |
| Batteriehalter und Schiebeschalter | Der Retro Gamer hat ein eigenes 4×AAA-Fach. Ein Hauptschalter ist für ein Tamagotchi eher unerwünscht, weil Evi weiterlaufen soll. |
| Litze | Die beiliegenden Kabel und die Leitungen im Retro Gamer reichen voraussichtlich. |
| eneloop-Akkus und Ladegerät | Normale AAA-Batterien funktionieren. |
| Zweites Board | Nicht nötig, solange am Board selbst kaum gelötet wird. |

### Frühere Vorschläge für das tragbare Gerät

- **Displaypräferenz bestätigt:** Flüssige Bildbewegungen sind wichtiger als die besprochene E-Paper-Variante mit langsameren Bildwechseln. Eine konkrete Bildrate ist noch offen.
- **Adafruit SHARP Memory Display Breakout 1,3 Zoll, 168 × 144, Produkt 3502:** als stromsparendes, reflektives Display ohne Hintergrundbeleuchtung vorgeschlagen. Sichtbare Fläche laut Produktseite nur **24,5 × 21 mm**, Platine etwa **40 × 39 mm**. Die kleinere sichtbare Fläche wurde noch nicht ausdrücklich akzeptiert. Laufzeit und Animationstempo im Gesamtgerät wurden nicht getestet.
- **Adafruit Grayscale OLED 1,5 Zoll, 128 × 128, Produkt 4741:** als weitere animationsfähige Möglichkeit besprochen. Energiebedarf bei Daueranzeige und Alterung dauerhaft leuchtender Pixel sind bei der Auswahl zu berücksichtigen. Keine Kaufentscheidung.
- **Waveshare 1,54-Zoll-Schwarz-Weiß-E-Paper-Modul:** kam wegen seiner annähernd quadratischen Fläche von ungefähr 27 × 27 mm in Betracht. Langsamere Bildwechsel und Auffrischvorgänge passen weniger gut zur inzwischen bestätigten Bewegungspräferenz; aktuell nicht bevorzugt.
- **Seeed Studio XIAO nRF52840:** kleiner Mikrocontroller mit mehr Arbeitsspeicher als der klassische Nano, als möglicher Kandidat genannt. Pinbelegung, Versorgung und gesamte Schaltung sind noch nicht festgelegt. Seine Ladefunktion darf bei der späteren Auswahl nicht mit beliebigen Batterien kombiniert werden; der gewünschte Batteriebetrieb bleibt gesondert zu planen.
- **Adafruit PCF8523 RTC, Produkt 5189, mit separat erhältlicher CR1220-Zelle:** als mögliche gepufferte Uhr genannt. Damit wäre nur die Zeit gesichert; das Speichern des Spielstands ist eine andere Aufgabe. Noch nicht ausgewählt.
- Mindestens drei Taster, ein Piezo-Signalgeber, Verbindungsmaterial, Speicher, Batteriehalter, Spannungsversorgung und Gehäuse wären gemeinsam auf die gewählte Plattform abzustimmen. Zuerst ist zu prüfen, welchen Speicher der gewählte Controller bietet.
- Beim bevorzugten Waveshare sind Mikrocontroller, Display, Uhr und Audio bereits kombiniert. Für das Batterieziel von etwa einer Woche ist der Verbrauch im Gesamtaufbau bei gut lesbarer Daueranzeige zu messen.

### Umbauversuch mit dem vorhandenen Billig-Game-Boy

Das Typenschild nennt „Retro Gamer“, Karsten International, Art.-Nr. 3307, und vier AAA-Batterien. Vorn gibt es Steuerkreuz, A, B, Start und Reset. Die Fotos zeigen die Tastenmatten, die Kontakte auf der Platine und den Lautsprecher. Auf einem Foto ist das Originaldisplay zur Seite geklappt und gibt einen Teil seiner Rückseite, das Flachbandkabel und die Platine darunter frei. [Vorderseite](billig-Gameboy1.jpeg) · [Typenschild](billig-Gameboy2.jpeg) · [Gehäuse innen](billig-Gameboy3.jpeg) · [Display und Tasten](billig-Gameboy4.jpeg) · [Platine unter dem Display](billig-Gameboy5.jpeg)

Beim Umbau mit dem Waveshare übernimmt das neue Board Anzeige und Spielsteuerung. Gehäuse, Tasten und Steuerkreuz kommen für die Wiederverwendung infrage, ebenso Lautsprecher und Batteriefach, falls sie mechanisch und elektrisch passen.

#### Bei der nächsten Zerlegung

Aktualisiert am 20. November 2026 nach der Boardentscheidung. Beim ersten Zerlegen nur schauen, fotografieren und messen; nichts abschneiden oder auslöten, bevor das Waveshare-Board da ist.

Maße des Waveshare zum Abgleich: außen **73,06 × 50,54 mm**, sichtbare Bildfläche **57,60 × 43,20 mm**. Die Bauhöhe des Boards wird nach Lieferung gemessen.

1. **Batterien entnehmen.** Vor jedem Lösen eines Teils fotografieren.
2. **Displayfenster:** Innenmaß der Öffnung in der Front (Breite × Höhe) messen. Ist die Sichtscheibe herausnehmbar? Wie dick ist sie? Durch eine Kunststoffscheibe mit Luftspalt funktioniert kapazitiver Touch voraussichtlich schlecht; eventuell entfällt die Scheibe.
3. **Pappprobe:** Ein Stück Pappe auf 73 × 51 mm zuschneiden und hinter die Front halten. Stoßen Schraubdome, Rippen, Tasten oder das Batteriefach an?
4. **Freier Raum:** Breite, Höhe und **Tiefe** hinter der Front bis zur geschlossenen Rückwand. Foto mit Lineal von innen, Front und Rückschale getrennt.
5. **Tasten:** Beide Seiten der Platine scharf fotografieren, besonders die Kontaktflächen unter Steuerkreuz, A, B, Start und Reset. Gesucht: Wie viele Leiterbahnen gehen von jeder Kontaktfläche ab? Haben alle Tasten eine gemeinsame Leitung? Wo sitzt der Chip der Originalelektronik (oft ein schwarzer Klecks)? Die Durchgangsprüfung folgt, wenn das Multimeter da ist.
6. **Batteriefach und Schalter:** Wohin führen die Leitungen vom Fach? Gibt es einen Ein/Aus-Schalter, wo sitzt er und was schaltet er?
7. **Lautsprecher:** Durchmesser und Aufdruck (Ohm, Watt), Befestigung. Alternativ passt eventuell der beiliegende Lautsprecher des Boards.
8. **Weitere Öffnungen:** Kopfhörerbuchse, Lautstärkerad, Ladebuchse, Steckplätze. Wo am Rand könnte eine Öffnung für USB-C hin? Über USB-C wird das Board programmiert.

Die Pinbelegung des Originaldisplays und die Programmierbarkeit der Originalplatine spielen keine Rolle mehr, weil beides ersetzt wird.

#### Befund der ersten Zerlegung (20. November 2026, nach Fotos der Nutzerin)

- **Originaldisplay:** Auf dem Flachbandkabel steht „TF-280S006-B-A2“. Das spricht für ein 2,8-Zoll-TFT, also dieselbe Größenklasse wie beim Waveshare. Es liegt im **Querformat** über dem Chip der Originalplatine; das Flachbandkabel geht an der rechten kurzen Seite ab. Eine erste Auswertung hatte das Display irrtümlich als hochkant beschrieben, weil ein Foto gedreht war.
- **Entscheidung der Nutzerin: Querformat**, 320 × 240 Pixel (4:3), wie in der Handy-App angelegt. Weil das Original schon quer liegt, muss das Fenster voraussichtlich nicht oder nur wenig umgebaut werden. Auf den Fotos wirkt das Fenster etwas flacher als 4:3. Ist es kleiner als die Bildfläche von 57,6 × 43,2 mm, wird es mit einer Feile vergrößert, oder das Spiel lässt am Rand etwas frei.
- **Waveshare-Maßzeichnung** ([Wiki](https://www.waveshare.com/wiki/RP2350-Touch-LCD-2.8)): Glas 73,06 × 50,54 mm, Platine dahinter 69,00 × 49,90 mm. Die Bildfläche liegt **nicht mittig**. An einer kurzen Seite bleiben etwa 4,7 mm Rand, an der anderen etwa 10,8 mm. USB-C sitzt an einer kurzen Kante, nach der Zeichnung vermutlich auf der Seite mit dem schmalen Rand. Am Board prüfen.
- **Tasten:** Acht Kontaktflächen mit Kammmuster auf der Vorderseite der Platine: Steuerkreuz (4, unter einer durchsichtigen Gummimatte), A, B, Start, Reset. Von jeder Fläche führt eine Leiterbahn schräg nach oben zum Chip. Vermutlich hat jede Taste eine eigene Leitung und eine gemeinsame Gegenseite; das prüft das Multimeter. Der MCP23017 hat genug Eingänge für alle acht.
- **Idee für die Tasten:** Den unteren Teil der Originalplatine als Tastenplatine behalten. Die Leiterbahnen zum alten Chip trennen und dünne Drähte zum MCP23017 anlöten. So bleiben Gummitasten und Tastengefühl erhalten. Ob die Originalplatine dafür gekürzt werden muss, hängt von der Bauhöhe des Waveshare ab.
- **Rückseite der Platine:** fast leer.
- **Batteriefach:** 4 × AAA in der Rückschale, rote und schwarze Leitung zur Platine (Aufdruck „VDD“). Auf der Platine sitzt ein kleiner Spannungsregler mit Aufdruck „6206A 3.6V“, der mit dem Umbau entfällt.
- **Bedienelemente an der linken Seite** (von vorn gesehen, nach den Fotos): ein Rad mit Beschriftung „VR“ auf der Platine, also ein Drehwiderstand, vermutlich für die Lautstärke, und ein kleiner Schiebeschalter, der hoch und runter geschoben wird. Der Schiebeschalter ist wahrscheinlich der Ein/Aus-Schalter; die Nutzerin vermutet das ebenfalls.
- **Lautsprecher:** rund, in der unteren Ecke der Front, Aufdruck **8 Ω, 0,25 W**. Die Impedanz passt zum Verstärker des Waveshare, der für einen 8-Ω-Lautsprecher mit 2 W ausgelegt ist. Wegen der kleineren Belastbarkeit die Lautstärke in der Software begrenzen. Alternativ den beiliegenden Lautsprecher verwenden, falls er in die Halterung passt.
- **USB:** Der Retro Gamer hat keine USB-Buchse. Für USB-C muss eine Öffnung in den Rand. Im Querformat liegt die Buchse des Waveshare an der linken oder rechten Gehäuseseite. Links sind schon Öffnungen für Rad und Schalter.
- **Offener Punkt Stromzufuhr:** Wenn die Batterien über ein aufgeschnittenes Kabel in die einzige USB-C-Buchse eingespeist werden, ist sie zum Programmieren belegt. Besser ist es, die 5 V innen über eine Diode auf die USB-Versorgung des Boards zu geben und die Buchse frei nach außen zu führen. Dafür muss der Schaltplan geprüft werden.

**Aufbaukonzept (20. November 2026):** Das Waveshare-Board bringt Bildschirm, Rechner, Uhr und Tonausgabe fertig verbunden mit; ein Display muss nicht angeschlossen werden. Es ersetzt das Originaldisplay hinter dem Fenster. Die Originalplatine bleibt als Tastenplatine im Gehäuse: Die Leiterbahnen von den Tasten zum alten Chip werden getrennt, die alte Elektronik bekommt keinen Strom mehr. Von den Tastenkontakten führen dünne Drähte zum MCP23017, von dort ein 4-adriges Kabel (3,3 V, GND, SDA, SCL) zum I²C-Anschluss des Waveshare. Batteriefach → Pololu S9V11F5 → 5 V zum Waveshare. Lautsprecherdrähte von der alten Platine ablöten und an den Lautsprecheranschluss des Waveshare führen. Ob die alte Platine ganz bleiben kann oder oben gekürzt werden muss, hängt vom Platz hinter dem Fenster ab.

Die Nutzerin misst vorerst nicht. Passt der Retro Gamer nicht, kommt ein eigenes Gehäuse in Frage.

**Noch zu messen (später):**

1. Das ausgebaute Originaldisplay: Breite × Höhe × Dicke. Das ist der beste Vergleich zum Waveshare.
2. Innenmaß des Displayfensters.
3. Freie Höhe zwischen Front und Rückschale an der Stelle des Displays, bei geschlossenem Gehäuse.
4. Mit Batterien prüfen: Schaltet der Schiebeschalter das Gerät ein und aus? Ändert das Rad die Lautstärke?

#### Einbauversuch mit dem Waveshare-Board (23. November 2026, Foto der Nutzerin)

- Das Waveshare-Board ist laut Nutzerin **ein paar Millimeter zu groß** für die Öffnung im Gehäuse. Sie will etwas Plastik wegschneiden.
- Die grüne Originalplatine trägt den Aufdruck **WL0218-04-24P**. Unten liegen die Kontakte für Steuerkreuz (unter durchsichtiger Gummimatte), A, B, START und RESET, jeweils als runde, vergoldete Kammkontakte. Oben sitzen der alte Chip (Aufdruck SPANSION), ein schwarzer Chip-Klecks und das Flachbandkabel TF-280S006-B-A2 zum alten Display. Links sind das Lautstärkerad (VR), der Regler 6206A 3.6V und der Lautsprecher mit zwei schwarzen Drähten.
- **Tastentest mit dem Multimeter (23. November 2026, laut Nutzerin):** Die linke Kammhälfte von A piept mit allen anderen Tasten (B, START, RESET, Steuerkreuz): Das ist die **gemeinsame Leitung**, und sie ist **Minus** (piept mit dem schwarzen Batteriedraht). Die rechte Hälfte von A piept mit nichts. Jede Taste hat also eine **eigene Leitung** und schaltet sie beim Drücken auf Minus. Passt direkt zum MCP23017: gemeinsame Leitung an GND, jede eigene Leitung an einen Eingang mit eingeschaltetem Pull-up. Gedrückt = LOW.
- **Passprobe (23. November 2026, Foto der Nutzerin):** Nach dem Wegschneiden von Plastik liegt das Waveshare-Board in der oberen Hälfte der Front, USB-C von vorn gesehen rechts. Das Bild ist am linken Rand minimal abgeschnitten, laut Nutzerin nicht schlimm. Die Unterkante des Boards endet knapp über den Öffnungen für Steuerkreuz, A und B. Die grüne Platine muss deshalb etwa zur Hälfte weg: Schnitt unterhalb des schwarzen Bauteils bei R19 und oberhalb des Steuerkreuzes. Der untere Teil mit allen Tastenkontakten bleibt als Tastenplatine. Vorschlag mit eingezeichneter Linie: [retro-gamer-schnitt.png](retro-gamer-schnitt.png). Die genaue Lage wird am Gehäuse angezeichnet, noch nicht geschnitten. Im wegfallenden Teil sind auch die Lötpunkte der Batteriedrähte (rot und schwarz, knapp über dem Lautstärkerad). Die alte Elektronik wird nicht mehr gebraucht. Die beiden Drähte werden vor dem Schneiden abgelötet (in voller Länge) und später an den Pololu-Wandler gelötet. Minus für die Tasten kommt vom MCP23017-GND an die gemeinsame Tastenleitung der Tastenplatine.
- **Lautsprecher (Beschluss 23. November 2026, Wunsch der Nutzerin):** Es wird der **beiliegende Waveshare-Lautsprecher** verwendet. Er hat schon den passenden Stecker, ist für das Board gemacht und lief im Test (Schritt 8). Der alte Lautsprecher (8 Ω, 0,25 W) wird an der grünen Platine abgelötet und ausgebaut. Wegwerfen erst, wenn der Waveshare-Lautsprecher sicher passt: Er kommt an die Stelle des alten hinter die Lautsprecherlöcher (mit Heißkleber oder doppelseitigem Klebeband befestigen). Prüfen, ob sein Kabel bis zum Anschluss am Waveshare reicht.
- **Linie angezeichnet (23. November 2026, Foto der Nutzerin):** [retro-gamer-platine-linie.jpg](retro-gamer-platine-linie.jpg). Batterie- und Lautsprecherdrähte sind abgelötet. Die Linie läuft waagerecht direkt unter R20 und C16, über dem Steuerkreuz. Links berührt sie fast eine kleine Kupferdurchkontaktierung oben am Steuerkreuz-Kreis. Empfehlung: etwa 1 mm **oberhalb** der Linie schneiden (auf der Seite, die wegfällt), damit dieser Punkt heil bleibt, danach die Kante glatt feilen, sodass keine Kupferfasern benachbarte Leiterbahnen verbinden. Glasfaserstaub feucht abwischen, nicht einatmen. Nach dem Schnitt mit dem Multimeter prüfen: Linke Kammhälfte von A piept weiterhin mit allen anderen Tasten (gemeinsame Leitung); die eigenen Leitungen piepen nicht miteinander. Über A und B sitzt je ein runder Kupferpunkt auf der eigenen Leitung: gute Lötpunkte für die Drähte zum MCP23017. Das Befestigungsloch rechts oberhalb der Linie fällt weg; prüfen, ob die Tastenplatine beim Drücken fest sitzt, sonst mit Heißkleber sichern.
- **Neue Möglichkeit (23. November 2026, laut Nutzerin passt es auch):** Schnitt **oberhalb der beiden Schraublöcher** (links neben „6206A“, rechts unter „DDB8“). Vorteil: Die Tastenplatine bleibt verschraubt, die Kupferdurchkontaktierung am Steuerkreuz ist weit weg. Worauf achten: Das rechte Loch sitzt etwas höher als das linke, also vom rechten Loch aus messen und über beiden Löchern **mindestens 2–3 mm Material** stehen lassen, sonst bricht der Steg beim Schrauben. Auf dieser Höhe liegen kleine Bauteile im Weg (Regler U4 „6206A“, C11, eventuell R19). Sie gehören zur alten Elektronik und können vorher mit dem Lötkolben oder der Zange entfernt werden; dann lässt sich sauber ritzen oder sägen. Weil mehr alte Elektronik auf der Tastenplatine bleibt, nach dem Schnitt zusätzlich im **Ohm-Bereich** prüfen: Zwischen jeder eigenen Tastenleitung und der gemeinsamen Leitung muss „unendlich“ (1 oder OL) stehen. Zeigt das Multimeter einen Wert (z. B. einige kΩ), hängt noch ein altes Bauteil dazwischen und die Leiterbahn muss getrennt werden, weil die Pull-ups des MCP23017 nur etwa 100 kΩ haben.
- **Platine geschnitten, passt perfekt (23. November 2026, Fotos der Nutzerin):** Schnitt oberhalb der Schraublöcher, beide Löcher erhalten. Rechts ist die Kante gerade, links zwischen „6206A“ und R19 unregelmäßig ausgebrochen; U4, C11 und R19 sind entfernt. Waveshare-Board (oben) und Tastenplatine (unten) liegen nebeneinander in der Front: [retro-gamer-einbau.jpg](retro-gamer-einbau.jpg), [retro-gamer-tastenplatine.jpg](retro-gamer-tastenplatine.jpg). Der alte runde Lautsprecher sitzt noch unten rechts in seiner Halterung, der ovale Waveshare-Lautsprecher liegt daneben. USB-C des Waveshare zeigt (von vorn) nach rechts an die Gehäusewand, dort ist noch keine Öffnung. Der I²C-Anschluss (GND, 3V3, SDA, SCL) sitzt am Waveshare (von vorn) rechts oben.
- **Tasten nach dem Schnitt (23. November 2026, laut Nutzerin):** Es piept weiterhin, aber nicht immer mit der linken Hälfte der anderen Tasten. Erklärung: Die Kontakte sind unterschiedlich geteilt (nach Foto). UP, DOWN, A, B: linke/rechte Hälfte. L, R, START, RESET: obere/untere Hälfte. Für jede Taste wurde festgestellt, welche Hälfte mit der linken Hälfte von A piept (= gemeinsame Leitung). **Ergebnis: Alle Tasten hängen nach dem Schnitt noch an der gemeinsamen Leitung, keine Brücke nötig.**

  | Taste | gemeinsame Leitung (Minus) | eigene Leitung (zum MCP23017) |
  |---|---|---|
  | A | links | rechts |
  | B | rechts | links |
  | UP | links | rechts |
  | DOWN | links | rechts |
  | L | unten | oben |
  | R | unten | oben |
  | START | unten | oben |
  | RESET | unten | oben |

  Die Platine ist **einseitig**: Leiterbahnen und Lötpunkte nur auf der Vorderseite (Tastenseite), Rückseite blank mit ein paar Löchern. Für A, B, START und RESET gibt es je einen runden Kupferpunkt auf der eigenen Leitung (nach Foto, Leiterbahn sichtbar). Für das Steuerkreuz kommen acht kleine Punkte in Frage, nummeriert in [retro-gamer-messpunkte.jpg](retro-gamer-messpunkte.jpg). **Gemessen (23. November 2026, laut Nutzerin):** UP = Punkt 1, R = 2, L = 3, DOWN = 4. Damit hat jede Taste einen Lötpunkt (in der Bilddatei jetzt blau beschriftet). Die Ohm-Messung entfällt: Auf der Tastenplatine sind nur noch Kondensatoren (C10, C13, C16) und die Drahtbrücke R20 („0“ = 0 Ω) übrig, die keine Taste dauerhaft mit Minus verbinden können; beim Piepen war jeweils nur eine Hälfte mit der gemeinsamen Leitung verbunden. **Gemeinsame Leitung (GND): Punkt 10** (quadratisches Lötauge mit Loch unten links), laut Nutzerin piept nur dieser; 5–9 nicht.

  **Geplante Belegung des MCP23017** (GPA7 und GPB7 bleiben frei, weil sie laut Datenblatt nur Ausgänge sind; Aufdrucke am Modul noch am Foto prüfen):

  | Taste | Lötpunkt | MCP23017 |
  |---|---|---|
  | UP | 1 | GPA0 (Aufdruck A0) |
  | DOWN | 4 | GPA1 (A1) |
  | L | 3 | GPA2 (A2) |
  | R | 2 | GPA3 (A3) |
  | A | blauer Punkt über A | GPA4 (A4) |
  | B | blauer Punkt über B | GPA5 (A5) |
  | START | blauer Punkt rechts über START | GPA6 (A6) |
  | RESET | blauer Punkt rechts über RESET | GPB0 (B0) |
  | gemeinsame Leitung | 10 | GND |

  MCP23017 zum Waveshare über das 4-polige SH1.0-/STEMMA-QT-Kabel (GND, 3V3, SDA, SCL). Eingänge mit eingeschaltetem Pull-up, gedrückt = LOW. Adresse voraussichtlich 0x20 (kein Konflikt mit Touch 0x1A/0x58, Uhr 0x51). **Vor dem Löten klären:** (a) Liegen die Gummimatten auf Lötpunkten? R, L und DOWN liegen innerhalb des weißen Kreises vom Steuerkreuz, also vermutlich unter der Gummimatte; ein Lötklecks könnte sie anheben. Dann stattdessen außerhalb auf der Leiterbahn löten (Lack abkratzen). (b) Wo der MCP23017 sitzt und wie die Drähte von der Vorderseite nach hinten kommen. (c) Aufdrucke des Moduls (Foto). **Beschluss 23. November 2026:** Am Waveshare wird nichts gelötet; der MCP23017 hängt nur über das 4-polige Kabel am I²C-Stecker. Die beiliegenden Stiftleisten des MCP23017 werden **nicht** eingelötet, damit das Modul flach bleibt: Drähte direkt in die Löcher des Moduls löten. Laut Nutzerin hat jede Taste eine eigene Gummimatte. **Verdeckt sind die Punkte von L, R und DOWN** (2, 3, 4); UP (1), A, B, START, RESET und Punkt 10 sind frei. Ersatz für L, R, DOWN: Zwischen C11 und C10 laufen 4–5 feine Leiterbahnen senkrecht vom Steuerkreuz zur Schnittkante (früher zum alten Chip), markiert in [retro-gamer-steuerkreuz-linien.jpg](retro-gamer-steuerkreuz-linien.jpg). Vorgehen: Steuerkreuz-Matte abnehmen, eine Messspitze auf die eigene Hälfte von L (oben), R (oben), DOWN (rechts), mit der anderen Spitze fest auf jede feine Linie drücken und leicht kratzen, bis es piept; Linie mit Stift markieren. Danach auf den drei Linien je ein 1–2 mm langes Fenster in den grünen Lack schaben (Cutterklinge flach, nur schaben) und **versetzt in der Höhe** anlöten, damit sich benachbarte Lötstellen nicht berühren. Freiliegendes Kupfer, das nicht verlötet wird, mit Nagellack oder Klebeband abdecken. Drähte durch die ausgebrochene Kerbe direkt darüber nach hinten führen. Foto mit aufgelegter Steuerkreuz-Matte (23. November 2026): Die Matte ist durchsichtig, rund, mit Loch in der Mitte; sie endet knapp unter Punkt 1 (UP), der Bereich mit den feinen Linien bleibt frei. Die Haltestifte der Matte stecken in den Löchern mit weißem Ring; diese Löcher sind also nicht für Drähte nutzbar. **Messung (23. November 2026, laut Nutzerin):** Von L, R und DOWN piept weder eine der feinen Linien (Spitze durch den Lack gedrückt) noch einer der Punkte 5–8. Nächster Test: alle blanken Metallflächen oben auf der Tastenplatine (Lötflächen der übrigen und entfernten Bauteile C16, C11, R20, C10, R19, C13, C15), beschriftet a–n in [retro-gamer-loetflaechen-oben.jpg](retro-gamer-loetflaechen-oben.jpg). **Ergebnis:** Von den eigenen Hälften (L oben, R oben, DOWN rechts) piept keine der Flächen a–n. Die **untere** Hälfte von L piept mit **a** (linke Lötfläche von C16): a liegt also auf der gemeinsamen Leitung (Minus), wie Punkt 10. Nächster Schritt: Lack an der oberen Kante über den feinen Linien (zwischen C11 und der Kerbe) mit feinem Schleifpapier oder einer Nagelfeile vorsichtig abschmirgeln, bis die Kupferlinien glänzen, dann erneut von 2, 3 und 4 aus messen.
- **Beschluss 23. November 2026 (Vorschlag der Nutzerin):** L, R und DOWN werden **direkt auf die Punkte 3, 2 und 4 unter der Matte** gelötet, sehr flach. Abschmirgeln entfällt vorerst. Vorgehen: Punkt mit wenig Zinn verzinnen, Drahtende nur 2 mm abisolieren und verzinnen, Draht **flach** auf die Platine legen (nicht hochkant) und auf kürzestem Weg unter dem Mattenrand herausführen; R nach rechts oben, L nach oben zwischen L und UP, DOWN nach rechts unten. Mitte der Matte frei lassen (dort drückt das Steuerkreuz). Erst einen Punkt (R) löten und mit Matte, Steuerkreuz und Front probieren: in Ruhe kein Piepen zwischen Draht und Punkt 10, beim Drücken Piepen. Klemmt es, kleine Rille in die Unterseite der Matte schneiden. Drähte vorerst lang lassen (20 cm), später kürzen. **Farben:** Minus (10) schwarz, UP weiß, DOWN grau, L lila, R blau, A grün, B gelb, START orange, RESET braun; Rot bleibt frei. Lötplan mit Farben: [retro-gamer-loetplan.jpg](retro-gamer-loetplan.jpg). **Kabel und Modul (Fotos der Nutzerin, 23. November 2026):** Für die 9 Tastendrähte die bunten Jumperkabel Stecker–Stecker (40 Stück, 20 cm). Am MCP23017 (Adafruit) stehen außen die Aufdrucke VIN, GND, SCL, SDA, Rst, A0–A7 und B7–B0 sowie GND; an beiden Enden je eine STEMMA-QT-Buchse (JST SH, 4-polig). Vom Waveshare liegen bei: 1 × 12-poliges SH1.0-Kabel (wird nicht gebraucht) und 2 × 4-polige SH1.0-Kabel mit kleinem weißem Stecker an einem Ende und schwarzen Steckbuchsen am anderen. Für MCP23017 ↔ Waveshare am einfachsten ein Kabel mit kleinen weißen Steckern an **beiden** Enden – **vorhanden: Adafruit 4401, JST SH 4-Pin, 200 mm, Stecker an beiden Enden** (Foto der Nutzerin). Ein Ende in eine STEMMA-QT-Buchse des MCP23017, das andere in den I²C-Stecker des Waveshare; kein Löten nötig. Farben: schwarz GND, rot 3V3, blau SDA, gelb SCL. Sonst ein Waveshare-4-Pol-Kabel: weißer Stecker ans Waveshare, schwarze Buchsen abschneiden und an VIN, GND, SDA, SCL des MCP23017 löten (Farben vorher am Stecker zuordnen). Draht: je eine Ader aus dem bunten Jumperkabel (BerryBase) abziehen, an einem Ende den Stecker abschneiden, 2 mm abisolieren. **Erster Draht gelötet (23. November 2026, Foto der Nutzerin):** Blau an Punkt 2 (R), flach nach oben zur Kante geführt. Gemessen: Drahtende ↔ R oben piept, Drahtende ↔ Minus (10) piept nicht, Minus ↔ R unten piept. Mit aufgelegter Matte und gedrücktem R: kein Piepen, aber im Ohm-Bereich (20k) **0,17–0,3 kΩ** (170–300 Ω), je nach Druck. Das Durchgangspiepen reicht bei den Steuerkreuz-Flächen (leitende Kohle, schwarz) und dem Gummikontakt nicht aus; für den MCP23017 mit 100-kΩ-Pull-up ist alles unter etwa 10 kΩ sicher „gedrückt“. Der Lötpunkt unter der Matte stört nicht. Zum Prüfen gedrückter Steuerkreuz-Tasten deshalb immer den Ohm-Bereich nehmen.
- **Alle 9 Drähte angelötet (23. November 2026, Foto der Nutzerin):** [retro-gamer-draehte.jpg](retro-gamer-draehte.jpg). Lila (L), weiß (UP) und grau (DOWN) gehen unter bzw. neben der Matte heraus, schwarz ist von hinten durch das Loch von Punkt 10 gesteckt. Nach Foto noch zu erledigen: (a) Das Ende des schwarzen Drahts steht vorne etwa 5 mm über und liegt blank auf der Platine; auf etwa 1 mm kürzen. (b) Weiß (UP) hat etwa 5 mm blanken Draht zwischen Lötpunkt und Hülle, nahe R20 und C10; mit Klebeband festlegen, damit er nichts berührt. (c) Die Drähte liegen noch lose: Grau läuft über die RESET-Fläche, Blau über die A-Fläche. Am Ende darf kein Draht über einer Tastenfläche oder unter einem Gummiknopf liegen; jeder Draht auf kürzestem Weg zur nächsten Kante und nach hinten. Die Richtung hängt davon ab, wo der MCP23017 sitzt (noch offen, Platz hinter der Tastenplatine prüfen). (d) Jeden Draht prüfen: Drahtende ↔ eigene Hälfte piept, Drahtende ↔ Minus piept nicht.
- **Lautsprecher (Nachtrag 23. November 2026):** Der Waveshare-Lautsprecher passt nicht an die Stelle des alten. Der alte ist mit dem Gehäuse verschmolzen und lässt sich nicht herausnehmen. Wird **als Letztes** entschieden. Möglichkeiten: den alten drin lassen und über das Steckerkabel des Waveshare-Lautsprechers anschließen (Lautstärke in der Software begrenzen, 0,25 W), oder den Waveshare-Lautsprecher an einer anderen Stelle unterbringen.
- **Noch offen am Retro Gamer:** (1) Kante glatt feilen. (2) ~~Tasten messen~~ erledigt. (3) Lautsprecher (als Letztes, siehe oben). (4) MCP23017 verdrahten: Lötpunkte an der Tastenplatine festlegen, Platz für das Modul suchen. (5) Öffnung für USB-C. (6) Stromversorgung: Batteriedrähte → Pololu S9V11F5 → Diode 1N5819 → 5 V des Boards. (7) Prüfen, ob die Gehäusehälften schließen.
- Das Waveshare-Board liegt mit seiner Platine dort, wo jetzt die obere Hälfte der grünen Platine ist. Vermutlich muss die grüne Platine oberhalb der Tasten abgetrennt werden (Idee „Tastenplatine“). Vorher die Leitungen der Tasten mit dem Multimeter prüfen.

#### Eingänge für die Spieltasten

- Für drei bis vier Taster kommen **GP28 und GP29** sowie **GP0 und GP1** bei freiem UART infrage. Touch kann parallel Spielbefehle auslösen. [Waveshare-Schaltplan](https://files.waveshare.com/wiki/RP2350-Touch-LCD-2.8/RP2350-Touch-LCD-2.8-Schematic.pdf) · [Pinbelegung am 12-poligen Stecker](https://github.com/subtilitas/DshotDisplay/blob/main/src/board_rp2350_touch_lcd_2_8.h)
- Für das vollständige Steuerkreuz plus **A, B und Start** sind **sieben Eingänge** einzuplanen, wenn jede Richtung einzeln erfasst wird. Dafür ist der **Adafruit MCP23017** (BerryBase **ADA5346**, recherchierter Preis **5,90 €**) ein Vorschlag. Er wird über 3,3 V, GND, SDA und SCL angeschlossen und hat 14 für Taster nutzbare Eingänge. GPA7 und GPB7 sind laut Datenblatt Ausgänge. [BerryBase](https://www.berrybase.de/adafruit-mcp23017-i2c-gpio-expander-breakout) · [Microchip-Datenblatt](https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP23017-Data-Sheet-DS20001952.pdf)
- Als Versuchsbelegung kann das Steuerkreuz die Auswahl bewegen, A bestätigen, B zurückführen und Start das Menü öffnen. Die Nutzung von Reset ist offen.

#### Vom Testaufbau zum Einbau

1. **Passform klären.** Die Außenmaße des Waveshare mit dem freien Innenraum und seine sichtbare Fläche mit dem Displayfenster abgleichen. Mit einer Pappschablone lässt sich die Position vor einem Gehäuseumbau prüfen. Für die Front eine Halterung und einen erreichbaren Touchbereich vorsehen. Falls die vorhandene Sichtscheibe übernommen werden soll, die Touchreaktion mit der Scheibe ausprobieren.
2. **Das Board außerhalb des Gehäuses testen.** Das Waveshare per USB-C versorgen. Mit den Herstellerbeispielen Bild, Touch und Helligkeit erproben und den mitgelieferten Lautsprecher am Board testen. Vor dem Anschluss des Original-Lautsprechers dessen Impedanz mit der Vorgabe des Boardherstellers abgleichen. [Waveshare-Beispiele](https://www.waveshare.com/wiki/RP2350-Touch-LCD-2.8)
3. **Die Spieltasten anbinden.** Für eine Weiterverwendung der vorhandenen Kontaktflächen erst ihre Verschaltung prüfen und sie elektrisch von der Originalelektronik trennen. Alternativ passende Kontakte unter den Originaltasten und dem Steuerkreuz vorsehen. Jeden Tastendruck am USB-Testaufbau prüfen.
4. **Das Spiel auf das Board übertragen.** Die vorhandene Spiellogik als Ausgangspunkt nehmen, die Darstellung für 320 × 240 Pixel im Querformat anpassen und Touch, Tasten und Ton mit den Spielaktionen verbinden. Spielstand und letzten Zeitstempel dauerhaft speichern und verstrichene Zeit über die Echtzeituhr nachrechnen. Für die Uhr beim Wechsel der Hauptbatterien ist ein passender Pufferakku vorgesehen.
5. **Stromversorgung entscheiden.** Der Ladeanschluss ist für einen einzelnen 3,7-V-Lithium-Akku vorgesehen. Für die vier AAA-Zellen des Originalfachs braucht es eine zum Board passende geregelte Versorgung. Den Verbrauch bei gut lesbarer Daueranzeige messen und damit die Laufzeit abschätzen. [Waveshare](https://www.waveshare.com/product/rp2350-touch-lcd-2.8.htm)
6. **Einbauen und prüfen.** Display, Tasten, Lautsprecher und Versorgung befestigen und die Kabel so verlegen, dass die Gehäusehälften schließen. Am zusammengebauten Gerät Touch, Tasten, Ton, USB-C-Zugang, Spielstand nach Neustart und Uhrzeit nach Batteriewechsel prüfen.

Die Zerlegeansicht des Tamagotchi aus „Ideen und Infos.md“ dient zusätzlich als Anregung für die räumliche Anordnung von Display, Tasten, Platine, Summer und Batterien.

## 11. Aktueller Projektstand und nächste Schritte

- Entwicklungsrichtung: erster kleiner spielbarer Prototyp auf dem Funduino Cube.
- Späterer Hardwarewechsel: akzeptiert.
- Bausatz: geprüft und für diesen Einstieg empfohlen.
- Lötkolben: älteres Gerät vorhanden; neue Zhongdi ZD-99-Lötstation am 20. November 2026 angekommen.
- Beschaffung: alle Funduino- und BerryBase-Artikel gemäß Abschnitt 8 laut Nutzerin bestellt. Keine Bestellung der Knipex-Zange; die günstige Funduino-Zange bleibt die Auswahl.
- Lieferung: Funduino am 19. November 2026, BerryBase am 20. November 2026 angekommen (laut Nutzerin). Botland (Pololu-Wandler, Dioden-Set) am 23. November 2026 angekommen. Vollständigkeit und Zustand der Teile vor dem Aufbau anhand der Anleitung prüfen.
- Montage begonnen (19. November 2026, laut Nutzerin, mit dem alten Lötkolben): Display, Nano, eine LED, einige Widerstände und ein Taster sind angelötet. Eine Textanzeige auf dem Display funktioniert. Welche Bauteile noch fehlen (z. B. Summer, LED-Ring, Sensor), ist nicht dokumentiert.
- Am 20. November 2026 für Evi fertig gelötet: Widerstände, Fotowiderstand, drei LEDs, Taster, Lautsprecher (zusätzlich zu Display und Nano). Die übrigen Sensoren und der LED-Ring sind für Evi nicht nötig und noch offen.
- Nächste Schritte: `software/schritt-05/cube-test` aufspielen und alle Bauteile prüfen, dann `software/schritt-05/evi-cube`.

## Quellen

Die Produktangaben und Kaufhinweise beruhen auf der Recherche dieses Gesprächs.

Die konkreten Artikel, bisherigen Warenkorbpreise und die Auswahl von 20 g Lötzinn wurden zusätzlich im Chat „Warenkorb prüfen“ vom 16. November 2026 nachgeschlagen. Für den Bestellstatus ist die ausdrückliche Bestätigung der Nutzerin in diesem Projektchat maßgeblich; dort wurde auch das Modell Zhongdi ZD-99 bestätigt.

- [Funduino Cube Lötübung – Produktseite](https://funduino.com/products/funduino-cube-lotubung-inkl-anleitung)
- [Funduino Cube – Komponentenübersicht](https://funduino-cube.de/uebersicht/)
- [Funduino Cube – Anleitungen und Lektionen](https://funduino-cube.de/)
- [Offizielle Lötanleitung Funduino Cube DE V2](https://funduinoshop.com/media/pdf/eb/80/f6/Loetanleitung-Funduino-Cube-DE-V2.pdf)
- [Funduino – Werkzeugübersicht mit den ausgewählten Zubehörnamen](https://funduino.com/collections/werkzeuge)
- [Funduino – ausgewählte Filamentzange](https://funduino.com/products/filamentzange-kneifzange-fur-filament)
- [Ältere offizielle Funduinoshop-Seite derselben Zange F23107297 – nennt auch elektronische Komponenten](https://funduinoshop.com/werkstatt/werkzeuge/zangen/filamentzange-kneifzange-fuer-filament)
- [Knipex – Electronic Super Knips 78 61 125, optionale Alternative](https://www.knipex.de/produkte/elektronikzangen/electronic-super-knips/electronic-super-knips/7861125)
- [BerryBase – Knipex 78 61 125, recherchierter Preis 20,90 €](https://www.berrybase.de/knipex-electronic-super-knips-seitenschneider-werkzeugstahl-ohne-facette-125mm-019d8cf138b373e1a5c5eaf1667be8a6)
- [BerryBase – ausgewähltes Goobay-Lötzinn, 20 g](https://www.berrybase.de/goobay-loetzinn-bleifrei-oe-0-8-mm-no-clean-flussmittel-kolophoniumkern-2-5-217-0c-019c4c39967e71bbb08cce62bcc8e1b8)
- [BerryBase – bestellte Zhongdi ZD-99-Lötstation](https://www.berrybase.de/zhongdi-zd-99-analoge-loetstation-temperaturregelung-150-4500c-loetspitzenwechsel-58w-230v)
- [Arduino Nano – technische Daten des klassischen Nano](https://store.arduino.cc/products/arduino-nano)
- [Arduino – EEPROM-Bibliothek und nichtflüchtiger Speicher](https://docs.arduino.cc/learn/built-in-libraries/eeprom/)
- [Adafruit – Speicherbedarf eines Displaypuffers](https://learn.adafruit.com/memories-of-an-arduino/large-memory-consumers)
- [Adafruit – SHARP Memory Display 3502](https://www.adafruit.com/product/3502)
- [Adafruit – Grayscale OLED 4741](https://www.adafruit.com/product/4741)
- [Waveshare – 1,54-Zoll-E-Paper-Modul](https://www.waveshare.com/1.54inch-e-paper-module.htm)
- [Seeed Studio – XIAO nRF52840](https://wiki.seeedstudio.com/XIAO_BLE/)
- [Adafruit – PCF8523 RTC 5189](https://www.adafruit.com/product/5189)
- [NT-Elektronik – eigene Website und Kontaktdaten](https://www.nt-elektronik.de/)
- [Kaufland – gefundenes Marktplatzangebot](https://www.kaufland.de/product/537928873/)

