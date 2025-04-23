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
// #include <LITTLEFS.h>
#include <time.h>
#include <ESPWebDAV.h>

// --- used in 'main.cpp' ---- 
extern bool setupNetwork();
extern void STOP();
//---------------------------


// typedef struct
// {
//   String filename;
//   String ftype;
//   String fsize;
// } fileinfo;

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

// -- REQUEST Manager --
#define REQ_NONE 0
#define REQ_REBOOT 98
#define REQ_SHUTDOWN 99

extern void prt(String message);
extern String ConvBytesUnits(uint64_t bytes, int dp, int unit = UNIT_AUTO);
extern String getTmRTC();
extern String getTmNTP();
extern String urlEncode(const String &input);
extern String urlDecode(const String &input);
// -------------------------------------------------------
extern const String PROG_NAME,VERSION,GITHUB_URL;
extern const String YOUR_SSID, YOUR_SSID_PASS, YOUR_HOST_NAME;
extern const String WIFI_TXT;
// extern bool SD_USE, SPIFFS_USE;
extern bool DISP_ON, RTC_ENABLE;
// extern bool RTC_ADJUST_ON;
// extern uint32_t TM_SETUP_DONE;
// extern uint32_t TM_RTC_ADJUST;
// extern uint32_t SHUTDOWN_TM_SEC;
// -------------------------------------------------------
#endif
