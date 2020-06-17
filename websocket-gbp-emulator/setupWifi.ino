
void setupWifi() {
  StaticJsonDocument<256> conf;

  File confFile = FS.open("/conf.json", "r");
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

  bool connectionBlink = false;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    digitalWrite(LED_BLINK_PIN, connectionBlink);
    connectionBlink = !connectionBlink;
    Serial.print(F("."));
  }

  Serial.println(F("\nWiFi connected"));
}
