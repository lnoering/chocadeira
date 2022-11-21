#pragma once
#ifndef NSE_MENU_H
#define NSE_MENU_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "../../Sensor/Button/NSE-Button.h"

#if !defined(ARRAY_SIZE)
    #define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

#define FIVE_BUTTONS false

#if FIVE_BUTTONS==true
    enum MENUBUTTON { Unknown, Ok, Up, Down, Left, Right };
#else
    enum MENUBUTTON { Unknown, Ok, Up, Down};
#endif
enum MENUTYPE { Menu1, Menu2, Flag, Number, Float, List, Date, Time };

struct TIME{
    unsigned int hora;
    unsigned int minuto;
    unsigned int segundo;
};

struct DATE{
    unsigned int dia;
    unsigned int mes;
    unsigned int ano;
};

/* CONFIGURACOES */
struct MYDATA{             // Estrutura STRUCT com as variables que armazenarao ds datos que na memoria EEPROM
    DATE date;
    TIME time;
    float tempSetPoint;
    float tempSetKi;
    float tempSetKp;
    float tempSetKd;
    float hmdtSetPoint;
    float hmdtSetKi;
    float hmdtSetKp;
    float hmdtSetKd;
};
union MEMORY{             // Estrutura UNION para facilitar a leitura e escrita na EEPROM da estrutura STRUCT
    MYDATA d;
    byte b[sizeof(MYDATA)];
};

class Menu
{
    public:
        Menu(LiquidCrystal &lcd, char *menuOptions[], uint8_t btnLeft, uint8_t btnRight, uint8_t btnUp, uint8_t btnDown, uint8_t enter);
        Menu(LiquidCrystal &lcd, char *menuOptions[], uint8_t btnUp, uint8_t btnDown, uint8_t enter);
        virtual ~Menu(); 
        void control(float temperature);
    
        void setup(void (*__upParams)(MYDATA));
        void loop(void (*controls)());
        void render(char * render, byte x, byte y);
        void openMenu(void (*controls)());    
        void readConfiguration();
        void writeConfiguration();
        MENUBUTTON readButtons();
        void openSubMenu(byte menuID, MENUTYPE screen, float *value, float minValue, float maxValue, char * format);
        void openSubMenu(byte menuID, MENUTYPE screen, int *value, int minValue, int maxValue, char * format);
        void openSubMenu (byte menuID, MENUTYPE screen, byte * value, const char * options[]);
        void openSubMenu(byte menuID, MENUTYPE screen, struct DATE *dataStruct);
        void openSubMenu(byte menuID, MENUTYPE screen, struct TIME *timeStruct);

    private:
        float           _temperature;
        unsigned int    _timeToControl;
        uint8_t         _pin;
        unsigned int    _timeCount;
        MENUBUTTON btnPressed;
        MEMORY memory;

        LiquidCrystal &_lcd;
        unsigned int _columnsLCD = 20;    
        unsigned int _rowsLCD = 4;    
        
        char **_txMENU;
        byte _totOptions;

        const byte _iARROW = 0;
        byte _bARROW[8] = {
            B00000, B00100, B00110, B11111,
            B00110, B00100, B00000, B00000
        };

        #if FIVE_BUTTONS==true
            Button *_btnLeft;
            Button *_btnRight;
        #endif
        Button *_btnUp;
        Button *_btnDown;
        Button *_btnEnter;

        void (*_upParams)(MYDATA);          
};

#endif