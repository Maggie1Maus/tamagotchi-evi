const CACHE = 'kleines-zuhause-v12-momente';
const ASSETS = ['./', './index.html', './style.css', './app.js', './pet.js', './moments.js', './sky.js', './evi-sprite.js', './art.js',
  './icon.svg', './icon-192.png', './icon-512.png', './manifest.webmanifest'];
// Evi's drawings (img/*.png, listed in art.js) are stored as soon as they are first shown.
const isArt = url => url.pathname.startsWith(new URL('./img/', self.registration.scope).pathname);
self.addEventListener('install', event => {
  event.waitUntil(caches.open(CACHE).then(cache => cache.addAll(ASSETS)));
  self.skipWaiting();
});
self.addEventListener('activate', event => {
  event.waitUntil(caches.keys().then(keys => Promise.all(keys.filter(key => key.startsWith('kleines-zuhause-') && key !== CACHE).map(key => caches.delete(key)))).then(() => self.clients.claim()));
});
self.addEventListener('fetch', event => {
  if (event.request.method !== 'GET' || new URL(event.request.url).origin !== self.location.origin) return;
  const assetPaths = new Set(ASSETS.map(path => new URL(path, self.registration.scope).pathname));
  const url = new URL(event.request.url);
  if (!assetPaths.has(url.pathname) && !isArt(url)) return;
  // Revalidate when online; use the last complete app offline.
  event.respondWith(fetch(event.request).then(response => {
    if (response.ok && response.type === 'basic' && !response.redirected) {
      const copy = response.clone();
      event.waitUntil(caches.open(CACHE).then(cache => cache.put(event.request, copy)));
    }
    return response;
  }).catch(async () => {
    const cached = await caches.match(event.request);
    if (cached) return cached;
    if (event.request.mode === 'navigate') return caches.match('./index.html');
    return Response.error();
  }));
});
