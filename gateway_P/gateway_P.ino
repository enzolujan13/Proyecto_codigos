#include "Arduino.h"
#include "HT_st7735.h"

#include <RadioLib.h>

// ======================
// PANTALLA
// ======================
HT_st7735 st7735;

// ======================
// LORA
// ======================
SX1262 radio = new Module(
  8,
  14,
  12,
  13
);

void setup() {

  Serial.begin(115200);

  st7735.st7735_init();

  st7735.st7735_fill_screen(ST7735_BLACK);

  st7735.st7735_write_str(0, 0, "Gateway");

  int state = radio.begin(915.0);

  if (state == RADIOLIB_ERR_NONE) {

    st7735.st7735_write_str(0, 20, "LoRa OK");

  } else {

    st7735.st7735_write_str(0, 20, "LoRa ERROR");

    while (1);
  }
}

void loop() {

  String str;

  int state = radio.receive(str);

  if (state == RADIOLIB_ERR_NONE) {

    // ======================
    // SERIAL PARA RASPBERRY
    // ======================
    Serial.println(str);

    // ======================
    // RSSI
    // ======================
    float rssi = radio.getRSSI();

    // ======================
    // PANTALLA
    // ======================
    st7735.st7735_fill_screen(ST7735_BLACK);

    st7735.st7735_write_str(0, 0, "Gateway RX");

    st7735.st7735_write_str(0, 20, str);

    st7735.st7735_write_str(
      0,
      40,
      "RSSI: " + String(rssi)
    );
  }
}
