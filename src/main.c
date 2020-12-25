#include <avr/io.h>
#include <util/delay.h>
#include "../headers/spi.h"
#include "../headers/usart.h"
#include "../headers/hall.h"

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
    DDRD |= _BV(PD6);

    USART_Init(MYUBRR);
    SPI_MasterInit();

    detectHall();

     while(1) {
        USART_Transmit_String("Bonjour!");
        _delay_ms(1000);

        unsigned char led=0xFE;
        SPI_MasterTransmit(led);

        PORTC |= (1<<PC2);
        PORTC &= ~(1<<PC2);

        _delay_ms(1000);

        SPI_MasterTransmit(0x00);

        PORTC |= (1<<PC2);
        PORTC &= ~(1<<PC2);

        _delay_ms(1000);

    }

}