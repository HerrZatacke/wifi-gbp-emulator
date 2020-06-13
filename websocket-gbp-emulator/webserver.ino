#include <uri/UriBraces.h>

// stream binary dump data to web-client
void printerLog_getDump(String dump) {
  String path = "/d/" + dump + ".txt";
  String content = "";
  if(LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    size_t sent = server.streamFile(file, "text/plain");
    file.close();
  }

  server.send(404, "text/plain", "REKT");
}

// serve list of saved dumps
String printerLog_getList() {
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

  const size_t capacity = JSON_ARRAY_SIZE(dumpcount + 1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4);

  DynamicJsonDocument doc(capacity);

  JsonArray dumps = doc.createNestedArray("dumps");
  JsonObject fs = doc.createNestedObject("fs");

  fs["total"] = total;
  fs["used"] = used;
  fs["available"] = avail;
  fs["dumpcount"] = dumpcount;

  while(dumpDir.next()) {
    fileName = dumpDir.fileName();
    fileShort = fileName.substring(3, fileName.length() - 4);
    dumps.add(fileShort.toInt());
  }

  String out;
  serializeJson(doc, out);
  return out;
}

void handleDump() {
  String dump = server.pathArg(0);
  if (dump == "list") {
    server.send(200, "application/json", printerLog_getList());
    return;
  }

  printerLog_getDump(dump);
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
  server.on(UriBraces("/dump/{}"), handleDump);
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "REKT");
  });
  server.begin();
  Serial.println(F("Server started"));
}

void webserver_loop() {
  server.handleClient();
}
