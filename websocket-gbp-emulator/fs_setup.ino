void fs_setup() {
  LittleFS.begin();
  FSInfo fs_info;
  LittleFS.info(fs_info);
  LittleFS.gc();
  Serial.print("FILESYSTEM total: ");
  Serial.print(fs_info.totalBytes);
  Serial.print(" | used: ");
  Serial.println(fs_info.usedBytes);
}
