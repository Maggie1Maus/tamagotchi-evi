# Bausatz-Test (27. November 2026)

Kleiner Versuch, ob Ohren, Schwänze und Fell **per Skript** statt per Bild-KI entstehen können. Nur ein Test, nichts davon ist im Spiel.

- `bausatz.py` erzeugt `pruefbogen-test.png` (`python3 bausatz.py`).
- **Runde Basis** (64 × 64, 16 Graustufen), nicht Evi: Kopf, Körper, Füße, Gesicht, Bauchfleck.
- **Teile sind nur Formen.** Füllung, Schattierung und der 1-Pixel-Umriss entstehen automatisch, deshalb passen alle zusammen. Ansatzpunkte stehen in `ANKER`.
- 5 Ohren (spitz, rund, lang, hängend, Büschel), 4 Schwänze (Puschel, buschig mit heller Spitze, dünn, Blitz), Fellton, Bauchton, Muster (Flecken, Streifen).
- Zeichnung und Gesicht der Basis sind eigene Formen, keine Evi-Pixel.

## Version 2 (größer, mit Details)
`bausatz2.py` → `pruefbogen-test2.png`: 96 × 96 Pixel, Fellpunkte, gerasterte Verläufe, flauschiger Rand, Augen mit Glanzpunkten, Mund mit Zunge, Schnurrhaare, Bauchfell, Zehen, Ohrinnenfell.
`schablonen.py` erzeugt die Schablonen für die Bild-KI. Prompt: [PROMPT-ki-teile.md](PROMPT-ki-teile.md).

## Freistehende KI-Teile

In [ki/teile](ki/teile) liegt je Ohr- und Schwanzform die ausgewählte von drei Varianten. `python3 bausatz2.py` lädt die PNGs anhand von Form und Varianten-Nummer und erstellt den [Prüfbogen](pruefbogen-test2.png). Weiße Pixel sind beim Zusammensetzen durchsichtig. Das rechte Ohr wird aus dem linken gespiegelt. Fehlt ein Teil, verwendet das Skript die bisherige Form.

## Gesichtsausdrücke

`tiere(gesicht='lachend')` setzt ein freistehendes Gesicht auf die Figur.
Verfügbare Ausdrücke sind `freundlich`, `lachend`, `ueberrascht`,
`traurig`, `wuetend` und `muede`. Ohne Gesichtsauswahl bleibt
`mund='offen'` oder `mund='zu'` wie bisher verfügbar. Schablone und
Bildauftrag stehen im [Prompt](PROMPT-ki-teile.md).

`python3 gesichterbogen.py` erstellt den [Prüfbogen](pruefbogen-gesichter.png)
mit demselben Tier pro Ausdruck, vergrößert sowie mit dunklerem Fell in Originalgröße.

## Pose-Test (27. November 2026)
`posen.py` → `pruefbogen-posen.png`: dieselben Teile (Ohren, Schwänze, Gesichter, Fell, Muster) in vier Posen: stehen, hüpfen, schlafen, neugierig (Kopf schief). Eine Pose ist nur eine Tabelle in `POSEN` (Formen von Kopf, Körper, Armen, Füßen plus Ansatzpunkte für Ohren links/rechts, Schwanz und Gesicht). Die Teile bleiben unverändert.
Grenzen: Seitenansichten (Laufen, Schnüffeln) gehen so nicht, dafür brauchen Kopf, Ohren und Gesicht eigene Teile im Profil.

## 3/4-Basis aus KI-Teilen (27. November 2026)
`basis3.py` setzt Körper, Kopf und Halskrause (`ki/teile/koerper-sitzend-*`, `kopf-3-4-*`, `kruse-flauschig-*`) mit Ohren, Schwanz und Gesicht auf 112 × 112 zusammen (Anordnung: `basis3_layout.py`). Prüfbögen: `pruefbogen-basis3.png` (alle neun Körper/Kopf-Kombinationen), `pruefbogen-basis3-varianten.png` (acht Beispiele mit Ohren, Schwänzen, Fellton, Kranzton, Gesicht). Ohren, Schwänze und Gesichter sind noch die frontalen Teile; sie müssen für 3/4 neu bestellt werden.

## Posen auf der 3/4-Basis (27. November 2026)
`posen3.py` → `pruefbogen-posen-basis3.png`: sitzend, laufen, hüpfen, geduckt, eingerollt (+ Zzz) mit denselben Köpfen, Ohren, Schwänzen und Gesichtern. Körper: `ki/teile/koerper-<pose>-<nr>.png`, Ansatz- und Zielpunkte je Pose in `basis3_layout.py` (`POSEN`). `emotionen3.py` zeigt 11 Gefühle, `aufstellung.py` alle 21 Figuren als Rezept (Variationstest, `pruefbogen-aufstellung.png`).

## Weitere Körper- und Kopfformen (27. November 2026)
Körper rundlich, schlank, kräftig und Köpfe klein-rund und breit (der spitze Kopf wurde verworfen) (`ki/teile/…`). `formen.py` setzt sitzende Figuren aus beliebigen Körper- und Kopfformen zusammen (`pruefbogen-formen.png`, 4 × 3 Grundformen). `aufstellung.py` baut mit diesen Formen alle 21 Figuren (`pruefbogen-aufstellung-formen.png`; der ältere Bogen mit nur einer Form heißt `pruefbogen-aufstellung.png`). Nur sitzend; die Posen laufen, hüpfen, geduckt, eingerollt gibt es bisher nur für die Normalform.

## Artmerkmale (27. November 2026)
Teile in `ki/teile/`: `merkmal-maehne`, `merkmal-stacheln`, `merkmal-panzer`, `merkmal-fluegel-fee`, `merkmal-fluegel-drache`, `merkmal-ruessel`, `ohr-antenne`. `formen.py` setzt sie über den Parameter `merkmale` ein (Mähne hinter dem Kopf, Stacheln als Band entlang Kopf und Rücken, Panzer wie ein Rucksack, Flügel hinter dem Körper, Rüssel vor dem Gesicht, Antennen als Ohren). Prüfbogen: `pruefbogen-artmerkmale.png` (Leo, Rolli, Tetro, Lumi, Blubber, Bit, Blip, Ella). Nur sitzend.
