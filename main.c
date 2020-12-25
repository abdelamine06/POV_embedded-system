#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

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


void USART_Init(unsigned int ubrr)
{
  /*Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  // Enable receiver and transmitter 
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}


void USART_Transmit_char(unsigned char data)
{
    /* Wait for empty transmit buffer */
  while (!(UCSR0A & (1<<UDRE0)));
  /* Put data into buffer, sends the data */
  UDR0 = data;

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
    //unsigned char *str2 = "azul";
    USART_Init(MYUBRR);
    SPI_MasterInit();
   
   
   
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