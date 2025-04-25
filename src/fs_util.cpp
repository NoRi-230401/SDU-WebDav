// *******************************************************
//  SDU_webDav          by NoRi 2025-04-15
// -------------------------------------------------------
// fs_util.cpp
// *******************************************************
#include "SDU_webDav.h"

bool setupNetwork();
void prt(String message);
bool wifiStart();
bool mdnsStart(void);
bool getWiFiSettings(int flType, const String filename);
void STOP();
bool FS_start(int flType);
bool SD_cardInfo(void);

// - File System Types -
#define FS_SPIFFS 1
#define FS_SD 2
// -------------------------------------------------------
// NTP connection information.
#define NTP_SVR1 "ntp.nict.jp"         // NTP server1
#define NTP_SVR2 "ntp.jst.mfeed.ad.jp" // NTP server2
#define NTP_GMT_OFFSET 9 * 3600L       // Sec  : GMT offset
#define NTP_DAYLIGHT_OFFSET 0          // Sec  : daylight offset

String SSID, SSID_PASS, HOST_NAME,IP_ADDR;
bool SD_ENABLE, SPIFFS_ENABLE;
bool SD_USE = true;
bool SPIFFS_USE = true;


bool setupNetwork()
{
  Serial.println(__FILE__);
  Serial.println(GITHUB_URL);
  Serial.println(VERSION);
  prt("-   " + PROG_NAME + "   -\n");
    
  // --- SD and SPIFFS start ---
  SD_ENABLE = false;
  if (SD_USE)
  {
    SD_ENABLE = FS_start(FS_SD);
    if (SD_ENABLE)
      prt("SD      .....  OK");
    else
      prt("SD      .....  NG");
  }

  SPIFFS_ENABLE = false;
  if (SPIFFS_USE)
  {
    SPIFFS_ENABLE = FS_start(FS_SPIFFS);
    if (SPIFFS_ENABLE)
      prt("SPIFFS  .....  OK");
    else
      prt("SPIFFS  .....  NG");
  }

  if (!SPIFFS_ENABLE && !SD_ENABLE)
  {
    prt("Both SD and SPIFFS are not available");
    return false;
  }

  // ------- Network Settings Read ---------
  SSID = "";
  SSID_PASS = "";
  HOST_NAME = "";

  if (SD_ENABLE && getWiFiSettings(FS_SD, WIFI_TXT))
    prt(" Settings read from SD");
  else if (SPIFFS_ENABLE && getWiFiSettings(FS_SPIFFS, WIFI_TXT))
    prt(" Settings read from SPIFFS");

  if (SSID == "")
    SSID = YOUR_SSID;
  prt(" SSID: " + SSID);

  if (SSID_PASS == "")
    SSID_PASS = YOUR_SSID_PASS;

  if (HOST_NAME == "")
    HOST_NAME = YOUR_HOST_NAME;

  if (SSID == "" || SSID_PASS == "" || HOST_NAME == "")
  {
    prt("SETTINGS.....  NG");
    return false;
  }

  // --- wifi and Server Start -------
  if (!wifiStart())
  {
    prt("WiFi    .....  NG");
    return false;
  }
  prt("WiFi    .....  OK");

  if (!mdnsStart())
  {
    prt("mDNS    .....  NG");
    return false;
  }
  prt("mDNS    .....  OK");

  // NTP Server config
  configTime(NTP_GMT_OFFSET, NTP_DAYLIGHT_OFFSET, NTP_SVR1, NTP_SVR2);

  // check RTC enable
  // if (RTC_ENABLE = M5.Rtc.isEnabled())
  // {
  //   Serial.println("RTC is enable");
  // }
  // else
  // {
  //   Serial.println("RTC is disable");
  //   RTC_ADJUST_ON = false;
  // }

  prt("\nIP Addr: " + IP_ADDR);
  return true;
}

void prt(String message)
{
  Serial.println(message);

  if (DISP_ON)
    M5.Display.println(message);
}

bool wifiStart()
{
  WiFi.disconnect();
  delay(500);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, SSID_PASS);
  Serial.printf(".");
  int count = 1;
  const int COUNT_MAX = 20;
  delay(500);

  while (WiFi.status() != WL_CONNECTED)
  {
    count++;
    Serial.printf(".");
    delay(500);
    if (count >= COUNT_MAX)
    {
      Serial.println("\ncannot connect ,Wifi faile!");
      return false;
    }
  }

  IP_ADDR = WiFi.localIP().toString();
  return true;
}

bool mdnsStart(void)
{
  if (!MDNS.begin(HOST_NAME.c_str()))
  {
    Serial.println("ERR: MDNS cannot start");
    Serial.println("ERR: HostName = " + HOST_NAME);
    return false;
  }

  Serial.println("mDNS HostName = " + HOST_NAME);
  return true;
}

bool getWiFiSettings(int flType, const String filename)
{
  File fs;
  if (flType == FS_SPIFFS)
  {
    if (!SPIFFS.exists(filename))
      return false;

    fs = SPIFFS.open(filename, FILE_READ);

    if (!fs)
      return false;
  }
  else if (flType == FS_SD)
  {
    if (!SD.exists(filename))
      return false;

    fs = SD.open(filename, FILE_READ);
    if (!fs)
      return false;
  }
  else
  {
    Serial.println("getWiFiSettings Err: invalid flType");
    return false;
  }

  size_t length = fs.size();
  if (length <= 3) // at least 3bytes size
    return false;

  char buf[length + 1];
  fs.read((uint8_t *)buf, length);
  buf[length] = 0;
  fs.close();

  int x;
  int y = 0;
  int z = 0;
  for (x = 0; x < length; x++)
  {
    if (buf[x] == 0x0a || buf[x] == 0x0d)
      buf[x] = 0;
    else if (!y && x > 0 && !buf[x - 1] && buf[x])
      y = x;
    else if (!z && x > 0 && !buf[x - 1] && buf[x])
      z = x;
  }

  if (y == 0)
    return false;
  SSID = String(buf);
  SSID_PASS = String(&buf[y]);
  Serial.println("SSID        = " + SSID);
  Serial.println("SSID_PASS   = " + SSID_PASS);

  if (z == 0)
    return false;
  HOST_NAME = String(&buf[z]);
  Serial.println("HOST_NAME = " + HOST_NAME);

  if (SSID == "" || SSID_PASS == "" || HOST_NAME == "")
    return false;

  return true;
}

void STOP()
{
  Serial.println(" *** Stop *** fatal error");
  SD.end();
  SPIFFS.end();
  delay(5000);

  for (;;)
  {
    delay(1000);
  }
}

bool FS_start(int flType)
{
  if (flType == FS_SPIFFS)
  {
    if (!SPIFFS.begin(true))
    {
      Serial.println("ERR: SPIFFS begin erro...");
      return false;
    }
    return true;
  }
  else if (flType == FS_SD)
  {
    if (!SD.begin(GPIO_NUM_4, SPI, 25000000))
    {
      Serial.println("ERR: SD begin erro...");
      return false;
    }

    if (!SD_cardInfo())
      return false;

    return true;
  }
  else
  {
    Serial.println("FS_start Err: invalid flType");
    return false;
  }
}

bool SD_cardInfo(void)
{
  sdcard_type_t cardType = SD.cardType();
  switch (cardType)
  {
  case CARD_MMC:
    Serial.println("MMC detected");
    break;
  case CARD_SD:
    Serial.println("SD detected");
    break;
  case CARD_SDHC:
    Serial.println("SDHC detected");
    break;
  case CARD_NONE:
    Serial.println("ERR: No SD card attached");
    return false;
  case CARD_UNKNOWN:
    Serial.println("ERR: SD card unknown Type");
    return false;
  default:
    Serial.println("ERR: SD cardType is default Type");
    return false;
  }
  return true;
}
