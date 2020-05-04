#include "Arduino.h"
#include <LiquidCrystal.h>
#include "Sensor/RTC/NSE-DS3231.h"
#include "Sensor/Temperature/NSE-BME280.h"
#include "Control/Temperature/NSE-Temperature.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
DS3231 rtc;
BME280 bme;
Temperature controlPID(9);

void setup() {
    lcd.begin(20, 4);
    rtc.setup();
    bme.setup();
    controlPID.setup();
    controlPID.setPoint(27.27);
}

void loop() {
    
    rtc.show(lcd,0,0);
    bme.show(lcd,0,1,BME280Types::TEMPERATURE);
    bme.show(lcd,0,2,BME280Types::HUMIDITY);
    controlPID.control(bme.getTemperature(), lcd);
	delay(1000);
}