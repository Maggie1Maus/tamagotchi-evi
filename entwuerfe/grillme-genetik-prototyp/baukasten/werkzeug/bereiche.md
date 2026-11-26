# Bereichskarten für den Probelauf

Jede Datei `*.bereiche.png` hat dieselbe Größe wie ihr Teil. Transparent bedeutet „kein Teil“. Die übrigen Pixel haben nur diese festen Graustufen:

| Wert | Bereich | Prüfbogenfarbe | Wirkung im Baukasten |
|---:|---|---|---|
| 17 | Linie | schwarz | bleibt gleich |
| 34 | Gesicht und Gesichtslinien | rot | bleibt gleich |
| 51 | Fell | blau | Fellton 0–4 |
| 68 | Halskrause und helle Schwanzspitze | gelb | Kranzton 0–3 |
| 85 | Ohrinneres | violett | folgt Fellton, bleibt dunkel |
| 102 | helle Pfoten | grün | bleibt gleich |
| 119 | Fellstruktur | orange | folgt Fellton |

`BuildParts.cs` ordnet die Bereiche anhand von Grauwert und Position zu. Die Karten sind **Entwürfe**. Im Prüfbogen sind Basis-Karten farbig zu sehen; einzelne Pixel an Wangen, Ohransätzen, Schwanzspitzen und zwischen Kranz und Körper sollten vor Freigabe von Hand kontrolliert werden.

Der ursprüngliche weibliche Graustufen-Sprite hat 40 verschiedene Graustufen. Die Bedingung „Original außerhalb der entfernten Teile Pixel für Pixel erhalten“ widerspricht daher einer sofortigen Rundung der gesamten Basis auf 16 Stufen. Der Probelauf erhält dort die Originalwerte; neu gezeichnete Teile und rechnerische Varianten verwenden 16 Stufen. Für die spätere 16-Stufen-Anzeige ist eine gemeinsame Quantisierung beim Export nötig.

Die eigenen Evi-Schwänze werden beim Zusammensetzen vor den neu gezeichneten Schwänzen behandelt: Evis eigener Schwanz liegt als Korrekturebene vor der Basis. So wird `Basis + Evi-Ohren + Evi-Schwanz + Punkte` **pixelgenau** zum Original. Bei fremden Schwänzen liegt der Schwanz hinter der Basis.
