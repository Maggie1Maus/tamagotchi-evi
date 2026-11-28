# Bauteile selbst zeichnen – Anleitung für iPad und Apple Pencil

Du zeichnest jedes Teil (Ohren, Gesicht, Schwanz, Fell-Muster) **einmal** auf eine eigene Ebene, immer über Evi. Alles Weitere (Kombinieren, Graustufen, Fellfarben) macht Claude am PC.

## Was du brauchst
- iPad mit Apple Pencil
- **Procreate** (einmalig rund 13 €, kein Abo). Kostenlos geht auch Adobe Fresco; die Schritte sind sehr ähnlich.
- Die Vorlage aus diesem Ordner:
  - `erwachsen/vorlagen/Vorlage-Evi.png` – Evi blass als Unterlage, 2048 × 2048 Pixel
  - `erwachsen/vorlagen/Vorlage-Evi-mit-Raster.png` – dasselbe mit hellem Raster als Orientierung

## 1. Vorlage aufs iPad bringen
Lege die PNG-Datei in iCloud Drive oder schicke sie dir per AirDrop oder Mail. Ein Weg über das Repository: auf GitHub `tamagotchi-evi` die Datei öffnen und „Download“ wählen.

## 2. In Procreate öffnen
1. Procreate starten, oben rechts **Importieren** → Datei wählen. Sie erscheint in der Galerie.
2. Öffnen. Die Leinwand ist 2048 × 2048, **nicht verändern** (nicht beschneiden, nicht drehen, nicht skalieren). Nur so passen alle Teile später deckungsgleich.
3. Evi liegt auf der untersten Ebene. Ebenen öffnest du mit dem Symbol aus zwei Quadraten (oben rechts). Tippe die Ebene an → **Sperren**, damit du sie nicht aus Versehen bemalst.

## 3. Pinsel einstellen (für saubere Linien)
- Pinsel: **Studio-Stift** (unter „Tinte“) oder **Monoline** (unter „Kalligrafie“).
- Farbe: **Schwarz**.
- Pinselgröße: um 10–14 % (Evis Linien sind etwa so dick wie ein Pixel-Block).
- **Stabilisierung hochdrehen:** Pinsel antippen → *Stabilisierung* → *Streamline* auf etwa 60–80 %. Das glättet wackelige Linien.
- **Zeichenhilfe** (Aktionen → Leinwand → Zeichenhilfe) kann ein Raster anzeigen, wenn du magst.
- Radieren: Radiergummi-Symbol, gleiche Größe wie der Stift.

## 4. Pro Teil eine Ebene
Tippe in der Ebenenliste auf **+**. Gib jeder Ebene einen Namen: Doppeltippen auf die Ebene → *Umbenennen*. Zeichne **nur das Teil**, nicht ganz Evi. Evis blasse Linien zeigen dir, wo es sitzt.

### Anfang: nur diese vier Teile
| Ebenenname | Was du zeichnest |
|---|---|
| `ohr-spitz` | ein Ohr (oben links), spitz |
| `ohr-rund` | ein Ohr, rund |
| `schwanz-buschig` | ein Schwanz, buschig |
| `muster-streifen` | Streifen auf dem Körper |

**Nur das linke Ohr und nur eine Seite zeichnen.** Spiegeln mache ich am PC. Ohren-Innenseite (dunkle Fläche) ruhig schwarz füllen.

Für jede Ebene: Linie zeichnen, dann Fläche füllen. Fülle in Procreate, indem du die **Farbe aus dem Farbkreis oben rechts auf die Fläche ziehst** (ColorDrop). Danach stimmt es, wenn die Linie geschlossen ist.

### Später (wenn der Anfang gefällt)
- Weitere Ohren: Schlappohren, kleine Ohren, Pinselohren …
- Gesichtsmerkmale: andere Augen, Nasen, Schnurrhaare, Wangenflecken
- Weitere Schwänze: dünn, kurz, lang, geringelt
- Weitere Muster: Flecken, Tupfen, Tigerstreifen, Masken am Gesicht

## 5. Wichtige Tricks
- **Beschneidungsmaske:** Muster-Ebene über die Körper-Ebene legen, Muster antippen → *Beschneidungsmaske*. Dann bleibt das Muster in den Linien.
- **Hintergrund ausblenden:** Weißer Hintergrund und Evis blasse Unterlage ausblenden (Häkchen weg), bevor du exportierst. Sonst sind die Teile nicht transparent.
- **Rückgängig:** Mit zwei Fingern kurz auf die Leinwand tippen. Vorwärts: mit drei Fingern.
- **Zwischendurch speichern:** Procreate speichert automatisch.

## 6. Abgeben
1. Aktionen (Schraubenschlüssel) → **Teilen** → **PSD**. Alle Ebenen bleiben erhalten.
2. Auf den PC bringen (iCloud Drive, AirDrop, Mail oder Cloud-Ordner) und im Projekt ablegen unter
   `entwuerfe/evi/zeichnen/` in den passenden Ordner (siehe unten, zum Beispiel `ohren/`).
3. Sag Claude Bescheid. Claude schneidet die Ebenen auseinander, spiegelt, setzt Graustufen und Fellfarben und baut alle Kombinationen.

## Regeln, damit alles passt
- Leinwandgröße immer 2048 × 2048, nichts verschieben oder skalieren.
- Jedes Teil auf eigener Ebene mit klarem Namen.
- Nur schwarze Linien und schwarze Flächen, keine Farben. Fellfarben und Graustufen setzt Claude.
- Evis Körperlinien nicht mit abpausen, nur dein Teil zeichnen.
- Hängt ein Teil an Evi (zum Beispiel ein Ohr), lass es ein kleines Stück in Evi hineinreichen, damit später keine Lücke entsteht.

## Welche Vorlage wofür
Alle sind 2048 × 2048, Evi ist immer an derselben Stelle. Die Teile passen deshalb zu jeder Vorlage.

| Datei | Wofür |
|---|---|
| `erwachsen/vorlagen/Vorlage-Evi.png` | ganze Evi blass, zum Größenvergleich |
| `erwachsen/vorlagen/Vorlage-ohne-Ohren.png` | Ohren und Schwänze (Kopf und Körper blass, ohne Ohren und Schwanz) |
| `erwachsen/vorlagen/Vorlage-ohne-Kranz.png` | Halskrause weg, zum Beispiel für Schleife oder Halsband |
| `erwachsen/vorlagen/Vorlage-ohne-Gesicht.png` | Gesichter (Augen, Nase, Mund) |
| `erwachsen/vorlagen/Vorlage-Silhouette.png` | nur Kopf- und Körperform, ohne Gesicht und Kranz |

Fell-Muster und Zubehör kommen jeweils als **eigene Ebene oder eigene Datei** (Namen wie `muster-streifen`, `zubehoer-schleife`). Dann kann ich sie mit allen Ohren und Schwänzen kombinieren.

## Ordner nach Altersstufen
Jede Altersstufe hat einen eigenen Ordner mit denselben Unterordnern: `erwachsen/` (fertig angelegt, hier liegt Evi), `baby/`, `teenager/` und `senior/` (noch leer, warten auf Zeichnungen). Pfade unten gelten deshalb **je Stufe**, zum Beispiel `baby/gesichter/gesicht-gluecklich.png`. Die Dateinamen sind in allen Stufen gleich, nur der Ordner unterscheidet sie.

Reihenfolge pro neuer Stufe: 1. `vorlagen/` (Claude baut sie aus der Evi der Stufe), 2. je ein Kopf, Körper, Halsschmuck, Gesicht, Ohr und Schwanz in `frontal`, 3. weitere Gesichter, 4. weitere Ohren, Schwänze, Muster, Haltungen, 5. die anderen Ansichten. Das Werkzeug (`werkzeug/`) liegt bei `erwachsen/` und wird für eine neue Stufe dorthin kopiert, wenn die Grundebenen da sind.

## Wohin mit den fertigen Zeichnungen
Jedes Teil ist **eine PNG-Datei** (2048 × 2048, durchsichtiger Hintergrund, Figur an derselben Stelle wie in den Vorlagen). Alles liegt in `entwuerfe/evi/zeichnen/<stufe>/`.

**Ansichten:** Alle Teile haben je einen Unterordner `frontal/` (von vorn, zuerst), `seite/`, `schnueffeln/` (leichte Draufsicht, Schnüffeln am Boden) und `hinten/`.  Die Haltung (sitzend, laufen, schlafend …) steckt im Dateinamen, zum Beispiel `koerper/frontal/koerper-sitzend.png`.

| Ordner | Inhalt | Dateinamen (Beispiele) |
|---|---|---|
| `koepfe/<ansicht>/` | Kopfform | `kopf-rund.png` |
| `gesichter/<ansicht>/` | Gesichter für Stimmungen | `gesicht-neutral.png`, `gesicht-gluecklich.png`, `gesicht-traurig.png`, `gesicht-muede.png`, `gesicht-krank.png` … |
| `ohren/<ansicht>/` | Ohr-Varianten (nur linkes Ohr) | `ohr-buschel.png`, `ohr-spitz.png`, `ohr-haengend.png` |
| `halsschmuck/<ansicht>/` | Krause, Schleife, Halsband … | `krause-evi.png`, `schleife.png`, `halsband.png` |
| `koerper/<ansicht>/` | Körper je Haltung | `koerper-sitzend.png`, `koerper-laufen.png`, `koerper-schlafend.png`, `koerper-huepfen.png` |
| `schwaenze/<ansicht>/` | Schwanz-Varianten | `schwanz-buschel.png`, `schwanz-duenn.png` |
| `muster/<ansicht>/` | Fell-Muster für Gesicht und Körper | `muster-flecken-gesicht.png`, `muster-flecken-koerper.png`, `muster-streifen-koerper.png` |
| `posen/` | Textdatei je Pose: welche Teile in welcher Reihenfolge | `sitzen-frontal.txt` (Beispiel liegt in `erwachsen/`) |

Dazu in jeder Stufe: `vorlagen/` (Zeichenvorlagen), `ergebnis/` (fertige Zusammensetzungen) und bei `erwachsen/` das `werkzeug/`. Einen Ordner `basis/` gibt es nicht mehr: Evis Grundteile liegen jetzt einfach als erste Variante in den Teile-Ordnern. Die ChatGPT-Vorschläge für Gesichter liegen in `erwachsen/gesichter/frontal/vorschlaege-chatgpt/`.

**Ohren und Schwänze mit Flecken:** gibt es als eigene Datei mit Endung `-flecken` (`ohr-schlapp-flecken__vorne.png`, `schwanz-duenn-flecken.png`). Sie sind das normale Teil samt Flecken. Ohren dürfen beide Seiten enthalten (links und rechts gezeichnet, dann nicht spiegeln; vermerkt in der Pose).

**Vorn oder hinten festlegen:** Endet ein Dateiname auf `__vorne` (zwei Unterstriche), liegt das Teil beim Zusammensetzen ganz vorn, vor Kopf, Krause und allem anderen. Endet er auf `__hinten`, liegt es ganz hinten. Beispiele: `ohr-schlapp__vorne.png`, `schleife__vorne.png`, `krause-evi__hinten.png`. Ohne Endung gilt die normale Reihenfolge (Schwanz, Ohren, Körper, Kopf, Krause, Gesicht). Das gilt für alle Teile und Ansichten.

**Zusammensetzen:** `erwachsen/werkzeug/zusammensetzen.ps1` wählt die Teile über die Dateinamen, zum Beispiel `.\zusammensetzen.ps1 -Gesicht gluecklich -Hals schleife`. Ohne Angaben entsteht die normale Evi.

**Namensregeln:** nur Kleinbuchstaben, keine Leerzeichen, Umlaute als `ae`, `oe`, `ue` (müde → `muede`, hüpfen → `huepfen`), Wörter mit Bindestrich trennen. Ein Teil pro Datei, kein Wort „neu“ oder Nummer im Namen; eine überarbeitete Version ersetzt einfach die alte Datei (der Verlauf bei GitHub hebt die alte auf).
