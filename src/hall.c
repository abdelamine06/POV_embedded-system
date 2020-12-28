#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../headers/hall.h"
#include "../headers/usart.h"
#include "../headers/interrupt.h"


void Hall_Init()
{
    number_rotation = 0;
    time_last_rotation = 0;
    DDRD &= ~(1 << DDD2);     
    PORTD |= (1 << PORTD2);  
    EICRA |= (1 << ISC00) | (1 << ISC01); 
    EIMSK |= (1 << INT0);     // Enable interrupt
}


// ********************* Interrupt *************************

ISR (INT0_vect)
{
    time_last_rotation = TCNT1;
    TCNT1 = 0;
    number_rotation++;
}
