/*

sdFat Test with M5Stack and TFT display (sharing one SPI bus)

*/

#include "SdFat.h"
#include <Arduino.h>
#include <M5Unified.h>

// Optional reduced SPI frequency for reading TFT
// #define SPI_READ_FREQUENCY  16000000
// https://github.com/m5stack/M5Stack/blob/6cd8ce15e723c2043a3c01763320413ebff300d5/src/utility/In_eSPI_Setup.h#L266

// but in M5Stack.cpp original SD starts with 40 MHz ?!
// SD.begin(TFCARD_CS_PIN, SPI, 40000000);
// https://github.com/m5stack/M5Stack/blob/6cd8ce15e723c2043a3c01763320413ebff300d5/src/M5Stack.cpp#L27

#define TFCARD_CS_PIN 4

#define SPI_SPEED SD_SCK_MHZ(10) // MHz: OK 4 10 20 25   too much: 29 30 40
#define SD_CONFIG SdSpiConfig(TFCARD_CS_PIN, SHARED_SPI, SPI_SPEED, &SPI)

SdFat sd;
SdBaseFile file;
SdBaseFile root;

M5GFX display;


void setup() {
  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;
  M5.begin(cfg);
  display.begin();

  delay(1000);
  int fNameLength = 100;

  if (!sd.begin(SD_CONFIG)) {
    sd.initErrorHalt(&Serial);
  }

  if (!root.open("/")) {
    Serial.println(F("dir.open failed"));
  }

  while (file.openNext(&root, O_RDONLY)) {
    file.printFileSize(&Serial);
    Serial.write(' ');
    file.printModifyDateTime(&Serial);
    Serial.write(' ');
    file.printName(&Serial);
    char f_name[fNameLength];
    file.getName(f_name, fNameLength);
    if (file.isDir()) {
      Serial.write('/');
    }
    Serial.println();
    file.close();
  }
  if (root.getError()) {
    Serial.println(F("openNext failed"));
  } else {
    Serial.println(F("Done!"));
  }
}

void loop() {}