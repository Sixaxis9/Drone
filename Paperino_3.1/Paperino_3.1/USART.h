/*
* USART.h
*
*  Author: mdotg
*/


#ifndef USART_H_
#define USART_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdint.h>

void USART_Init( uint16_t baud );
void USART_Transmit( uint8_t data );
void USART_Transmit_Integer( int8_t data );
void USART_String(char* string, uint8_t lenght);

extern volatile uint8_t ISR_USART_Receive;

#endif /* USART_H_ */