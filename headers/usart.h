#ifndef USART_H
#define USART_H

void USART_Init(unsigned int ubrr);
void USART_Transmit_Char(unsigned char data);
void Update_Buffer(int* index);
void USART_Transmit_String(char* str);
void USART_Transmit_String_Interrupt(char *s);
ISR(USART_UDRE_vect);


#endif // USART_H