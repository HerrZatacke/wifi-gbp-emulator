#ifdef USE_OLED

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#ifndef OLED_SDA
#define OLED_SDA 0
#endif
#ifndef OLED_SCL
#define OLED_SCL 4
#endif

#define LOGO_HEIGHT 27
#define LOGO_WIDTH 32

static const unsigned char PROGMEM logo_bmp[] =
{B00000000, B00000110, B00000000, B00000000,
 B00000000, B00011111, B10000000, B00000000,
 B00000000, B00111001, B11000000, B00000000,
 B00000000, B11100000, B11100000, B00000000,
 B00000011, B11000000, B01100000, B00000000,
 B00000111, B00000000, B00110000, B00000000,
 B00011110, B00000000, B00011000, B00000000,
 B00111110, B00000000, B00111110, B00000000,
 B01110011, B00000000, B11111111, B10000000,
 B11100001, B10000001, B11110011, B11000000,
 B11110000, B11000111, B11000101, B11110000,
 B10011100, B11101111, B00010011, B11111000,
 B10001110, B01111100, B01001111, B00111110,
 B10000111, B11110001, B00111110, B00011111,
 B10000001, B11100100, B11111111, B00111111,
 B11000000, B11110011, B11111111, B11111011,
 B01100000, B00111111, B11001111, B11110001,
 B00111000, B00011111, B10000111, B11100001,
 B00001100, B00000111, B11001111, B10000001,
 B00000111, B00000001, B11111110, B00000011,
 B00000001, B10000000, B11111000, B00000110,
 B00000000, B11100000, B00110000, B00011100,
 B00000000, B00110000, B00000000, B00110000,
 B00000000, B00011100, B00000000, B11100000,
 B00000000, B00000110, B00000011, B10000000,
 B00000000, B00000011, B10000110, B00000000,
 B00000000, B00000000, B11111100, B00000000,
};

#define ICON_WIFI_HEIGHT 12
#define ICON_WIFI_WIDTH 16

static const unsigned char PROGMEM icon_wifi_bmp[] =
{ B00001111, B10000000,
  B00000000, B11100000,
  B00000000, B00111000,
  B00001111, B00001100,
  B00000001, B11000100,
  B00000000, B01100010,
  B00001110, B00110011,
  B00000011, B00010001,
  B00000001, B10011001,
  B00001000, B10001001,
  B00011100, B10001001,
  B00001000, B00000000,
};

void oled_setup() {
  Wire.begin(OLED_SDA, OLED_SCL);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  oled_msg("Booting...");
}

void oled_msg(String message) {
  oled_msg(message, 12);
}

void oled_msg(String message1, String message2) {
  oled_msg(message1 + "\n" + message2, 8);
}

void oled_msg(String message1, String message2, String message3) {
  oled_msg(message1 + "\n" + message2 + "\n" + message3, 4);
}

void oled_msg(String message1, String message2, String message3, String message4) {
  oled_msg(message1 + "\n" + message2 + "\n" + message3 + "\n" + message4, 0);
}

void oled_msg(String message, byte y) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, y);
  display.println(message);
  display.display();
  #ifdef OLED_INVERT
  display.invertDisplay(true);
  #endif
}

void oled_drawLogo() {
  display.drawBitmap(88, 3, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}

void oled_drawIcon() {
  display.drawBitmap(112, 0, icon_wifi_bmp, ICON_WIFI_WIDTH, ICON_WIFI_HEIGHT, 1);
  display.display();
}

#endif
