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

# Formen (sitzend). Platzierung: Boden bei y = 110, Körper-Hals mittig bei x = 56.
#   hals_ziel_y = 110 - (Höhe - 1) + hals_y ; Kopf-Hals liegt 4 Pixel darüber.
KOERPER_FORMEN = {
    'sitzend':  dict(groesse=(80, 48), hals=(40, 3), schwanz=(66, 26)),
    'rundlich': dict(groesse=(72, 40), hals=(36, 3), schwanz=(60, 22)),
    'schlank':  dict(groesse=(76, 56), hals=(36, 3), schwanz=(62, 34)),
    'kraeftig': dict(groesse=(84, 50), hals=(42, 3), schwanz=(70, 28)),
}
# Köpfe: Hals-Ansatz, Ohr links/rechts, Gesicht (alle in Rasterpixeln des Kopfteils)
KOPF_FORMEN = {
    '3-4':        dict(groesse=(60, 44), hals=(30, 40), ohr_l=(13, 8), ohr_r=(47, 8), gesicht=(30, 24)),
    'klein-rund': dict(groesse=(52, 40), hals=(26, 36), ohr_l=(11, 7), ohr_r=(41, 7), gesicht=(26, 22)),
    'breit':      dict(groesse=(68, 44), hals=(34, 40), ohr_l=(15, 8), ohr_r=(53, 8), gesicht=(34, 24)),
}

# Artmerkmale: Ansatz auf der Leinwand wird je Körperform aus der Rückenlinie abgeleitet:
#   Rücken-Ansatz = (Hals-x + 16, Oberkante des Körpers an dieser Spalte).
#   Mähne: Mitte hinter dem Kopf (Kopfmitte = Gesicht-Ansatz des Kopfes).
#   Antennen und Hörner sind Ohren-Teile (ki/teile/ohr-antenne-*.png, ohr-horn-*.png) und nutzen die Ohren-Ansätze.
RUECKEN_VERSATZ = 16
MERKMALE = {
    'maehne':   dict(art='hinter_kopf', groesse=(88, 68), anker=(44, 38)),
    'stacheln': dict(art='ruecken_hinten', groesse=(48, 32), anker=(6, 26)),
    'panzer':   dict(art='ruecken_vorn', groesse=(56, 44), anker=(6, 40)),
    'fluegel':  dict(art='ruecken_hinten', groesse=(56, 56), anker=(6, 50)),
}
