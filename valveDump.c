/**
* @file     valveDump.C
* @Author   Ben S. Eishen(ben@eishen.com)
* @date     November, 2013
* @brief    Moisture blow off valve timer for air compressor.
*
* This software is designed to routinely(curently 1 week intervals) blow off condensate in an air compressor. The software
* is written for the ATMEL ATtiny24 microcontroller. Features include an led light that flashes every minute, a switch that alows manual blow off control.
* 
* PIN MAPPING
* PA0 - PUSHBUTTON MANUAL DUMP
* PB0 - STATUS LED
* PB1 - VALVE TRIGGER(ACTIVE HIGH)
*/

#define F_CPU 1000000UL

#define MINUTE_INTERVAL 10080 //10080 minutes in a week.

#define set_bit(reg,bit) reg |= (1<<bit)
#define clr_bit(reg,bit) reg &= ~(1<<bit)
#define check_bit(reg,bit) (reg&(1<<bit))
#define ledOn() set_bit(PORTB,0)
#define ledOff() clr_bit(PORTB,0)

#include <stdint.h>
#include <stdbool.h> 
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <avr/io.h>

volatile unsigned int minuteCount = 0;
volatile unsigned char manualOveride = 0;

void init(){
    clr_bit(DDRA,0);
    set_bit(PORTA,0);
    set_bit(DDRB,0);
    set_bit(DDRB,1);
   
    //setup Timer for CTC
    TIMSK1 |= (1 << TOIE1); // Enable overflow interrupt 
    TCNT1 = 0x1B1D;// Preload timer with precalculated value, every 1 minute.
    TCCR1B |= ((1 << CS10) | (1 << CS12)); // Set up timer at Fcpu/1024 
   
    //setup PinChange interrupt
    PCMSK0 |= (1 << PCINT0);
    GIMSK |= (1 << PCIE0);    

    sei();
}

void dumpValve(){

    set_bit(PORTB,1);
    _delay_ms(5000);
    clr_bit(PORTB,1);
    _delay_ms(1000);

    set_bit(PORTB,1);
    _delay_ms(500);
    clr_bit(PORTB,1);
    _delay_ms(500);

    set_bit(PORTB,1);
    _delay_ms(500);
    clr_bit(PORTB,1);
    _delay_ms(500);

    set_bit(PORTB,1);
    _delay_ms(500);
    clr_bit(PORTB,1);
    _delay_ms(500);
}




int main(){

    init();

    for (;;) 
    { 
        
        if(minuteCount>MINUTE_INTERVAL){
            minuteCount = 0;
            ledOn();
            dumpValve();
            ledOff();
        }
        if(manualOveride){
            ledOn();
            dumpValve();
            ledOff();
            manualOveride = 0;
        }

    } 

}


ISR(TIM1_OVF_vect) 
{ 
   TCNT1=0x1B1D;
   ledOn();
   _delay_ms(500);
   ledOff();
   minuteCount++;
}

ISR(PCINT0_vect){
        manualOveride = 1;
        _delay_ms(50);
}
