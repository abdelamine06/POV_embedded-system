#include <avr/io.h>
#include <util/delay.h>
#include "../headers/usart.h"
#include "../headers/interrupt.h"
#include <avr/interrupt.h>

#define BUFFER_SIZE 128
char transmit_buffer[BUFFER_SIZE];
int transmit_tail=0;
int transmit_head=0;

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

void USART_Transmit_Char(unsigned char data)
{
    /* Wait for empty transmit buffer */
  while (!(UCSR0A & (1<<UDRE0)));
  /* Put data into buffer, sends the data */
  UDR0 = data;
}

void USART_Transmit_String(char* str) 
{
	int i = 0;
	while(str[i] != '\0') 
  {
		USART_Transmit_Char(str[i++]);
	}
}

// Mettre a jour le Buffer circulaire 
void Update_Buffer(int* i)
{
  ++*i;
  if(*i >= BUFFER_SIZE)
  {
    *i -= BUFFER_SIZE;
  }
}

void USART_Transmit_String_Interrupt(char *s) 
{
  int pos = 0;
    while (s[pos]!='\0')
    {
        transmit_buffer[transmit_head] = s[pos];
        Update_Buffer(&transmit_head);
        pos++;
    }
    UCSR0B |= _BV(UDRIE0); // Activer l'interruption
}

// USART interrupt
ISR(USART_UDRE_vect)
{
  if(transmit_tail!=transmit_head)
  {
    UDR0 = transmit_buffer[transmit_tail];
    Update_Buffer(&transmit_tail);
  }
  else
  {
    UCSR0B &= ~_BV(UDRIE0); // Désativer l'interruption
  }
}
