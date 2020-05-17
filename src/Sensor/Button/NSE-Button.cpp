#ifndef NSE_BUTTON_CPP
#define NSE_BUTTON_CPP

#include "NSE-Button.h"

Button::Button (uint8_t pin, bool pullup = false, uint16_t debounceDelay = 50)
{
    _pin = pin;
    _state = LOW;
    _lastState = LOW;
    _lastMillis = 0;
    _debounceDelay = debounceDelay;
    _lastDebounceTime = 0;
    if (pullup == true) {
        pinMode(_pin, INPUT_PULLUP);
    } else {
        pinMode(_pin, INPUT);
    }
}

Button::~Button()
{

}

bool Button::check(bool triggerState = LOW) {
    bool reading = digitalRead(_pin);
    
    if (reading != _lastState) {
        _lastDebounceTime = millis();
    }
    
    if ((millis() - _lastDebounceTime) > _debounceDelay) {
        if (reading != _state) {
            _state = reading;
            return _state;
        }
    }
    _lastState = reading;
    
    if (triggerState == HIGH) {
        return LOW;
    } else {
        return HIGH;
    }
}

#endif //NSE_BUTTON_CPP