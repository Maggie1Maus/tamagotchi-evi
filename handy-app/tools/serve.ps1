param([string]$Root, [int]$Port = 4173)
$Root = (Resolve-Path $Root).Path
$mime = @{ '.html'='text/html; charset=utf-8'; '.css'='text/css'; '.js'='text/javascript'; '.webmanifest'='application/manifest+json'; '.svg'='image/svg+xml'; '.png'='image/png' }
$l = [System.Net.HttpListener]::new()
$l.Prefixes.Add("http://127.0.0.1:$Port/")
$l.Start()
Write-Host "Local: http://127.0.0.1:$Port"
while ($l.IsListening) {
  $c = $l.GetContext()
  try {
    $p = [Uri]::UnescapeDataString($c.Request.Url.AbsolutePath)
    if ($p -eq '/') { $p = '/index.html' }
    $f = [System.IO.Path]::GetFullPath((Join-Path $Root $p.TrimStart('/')))
    if (-not $f.StartsWith($Root) -or -not (Test-Path $f -PathType Leaf)) { $c.Response.StatusCode = 404 }
    else {
      $b = [System.IO.File]::ReadAllBytes($f)
      $ext = [System.IO.Path]::GetExtension($f)
      $c.Response.ContentType = if ($mime[$ext]) { $mime[$ext] } else { 'application/octet-stream' }
      $c.Response.Headers.Add('Cache-Control', 'no-cache')
      $c.Response.OutputStream.Write($b, 0, $b.Length)
    }
  } catch { $c.Response.StatusCode = 500 }
  $c.Response.Close()
}
