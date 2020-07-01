#include "wifi-gbp-emulator.h"

#define VERSION "0.1.0"

// Variables used across multiple files, so they need to be defined here
String mdnsName = DEFAULT_MDNS_NAME;
String accesPointSSID = DEFAULT_AP_SSID;
String accesPointPassword = DEFAULT_AP_PSK;
bool hasNetworkSettings = false;
bool bootMode;

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n\n\n");

  #ifdef USE_OLED
  oled_setup();
  #endif

  fs_setup();

  WiFi.disconnect();
  delay(1000);

  pinMode(GB_5V_OUT, INPUT);
  pinMode(LED_BLINK_PIN, OUTPUT);

  #ifdef SENSE_BOOT_MODE
  bootMode = digitalRead(GB_5V_OUT);
  #else
  bootMode = fs_alternateBootMode();
  #endif

  if (bootMode == MODE_PRINT) {
    Serial.println("\n\n-----------------------");
    Serial.println("Booting in printer mode");
    Serial.println("-----------------------\n");
    digitalWrite(LED_BLINK_PIN, false);
    fs_info();
    espprinter_setup();
    #ifdef USE_OLED
    showPrinterStats();
    #endif
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

  #ifdef SENSE_BOOT_MODE
  if (bootMode != digitalRead(GB_5V_OUT)) {
   ESP.restart();
  }
  #endif
}
