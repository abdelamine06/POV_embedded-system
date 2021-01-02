#include <avr/io.h>
#include <util/delay.h>
#include "../headers/spi.h"
#include "../headers/usart.h"
#include "../headers/hall.h"
#include "../headers/timer.h"
#include "../headers/interrupt.h"

#define F_CPU 13000000 // Clock Speed
#define BAUD 38400
#define MYUBRR F_CPU/16/BAUD-1

void Debug()
{
  PORTD |= _BV(PD6);
  _delay_ms(500);
  PORTD &= ~_BV(PD6);
  _delay_ms(500);
}

int main(){

    USART_Init(MYUBRR);
    SPI_MasterInit();
    TIMER_Init();
    Hall_Init();
    Interrupt_Init();
    while(1) 
    {
      Update_Time();
    }

}
