#include <Wire.h>

#define SDA_PIN 33
#define SCL_PIN 34

void setup() {

  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("Buscando I2C...");
}

void loop() {

  byte error, address;

  int devices = 0;

  for(address = 1; address < 127; address++) {

    Wire.beginTransmission(address);

    error = Wire.endTransmission();

    if(error == 0){

      Serial.print("I2C encontrado: 0x");

      Serial.println(address, HEX);

      devices++;
    }
  }

  if(devices == 0){

    Serial.println("No se encontraron dispositivos");
  }

  delay(5000);
}
