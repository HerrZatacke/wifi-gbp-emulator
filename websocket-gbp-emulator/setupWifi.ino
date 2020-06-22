
ESP8266WiFiMulti wifiMulti;

void setupWifi() {
  StaticJsonDocument<1023> conf;
  File confFile = FS.open("/conf.json", "r");

  if (confFile) {
    DeserializationError error = deserializeJson(conf, confFile.readString());
    confFile.close();

    if (!error) {
      if (conf.containsKey("mdns")) {
        mdnsName = String(conf["mdns"].as<String>());
      }

      if (conf.containsKey("networks")) {
        JsonArray networks = conf["networks"].as<JsonArray>();
        for(JsonVariant networkSetting : networks) {
          const char *ssid = networkSetting["ssid"].as<const char*>();
          const char *password = networkSetting["psk"].as<const char*>();

          if (ssid != "null") {
            wifiMulti.addAP(ssid, password);
            hasNetworkSettings = true;
          }
        }
      }

      if (conf.containsKey("ap")) {
        accesPointSSID = String(conf["ap"]["ssid"].as<String>());
        accesPointPassword = String(conf["ap"]["psk"].as<String>());

        if (accesPointSSID == "null") {
          accesPointSSID = DEFAULT_AP_SSID;
          accesPointPassword = DEFAULT_AP_PSK;
        }
      } else {
        Serial.println("No AccessPoint settings configured - using default");
      }
    } else {
      Serial.println("Error parsing conf.json");
      Serial.println(error.c_str());
    }
  } else {
    Serial.println("Could not open conf.json");
  }

  // Connect to existing WiFi
  if (hasNetworkSettings) {
    Serial.print("Connecting to wifi ");
    WiFi.mode(WIFI_STA);

    bool connectionBlink = false;
    unsigned int connTimeout = millis() + WIFI_CONNECT_TIMEOUT;
    unsigned int connTick = 0;

    while (hasNetworkSettings && (wifiMulti.run() != WL_CONNECTED)) {
      delay(250);
      digitalWrite(LED_BLINK_PIN, connectionBlink);
      connectionBlink = !connectionBlink;
      connTick++;

      unsigned int remain = connTimeout - millis();
      if (remain <= 0 || remain > WIFI_CONNECT_TIMEOUT) {
        Serial.println("WiFi Connection timeout - starting AccesPoint");
        hasNetworkSettings = false;
      }

      if (connTick % 4 == 0) {
        Serial.print(".");
        #ifdef USE_OLED
        oled_msg("Connecting to wifi...", String(remain / 1000) + "s");
        #endif
      }
    }
  }

  // will be false if no connection to any ssid could be made
  if (hasNetworkSettings) {
    Serial.print("\nConnected to ");
    Serial.print(WiFi.SSID());
    Serial.print(" with IP address: ");
    Serial.println(WiFi.localIP());
    return;
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(accesPointSSID, accesPointPassword);
    Serial.println("AccessPoint " + accesPointSSID + " started");
  }
}

#ifdef USE_OLED
void showWifiStats(String ip, String mdnsName) {
  String protocolShort = F("IP: ");

  if(hasNetworkSettings) {
    oled_msg(
      "Connected to WiFi",
      "SSID: " + WiFi.SSID(),
      protocolShort + ip,
      mdnsName
    );
  } else {
    oled_msg(
      "AP: " + accesPointSSID,
      "PASS: " + accesPointPassword,
      protocolShort + ip,
      mdnsName
    );
  }

  oled_drawIcon();
}
#endif
