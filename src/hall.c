#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../headers/hall.h"
#include "../headers/usart.h"
#include "../headers/interrupt.h"
#include "../headers/spi.h"
#include "../headers/timer.h"

void Hall_Init()
{
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);
    EIMSK |= (1 << INT0);     // Enable interrupt

}

// ********************* Interrupt *************************

ISR (INT0_vect)
{
    update_time_register();
    tcnt1_value = TCNT1;
    TCNT1 = 0;
}

