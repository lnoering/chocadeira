#include "Arduino.h"
#include <LiquidCrystal.h>
#include "Sensor/RTC/NSE-DS3231.h"
#include "Sensor/Temperature/NSE-BME280.h"
#include "Control/PID/NSE-PID.h"
#include "Control/Humidity/NSE-Humidity.h"


LiquidCrystal lcd(7, 6, 5, 4, 9, 2);
DS3231 rtc;
BME280 bme;
PID tmp(3);
Humidity hmdt(17,11);

uint8_t fiveSeconds = 0;
uint8_t oneSecond = 0;


void setup() {
    lcd.begin(20, 4);
    
    rtc.setup();
    
    bme.setup();

    tmp.setup();
    tmp.setPoint(27.27);

    hmdt.setup();
    hmdt.setPoint(70.5);

    // Configuração do timer2
    // TCNT2 //Timer/Counter2 (8-bit)
    // OCR2A //Timer/Counter2 Output Compare Register A
    // OCR2B //Timer/Counter2 Output Compare Register B

    cli(); //desabilita a interrupção global
        //TIMER 1 - para controle de leituras
        TCCR1A = 0;                         //confira timer para operação normal pinos OC1A e OC1B desconectados
        TCCR1B = 0;                         //limpa registrador
        TCCR1B |= (1<<CS10)|(1 << CS12);    // configura prescaler para 1024: CS12 = 1 e CS10 = 1
        
        TCNT1 = 0xC2F7;                     // incia timer com valor para que estouro ocorra em 1 segundo
                                            // 65536-(16MHz/1024/1Hz) = 49911 = 0xC2F7
        
        TIMSK1 |= (1 << TOIE1);             // habilita a interrupção do TIMER1

        //TIMER 2 - para as saidas 11 e 3 de uso do PWM
        TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
        TCCR2B = _BV(CS22);
        OCR2A = 180;
        OCR2B = 50;
    sei(); //habilita a interrupção global

 }

void loop() {
    if (oneSecond >= 1) {
        rtc.show(lcd,0,0);
        bme.show(lcd,0,1,BME280Types::TEMPERATURE);
        bme.show(lcd,0,2,BME280Types::HUMIDITY);
        tmp.show(lcd,0,3);
        hmdt.show(lcd,6,2);

        tmp.control(bme.getTemperature());

	    hmdt.control(bme.getHumidity());

        oneSecond = 0;
    }
}

ISR(TIMER1_OVF_vect)
{
    TCNT1 = 0xC2F7;                                 // Renicia TIMER

    oneSecond += 1;
    // tmp.control(bme.getTemperature());
    // tmp.show(lcd,0,3);
    // hmdt.control(bme.getHumidity());
    // hmdt.show(lcd,6,2);
}