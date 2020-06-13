#define PRINTER_LOG_IDLE_DELAY 5000

String dump = "";

unsigned long printerLog_nextMillis = 0;

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

void printerLog_resetBuffer() {
  if (dump.length() == 0) {
    return;
  }

  dump = "";
  // maximum filesize of a horizontal panorama print (which should be the max required filesize)
  dump.reserve(0x12fff);
  printerLog_nextMillis = millis() + PRINTER_LOG_IDLE_DELAY;
}

void printerLog_setup() {
  FSInfo fs_info;
  LittleFS.info(fs_info);
  LittleFS.gc();
  Serial.print("FILESYSTEM total: ");
  Serial.print(fs_info.totalBytes);
  Serial.print(" | used: ");
  Serial.println(fs_info.usedBytes);
  printerLog_resetBuffer();
}

void printerLog_add(String line) {
  dump += line + "\n";
  Serial.println(dump.length());
  printerLog_nextMillis = millis() + PRINTER_LOG_IDLE_DELAY;
}

String nextFreeFilename() {
  for(int i = 1; i < 200; i++) {
    char path[31];
    sprintf(path, "/d/%d.txt", i);
    if(!LittleFS.exists(path)) {
      return path;
    }
  }
}

void printerLog_finalize() {

  if (dump.length() < 1000) {
    Serial.println("wont save");
    printerLog_resetBuffer();
    return;
  }

  Serial.println("will save");

  String fileName = nextFreeFilename();

  File dumpFile = LittleFS.open(fileName, "w");
  if(!dumpFile){
    Serial.print("Error opening " + fileName);
    return;
  }

  if(dumpFile.print(dump)){
    Serial.print(fileName + " written");
    dumpFile.close();
  } else {
    Serial.println("File write failed");
  };

  printerLog_resetBuffer();
}

void printerLog_loop() {
  unsigned long currentMillis = millis();
  if (currentMillis >= printerLog_nextMillis) {
    printerLog_resetBuffer();
  }
}
