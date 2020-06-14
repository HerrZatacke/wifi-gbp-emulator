#include <uri/UriBraces.h>

// delete all stored dumps
void clearDumps() {
  Dir dumpDir = LittleFS.openDir("/d/");

  unsigned int dumpcount = 0;
  while(dumpDir.next()) {
    LittleFS.remove("/d/" + dumpDir.fileName());
    dumpcount++;
  }

  char out[24];
  sprintf(out, "{\"deleted\":%d}", dumpcount);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", out);
}

// serve list of saved dumps
void getDumpsList() {
  Dir dumpDir = LittleFS.openDir("/d/");

  FSInfo fs_info;
  LittleFS.info(fs_info);
  unsigned long total = fs_info.totalBytes;
  unsigned long used = fs_info.usedBytes;
  unsigned long avail = total - used;

  String fileName = "";
  String fileShort = "";

  // get number of files in /d/
  unsigned int dumpcount = 0;
  while(dumpDir.next()) {
    dumpcount++;
  }
  dumpDir.rewind();

  const size_t capacity = 0x1fff;
  DynamicJsonDocument doc(capacity);
  JsonArray dumps = doc.createNestedArray("dumps");
  JsonObject fs = doc.createNestedObject("fs");

  fs["total"] = total;
  fs["used"] = used;
  fs["available"] = avail;
  fs["dumpcount"] = dumpcount;

  while(dumpDir.next()) {
    dumps.add("/dumps/" + dumpDir.fileName());
  }

  String out;
  serializeJson(doc, out);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", out);
}

// stream binary dump data to web-client
void handleDump() {
  String path = "/d/" + server.pathArg(0);

  if(LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    size_t sent = server.streamFile(file, "text/plain");
    file.close();
  }

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(404, "text/plain", "REKT");
}

bool handleFileRead(String path) {
  path = "/w" + path;

  if (path.endsWith("/")) {
    path += "index.html";
  }

  String pathWithGz = path + ".gz";
  if(LittleFS.exists(pathWithGz) || LittleFS.exists(path)) {
    String contentType = getContentType(path);

    if(LittleFS.exists(pathWithGz)) {
      path += ".gz";
    }

    File file = LittleFS.open(path, "r");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }

  Serial.print(path);
  Serial.println(" - Not Found");
  return false;
}

String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

void webserver_setup() {
  server.on("/dumps/clear", clearDumps);
  server.on("/dumps/list", getDumpsList);
  server.on(UriBraces("/dumps/{}"), handleDump);
  server.onNotFound([]() {
    if (!handleFileRead(server.uri())) {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(404, "text/plain", "REKT");
    }
  });
  server.begin();
  Serial.println(F("Server started"));
}

void webserver_loop() {
  server.handleClient();
}
