#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/timer.h"
#include "../headers/usart.h"
#include "../headers/spi.h"

int time_register_compare = 0;
int time_seconds;

void TIMER_Init()
{

    // The counting sequence is determined by the setting of the WGM01 and WGM00 
    // bits located in the Timer/Counter control register (TCCR0A) and the WGM02 
    // bit located in the Timer/Counter control 
    // register B (TCCR0B).

    // TCCR0A |= (1 << WGM01) | (0 << WGM00);
    TCCR0B |= (0 << WGM02); // Active Time/count0 
    OCR0A = 125; // output compare register, (TCNT0). 
                 // Defines the top value for the counter, hence also its 
                 // resolution
    TIMSK0 |= (1 << OCIE0A); // active interrupt 
    TCCR0B |= (1<< CS00) | (1 << CS01) | (0 << CS02); // clkI/O/64 (from prescaler)

    TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);// Prescaler to 1024. Timer1(16-bit)
}

void Update_Time()
{
    if (time_register_compare == 1625)
    {
        time_seconds++;
        char number_str[128];
        
        time_register_compare = 0;
    }
}

/*
 * return time seconds
 */
int Seconds()
{
    return time_seconds;
}

// ******************* interrupt *************************

ISR(TIMER0_COMPA_vect){
    time_register_compare++;
}


