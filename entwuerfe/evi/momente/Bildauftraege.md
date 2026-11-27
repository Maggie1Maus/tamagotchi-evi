# Bildaufträge: Was Evi von selbst tut (erwachsene Evi)

Stand: 23. November 2026

**Erledigt (24. November 2026): auch Baby, Teenager und Oma.** Je 8 Zeichnungen (`evi-baby-*.png`, `evi-teenager-*.png`, `evi-oma-*.png`) sind seit Version 0.12 in der Handy-App und auf dem Waveshare eingebaut; Evi tut jetzt in jeder Lebensphase außer dem Ei selbst etwas. `schmetterling.png` und `vogel.png` sind seit 24. November 2026 ebenfalls eingebaut (App und Waveshare; der Vogel auf drei Viertel verkleinert, siehe `handy-app/tools/prepare_besuch.py`).

**Erledigt (23. November 2026):** Alle 8 Zeichnungen sind da und seit Version 0.10.1 im Spiel eingebaut (`handy-app/dist/img/adult-<name>.png`). Bei „Drehen“ hat der Umriss am rechten Ohransatz eine winzige Lücke (1 Pixel). Deshalb erkennt das Werkzeug seitdem „außen“ mit 1 Pixel abgedichtetem Umriss. Die Zeichnung selbst wurde nicht verändert.

Seit Version 0.10 tut die erwachsene Evi ab und zu etwas von selbst: Sie läuft herum, schaut aus dem Fenster, denkt an etwas, summt, gähnt, dreht sich im Kreis, begrüßt dich und bekommt Besuch von einem Schmetterling. Die Nutzerin wünscht sich dafür **eigene Zeichnungen**. Bis eine Zeichnung da ist, zeigt das Spiel die vorhandenen Emotionsbilder. Erst einmal nur für die **erwachsene Evi**.

## So kommen die Zeichnungen ins Spiel

1. Bild mit dem Auftrag unten erzeugen. Als Vorlage dient `../emotionen/erwachsen/evi-erwachsen-gluecklich.png`.
2. In diesem Ordner speichern, **genau** unter dem angegebenen Namen, zum Beispiel `evi-erwachsen-laufen.png`.
3. `archiv/handy-app/tools/build-art.ps1` ausführen (oder Claude darum bitten). Das Skript schneidet zu, entfernt die Beschriftung und macht den Rand durchsichtig, wie bei den Emotionen. Das Bild selbst bleibt unverändert.
4. Fertig: Jede vorhandene Zeichnung wird sofort benutzt. Es müssen nicht alle auf einmal da sein.

Gut zu wissen:
- **Laufen** wird gespiegelt, wenn Evi nach rechts läuft. Deshalb bitte nach **links** laufend zeichnen, den Schwanz rechts.
- Gedankenwolke, Noten und Schmetterling zeichnet das Spiel selbst dazu. Sie sollen **nicht** im Bild sein.
- Wie groß Evi im Bild ist, bestimmt, wie groß sie im Spiel ist. Darum sollte sie im neuen Bild ungefähr so groß sein wie in der Vorlage.

## Reihenfolge (Vorschlag)

| Nr. | Datei | Wofür im Spiel | Wird gebraucht bei |
| --- | --- | --- | --- |
| 1 | `evi-erwachsen-laufen.png` | Evi läuft | Spaziergang, zum Fenster, hinter dem Schmetterling her (am häufigsten) |
| 2 | `evi-erwachsen-huepfen.png` | Evi hüpft vor Freude | Begrüßung, Schmetterling, Ende des Spaziergangs |
| 3 | `evi-erwachsen-gaehnen.png` | Evi gähnt | Gähnen (bisher mit dem Bild „müde“) |
| 4 | `evi-erwachsen-staunen.png` | Evi staunt nach oben | Schmetterling kreist und landet auf ihrem Kopf |
| 5 | `evi-erwachsen-fenster.png` | Evi von hinten | schaut aus dem Fenster |
| 6 | `evi-erwachsen-denken.png` | Evi träumt vor sich hin | Gedankenwolke (Keks, Ball, Herz, Lied, Essen, Baden) |
| 7 | `evi-erwachsen-singen.png` | Evi summt | Summen mit Noten |
| 8 | `evi-erwachsen-drehen.png` | Evi jagt ihren Schwanz | dreht sich im Kreis |

## Die Bildaufträge

Jeder Auftrag besteht aus dem **gemeinsamen Anfang** und dem Teil für die jeweilige Haltung. Beides zusammen in die Bildgenerierung geben, mit `evi-erwachsen-gluecklich.png` als Vorlage.

### Gemeinsamer Anfang

```
Use case: precise-object-edit / character-pose concept. Input image: the exact adult EVI reference, identity and style reference. Create ONE full-body pose variation of this SAME Evi for a matching set of black-and-white virtual-pet concept drawings. Closely preserve her proportions, exact adult identity, size on the canvas and distinctive features: huge wide pointed ears with black interiors, three forehead tufts, rounded face, shiny black eyes with square white catchlights when open, tiny black nose, white fluffy neck ruff with rounded hanging tufts, four little paws, bushy tail with white tip. Keep sparse evenly spaced square fur dots only on fur, with clean readable white face. Preserve the crisp low-resolution stepped PIXEL-ART outlines of the reference, solid BLACK AND WHITE ONLY. White backdrop, no gray, no color, no gradients, no realistic texture, no smooth vector lines. Same scale as the reference, centered with generous white margins. No scene, no floor, no ground shadow, no props, no UI, no watermarks, no clothing, no human anatomy. One discreet black pixel-font caption directly below the feet; no other text.
```

### 1. Laufen – `evi-erwachsen-laufen.png`

```
Walking happily toward the viewer's LEFT in a three-quarter side view, mid-step: one front paw lifted forward, the opposite hind paw pushing off. Head turned slightly toward the viewer with open shiny eyes and a small happy smile, ears perked, bushy tail trailing behind her on the viewer's RIGHT, slightly raised. Cheerful, light, bouncy step. Caption text verbatim: "LAUFEN".
```

### 2. Hüpfen – `evi-erwachsen-huepfen.png`

```
Jumping for joy: the whole body in mid-air, all four paws off the ground and spread out, ears up high, eyes closed in joyful upward-curving crescent arcs, wide open laughing smile with a tiny white tongue, tail flung up on the viewer's right. Front view. No ground, no shadow, no motion lines, no hearts. Caption text verbatim: "HÜPFEN".
```

### 3. Gähnen – `evi-erwachsen-gaehnen.png`

```
A big yawn: seated front view, eyes squeezed shut, mouth stretched wide open in a big round yawn showing a tiny white tongue, one front paw raised near the mouth, ears relaxed and slightly back, tail resting on the viewer's right. Sleepy but cute, not sad. No letters, no "Z" symbols. Caption text verbatim: "GÄHNEN".
```

### 4. Staunen – `evi-erwachsen-staunen.png`

```
Amazed and delighted, looking straight up at something tiny just above her head: seated front view, head tilted back a little, big shiny eyes looking upward, small round "o" mouth, ears perked forward, one front paw slightly lifted, tail curled up on the viewer's right. Do NOT draw what she looks at; leave the space above her head empty. Caption text verbatim: "STAUNEN".
```

### 5. Fenster – `evi-erwachsen-fenster.png`

```
Seen from BEHIND: Evi sits with her back to the viewer, looking up and away as if gazing out of a window. Show the back of her head with both huge ears from behind (no face visible), the back of the fluffy white ruff peeking out at the sides, the dotted back and her bushy tail with white tip curled on the floor to the viewer's right. Calm, curious posture. No window drawn. Caption text verbatim: "FENSTER".
```

### 6. Denken – `evi-erwachsen-denken.png`

```
Daydreaming: seated front view, head tilted slightly, eyes looking up toward the viewer's upper RIGHT, one front paw raised to her chin, a small closed dreamy smile, ears relaxed, tail on the viewer's right. Leave empty space at the upper right; do NOT draw a thought bubble. Caption text verbatim: "DENKEN".
```

### 7. Singen – `evi-erwachsen-singen.png`

```
Humming a little song: seated front view, body swaying slightly to one side, eyes closed in content arcs, small round open singing mouth, ears relaxed, tail swaying on the viewer's right. Happy and peaceful. Do NOT draw music notes. Caption text verbatim: "SINGEN".
```

### 8. Drehen – `evi-erwachsen-drehen.png`

```
Playfully chasing her own tail: body turned in a three-quarter view, head looking back over her shoulder toward her bushy tail on the viewer's RIGHT, eager shiny eyes, open playful smile, one front paw lifted. Fun and silly. Caption text verbatim: "DREHEN".
```
