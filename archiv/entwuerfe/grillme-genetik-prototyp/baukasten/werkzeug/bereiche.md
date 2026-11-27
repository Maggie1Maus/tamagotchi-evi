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

`BuildParts.cs` trennt den Kranz durch eine zusammenhängende helle Fläche und handgesetzte Grenzen für beide Körperformen. Die helle Schwanzspitze bekommt eine eigene zusammenhängende Maske. Gesichtslinien sind auf den tatsächlichen Gesichtsbereich begrenzt; dunkle Linien an Brust und Schwanz werden nicht als Gesicht markiert. Die farbigen Gesamtkarten im Prüfbogen enthalten Basis, eigene Ohren und eigenen Schwanz. Gelb zeigt jetzt den ganzen Kranz und die ganze helle Schwanzspitze. Die Karten bleiben **Entwürfe**; einzelne Pixel an Ohransatz und Schwanznaht brauchen weiter optische Kontrolle.

Der ursprüngliche weibliche Graustufen-Sprite hat 40 verschiedene Graustufen. Die Bedingung „Original außerhalb der entfernten Teile Pixel für Pixel erhalten“ widerspricht daher einer sofortigen Rundung der gesamten Basis auf 16 Stufen. Der Probelauf erhält dort die Originalwerte; neu gezeichnete Teile und rechnerische Varianten verwenden 16 Stufen. Für die spätere 16-Stufen-Anzeige ist eine gemeinsame Quantisierung beim Export nötig.

Evis eigener Schwanz liegt als Korrekturebene über der Basis. So wird `Basis + Evi-Ohren + Evi-Schwanz + Punkte` **pixelgenau** zum Original. Fremde Schwänze liegen hinter der Basis.
