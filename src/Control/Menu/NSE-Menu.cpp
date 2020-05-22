#ifndef NSE_MENU_CPP
#define NSE_MENU_CPP

#include "NSE-Menu.h"
#include <EEPROM.h>


const char *txtFormats[] = {"C","F"};

Menu::Menu(LiquidCrystal &lcd, char *menuOptions[], uint8_t btnLeft, uint8_t btnRight, uint8_t btnUp, uint8_t btnDown, uint8_t enter)
: _lcd(lcd)
{
    _totOptions = 12;

    _txMENU = menuOptions;
    #if FIVE_BUTTONS==true
        _btnLeft = new Button(btnLeft, false, 10);
        _btnRight = new Button(btnRight, false, 10);
    #endif 
    _btnUp = new Button(btnUp, false, 10);
    _btnDown = new Button(btnDown, false, 10);
    _btnEnter = new Button(enter, false);


}

Menu::Menu(LiquidCrystal &lcd, char *menuOptions[], uint8_t btnUp, uint8_t btnDown, uint8_t enter)
: _lcd(lcd)
{
    _totOptions = 12;

    _txMENU = menuOptions;

    _btnUp = new Button(btnUp, false, 10);
    _btnDown = new Button(btnDown, false, 10);
    _btnEnter = new Button(enter, false);


}

Menu::~Menu() 
{
    #if FIVE_BUTTONS==true
        delete _btnLeft;
        delete _btnRight;
    #endif
    delete _btnUp;
    delete _btnDown;
    delete _btnEnter;
}

void Menu::setup(void (*__upParams)(MYDATA))
{
    // memory.d.date.dia = 17;
    // memory.d.date.mes = 05;
    // memory.d.date.ano = 2020;
    // memory.d.time.hora = 10;
    // memory.d.time.minuto = 25;
    // memory.d.time.segundo = 45;
    // memory.d.tempSetPoint = 10.0;
    // memory.d.tempSetKi = 1.0;
    // memory.d.tempSetKp = 1.0;
    // memory.d.tempSetKd = 1.0;

    // memory.d.hmdtSetPoint = 10.0;
    // memory.d.hmdtSetKi = 1.0;
    // memory.d.hmdtSetKp = 1.0;
    // memory.d.hmdtSetKd = 1.0;

    _upParams = __upParams; 
    
    readConfiguration();
    (*_upParams)(memory.d);

    _lcd.createChar(_iARROW, _bARROW);

    _lcd.clear();
}

void Menu::loop()
{
    btnPressed = readButtons();

    if( btnPressed == MENUBUTTON::Ok ){
        openMenu();
    }
}

void Menu::render(char * render, byte x, byte y)
{
    _lcd.setCursor(x,y);
    _lcd.print(render);
}

void Menu::openMenu()
{
    byte idxMenu       = 0;
    boolean exitMenu   = false;
    boolean forcePrint = true;
    
    int optionControl = 1;
    char format[_columnsLCD];

    int graphMenu     = 0;


    _lcd.clear();

    while( !exitMenu )
    {
        btnPressed = readButtons();

        if( btnPressed == MENUBUTTON::Up )
        {
            if (idxMenu-1 >= 0) {
                idxMenu--;
            } else {
                idxMenu = (_totOptions -1);
            }

            optionControl = +1;

            graphMenu = (idxMenu >= _rowsLCD?(idxMenu - _rowsLCD)+1:0); 
       }
        
        if( btnPressed == MENUBUTTON::Down )
        {
            if (idxMenu+1 < _totOptions) {
                idxMenu++;
            } else {
                idxMenu = 0;
            }

            optionControl = -1;

            graphMenu = (idxMenu >= _rowsLCD?(idxMenu - _rowsLCD)+1:0);    
                    
        }
        
        if( btnPressed == MENUBUTTON::Ok )
        {
            switch( idxMenu )
            {
                case 0:
                        openSubMenu( idxMenu, MENUTYPE::Date,   &memory.d.date); 
                    break;
                case 1:
                        openSubMenu( idxMenu, MENUTYPE::Time,   &memory.d.time); 
                    break;
                case 2:     
                        sprintf(format,"%c%c",char(223),'C');
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.tempSetPoint, -20.0, 99.9, format); 
                    break;
                case 3:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.tempSetKi, -20.0, 99.9,(char*)"Ki"); 
                    break;
                case 4:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.tempSetKp, -20.0, 99.9,(char*)"Kp"); 
                    break;
                case 5:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.tempSetKd, -20.0, 99.9,(char*)"Kd"); 
                    break;
                case 6:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.hmdtSetPoint, 0.0, 99.9,(char*)"%"); 
                    break;
                case 7:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.hmdtSetKi, -20.0, 99.9,(char*)"Ki"); 
                    break;
                case 8:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.hmdtSetKp, -20.0, 99.9,(char*)"Kp"); 
                    break;
                case 9:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.hmdtSetKd, -20.0, 99.9,(char*)"Kd"); 
                    break;
                // case 1: openSubMenu( idxMenu, MENUTYPE::Number,  &memory.d.delay, 0, 60, "s"    ); break;
                // case 2: openSubMenu( idxMenu, MENUTYPE::List,  &memory.d.format,  txtFormats    ); break;
                // case 3: 
                //         sprintf(format,"%c%c",char(223),getTemperatureFormat());
                //         openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.offsetTemp, -20.0, 99.9, format); 
                //     break;
                case 10: writeConfiguration(); exitMenu = true;                                     break;
                case 11: readConfiguration();  exitMenu = true;                                     break;
            }
            forcePrint = true;
        }


        if( !exitMenu && (forcePrint || btnPressed != MENUBUTTON::Unknown) )
        {
            forcePrint = false;

            byte endFor2 = graphMenu+_rowsLCD;

            //Criar o tamanho de colunas em branco conforme a config do display.
            String print;            
            for(unsigned int y=0; y < _columnsLCD; y++)
            {
                print += ' ';
            }

            for( int i=graphMenu, j=0; i< endFor2 ; i++, j++ )
            {
                _lcd.setCursor(1, j);
                if (i<_totOptions) {
                    _lcd.print(_txMENU[i]);
                } else {
                    _lcd.print(print);
                }
                // _lcd.setCursor(15, j);
                // _lcd.print(i);
            }
            
            _lcd.setCursor(0, ((idxMenu - graphMenu)+optionControl) % _rowsLCD );
            _lcd.print(" ");
            _lcd.setCursor(0, (idxMenu - graphMenu) % _rowsLCD );
            _lcd.write(_iARROW);
        }
    }

    _lcd.clear();
}

void Menu::openSubMenu(byte menuID, MENUTYPE screen, struct TIME *timeStruct) {
    boolean exitSubMenu = false;
    boolean forcePrint  = true;
    uint8_t countToExit = 0;

    unsigned int minValue = 0;
    unsigned int maxValue = 23;

    unsigned int *value;

    char linhaDisplay[20];

    _lcd.clear();

    value = (&(*timeStruct).hora);

    while( !exitSubMenu )
    {
        btnPressed = readButtons();

        if( btnPressed == MENUBUTTON::Ok )
        {
            countToExit ++;
            forcePrint = true;
            switch (countToExit)
            {
                case 1 :
                        (*timeStruct).hora = (*value);
                        value = (&(*timeStruct).minuto);
                        minValue = 0;
                        maxValue = 59;
                    break;
                case 2 :
                        (*timeStruct).minuto = (*value);
                        value = (&(*timeStruct).segundo);
                        minValue = 0;
                        maxValue = 59;
                    break;
                
                default: //acima de 3 sai do menu
                        (*timeStruct).segundo = (*value);
                        exitSubMenu = true;
                    break;
            }
        }
        else if( btnPressed == MENUBUTTON::Up && (*value)-1 >= minValue )
        {
            (*value)--;
        }
        else if( btnPressed == MENUBUTTON::Down && (*value)+1 <= maxValue )
        {
            (*value)++;
        }


        if( !exitSubMenu && (forcePrint || btnPressed != MENUBUTTON::Unknown) )
        {
            forcePrint = false;

            switch (countToExit) {
                case 0 :
                        _lcd.setCursor(0,0);
                        _lcd.print(_txMENU[menuID]);
                        _lcd.setCursor(0,1);
                        sprintf(linhaDisplay,"     <%02u>:%02u:%02u     ",(*value),(*timeStruct).minuto,(*timeStruct).segundo);
                        _lcd.print(linhaDisplay);
                    break;
                case 1 :
                        _lcd.setCursor(0,0);
                        _lcd.print(_txMENU[menuID]);
                        _lcd.setCursor(0,1);
                        sprintf(linhaDisplay,"     %02u:<%02u>:%02u     ",(*timeStruct).hora,(*value),(*timeStruct).segundo);
                        _lcd.print(linhaDisplay);
                    break;
                case 2 :
                        _lcd.setCursor(0,0);
                        _lcd.print(_txMENU[menuID]);
                        _lcd.setCursor(0,1);
                        sprintf(linhaDisplay,"     %02u:%02u:<%02u>     ",(*timeStruct).hora,(*timeStruct).minuto,(*value));
                        _lcd.print(linhaDisplay);
                    break;
            }
         
        }

    }

    _lcd.clear();
}

void Menu::openSubMenu(byte menuID, MENUTYPE screen, struct DATE *dataStruct) {
    boolean exitSubMenu = false;
    boolean forcePrint  = true;
    uint8_t countToExit = 0;

    unsigned int minValue = 0;
    unsigned int maxValue = 31;

    unsigned int *value;

    char linhaDisplay[20];

    _lcd.clear();

    value = (&(*dataStruct).dia);

    while( !exitSubMenu )
    {
        btnPressed = readButtons();

        if( btnPressed == MENUBUTTON::Ok )
        {
            countToExit ++;
            forcePrint = true;
            switch (countToExit)
            {
                case 1 :
                        (*dataStruct).dia = (*value);
                        value = (&(*dataStruct).mes);
                        minValue = 1;
                        maxValue = 12;
                    break;
                case 2 :
                        (*dataStruct).mes = (*value);
                        value = (&(*dataStruct).ano);
                        minValue = 2020;
                        maxValue = 2090;
                    break;
                
                default: //acima de 3 sai do menu
                        (*dataStruct).ano = (*value);
                        exitSubMenu = true;
                    break;
            }
        }
        else if( btnPressed == MENUBUTTON::Up && (*value)-1 >= minValue )
        {
            (*value)--;
        }
        else if( btnPressed == MENUBUTTON::Down && (*value)+1 <= maxValue )
        {
            (*value)++;
        }


        if( !exitSubMenu && (forcePrint || btnPressed != MENUBUTTON::Unknown) )
        {
            forcePrint = false;

            switch (countToExit) {
                case 0 :
                        _lcd.setCursor(0,0);
                        _lcd.print(_txMENU[menuID]);
                        _lcd.setCursor(0,1);
                        sprintf(linhaDisplay,"    <%02u>/%02u/%04u    ",(*value),(*dataStruct).mes,(*dataStruct).ano);
                        _lcd.print(linhaDisplay);
                    break;
                case 1 :
                        _lcd.setCursor(0,0);
                        _lcd.print(_txMENU[menuID]);
                        _lcd.setCursor(0,1);
                        sprintf(linhaDisplay,"    %02u/<%02u>/%04u    ",(*dataStruct).dia,(*value),(*dataStruct).ano);
                        _lcd.print(linhaDisplay);
                    break;
                case 2 :
                        _lcd.setCursor(0,0);
                        _lcd.print(_txMENU[menuID]);
                        _lcd.setCursor(0,1);
                        sprintf(linhaDisplay,"    %02u/%02u/<%04u>    ",(*dataStruct).dia,(*dataStruct).mes,(*value));
                        _lcd.print(linhaDisplay);
                    break;
            }
         
        }

    }

    _lcd.clear();
}


void Menu::openSubMenu (byte menuID, MENUTYPE screen, int * value, int minValue, int maxValue, char * format = (char*)"")
{
    boolean exitSubMenu = false;
    boolean forcePrint  = true;

    _lcd.clear();

    while( !exitSubMenu )
    {
        btnPressed = readButtons();

        if( btnPressed == MENUBUTTON::Ok )
        {
            exitSubMenu = true;
        }
        else if( btnPressed == MENUBUTTON::Up && (*value)-1 >= minValue )
        {
            (*value)--;
        }
        else if( btnPressed == MENUBUTTON::Down && (*value)+1 <= maxValue )
        {
            (*value)++;
        }


        if( !exitSubMenu && (forcePrint || btnPressed != MENUBUTTON::Unknown) )
        {
            forcePrint = false;

            _lcd.setCursor(0,0);
            _lcd.print(_txMENU[menuID]);

            _lcd.setCursor(0,1);
            _lcd.print("<");
            _lcd.setCursor(_columnsLCD-1,1);
            _lcd.print(">");

            switch (screen) {
                case MENUTYPE::Flag :
                        _lcd.setCursor(_columnsLCD/2-1, 1);
                        _lcd.print((*value) == 0 ? "Não" : "Sim");
                    break;
                case MENUTYPE::Number :
                        _lcd.setCursor(_columnsLCD/2-1, 1);
                        _lcd.print((*value));
                        _lcd.print(format);
                        _lcd.print(" ");
                    break;
                case MENUTYPE::Float :
                        _lcd.setCursor(_columnsLCD/2-1, 1);
                        _lcd.print((*value));
                        _lcd.print(" ");
                    break;
                default:
                    break;
            }
        }

    }

    _lcd.clear();
}

void Menu::readConfiguration()
{
    for( unsigned int i=0 ; i < sizeof(memory.d) ; i++ ) {
        memory.b[i] = EEPROM.read(i);
    }
    writeConfiguration();
}

void Menu::writeConfiguration()
{
    for( unsigned int i=0 ; i<sizeof(memory.d) ; i++ ) {
        EEPROM.write( i, memory.b[i] );
    }

    (*_upParams)(memory.d);
}

/**
 *
 * @param menuID    ID do menu principal para colocar o titulo no submenu
 * @param screen    Tipo que representa o submenu
 * @param value     Ponteiro para o valor na STRUCT da EEPROM
 * @param minValue  Valor minimo que pode ter a variavel.
 * @param maxValue  Valor maximo que pode ter a variavel.
 */

void Menu::openSubMenu (byte menuID, MENUTYPE screen, float *value, float minValue, float maxValue, char * format = (char*)"")
{
    boolean exitSubMenu = false;
    boolean forcePrint  = true;
    boolean isFloat = true;
    float valueAction = 0.1;

    _lcd.clear();

    while( !exitSubMenu )
    {
        btnPressed = readButtons();

        if( btnPressed == MENUBUTTON::Ok )
        {
            if(!isFloat) {
                exitSubMenu = true;
            }
            valueAction = 1;
            isFloat = false;
        }
        else if( btnPressed == MENUBUTTON::Up && (*value)-valueAction >= minValue )
        {
            (*value) -= valueAction;
        }
        else if( btnPressed == MENUBUTTON::Down && (*value)+valueAction <= maxValue )
        {
            (*value) += valueAction;
        }

        if( !exitSubMenu && (forcePrint || btnPressed != MENUBUTTON::Unknown) )
        {
            forcePrint = false;

            _lcd.setCursor(0,0);
            _lcd.print(_txMENU[menuID]);

            _lcd.setCursor(0,1);
            _lcd.print("<");
            _lcd.setCursor(_columnsLCD-1,1);
            _lcd.print(">");

            _lcd.setCursor(_columnsLCD/2-5, 1);
            
            // char display[_columnsLCD] = "";
            _lcd.print((*value));
            // sprintf(display,"%c%c",char(223),getTemperatureFormat());

            _lcd.print(format);
            _lcd.print(" ");
        }
    }
    _lcd.clear();
}

void Menu::openSubMenu (byte menuID, MENUTYPE screen, byte * value, const char * options[])
{
    boolean exitSubMenu = false;
    boolean forcePrint  = true;

    int minValue = 0;
    int maxValue = sizeof(*options)-1;

    _lcd.clear();

    while( !exitSubMenu )
    {
        btnPressed = readButtons();

        if( btnPressed == MENUBUTTON::Ok )
        {
            exitSubMenu = true;
        }
        else if( btnPressed == MENUBUTTON::Up && (*value)-1 >= minValue )
        {
            (*value) --;
        }
        else if( btnPressed == MENUBUTTON::Down && (*value)+1 <= maxValue )
        {
            (*value) ++;
        }

        if( !exitSubMenu && (forcePrint || btnPressed != MENUBUTTON::Unknown) )
        {
            _lcd.setCursor(0,0);
            _lcd.print(_txMENU[menuID]);

            _lcd.setCursor(0,1);
            _lcd.print("<");
            _lcd.setCursor(_columnsLCD-1,1);
            _lcd.print(">");

            _lcd.setCursor(_columnsLCD/2-1, 1);
            _lcd.print(options[(*value)]);
            _lcd.print(" ");

            forcePrint = false;        
        }
    }
    _lcd.clear();
}


MENUBUTTON Menu::readButtons()
{
    btnPressed = MENUBUTTON::Unknown;
    #if FIVE_BUTTONS==true
        if (_btnLeft->check(HIGH)) {
            btnPressed = MENUBUTTON::Left;
        }

        if (_btnRight->check(HIGH)) {
            btnPressed = MENUBUTTON::Right;
        }
    #endif

    if (_btnUp->check(HIGH)) {
        btnPressed = MENUBUTTON::Up;
    }

    if (_btnDown->check(HIGH)) {
        btnPressed = MENUBUTTON::Down;
    }

    if (_btnEnter->check(HIGH)) {
        btnPressed = MENUBUTTON::Ok;
    }

    return btnPressed;
}

#endif