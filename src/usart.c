#include <avr/io.h>
#include <util/delay.h>
#include "../headers/usart.h"
#include "../headers/interrupt.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include<stdlib.h>
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
    //wysyla kolejne litery napisu
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
    UCSR0B |= _BV(UDRIE0); // Enable interrupt
}



void USART_Receive()
{
  char str[1];
  sprintf(str,"%c",UDR0);
  switch (atoi(str))
    {
     case 1:
          USART_Transmit_String_Interrupt("POV TERMINAL1!\n");
          break;
     case 2:
          USART_Transmit_String_Interrupt("POV TERMINAL2 !\n");
          break;
     default:
          break;
    }
}

void USART_Println(int x)
{
  char number_str[128];
  sprintf(number_str, "%d\n", x);
  USART_Transmit_String_Interrupt(number_str);
}





// *************** Interrupt **********************************
ISR(USART_RX_vect)
{
  USART_Receive();
  UCSR0B &= ~ (1<<RXCIE0);
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
