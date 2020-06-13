#include <ESP8266mDNS.h>

void mdns_setup() {
  String mdnsName = F("gameboyprinter");
  if (!MDNS.begin(mdnsName)) {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");

  Serial.print(F("http://"));
  Serial.println(WiFi.localIP());

  Serial.print(F("http://"));
  Serial.println(mdnsName);

  Serial.print(F("http://"));
  Serial.print(mdnsName);
  Serial.println(F(".local"));
}

void mdns_loop() {
  MDNS.update();
}
