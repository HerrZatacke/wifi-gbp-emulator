
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

  Dir dir = SPIFFS.openDir("/w");
  while (dir.next ()) {
    Serial.print(dir.fileSize());
    Serial.print("\t\t");
    Serial.println(dir.fileName());
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
