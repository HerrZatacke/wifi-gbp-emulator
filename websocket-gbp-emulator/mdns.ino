#include <ESP8266mDNS.h>

void mdns_setup() {
  if (!MDNS.begin(mdnsName)) {
    Serial.println("Error setting up MDNS responder!");
  }

  Serial.println("mDNS responder started");

  String protocol = F("http://");
  String ip = "";
  String local = F(".local");

  if (hasNetworkSettings) {
    ip = WiFi.localIP().toString();
  } else {
    ip = WiFi.softAPIP().toString();
  }

  Serial.println(protocol + ip);
  Serial.println(protocol + mdnsName);
  Serial.println(protocol + mdnsName + local);

  #ifdef USE_OLED
  showWifiStats(ip, mdnsName + local);
  #endif
}

void mdns_loop() {
  MDNS.update();
}
