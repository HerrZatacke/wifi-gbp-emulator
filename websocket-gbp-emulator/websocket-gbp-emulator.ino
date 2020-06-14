#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define LED_BLINK_PIN 2
#define WIFI_BLINK_DELAY 2000
#define GB_5V_OUT 5

#define MODE_PRINT true
#define MODE_SERVE false

// #define ALTERNATE_BOOT_MODE

String dumpFileExtension = ".bin";

ESP8266WebServer server(80);

bool bootMode;

void setup() {
  fs_setup();

  Serial.begin(115200);
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
  }

  #ifndef ALTERNATE_BOOT_MODE
  if (bootMode != digitalRead(GB_5V_OUT)) {
   ESP.restart();
  }
  #endif
}
