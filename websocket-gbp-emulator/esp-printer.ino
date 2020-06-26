void processByte(byte data);
void storeData(byte *image_data);
unsigned int nextFreeFileIndex();

byte clock_count = 0x00;
byte current_byte = 0x00;
uint32_t packet_count = 0x00;
uint32_t packet_length = 0x00;
byte current_packet_type = 0x00;
bool printed = false;
byte inquiry_count = 0x00;
byte image_data[11520] = {};
uint32_t img_index = 0;

unsigned long lastByteReceived = 0;
unsigned long blinkClockHit = 0;
bool blinkCycle = false;

unsigned int freeFileIndex = 0;

void ICACHE_RAM_ATTR gbClockHit() {
  if (digitalRead(GB_MOSI) == HIGH) {
    current_byte |= 0x01;
  }

  if (packet_count == (packet_length - 3)) {
    if (clock_count == 7) {
      digitalWrite(GB_MISO, HIGH);
    }
  }
  if (packet_count == (packet_length - 2)) {
    if (clock_count == 0 || clock_count == 7) {
      digitalWrite(GB_MISO, LOW);
    } else if (clock_count == 6) {
      digitalWrite(GB_MISO, HIGH);
    }
  }

  if (clock_count == 0) {
    // Blink while receiving data
    lastByteReceived = millis();
    if (blinkClockHit < lastByteReceived) {
      blinkClockHit = lastByteReceived + 50;
      blinkCycle = !blinkCycle;
      digitalWrite(LED_BLINK_PIN, blinkCycle);
    }
  }

  if (clock_count == 7) {
    processByte(current_byte);
    clock_count = 0;
    current_byte = 0x00;
  } else {
    current_byte = current_byte << 1;
    clock_count++;
  }
}

void processByte(byte data) {
  if (packet_count == 2) { //command type
    current_packet_type = data;
    switch (data) {
      case 0x04:
      packet_length = 0x28A; // 650 bytes
      break;

      case 0x02:
      packet_length = 0x0E; // 14 bytes
      break;

      default:
      packet_length = 0x0A; // 10 bytes
      break;
    }
  }

  // Handles that special empty body data packet
  if ((current_packet_type == 0x04) && (packet_count == 4) && (data == 0x00)) {
    packet_length = 0x0A;
  }

  if ((current_packet_type == 0x04) && (packet_count >= 6) && (packet_count <= packet_length - 5)) {
    image_data[img_index++] = data;
  }

  if (current_packet_type == 0x02) {
    printed = true;
  }

  if (printed && (packet_count == 2) && (data == 0x0F)) {
    inquiry_count++;
  }

  if (packet_count == (packet_length - 1)) {
    packet_count = 0x00;
    if (inquiry_count == 4) {
      storeData(image_data);
    }
  } else {
    packet_count++;
  }
}

unsigned int nextFreeFileIndex() {
  for (int i = 1; i < MAX_IMAGES; i++) {
    char path[31];
    sprintf(path, "/d/%05d.bin", i);
    if (!FS.exists(path)) {
      return i;
    }
  }
  return MAX_IMAGES + 1;
}

void resetValues() {
  clock_count = 0x00;
  current_byte = 0x00;
  packet_count = 0x00;
  packet_length = 0x00;
  current_packet_type = 0x00;
  printed = false;
  inquiry_count = 0x00;
  img_index = 0x00;

  lastByteReceived = 0;

  // Turn LED ON
  digitalWrite(LED_BLINK_PIN, false);
  Serial.println("Printer ready.");
}

void storeData(byte *image_data) {
  detachInterrupt(GB_SCLK);

  unsigned long perf = millis();
  char fileName[31];
  sprintf(fileName, "/d/%05d.bin", freeFileIndex);

  File file = FS.open(fileName, "w");

  if (!file) {
    Serial.println("file creation failed");
  }

  file.write("GB-BIN01", 8);
  file.write(image_data, img_index);
  file.close();

  perf = millis() - perf;
  Serial.printf("File /d/%05d.bin written in %lums\n", freeFileIndex, perf);

  freeFileIndex++;

  if (freeFileIndex <= MAX_IMAGES) {
    attachInterrupt(GB_SCLK, gbClockHit, RISING);
  } else {
    Serial.println("no more space on printer\nrebooting...");
    ESP.restart();
  }
}

// Blink if printer is full.
void full() {
  Serial.println("no more space on printer");
  while(true) {
    digitalWrite(LED_BLINK_PIN, LOW);
    delay(1000);
    digitalWrite(LED_BLINK_PIN, HIGH);
    delay(500);
  }
}

void espprinter_setup() {
  // Setup ports
  pinMode(GB_MISO, OUTPUT);
  pinMode(GB_MOSI, INPUT);
  pinMode(GB_SCLK, INPUT);

  freeFileIndex = nextFreeFileIndex();

  if (freeFileIndex <= MAX_IMAGES) {
    Serial.printf("Next file: /d/%05d.bin\n", freeFileIndex);
  } else {
    full();
  }

  resetValues();

  // Setup Clock Interrupt
  attachInterrupt(GB_SCLK, gbClockHit, RISING);
}


#ifdef USE_OLED
void showPrinterStats() {
  char printed[20];
  char remain[20];
  sprintf(printed, "%3d printed", freeFileIndex - 1);
  sprintf(remain, "%3d remaining", MAX_IMAGES + 1 - freeFileIndex);
  oled_msg(
    printed,
    remain
  );
  oled_drawLogo();
}
#endif

void espprinter_loop() {
  if (lastByteReceived != 0 && lastByteReceived + 500 < millis()) {
    resetValues();
    #ifdef USE_OLED
    showPrinterStats();
    #endif
  }
}
