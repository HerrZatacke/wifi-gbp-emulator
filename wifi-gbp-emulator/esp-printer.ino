#include <stdint.h> // uint8_t
#include <stddef.h> // size_t

#include "gameboy_printer_protocol.h"
#include "gbp_serial_io.h"

unsigned int nextFreeFileIndex();
unsigned int freeFileIndex = 0;
File file;
uint32_t writeFrom = 0x00;
uint32_t writeTo = 0x00;

String cmdPRNT="";
int chkHeader=99;

byte image_data[30000] = {};
uint32_t img_index=0x00;

// Dev Note: Gamboy camera sends data payload of 640 bytes usually
#define GBP_BUFFER_SIZE 650

/* Serial IO */
// This circular buffer contains a stream of raw packets from the gameboy
uint8_t gbp_serialIO_raw_buffer[GBP_BUFFER_SIZE] = {0};

inline void gbp_packet_capture_loop();

/*******************************************************************************
  Utility Functions
*******************************************************************************/

const char *gbpCommand_toStr(int val) {
  switch (val) {
    case GBP_COMMAND_INIT    : return "INIT";
    case GBP_COMMAND_PRINT   : return "PRNT";
    case GBP_COMMAND_DATA    : return "DATA";
    case GBP_COMMAND_BREAK   : return "BREK";
    case GBP_COMMAND_INQUIRY : return "INQY";
    default: return "?";
  }
}

/*******************************************************************************
  Interrupt Service Routine
*******************************************************************************/

#ifdef ESP8266
void ICACHE_RAM_ATTR serialClock_ISR(void)
#else
void serialClock_ISR(void)
#endif
{
  // Serial Clock (1 = Rising Edge) (0 = Falling Edge); Master Output Slave Input (This device is slave)
#ifdef GBP_FEATURE_USING_RISING_CLOCK_ONLY_ISR
  const bool txBit = gpb_serial_io_OnRising_ISR(digitalRead(GB_MOSI));
#else
  const bool txBit = gpb_serial_io_OnChange_ISR(digitalRead(GB_SCLK), digitalRead(GB_MOSI));
#endif
  digitalWrite(GB_MISO, txBit ? HIGH : LOW);
}

unsigned int nextFreeFileIndex() {
  for (int i = 1; i < MAX_IMAGES; i++) {
    char path[31];
    sprintf(path, "/d/%05d.txt", i);
    if (!FS.exists(path)) {
      return i;
    }
  }
  return MAX_IMAGES + 1;
}

void resetValues() {
  img_index = 0x00;
  writeFrom = 0x00;
  writeTo = 0x00;

  file.close();

  // ToDo: Handle percentages
  //int percUsed = fs_info();
  // if (percUsed > 5) {
  //   resetValues();
  // } else {
  //   Serial.println("no more space on printer\nrebooting...");
  //   ESP.restart();
  // }

  // /* Attach ISR Again*/
  // #ifdef GBP_FEATURE_USING_RISING_CLOCK_ONLY_ISR
  //   attachInterrupt( digitalPinToInterrupt(GB_SCLK), serialClock_ISR, RISING);  // attach interrupt handler
  // #else
  //   attachInterrupt( digitalPinToInterrupt(GB_SCLK), serialClock_ISR, CHANGE);  // attach interrupt handler
  // #endif

  Serial.println("a");

  memset(image_data, 0x00, sizeof(image_data));
  Serial.println("b");
  #ifdef USE_OLED
  showPrinterStats();
  #endif
  Serial.println("c");

  // Turn LED ON
  digitalWrite(LED_BLINK_PIN, false);
  Serial.println("d");
  Serial.println("Printer ready.");
  Serial.println("e");
}

void createNextFile() {
  if (file) {
    file.close();
    Serial.printf("File /d/%05d.txt closed\n", freeFileIndex);
    freeFileIndex++;
  }

  char fileName[31];
  sprintf(fileName, "/d/%05d.txt", freeFileIndex);
  Serial.printf("Creating file /d/%05d.txt\n", freeFileIndex);
  file = FS.open(fileName, "w");

  if (!file) {
    Serial.println("file creation failed");
  }
}

// Blink if printer is full.
void full() {
  Serial.println("no more space on printer");

  #ifdef USE_OLED
  oled_msg((String)"Printer is full :-(");
  #endif

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

  /* Default link serial out pin state */
  digitalWrite(GB_MISO, LOW);

  freeFileIndex = nextFreeFileIndex();

  if (freeFileIndex <= MAX_IMAGES) {
    Serial.printf("Next file: /d/%05d.txt\n", freeFileIndex);
  } else {
    full();
  }

  // createNextFile();

  /* Setup */
  gpb_serial_io_init(sizeof(gbp_serialIO_raw_buffer), gbp_serialIO_raw_buffer);

  /* Attach ISR */
  #ifdef GBP_FEATURE_USING_RISING_CLOCK_ONLY_ISR
  attachInterrupt( digitalPinToInterrupt(GB_SCLK), serialClock_ISR, RISING);  // attach interrupt handler
  #else
  attachInterrupt( digitalPinToInterrupt(GB_SCLK), serialClock_ISR, CHANGE);  // attach interrupt handler
  #endif
}


#ifdef USE_OLED
void showPrinterStats() {
  // char printed[20];
  // int percUsed = fs_info();
  // sprintf(printed, "%3d files", freeFileIndex - 1);
  // oled_msg(((String)percUsed)+((char)'%')+" remaining",printed);
  // oled_drawLogo();
}
#endif

inline void gbp_packet_capture_loop() {

  if (gbp_serial_io_print_isr() && gbp_serial_io_should_print()) {

    #define WRITE_CHUNK_SIZE 512

    writeTo = min(writeFrom + WRITE_CHUNK_SIZE, img_index);

    Serial.print(writeFrom);
    Serial.print(" -> ");
    Serial.print(writeTo);
    Serial.print(" --> ");
    Serial.print(img_index);

    // char writeChunk[WRITE_CHUNK_SIZE];

    // for (int i = writeFrom; i < writeTo; i++) {
    //   // writeChunk[i - writeFrom] = (char)image_data[i];
    //   if (file) {
    //     file.print((char)image_data[i]);
    //   } else {
    //     Serial.println('No File!!!!');
    //   }
    // }

    Serial.println(" !");

    if (writeTo == img_index) {
      Serial.println("Done");
      // createNextFile();
      resetValues();
      Serial.println("Don2");
      gbp_serial_io_print_done();
    } else {
      Serial.println("Moar");
      writeFrom = writeTo;
    }


    gbp_serial_io_print_isr_done();
    return;
  }

  /* tiles received */
  static uint32_t pktByteIndex = 0;
  static uint16_t pktDataLength = 0;
  const size_t dataBuffCount = gbp_serial_io_dataBuff_getByteCount();
  if (
    ((pktByteIndex != 0) && (dataBuffCount > 0)) ||
    ((pktByteIndex == 0) && (dataBuffCount >= 6))
  ) {
    uint8_t data_8bit = 0;

    // Display the data payload encoded in hex
    for (int i = 0 ; i < dataBuffCount ; i++) {
      // Start of a new packet
      if (pktByteIndex == 0) {
        pktDataLength = gbp_serial_io_dataBuff_getByte_Peek(4);
        pktDataLength |= (gbp_serial_io_dataBuff_getByte_Peek(5)<<8)&0xFF00;

        digitalWrite(LED_BLINK_PIN, HIGH);
      }

      // Print Hex Byte
      data_8bit = gbp_serial_io_dataBuff_getByte();

      // fill the image data buffer
      image_data[img_index] = (byte)data_8bit;
      img_index++;

      if ((pktByteIndex > 5) && (pktByteIndex >= (9 + pktDataLength))) {
        digitalWrite(LED_BLINK_PIN, LOW);
        pktByteIndex = 0;
      } else {
        pktByteIndex++; // Byte hex split counter
      }
    }
  }
}

void espprinter_loop() {

  gbp_packet_capture_loop();

  // Trigger Timeout and reset the printer if byte stopped being received.
  static uint32_t last_millis = 0;
  uint32_t curr_millis = millis();
  if (curr_millis > last_millis) {
    uint32_t elapsed_ms = curr_millis - last_millis;
    if (gbp_serial_io_timeout_handler(elapsed_ms)) {
      digitalWrite(LED_BLINK_PIN, LOW);
    }
  }

  last_millis = curr_millis;
}
