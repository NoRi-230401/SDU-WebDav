// *******************************************************
//  SDU_webDav          by NoRi 2025-04-15
// -------------------------------------------------------
// sdu_webDav.h
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

// --- used in 'main.cpp' ---- 
extern bool setupNetwork();
extern void STOP();
extern void prt(String message);

//---- units ------
#define UNIT_AUTO 1
#define UNIT_BYTE 2
#define UNIT_KIRO 3
#define UNIT_MEGA 4
#define UNIT_GIGA 5
#define UNIT_TERA 6

// - File System Types -
#define FS_SPIFFS 1
#define FS_SD 2

extern void prt(String message);
// -------------------------------------------------------
extern const String PROG_NAME,VERSION,GITHUB_URL;
extern const String YOUR_SSID, YOUR_SSID_PASS, YOUR_HOST_NAME;
extern const String WIFI_TXT;
extern bool DISP_ON;
extern String SSID, SSID_PASS, HOST_NAME,IP_ADDR;
// -------------------------------------------------------
#endif
