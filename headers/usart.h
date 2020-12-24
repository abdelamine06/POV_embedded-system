#ifndef USART_H
#define USART_H

void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);

#endif // USART_H