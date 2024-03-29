#ifndef NSE_BME280_CPP
#define NSE_BME280_CPP

#include "NSE-BME280.h"

BME280::BME280()
{
    _bme = new Adafruit_BME280;
}

BME280::~BME280()
{
    free(_bme);
}

void BME280::setup()
{
    bool status;
    status = _bme->begin(0x76);  
    if (!status) {
        while (1);
    }
}

float BME280::getTemperature()
{
    return _bme->readTemperature();
}

float BME280::getHumidity()
{
    return _bme->readHumidity();
}

void BME280::show(LiquidCrystal & lcd, uint8_t col, uint8_t lin, BME280Types type, boolean toRead, float value)
{
    char parse[20] = "",
         print[20];
    float read = value;

    switch (type)
    {
        case BME280Types::PRESSURE:
                if (toRead) {
                    read = _bme->readPressure();
                }
                dtostrf(read,4,0,parse);
                sprintf(
                        print,
                        "%shPa",
                        parse);
            break;
        case BME280Types::ALTIMETER:
                if (toRead) {
                    read = _bme->readAltitude(SEALEVELPRESSURE_HPA); //Change the "1050.35" to your city current barrometric pressure (https://www.wunderground.com)
                }
                dtostrf(read,4,2,parse);
                sprintf(
                        print,
                        "%sft",
                        parse);
            break;
        case BME280Types::HUMIDITY:
                if (toRead) {
                    read = _bme->readHumidity();
                }
                dtostrf(read,4,1,parse);
                sprintf(
                        print,
                        "%s%c",
                        parse, 37);
            break;
        default:
                if (toRead) {
                    read = _bme->readTemperature();
                }
                dtostrf(read,4,2,parse);
                sprintf(
                        print,
                        "%s%cC",
                        parse,char(223));
    }
    lcd.setCursor(col,lin);
    lcd.print(print);

}

#endif //NSE_BME280_CPP