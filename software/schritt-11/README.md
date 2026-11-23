# Schritt 11: Tasten-Test für den Retro Gamer

Das Programm `tasten-test` prüft die 8 echten Tasten des Retro Gamers, bevor Evi sie benutzt. Die Tasten hängen über den **MCP23017** am Waveshare-Board.

## Aufbau (Stand 23. November 2026)

```
Tastenplatine ──9 Drähte──▶ MCP23017 ──Kabel Adafruit 4401──▶ I²C-Stecker am Waveshare (GND, 3V3, SDA, SCL)
```

| Taste | Lötpunkt auf der Tastenplatine | Drahtfarbe | Loch am MCP23017 |
|---|---|---|---|
| UP | 1 | weiß | A0 |
| DOWN | 4 | grau | A1 |
| L | 3 | lila | A2 |
| R | 2 | blau | A3 |
| A | blauer Punkt über A | grün | A4 |
| B | blauer Punkt über B | gelb | A5 |
| START | blauer Punkt rechts über START | orange | A6 |
| RESET | blauer Punkt rechts über RESET | braun | B0 |
| Minus | 10 | schwarz | GND (Masse-Loch direkt unter B0 oder neben VIN) |

Lötpunkte und Farben: [Lötplan](../../retro-gamer-loetplan.jpg), Löcher am Modul: [MCP23017-Belegung](../../retro-gamer-mcp-belegung.jpg). Jede Taste verbindet beim Drücken ihre Leitung mit Minus. Der MCP23017 hält jede Leitung über einen eingebauten Widerstand (Pull-up) auf „nicht gedrückt“. VIN, SCL und SDA am MCP23017 bleiben frei, Strom und Daten kommen über das Kabel.

## Aufspielen

1. **USB-Kabel abziehen.** Dann das Kabel Adafruit 4401 einstecken: ein Ende in eine der kleinen Buchsen am MCP23017, das andere in den I²C-Stecker (GND/3V3/SDA/SCL) am Waveshare.
2. USB wieder einstecken.
3. In der Arduino IDE `software/schritt-11/tasten-test/tasten-test.ino` öffnen, Board **Raspberry Pi Pico 2**, hochladen (wie in Schritt 8 beschrieben).

## Was auf dem Bildschirm steht

- **Oben:** „MCP23017 gefunden (0x20)“ oder „NICHT gefunden“, darunter alle Geräte am I²C-Bus. Normal sind **1A** (Touch), **20** (MCP23017), **51** (Uhr) und **6B** (Lagesensor).
- **Mitte:** die Tasten wie am Gerät. Eine gedrückte Taste wird **weiß**. Die Zahl darunter zählt, wie oft sie gedrückt wurde.
- **Unten links:** die Rohwerte. In Ruhe steht bei Reihe A **7F** und bei Reihe B **7F**.

Ohne MCP23017 sucht das Programm jede Sekunde neu.

## Was zurückmelden

1. Steht „MCP23017 gefunden“? Welche Geräte stehen in der Zeile „I2C“?
2. Ist in Ruhe **keine** Taste weiß?
3. Wird beim Drücken **genau die richtige** Taste weiß, und nur diese? (UP, DOWN, L, R, A, B, START, RESET)
4. Zählt die Zahl bei einem Druck um genau 1 weiter? Springt sie um mehrere, prellt der Kontakt; das wird im Spiel abgefangen.

**Wenn etwas nicht stimmt:**
- *MCP23017 nicht gefunden, aber 1A, 51 und 6B stehen da:* Kabel prüfen (beide Enden ganz eingesteckt?). Hängt der I²C-Stecker an einem anderen Bus, muss das Programm angepasst werden.
- *Eine Taste ist dauernd weiß:* Ihr Draht berührt Minus (blanke Stelle, Lötklecks) oder die Matte drückt dauernd.
- *Zwei Tasten werden gleichzeitig weiß:* Zwei Drähte berühren sich.
- *Eine Taste wird nie weiß:* Lötstelle an der Tastenplatine oder am MCP23017 prüfen, oder falsches Loch.

## Technik

- `board.h`, `bildschirm.h`: aus Schritt 8.
- MCP23017 an **Wire1 (GP6 = SDA, GP7 = SCL)**, 100 kHz, wie Touch, Uhr und Lagesensor. Dass der äußere I²C-Stecker an diesem Bus hängt, ist noch nicht am Schaltplan bestätigt; die Gerätezeile zeigt es.
- Adresse 0x20 bis 0x27 wird automatisch erkannt (Werkseinstellung 0x20).
- Register bei Werkseinstellung (IOCON.BANK = 0): A0–A6 und B0–B6 Eingänge mit Pull-up (IODIR = 0x7F, GPPU = 0x7F). A7 und B7 sind laut neuem Datenblatt nur Ausgänge und werden auf LOW gestellt.
- GPIOA und GPIOB werden etwa 100-mal pro Sekunde zusammen gelesen. Bit = 0 bedeutet gedrückt.

## Geprüft (23. November 2026, in der Cloud)

- Kein Arduino-Paket in der Cloud: nur mit g++ und Ersatz-Headern auf Schreibfehler geprüft (ohne Fehler). **Noch nicht in der Arduino IDE übersetzt und noch nicht auf dem Board.**
