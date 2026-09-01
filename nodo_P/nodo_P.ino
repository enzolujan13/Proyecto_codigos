#include <Wire.h>
#include <Adafruit_SHT31.h>

#include "Arduino.h"
#include "HT_st7735.h"

#include <RadioLib.h>

// ======================
// PANTALLA
// ======================
HT_st7735 st7735;

// ======================
// SENSOR
// ======================
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// ======================
// I2C SENSOR
// ======================
#define SDA_SENSOR 4
#define SCL_SENSOR 5

// ======================
// LORA SX1262
// ======================
SX1262 radio = new Module(
  8,   // NSS
  14,  // DIO1
  12,  // RESET
  13   // BUSY
);

// ======================
// VARIABLES
// ======================
unsigned long lastSend = 0;

void setup() {

  Serial.begin(115200);

  // ======================
  // PANTALLA
  // ======================
  st7735.st7735_init();

  st7735.st7735_fill_screen(ST7735_BLACK);

  st7735.st7735_write_str(0, 0, "Iniciando...");

  // ======================
  // I2C
  // ======================
  Wire.begin(SDA_SENSOR, SCL_SENSOR);

  // ======================
  // SENSOR
  // ======================
  if (!sht31.begin(0x44)) {

    st7735.st7735_write_str(0, 20, "Sensor ERROR");

    while (1);
  }

  st7735.st7735_write_str(0, 20, "Sensor OK");

  // ======================
  // LORA
  // ======================
  int state = radio.begin(915.0);

  if (state == RADIOLIB_ERR_NONE) {

    st7735.st7735_write_str(0, 40, "LoRa OK");

  } else {

    st7735.st7735_write_str(0, 40, "LoRa ERROR");

    while (1);
  }

  delay(2000);
}

void loop() {

  if (millis() - lastSend >= 5000) {

    lastSend = millis();

    // ======================
    // LEER SENSOR
    // ======================
    float temp = sht31.readTemperature();
    float hum  = sht31.readHumidity();

    // ======================
    // CREAR MENSAJE
    // ======================
    String mensaje =
      "S1," +
      String(temp, 2) + "," +
      String(hum, 2);

    // ======================
    // ENVIAR LORA
    // ======================
    int state = radio.transmit(mensaje);

    // ======================
    // MOSTRAR PANTALLA
    // ======================
    st7735.st7735_fill_screen(ST7735_BLACK);

    st7735.st7735_write_str(0, 0, "NODO S1");

    st7735.st7735_write_str(
      0,
      20,
      "Temp: " + String(temp,1)
    );

    st7735.st7735_write_str(
      0,
      40,
      "Hum: " + String(hum,1)
    );

    if (state == RADIOLIB_ERR_NONE) {

      st7735.st7735_write_str(0, 60, "Enviado OK");

      Serial.println("Enviado:");
      Serial.println(mensaje);

    } else {

      st7735.st7735_write_str(0, 60, "Error envio");
    }
  }
}
