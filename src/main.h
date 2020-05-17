#ifndef __MAIN__H__
#define __MAIN__H__

#include "Arduino.h"
#include <LiquidCrystal.h>
#include "Sensor/RTC/NSE-DS3231.h"
#include "Sensor/Temperature/NSE-BME280.h"
#include "Control/PID/NSE-PID.h"
#include "Control/Humidity/NSE-Humidity.h"
#include "Control/Menu/NSE-Menu.h"

char *txtMENU[] = {
    "Set Point Temp.    ",
    "Set Ki Temperature ",
    "Set Kp Temperature ",
    "Set Kd Temperature ",
    "Set Point Humidity ",
    "Set Ki Humidity    ",
    "Set Kp Humidity    ",
    "Set Kd Humidity    ",
    "Salvar e Sair      ",
    "Sair               ",
};

LiquidCrystal lcd(7, 6, 5, 4, 9, 8);
Menu menu(lcd, txtMENU, 13,10,12);
DS3231 rtc;
BME280 bme;
PID tmp(3);
Humidity hmdt(17,11);
uint8_t _pinInterrupt = 2;

uint8_t fiveSeconds = 0;
uint8_t oneSecond = 0;

volatile boolean zero_cross = false;  // Boolean to store a "switch" to tell us if we have crossed zero

//65 = 60kz
//75 = 50hz
volatile uint8_t freqStep = 65;    // This is the delay-per-brightness step in microseconds.

volatile uint8_t step = 0; 

void setup();

void loop();

void interruptZeroCross(); 

#endif //__MAIN__H__