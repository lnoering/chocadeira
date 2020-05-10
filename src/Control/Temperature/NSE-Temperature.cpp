#ifndef NSE_TEMPERATURE_CPP
#define NSE_TEMPERATURE_CPP

#include "NSE-Temperature.h"

Temperature::Temperature(uint8_t pinPWM)
{
    _pinControl = pinPWM;
}

Temperature::~Temperature() 
{

}

void Temperature::setup()
{
    _kP = 1.0;
    _kI = 1.0;
    _kD = 0.5;
    _lastTemperature = 0.0;
    _integral = 0.0;
    _pwmOut = 0;
    _lastTimeProcess = 0;

    pinMode(_pinControl,OUTPUT);
}

void Temperature::setKI(float ki)
{
    _kI = ki;
}

void Temperature::setKP(float kp)
{
    _kP = kp;
}

void Temperature::setKD(float kd)
{
    _kD = kd;
}

void Temperature::setPoint(float setPoint)
{
    _setPoint = setPoint;
}

float Temperature::proccess(float temperature)
{
    float erro,
          propotional = 0,
          derivative = 0,
          deltaTime = proccessTime();

    erro = getErro(temperature);
    propotional = getProportional(erro);
    _integral += getIntegral(erro) * deltaTime;
    derivative = getDerivative(temperature) * deltaTime;
    //Soma tudo
    // PID = propotional + integral + derivative; 
    _lastTemperature = temperature;

    return (propotional + _integral + derivative);
}

void Temperature::control(float temperature)
{
    float pid = proccess(temperature);

    _pwmOut = convertPIDtoPWM(pid);

    analogWrite(_pinControl,_pwmOut);
}

void Temperature::show(LiquidCrystal & lcd,uint8_t col, uint8_t lin)
{
    char display[(20 - col)],
         x[8];
    dtostrf(_integral,5,2,x);
    sprintf(display,"%03u %s",_pwmOut,x);
    lcd.setCursor(col,lin);
    lcd.print(display);
}

float Temperature::proccessTime()
{
    float deltaTime = (millis() - _lastTimeProcess) / 1000.0;

    _lastTimeProcess = millis();

    return deltaTime;

}

uint8_t Temperature::convertPIDtoPWM(float pid)
{
    if ((pid + PIDtoPWM::PERFEITO) >= PWM_PRECISION::MAX) {
        return PWM_PRECISION::MAX;
    }

    if ((pid + PIDtoPWM::PERFEITO) <= PWM_PRECISION::MIN) {
        return PWM_PRECISION::MIN;
    }

    return pid + PIDtoPWM::PERFEITO;
}

float Temperature::getErro(float temperature)
{
    return _setPoint - temperature;
}

float Temperature::getProportional(float erro)
{
    return erro * _kP;
}

float Temperature::getIntegral(float erro)
{
    if ((PWM_PRECISION::MAX == _pwmOut) ||
        (PWM_PRECISION::MIN == _pwmOut)
    ) {
        return 0;
    }
    return erro * _kI;
}

float Temperature::getDerivative(float nowTemperature)
{
    return (_lastTemperature - nowTemperature) * _kD;
}

#endif //NSE_TEMPERATURE_CPP