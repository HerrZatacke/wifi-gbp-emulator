//*

#include "FS.h"
#include "ArduinoJson.h"

void setupWifi() {
  StaticJsonDocument<256> conf;
  SPIFFS.begin();
  
  File confFile = SPIFFS.open( "/conf.json", "r"); // Datei zum lesen öffnen
  while (!confFile) {
    Serial.println("opening conf.json failed");
    delay(1250);
  }

  deserializeJson(conf, confFile.readString());
  confFile.close();

  const char* ssid = conf["STASSID"];
  const char* password = conf["STAPSK"];

  // Connect to WiFi network
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(F("."));
  }

  Serial.println(F("WiFi connected"));

  // Print the IP address
  Serial.println(WiFi.localIP());  
}
/**/
