#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
  
void Debug() 
{
  PORTD |= _BV(PD6);
  _delay_ms(500);
  PORTD &= ~_BV(PD6);
  _delay_ms(500);
}

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

int main(){
    DDRD |= _BV(PD6);

    SPI_MasterInit();
    //DDRB |= _BV(PD6);
     while(1) {
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