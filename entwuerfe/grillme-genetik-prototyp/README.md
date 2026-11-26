# Grillme: Genetik, Schritte 1–5

Stand: 26. November 2026. Dieser Ordner ist eine isolierte Vorschau. Die Firmware und die Handy-App bleiben unverändert. `index.html` im Browser über einen lokalen Webserver öffnen (vom Repo-Wurzelordner z. B. `npx serve .`). Die vier Altersstufen und die Herkunft jedes sichtbaren Teils lassen sich wechseln; „Neue Kreuzung“ erzeugt weitere Kombinationen. Die kleine Auf-und-ab-Bewegung verwendet dieselbe Logik für alle vier Grundformen und Altersstufen. Sie ersetzt noch keine Lauf- oder Schlafanimation.

## Beschlossene Anforderungen

- Genetische Basis: 21 bestehende Tamagotchis, davon Evi plus 20 Parkfiguren. Kind und erwachsenes Tier haben eine individuell zusammengesetzte Gestalt. Dieselbe Kombination soll sichtbar durch Baby, Teenager, Erwachsen und Senior altern.
- Kopf, Hals, Körper und Beine bleiben eine zusammenhängende Grundform. Fellzeichnung und schwierige anatomische Merkmale gehören ebenfalls dazu. Ohren, Augen, Schnauze und Schwanz sind grundsätzlich separat vererbbar. Flügel, Hörner und Geweih nur, wenn die Grundform sie aufnehmen kann.
- Ausschließlich direkte, sichtbare Vererbung aus den Eltern; keine Mutation, keine rezessiven Gene. Bei der Auswahl nach Möglichkeit eine Kombination wählen, die sich von beiden Eltern unterscheidet. Ist dies wegen kompatibler Teile nicht möglich, geht anatomische Stimmigkeit vor.
- Jede Grundform legt erlaubte Teilarten und Ankerpunkte pro Altersstufe fest. Unpassende Teile werden ausgeschlossen. Ungewöhnliche, aber gut sitzende Kombinationen bleiben erlaubt.
- Eine gemeinsame Animationslogik soll später für alle Kombinationen Laufen, Schlafen und weitere Posen tragen. Vor einer Integration müssen diese Posen und Speicherbedarf separat geprüft werden (Schritt 6).
- Bei jedem Generationswechsel wird die Dorfbevölkerung neu zusammengesetzt: Nachkommen bisheriger Bewohner und neue Kreuzungen der 21 Ausgangsfiguren. Ohne Partner bleibt das bisherige Tamagotchi ein genetischer Elternteil; der zweite wird aus der Bibliothek zufällig zusammengestellt.
- Stammbaum: individuelles Porträt aus gespeicherten Merkmals-IDs und Name. Bestehende Stammdaten bleiben erhalten. Keine detaillierte Auflistung, welcher Elternteil welches Teil gab. Die derzeitige Firmware speichert nur acht Verstorbene, während früher einmal 50 diskutiert wurden; eine spätere Speicherentscheidung ist nötig.
- Zehn Persönlichkeitstypen, zufällig einer je Tamagotchi. Sie beeinflussen Verhalten, Bedürfnisse und Vorteile spürbar. Enthüllung beim Erwachsenwerden nach drei Lebenstagen, danach im Steckbrief und Stammbaum. Die konkreten zehn Typen und Effekte sind noch nicht beschlossen. Der aktuelle Code kennt sechs Wesen; die neue Regel ist daher eine spätere Änderung.
- Zusätzliche Bedürfnisse Energie und Geselligkeit mit eigenen Anzeigen. Pflegeverantwortung wird gemeinsam getragen. Konkrete Werte und Spielregeln sind noch offen.

## Bestandsaufnahme

| Quelle | Befund |
|---|---|
| `entwuerfe/neue-tamagotchis/graustufen/<name>/<phase>-gluecklich.png` | 20 Figuren × vier Phasen = 80 PNG. Einheitliche Leinwand 72 × 64, 16 Graustufen, binäres Alpha, Standlinie Zeile 61 laut geprüftem Exportskript `make-besucher-grau.py`. |
| `entwuerfe/evi/graustufen-vorschau/Evi-Graustufen.zip` | Vier aktuelle `Sprites/<Phase>/gluecklich.png` für Evi: Baby 94 × 89, Teenager 81 × 107, Erwachsen 114 × 116, Oma 115 × 110. Weitere Emotionen und Posen sind enthalten. Die älteren Schwarz-Weiß-Zeichnungen sind nicht die Prototypquelle. |
| `software/schritt-10/tools/make-besucher-grau.py` | Schneidet transparente Außenränder weg und packt die 80 Bilder für `besucherbilder.h`; es zerlegt keine Körperteile. |
| `software/schritt-10/tools/make-evi-grau.py` | Verknüpft Evis aktuelle Graustufen-Sprites mit bisherigen Formen; ebenfalls keine modularen Teile. |
| `software/schritt-10/` | Waveshare/Pico-2-Renderer, 320 × 240 Display, Graustufen. Zimmer und Park skalieren die Figur aus der Fußposition; Parkfiguren werden altersgleich zu Evi gezeigt. |
| `PROJEKTSTAND.md` | Spielstand im 4096-Byte-EEPROM weitgehend belegt, etwa 330 Byte Lücken. Flash und RAM müssen vor einer Integration gemessen werden. |

Die 84 Bilder sind Altersentwürfe in **einer glücklichen Pose**, keine 84 vollständigen Animationssätze. Die bisherigen Bilder sind flach gezeichnet; vollständige saubere Trennung von überlappenden Ohren, Gesicht und Schwanz erfordert neue Freistellmasken oder zeichnerische Korrekturen. Die Vorschau schneidet sie nur technisch aus den Originalen.

## Modularer Vertrag

Koordinaten beziehen sich auf eine 72 × 64 Entwurfsfläche, Fußlinie `y=61`. Für Evi wird die aktuelle Vorlage proportional eingepasst. Teilbilder behalten 16 Graustufen und Alpha. Das eigentliche Laufzeitschema soll pro Figur und Alter `grundform`, `ohren`, `augen`, `schnauze`, `schwanz` sowie optional `fluegel`, `hoerner`, `geweih` referenzieren. Die Vorschau koppelt Augen und Schnauze vorläufig zur Gesichtszone, damit keine übermalten Pixel als fertige Trennung missverstanden werden.

| Anker je Altersstufe | Bedeutung | Vorschauwert/Herleitung |
|---|---|---|
| `fuss` | Skalierung und Platzierung | `(36,61)`; Evi-Sprite an Fußlinie eingepasst |
| `ohr_links`, `ohr_rechts` | Ansatz oben am Kopf | aus je zwei Quellrechtecken; bei Baby 82 %, Teenager 92 %, Erwachsene/Senior 100 % um Fußlinie skaliert |
| `augen`, `schnauze` | Gesichtsmitte und Überdeckung | vorläufig gemeinsame Gesichtszone; pro Figur in `index.html` definiert |
| `schwanz` | hinterer Rumpfansatz | Quellrechteck pro Figur, hinter der Grundform zu zeichnen, wenn eine saubere Maske vorliegt |
| `fluegel`, `hoerner`, `geweih` | optionale Ansatzpunkte | in diesen vier Quellen nicht vorhanden; erst mit geeigneten Basisfiguren kalibrieren |

Die in `index.html` hinterlegten Rechtecke sind **Prototyp-Anker**, keine freigegebenen Pixelmasken. Die endgültigen Anker müssen pro Figur und Alter anhand der tatsächlichen Kontur vermessen und optisch abgenommen werden. Für den Renderer gilt künftig eine feste Zeichenreihenfolge: hintere Teile → Grundform → Ohren/Gesicht → vordere Teile. Gemeinsame Bewegungszustände transformieren Anker und Teile zusammen.

## Kompatibilität und Vorschautest

Für diese vier Figuren sind Ohren, Gesicht und Schwanz auf den vier vierbeinigen Grundformen grundsätzlich erlaubt. Fehlende optionale Merkmale bleiben leer. Die spätere Matrix für alle 21 Figuren muss Körperklassen unterscheiden: vierbeinig, sitzend/aufrecht, Schnecke (Mimi), schwebend (Spuki) und besondere Grundformen. Eine Schnecke bekommt nicht automatisch klassische Beine oder Ohren; ein schwebender Körper benötigt keinen Beinanker. Die Übertragung ist nur erlaubt, wenn für die Altersstufe ein gültiger Anker und eine geprüfte Überdeckung existieren. Bei fehlendem kompatiblem Spenderteil bleibt das entsprechende Teil des Grundkörpers.

Vorgeprüfte Paarungen: Evi × Lotti, Evi × Glöckchen, Evi × Mia sowie Lotti × Mia; in der interaktiven Vorschau sind alle vier Quellen für jede der drei vererbbaren Zonen wählbar. Die Vorschau zeigt, dass die vier Lebensphasen gemeinsam anzeigbar sind. Besonders Gesichts- und Schwanzüberlappungen können sichtbare doppelte Konturen erzeugen; deshalb sind die Bilder **noch nicht freigegeben**. Vor Massengenerierung: jedes Teil freistellen, Nähte und Konturen in allen vier Altern prüfen, Schlafen/Laufen testen und am echten Bildschirm beurteilen.
