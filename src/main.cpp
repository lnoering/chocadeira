#ifndef __MAIN__CPP__
#define __MAIN__CPP__

#include "main.h"


void setup() {
    lcd.begin(20, 4);
    
    rtc.setup();
    
    bme.setup();

    tmp.setup();
    tmp.setPoint(22.27);

    hmdt.setup();
    hmdt.setPoint(70.5);

    noInterrupts(); //desabilita a interrupção global
        //TIMER 1 - para controle de leituras
        TCCR1A = 0;                         //confira timer para operação normal pinos OC1A e OC1B desconectados
        TCCR1B = 0;                         //limpa registrador
        TCCR1B |= (1<<CS10)|(1 << CS12);    // configura prescaler para 1024: CS12 = 1 e CS10 = 1
        
        TCNT1 = 0xC2F7;                     // incia timer com valor para que estouro ocorra em 1 segundo
                                            // 65536-(16MHz/1024/1Hz) = 49911 = 0xC2F7
        
        TIMSK1 |= (1 << TOIE1);             // habilita a interrupção do TIMER1

        //TIMER 2
		TCCR2A = 0; // set entire TCCR2A register to 0
		TCCR2B = 0; // same for TCCR2B
		TCNT2  = 0; // initialize counter value to 0
		// set compare match register for 8333.333333333334 Hz increments
		OCR2A = 239; // = 16000000 / (8 * 8333.333333333334) - 1 (must be <256)
		// turn on CTC mode
		TCCR2B |= (1 << WGM21);
		// Set CS22, CS21 and CS20 bits for 8 prescaler
		TCCR2B |= (0 << CS22) | (1 << CS21) | (0 << CS20);
		// enable timer compare interrupt
		TIMSK2 |= (1 << OCIE2A);
    interrupts(); //habilita a interrupção global

    pinMode(_pinInterrupt, INPUT_PULLUP);              
    attachInterrupt(digitalPinToInterrupt(_pinInterrupt), interruptZeroCross, FALLING);
	pinMode(16, OUTPUT);  
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

void interruptZeroCross()
{  
	if (step > debouceFreqStep || !zero_cross) {
		// digitalWrite(AC_pin, LOW);       // turn off TRIAC (and AC)
		TCNT2  = 0;
		step = 0;
		zero_cross = true;   
		digitalWrite(16, state);
      	state = !state;
	}
}

// Turn on the TRIAC at the appropriate time
ISR(TIMER2_COMPA_vect){        
    if(zero_cross) {
        if(step >= tmp.getPwmOut()) { //tmp.getPwmOut()
			tmp.setOutput(false);
        } else {
            tmp.setOutput(true);
        }

		if(step >= hmdt.getPwmOut()) {
            hmdt.setOutput(false);
        } else {
			hmdt.setOutput(true);
        }

        step ++;       
        if (step >= freqStep) {
            zero_cross = false; //reset zero cross detection
            step = 0;
        }           
                
    }    
}  

#endif //__MAIN__CPP__