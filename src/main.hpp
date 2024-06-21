
#include <Arduino.h>
#include <SD.h>
#include <M5Unified.h>
#include <ESP32-targz.h>
#include <M5StackUpdater.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <LITTLEFS.h>
#include <ESPWebDAV.h>


void POWER_OFF();
void REBOOT();
void ntp_setup();

void MD02_disp();
void MD03_disp();
void MD02_BtnChk();
void MD03_BtnChk();

void setupSDUpdater(const char *appName);
void webDav_setup();
void wifi_setup();
bool SdBegin();
bool wifiTxtSDRead();

#define D1_SERI 1
#define D2_DISP 2
#define D3_BOTH 3
void prt(String sData, int direction=D3_BOTH);
void prtln(String sData, int direction=D3_BOTH);