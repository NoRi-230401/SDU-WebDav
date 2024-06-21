// *****************************************************
//  SDU-WebDav
//                          2024-06-20  by NoRi
// *****************************************************
#include "main.hpp"
#define APP_VER "SDU-WebDav-230619"
#define APP_NAME "SDU-WebDav"    // app Name
#define APP_BIN "/00_WebDav.bin" // app bin file-name
#define TIMEOUT00 5000           // lobby countdouwn timeout : msec
#define STASSID "YOUR_SSID"      // 自分のルータSSID
#define STAPSK "YOUR_SSID_PASS"  // 自分のルータPassword
#define HOSTNAME "webDav"
#define WIFI_TXT "/wifi.txt"

String SSID = "";
String SSID_PASS = "";
int SDCARD_CS_PIN = 4;
FS &gfs = SD;
WiFiServer tcp(80);
ESPWebDAV dav;

#define MD00 0  // init
#define MD01 1  // setup start
#define MD02 2  // Normal (setup end -> Loop in )
#define MD03 3  // Special Mode 

int MODE_ST = MD00;


void setup(void)
{
  MODE_ST = MD01;
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // SDUpdater Lobby Screen 
  SDCARD_CS_PIN = (int)M5.getPin(m5::sd_spi_cs);
  setupSDUpdater(APP_NAME);
  
  // Disp setup
  MD02_disp();
  prtln(String(APP_VER), D3_BOTH);

  // SD begin
  if (!SdBegin())
  {
    prtln("SD Begin fail", D3_BOTH);
    REBOOT();
  }

  // SPIFFS begin
  if (!SPIFFS.begin())
  {
    // SPIFFS is unformatted
    prtln("SPIFFS Format ... (please wait)", D3_BOTH);
    delay(100);
    SPIFFS.format();
    REBOOT();
  }

  wifi_setup();
  webDav_setup();
  ntp_setup();

  prtln("\n------------------------", D3_BOTH);
  prtln("file://" + String(HOSTNAME) + "/DavWWWRoot", D3_BOTH);
  prtln("------------------------", D3_BOTH);
  prtln("--- setup done ! ---", D1_SERI);
  MODE_ST = MD02;
}

void loop(void)
{
  M5.update();
  
  if(MODE_ST == MD02)
    MD02_BtnChk();
  else if(MODE_ST == MD03)
    MD03_BtnChk();

  dav.handleClient();
  delay(1);
}

void ntp_setup()
{
  configTime(9 * 3600, 0, "pool.ntp.org");
  struct tm timeInfo;
  if (getLocalTime(&timeInfo))
  {
    Serial.print("Local_Time:");
    Serial.printf("%04d-%02d-%02d ", timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday);
    Serial.printf("%02d:%02d:%02d\n", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);

    // M5.Display.print("Time:");
    M5.Display.printf("%04d-%02d-%02d ", timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday);
    M5.Display.printf("%02d:%02d:%02d\n", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  }
}

void setupSDUpdater(const char *appName)
{
  SDUCfg.setLabelMenu("< Menu"); // load menu.bin
  SDUCfg.setLabelSkip("Skip");   // skip the lobby countdown and run the app
  SDUCfg.setAppName(appName);    // lobby screen label: application name
  checkSDUpdater(
      SD,        // filesystem (default=SD)
      MENU_BIN,  // path to binary (default=/menu.bin, empty string=rollback only)
      TIMEOUT00, // wait delay, (default=0, will be forced to 2000 upon ESP.restart() )
      SDCARD_CS_PIN);
}

void MD02_BtnChk()
{
  if ( M5.BtnB.wasHold() )
  {
    prtln("BtnB was Hold ,  goto MD03",D1_SERI);
    MODE_ST = MD03;   // Special Mode in
    MD03_disp();
    delay(100);
  }
}


void MD03_BtnChk()
{
  if (M5.BtnA.wasClicked())
  {
    prtln("BtnA Cliked! goto Normal Mode",D1_SERI);
    MD02_disp();
    MODE_ST = MD02;  // -- normal mode
    delay(100);
  }
  else if (M5.BtnB.wasClicked())
  {
    M5.Log.println("Will Load menu binary");
    updateFromFS(SD);
    REBOOT();
  }
  // else if (M5.BtnC.wasPressed())
  else if (M5.BtnC.wasClicked())
  {
    prtln("BtnC was Clicked -> goto PowerOff",D3_BOTH);
    POWER_OFF();

    // M5.Log.println("Will store BIN_FILE to SD");
    // saveSketchToFS(SD, APP_BIN);
    // MD03_disp();
    // delay(500);
  }
}



void MD02_disp()
{
  M5.Display.setTextFont(1);
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.setTextDatum(0);
  M5.Display.setCursor(0, 0);
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextScroll(true);

  M5.Display.printf(" *** SDU-WebDav *** \n");
  M5.Display.printf("(BtnB)Hold => Mode03\n\n");
}

void MD03_disp()
{
  M5.Display.setTextFont(1);
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.setTextDatum(0);
  M5.Display.setCursor(0, 0);
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextScroll(false);
  
  M5.Display.print("*** Mode03 *** \n\n");
  M5.Display.print("(BtnA)click: exit to Normal Mode\n\n");
  M5.Display.print("(BtnB)click: load menu binary\n");
  M5.Display.print("(BtnB)hold : save bin to SD\n\n");
  M5.Display.print("(BtnC)click: Power Off\n");
}

void wifi_setup()
{
  WiFi.persistent(true);
  WiFi.setHostname(HOSTNAME);
  WiFi.mode(WIFI_STA);
  // Serial.begin(115200);
  // Serial.begin(921600);

  if (wifiTxtSDRead())
  {
    prtln("wifi.txt read success ", D1_SERI);
    prtln("SSID = " + SSID, D1_SERI);
    prtln("SSID_PASS = " + SSID_PASS, D1_SERI);
    prt("Connecting to " + String(SSID), D3_BOTH);
    WiFi.begin(SSID.c_str(), SSID_PASS.c_str());
  }
  else
  {
    prtln("wifi.txt read fail", D3_BOTH);
    REBOOT();
  }

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    prt(".", D3_BOTH);
  }
  prtln("\nConnected!", D3_BOTH);
  prtln("IP_Addr: " + WiFi.localIP().toString(), D3_BOTH);
  prtln("RSSI: " + String(WiFi.RSSI()), D1_SERI);
}


void webDav_setup()
{
  // setupSTA();

  MDNS.begin(HOSTNAME);
  tcp.begin();
  dav.begin(&tcp, &gfs);
  dav.setTransferStatusCallback([](const char *name, int percent, bool receive)
                                {
                                  Serial.printf("%s:%s:%d%%\n", receive ? "recv" : "send", name, percent);
                                  M5.Display.printf("%s:%s:%d%%\n", receive ? "recv" : "send", name, percent); });

  prtln("HOSTNAME: " + String(HOSTNAME), D3_BOTH);
  prtln("Server Started !", D1_SERI);
}

// void loopServer()
// {
//   dav.handleClient();
// }

bool SdBegin()
{
  // --- SD begin -------
  int i = 0;
  bool success = false;
  prtln("SD.begin Start",D1_SERI);

  while (i < 3)
  { // SDカードマウント待ち
    // success = SD.begin(SDCARD_CS_PIN, SPI, 15000000);
    success = SD.begin(SDCARD_CS_PIN, SPI, 4000000L);
    if (success)
      return true;

    prtln("SD Wait...",D1_SERI);
    delay(500);
    i++;
  }

  if (i >= 3)
  {
    prtln("SD.begin faile",D3_BOTH);
    return false;
  }
  else
    return true;
}

bool wifiTxtSDRead()
{
  File file = SD.open(WIFI_TXT, FILE_READ);
  if (!file)
  {
    // SD.end();
    return false;
  }

  size_t sz = file.size();
  char buf[sz + 1];
  file.read((uint8_t *)buf, sz);
  buf[sz] = 0;
  file.close();

  int y = 0;
  for (int x = 0; x < sz; x++)
  {
    if (buf[x] == 0x0a || buf[x] == 0x0d)
      buf[x] = 0;
    else if (!y && x > 0 && !buf[x - 1] && buf[x])
      y = x;
  }

  SSID = String(buf);
  SSID_PASS = String(&buf[y]);

  if ((SSID == "") || (SSID_PASS == ""))
  {
    Serial.println("Fail : ssid or passwd is void ");
    // SD.end();
    return false;
  }

  // SD.end();
  return true;
}

// #define D1_SERI 1
// #define D2_DISP 2
// #define D3_BOTH 3

void prt(String sData, int direction)
{
  switch (direction)
  {
  case D3_BOTH:
    M5.Log.print(sData.c_str());
    M5.Display.print(sData.c_str());
    break;

  case D2_DISP:
    M5.Display.print(sData.c_str());
    break;

  case D1_SERI:
    M5.Log.print(sData.c_str());
    break;

  default:
    break;
  }
}

void prtln(String sData, int direction)
{
  String strData = sData + "\n";
  prt(strData, direction);
}

void POWER_OFF()
{
  prtln("\nPOWER_OFF", D3_BOTH);
  SPIFFS.end();
  SD.end();

  delay(3000);
  M5.Power.powerOff();
  for (;;)
  {
    delay(10);
  }
}

void REBOOT()
{
  prtln("\nREBOOT", D3_BOTH);
  SPIFFS.end();
  SD.end();

  delay(3000);
  ESP.restart();
  for (;;)
  {
    delay(10);
  }
}