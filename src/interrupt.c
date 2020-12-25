#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void Interrupt_Init() 
{
    sei(); // Enable interrupts
}
