# Prompt: Baukasten-Prototyp für gekreuzte Tamagotchis

Stand 26. November 2026. Dieser Prompt ist für eine Bild-/Code-KI (zum Beispiel ChatGPT) gedacht.

**Beschlüsse der Nutzerin (26. November 2026):**
- Kinder sehen aus wie **Verwandte von Evi**: gleicher Kopf, gleiches Gesicht, gleicher Körper. Sie unterscheiden sich durch Ohren, Schwanz, Besonderheit, Fellton, Fellmuster und Zubehör.
- Evis Familie muss sich mit **allen 20 Parkfiguren** kreuzen lassen, nicht nur mit Lotti, Glöckchen und Mia.
- Gekreuzt werden **nur Männchen mit Weibchen**.
- Es braucht eine **männliche Evi** mit allen Altersstufen, Emotionen und Posen (Bildauftrag: [`entwuerfe/evi/bildauftrag-maennlich.md`](../evi/bildauftrag-maennlich.md)). Jungen bauen auf ihr auf, Mädchen auf Evi.

**Anhängen:** `entwuerfe/evi/graustufen-vorschau/Evi-Graustufen.zip`, die männliche Evi (sobald vorhanden), und die 20 Ordner aus `entwuerfe/neue-tamagotchis/graustufen/` (oder die 20 Bilder `entwuerfe/neue-tamagotchis/<name>-gluecklich.png`).

---

## Prompt (zum Kopieren)

Du baust einen **Browser-Prototyp zum Ausprobieren** für ein Tamagotchi-Spiel. Auf einem 320 × 240-Display mit 16 Graustufen werden Kinder aus zwei Eltern zusammengesetzt. Ich möchte **eine Seite, auf der ich Eltern wähle, Kinder auswürfle, jedes Merkmal einzeln umstelle und über mehrere Generationen weiterkreuze**.

### Die Figuren
- **Evis Familie** (die Spielfigur): **Evi** (weiblich) und die **männliche Evi**. Alle Kinder sehen aus wie Verwandte von Evi: Ein Mädchen baut auf Evi auf, ein Junge auf der männlichen Evi. Gesicht, Kopf und Körper kommen immer von dieser Basis.
- **20 Parkfiguren**, jede mit eigenem Geschlecht:
  - weiblich: Lotti (Rehkitz), Ella (Elefantenbaby), Glöckchen (Hündin), Kiwi (Dino), Klecksi (Dalmatinerwelpe), Lumi (Glühwürmchen-Fee), Mia (Kätzchen), Mimi (Schnecke), Moosi (Waldgeist), Spuki (Geisterkätzchen)
  - männlich: Bit (Roboterhund), Blip (Außerirdischer), Blubber (kleiner Drache), Funke (Elektro-Eichhörnchen), Groschen (Waschbär), Leo (Löwenjunges), Linus (Hase), Mampf (Hamster), Rolli (Igel), Tetro (Schildkröte)

### Harte Regeln
1. **Nur Männchen × Weibchen.** Ein Mädchen aus Evis Familie kann nur mit einer der 10 männlichen Figuren gekreuzt werden, ein Junge nur mit einer der 10 weiblichen. Die Auswahl der Seite lässt andere Paare gar nicht erst zu.
2. **Das Kind bekommt zufällig ein Geschlecht** und damit die passende Basis (Mädchen → Evi, Junge → männliche Evi).
3. **Zeichne nichts neu und interpretiere nichts.** Die angehängten Bilder sind meine Zeichnungen. Sie werden nur verwendet, nicht nachgebaut, nicht „verbessert“. Wo ein Bauteil noch fehlt, zeigst du einen deutlich beschrifteten Platzhalter (z. B. „PLATZHALTER Ohren Leo, Erwachsen, Junge“), nie ein erfundenes Bild.
4. **Ich liefere die Bauteile später selbst.** Die Seite muss mit meinen PNGs funktionieren, sobald ich sie in die richtigen Ordner lege, ohne Codeänderung.
5. Nur **Graustufen** (16 Stufen: 0, 17, 34 … 255), Pixelgrafik, `image-rendering: pixelated`, nur ganzzahlige Vergrößerung.
6. **Kein Server, keine Installation, kein Internet.** Es muss per Doppelklick auf `index.html` laufen (auch `file://`). Weil Browser dort das Pixel-Lesen von Bildern sperren können, lege alle Bilder zusätzlich als Base64 in `teile-daten.js` ab. Liefere ein PowerShell-Skript mit, das diese Datei aus den PNG-Ordnern neu erzeugt.
7. Reines HTML/CSS/JavaScript ohne Bibliotheken. Deutsche Oberfläche, einfache Sprache.

### Merkmale (was ein Kind erbt)
| Merkmal | Herkunft | Hinweis |
|---|---|---|
| Ohren | von einem Elternteil | jede Figur hat eine Ohrform, gezeichnet für Evis Kopf |
| Schwanz | von einem Elternteil | gezeichnet für Evis Körper |
| Besonderheit | von einem Elternteil oder keine | z. B. Flügel (Blubber, Lumi), Hörnchen (Blubber), Mähne (Leo), Stacheln (Rolli), Panzer (Tetro), Antennen (Blip, Bit), Fühler (Mimi), Blätter (Moosi), Rüssel-Andeutung (Ella). Nur ein Platz, damit es übersichtlich bleibt |
| Fellton | zwischen beiden Eltern ± 1 Stufe | fünf Stufen von sehr hell bis sehr dunkel, gerechnet |
| Fellmuster | von einem Elternteil | keins, Flecken (Klecksi), Streifen/Ringe (Groschen), helle Brust, Punkte (Kiwi) … als Maske |
| Zubehör | von einem Elternteil oder keins | z. B. Schleife (Mia), Glöckchen (Glöckchen) |

Figuren ohne passendes Teil (z. B. Mimi und Spuki ohne Ohren, Blubber ohne Fell) haben dort den Eintrag **„keins“**. Dann kommt dieses Merkmal automatisch vom anderen Elternteil. Evis Familie gibt ihre eigenen Ohren, ihren eigenen Schwanz und ihre Werte genauso weiter.

Lege alle Werte aller 22 Figuren (Evi, männliche Evi, 20 Parkfiguren) in einer gut lesbaren Tabelle in `figuren.js` ab: Name, Geschlecht, Tierart, Ohren, Schwanz, Besonderheit, Fellton, Muster, Zubehör. Fülle sie anhand der angehängten Bilder aus und **markiere, was du geschätzt hast**. Ich will die Tabelle selbst ändern können.

### Aufbau der Bauteile (deckungsgleiche Ebenen)
Alle Ebenen einer Altersstufe und eines Geschlechts haben **exakt dieselbe Bildgröße** und liegen **deckungsgleich übereinander**; außerhalb des Teils sind sie transparent. Dadurch braucht es keine Ankerpunkte, und alles passt per Konstruktion. Die Größe ist die der jeweiligen Basis (Evi: Baby 94 × 89, Teenager 81 × 107, Erwachsen 114 × 116, Senior 115 × 110; männliche Evi: Größe ihrer Bilder).

Zeichenreihenfolge von hinten nach vorn: **Schwanz → Besonderheit hinten (Flügel, Panzer) → Basis (Körper, Kopf, Gesicht) → Fellmuster → Ohren → Besonderheit vorn (Mähne, Antennen, Hörnchen) → Zubehör**.

```
teile/<geschlecht>/<alter>/basis.png                    weiblich = Evi, maennlich = männliche Evi; ohne Ohren und Schwanz
teile/<geschlecht>/<alter>/ohren/<figur>.png            beide Ohren dieser Figur, auf der Basis sitzend
teile/<geschlecht>/<alter>/schwanz/<figur>.png
teile/<geschlecht>/<alter>/besonderheit/<figur>-hinten.png und <figur>-vorn.png
teile/<geschlecht>/<alter>/muster/<muster>.png          weiß = Muster, schwarz = kein Muster
teile/<geschlecht>/<alter>/zubehoer/<zubehoer>.png
```
`<geschlecht>`: `weiblich`, `maennlich`. `<alter>`: `baby`, `teenager`, `erwachsen`, `senior`. `<figur>`: `evi`, `evi-m`, `lotti`, `ella`, `gloeckchen`, `kiwi`, `klecksi`, `lumi`, `mia`, `mimi`, `moosi`, `spuki`, `bit`, `blip`, `blubber`, `funke`, `groschen`, `leo`, `linus`, `mampf`, `rolli`, `tetro`.

Solange `basis.png` fehlt, nimm das angehängte Originalbild unverändert und zeige den Hinweis „Original-Ohren und -Schwanz noch in der Basis“. Fehlt die männliche Evi ganz, zeigst du für Jungen einen großen Platzhalter „männliche Evi fehlt noch“, nimmst aber **nicht** Evi als Ersatz. Fehlende Teile erscheinen als beschriftete Platzhalter und werden in einer **Liste „Noch zu zeichnen“** gesammelt (Geschlecht, Alter, Merkmal, Figur), die ich kopieren kann.

### Fellton und Muster (gerechnet, nicht gezeichnet)
- **Fellton** verschiebt die Graustufen von Basis, Ohren, Schwanz und Besonderheit einheitlich (Lookup-Tabelle, Ergebnis auf 16 Stufen gerundet). Die **Umrisslinie (die dunkelsten Stufen) und die Augen bleiben unverändert**, ebenso das Weiß der Halskrause.
- **Muster** wird nur **innerhalb der Körperfläche** (Alphakanal der Basis) eingeblendet und verändert dort die Graustufen um 2–3 Stufen. Linien bleiben unberührt.

### Die Seite
1. **Elternwahl**: links „Evis Familie“ (Evi, männliche Evi oder ein gespeichertes Kind), rechts **nur die Figuren des anderen Geschlechts** als Raster mit kleinem Bild und Namen. Wechselt links das Geschlecht, wechselt rechts die Auswahl mit.
2. **Anzeige**: Elternteil A – Kind – Elternteil B nebeneinander, darunter der **Altersstrahl** Baby → Teenager → Erwachsen → Senior des Kindes. Zoom 1× bis 4×.
3. **„Neue Kreuzung“**: Geschlecht des Kindes zufällig. Für Ohren, Schwanz, Besonderheit, Muster und Zubehör zufällig Elternteil A oder B (bei „keins“ der andere). Fellton zufällig zwischen den Elterntönen ± 1. Das Kind soll sich **nach Möglichkeit von beiden Eltern unterscheiden**: Gleicht es in allen Merkmalen einem Elternteil, wird neu gewürfelt (höchstens 20 Versuche; sonst sagt die Seite es offen). Keine Mutationen, keine versteckten Gene: Ein Kind kann nur haben, was ein Elternteil sichtbar hat.
4. **Von Hand umstellen**: Pro Merkmal ein Auswahlfeld („von A“, „von B“ oder direkt jede Figur zum Testen). Jedes Merkmal lässt sich **sperren**, sodass „Neue Kreuzung“ es nicht ändert. Auch das Geschlecht des Kindes lässt sich umstellen.
5. **Generationen**: Knopf „Kind als neuen Elternteil nehmen“. Das Kind wandert nach links (Evis Familie) und wird mit seinen geerbten Merkmalen weitergegeben, z. B. Leos Ohren an die nächste Generation. Rechts stehen wieder die Figuren des anderen Geschlechts. Ein kleiner **Stammbaum** zeigt die bisherigen Generationen als Bildreihe.
6. **Alle Kombinationen zeigen**: Knopf, der für den linken Elternteil ein Raster mit **einem ausgewürfelten Kind pro möglichem Partner** zeigt (also 10 Kinder). So sehe ich schnell, welche Teile noch nicht gut passen.
7. **Prüfhilfen**: Ebenen einzeln ein-/ausblenden, „Nähte zeigen“ (Ränder jedes Teils farbig), Pixelraster, Hintergrund (weiß, grau, dunkel), „Mit Original vergleichen“.
8. **Speichern**: Das Kind ist eine kurze Liste von Kennungen, z. B. `geschlecht=maennlich, ohren=leo, schwanz=evi, besonderheit=keins, ton=3, muster=flecken, zubehoer=keins`. Als Text anzeigen, mit „Kopieren“ und „Einfügen“ zum Wiederherstellen. Genau diese Liste soll später im Spiel gespeichert werden.
9. **Schablonen-Export**: pro Geschlecht und Altersstufe ein Knopf, der ein PNG in Originalgröße erzeugt: Basis **blass** (etwa 25 % Deckkraft) auf weißem Grund mit Pixelraster. Darauf werden neue Ohren, Schwänze usw. gezeichnet; danach passt alles deckungsgleich.

### Fehler, die ich nicht will
- Kreuzungen zwischen zwei Weibchen oder zwei Männchen.
- Evi als stiller Ersatz für die männliche Evi.
- Doppelte Umrisse oder Lücken an den Ansätzen: im Prototyp sichtbar machen statt verstecken.
- Weichgezeichnete oder verschobene Pixel.
- Neu gezeichnete oder „geglättete“ Figuren.
- Abhängigkeit von Internet, Build-Werkzeug oder Server.

### Ergebnis
Liefere `index.html`, `figuren.js`, `teile-daten.js` samt PowerShell-Skript zum Neuerzeugen, die leere Ordnerstruktur `teile/` und eine **kurze Anleitung in einfachem Deutsch**: wie ich Bauteile hineinlege, wie die Seite aktualisiert wird, was noch Platzhalter ist. Sage am Ende ehrlich, was du **nicht** geprüft hast.
