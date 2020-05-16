#ifndef NSE_PID_CPP
#define NSE_PID_CPP

#include "NSE-PID.h"

PID::PID(uint8_t pinPWM) : _pinControl(pinPWM)
{
    
}

PID::~PID() 
{
    // delete this;
}

void PID::setup()
{
    _kP = 1.0;
    _kI = 1.0;
    _kD = 0.5;

    reset();

    pinMode(_pinControl,OUTPUT);
    // analogWrite(_pinControl,_pwmOut);
    digitalWrite(_pinControl,LOW);
}

void PID::setKI(float ki)
{
    _kI = ki;
}

void PID::setKP(float kp)
{
    _kP = kp;
}

void PID::setKD(float kd)
{
    _kD = kd;
}

void PID::setPoint(float setPoint)
{
    _setPoint = setPoint;
}

float PID::proccess(float inputValue)
{
    float erro,
          propotional = 0,
          derivative = 0,
          deltaTime = proccessTime();

    erro = getErro(inputValue);
    propotional = getProportional(erro);
    _integral += getIntegral(erro) * deltaTime;
    derivative = getDerivative(inputValue) * deltaTime;
    //Soma tudo
    // PID = propotional + integral + derivative; 
    _lastInputValue = inputValue;

    return (propotional + _integral + derivative);
}

void PID::control(float inputValue)
{
    if (isEnable()) {
        float pid = proccess(inputValue);

        _pwmOut = convertPIDtoPWM(pid);

        // analogWrite(_pinControl,_pwmOut);
    }
}

void PID::show(LiquidCrystal & lcd,uint8_t col, uint8_t lin)
{
    char display[(20 - col)],
         x[8];
    dtostrf(_integral,5,2,x);
    sprintf(display,"%03u %s",_pwmOut,x);
    lcd.setCursor(col,lin);
    lcd.print(display);
}

float PID::proccessTime()
{
    float deltaTime = (millis() - _lastTimeProcess) / 1000.0;

    _lastTimeProcess = millis();

    return deltaTime;

}

uint8_t PID::convertPIDtoPWM(float pid)
{
    if ((pid + PIDtoPWM::PERFEITO) >= PWM_PRECISION::MAX) {
        return PWM_PRECISION::MAX;
    }

    if ((pid + PIDtoPWM::PERFEITO) <= PWM_PRECISION::MIN) {
        return PWM_PRECISION::MIN;
    }

    return pid + PIDtoPWM::PERFEITO;
}

float PID::getErro(float inputValue)
{
    return _setPoint - inputValue;
}

float PID::getProportional(float erro)
{
    return erro * _kP;
}

float PID::getIntegral(float erro)
{
    if ((PWM_PRECISION::MAX == _pwmOut) ||
        (PWM_PRECISION::MIN == _pwmOut)
    ) {
        return 0;
    }
    return erro * _kI;
}

float PID::getDerivative(float nowInputValue)
{
    return (_lastInputValue - nowInputValue) * _kD;
}

void PID::reset()
{
    _lastInputValue = 0.0;
    _integral = 0.0;
    _pwmOut = PIDtoPWM::RESFRIAR;
    _lastTimeProcess = 0;
}

void PID::setEnable(bool value)
{
    _enable = value;
    if (!_enable) {
        reset();
        // analogWrite(_pinControl,PIDtoPWM::RESFRIAR);
        digitalWrite(_pinControl,LOW);
    }
}

bool PID::isEnable()
{
    return _enable;
}

uint8_t PID::getPwmOut()
{
    return (uint8_t) map(_pwmOut,0,255,0,65); 
}

void PID::setOutput(bool on) 
{
    if (this->isEnable()) {
        digitalWrite(_pinControl,(on?HIGH:LOW));
    }
}

#endif //NSE_PID_CPP