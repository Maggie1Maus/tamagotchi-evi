# Tamagotchi-Projekt – Hardware-Einstieg mit dem Funduino Cube

Stand: 17. November 2026  
Status: Cube, Lötzubehör, Lötzinn und Zhongdi ZD-99 am 16. November 2026 laut Nutzerin bestellt. Lieferung, Montage und Funktionstests noch nicht bestätigt.

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

Die Nutzerin hat am **16. November 2026** in diesem Projektchat bestätigt, den Cube, das Funduino-Zubehör, Lötzinn und einen neuen Lötkolben bestellt zu haben. Das Lötkolbenmodell **Zhongdi ZD-99** wurde auf Nachfrage ausdrücklich bestätigt. Die Produktnamen und die bisherigen Warenkorbpreise wurden im Chat „Warenkorb prüfen“ nachgeschlagen. Bestellbestätigungen und Rechnungen wurden nicht eingesehen; Versandkosten und tatsächliche Endbeträge sind deshalb noch nicht dokumentiert. Lieferung und Empfang sind noch nicht bestätigt.

### Funduino

Bestellt: jeweils ein Artikel beziehungsweise ein Set gemäß der zuvor festgelegten Einkaufsliste.

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

### Neue Lötstation: Zhongdi ZD-99 bestellt

- **Zhongdi ZD-99 Analoge Lötstation, Temperaturregelung 150–450 °C, Lötspitzenwechsel, 58 W, 230 V; Artikelnummer ZD-99-58W.**
- Bei BerryBase bestellt; Modell und Bestellung von der Nutzerin ausdrücklich bestätigt. Die frühere offene Neukaufentscheidung ist damit erledigt.
- Bisher dokumentierter Preis: **18,90 € zuzüglich Versand**. Laut bisheriger Produktbeschreibung sind Lötkolben und Halterung enthalten; nach Lieferung den tatsächlichen Lieferumfang prüfen.
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

**Bestellstatus 20. November 2026:** Laut Nutzerin sind die Artikel der BerryBase- und der Botland-Bestellung unten bestellt, außer der Pufferbatterie. Ob Multimeter und USB-C-Kabel dabei waren (nur „falls nicht vorhanden“), wurde nicht angegeben. Bestellbestätigungen wurden nicht eingesehen. Lieferung noch nicht bestätigt.

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
| 1 | [Gleichrichterdioden-Set, 200 Stück, justPi](https://botland.de/gleichrichterdioden/23836-gleichrichterdioden-set-verschiedene-typen-200-stueck-justpi-5904422384258.html) | 7,90 € | Ja, für die Schottky-Diode der Batterieeinspeisung. Enthält laut Botland 1N5819 und 1N5822. Einzelne Schottky-Dioden führen weder BerryBase noch Botland. |

**Einspeisestelle für die Batterie:** Laut [Waveshare-Pinbelegung](https://www.waveshare.com/wiki/RP2350-Touch-LCD-2.8) führt der 12-polige Stecker **VBus (5 V)** und GND. Dort speist der Pololu-Wandler über die Schottky-Diode ein (Anode am Wandler, Kathode an VBus). Die USB-C-Buchse bleibt frei zum Programmieren; die Diode verhindert, dass USB-Strom in den Wandler zurückfließt.

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
- Lötkolben: älteres Gerät vorhanden; neue Zhongdi ZD-99-Lötstation bestellt.
- Beschaffung: alle Funduino- und BerryBase-Artikel gemäß Abschnitt 8 laut Nutzerin bestellt. Keine Bestellung der Knipex-Zange; die günstige Funduino-Zange bleibt die Auswahl.
- Lieferung: noch nicht bestätigt; nach Eingang Vollständigkeit und Zustand der Teile prüfen.
- Montage, Programmierung und praktische Tests: noch nicht erfolgt.
- Nächste Schritte: Lieferung entgegennehmen und prüfen, Arbeitsplatz und neue Lötstation vorbereiten, Cube nach Anleitung aufbauen und Display, Taster, Ton sowie USB-Programmierung prüfen. Danach kleinste spielbare Version konkretisieren.

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

