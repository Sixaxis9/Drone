/*
* USART.c
*
*  Author: mdotg
*/

#include "USART.h"
#include <avr/io.h>

#include <avr/interrupt.h>


void USART_Init( uint16_t baud )
{
	uint8_t baud_reg = ((F_CPU)/(baud*16UL)-1);
	// Set baud rate
	UBRR1H = (baud_reg>>8);
	UBRR1L = baud_reg;


	UCSR1B = (1<<RXCIE1)|(0<<TXCIE1)|(0<<UDRIE1)|(0<<UCSZ12); //Disable interrupts, set 8 bit data
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10)|(0<<USBS1)|(0<<UMSEL11)|(0<<UMSEL10)|(0<<UPM11)|(0<<UPM10);//set asyncronus serial, disabled parity bits,
	UCSR1B |= (1<<RXEN1)|(1<<TXEN1); //Enable writing and reading
}


void USART_Transmit( uint8_t data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE1)) ) //W8 for USART Data Register Empty
	;
	/* Put data into buffer, sends the data */
	UDR1 = data;
}

void USART_Transmit_Integer( int8_t data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE1)) ) //W8 for USART Data Register Empty
	;
	/* Put data into buffer, sends the data */
	UDR1 = data;
}

void USART_String(char* string, uint8_t lenght){
	for (int i=0; i< lenght; i++)
	{
		USART_Transmit(string[i]);
	}
}


ISR(USART1_RX_vect){
	
	if ((FE1 == 1) | (DOR1 == 1) | (UPE1 == 1) )
	{
		ISR_USART_Receive = 255;
	}
	
	ISR_USART_Receive = UDR1;
	USART_Transmit(ISR_USART_Receive);
	selected_value(ISR_USART_Receive);
	USART_Transmit('\n');
}
