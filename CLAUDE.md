# Tamagotchi-Projekt „Evi“ – Hinweise für Claude

Dieses Projekt wird auf mehreren Geräten bearbeitet (PC, anderer PC, Handy/Cloud-Session).
Alles Wichtige muss deshalb **im Repository** stehen, nicht nur im lokalen Gedächtnis.

## Wo steht was
- `PROJEKTSTAND.md` – aktueller Stand, Beschlüsse, nächste Schritte. **Zu Beginn lesen, nach jeder größeren Änderung aktualisieren.**
- `Tamagotchi_Anforderungen.md`, `Tamagotchi_Hardware_Cube.md`, `Ideen und Infos.md` – Anforderungen, Hardware, Ideen.
- `handy-app/` – spielbare Web-App (siehe `handy-app/README.md`), Spielregeln in `handy-app/dist/pet.js`.
- `software/` – Arduino-/Hardware-Schritte.
- `entwuerfe/evi/` – Evis Zeichnungen von der Nutzerin. In `entwuerfe/evi/momente/` stehen die Bildaufträge für das, was Evi von selbst tut.
- `entwuerfe/neue-tamagotchis/` – Vorschläge für Partner und weitere Tamagotchis mit Bildaufträgen.

## Feste Regeln
- **Evis Zeichnungen 1:1 verwenden.** Nur zuschneiden, verkleinern, außen transparent machen (`handy-app/tools/build-art.ps1`). Nie neu zeichnen oder „interpretieren“ – ein nachgebautes Baby wurde am 18.11.2026 deutlich abgelehnt. Für neue Varianten die Nutzerin um Zeichnungen bitten.
- Seit Version 0.8 nutzen alle Altersstufen (auch Erwachsen) ihre Zeichnungen. Die prozedurale Pixel-Evi in `evi-sprite.js` ist nur noch Rückfall.
- **Die Handy-App nicht mehr mitändern** (Wunsch der Nutzerin vom 24.11.2026). Neues wird nur noch auf dem Waveshare (`software/schritt-10/`) gebaut. Die App nur anfassen, wenn die Nutzerin ausdrücklich danach fragt.
- Darstellung auf dem Waveshare seit 25.11.2026 in **Graustufen** (16 Stufen, Wunsch der Nutzerin), keine Farben. Vorher strikt schwarz-weiß. Bilder ohne Graustufen-Gegenstück (Zimmer, Fundstücke, Symbole) bleiben vorerst schwarz-weiß. Die Handy-App bleibt wie sie ist.
- Evis Graustufen-Bilder: `entwuerfe/evi/graustufen-vorschau/Evi-Graustufen.zip` → `software/schritt-10/tools/make-evi-grau.py` → `evi-handy/evigrau.h`. Wieder 1:1, nie neu zeichnen.
- Seit 26.11.2026 heißen die Tiere allgemein **Tamagotchis**; Evi war das erste. Jedes Baby bekommt von der Nutzerin einen Namen, die Parkfreunde heißen bei jedem Tamagotchi anders (50 + 50 Namen, `namen.h`).
- Kommunikation auf Deutsch, einfach erklärt.

## Technik
- Node.js ist auf dem Haupt-PC nicht installiert. Vorschau: `.claude/launch.json` → `handy-app/tools/serve.ps1` (PowerShell-Server, Port 4173).
- Arbeitsablauf mit Git: vor dem Arbeiten `git pull`, danach committen und `git push`, damit andere Geräte den Stand haben.
- **Immer auch auf `main` hochladen** (Wunsch der Nutzerin vom 20.11.2026). Arbeitet eine Sitzung auf einem eigenen Zweig, die Änderungen zusätzlich auf `main` bringen, ohne nachzufragen. Die anderen Geräte holen sich nur `main`.
- Der Projektordner heißt auf dem Haupt-PC „Tamagotchi Projekt 2“ (unter Dokumente\ChatGPT), auf GitHub `tamagotchi-evi`.
