#ifndef NSE_HUMIDITY_H
#define NSE_HUMIDITY_H

#include <Arduino.h>
#include <inttypes.h>
#include <LiquidCrystal.h>
#include "../PID/NSE-PID.h"

class Humidity: public PID
{
    public:
        Humidity(uint8_t pinInWater, uint8_t pinOutHeatsWater);
        // Humidity(uint8_t pinInWater, uint8_t pinOutHeatsWater) : _pinInWater(pinInWater), PID(pinOutHeatsWater) {};
        ~Humidity();

        void setup();

        void control(float humidity);

        void show(LiquidCrystal & lcd,uint8_t col, uint8_t lin);
    
    protected:

    private:
        uint8_t _pinInWater;

};


#endif //NSE_HUMIDITY_H