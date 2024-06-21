// *****************************************************
//  SDU-WebDav  main.h           2024-06-20  by NoRi
// *****************************************************
#ifndef _MAIN_H_
#define _MAIN_H_

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

void setup(void);
void loop(void);
void BtnChk();
void ntp_setup();
void setupSDUpdater(const char *appName);
void MD00_BtnChk();
void MDxx_BtnChk();
void doWork(int mode);
void MD00_disp();
void MDxx_disp(int mode);
void wifi_setup();
void webDav_setup();
bool SdBegin();
bool wifiTxtSDRead();

#define D1_SERI 1
#define D2_DISP 2
#define D3_BOTH 3
void prt(String sData, int direction = D3_BOTH);
void prtln(String sData, int direction = D3_BOTH);

void POWER_OFF();
void REBOOT();
void setFileSystemNVM(int flSys);
int getFileSystemNVM();

#endif //_MAIN_H_
