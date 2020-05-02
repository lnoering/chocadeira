#ifndef NSE_BME280_H
#define NSE_BME280_H

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal.h>

#define SEALEVELPRESSURE_HPA (1013.25)

enum BME280Types
{
    // bit order:  A1M4  DY/DT  A1M3  A1M2  A1M1
    PRESSURE = 1,
    TEMPERATURE = 2,
    ALTIMETER = 3,
    HUMIDITY = 4,
};

class BME280
{
    public:
        BME280();
        ~BME280();

        void setup();
        void show(LiquidCrystal & lcd, uint8_t col, uint8_t lin, BME280Types type);
    protected:

    private:
        Adafruit_BME280 * _bme;
};

#endif //NSE_BME280_H