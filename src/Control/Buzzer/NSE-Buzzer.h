#ifndef NSE_BUZZER_H
#define NSE_BUZZER_H

#include <Arduino.h>


class Buzzer
{
    public:
        Buzzer(uint8_t pinBuzzer);
        ~Buzzer();

        void setup();
        void interruptControl();

        void setEnable(bool val);

        bool isEnable();
        
    protected:

    private:
        uint8_t _pinControl;
        unsigned int _stepBuzzer = 0;

        bool _enable = true;
        
};


#endif //NSE_BUZZER_H