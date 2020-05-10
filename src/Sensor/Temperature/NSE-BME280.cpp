#ifndef NSE_BME280_CPP
#define NSE_BME280_CPP

#include "NSE-BME280.h"

BME280::BME280()
{
    _bme = new Adafruit_BME280;
}

BME280::~BME280()
{
    delete & _bme;
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

void BME280::show(LiquidCrystal & lcd, uint8_t col, uint8_t lin, BME280Types type)
{
    char parse[20] = "",
         print[20];
    float read;

    switch (type)
    {
        case BME280Types::PRESSURE:
                read = _bme->readPressure();
                dtostrf(read,4,0,parse);
                sprintf(print,"%s",parse);
                lcd.setCursor(col,lin);
                lcd.print(parse);
                lcd.print(" hPa");
            break;
        case BME280Types::ALTIMETER:
                read = _bme->readAltitude(SEALEVELPRESSURE_HPA); //Change the "1050.35" to your city current barrometric pressure (https://www.wunderground.com)
                dtostrf(read,4,2,parse);
                sprintf(print,"%s",parse);
                lcd.setCursor(col,lin);
                lcd.print(parse);
                lcd.print(" ft");
            break;
        case BME280Types::HUMIDITY:
                read = _bme->readHumidity();
                dtostrf(read,4,1,parse);
                sprintf(print,"%s",parse);
                lcd.setCursor(col,lin);
                lcd.print(parse);
                lcd.print(char(37));
            break;
        default:
                read = _bme->readTemperature();
                dtostrf(read,4,2,parse);
                sprintf(print,"%s C",parse);
                lcd.setCursor(col,lin);
                lcd.print(parse);
                lcd.print(char(223));
                lcd.print("C");
    }
}

#endif //NSE_BME280_CPP