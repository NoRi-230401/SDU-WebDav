
#include <Arduino.h>
#include <SD.h>
#include <M5Unified.h>
#include <ESP32-targz.h>
#include <M5StackUpdater.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <LITTLEFS.h>
#include <nvs.h>
#include <ESPWebDAV.h>

int getFileSystemNVM();
void setFileSystemNVM(int flSys);

void POWER_OFF();
void REBOOT();
void ntp_setup();

void MD00_disp();
void MDxx_disp(int mode);
void doWork(int mode);

void MD00_BtnChk();
void MDxx_BtnChk();

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