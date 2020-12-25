#ifndef USART_H
#define USART_H

void USART_Init(unsigned int ubrr);
void USART_Transmit_Char(unsigned char data);
void USART_Transmit_String(char* str);

#endif // USART_H