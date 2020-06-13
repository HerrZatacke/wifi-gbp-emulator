
#define LED_BLINK_PIN 2
#define WIFI_BLINK_DELAY 2000
#define GB_5V_OUT 5

#define MODE_PRINT true
#define MODE_SERVE false

#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

bool mode;

void setup() {
  mode = digitalRead(GB_5V_OUT);
  Serial.begin(115200);
  pinMode(GB_5V_OUT, INPUT);
  pinMode(LED_BLINK_PIN, OUTPUT);

  LittleFS.begin();

  Serial.println("\n\n-----------------------");
  if (mode == MODE_PRINT) {
    Serial.println("Booting in printer mode");
    Serial.println("-----------------------\n");
    printerLog_setup();
    digitalWrite(LED_BLINK_PIN, false);
    gbp_emulator_setup();
  } else {
    Serial.println("Booting in server mode");
    Serial.println("-----------------------\n");
    digitalWrite(LED_BLINK_PIN, true);
    printerLog_setup();
    setupWifi();
    mdns_setup();
    webserver_setup();
    wifi_blink_setup();
  }
}

void loop() {

  if (mode == MODE_SERVE) {
    wifi_blink_loop();
    webserver_loop();
    mdns_loop();
  } else {
    gbp_emulator_loop();
  }

  if (mode != digitalRead(GB_5V_OUT)) {
   ESP.restart();
  }

  printerLog_loop();
}
