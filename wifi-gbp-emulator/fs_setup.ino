
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
    Serial.println("---------------------\n 'pathmap.txt' found \n---------------------");
    if (FULL_PATHMAP_CHECK) {
      uint16_t index = 0;
      Serial.print("Checking missing files from 'pathmap.txt' (FULL_PATHMAP_CHECK)");
      while (mapFile.available()) {
        String line = mapFile.readStringUntil('\n');
        line.trim(); // Remove leading/trailing whitespace
        int sep = line.indexOf(';');
        if (sep > 0) {
          String mappedPath = "/w/" + line.substring(sep + 1) + ".gz";
          if(!FS.exists(mappedPath)) {
            Serial.print("\n");
            Serial.print(mappedPath);
            Serial.print("\t\t");
            Serial.println("missing");
          } else {
            if (index % 64 == 0) {
              Serial.print("\n");
            }
            Serial.print(".");
          }
        }
        index++;
      }
      mapFile.close();
      Serial.println(" done");
    }
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
  uint32_t totalBytes = fs_info.totalBytes;
  uint32_t usedBytes = fs_info.usedBytes;
  uint32_t freeBytes = totalBytes - usedBytes;
  uint32_t maxImages = freeBytes / 6144; // reserves 6kb per image
  Serial.println("------------------------");
  Serial.println("FILESYSTEM");
  Serial.print("total: ");
  Serial.println(totalBytes);
  Serial.print("used: ");
  Serial.println(usedBytes);
  Serial.print("free: ");
  Serial.println(freeBytes);
  Serial.print("images: ");
  Serial.println(maxImages);
  if (MAX_IMAGES > maxImages) {
    Serial.println("MAX_IMAGES defined in config is higher than currently available space");
  }
  Serial.println("------------------------");
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
