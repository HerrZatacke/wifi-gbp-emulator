
String JsonErrorResponse (String msg) {
  return "{\"error\":\"" + msg + "\"}";
}

String wifiGetConfig() {
  StaticJsonDocument<1023> conf;
  File confFile = FS.open("/conf.json", "r");

  if (confFile) {
    DeserializationError error = deserializeJson(conf, confFile.readString());
    confFile.close();

    if (!error) {
      if (conf.containsKey("networks")) {
        JsonArray networks = conf["networks"].as<JsonArray>();
        for(JsonVariant networkSetting : networks) {
          networkSetting.remove("psk");
        }
      }

      if (conf.containsKey("ap")) {
        JsonVariant ap = conf["ap"].as<JsonVariant>();
        ap.remove("psk");
      }
    } else {
      return JsonErrorResponse("JSON deserialization error");
    }
  } else {
    return JsonErrorResponse("No conf file");
  }

  String out;
  serializeJson(conf, out);
  conf.clear();
  return out;
}

void networkUpdate(JsonArray networks, JsonObject updatedNetwork) {
  bool updated = false;
  Serial.print(updatedNetwork["ssid"].as<String>());

  for(JsonVariant networkSetting : networks) {
    if (networkSetting["ssid"].as<String>() == updatedNetwork["ssid"].as<String>()) {
      networkSetting["psk"].as<String>() = updatedNetwork["psk"].as<String>();
      Serial.println(" updated");
      updated = true;
    }
  }

  if (!updated) {
    networks.add(updatedNetwork);
    Serial.println(" added");
  }
}

void networkRemove(JsonArray networks, String ssid) {
  int index = 0;
  for(JsonVariant networkSetting : networks) {
    if (networkSetting["ssid"].as<String>() == ssid) {
      networks.remove(index);
      Serial.print(ssid);
      Serial.println(" removed from config");
    }

    index ++;
  }
}

String wifiSetConfig(String body) {
  StaticJsonDocument<1023> confUpdate;
  StaticJsonDocument<1023> conf;
  DeserializationError error = deserializeJson(confUpdate, body);

  if (error) {
    return JsonErrorResponse("cannot parse request body");
  }

  File confFile = FS.open("/conf.json", "r");

  if (confFile) {
    deserializeJson(conf, confFile.readString()); // no need to check for an error - file will be re-written anyways
    confFile.close();
  }

  if (confUpdate.containsKey("mdns")) {
    conf["mdns"] = confUpdate["mdns"].as<String>();
  }

  if (confUpdate.containsKey("ap")) {
    if (
      confUpdate["ap"].containsKey("ssid") &&
      confUpdate["ap"].containsKey("psk")
    ) {
      conf.remove("ap");
      JsonObject ap = conf.createNestedObject("ap");
      ap["ssid"] = confUpdate["ap"]["ssid"];
      ap["psk"] = confUpdate["ap"]["psk"];
    }
  }

  if (confUpdate.containsKey("networks")) {
    if (!conf.containsKey("networks")) {
      conf.createNestedArray("networks");
    }

    JsonArray networks = conf["networks"].as<JsonArray>();
    JsonArray networksUpdate = confUpdate["networks"].as<JsonArray>();

    // iterate through updated networks
    for(JsonVariant networkSetting : networksUpdate) {
      if (networkSetting.containsKey("ssid")) {
        if (networkSetting.containsKey("delete") && networkSetting["delete"].as<bool>()) {
          networkRemove(networks, networkSetting["ssid"].as<String>());
        } else if (networkSetting.containsKey("psk")) {
          networkUpdate(networks, networkSetting);
        }
      }
    }
  }

  File confFileUpdated = FS.open("/conf.json", "w");
  serializeJson(conf, confFileUpdated);
  confFileUpdated.close();
  conf.clear();
  confUpdate.clear();

  return "\"ok\"";
}
