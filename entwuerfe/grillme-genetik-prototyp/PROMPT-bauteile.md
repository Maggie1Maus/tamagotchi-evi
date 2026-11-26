# Prompt: Bauteile für den Familien-Baukasten

Stand 26. November 2026. Der [Baukasten](baukasten/index.html) läuft, zeigt aber noch keine echten Kreuzungen. Es fehlen die **Bauteile**: Evi und die männliche Evi ohne Ohren und Schwanz (Basis) sowie die Ohren, Schwänze und Besonderheiten der 20 Parkfiguren, **im Evi-Stil und passend auf Evis Kopf gezeichnet**.

Wichtig: Die Teile werden **nicht aus den Parkfiguren ausgeschnitten**. Das wurde schon versucht und passte nicht (anderer Stil, andere Größe, andere Ansätze). Stattdessen zeichnet die Bild-KI auf Evis Basis neu: „Leos Ohren, wie sie an Evi aussehen würden“.

**Erst ein kleiner Probelauf**, dann der Rest. Für alle 20 Figuren, 4 Altersstufen und 2 Geschlechter wären es über 300 Teile. Lohnt sich das nur, wenn der Probelauf gut aussieht.

---

## Prompt (zum Kopieren)

Du arbeitest im Ordner `entwuerfe/grillme-genetik-prototyp/baukasten/` des Tamagotchi-Projekts. Lies zuerst `README.md`, `figuren.js` und `app.js` dort, und `../PROMPT-baukasten.md`. Die Seite ist fertig. Deine Aufgabe sind jetzt die **Bauteile** (PNG-Ebenen), damit echte Kreuzungen sichtbar werden.

### Ziel dieses Durchgangs: Probelauf
Nur Altersstufe **Erwachsen**, beide Geschlechter, drei Parkfiguren mit sehr verschiedenen Ohren und Schwänzen: **Leo** (Löwe), **Linus** (Hase), **Groschen** (Waschbär). Mehr nicht. Danach hörst du auf und zeigst mir das Ergebnis.

Zu liefern:
```
teile/weiblich/erwachsen/basis.png
teile/maennlich/erwachsen/basis.png
teile/<weiblich|maennlich>/erwachsen/ohren/evi.png        (bzw. evi-m.png: Evis eigene Ohren als Ebene)
teile/<weiblich|maennlich>/erwachsen/schwanz/evi.png      (bzw. evi-m.png)
teile/<weiblich|maennlich>/erwachsen/ohren/leo.png, linus.png, groschen.png
teile/<weiblich|maennlich>/erwachsen/schwanz/leo.png, linus.png, groschen.png
teile/<weiblich|maennlich>/erwachsen/besonderheit/leo-vorn.png   (Mähne)
```

### Harte Regeln
1. **Evis Zeichnung bleibt Pixel für Pixel erhalten**, außer genau dort, wo Ohren und Schwanz entfernt werden. Nichts glätten, nichts „verbessern“, keine neue Evi.
2. Die neuen Teile werden **im Stil von Evi** gezeichnet: gleiche dunkelgraue Umrisslinie (gleiche Stärke, gestufte Pixelkanten), gleiche 16 Graustufen, gleiche weichen grauen Fellpunkte, gleiche Art von dunklem Ohrinneren. Sie sollen aber **eindeutig die Form der Parkfigur** haben: Leo runde Löwenohren und Schwanz mit Quaste, Linus lange Hasenohren und Puschelschwanz, Groschen Waschbärohren mit hellem Rand und geringelter Schwanz.
3. Jede Ebene hat **exakt die Größe der Basis** und ist außerhalb des Teils transparent. Nur Graustufen (0, 17, 34 … 255), keine Halbtransparenz (Alpha nur 0 oder 255).
4. Alle Bilder von dir sind **Entwürfe**. Ich gebe sie frei, nicht du.

### Arbeitsweise (damit nichts verrutscht)
Bild-KIs halten kein Pixelraster ein. Deshalb arbeitest du in diesen Schritten und schreibst dafür ein wiederholbares Skript (PowerShell oder Python) in `baukasten/werkzeug/`:

**Schritt A – Vergrößern:** Das Original-Sprite (`Evi-Graustufen.zip` → `Sprites/04-Erwachsen/gluecklich.png`, für den Jungen `../../evi/maennlich/Sprites/04-Erwachsen/gluecklich.png`) mit Nearest Neighbour **8-fach** vergrößern, auf weißem Grund. Dieses Bild bekommt die Bild-KI.

**Schritt B – Bild-KI bearbeitet nur einen Bereich** (Bearbeiten/Inpainting, nicht neu erzeugen). Englische Aufträge siehe unten.

**Schritt C – Zurück ins Raster:** Ergebnis auf das 8-fache Raster ausrichten (gleiche Position wie Schritt A, notfalls über die Fußlinie und die Augen), je 8 × 8 Block auf einen Pixel zurückrechnen (Mehrheitswert/Median), auf 16 Graustufen runden, Weiß außen → transparent.

**Schritt D – Schutzmaske:** Alle Pixel **außerhalb** des erlaubten Bereichs werden hart auf das Original zurückgesetzt. Erlaubter Bereich: für Ohren der obere Kopfbereich (Ohren plus ein paar Pixel Kopfrand), für den Schwanz die rechte Seite hinter dem Körper. So kann die Bild-KI am Gesicht nichts verändern.

**Schritt E – Ebene herauslösen:** Teil = alle Pixel, die sich vom Basisbild unterscheiden, innerhalb des erlaubten Bereichs. Der Rest ist transparent.

**Schritt F – Prüfbogen:** Erzeuge `baukasten/werkzeug/pruefbogen-erwachsen.png`: pro Geschlecht eine Zeile mit Basis allein, Basis + Evi-Teile (muss aussehen wie das Original), Basis + Leo-Teile, + Linus-Teile, + Groschen-Teile, jeweils 3-fach vergrößert, auf weiß und auf dunkelgrau. Daneben klein die Originalfigur aus dem Park zum Vergleich.

Danach `teile-aktualisieren.ps1` ausführen und prüfen, dass der Baukasten die Teile zeigt (Evi × Leo, Evi × Linus, Evi × Groschen).

### Bildaufträge für Schritt B

**B1 – Basis ohne Ohren und Schwanz** (einmal je Geschlecht):
```text
Edit the attached pixel-art sprite. It is my own approved drawing; keep every pixel identical except the areas described.
Remove BOTH EARS completely and close the top of the head with a smooth, rounded head outline in exactly the same dark-gray outline thickness and the same light-gray fur with soft gray fur dots as the rest of the head. Keep the small forehead tuft.
Remove the TAIL completely; where it overlapped the body, continue the body outline and fur naturally.
Do not change the face, eyes, mouth, neck ruff, body, paws or pose. No new details, no color, white background, same canvas size and position.
```

**B2 – Ohren einer Parkfigur** (je Figur und Geschlecht; Basis aus B1 anhängen und dazu das Bild der Parkfigur als Formvorlage):
```text
Edit the first attached image (a pixel-art character without ears). Add ears ONLY — nothing else may change.
The ears must have the SHAPE of the ears of the second attached character ([LEO: round lion ears / LINUS: long upright rabbit ears / GROSCHEN: raccoon ears with light rims]), but be drawn in the EXACT STYLE of the first image: same dark-gray stepped pixel outline thickness, same light-gray fur with soft gray dots, dark-gray inner ear like the original character, 16 gray levels only.
Place them where the original ears were attached on the head, symmetrical, in proportion to the head. Same canvas size and position, white background, no color, no text.
```

**B3 – Schwanz einer Parkfigur** (wie B2):
```text
Edit the first attached image (a pixel-art character without a tail). Add a tail ONLY, behind the body on the viewer's right — nothing else may change.
The tail must have the SHAPE of the tail of the second attached character ([LEO: thin lion tail with a dark tuft / LINUS: small round fluffy rabbit tail / GROSCHEN: bushy raccoon tail with dark rings]), drawn in the EXACT STYLE of the first image: same outline thickness, same fur dots, 16 gray levels only.
Same canvas size and position, white background, no color, no text.
```

**B4 – Leos Mähne** (Besonderheit vorn, gleiche Art): eine Löwenmähne, die **um das Gesicht** liegt und es nicht verdeckt.

### Was du am Ende sagst
- Welche Dateien entstanden sind und den Prüfbogen.
- Wo Nähte, doppelte Linien, Lücken oder Stilbrüche zu sehen sind (ehrlich, mit Figur und Stelle).
- Was du nicht geprüft hast.
- **Keine weiteren Figuren oder Altersstufen**, bevor ich den Probelauf freigebe.

---

## Danach (wenn der Probelauf gut ist)
1. Die übrigen 17 Figuren für Erwachsen, beide Geschlechter. Für Mimi und Spuki gibt es keine Ohren und Schwänze, nur ihre Besonderheit (Fühler, Geisterschweif?).
2. Dieselben Teile für Baby, Teenager und Senior. Die Teile werden je Alter neu auf die jeweilige Basis gezeichnet, nicht nur verkleinert.
3. Muster (Flecken, Ringe, helle Brust, Punkte) als weiß/schwarze Masken je Basis und Zubehör (Schleife, Glöckchen).
4. Später: Emotionen und Posen. Dafür bräuchten die Teile je Pose angepasste Fassungen. Erst entscheiden, wenn die Grundpose überzeugt.
