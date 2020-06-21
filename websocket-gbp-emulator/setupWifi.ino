
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

        if (accesPointSSID != "null") {
          hasAccesPointSettings = true;
        } else {
          Serial.println("AccessPoint ssid is null");
        }
      } else {
        Serial.println("No AccessPoint settings configured");
      }
    } else {
      Serial.println("Error parsing conf.json");
      Serial.println(error.c_str());
    }
  } else {
    Serial.println("Could not open conf.json");
  }

  if (!hasNetworkSettings && !hasAccesPointSettings) {
    Serial.println("\nNo network settings found - using default AccesPoint");
    accesPointSSID = DEFAULT_AP_SSID;
    accesPointPassword = DEFAULT_AP_PSK;
    hasAccesPointSettings = true;
  }

  // Set correct wifi mode depending on configuration
  WiFiMode_t wifiMode = WIFI_OFF;
  if(hasAccesPointSettings && hasNetworkSettings) {
    wifiMode = WIFI_AP_STA;
  } else if(hasNetworkSettings) {
    wifiMode = WIFI_STA;
  } else if(hasAccesPointSettings) {
    wifiMode = WIFI_AP;
  }
  WiFi.mode(wifiMode);

  // Open AccessPoint
  if(hasAccesPointSettings) {
    WiFi.softAP(accesPointSSID, accesPointPassword);
    Serial.println("AccessPoint " + accesPointSSID + " started");
  }

  // Connect to existing WiFi
  if (hasNetworkSettings) {
    Serial.print("Connecting to wifi ");

    bool connectionBlink = false;
    unsigned int connTimeout = millis() + WIFI_CONNECT_TIMEOUT;
    unsigned int connTick = 0;

    while (hasNetworkSettings && (wifiMulti.run() != WL_CONNECTED)) {
      delay(200);
      digitalWrite(LED_BLINK_PIN, connectionBlink);
      connectionBlink = !connectionBlink;
      connTick++;

      unsigned int remain = connTimeout - millis();
      if (remain <= 0 || remain > WIFI_CONNECT_TIMEOUT) {
        hasNetworkSettings = false;
      }

      if (connTick % 5 == 0) {
        Serial.print(".");
        #ifdef USE_OLED
        oled_msg("Connecting to wifi...", String(remain / 1000) + "s");
        #endif
      }
    }

    if (!hasNetworkSettings) {
      if (!hasAccesPointSettings) {
        Serial.println("WiFi Connection timeout - starting Default AccesPoint");
        accesPointSSID = DEFAULT_AP_SSID;
        accesPointPassword = DEFAULT_AP_PSK;
        hasAccesPointSettings = true;
        WiFi.softAP(accesPointSSID, accesPointPassword);
        Serial.println("AccessPoint " + accesPointSSID + " started");
      }
    } else {
      Serial.print("\nConnected to ");
      Serial.print(WiFi.SSID());
      Serial.print(" with IP address: ");
      Serial.println(WiFi.localIP());
    }
  }
}

#ifdef USE_OLED
void showWifiStats() {
  String protocolShort = F("://");

  if(hasAccesPointSettings && hasNetworkSettings) {
    oled_msg(
      "AP: " + accesPointSSID, // + " " + protocolShort + mdnsName,
      protocolShort + WiFi.softAPIP().toString(),
      "NET: " + WiFi.SSID(),
      protocolShort + WiFi.localIP().toString()
    );
  } else if(hasAccesPointSettings) {
    oled_msg(
      "AP: " + accesPointSSID, // + " " + protocolShort + mdnsName,
      protocolShort + WiFi.softAPIP().toString()
    );
  } else if (hasNetworkSettings) {
    oled_msg(
      "NET: " + WiFi.SSID(), // + " " + protocolShort + mdnsName,
      protocolShort + WiFi.localIP().toString()
    );
  }
}
#endif
