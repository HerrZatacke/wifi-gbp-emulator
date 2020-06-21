#include <ESP8266mDNS.h>

void mdns_setup() {
  if (!MDNS.begin(mdnsName)) {
    Serial.println("Error setting up MDNS responder!");
  }

  Serial.println("mDNS responder started");

  String protocol = F("http://");
  String nwip = WiFi.localIP().toString();
  String apip = WiFi.softAPIP().toString();
  String local = F(".local");

  if (hasAccesPointSettings) {
    Serial.println(protocol + apip);;
  }

  if (hasNetworkSettings) {
    Serial.println(protocol + nwip);
  }

  Serial.println(protocol + mdnsName);
  Serial.println(protocol + mdnsName + local);

  #ifdef USE_OLED
  showWifiStats();
  #endif
}

void mdns_loop() {
  MDNS.update();
}
