#ifndef NSE_TEMPERATURE_H
#define NSE_TEMPERATURE_H

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

class Temperature
{
    public:
        Temperature(uint8_t pinPWM);
        ~Temperature();

        void setup();
        void setKI(float ki);
        void setKP(float kp);
        void setKD(float kd);
        void setPoint(float setPoint);
        
        void control(float temperature);

        void show(LiquidCrystal & lcd,uint8_t col, uint8_t lin);
        
    protected:
        float getErro(float temperature);
        float getProportional(float erro);
        float getIntegral(float erro);
        float getDerivative(float nowTemperature);

        float proccess(float temperature);
        float proccessTime();

        uint8_t convertPIDtoPWM(float pid);

    private:
        uint8_t _pinControl;
        float _setPoint;
        float _kP;
        float _kI;
        float _kD;
        float _lastTemperature;
        float _integral;
        uint8_t _pwmOut;
        long _lastTimeProcess;
};


#endif //NSE_TEMPERATURE_H