#include <Wire.h>
#include "Adafruit_SHT31.h"

// Inicializar el sensor
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  Serial.begin(115200);
  
  // Dirección I2C predeterminada 0x44
  if (! sht31.begin(0x44)) { 
    Serial.println("No se encuentra el sensor NSHT30");
    while (1) delay(1);
  }
}

void loop() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (! isnan(t)) {  // Comprobar si la lectura es correcta
    Serial.print("Temp *C = "); Serial.println(t);
    Serial.print("Hum. % = "); Serial.println(h);
  } else {
    Serial.println("Error leyendo el sensor");
  }
  
  Serial.println();
  delay(2000); // Leer cada 2 segundos
}
