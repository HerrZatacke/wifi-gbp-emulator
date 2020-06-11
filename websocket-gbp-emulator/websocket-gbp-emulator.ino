
#define LED_BLINK_PIN 2
#define WIFI_BLINK_DELAY 2000

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

WebSocketsServer webSocket(81); 

void setup() {
  Serial.begin(115200);
  setupWifi();
  startWebSocket();
  gbp_emulator_setup();
  wifi_blink_setup();
}

void loop() {
  webSocket.loop();
  gbp_emulator_loop();
  wifi_blink_loop();
}
