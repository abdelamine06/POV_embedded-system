#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
  


void SPI_MasterInit(void)
{
  // set SS, MOSI and SCK to output
    DDRB |= (1 << DDB3) | (1 << DDB2) | (1 << DDB5);

    // set OE,LE to outputs
    DDRC |= (1 << DDC1)| (1 << DDC2);
 
}

void SPI_MasterTransmit(char cData)
{
  /* Start transmission */
  SPDR = cData;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF)))
;
}

int main(){
    SPI_MasterInit();
    //DDRB |= _BV(PD6);
     while(1) {
       unsigned char led=0x2;
        SPI_MasterTransmit(led);
        _delay_ms(4000);
        SPI_MasterTransmit(0x0);
    }

}