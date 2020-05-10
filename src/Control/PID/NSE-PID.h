#ifndef NSE_PID_H
#define NSE_PID_H

#include <Arduino.h>
#include <inttypes.h>
#include <LiquidCrystal.h>


enum PIDtoPWM {
    RESFRIAR = 0,
    PERFEITO = 50,
    AQUECER = 100,
};

enum PWM_PRECISION {
    MAX = 255,
    MIN = 0,
};

class PID
{
    public:
        PID(uint8_t pinPWM);
        ~PID();

        void setup();
        void setKI(float ki);
        void setKP(float kp);
        void setKD(float kd);
        void setPoint(float setPoint);
        
        void control(float inputValue);

        void show(LiquidCrystal & lcd,uint8_t col, uint8_t lin);

        void setEnable(bool value);
        bool isEnable();
        
    protected:
        float getErro(float inputValue);
        float getProportional(float erro);
        float getIntegral(float erro);
        float getDerivative(float nowInputValue);

        float proccess(float inputValue);
        float proccessTime();

        uint8_t convertPIDtoPWM(float pid);

        void reset();

    private:
        uint8_t _pinControl;
        float _setPoint;
        float _kP;
        float _kI;
        float _kD;
        float _lastInputValue;
        float _integral;
        uint8_t _pwmOut;
        long _lastTimeProcess;

        bool _enable = true;
};


#endif //NSE_PID_H