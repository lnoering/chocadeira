#ifndef __MAIN__H__
#define __MAIN__H__

#include "Arduino.h"
#include <LiquidCrystal.h>
#include "Sensor/RTC/NSE-DS3231.h"
#include "Sensor/Temperature/NSE-BME280.h"
#include "Control/PID/NSE-PID.h"
#include "Control/Humidity/NSE-Humidity.h"

LiquidCrystal lcd(7, 6, 5, 4, 9, 8);
DS3231 rtc;
BME280 bme;
PID tmp(3);
Humidity hmdt(17,11);
uint8_t _pinInterrupt = 2;

uint8_t fiveSeconds = 0;
uint8_t oneSecond = 0;

volatile boolean zero_cross = false;  // Boolean to store a "switch" to tell us if we have crossed zero
volatile int freqStep = 65;    // This is the delay-per-brightness step in microseconds.
volatile int debouceFreqStep = (freqStep - 3);
volatile int dim = 0;
volatile int step = 0; 

volatile byte state = LOW;

void setup();

void loop();

void interruptZeroCross(); 

#endif //__MAIN__H__