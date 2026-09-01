#include <Wire.h>
#include "Adafruit_SHT31.h"
#include "Arduino.h"
#include "HT_st7735.h"

#define SDA_PIN 4
#define SCL_PIN 5

Adafruit_SHT31 sht31 = Adafruit_SHT31();
HT_st7735 st7735;

unsigned long previousMillis = 0;
const long interval = 2000;

void setup() {

  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("Iniciando NSHT30...");

  if (!sht31.begin(0x44)) {

    Serial.println("Sensor NO encontrado");

    while (1);
  }

  Serial.println("Sensor OK");
  st7735.st7735_fill_screen(ST7735_BLACK);
  delay(100);
  st7735.st7735_write_str(0, 0, (String)"GPS_test");
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;

    float temp = sht31.readTemperature();
    float hum = sht31.readHumidity();

    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.println(" C");

    Serial.print("Humedad: ");
    Serial.print(hum);
    Serial.println(" %");

    Serial.println("-------------------");
    st7735.st7735_fill_screen(ST7735_BLACK);
    st7735.st7735_write_str(0, 0, (String)"tem_test");
    String latitude = "temp: " + (String)temp;
    st7735.st7735_write_str(0, 40, latitude);
    String longitude  = "hum: "+  (String)hum;
    st7735.st7735_write_str(0, 60, longitude);
  }
}
