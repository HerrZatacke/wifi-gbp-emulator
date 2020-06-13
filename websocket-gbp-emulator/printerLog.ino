#define PRINTER_LOG_IDLE_DELAY 5000

String dump = "";

unsigned long printerLog_nextMillis = 0;

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
  printerLog_resetBuffer();
}

void printerLog_add(String line) {
  dump += line + "\n";
  printerLog_nextMillis = millis() + PRINTER_LOG_IDLE_DELAY;
}

void printerLog_finalize() {

  if (dump.length() < 1000) {
    printerLog_resetBuffer();
    return;
  }

  // String fName = "/d/" + millis() + ".txt";
  String fName = "/w/dump.txt";
  File dumpFile = LittleFS.open(fName, "w");;
  if(!dumpFile){
    Serial.println("Error opening " + fName);
    return;
  }

  if(dumpFile.print(dump)){
    Serial.println("File was written");
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
