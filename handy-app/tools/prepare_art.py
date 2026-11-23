"""Python copy of prepare-art.ps1, for sessions without PowerShell (e.g. Claude in the cloud).

Same steps, same numbers: crop the figure (without the label below it), scale it, keep pure
black and white, make the white around the figure transparent, add a 1-pixel clear border.
The drawing itself is not changed.

    python3 prepare_art.py IN.png OUT.png 0.31      -> prints "width height"
"""
import sys
from collections import deque

from PIL import Image


def prepare(path_in, path_out, scale):
    src = Image.open(path_in).convert('RGB')
    w, h = src.size
    px = src.load()

    def ink(x, y):
        r, g, b = px[x, y]
        return r + g + b < 384

    row_ink = [any(ink(x, y) for x in range(0, w, 2)) for y in range(h)]
    # Ink rows grouped into blocks; a gap of more than 3 blank rows starts a new block.
    blocks, start, last = [], -1, -1
    for y in range(h):
        if not row_ink[y]:
            continue
        if start < 0:
            start = y
        elif y - last > 3:
            blocks.append((start, last))
            start = y
        last = y
    if start >= 0:
        blocks.append((start, last))
    # Label lines: short blocks at the bottom of the picture. Drop them.
    while len(blocks) > 1:
        b0, b1 = blocks[-1]
        if b1 - b0 < 100 and b0 > h * 0.7:
            blocks.pop()
        else:
            break
    top, bottom = blocks[0][0], blocks[-1][1]
    left, right = w, 0
    for y in range(top, bottom + 1):
        for x in range(w):
            if ink(x, y):
                left, right = min(left, x), max(right, x)
    cw, ch = right - left + 1, bottom - top + 1
    height, ow = round(ch * scale), round(cw * scale)     # round half to even, like Math.Round
    scaled = src.crop((left, top, right + 1, bottom + 1)).resize((ow, height), Image.BICUBIC)
    sp = scaled.load()
    # 0 = outside (transparent), 1 = black, 2 = white inside
    m = [[1 if sum(sp[x, y]) < 3 * 140 else 2 for x in range(ow)] for y in range(height)]
    # The outside is found with the outline thickened by 1 pixel, so a tiny gap in the outline
    # (one pixel wide) does not let the outside run into the figure. Then the white seam right
    # next to the outline is added to the outside. The black and white pixels stay as they are.
    near = [[any(m[yy][xx] == 1 for yy in range(max(0, y - 1), min(height, y + 2))
                 for xx in range(max(0, x - 1), min(ow, x + 2))) for x in range(ow)] for y in range(height)]
    q = deque()
    for y in range(height):
        for x in range(ow):
            if (x == 0 or y == 0 or x == ow - 1 or y == height - 1) and m[y][x] == 2 and not near[y][x]:
                m[y][x] = 0
                q.append((x, y))
    while q:
        x, y = q.popleft()
        for nx, ny in ((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)):
            if 0 <= nx < ow and 0 <= ny < height and m[ny][nx] == 2 and not near[ny][nx]:
                m[ny][nx] = 0
                q.append((nx, ny))
    seam = [(x, y) for y in range(height) for x in range(ow) if m[y][x] == 2 and (
        x == 0 or y == 0 or x == ow - 1 or y == height - 1 or any(
            m[yy][xx] == 0 for yy in range(max(0, y - 1), min(height, y + 2)) for xx in range(max(0, x - 1), min(ow, x + 2))))]
    for x, y in seam:
        m[y][x] = 0
    out = Image.new('RGBA', (ow + 2, height + 2), (255, 255, 255, 0))   # like Color.Transparent
    op = out.load()
    for y in range(height):
        for x in range(ow):
            if m[y][x]:
                op[x + 1, y + 1] = (0, 0, 0, 255) if m[y][x] == 1 else (255, 255, 255, 255)
    out.save(path_out)
    return ow + 2, height + 2


if __name__ == '__main__':
    width, height = prepare(sys.argv[1], sys.argv[2], float(sys.argv[3]))
    print(width, height)
