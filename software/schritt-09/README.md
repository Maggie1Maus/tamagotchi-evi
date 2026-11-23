# Schritt 9: Evi zieht aufs Waveshare-Board

Evi läuft jetzt auf dem **Waveshare RP2350-Touch-LCD-2.8**, mit denselben Regeln wie auf dem Cube (Schritt 7).

## Aufspielen

1. In der Arduino IDE `software/schritt-09/evi-waveshare/evi-waveshare.ino` öffnen.
2. Board: **Raspberry Pi Pico 2** (wie in Schritt 8).
3. Mit dem Pfeil hochladen. Klappt es nicht: BOOT-Taste halten, USB einstecken, Port **UF2 Board** wählen, noch einmal hochladen.

Beim ersten Start beginnt eine neue Evi. Danach merkt sie sich ihren Stand.

## Was du siehst

- **Links Evi** aus deinen Zeichnungen der erwachsenen Evi: glücklich, hungrig, traurig, schmutzig, schlafend. Schwarze Linien auf weißem Grund wie auf Papier. Die Bilder sind nur verkleinert (das größte auf 170 Pixel Höhe) und in Schwarz und Weiß umgerechnet.
- **Rechts** die Balken **Satt**, **Laune** und **Sauber**. Ein „!“ heißt: 30 oder weniger.
- **Unten vier Knöpfe:**
  - **Essen:** +40 satt (Melodie, „Mjam!“)
  - **Spiel:** +30 Laune, Evi tanzt hin und her („Juhu!“)
  - **Putzen:** +60 sauber, Evi schüttelt sich („Sauber!“)
  - **Licht:** Licht aus, Evi schläft. Der Bildschirm wird dunkel mit hellen Linien. Im Schlaf werden die Werte viel langsamer weniger, und es gibt nur den Lichtknopf. Noch einmal tippen = Licht an, Evi wacht auf.
- Sinkt ein Wert auf 30, piept Evi einmal und der Bildschirm geht an. Im Schlaf wartet sie damit bis zum Aufwachen.

Regeln wie auf dem Cube und in der Handy-App (erwachsene Evi): pro Stunde wach −20 satt, −15 Laune, −12 sauber; im Schlaf nur 15 % bzw. 10 % davon.

**Bildschirm aus:** Nach 1 Minute ohne Berührung geht die Beleuchtung aus. Evi lebt weiter. Der erste Tipper schaltet den Bildschirm nur wieder an, erst der zweite drückt einen Knopf.

**Neue Evi:** Einen Finger auf den Bildschirm legen und dabei das USB-Kabel einstecken (oder RESET drücken). Evi fragt „Neue Evi?“. Mit **Ja** beginnt sie bei 80/80/100, mit **Nein** geht es normal weiter.

## Spielstand

- Gespeichert wird alle 30 Minuten, nach jeder Pflege und beim Licht an/aus, und nur, wenn sich etwas geändert hat.
- Das Board hat kein EEPROM wie der Nano. Die EEPROM-Bibliothek des Board-Pakets nutzt dafür einen 4-KB-Block im Flash, der bei jedem Speichern gelöscht und neu geschrieben wird. Flash hält etwa 100.000 Löschvorgänge. Bei rund 60 Speicherungen am Tag reicht das für mehrere Jahre. Später kann ein Dateisystem (LittleFS) das verteilen.
- **Zeit ohne Strom zählt als Pause**, wie beim Cube. Die Uhr auf dem Board (PCF85063) wird noch nicht benutzt. Sobald die Pufferbatterie da ist, kann Evi auch über die Zeit ohne Strom weiterrechnen.

## Dateien

- `evi-waveshare.ino`: Bildschirmaufteilung, Knöpfe, Licht, Hinweiston, Speichern, Bildschirm aus.
- `pet.h`, `save.h`: unverändert aus Schritt 7.
- `board.h`, `touch.h`: unverändert aus Schritt 8.
- `bildschirm.h`: aus Schritt 8, `zeigen(true)` zeigt das Bild umgekehrt (Nacht).
- `ton.h`: Melodien vom Cube, jetzt ohne Warten abgespielt, damit Bild und Touch weiterlaufen. Lautstärke 700.
- `evi_art.h`: die fünf Bilder, 15,5 KB. Nicht von Hand ändern, sondern neu erzeugen:
  `powershell -ExecutionPolicy Bypass -File software\schritt-09\tools\make-art.ps1`
  (Windows-PowerShell, kein Python nötig.)

## Geprüft (23. November 2026)

- Für „Raspberry Pi Pico 2“ ohne Fehler und Warnungen übersetzt: 96,5 KB Programm, 13,2 KB Arbeitsspeicher.
- Die umgerechneten Bilder am PC angesehen: alle fünf Zeichnungen gut erkennbar.
- **Auf dem Board (23. November 2026, Foto der Nutzerin):** Evi, Balken und Knöpfe werden richtig angezeigt. Der Grund ist laut Nutzerin in echt weiß (nur auf Fotos hellblau). Laut Nutzerin funktionieren alle Knöpfe mit Melodie und Bewegung, Licht und Schlafen, Bildschirm aus und Wecken sowie der Spielstand nach dem Ausstecken. Die Knöpfe sind gut zu treffen.
