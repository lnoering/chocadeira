#ifndef NSE_DS3231_CPP
#define NSE_DS3231_CPP

#include "NSE-DS3231.h"

DS3231::DS3231()
{
    _rtc = new RtcDS3231<TwoWire>(Wire);
}

DS3231::~DS3231()
{
    free(_rtc);
}

void DS3231::setup()
{
    _rtc->Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    if (!_rtc->IsDateTimeValid()) 
    {
        if (_rtc->LastError() != 0)
        {
            //TODO - Mostrar o erro
            // lcd.setCursor(0,0);
            // lcd.println("RTC error =        ");
            // char display[20];
            // sprintf(display,"%s",_rtc->LastError());
            // lcd.print(display);
        }
        else
        {
            _rtc->SetDateTime(compiled);
        }
    }
  
    if (!_rtc->GetIsRunning())
    {
        _rtc->SetIsRunning(true);
    }

    RtcDateTime now = _rtc->GetDateTime();
    if (now < compiled) 
    {
        _rtc->SetDateTime(compiled);
    }

    _rtc->Enable32kHzPin(false);
    _rtc->SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
}

bool DS3231::isOperating()
{
    if (!_rtc->IsDateTimeValid()) 
    {
        //TODO criar template de alarme

        // if (rtc.LastError() != 0)
        // {
        //     lcd.setCursor(0,0);
        //     lcd.println("RTC error =        ");
        //     char display[20];
        //     sprintf(display,"%s",rtc.LastError());
        //     lcd.print(display);
        // }
        // else
        // {
        //     lcd.setCursor(0,0);
        //     lcd.print("Battery / Power off");
        // }
        return false;
    }

    return true;
}

RtcDateTime DS3231::getDateTime()
{
    return _rtc->GetDateTime();
}

void DS3231::show(LiquidCrystal& lcd, uint8_t col, uint8_t lin)
{
    RtcDateTime now = this->getDateTime();
    char display[20];

    lcd.setCursor(0,lin);
    sprintf(display,
            "%02u/%02u/%02u %02u:%02u:%02u",
            now.Day(),
            now.Month(),
            now.Year(),
            now.Hour(),
            now.Minute(),
            now.Second() );
    lcd.print(display);
}

void DS3231::setDateTime(RtcDateTime datetime)
{
    _rtc->SetDateTime(datetime);
}


#endif