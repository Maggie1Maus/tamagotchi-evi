# Evis Familien-Baukasten

`index.html` per Doppelklick öffnen. Die Seite läuft ohne Internet und ohne Server. Sie bietet Elternwahl, Kind auswürfeln, Merkmale ändern und sperren, mehrere Generationen, zehn Kombinationen auf einen Blick, Altersstufen, Ansichtshilfen, Merkmalliste und Schablonen-Export.

Die Originalbilder von Evi und allen 20 Parkfiguren sowie die vier glücklichen Sprites der männlichen Evi sind in `teile-daten.js` eingebettet. **Die Zeichnungen werden im Browser nicht nachgebaut.** Noch nicht gelieferte Einzelteile erscheinen in der Liste „Noch zu zeichnen“. Für Erwachsene gibt es jetzt einen Probelauf mit Evi-Basen sowie neu gezeichneten Ohren und Schwänzen von Leo, Linus und Groschen. Andere Altersstufen zeigen vorerst das ganze Original und kennzeichnen fehlende Bauteile.

## Eigene Bauteile einlegen

Jede PNG-Ebene einer Altersstufe braucht exakt dieselbe Größe wie `basis.png` und einen transparenten Hintergrund außerhalb des Teils. Die Ordner stehen unter `teile/<weiblich|maennlich>/<baby|teenager|erwachsen|senior>/`:

- `basis.png` — Körper und Gesicht ohne Ohren und Schwanz
- `ohren/<figur>.png`, `schwanz/<figur>.png`
- `besonderheit/<figur>-hinten.png` und `<figur>-vorn.png`
- `struktur/<struktur>.png`, `muster/<muster>.png`, `zubehoer/<zubehoer>.png`
- `<name>.bereiche.png` neben jedem Teil — Bereichskarte für Fell- und Kranzton; Werte in [werkzeug/bereiche.md](werkzeug/bereiche.md)

Nach dem Einlegen `teile-aktualisieren.ps1` per Rechtsklick → „Mit PowerShell ausführen“ starten und `index.html` neu öffnen. Das Skript packt die PNGs als Base64 in `teile-daten.js`, damit auch `file://` funktioniert. Die Schablone enthält die aktuelle Basis blass auf weißem Grund mit Pixelraster. Die Tabelle mit den 22 Ausgangsfiguren steht in `figuren.js`; geschätzte Merkmale sind dort markiert.

## Probelauf für Erwachsene

Der [Prüfbogen](werkzeug/pruefbogen-erwachsen.png) zeigt beide Evis mit eigenen und fremden Teilen, hellem und dunklem Hintergrund, Fell- und Kranztöne für beide Geschlechter, Fellstrukturen auch in Originalgröße, runde Körpermuster, Zubehör, farbige Gesamtkarten und drei Beispielkinder. Die Parkvorlagen stehen unten daneben. `werkzeug/pixelvergleich.txt` prüft die Rückmontage von Evis eigenen Teilen: bei beiden Geschlechtern 0 abweichende Pixel zum Original.

Zum Wiederholen: `werkzeug/bauteile-bauen.ps1`, danach `werkzeug/pruefbogen-bauen.ps1` und `teile-aktualisieren.ps1` ausführen. Die ersten beiden Skripte nutzen .NET/System.Drawing; ihre Bild-KI-Quellen und die 8-fach vergrößerten Vorlagen liegen unter `werkzeug/`. Alle Teile bleiben Entwürfe bis zur optischen Freigabe.

Korrekturrunde vom 27. November: Leos Mähne schließt jetzt unter dem Gesicht; die weißen Querbalken wurden entfernt. Farbflächen für Kranz und helle Schwanzspitze folgen den Konturen. Die beiden ursprünglichen Kartenfehler („Gesicht“ an der Schwanzspitze und als Brustband) sind korrigiert. Flecken, Ringe, helle Brust und „Maske“ liegen nur auf dem Körperfell, nicht auf Gesicht oder Kranz; die „Maske“ ist deshalb eine seitliche Körperzeichnung. Groschens helle schräge Bauchstreifen sind entfernt. Schleife und Glöckchen sind größer und neu positioniert. Die gezeichneten KI-Vorlagen (`werkzeug/ki/*-v2.png`) dienen als Vorlage; das Skript bereinigt die pixelgenauen Ebenen und zeichnet das kleine Zubehör im Raster nach.

Sichtbare Restpunkte: Leos Mähne wirkt beim Jungen an den Wangen und unter dem Kinn noch wie ein zusätzlicher grauer Saum. Groschens Schwanzansatz rechts ist weiterhin etwas unruhig. Die Körpermuster sind in Originalgröße aufgrund der kleinen freien Fellfläche nur schwach erkennbar; auch Evis ursprüngliche Fellpunkte sind dort sehr fein. Der ursprüngliche weibliche Sprite enthält 40 Graustufen. Um ihn Pixel für Pixel zu bewahren, werden Originalpixel nicht gerundet; neu gezeichnete Teile und Varianten verwenden 16 Stufen. Der Probelauf ist weiterhin **nicht freigegeben**.

## Stand und Grenzen

Die männliche Evi hat 80 zugeschnittene Graustufen-Einzelbilder in `../../evi/maennlich/Sprites/`. Die Bildbögen und das reproduzierbare Zuschnittskript liegen daneben. In den Fenster- und Gartenposen ist nur die Figur; der beleidigte Opa hat vier Pfoten. Die 80 Bilder sind Bildentwürfe, noch keine optisch freigegebenen Spielgrafiken.

Für die übrigen Figuren und Altersstufen fehlen weiter deckungsgleiche Bauteile. Die Seite zeigt das offen an. Der Probelauf und seine Farbvarianten sind nur Vorschauen; ein physischer Test am Waveshare ist nicht erfolgt. Ein interaktiver Browsertest von `file://` war wegen der Browser-Sicherheitsrichtlinie nicht möglich.
