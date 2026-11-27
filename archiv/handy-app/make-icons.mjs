// Generate the SVG and PNG versions of the small Evi icon from the same pixel grid.
import { deflateSync } from 'node:zlib';
import { writeFileSync } from 'node:fs';
const gridSize = 32;
const shapes = [
  ['000000', [[8.7,16],[6.1,14.3],[4,11.8],[3.1,9],[3.2,6.6],[4.1,5.5],[5.8,5.3],
    [8,6.5],[10.2,8.7],[12.1,11.6],[13.2,14.8]]],
  ['ffffff', [[9,14.7],[6.9,13.4],[5.1,11.3],[4.3,8.8],[4.4,6.8],[5.7,6.6],
    [7.5,7.5],[9.3,9.4],[11,12],[11.8,14.1]]],
  ['000000', [[5.3,7.5],[7.3,8.9],[9.2,11.2],[10.1,13.5],[8.6,12.8],[6.7,10.8]]],
  ['000000', [[18.1,14.9],[19.3,11.5],[21.6,8.2],[24.5,5.3],[26.6,4.2],[28,4.6],
    [28.8,6.2],[28.6,8.6],[27.5,11.9],[25.5,14.7],[23,16.6]]],
  ['ffffff', [[19.5,14.1],[20.4,11.8],[22.5,8.9],[25.1,6.3],[26.6,5.5],[27.3,5.8],
    [27.6,6.4],[27.3,8.4],[26.4,11.3],[24.7,13.8],[22.9,15.3]]],
  ['000000', [[26.6,6.8],[26.2,9.5],[25,12],[23.6,13.6],[21.9,14],[23.6,10.7]]],
  ['000000', { cx: 16, cy: 18.7, rx: 11.8, ry: 8.4 }],
  ['ffffff', { cx: 16, cy: 18.7, rx: 10.5, ry: 7.1 }],
  ['000000', { cx: 11.3, cy: 18.5, rx: 2.4, ry: 3 }],
  ['000000', { cx: 20.7, cy: 18.5, rx: 2.4, ry: 3 }],
  ['ffffff', { cx: 10.7, cy: 17.5, rx: .85, ry: .85 }],
  ['ffffff', { cx: 20.1, cy: 17.5, rx: .85, ry: .85 }],
  ['ffffff', { cx: 12, cy: 19.5, rx: .35, ry: .35 }],
  ['ffffff', { cx: 21.4, cy: 19.5, rx: .35, ry: .35 }],
  ['000000', { cx: 16, cy: 20.9, rx: 1.15, ry: .7 }],
  ['000000', [[15.6,21.1],[16.4,21.1],[16.4,22.1],[17.3,22.4],[17.8,22.1],
    [18.2,22.6],[17.3,23.2],[16,22.6],[14.7,23.2],[13.8,22.6],[14.2,22.1],
    [14.7,22.4],[15.6,22.1]]]
];
const artwork = shapes.map(([fill, shape]) => {
  if (Array.isArray(shape)) {
    return `<polygon fill="#${fill}" points="${shape.map(point => point.join(',')).join(' ')}"/>`;
  }
  const { cx, cy, rx, ry } = shape;
  return `<ellipse fill="#${fill}" cx="${cx}" cy="${cy}" rx="${rx}" ry="${ry}"/>`;
});
const svg = [
  `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 ${gridSize} ${gridSize}">`,
  `  <rect width="${gridSize}" height="${gridSize}" rx="7" fill="#ffffff"/>`,
  '  <g>',
  ...artwork.map(shape => `    ${shape}`),
  '  </g>',
  '</svg>',
  ''
].join('\n');
writeFileSync(new URL('./dist/icon.svg', import.meta.url), svg);
function contains(shape, x, y) {
  if (!Array.isArray(shape)) {
    const { cx, cy, rx, ry } = shape;
    return ((x - cx) / rx) ** 2 + ((y - cy) / ry) ** 2 <= 1;
  }
  let inside = false;
  for (let i = 0, j = shape.length - 1; i < shape.length; j = i++) {
    const [xi,yi] = shape[i], [xj,yj] = shape[j];
    if ((yi > y) !== (yj > y) && x < (xj-xi)*(y-yi)/(yj-yi)+xi) inside = !inside;
  }
  return inside;
}
function crc32(bytes) {
  let crc = 0xffffffff;
  for (const b of bytes) { crc ^= b; for (let i=0;i<8;i++) crc=(crc>>>1)^((crc&1)?0xedb88320:0); }
  return (crc ^ 0xffffffff) >>> 0;
}
function chunk(type, data) {
  const name = Buffer.from(type), len = Buffer.alloc(4), sum = Buffer.alloc(4);
  len.writeUInt32BE(data.length); sum.writeUInt32BE(crc32(Buffer.concat([name,data])));
  return Buffer.concat([len,name,data,sum]);
}
for (const size of [192,512]) {
  const row = size*3+1, raw = Buffer.alloc(size*row);
  for (let y=0;y<size;y++) for (let x=0;x<size;x++) {
    let color='ffffff';
    for (const [fill,shape] of shapes) if (contains(shape,(x+.5)*gridSize/size,(y+.5)*gridSize/size)) color=fill;
    Buffer.from(color,'hex').copy(raw,y*row+1+x*3);
  }
  const header = Buffer.alloc(13); header.writeUInt32BE(size,0);header.writeUInt32BE(size,4);header[8]=8;header[9]=2;
  const png = Buffer.concat([Buffer.from('89504e470d0a1a0a','hex'),chunk('IHDR',header),chunk('IDAT',deflateSync(raw)),chunk('IEND',Buffer.alloc(0))]);
  writeFileSync(new URL(`./dist/icon-${size}.png`,import.meta.url),png);
}
