// *******************************************************
//  SDU_webDav          by NoRi 2025-04-15
// -------------------------------------------------------
// SDU_webDav.h
// *******************************************************
#ifndef _SDU_WEB_DAV_H
#define _SDU_WEB_DAV_H
// -------------------------------------------------------
#include <Arduino.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <SD.h>
#include <SPIFFS.h>
#include <LITTLEFS.h>
#include <time.h>
#include <ESPWebDAV.h>

// - File System Types -
#define FS_SPIFFS 1
#define FS_SD 2

extern bool setupNetwork();
extern void STOP();
extern void prt(String message);
extern void SD_start();
extern void SPIFFS_start();
extern void DISP_start();
// -------------------------------------------------------
extern const String PROG_NAME,VERSION,GITHUB_URL;
extern const String YOUR_SSID, YOUR_SSID_PASS, YOUR_HOST_NAME;
extern const String WIFI_TXT;
extern bool DISP_ON;
extern String SSID, SSID_PASS, HOST_NAME,IP_ADDR;
extern bool SD_ENABLE, SPIFFS_ENABLE;
// -------------------------------------------------------
#endif
