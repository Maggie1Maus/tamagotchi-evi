"""Anordnung der 3/4-Basis auf der Leinwand 112 x 112 (alle Angaben in Rasterpixeln).

Jedes Teil hat in seiner Schablone rote/blaue/grüne Ansatzpunkte. Hier steht, wohin sie auf der Leinwand kommen.
Reihenfolge von hinten nach vorn: Schwanz, Körper, Kopf (Ohren hinter dem Kopf), Halskrause, Gesicht.
"""
LEINWAND = (112, 112)

# Teil: (Größe, Ansatzpunkte im Teil)  ->  Ziel auf der Leinwand
KOERPER_ZIEL = (56, 66)        # Hals des Körpers auf der Leinwand (Körper reicht von x 16 bis 95, y 63 bis 110)
KOPF_ZIEL = (56, 62)           # Hals des Kopfes (Kopf reicht von x 26 bis 85, y 22 bis 65)
KRUSE_ZIEL = (56, 62)          # Oberkante Halskrause sitzt am Hals
SCHWANZ_ZIEL = (82, 89)        # Schwanz-Ansatz des Körpers (66, 26) -> Leinwand (82, 89); Schwanzteil-Ansatz (3, 29)
OHR_LINKS_ZIEL = (39, 30)      # Kopf-Ansatz (13, 8) -> Leinwand
OHR_RECHTS_ZIEL = (73, 30)     # Kopf-Ansatz (47, 8) -> Leinwand
GESICHT_ZIEL = (56, 46)        # Kopf-Ansatz (30, 24) -> Leinwand
KOERPER_SCHWANZ_ANSATZ = (66, 26)
OHR_SEITE_LINKS_ZIEL = (31, 38)   # Hängeohren: oben Mitte des Ohres am Kopfrand links
OHR_SEITE_RECHTS_ZIEL = (82, 38)
