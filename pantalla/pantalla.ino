#include <Wire.h>
#include "Adafruit_SHT31.h"

#define SDA_PIN 4
#define SCL_PIN 5

Adafruit_SHT31 sht31 = Adafruit_SHT31();

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
  }
}
