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
bool SPIFFS_begin();
void SPIFFS_start();
bool SD_begin();
void SD_start();
bool SD_cardInfo(void);
void DISP_start();

// -------------------------------------------------------
// NTP connection information.
#define NTP_SVR1 "ntp.nict.jp"         // NTP server1
#define NTP_SVR2 "ntp.jst.mfeed.ad.jp" // NTP server2
#define NTP_GMT_OFFSET 9 * 3600L       // Sec  : GMT offset
#define NTP_DAYLIGHT_OFFSET 0          // Sec  : daylight offset

String SSID, SSID_PASS, HOST_NAME, IP_ADDR;
bool SD_ENABLE, SPIFFS_ENABLE;
bool SD_USE = true;
bool SPIFFS_USE = true;

bool setupNetwork()
{
  // ------- Network Settings Read ---------
  SSID = "";
  SSID_PASS = "";
  HOST_NAME = "";

  if (SD_ENABLE && getWiFiSettings(FS_SD, WIFI_TXT))
    Serial.println(" SD Settings RD");
  else if (SPIFFS_ENABLE && getWiFiSettings(FS_SPIFFS, WIFI_TXT))
    Serial.println(" SPIFFS Settings RD");

  if (SSID == "")
    SSID = YOUR_SSID;
  Serial.println(" SSID: " + SSID);

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

  prt("IP: " + IP_ADDR);
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

bool SPIFFS_begin()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("ERR: SPIFFS begin erro...");
    return false;
  }
  return true;
}

void SPIFFS_start()
{
  SPIFFS_ENABLE = false;
  if (SPIFFS_USE)
  {
    SPIFFS_ENABLE = SPIFFS_begin();
    if (SPIFFS_ENABLE)
      prt("SPIFFS  .....  OK");
    else
      prt("SPIFFS  .....  NG");
  }
}

SPIClass SPI2;
bool SD_begin()
{
  int i;

#if defined(CARDPUTER)
  // ------------- CARDPUTER -------------
  SPI2.begin(
      M5.getPin(m5::pin_name_t::sd_spi_sclk),
      M5.getPin(m5::pin_name_t::sd_spi_miso),
      M5.getPin(m5::pin_name_t::sd_spi_mosi),
      M5.getPin(m5::pin_name_t::sd_spi_ss));

  i = 0;
  while (!SD.begin(M5.getPin(m5::pin_name_t::sd_spi_ss), SPI2) && i < 10)
  {
    delay(500);
    i++;
  }

#else
  // ----------- Core2 and CoreS3 ----------
  i = 0;
  while (!SD.begin(GPIO_NUM_4, SPI, 25000000) && i < 10)
  {
    delay(500);
    i++;
  }
#endif

  if (i >= 10)
  {
    Serial.println("ERR: SD begin erro...");
    return false;
  }

  if (!SD_cardInfo())
    return false;

  return true;
}

void SD_start()
{
  // --- SD and SPIFFS start ---
  SD_ENABLE = false;
  if (SD_USE)
  {
    SD_ENABLE = SD_begin();
    if (SD_ENABLE)
      prt("SD      .....  OK");
    else
      prt("SD      .....  NG");
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

void DISP_start()
{
  M5.Display.setBrightness(120);
  M5.Lcd.setTextSize(2);
  prt("- " + PROG_NAME + " -");
}

