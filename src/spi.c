#include <avr/io.h>
#include <util/delay.h>
#include "../headers/spi.h"

void SPI_MasterInit(void)
{

  // set SS, MOSI and SCK to output
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);

    // set OE,LE to outputs
    DDRC |= (1 << PC1)| (1 << PC2);

    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

    PORTC &= ~(1<<PC1);
}

void SPI_MasterTransmit(char cData)
{

  /* Start transmission */
  SPDR = cData;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF))){
  }
}
