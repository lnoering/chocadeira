#pragma once
#ifndef NSE_BUTTON_H
#define NSE_BUTTON_H

#include <Arduino.h>       // for delayMicroseconds, digitalPinToBitMask, etc

class Button {
    public:
        Button (uint8_t pin, bool pullup, uint16_t debounceDelay = 50);
        virtual ~Button();
        bool check(bool triggerState = LOW);

    private:
        uint8_t _pin;
        bool _state;
        bool _lastState;
        uint32_t _lastMillis;
        uint16_t _debounceDelay;
        uint32_t _lastDebounceTime;
};

#endif //NSE_BUTTON_H