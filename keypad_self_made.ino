#include "Keypad.h"
#include "Encoder.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <SD.h>

#define SD_CS_PIN 18

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define FONT_SIZE_UNIT 8
#define FONT_SCALE 2

const byte COLS = 3;
const byte ROWS = 4;
const byte MAX_NUMBER_OF_KEYS_PRESSED = 4;

byte rowPins[ROWS] = { 7, 8, 9, 10 };  // connect to the row pinouts of the keypad
byte colPins[COLS] = { 4, 5, 6 };      // connect to the column pinouts of the keypad

Keypad keypad(COLS, ROWS, MAX_NUMBER_OF_KEYS_PRESSED, rowPins, colPins);

const byte NUM_OF_OPTIONS = 5;
const String menuOptions[] = { "Work", "Personal", "Emails", "Spotify", "Passwords" };
byte menuIndex = 0;
byte layerIndex = 0;
byte pageIndex = 0;

struct shortcut {
  char key;
  char name[20];
};

const shortcut shortcuts[][ROWS * COLS] PROGMEM = {
  {
    { '1', "Unlock PC" },
    { '2', "Win Password" },
    { '3', "Visual Studio" },
    { '4', "VS Code" },
    { '5', "Remote Desktop" },
    { '6', "Open PRU3WA" },
    { '7', "Open PRS62" },
    { '8', "Teams" },
    { 'D', "Sourcetree" },
    { 'M', "Create MR" },
    { 'H', "Create Ticket" },
    { 'S', "Open Code" },
  },
  {
    { '1', "Unlock PC" },
    { '2', "League of legends" },
    { '3', "Empty" },
    { '4', "Empty" },
    { '5', "Empty" },
    { '6', "Empty" },
    { '7', "Empty" },
    { '8', "Empty" },
    { 'D', "Empty" },
    { 'M', "Empty" },
    { 'H', "Empty" },
    { 'S', "Empty" },
  },
  {
    { '1', "Unlock PC" },
    { '2', "League of legends" },
    { '3', "Empty" },
    { '4', "Empty" },
    { '5', "Empty" },
    { '6', "Empty" },
    { '7', "Empty" },
    { '8', "Empty" },
    { 'D', "Empty" },
    { 'M', "Empty" },
    { 'H', "Empty" },
    { 'S', "Empty" },
  },
  {
    { '1', "Unlock PC" },
    { '2', "League of legends" },
    { '3', "Empty" },
    { '4', "Empty" },
    { '5', "Empty" },
    { '6', "Empty" },
    { '7', "Empty" },
    { '8', "Empty" },
    { 'D', "Empty" },
    { 'M', "Empty" },
    { 'H', "Empty" },
    { 'S', "Empty" },
  },
  {
    { '1', "Unlock PC" },
    { '2', "League of legends" },
    { '3', "Empty" },
    { '4', "Empty" },
    { '5', "Empty" },
    { '6', "Empty" },
    { '7', "Empty" },
    { '8', "Empty" },
    { 'D', "Empty" },
    { 'M', "Empty" },
    { 'H', "Empty" },
    { 'S', "Empty" },
  }
};

String previousPressedKeys = "";
char displayedKey = 0;
// 'KIKI', 128x64px
const unsigned char kiki_bitmapKIKI[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0xf8, 0x02, 0x01, 0x80, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0e, 0x0f, 0x03, 0xff, 0x38, 0x60, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x38, 0x03, 0x87, 0x0f, 0xe1, 0x80, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x30, 0x20, 0xfc, 0x00, 0x7f, 0x18, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x60, 0x38, 0x00, 0x00, 0x03, 0x10, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0x08, 0x00, 0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0x08, 0x00, 0x00, 0x00, 0x1e, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0xf8, 0x00, 0x00, 0x00, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0x30, 0x00, 0x00, 0x00, 0x0f, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x61, 0xe0, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x71, 0xc0, 0x00, 0x00, 0x00, 0x03, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x18, 0xc0, 0x00, 0x00, 0x00, 0x01, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x70, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1c, 0x03, 0x00, 0x3e, 0x00, 0x0f, 0x80, 0xc0, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x02, 0x00, 0x6e, 0x00, 0x0e, 0xc0, 0x60, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0xe6, 0x00, 0xcf, 0x00, 0x1e, 0x60, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x3c, 0x00, 0x7f, 0x00, 0x1f, 0xc0, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x3e, 0x00, 0x0f, 0x83, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x18, 0x78, 0x08, 0x00, 0x02, 0x1e, 0x08, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x10, 0x0e, 0x00, 0x3e, 0x00, 0xf0, 0x0d, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x10, 0x01, 0xc0, 0x41, 0x07, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x30, 0x00, 0x38, 0x22, 0x1c, 0x07, 0xc2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xf8, 0x08, 0x08, 0x10, 0x38, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0x07, 0xe0, 0x08, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0x00, 0x00, 0x08, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0x00, 0x01, 0x08, 0x40, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x70, 0x07, 0x00, 0xc9, 0x80, 0x78, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x78, 0x38, 0x00, 0x77, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x79, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x8e, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0xb3, 0xc0, 0x00, 0x00, 0x00, 0x07, 0xfc, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0xc3, 0xfc, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x00, 0xff, 0x80, 0x00, 0x01, 0xc0, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0e, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xe2, 0x00, 0x01, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x82, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0xe0, 0x00, 0x01, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x01, 0x00, 0x20, 0x00, 0x01, 0x00, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x01, 0xc0, 0x20, 0x00, 0x01, 0x00, 0xc0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x60, 0x60, 0x00, 0x01, 0x01, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x30, 0x40, 0x00, 0x01, 0x83, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x1b, 0xc0, 0x00, 0x00, 0xce, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x78, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x06, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0e, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xc1, 0x80, 0xf8, 0x00, 0x0f, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0xfc, 0x83, 0x8f, 0xff, 0xf8, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1f, 0xab, 0x00, 0x00, 0x00, 0xa5, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'KIKI2', 128x64px
const unsigned char kiki_bitmapKIKI2[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x08, 0x18, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0e, 0x0f, 0xf3, 0x08, 0x38, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x07, 0x18, 0x7e, 0x30, 0x0c, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x21, 0xf0, 0x07, 0xe6, 0x04, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x64, 0x06, 0x00, 0x20, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x07, 0x02, 0x08, 0x03, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xf0, 0x00, 0x00, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x30, 0x00, 0x00, 0x03, 0xc2, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0xe0, 0x00, 0x00, 0x01, 0x06, 0x00, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xc0, 0x00, 0x00, 0x00, 0x84, 0x00, 0x80, 0x0c, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0xc0, 0x00, 0x00, 0x00, 0xc8, 0x00, 0x82, 0x08, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x78, 0x00, 0x82, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x78, 0x01, 0xf0, 0x60, 0x00, 0x82, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xb8, 0x01, 0xd0, 0x30, 0x0c, 0xa2, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf2, 0x01, 0xb8, 0x01, 0xc8, 0x1f, 0x06, 0xab, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0xf8, 0x01, 0xf0, 0x38, 0x06, 0xa8, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x78, 0x01, 0xf0, 0xe0, 0x07, 0xbe, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x70, 0x20, 0x00, 0x47, 0x00, 0x03, 0xfe, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0c, 0x01, 0xe0, 0x1c, 0x05, 0xf1, 0x33, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x03, 0x82, 0x10, 0xf0, 0x1f, 0x07, 0xf0, 0xc0, 0x20,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x61, 0x21, 0x81, 0xe2, 0x1c, 0xd8, 0x40, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf0, 0x20, 0x40, 0x0e, 0x02, 0x00, 0x8c, 0x40, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x0f, 0x80, 0x40, 0x18, 0x03, 0x01, 0x86, 0x40, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x40, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x44, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x0e, 0x06, 0x58, 0x0e, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x30, 0x03, 0xb0, 0x03, 0x83, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xc0, 0x00, 0x00, 0x00, 0xe3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x03, 0x85, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x01, 0x9f, 0x81, 0x00, 0x60,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xf1, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x00, 0x00, 0x00, 0x1f, 0x80, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0xc1, 0xff, 0xff, 0xc0, 0x30, 0x03, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x03, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x20, 0x70, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x30, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x17, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x83, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc1, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe1, 0x81, 0xe0, 0x01, 0xf0, 0x40, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x87, 0x3f, 0xff, 0x10, 0x60, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xa6, 0x00, 0x00, 0x19, 0xa0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 2080)
const int kiki_bitmapallArray_LEN = 2;
const unsigned char* kiki_bitmapallArray[2] = {
  kiki_bitmapKIKI,
  kiki_bitmapKIKI2
};


Encoder encoder(A2, A3, A1);

unsigned int counter = 0;
bool timer1 = 0;

//File myFile;

void setup() {
  Serial.begin(9600);
  
  delay(3000);

  // Serial.println("Initializing SD card...");

  // if (!SD.begin(SD_CS_PIN)) {
  // Serial.println("init failed");
  //   while (true)
  //     ;
  // }
  // Serial.println("initialization done.");



  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  keypad.initKeypad();
  encoder.initEncoder();
  renderMenu();
}

void renderMenu() {
  display.clearDisplay();

  if (layerIndex == 0) {
    display.setTextSize(FONT_SCALE);
    display.setTextColor(SSD1306_WHITE);  // Draw white text
    display.cp437(true);                  // Use full 256 char 'Code Page 437' font

    display.setCursor(10, (SCREEN_HEIGHT - FONT_SCALE * FONT_SIZE_UNIT) / 2);
    display.print(menuOptions[menuIndex]);
    display.drawRect(5, (SCREEN_HEIGHT - FONT_SCALE * FONT_SIZE_UNIT) / 2 - 5, display.width() - 10, FONT_SCALE * FONT_SIZE_UNIT + 8, SSD1306_WHITE);

    if (menuIndex > 0) {
      display.setCursor(10, (SCREEN_HEIGHT - FONT_SCALE * FONT_SIZE_UNIT) / 2 - FONT_SCALE * FONT_SIZE_UNIT - 8);
      display.print(menuOptions[menuIndex - 1]);
    }
    if (menuIndex < NUM_OF_OPTIONS - 1) {
      display.setCursor(10, (SCREEN_HEIGHT + FONT_SCALE * FONT_SIZE_UNIT) / 2 + 6);
      display.print(menuOptions[menuIndex + 1]);
    }
  } else if (layerIndex == 1) {
    renderPage();
  }

  display.display();
}

void renderPage() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.cp437(true);                  // Use full 256 char 'Code Page 437' font

  for (byte i = 0; i < 6; i++) {
    shortcut shc;
    memcpy_P(&shc, &shortcuts[menuIndex][pageIndex + i], sizeof shc);
    display.setCursor(0, i * (FONT_SIZE_UNIT + 2));
    display.write(shc.key);
    display.setCursor(16, i * (FONT_SIZE_UNIT + 2));
    display.print(shc.name);
  }
}

void loop() {
  if (timer1) {
    counter++;
  } else if (counter) {
    counter = 0;
  }
  String x = String(keypad.getPressedKeys());
  char encoderValue = encoder.getEncoderEvent();
  if (encoderValue) {
    if (encoderValue == '+') {
      if (layerIndex && pageIndex < ROWS * COLS - 7) {
        pageIndex++;
      } else if (!layerIndex && menuIndex < NUM_OF_OPTIONS - 1) {
        menuIndex++;
      }
    } else if (encoderValue == '-') {
      if (layerIndex && pageIndex > 0) {
        pageIndex--;
      } else if (!layerIndex && menuIndex > 0) {
        menuIndex--;
      }
    } else if (encoderValue == 'B') {
      layerIndex = !layerIndex;
      pageIndex = 0;
    }
    renderMenu();
  }
  if (x != previousPressedKeys) {
    // if (x == "M" && menuIndex < NUM_OF_OPTIONS - 1) {
    //   menuIndex++;
    // } else if (x == "D" && menuIndex > 0) {
    //   menuIndex--;
    // }
    // renderMenu();
    if (x.length()) {
      display.clearDisplay();
      display.drawBitmap(0, 0, kiki_bitmapallArray[1], SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
      timer1 = 1;
      display.display();
      displayedKey = x[0];
    }
    Serial.println("Key Event");
    previousPressedKeys = x;
  }

  if (counter > 200) {
    display.clearDisplay();
    display.drawBitmap(0, 0, kiki_bitmapallArray[0], SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
    if (displayedKey) {
      display.setTextSize(2);
      display.setCursor(100, 10);
      display.write(displayedKey);
    }
    display.display();
    timer1 = 0;
  }
}
