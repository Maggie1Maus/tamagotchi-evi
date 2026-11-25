# Bestätigtes Zimmer V2 und Firmware-Einbau

Die Nutzerin bestätigte den korrigierten Fensterausblick mit vollständigem Bäumchen, Hügeln und großer Sonne mit Wolke. Anschließend wurden das Grafikpaket und der Einbau beauftragt.

- `bestaetigte-vorschau.png`: bestätigte gestalterische Grundlage.
- `Evi_Grafikpaket_V2.zip`: vollständige getrennte Ebenen, Masken, 51 Ansichten, 16 Animationen, vier editierbare OpenRaster-Szenen und reproduzierbare Exportwerkzeuge.
- `Evi_V2_Vier_Jahreszeiten.png`: native Spielgrafiken im Jahreszeitenvergleich.
- `Evi_V2_Wetter_und_Tageszeiten.png`: Wetter-/Tageszeitvergleich ohne Grautöne.
- `Evi_V2_Ebenenuebersicht.png`: wichtigste unabhängige Bausteine.
- `board/`: tatsächliche Bildschirmbilder der PC-Nachbildung des eingebauten Firmwarestands; die Nachbildung benutzt eine Ersatzschrift.
- `Fenster_aktuell.png` / `.json`: aktuelle 51 Referenzansichten einschließlich nachträglich gewünschter Kontrastvarianten. Dämmerung: weißer Baum, dunkler Rand, kein Mond. Nacht: schwarzer Baum, weißer Rand, Mond bei sichtbarem Himmel. Das ursprüngliche ZIP und seine Übersichten bleiben als Quellenstand erhalten.
- `board/fenster-tag-daemmerung-nacht.png`: direkter Vergleich der drei eingebauten Darstellungen.

Die native Szene ist 320 × 174 Pixel groß und wird auf dem Gerät unter der Statusleiste ab y = 27 gezeichnet. Das Fenster liegt dort bei x = 79, y = 46. Kalender, Wetter und Originalzeichnungen von Evi werden vom bestehenden Spiel geliefert.

`software/schritt-10/tools/make-zimmer-v2.py` verifiziert die Paket-Prüfsummen und erzeugt `zimmergrafiken.h` sowie die aktuellen Referenzansichten. Die Firmware zeichnet diese Ebenen direkt; der Fensterausblick wird nicht mehr aus einem anderen Gartenbild herauskopiert. Ein Test vergleicht sämtliche 51 Firmware-Fenster mit den aktuellen PNG-Referenzen pixelgenau und prüft zusätzlich, dass sämtliche Taggrafiken unverändert dem ursprünglichen Grafikpaket entsprechen. Eigene semantische Prüfungen sichern weiße Baumkörper in der Dämmerung, schwarze Baumkörper mit weißem Rand bei Nacht und den fehlenden Mond in der Dämmerung ab.

Der erste Einrichtungsmodus setzt weitere bestätigte Grillme-Entscheidungen um. Details, Bedienung und offene Punkte in [Einrichtung_und_Grillme.md](../../../../Einrichtung_und_Grillme.md).
