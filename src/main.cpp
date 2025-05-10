// *****************************************************
//  SDU-WebDav  main.cpp           2025-04-23  by NoRi
// *****************************************************
#include "SDU_webDav.h"
#include "SDUpdater.h"

#if defined(CARDPUTER)
#include <M5Cardputer.h>
#endif

const String VERSION = "v105-250510";
const String GITHUB_URL = "https://github.com/NoRi-230401/SDU-WebDav";
// -------------------------------------------------------

#if defined(ENABLE_SD_SELECT)
FS &DAV_FS = SD;
const String PROG_NAME = "WebDav SD";
#else
FS &DAV_FS = SPIFFS;
const String PROG_NAME = "WebDav SPIFFS";
#endif

// ******************
// *   SETTINGS     *
// ******************
const String WIFI_TXT = "/wifi.txt";
// - write the network settings in the above file(SD or SPIFFS) -
//      if those are no present, use in the 3-lines below.
const String YOUR_SSID = "your_wifi_ssid";
const String YOUR_SSID_PASS = "your_wifi_ssid_password";
const String YOUR_HOST_NAME = "stackchan";
//-------------------------------------------------------------------
bool DISP_ON = true; // 'false' if don't disp message on the display
//-------------------------------------------------------------------

WiFiServer tcp(80);
ESPWebDAV dav;

void setup(void)
{
  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;

// ---- CARDPUTER ---------------
#if defined(CARDPUTER)
  M5Cardputer.begin(cfg, true);
  DISP_start();
  SD_start();
#if defined(ENABLE_SD_UPDATER)
  SDU_lobby_cardputer();
#endif

// ---- Core2 CoreS3 -------------
#else
  M5.begin(cfg);
#if defined(ENABLE_SD_UPDATER)
  SDU_lobby(PROG_NAME);
#endif
  DISP_start();
  SD_start();
#endif // end of CARDPUTER

  SPIFFS_start();

  if (!setupNetwork())
    STOP();

  tcp.begin();
  dav.begin(&tcp, &DAV_FS);

  String msg = "\\\\" + HOST_NAME + "\\DavWWWRoot";
  prt(msg);
}

void loop(void)
{
  dav.handleClient();
  delay(1);
}
