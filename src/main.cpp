// *****************************************************
//  SDU-WebDav  main.cpp           2025-04-23  by NoRi
// *****************************************************
#include "sdu_webDav.h"

#if defined(ENABLE_SD_SELECT)
FS &DAV_FS = SD;
const String PROG_NAME = "WebDav SD";
#else
FS &DAV_FS = SPIFFS;
const String PROG_NAME = "WebDav SPIFFS";
#endif

const String VERSION = "v102c-250424";
const String GITHUB_URL = "https://github.com/NoRi-230401/SDU-WebDav";
// -------------------------------------------------------

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
bool DISP_ON = true;    // 'false' if don't disp message on the display
//-------------------------------------------------------------------

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

  if (!setupNetwork())
    STOP();

  tcp.begin();
  dav.begin(&tcp, &DAV_FS);

  String msg = "\n\\\\" + HOST_NAME + "\\DavWWWRoot";
  prt(msg);
  // dav.setDAVRoot(msg);
}

void loop(void)
{
  dav.handleClient();
  delay(1);
}
