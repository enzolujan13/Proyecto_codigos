#include <Arduino.h>
#include <Wire.h>// I2C bus
#include "HT_st7735.h"
#define VBAT_Read 1
#define ADC_Ctrl 2

HT_st7735 st7735;

void setup() {
 st7735.st7735_init();

  st7735.st7735_fill_screen(ST7735_BLACK);

  st7735.st7735_write_str(0, 0, "Iniciando...");
Serial.begin(115200);
delay(500);
Serial.println();
Serial.print("[setup] Battery Voltage Read");
Serial.println();
Serial.println("[setup] Commence Set-up...");


pinMode(ADC_Ctrl,OUTPUT);
pinMode(VBAT_Read,INPUT);

}


void loop() {
  float voltaje = leerVoltajeBateria();
  int porcentaje = calcularPorcentaje(voltaje);

  Serial.println("Voltaje Batería: ");
  Serial.println(voltaje);
  Serial.println(" V | Porcentaje: ");
  Serial.println(porcentaje);
  Serial.println(" %");

  delay(5000); // Muestra la lectura cada 5 segundos
}

float leerVoltajeBateria() {
  // 1. Encender el divisor de voltaje poniendo ADC_CTRL en LOW
  digitalWrite(ADC_Ctrl, LOW); 
  delay(10); // Breve espera para estabilizar el voltaje

  // 2. Leer el ADC (Por defecto el ESP32-S3 usa resolución de 12 bits: 0-4095)
  int valorADC = analogRead(VBAT_Read);

  // 3. Apagar el divisor de voltaje (HIGH) para ahorrar batería
  digitalWrite(ADC_Ctrl, HIGH); 

  // 4. Calcular el voltaje real 
  // Multiplicador típico de calibración para la resistencia interna de Heltec: ~4.9 (o 5.0)
  // Depende de la atenuación del ADC, pero este factor convierte el valor de forma precisa.
  float voltajeIn = (valorADC * 4.9) / 1000.0; 
  
  return voltajeIn;
}

int calcularPorcentaje(float mVolts) {
  // Mapeo aproximado de una celda de Litio (4.2V cargada, 3.2V vacía)
  if (mVolts >= 4.20) return 100;
  if (mVolts <= 3.20) return 0;
  
  // Convierte el rango linealmente de 3.2V-4.2V a un valor de 0 a 100
  int pct = (mVolts - 3.20) * 100 / (4.20 - 3.20);
  return pct;
}
