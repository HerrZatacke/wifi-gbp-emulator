void fs_setup() {
  LittleFS.begin();
}

void fs_info() {
  FSInfo fs_info;
  LittleFS.info(fs_info);
  LittleFS.gc();
  Serial.print("FILESYSTEM total: ");
  Serial.print(fs_info.totalBytes);
  Serial.print(" | used: ");
  Serial.println(fs_info.usedBytes);
}

#ifdef ALTERNATE_BOOT_MODE
bool fs_alternateBootMode() {
  String bootFile = "bootmode.txt";
  if (LittleFS.exists(bootFile)) {
    LittleFS.remove(bootFile);
    return false;
  } else {
    File file = LittleFS.open(bootFile, "w");
    file.write("BOOT", 4);
    file.close();
    return true;
  }
}
#endif
