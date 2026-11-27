# Bildauftrag: männliche Evi (alle Altersstufen, Emotionen und Posen)

Stand 26. November 2026. Wunsch der Nutzerin: Es braucht eine **männliche Gesamtzeichnung von Evi mit allem Drum und Dran**. Jungen aus Evis Familie bauen auf ihr auf, Mädchen auf Evi (siehe [Baukasten-Prompt](../../archiv/entwuerfe/grillme-genetik-prototyp/PROMPT-baukasten.md)).

Die männliche Evi ist **dieselbe Tierart wie Evi**, nur als Junge: sofort als Evis Bruder erkennbar, nicht als neues Tier.

## Was ihn von Evi unterscheidet (Vorschlag, gern ändern)
- etwas **breiterer Kopf und kräftigerer Körper** (etwa 5–10 % breiter), gleiche Höhe
- **vollere Halskrause**, die seitlich an den Wangen zu einer kleinen Mähne ausläuft
- **größeres Stirnbüschel** (drei etwas höhere Zacken)
- **buschigerer, dickerer Schwanz**, weiße Spitze bleibt
- Ohren, Augen, Nase, Mund, Fellpunkte, Pfoten: **genau wie Evi**
- Senior: **Opa statt Oma**, Brille wie Oma-Evi, dazu etwas buschigere Augenbrauen; die hellen Strähnen auf dem Kopf dürfen bleiben

## So benutzen
1. Anhängen: `entwuerfe/evi/graustufen-vorschau/Evi-Graustufen.zip` (oder mindestens die vier Bilder `Sprites/0x-…/gluecklich.png`) und `entwuerfe/evi/evi-aktuell-referenz.png`.
2. **Zuerst nur Runde 1** einfügen und das Ergebnis prüfen. Erst wenn der Junge passt, Runde 2 und 3. Das Bild aus Runde 1 bei jeder weiteren Runde wieder anhängen.
3. Weicht die KI ab, nur diesen Korrekturhinweis senden:
   > "Keep him exactly the same species and style as the attached Evi — same ear shape, same eye style, same outline weight, same grayscale shading and fur dots. Only apply the listed male differences."
4. Ergebnis wie Evis ZIP ablegen: `entwuerfe/evi/maennlich/Sprites/<02-Baby|03-Teenager|04-Erwachsen|05-Opa>/<name>.png`.

## Liste aller Bilder (wie bei Evi)
Je Altersstufe Baby, Teenager, Erwachsen, Opa:
- **Emotionen:** ruhe, gluecklich, hungrig, traurig, muede, krank, schmutzig, schlafend, beleidigt
- **Posen:** laufen, fenster, denken, singen, gaehnen, drehen, huepfen, staunen, garten-schnuppern, garten-untersuchen, garten-zeigen

Das sind 20 Bilder je Stufe, 80 insgesamt. Das Ei bleibt für Jungen und Mädchen gleich. Evi-Erwachsen hat kein eigenes „ruhe“-Bild; für den Jungen trotzdem eines anlegen.

---

## Runde 1: Charakterblatt (zum Kopieren)

```text
Use case: character design for a grayscale pixel-art virtual pet game. The attached images are EVI, the existing female character, in four life stages (baby, teen, adult, senior). They are my own approved drawings.

TASK: Create EVI'S MALE COUNTERPART — her brother, the same species, same art style. A viewer must instantly see he is the same kind of creature as Evi. Do NOT redesign the species.

KEEP EXACTLY AS IN THE REFERENCE:
- Same pixel-art style: stepped dark-gray outline of the same thickness, 16-level grayscale shading, soft gray fur dots
- Same large foxlike ears with dark inner ear, same ear shape for each life stage
- Same eye style for each emotion (closed happy arcs, large glossy eyes with square highlight when open), same tiny nose and open smile with small tongue
- Same white neck ruff, same white tail tip, same small white front paws, same seated front-facing pose
- Same proportions per life stage (baby round and small, teen slimmer and taller, adult, senior with glasses)

MALE DIFFERENCES (subtle, only these):
- Head and body about 5-10% wider, slightly sturdier, same height
- Fuller neck ruff that continues a little up the cheeks like a small mane
- Bigger forehead tuft (three slightly taller spikes)
- Thicker, bushier tail
- Senior stage: grandpa instead of grandma — same round glasses, slightly bushier eyebrows

OUTPUT: One character sheet, four poses side by side in a row: baby, teen, adult, grandpa — all in the HAPPY expression (eyes closed in happy arcs, open smile), seated, facing the viewer, same scale relationship as the reference. Pure white background, no text, no frame, no shadow other than a minimal one, no color at all — only black, white and grays.
```

## Runde 2: Emotionen (je Altersstufe einmal, zum Kopieren)

```text
Using the attached approved male Evi character sheet and the attached female Evi emotion sprites as the pose and expression reference, draw the male Evi as a [BABY / TEEN / ADULT / GRANDPA] in these 9 expressions, each as a separate figure in a 3x3 grid, same scale, same style, white background, no text:
1 neutral resting, 2 happy, 3 hungry, 4 sad, 5 tired, 6 sick, 7 dirty, 8 sleeping, 9 sulking/offended.
Match each expression exactly to the female Evi sprite of the same name (same eyes, mouth, props like dirt spots or sick marks). Only the male differences from the character sheet change. No color, only black, white and grays.
```

## Runde 3: Posen (je Altersstufe einmal, zum Kopieren)

```text
Using the attached approved male Evi character sheet and the attached female Evi pose sprites as reference, draw the male Evi as a [BABY / TEEN / ADULT / GRANDPA] in these 11 poses, each as a separate figure in a grid, same scale, same style, white background, no text:
1 walking, 2 looking out of a window (seen from behind/side as in the reference), 3 thinking, 4 singing, 5 yawning, 6 turning around, 7 hopping, 8 amazed, 9 garden sniffing, 10 garden investigating, 11 garden showing a found item.
Match each pose exactly to the female Evi sprite of the same name. Only the male differences from the character sheet change. No color, only black, white and grays.
```

## Danach
- Die Bilder einzeln ausschneiden und auf die Größe von Evis Sprites bringen (gleiche Fußlinie). Das kann Claude mit einem Skript machen, ohne etwas neu zu zeichnen.
- Für den Baukasten braucht es zusätzlich **„Basis ohne Ohren und Schwanz“** je Altersstufe, für Evi und den Jungen. Am sichersten radierst du das selbst aus, oder du lässt es mit Runde 1 als Vorlage von der KI erzeugen und prüfst es.
- Animationen (GIFs wie bei Evi) kommen erst, wenn die Einzelbilder freigegeben sind.
