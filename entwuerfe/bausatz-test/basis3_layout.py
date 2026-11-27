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

# Posen: Körper-Ansätze im Teil (Hals, Schwanz) und Zielpunkt des Halses auf der Leinwand.
# Kopf, Ohren und Gesicht liegen relativ zum Kopf-Hals (KOPF_HALS): Ohren (-17,-32)/(+17,-32), Gesicht (0,-16).
KOPF_OHR_LINKS = (-17, -32)
KOPF_OHR_RECHTS = (17, -32)
KOPF_SEITE_LINKS = (-25, -24)
KOPF_SEITE_RECHTS = (26, -24)
KOPF_GESICHT = (0, -16)
POSEN = {
    #  Teilname          Größe     Hals     Schwanz   Hals-Ziel auf der Leinwand  Kopf-Hals-Ziel
    'sitzend':    dict(groesse=(80, 48), hals=(40, 3),  schwanz=(66, 26), hals_ziel=(56, 66), kopf_ziel=(56, 62)),
    'laufen':     dict(groesse=(88, 48), hals=(22, 8),  schwanz=(76, 14), hals_ziel=(30, 70), kopf_ziel=(30, 66)),
    'huepfen':    dict(groesse=(80, 52), hals=(30, 8),  schwanz=(68, 18), hals_ziel=(40, 62), kopf_ziel=(40, 56)),
    'geduckt':    dict(groesse=(88, 44), hals=(14, 22), schwanz=(78, 8),  hals_ziel=(30, 76), kopf_ziel=(30, 72), schwanz_dx=-10),
    'eingerollt': dict(groesse=(88, 40), hals=(24, 10), schwanz=(66, 18), hals_ziel=(36, 78), kopf_ziel=(36, 74)),
}
