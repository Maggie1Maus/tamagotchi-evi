# Kompaktes Zimmer mit Raumtiefe

Der gewählte erste Zimmerentwurf ist die Grundlage. `normal.png` ist in 320×174 aus der bestätigten Zeichnung abgeleitet, `weihnachten.png` ergänzt Baum, Sterne und Kerzen im gleichen Raum. Der Fensterausblick wird im Spiel durch die aktuelle Gartenansicht ersetzt. Keine Evi neu gezeichnet.

`make-zimmer-tiefe.py` erzeugt `zimmerlandschaft.h`. Die Vordergrundstempel in `tiefenstempel.json` beschreiben Spielfeldgeometrie: Beim Verdecken von Evi bleiben die Originalpixel der zusammenhängenden Raum-/Gartenzeichnung stehen. Die Stempel sind keine neuen Möbelbilder. `bildauftrag-weihnachten.txt` enthält den vollständigen Bildauftrag der saisonalen Variante. `vorschauen/` zeigt die echte PC-Nachbildung.

Technik, Wege, Altersstufen und Prüfungen: `Raumtiefe_und_Zimmer.md` im Projektstamm. Änderungen sind auf dem Waveshare gebaut; die Handy-App bleibt unverändert. Weihnachten wie bisher vom 18. November bis einschließlich 5. Januar.
