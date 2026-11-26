# Evis Familien-Baukasten

`index.html` per Doppelklick öffnen. Die Seite läuft ohne Internet und ohne Server. Sie bietet Elternwahl, Kind auswürfeln, Merkmale ändern und sperren, mehrere Generationen, zehn Kombinationen auf einen Blick, Altersstufen, Ansichtshilfen, Merkmalliste und Schablonen-Export.

Die Originalbilder von Evi und allen 20 Parkfiguren sowie die vier freigegebenen glücklichen Sprites der männlichen Evi sind in `teile-daten.js` eingebettet. **Die Zeichnungen werden im Browser nicht nachgebaut.** Noch nicht gelieferte Einzelteile erscheinen in der Liste „Noch zu zeichnen“. Bis `basis.png` ohne Ohren und Schwanz vorliegt, zeigt die Seite das ganze Original und kennzeichnet, dass sich daran keine anderen Ohren oder Schwänze sauber montieren lassen.

## Eigene Bauteile einlegen

Jede PNG-Ebene einer Altersstufe braucht exakt dieselbe Größe wie `basis.png` und einen transparenten Hintergrund außerhalb des Teils. Die Ordner stehen unter `teile/<weiblich|maennlich>/<baby|teenager|erwachsen|senior>/`:

- `basis.png` — Körper und Gesicht ohne Ohren und Schwanz
- `ohren/<figur>.png`, `schwanz/<figur>.png`
- `besonderheit/<figur>-hinten.png` und `<figur>-vorn.png`
- `muster/<muster>.png`, `zubehoer/<zubehoer>.png`

Nach dem Einlegen `teile-aktualisieren.ps1` per Rechtsklick → „Mit PowerShell ausführen“ starten und `index.html` neu öffnen. Das Skript packt die PNGs als Base64 in `teile-daten.js`, damit auch `file://` funktioniert. Die Schablone enthält die aktuelle Basis blass auf weißem Grund mit Pixelraster. Die Tabelle mit den 22 Ausgangsfiguren steht in `figuren.js`; geschätzte Merkmale sind dort markiert.

## Stand und Grenzen

Die männliche Evi hat 80 zugeschnittene Graustufen-Einzelbilder in `../../evi/maennlich/Sprites/`. Die Bildbögen und das reproduzierbare Zuschnittskript liegen daneben. In den Fenster- und Gartenposen ist nur die Figur; der beleidigte Opa hat vier Pfoten. Die 80 Bilder sind Bildentwürfe, noch keine optisch freigegebenen Spielgrafiken.

Für echte Kreuzungsbilder fehlen die deckungsgleichen Bauteile aller Figuren. Die Seite zeigt das offen an. Die Felltonverschiebung, Musterflächen und Nähte sind Vorschauhilfen und müssen mit echten Bauteilen geprüft werden. Ein physischer Test am Waveshare ist nicht erfolgt.
