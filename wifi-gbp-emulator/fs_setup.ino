
void fs_setup() {
  FS.begin();

  // clean a little harder
  Serial.println("Cleaning FS");
  for (int i = 0; i < 100; i++) {
    if (i % 10 == 0) {
      Serial.print(".");
    }
    FS.gc();
  }
  Serial.println(" done");


  File mapFile = FS.open("/pathmap.txt", "r");
  if (mapFile) {
    while (mapFile.available()) {
      String line = mapFile.readStringUntil('\n');
      line.trim(); // Remove leading/trailing whitespace
      int sep = line.indexOf(';');
      if (sep > 0) {
        String mappedPath = "/w/" + line.substring(sep + 1) + ".gz";
        Serial.print(mappedPath);
        Serial.print("\t\t");
        if(!FS.exists(mappedPath)) {
          Serial.println("missing");
        } else {
          Serial.println("found");
        }
      }
    }
    mapFile.close();
  } else {
    Serial.println("------------------------\n No 'pathmap.txt' found \n------------------------");
    Dir dir = SPIFFS.openDir("/w");
    while (dir.next()) {
      Serial.print(dir.fileSize());
      Serial.print("\t\t");
      Serial.println(dir.fileName());
    }
  }
}

void fs_info() {
  FSInfo fs_info;
  FS.info(fs_info);
  Serial.print("FILESYSTEM total: ");
  Serial.print(fs_info.totalBytes);
  Serial.print(" | used: ");
  Serial.println(fs_info.usedBytes);
}

#ifndef SENSE_BOOT_MODE
bool fs_alternateBootMode() {
  String bootFile = "bootmode.txt";
  if (FS.exists(bootFile)) {
    FS.remove(bootFile);
    return false;
  } else {
    File file = FS.open(bootFile, "w");
    file.write("BOOT", 4);
    file.close();
    return true;
  }
}
#endif
