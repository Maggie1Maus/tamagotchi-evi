# Prompt: Bauteile Paket 1 (Mia, Glöckchen, Lotti, Klecksi, Funke)

Stand 27. November 2026. Der Probelauf mit Leo, Linus und Groschen ist von der Nutzerin **freigegeben** (siehe [BESCHLUESSE.md](BESCHLUESSE.md)). Jetzt folgen die übrigen 17 Parkfiguren in Paketen, zuerst nur Altersstufe **Erwachsen**.

**Paketplan** (Vorschlag, gern ändern):
| Paket | Figuren | Warum zusammen |
|---|---|---|
| **1** | Mia, Glöckchen, Lotti, Klecksi, Funke | Säugetiere mit klaren Ohren und Schwänzen, dazu neue Muster und Zubehör |
| 2 | Mampf, Rolli, Ella, Kiwi, Tetro | Sonderformen: Stacheln, Rüssel, Dino-Zacken, Panzer |
| 3 | Bit, Blip, Blubber, Lumi, Moosi | Fantasie und Technik: Antennen, Flügel, Blätter |
| 4 | Mimi, Spuki | ohne Ohren und Schwanz, nur Besonderheiten |

---

## Prompt (zum Kopieren)

Der Bauteil-Probelauf (Leo, Linus, Groschen, Erwachsen) ist freigegeben. Arbeite **genauso weiter** wie dort: gleiche Arbeitsweise (Schritte A–H aus `entwuerfe/grillme-genetik-prototyp/PROMPT-bauteile.md`), gleiche Werkzeuge in `baukasten/werkzeug/`, gleiche Regeln. Evis Basis und ihre eigenen Teile bleiben unverändert (Pixelvergleich weiter 0 Abweichungen).

### Paket 1: nur diese fünf Figuren, nur Erwachsen, beide Geschlechter
Für jede Figur: **Ohren** und **Schwanz** als Ebene auf der weiblichen und der männlichen Basis, im Evi-Stil, in der Form der Parkfigur (Parkbild als Formvorlage anhängen), dazu ihre Bereichskarten.

| Figur | Ohren | Schwanz | Weiteres |
|---|---|---|---|
| **Mia** (Kätzchen, w) | spitze Katzenohren, innen dunkel | flauschiger, nach oben eingerollter Katzenschwanz | Zubehör Schleife gibt es schon. Fellton sehr hell, Kranzton weiß |
| **Glöckchen** (Hündin, w) | lange **Schlappohren** mit Fellpunkten, hängen seitlich am Kopf herab. Sie dürfen über die Wangen fallen, aber **nie Augen oder Mund verdecken** | kurzer, eingerollter Schwanz | Zubehör Glöckchen gibt es schon. Das Notizbuch wird nicht übernommen |
| **Lotti** (Rehkitz, w) | große, breite Rehohren | kurzer, hoch stehender weißer Stummelschwanz (Spitze gehört zum Kranz) | **neues Muster `kitz-tupfen`**: Reihen heller runder Tupfen auf Rücken und Flanke, nur innerhalb von `fell`. Wimpern nicht übernehmen (Gesicht bleibt Evis) |
| **Klecksi** (Dalmatiner, w) | Schlappohren, **ein Ohr dunkel, eins hell** (genau so übernehmen) | dünner, gerader Schwanz | Muster `flecken` an Klecksis Dalmatinerflecken anpassen (unregelmäßig, kräftiger). **Neues Muster `augenfleck`**: dunkler Fleck um **ein** Auge, Augenlinien bleiben. **Neues Zubehör `halsband`**: schmales Halsband mit runder Marke, über dem Kranz |
| **Funke** (Eichhörnchen, m) | kurze spitze Ohren mit kleinen Pinselbüscheln an der Spitze | großer buschiger Schwanz in **Zickzack-Blitzform** mit heller Spitze (Spitze gehört zum Kranz) | **Neue Besonderheit `backenblitze`** (vorn): kleiner Blitz auf jeder Wange, liegt auf der Wange, verändert Augen und Mund nicht |

Trage die fünf Figuren mit ihren Werten in `figuren.js` ein (Ohren, Schwanz, Besonderheit, Fellton, Kranzton, Struktur, Muster, Zubehör) und entferne bei ihnen die Markierung „geschätzt“, wo du es am Parkbild geprüft hast.

### Besonders aufpassen
- **Schlappohren** (Glöckchen, Klecksi) sind neu: Sie hängen über den Kopfrand nach unten. Schutzmaske (Schritt D) so erweitern, dass sie seitlich am Kopf liegen dürfen, aber Augen, Mund und Wangenlinie unberührt bleiben. Bereichskarte: Ohr außen = `fell`, Ohr innen nur, wo es sichtbar ist.
- **Funkes Blitzschwanz** ist groß. Er darf hinter dem Körper herausragen, aber keine Pfoten verdecken.
- **Kranztöne** müssen auch an den neuen Schwanzspitzen (Lotti, Funke) sauber und ohne Rechtecke funktionieren.
- Die Regeln aus der letzten Korrekturrunde gelten weiter: heller Kranz auf hellem Fell erlaubt; keine weißen Restpixel am Kranz; keine Struktur auf der Stirnmitte.

### Prüfbogen `werkzeug/pruefbogen-paket-1.png`
- Je Figur eine Zeile: Mädchen-Basis + Teile, Jungen-Basis + Teile (auf weiß und auf dunkelgrau), daneben das Parkbild.
- Eine Zeile mit den neuen Mustern und dem neuen Zubehör (`kitz-tupfen`, angepasste `flecken`, `augenfleck`, `halsband`, `backenblitze`).
- Eine Zeile mit **sechs zufälligen Kindern**, darunter mindestens Evi × Funke (Mädchen-Seite) und männliche Evi × je eine der vier weiblichen Figuren.
- Kranzton 0–3 für Lotti und Funke (wegen der neuen Schwanzspitzen).
- Pixelvergleich Evis eigene Teile (muss 0 bleiben).

Danach `teile-aktualisieren.ps1` ausführen, `PROJEKTSTAND.md` kurz ergänzen, committen und auf `main` hochladen. Sag am Ende ehrlich, wo es noch hakt (Figur, Stelle). **Keine weiteren Figuren und keine anderen Altersstufen**, bevor ich Paket 1 freigebe.
