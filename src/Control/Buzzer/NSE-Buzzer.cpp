#ifndef NSE_BUZZER_CPP
#define NSE_BUZZER_CPP

#include "NSE-Buzzer.h"

Buzzer::Buzzer(uint8_t pinBuzzer) : _pinControl(pinBuzzer)
{
    
}

Buzzer::~Buzzer() 
{
    // delete this;
}

void Buzzer::setup()
{
    pinMode(_pinControl,OUTPUT);
    digitalWrite(_pinControl,LOW);
}

void Buzzer::setEnable(bool value)
{
    this->_enable = value;
}

bool Buzzer::isEnable()
{
    return this->_enable;
}

void Buzzer::interruptControl()
{
    if (this->isEnable()) {
        _stepBuzzer ++;
        if (_stepBuzzer <= 300) {
            if(_stepBuzzer & 1) {
                digitalWrite(_pinControl,HIGH);
            } else {
                digitalWrite(_pinControl,LOW);
            }
        } else {
            if (_stepBuzzer > 2000)
                _stepBuzzer = 0;
        }
    }
}

#endif //NSE_BUZZER_CPP