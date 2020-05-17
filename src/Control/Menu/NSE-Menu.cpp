#ifndef NSE_MENU_CPP
#define NSE_MENU_CPP

#include "NSE-Menu.h"

const char *txtFormats[] = {"C","F"};

Menu::Menu(LiquidCrystal &lcd, char *menuOptions[], uint8_t btnLeft, uint8_t btnRight, uint8_t btnUp, uint8_t btnDown, uint8_t enter)
: _lcd(lcd)
{
    _totOptions = 10;

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
    _totOptions = 10;

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

void Menu::setup()
{
    memory.d.tempSetPoint = 10.0;
    memory.d.tempSetKi = 1.0;
    memory.d.tempSetKp = 1.0;
    memory.d.tempSetKd = 1.0;

    memory.d.hmdtSetPoint = 10.0;
    memory.d.hmdtSetKi = 1.0;
    memory.d.hmdtSetKp = 1.0;
    memory.d.hmdtSetKd = 1.0;
    writeConfiguration();

    readConfiguration();

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
    
    int line = 0;
    int optionControl = 1;
    unsigned int invert= 0;
    char format[_columnsLCD];

    int graphMenu     = 0;
    int oldOptControl     = 1;
    int lineWalk      = 0;
    int countAction = 0;


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
                        sprintf(format,"%c%c",char(223),'C');
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.tempSetPoint, -20.0, 99.9, format); 
                    break;
                case 1:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.tempSetKi, -20.0, 99.9,'Ki'); 
                    break;
                case 2:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.tempSetKp, -20.0, 99.9,'Kp'); 
                    break;
                case 3:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.tempSetKd, -20.0, 99.9,'Kd'); 
                    break;
                case 4:     
                        sprintf(format,"%c%c",char(223),'C');
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.hmdtSetPoint, -20.0, 99.9, format); 
                    break;
                case 5:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.hmdtSetKi, -20.0, 99.9,'Ki'); 
                    break;
                case 6:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.hmdtSetKp, -20.0, 99.9,'Kp'); 
                    break;
                case 7:
                        openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.hmdtSetKd, -20.0, 99.9,'Kd'); 
                    break;
                // case 1: openSubMenu( idxMenu, MENUTYPE::Number,  &memory.d.delay, 0, 60, "s"    ); break;
                // case 2: openSubMenu( idxMenu, MENUTYPE::List,  &memory.d.format,  txtFormats    ); break;
                // case 3: 
                //         sprintf(format,"%c%c",char(223),getTemperatureFormat());
                //         openSubMenu( idxMenu, MENUTYPE::Float,   &memory.d.offsetTemp, -20.0, 99.9, format); 
                //     break;
                case 8: writeConfiguration(); exitMenu = true;                                     break;
                case 9: readConfiguration();  exitMenu = true;                                     break;
            }
            forcePrint = true;
        }


        if( !exitMenu && (forcePrint || btnPressed != MENUBUTTON::Unknown) )
        {
            forcePrint = false;

            static const byte endFor1 = (_totOptions+_rowsLCD-1)/_rowsLCD;

            // line = (idxMenu - graphMenu) % _rowsLCD;
            byte endFor2 = graphMenu+_rowsLCD;

            //Criar o tamanho de colunas em branco conforme a config do display.
            String print;            
            for(char y=0; y < _columnsLCD; y++)
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


                _lcd.setCursor(15, j);
                _lcd.print(i);
                _lcd.print(idxMenu);
                _lcd.print(graphMenu);
                // _lcd.print((idxMenu ) % _rowsLCD);

                _lcd.print(line);
            }
            
            _lcd.setCursor(0, ((idxMenu - graphMenu)+optionControl) % _rowsLCD );
            _lcd.print(" ");
            _lcd.setCursor(0, (idxMenu - graphMenu) % _rowsLCD );
            _lcd.write(_iARROW);
        }
    }

    _lcd.clear();
}



void Menu::openSubMenu (byte menuID, MENUTYPE screen, int * value, int minValue, int maxValue, char * format = "")
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
    for( int i=0 ; i < sizeof(memory.d) ; i++  )
        memory.b[i] = EEPROM.read(i);

    // memory.d.offsetTemp = 0.5;

    writeConfiguration();
}

void Menu::writeConfiguration()
{
    for( int i=0 ; i<sizeof(memory.d) ; i++  )
        EEPROM.write( i, memory.b[i] );
}

/**
 *
 * @param menuID    ID do menu principal para colocar o titulo no submenu
 * @param screen    Tipo que representa o submenu
 * @param value     Ponteiro para o valor na STRUCT da EEPROM
 * @param minValue  Valor minimo que pode ter a variavel.
 * @param maxValue  Valor maximo que pode ter a variavel.
 */

void Menu::openSubMenu (byte menuID, MENUTYPE screen, float *value, float minValue, float maxValue, char * format = "")
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
    int maxValue = sizeof(options)-1;

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

float Menu::getSetPoint()
{
    // return memory.d.setPoint;
}

int Menu::getOffsetRele()
{
    // return memory.d.delay;
}

char Menu::getTemperatureFormat()
{
    // return *txtFormats[memory.d.format];
}

float Menu::getTemperatureOffset()
{
    // return memory.d.offsetTemp;
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