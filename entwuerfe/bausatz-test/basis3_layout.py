"""Anordnung der 3/4-Basis auf der Leinwand 112 x 112 (alle Angaben in Rasterpixeln).

Jedes Teil hat in seiner Schablone rote/blaue/grüne Ansatzpunkte. Hier steht, wohin sie auf der Leinwand kommen.
Reihenfolge von hinten nach vorn: Schwanz, Körper, Kopf (Ohren hinter dem Kopf), Gesicht.
"""
LEINWAND = (112, 112)

# Teil: (Größe, Ansatzpunkte im Teil)  ->  Ziel auf der Leinwand
KOERPER_ZIEL = (56, 66)        # Hals des Körpers auf der Leinwand (Körper reicht von x 16 bis 95, y 63 bis 110)
KOPF_ZIEL = (56, 71)           # Hals des Kopfes (Kopf reicht von x 26 bis 85, y 22 bis 65)
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
    'sitzend':    dict(groesse=(80, 48), hals=(40, 3),  schwanz=(66, 26), hals_ziel=(56, 66), kopf_ziel=(56, 71)),
    'laufen':     dict(groesse=(88, 48), hals=(22, 8),  schwanz=(76, 14), hals_ziel=(30, 70), kopf_ziel=(30, 75)),
    'huepfen':    dict(groesse=(80, 52), hals=(30, 8),  schwanz=(68, 18), hals_ziel=(40, 62), kopf_ziel=(40, 65)),
    'geduckt':    dict(groesse=(88, 44), hals=(14, 22), schwanz=(78, 8),  hals_ziel=(30, 76), kopf_ziel=(30, 81), schwanz_dx=-10),
    'eingerollt': dict(groesse=(88, 40), hals=(24, 10), schwanz=(66, 18), hals_ziel=(36, 78), kopf_ziel=(36, 83)),
}

# Formen (sitzend). Platzierung: Boden bei y = 110, Körper-Hals mittig bei x = 56.
#   hals_ziel_y = 110 - (Höhe - 1) + hals_y ; Kopf-Hals liegt 4 Pixel darüber.
KOERPER_FORMEN = {
    'sitzend':  dict(groesse=(80, 48), hals=(40, 3), schwanz=(66, 26)),
    'rundlich': dict(groesse=(72, 40), hals=(36, 3), schwanz=(60, 22)),
    'schlank':  dict(groesse=(76, 56), hals=(36, 3), schwanz=(62, 34)),
    'kraeftig': dict(groesse=(84, 50), hals=(42, 3), schwanz=(70, 28)),
    # Altersstufen (27.11.)
    'baby':     dict(groesse=(56, 34), hals=(28, 3), schwanz=(46, 18)),
    'teenager': dict(groesse=(72, 44), hals=(36, 3), schwanz=(60, 24)),
    'senior':   dict(groesse=(76, 46), hals=(34, 8), schwanz=(62, 28)),
    # Formen x Altersstufen (27.11.)
    'rundlich-baby': dict(groesse=(50, 28), hals=(25, 3), schwanz=(42, 16)),
    'rundlich-teenager': dict(groesse=(64, 36), hals=(32, 3), schwanz=(54, 20)),
    'rundlich-senior': dict(groesse=(68, 38), hals=(30, 7), schwanz=(58, 24)),
    'schlank-baby': dict(groesse=(54, 40), hals=(26, 3), schwanz=(44, 26)),
    'schlank-teenager': dict(groesse=(68, 50), hals=(33, 3), schwanz=(56, 31)),
    'schlank-senior': dict(groesse=(72, 52), hals=(32, 8), schwanz=(58, 32)),
    'kraeftig-baby': dict(groesse=(58, 36), hals=(29, 3), schwanz=(49, 20)),
    'kraeftig-teenager': dict(groesse=(76, 46), hals=(38, 3), schwanz=(64, 25)),
    'kraeftig-senior': dict(groesse=(80, 48), hals=(36, 8), schwanz=(66, 27)),
}
# Köpfe: Hals-Ansatz, Ohr links/rechts, Gesicht (alle in Rasterpixeln des Kopfteils)
KOPF_FORMEN = {
    '3-4':        dict(groesse=(60, 44), hals=(30, 40), ohr_l=(13, 8), ohr_r=(47, 8), gesicht=(30, 24)),
    'klein-rund': dict(groesse=(52, 40), hals=(26, 36), ohr_l=(11, 7), ohr_r=(41, 7), gesicht=(26, 22)),
    'breit':      dict(groesse=(68, 44), hals=(34, 40), ohr_l=(15, 8), ohr_r=(53, 8), gesicht=(34, 24)),
    # Altersstufen (27.11.)
    'baby':       dict(groesse=(56, 44), hals=(28, 40), ohr_l=(11, 9), ohr_r=(45, 9), gesicht=(28, 25)),
    'teenager':   dict(groesse=(58, 42), hals=(29, 38), ohr_l=(12, 8), ohr_r=(46, 8), gesicht=(29, 23)),
    'senior':     dict(groesse=(58, 44), hals=(29, 40), ohr_l=(12, 9), ohr_r=(46, 9), gesicht=(29, 25)),
    # Formen x Altersstufen (27.11.)
    'klein-rund-baby': dict(groesse=(48, 40), hals=(24, 36), ohr_l=(9, 8), ohr_r=(39, 8), gesicht=(24, 23)),
    'klein-rund-teenager': dict(groesse=(50, 38), hals=(25, 34), ohr_l=(10, 7), ohr_r=(40, 7), gesicht=(25, 21)),
    'klein-rund-senior': dict(groesse=(52, 40), hals=(26, 36), ohr_l=(11, 8), ohr_r=(41, 8), gesicht=(26, 23)),
    'breit-baby': dict(groesse=(60, 44), hals=(30, 40), ohr_l=(12, 9), ohr_r=(48, 9), gesicht=(30, 25)),
    'breit-teenager': dict(groesse=(64, 42), hals=(32, 38), ohr_l=(13, 8), ohr_r=(51, 8), gesicht=(32, 23)),
    'breit-senior': dict(groesse=(68, 44), hals=(34, 40), ohr_l=(14, 9), ohr_r=(54, 9), gesicht=(34, 25)),
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

# Kleine Aufsätze: Lage relativ zum Kopf (kx, ky = Kopf-Hals; mx, my = Gesichtsmitte; ohr_r = rechter Ohr-Ansatz).
#   schleife:     rechtes Ohr, Ansatz = ohr_r + (-2, 1), vor dem Kopf
#   halsband:     um den Hals, (kx - 2, ky + 3), nach dem Kopf
#   augenmaske:   über den Augen, (mx, my - 4), VOR dem Gesicht
#   backenblitz:  Wange, links (mx - 17, my + 6), rechts gespiegelt
AUFSAETZE = {
    'schleife':     dict(groesse=(24, 20), anker=(12, 16)),
    'halsband':     dict(groesse=(44, 14), anker=(22, 3)),
    'augenmaske':   dict(groesse=(52, 16), anker=(26, 8)),
    'backenblitz':  dict(groesse=(12, 14), anker=(6, 7)),
}
