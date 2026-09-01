#include "Arduino.h"
#include "HT_st7735.h"
#include "HT_TinyGPS++.h"

TinyGPSPlus GPS;
HT_st7735 st7735;

#define VGNSS_CTRL 3

void GPS_test(void)
{
    pinMode(VGNSS_CTRL, OUTPUT);
    digitalWrite(VGNSS_CTRL, HIGH);

    Serial1.begin(115200, SERIAL_8N1, 33, 34);

    Serial.println("GPS_test");

    st7735.st7735_fill_screen(ST7735_BLACK);
    delay(100);

    st7735.st7735_write_str(0, 0, (String)"GPS_test");

    while (1)
    {
        if (Serial1.available() > 0)
        {
            if (Serial1.peek() != '\n')
            {
                GPS.encode(Serial1.read());
            }
            else
            {
                Serial1.read();

                if (GPS.time.second() == 0)
                {
                    continue;
                }

                // =========================
                // Ajuste horario Argentina
                // UTC - 3
                // =========================
                int horaArgentina = GPS.time.hour() - 3;

                if (horaArgentina < 0)
                {
                    horaArgentina += 24;
                }

                // Formato de hora
                char buffer[20];

                sprintf(buffer,
                        "%02d:%02d:%02d",
                        horaArgentina,
                        GPS.time.minute(),
                        GPS.time.second());

                String time_str = String(buffer);

                // =========================
                // Mostrar datos
                // =========================
                st7735.st7735_fill_screen(ST7735_BLACK);

                st7735.st7735_write_str(0, 0, (String)"GPS_test");

                st7735.st7735_write_str(0, 20, time_str);

                String latitude = "LAT: " + String(GPS.location.lat(), 6);
                st7735.st7735_write_str(0, 40, latitude);

                String longitude = "LON: " + String(GPS.location.lng(), 6);
                st7735.st7735_write_str(0, 60, longitude);

                delay(1000);

                while (Serial1.available() > 0)
                {
                    Serial1.read();
                }
            }
        }
    }
}

void setup()
{
    delay(100);

    Serial.begin(115200);

    st7735.st7735_init();

    GPS_test();
}

void loop()
{
    delay(100);
}
