#include <avr/io.h>
#include <util/delay.h>
#include "../headers/usart.h"
#include "../headers/interrupt.h"
#include "../headers/timer.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 128

// Used by interruption
char transmit_buffer[BUFFER_SIZE];
int transmit_tail=0;
int transmit_head=0;

int vero_hour = 0;
char mode_affichage;
int enter_i= 0;
char enter_input[4];

void USART_Init(unsigned int ubrr)
{
  /*Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  // Enable receiver and transmitter
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (1<<USBS0)|(3<<UCSZ00);

  UCSR0B |= (1<< RXCIE0); // Enable interrupt receive
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
    UCSR0B |= _BV(UDRIE0); // Enable interrupt
}


void USART_Println(int x)
{
  char number_str[128];
  sprintf(number_str, "%d\n", x);
  USART_Transmit_String_Interrupt(number_str);
}


/********************************* Affichage ***********************************************/

void USART_Receive(){

  char str[1];
  sprintf(str,"%c",UDR0);


  if (enter_i == 1)
  {
    enter_input[enter_i-2] =  str[0];
    enter_i++;
    // Detecter si l'heure et les minutes on bien été entrés
    if (enter_i == 6)
    { 
      char hour_array[3] = {enter_input[0], enter_input[1], 0};
      char min_array[3] = {enter_input[2], enter_input[3], 0};
      hours = atoi(hour_array);
      minutes = atoi(min_array);
      char strs[40];
      time_seconds = 0;
      sprintf(strs,"Mise à jour de l'heure avec succes : %d h %d \n\r",hours,minutes);
      USART_Transmit_String_Interrupt(strs);
      vero_hour = 0;

    }
  }
  else if (strcmp(str,"h") == 0){
    USART_Transmit_String_Interrupt("Introduisez l'heure au format: hhmm\n\r");
    vero_hour = 1;
    enter_i = 0;
  }
}

// ***************************************** Interrupt **********************************
ISR(USART_RX_vect)
{
  USART_Receive();
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
