#ifndef NSE_HUMIDITY_CPP
#define NSE_HUMIDITY_CPP

#include "NSE-Humidity.h"

Humidity::Humidity(uint8_t pinInWater, uint8_t pinOutHeatsWater) : PID(pinOutHeatsWater), _pinInWater(pinInWater)
{


}

Humidity::~Humidity() 
{
    
}

void Humidity::setup()
{
    pinMode(_pinInWater,INPUT);

    PID::setup();
}


void Humidity::control(float humidity)
{
    int input = digitalRead(_pinInWater);

    if (input && !PID::isEnable()) {
        PID::setEnable(true);
    } else if(!input && PID::isEnable()) {
        PID::setEnable(false);
    }
    
    PID::control(humidity);
}


void Humidity::show(LiquidCrystal & lcd,uint8_t col, uint8_t lin)
{
    char display[(20 - col)];
    sprintf(display,"I=%1u ",digitalRead(_pinInWater));
    lcd.setCursor(col,lin);
    lcd.print(display);

    PID::show(lcd,(col+4),lin);
}



#endif //NSE_HUMIDITY_CPP