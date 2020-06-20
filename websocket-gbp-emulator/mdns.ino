#include <ESP8266mDNS.h>

void mdns_setup() {
  String mdnsName = F("gameboyprinter");
  if (!MDNS.begin(mdnsName)) {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");

  String protocol = F("http://");
  String ip = WiFi.localIP().toString();
  String local = F(".local");

  Serial.println(protocol + ip);
  Serial.println(protocol + mdnsName);
  Serial.println(protocol + mdnsName + local);

  #ifdef USE_OLED
  oled_setup();
  oled_msg("Server mode\n" + protocol + ip, SPACE_TWO_LINES);
  #endif
}

void mdns_loop() {
  MDNS.update();
}
