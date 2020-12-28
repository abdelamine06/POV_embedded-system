#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../headers/hall.h"
#include "../headers/usart.h"
#include "../headers/interrupt.h"


int Detect_Hall()
{
    //Setting DDRD in listenning mode for PIND2
    DDRD &= ~ _BV(PIND2);
    // If the second bin in PIND is 0 trun on PD6
    if (!(PIND & _BV(PIND2)))
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

void Detect_Hall_Interrupt()
{
  //External interrupts

  EICRA |= _BV(ISC01);
  EIMSK |= _BV(INT0);  // Activate INT0 - in PD2

}


ISR (INT0_vect)
{
  cptHall++;
}

int getCptHall()
{
  return cptHall;
}
