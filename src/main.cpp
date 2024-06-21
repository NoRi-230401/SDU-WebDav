// *****************************************************
//  SDU-WebDav  main.cpp           2024-06-20  by NoRi
// *****************************************************
#include "main.h"

#define APP_VER "SDU-WebDavV101-230621"
#define APP_NAME "SDU-WebDav"    // app Name
#define APP_BIN "/00_WebDav.bin" // app bin file-name
#define TIMEOUT00 5000           // lobby countdouwn timeout : msec
#define HOSTNAME "webDav"
#define WIFI_TXT "/wifi.txt"

String SSID = "";
String SSID_PASS = "";
int SDCARD_CS_PIN = 4;
FS &gfs = SD;
WiFiServer tcp(80);
ESPWebDAV dav;

#define MDM2 -2     // init
#define MDM1 -1     // setup start
#define MD00 0      // setup don (Normal mode)
#define MD01 1      // MD01  FileSystem change
#define MD02 2      // MD02  load menu.bin
#define MD03 3      // MD03  save bin to SD
#define MD04 4      // MD04  power off
#define MD_END 4    //
int MODE_ST = MDM2; // mode status

#define FS_SD 1
#define FS_SPIFFS 2
int WD_FILE_SYSTEM = FS_SD;
const char *SETTING_NVS = "setting"; // setting --NVS の設定用ファイル

void setup(void)
{
  MODE_ST = MDM1;
  auto cfg = M5.config();
  M5.begin(cfg);

  // SDUpdater Lobby Screen
  SDCARD_CS_PIN = (int)M5.getPin(m5::sd_spi_cs);
  setupSDUpdater(APP_NAME);

  // Disp setup
  MD00_disp();
  prtln(String(APP_VER), D1_SERI);

  // SD begin
  if (!SdBegin())
  {
    prtln("SD Begin fail", D3_BOTH);
    REBOOT();
  }

  // wifi.txt read
  if (wifiTxtSDRead())
  {
    prtln("wifi.txt read success ", D1_SERI);
    prtln("SSID = " + SSID, D1_SERI);
    prtln("SSID_PASS = " + SSID_PASS, D1_SERI);
  }
  else
  {
    prtln("wifi.txt read fail", D3_BOTH);
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

  // File System Setting Read from NVM
  WD_FILE_SYSTEM = getFileSystemNVM();
  if (WD_FILE_SYSTEM == FS_SPIFFS)
  {
    gfs = SPIFFS;
    prtln("FileSystem is SPIFFS", D3_BOTH);
  }
  else
  {
    gfs = SD;
    prtln("FileSystem is SD", D3_BOTH);
  }
  setFileSystemNVM(WD_FILE_SYSTEM);

  // network setup
  wifi_setup();
  webDav_setup();
  ntp_setup();

  prtln("\n------------------------", D3_BOTH);
  prtln("  //" + String(HOSTNAME) + "/DavWWWRoot", D3_BOTH);
  prtln("------------------------", D3_BOTH);
  prtln("--- setup done ! ---", D1_SERI);
  MODE_ST = MD00;
}

void loop(void)
{
  M5.update();
  BtnChk();

  dav.handleClient();
  delay(1);
}

void BtnChk()
{
  if (MODE_ST == MD00)
    MD00_BtnChk();
  else if ((MODE_ST > MD00) && (MODE_ST <= MD_END))
    MDxx_BtnChk();
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

void MD00_BtnChk()
{
  if (M5.BtnB.wasHold())
  {
    prtln("BtnB was Hold ,  goto Special Mode", D1_SERI);
    MODE_ST = MD01; // Special Mode in
    MDxx_disp(MODE_ST);
    delay(100);
  }
}

void MDxx_BtnChk()
{
  if (M5.BtnA.wasClicked())
  {
    prtln("BtnA Cliked! [EXIT]", D1_SERI);
    MD00_disp();
    MODE_ST = MD00; // -- normal mode
    delay(100);
  }
  else if (M5.BtnB.wasClicked())
  {
    prtln("BtnB Cliked!  [OK]", D1_SERI);
    doWork(MODE_ST);
    MDxx_disp(MODE_ST);
  }
  else if (M5.BtnC.wasClicked())
  {
    prtln("BtnC Cliked!  [NEXT]", D1_SERI);
    MODE_ST++;
    if (MODE_ST > MD_END)
      MODE_ST = MD01;

    MDxx_disp(MODE_ST);
  }
}

void doWork(int mode)
{
  switch (mode)
  {
  case MD01:
    M5.Log.println("FileSystem change");
    if (WD_FILE_SYSTEM == FS_SD)
      setFileSystemNVM(FS_SPIFFS);
    else
      setFileSystemNVM(FS_SD);

    REBOOT();
    break;

  case MD02:
    M5.Log.println("Will Load menu binary");
    updateFromFS(SD);
    for (;;)
    {
      delay(10);
    }
    // REBOOT();
    break;

  case MD03:
    M5.Log.println("Will store BIN_FILE to SD");
    saveSketchToFS(SD, APP_BIN);
    delay(500);
    break;

  case MD04:
    POWER_OFF();
    break;

  default:
    break;
  }
}

void MD00_disp()
{
  M5.Display.setTextFont(1);
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.setTextDatum(0);
  M5.Display.setCursor(0, 0);
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextScroll(true);
  M5.Display.printf("****   SDU-WebDav   ****\n");
  M5.Display.printf("(BtnB)hold: Special Mode\n\n");
}

void MDxx_disp(int mode)
{
  M5.Display.setTextFont(1);
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLUE);
  M5.Display.fillScreen(BLUE);
  M5.Display.setTextDatum(0);
  M5.Display.setCursor(0, 0);
  M5.Display.setTextScroll(false);

  M5.Display.printf("**  Special Mode %d/%d  **\n\n", MODE_ST, MD_END);
  String msg = "";
  switch (MODE_ST)
  {
  case MD01:
    if (WD_FILE_SYSTEM == FS_SD)
      msg = "FileSystem SD -> SPIFFS";
    else
      msg = "FileSystem SPIFFS -> SD";

    break;

  case MD02:
    msg = "Load SD-Updater menu.bin";
    break;

  case MD03:
    msg = "Store bin-file to SD";
    break;

  case MD04:
    msg = "Power Off";
    break;

  default:
    break;
  }
  prtln(msg, D2_DISP);

  M5.Display.printf("\n\n\n(BtnA)click:    EXIT\n\n");
  M5.Display.printf("(BtnB)click:    OK\n\n");
  M5.Display.print("(BtnC)click:    NEXT\n");
}

void wifi_setup()
{
  WiFi.persistent(true);
  WiFi.setHostname(HOSTNAME);
  WiFi.mode(WIFI_STA);

  prt("Connecting to " + String(SSID), D3_BOTH);
  WiFi.begin(SSID.c_str(), SSID_PASS.c_str());

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

bool SdBegin()
{
  // --- SD begin -------
  int i = 0;
  bool success = false;
  prtln("SD.begin Start", D1_SERI);

  while (i < 3)
  { // SDカードマウント待ち
    // success = SD.begin(SDCARD_CS_PIN, SPI, 15000000);
    success = SD.begin(SDCARD_CS_PIN, SPI, 4000000L);
    if (success)
      return true;

    prtln("SD Wait...", D1_SERI);
    delay(500);
    i++;
  }

  if (i >= 3)
  {
    prtln("SD.begin faile", D3_BOTH);
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
    return false;
  }

  return true;
}

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

void setFileSystemNVM(int flSys)
{
  size_t tmp_flSys;

  if ((flSys < FS_SD) || (flSys > FS_SPIFFS))
    tmp_flSys = FS_SD;
  else
    tmp_flSys = flSys;

  if (tmp_flSys == FS_SD)
    prtln("FileSyste set to SD", D1_SERI);
  if (tmp_flSys == FS_SPIFFS)
    prtln("FileSyste set to SPIFFS", D1_SERI);

  uint32_t nvs_handle;
  if (ESP_OK == nvs_open(SETTING_NVS, NVS_READWRITE, &nvs_handle))
    nvs_set_u32(nvs_handle, "wdFlSys", tmp_flSys);
  nvs_close(nvs_handle);
}

int getFileSystemNVM()
{
  uint32_t nvs_handle;
  size_t getFlSys = FS_SD;

  if (ESP_OK == nvs_open(SETTING_NVS, NVS_READONLY, &nvs_handle))
    nvs_get_u32(nvs_handle, "wdFlSys", &getFlSys);
  nvs_close(nvs_handle);

  if (getFlSys < FS_SD || getFlSys > FS_SPIFFS)
    getFlSys = FS_SD;

  return (int)getFlSys;
}
