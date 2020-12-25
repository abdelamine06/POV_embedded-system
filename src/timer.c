#include <avr/io.h>
#include <util/delay.h>


void TIMER_Init()
{
    // Valeur du registre TWI
    TWBR = 12;
    // Timer Counter1
    TCCR1B = (1<<CS12) | (1<<CS10); 
}