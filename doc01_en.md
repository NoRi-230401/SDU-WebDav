# SDU-WebDav Project Source Code Explanation

The files in this project (`SDU_webDav.h`, `main.cpp`, `SDUpdater.cpp`, `fs_util.cpp`) constitute the source code for "SDU-WebDav," a WebDAV server project designed to run on ESP32 devices, particularly M5Stack devices. It allows users to access the device's file system (SD card or SPIFFS) via WiFi.

## 1. `SDU_webDav.h` (Header File)

*   **Role:** Defines settings, includes libraries, and declares functions and variables commonly used throughout the project.
*   **Key Contents:**
    *   **Include Guards (`#ifndef`, `#define`, `#endif`):** Prevents the header file from being included multiple times.
    *   **Library Includes:**
        *   `Arduino.h`, `M5Unified.h`: Basic libraries for the Arduino framework and M5Stack devices.
        *   `WiFi.h`, `ESPmDNS.h`: Libraries for WiFi connectivity and mDNS (hostname access) functionality.
        *   `SD.h`, `SPIFFS.h`, `LITTLEFS.h`: Libraries for handling file systems like SD card, SPIFFS, and LITTLEFS.
        *   `time.h`: Library for time-related functions.
        *   `ESPWebDAV.h`: Core library providing WebDAV server functionality.
    *   **External Function Declarations (`extern`):** Declares functions defined in other files (`fs_util.cpp`, specifically `setupNetwork`, `STOP`, `prt`) so they can be called from files that include this header.
    *   **External Variable Declarations (`extern`):** Declares global variables shared across the project (`PROG_NAME`, `VERSION`, `GITHUB_URL`, WiFi setting variables, etc.), allowing access from different `.cpp` files.

## 2. `main.cpp` (Main Program File)

*   **Role:** Contains the program's entry point (`setup` function) and main loop (`loop` function). Handles overall initialization, starts the WebDAV server, and processes client requests.
*   **Key Contents:**
    *   **Includes:** Includes `SDU_webDav.h` (common definitions) and `SDUpdater.h` (SD card update functionality).
    *   **Constant Definitions:** Defines the program version (`VERSION`) and the GitHub repository URL (`GITHUB_URL`).
    *   **File System Selection (`#if defined(ENABLE_SD_SELECT)`):**
        *   Uses a compile-time option (`ENABLE_SD_SELECT`) to switch the file system used by the WebDAV server between the SD card (`SD`) and SPIFFS (`SPIFFS`).
        *   The program name (`PROG_NAME`) is also set according to the selected file system.
    *   **Settings (`SETTINGS`):**
        *   `WIFI_TXT`: Defines the filename for the WiFi configuration file (SSID, password, hostname). If this file exists on the SD card or SPIFFS, its settings are prioritized.
        *   `YOUR_SSID`, `YOUR_SSID_PASS`, `YOUR_HOST_NAME`: Defines default WiFi settings and hostname used if `wifi.txt` is not found.
        *   `DISP_ON`: A flag to determine whether messages should be displayed on the M5Stack's screen.
    *   **Global Objects:**
        *   `WiFiServer tcp(80)`: Initializes the TCP server used by WebDAV on port 80.
        *   `ESPWebDAV dav`: Creates an instance of the WebDAV server.
    *   **`setup()` Function:**
        *   Initializes the M5Stack device (`M5.begin()`).
        *   If `ENABLE_SD_UPDATER` is defined, calls `SDU_lobby()` for the SD card firmware update feature.
        *   Sets the display brightness and text size.
        *   Calls the `setupNetwork()` function (from `fs_util.cpp`) to initialize the file system, load WiFi settings, connect to WiFi, start mDNS, etc. Calls `STOP()` if setup fails.
        *   Starts the TCP server (`tcp.begin()`) and the WebDAV server (`dav.begin()`), passing the selected file system (`DAV_FS`) to the WebDAV server.
        *   Prints the network path for connection (format: `\\HOSTNAME\DavWWWRoot`) to the serial monitor and display.
    *   **`loop()` Function:**
        *   Repeatedly calls `dav.handleClient()` to handle incoming WebDAV client connection requests and file operation requests.
        *   Includes a short `delay(1)` to yield CPU time to other tasks (like background WiFi processing).

## 3. `SDUpdater.cpp` (SD Card Updater Related File)

*   **Role:** Provides functionality to update the program from a firmware file (`.bin`) stored on the SD card, utilizing the M5StackUpdater library. This code is only compiled and used if the `ENABLE_SD_UPDATER` macro is defined.
*   **Key Contents:**
    *   **Conditional Compilation (`#if defined(ENABLE_SD_UPDATER)`):** The content of this file is only active when `ENABLE_SD_UPDATER` is defined.
    *   **Includes:** Includes `Arduino.h`, `M5Unified.h`, `M5StackUpdater.h`, and `SDUpdater.h`.
    *   **Constant Definition:** `SDU_SKIP_TMR` defines the time (in ms) before the updater screen is automatically skipped.
    *   **`SDU_lobby()` Function:**
        *   Called from the `setup()` function in `main.cpp`.
        *   Uses `SDUCfg` to set the application name and menu button labels displayed on the updater screen.
        *   Calls the `checkSDUpdater()` function to check for a specified binary file (`MENU_BIN`) on the SD card and initiates the update process if necessary.

## 4. `fs_util.cpp` (File System & Network Utility File)

*   **Role:** Consolidates utility functions related to network and file system operations, such as initializing file systems (SD card, SPIFFS), loading WiFi settings, establishing WiFi connection, starting mDNS, synchronizing time via NTP, printing messages, and handling errors. The central function `setupNetwork()` is called from `main.cpp`'s `setup()`.
*   **Key Contents:**
    *   **Includes:** Includes `SDU_webDav.h`.
    *   **Function Prototypes:** Declares functions defined within this file.
    *   **Constant Definitions:** Defines constants representing file system types (`FS_SPIFFS`, `FS_SD`) and NTP server addresses.
    *   **Global Variables:** Holds WiFi settings (`SSID`, `SSID_PASS`, `HOST_NAME`), IP address (`IP_ADDR`), and file system availability status (`SD_ENABLE`, `SPIFFS_ENABLE`).
    *   **`setupNetwork()` Function:**
        *   Displays the program name, version, and GitHub URL.
        *   Calls `FS_start()` to attempt initialization of the SD card and SPIFFS.
        *   Calls `getWiFiSettings()` to read WiFi settings from `wifi.txt` on the SD card or SPIFFS. Uses default values from `main.cpp` if the file is not found.
        *   Calls `wifiStart()` to connect to the WiFi network.
        *   Calls `mdnsStart()` to start the mDNS service, enabling access via hostname.
        *   Calls `configTime()` to connect to an NTP server and set the device's time.
        *   Displays the final IP address.
        *   Returns `false` if any error occurs during setup.
    *   **`prt()` Function:** Prints messages to both the serial monitor and the M5Stack display (if `DISP_ON` is `true`).
    *   **`wifiStart()` Function:** Handles the WiFi connection process. Retries connection and returns `false` on timeout.
    *   **`mdnsStart()` Function:** Starts mDNS and registers the specified hostname on the network.
    *   **`getWiFiSettings()` Function:** Reads `wifi.txt` from the specified file system (SD or SPIFFS), parses its content, and stores the values in `SSID`, `SSID_PASS`, and `HOST_NAME` variables. Returns `false` if the file doesn't exist or the content is invalid.
    *   **`STOP()` Function:** Called on fatal errors. Stops file systems, displays an error message, and enters an infinite loop to halt the program.
    *   **`FS_start()` Function:** Initializes the specified file system (SD or SPIFFS). For SD cards, it also checks card information using `SD_cardInfo()`.
    *   **`SD_cardInfo()` Function:** Detects the type of the connected SD card and prints information to the serial monitor. Returns `false` if the card cannot be recognized.

## Summary

These files work together to implement a WebDAV server on an M5Stack device. `main.cpp` controls the overall flow, `fs_util.cpp` handles preparatory tasks like network connection and file system setup, `SDU_webDav.h` provides common definitions and declarations, and `SDUpdater.cpp` optionally adds the SD card firmware update feature. This allows users to easily access the M5Stack's file system via WiFi.
