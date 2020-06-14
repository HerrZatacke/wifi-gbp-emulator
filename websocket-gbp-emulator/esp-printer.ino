#define MISO 12
#define MOSI 13
#define SCLK 14

void processData(uint8_t data);
void storeData(uint8_t *image_data);
String nextFreeFilename();

uint8_t clock_count = 0x00;
uint8_t current_data = 0x00;
uint32_t packet_count = 0x00;
uint32_t packet_legth = 0x00;
uint8_t current_packet_type = 0x00;
bool printed = false;
uint8_t inquiry_count = 0x00;
uint8_t image_data[11520] = {};
uint32_t img_index = 0;

unsigned long blinkUpdate = 0;
bool blinkCycle = false;

void ICACHE_RAM_ATTR gbClockHit() {
  if (digitalRead(MOSI) == HIGH) {
    current_data |= 0x01;
  }

  if (packet_count == (packet_legth - 3)) {
    if (clock_count == 7) {
      digitalWrite(MISO, HIGH);
    }
  }
  if (packet_count == (packet_legth - 2)) {
    if (clock_count == 0 || clock_count == 7) {
      digitalWrite(MISO, LOW);
    } else if (clock_count == 6) {
      digitalWrite(MISO, HIGH);
    }
  }

  if (clock_count == 7) {
    processData(current_data);
    clock_count = 0;
    current_data = 0x00;
  } else {
    current_data = current_data << 1;
    clock_count++;
  }

  // Blink while receiving data
  unsigned long now = millis();
  if (blinkUpdate < now) {
    blinkUpdate = now + 50;
    if (blinkCycle == false) {
      blinkCycle = true;
    } else {
      blinkCycle = false;
    }
    digitalWrite(LED_BLINK_PIN, blinkCycle);
  }
}

void processData(uint8_t data) {
  if (packet_count == 2) { //command type
    current_packet_type = data;
    switch (data) {
      case 0x04:
      packet_legth = 0x28A; // 650 bytes
      break;

      case 0x02:
      packet_legth = 0x0E; // 14 bytes
      break;

      default:
      packet_legth = 0x0A; // 10 bytes
      break;
    }
  }

  // Handles that special empty body data packet
  if ((current_packet_type == 0x04) && (packet_count == 4) && (data == 0x00)) {
    packet_legth = 0x0A;
  }

  if ((current_packet_type == 0x04) && (packet_count >= 6) && (packet_count <= packet_legth - 5)) {
    image_data[img_index++] = data;
  }

  if (current_packet_type == 0x02) {
    printed = true;
  }

  if (printed && (packet_count == 2) && (data == 0x0F)) {
    inquiry_count++;
  }

  if (packet_count == (packet_legth - 1)) {
    packet_count = 0x00;
    if (inquiry_count == 4) {
      storeData(image_data);
    }
  } else {
    packet_count++;
  }
}

String nextFreeFilename() {
  for(int i = 1; i < 250; i++) {
    char path[31];
    sprintf(path, "/d/%05d", i);
    if(!LittleFS.exists(path + dumpFileExtension)) {
      return path + dumpFileExtension;
    }
  }
}

void storeData(uint8_t *image_data) {
  detachInterrupt(SCLK);

  String fileName = nextFreeFilename();

  Serial.print("Storing received image to ");
  Serial.println(fileName);

  File file = LittleFS.open(fileName, "w");

  if (!file) {
    Serial.println("file creation failed");
  }

  file.write("GB-BIN01", 8);
  file.write(image_data, img_index);
  file.close();

  // Serial.println("Image saved! Attaching Interrupt...");
  clock_count = 0x00;
  current_data = 0x00;
  packet_count = 0x00;
  packet_legth = 0x00;
  current_packet_type = 0x00;
  printed = false;
  inquiry_count = 0x00;
  img_index = 0x00;

  digitalWrite(LED_BLINK_PIN, false);
  attachInterrupt(SCLK, gbClockHit, RISING);
}


void espprinter_setup() {
  // Setup ports
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCLK, INPUT);

  // Setup Clock Interrupt
  attachInterrupt(SCLK, gbClockHit, RISING);
  
  blinkUpdate = millis();

  Serial.println("Printer ready.");
}
