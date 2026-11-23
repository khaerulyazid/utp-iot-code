#pragma once
#include <pgmspace.h>

// Buat halaman HTML/CSS/JavaScript sendiri di dalam raw string ini.
// Pastikan halaman web mengakses:
//   GET /status  -> menerima JSON status sistem
//   GET /set?mode=manual atau /set?mode=auto
//   GET /set?led=on atau /set?led=off
//   GET /set?th=30
//
// Kontrak JSON dari /status:
// {"temp":28.0,"hum":65,"led":0,"mode":"MANUAL","th":30.0,"ok":1,"uno":true}
//
// Ganti isi HTML di bawah ini dengan dashboard buatan kelompok.
const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Dashboard IoT Kelompok</title>
</head>
<body>
  <h1>Dashboard Monitoring dan Kontrol IoT</h1>
  <p>Silakan buat antarmuka monitoring suhu, kelembapan, status koneksi,
     mode, LED, serta kontrol ambang suhu.</p>
</body>
</html>
)rawliteral";
