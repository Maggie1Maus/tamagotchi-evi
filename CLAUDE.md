# Tamagotchi-Projekt „Evi“ – Hinweise für Claude

Dieses Projekt wird auf mehreren Geräten bearbeitet (PC, anderer PC, Handy/Cloud-Session).
Alles Wichtige muss deshalb **im Repository** stehen, nicht nur im lokalen Gedächtnis.

## Wo steht was
- `PROJEKTSTAND.md` – aktueller Stand, Beschlüsse, nächste Schritte. **Zu Beginn lesen, nach jeder größeren Änderung aktualisieren.**
- `Tamagotchi_Anforderungen.md`, `Tamagotchi_Hardware_Cube.md`, `Ideen und Infos.md` – Anforderungen, Hardware, Ideen.
- `handy-app/` – spielbare Web-App (siehe `handy-app/README.md`), Spielregeln in `handy-app/dist/pet.js`.
- `software/` – Arduino-/Hardware-Schritte.
- `entwuerfe/evi/` – Evis Zeichnungen von der Nutzerin.

## Feste Regeln
- **Evis Zeichnungen 1:1 verwenden.** Nur zuschneiden, verkleinern, außen transparent machen (`handy-app/tools/build-art.ps1`). Nie neu zeichnen oder „interpretieren“ – ein nachgebautes Baby wurde am 18.11.2026 deutlich abgelehnt. Für neue Varianten die Nutzerin um Zeichnungen bitten.
- Seit Version 0.8 nutzen alle Altersstufen (auch Erwachsen) ihre Zeichnungen. Die prozedurale Pixel-Evi in `evi-sprite.js` ist nur noch Rückfall.
- Darstellung strikt **schwarz-weiß**, keine Farben, kein Graustufenfilter.
- Kommunikation auf Deutsch, einfach erklärt.

## Technik
- Node.js ist auf dem Haupt-PC nicht installiert. Vorschau: `.claude/launch.json` → `handy-app/tools/serve.ps1` (PowerShell-Server, Port 4173).
- Arbeitsablauf mit Git: vor dem Arbeiten `git pull`, danach committen und `git push`, damit andere Geräte den Stand haben.
