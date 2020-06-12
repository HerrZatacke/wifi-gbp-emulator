#include <ESP8266mDNS.h>

void mdns_setup() {
  if (!MDNS.begin("gameboyprinter")) {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
}


void mdns_loop() {
  MDNS.update();
}
