#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../headers/hall.h"
#include "../headers/usart.h"
#include "../headers/interrupt.h"
#include "../headers/spi.h"

volatile int time_rotation = 0;
volatile int number_rotation =0;
void Hall_Init()
{
    DDRD &= ~(1 << DDD2);     
    PORTD |= (1 << PORTD2);  
    EIMSK |= (1 << INT0);     // Enable interrupt
}


// ********************* Interrupt *************************

ISR (INT0_vect)
{
    time_rotation = TCNT1;
    TCNT1 = 0;
    number_rotation++;
}
