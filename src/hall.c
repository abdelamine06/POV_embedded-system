#include <avr/io.h>
#include <util/delay.h>
#include "../headers/hall.h"
#include "../headers/usart.h"

int detectHall()
{

    //Setting DDRD in listenning mode for PIND2
    DDRD &= ~ _BV(PIND2);

    // If the second bin in PIND is 0 trun on PD6
    while(1)
    {
        if ( ! (PIND & _BV(PIND2))  )
        {
            PORTD|= _BV(PD6);
            //USART_Transmit("hall detected");
        }
        else
        {
            PORTD &= ~_BV(PD6);
        }
    }
}
