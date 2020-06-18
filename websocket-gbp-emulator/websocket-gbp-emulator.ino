#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// LittleFS _seems_ 100% slower than SPIFFS in this use-case
// enable only if you think you know what you are doing
// #define FSTYPE_LITTLEFS

// Alternate boot mode lets you toggle the mode by using the reset switch
// If commented out, the signal on D0 is being used to determine mode
#define ALTERNATE_BOOT_MODE

// Number of dumps after which the printer reports to be full.
// due to performance reasons, there should always remain some free space on the Filesystem
#define MAX_DUMPS 180

#ifdef FSTYPE_LITTLEFS
#include <LittleFS.h>
#define FS LittleFS
#else
#define FS SPIFFS
#endif

#define LED_BLINK_PIN 2
#define WIFI_BLINK_DELAY 2000
#define GB_5V_OUT 5

#define MODE_PRINT true
#define MODE_SERVE false

ESP8266WebServer server(80);

bool bootMode;

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n\n\n");

  fs_setup();

  WiFi.disconnect();
  delay(1000);

  pinMode(GB_5V_OUT, INPUT);
  pinMode(LED_BLINK_PIN, OUTPUT);

  #ifdef ALTERNATE_BOOT_MODE
  bootMode = fs_alternateBootMode();
  #else
  bootMode = digitalRead(GB_5V_OUT);
  #endif

  if (bootMode == MODE_PRINT) {
    Serial.println("\n\n-----------------------");
    Serial.println("Booting in printer mode");
    Serial.println("-----------------------\n");
    digitalWrite(LED_BLINK_PIN, false);
    fs_info();
    espprinter_setup();
  } else {
    Serial.println("\n\n-----------------------");
    Serial.println("Booting in server mode");
    Serial.println("-----------------------\n");
    digitalWrite(LED_BLINK_PIN, true);
    fs_info();
    setupWifi();
    mdns_setup();
    webserver_setup();
    wifi_blink_setup();
  }
}

void loop() {
  if (bootMode == MODE_SERVE) {
    wifi_blink_loop();
    webserver_loop();
    mdns_loop();
  } else {
    espprinter_loop();
  }

  #ifndef ALTERNATE_BOOT_MODE
  if (bootMode != digitalRead(GB_5V_OUT)) {
   ESP.restart();
  }
  #endif
}
