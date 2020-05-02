#ifndef NSE_DS3231_H
#define NSE_DS3231_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <RtcDS3231.h>

class DS3231
{
    public:
        DS3231();
        ~DS3231();

        void setup();
        bool isOperating();
        RtcDateTime getDateTime();
        void show(LiquidCrystal& lcd, uint8_t col, uint8_t lin);

    protected:

    private:
        RtcDS3231<TwoWire> * _rtc;

};

#endif // NSE_DS3231_H