#include <uri/UriBraces.h>

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
