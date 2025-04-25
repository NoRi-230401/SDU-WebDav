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

extern bool setupNetwork();
extern void STOP();
extern void prt(String message);
// -------------------------------------------------------
extern const String PROG_NAME,VERSION,GITHUB_URL;
extern const String YOUR_SSID, YOUR_SSID_PASS, YOUR_HOST_NAME;
extern const String WIFI_TXT;
extern bool DISP_ON;
extern String SSID, SSID_PASS, HOST_NAME,IP_ADDR;
// -------------------------------------------------------
#endif
