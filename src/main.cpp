// *****************************************************
//  SDU-WebDav  main.cpp           2025-04-23  by NoRi
// *****************************************************
#include "sdu_webDav.h"
#if defined(ENABLE_SD_UPDATER)
#include "SDUpdater.h"
#endif

const String PROG_NAME = "SDU-WebDav";
const String VERSION = "v102a-250423";
const String GITHUB_URL = "https://github.com/NoRi-230401/SDU-WebDav";
// -------------------------------------------------------

void webDav_begin();
// -------------------------------------------------------


//--------------------
// ***  SETTINGS  ***
//--------------------
#if defined(ENABLE_SD_SELECT)
const int fs_select = FS_SD;        //SD     -- select file system
#else
const int fs_select = FS_SPIFFS;    //SPIFFS -- select file system
#endif
bool DISP_ON = true; // 'false' if don't disp message on the display
//-------------------------------------------------------------------
const String WIFI_TXT = "/wifi.txt";
// -- write the network settings in the above file(SD or SPIFFS)  --
//           if those are no present, use in the 3-lines below.
const String YOUR_SSID = "your_wifi_ssid";
const String YOUR_SSID_PASS = "your_wifi_ssid_password";
const String YOUR_HOST_NAME = "stackchan";
//-------------------------------------------------------------------

bool SD_USE = false;
bool SPIFFS_USE = false;
FS &DAV_FS = SPIFFS;
WiFiServer tcp(80);
ESPWebDAV dav;

void setup(void)
{
  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;
  M5.begin(cfg);

#if defined(ENABLE_SD_UPDATER)
  SDU_lobby(PROG_NAME);
#else
  delay(1000); // Wait until the serial setup is complete
#endif

  M5.Display.setBrightness(120);
  M5.Lcd.setTextSize(2);

  if (fs_select == FS_SD)
  {
    SD_USE = true;
    SPIFFS_USE = false;
    DAV_FS = SD;
  }
  else if (fs_select == FS_SPIFFS)
  {
    SD_USE = false;
    SPIFFS_USE = true;
    DAV_FS = SPIFFS;
  }
  else
  {
    prt("\nERR: file system is invalid");
    STOP();
  }

  if (!setupServer())
    STOP();

  webDav_begin();
}

void loop(void)
{
  dav.handleClient();
  delay(1);
}

void webDav_begin()
{
  tcp.begin();
  dav.begin(&tcp, &DAV_FS);
  
  // dav.setTransferStatusCallback([](const char *name, int percent, bool receive)
  //                               { Serial.printf("%s:%s:%d%%\n", receive ? "recv" : "send", name, percent); });
}
