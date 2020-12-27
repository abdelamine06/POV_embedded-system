#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/timer.h"
#include "../headers/usart.h"

int time_register_compare = 0;
int time_seconds;


void TIMER_Init()
{
    time_seconds = 0;

    // The counting sequence is determined by the setting of the WGM01 and WGM00 
    // bits located in the Timer/Counter control register (TCCR0A) and the WGM02 
    // bit located in the Timer/Counter control 
    // register B (TCCR0B).

    TCCR0A |= (1 << WGM01) | (0 << WGM00);
    TCCR0B |= (0 << WGM02); // active Time/count0
    OCR0A = 125; // output compare register
    TIMSK0 |= (1 << OCIE0A); // active interrupt 
    TCCR0B |= (1<< CS00) | (1 << CS01) | (0 << CS02); // clkI/O/64 (from prescaler)
}

void Update_Time()
{
    if (time_register_compare == 1625)
    {
        time_seconds++;
        char number_str[128];
        sprintf(number_str, "%d", time_seconds);
        USART_Transmit_String_Interrupt(number_str);
        USART_Transmit_String_Interrupt("\r\n");


        time_register_compare = 0;
    }
}

/*
 * return time seconds
 */
int Millis()
{
    return time_seconds;
}

ISR(TIMER0_COMPA_vect){
    time_register_compare++;
}

