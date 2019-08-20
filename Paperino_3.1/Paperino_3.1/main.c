/*
* Paperino3.1.c
*
* Author : mdotg
*/
#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "I2C.h"
#include "IMU.h"
#include "USART.h"
#include "Timer.h"
#include "Gui.h"
#include "RX.h"
#include "PID.h"

volatile uint8_t ISR_USART_Receive = 0;


int main(void)
{
	DDRD = 0b00000000; //Makes PORTD as Input
	PORTD = 0b00001111; //Internal Pull Up enabled
	DDRB = 0b11111111;


	timer_init();
	
	TWI_init(0x68, 400000);
	USART_Init(57600);
	
	USART_Transmit(IMU_Init());
	
	interrupt_init();
	
	sei(); //cli(); countermeasure
	
	
	while (1)
	{
		aux1 = 50; //for development purpose only
		if (aux1 > 25) //taking some noise into account
		{
			is_started = 1;
			}else{
			is_started = 0;
		}
		
		if (is_started == 1)
		{
			read_acceleration(1); //refresh acc reading - arg for offset
			read_gyroscope(1); //refresh gyro reading - arg for offset
			
			computed_acceleration(0); //compute acc reading
			computed_gyroscope(0); //compute gyro reading
			
			USART_Transmit((uint8_t)(period/10.0));

			compute_angle_acc(0); //compute angle by acc information
			compute_angle_gyro(0); //compute angle by gyro information
			USART_Transmit(throttle);
			angle_filtered(1); //compute angle filtered btw acc and gyro*/

		if (PORTC == 0b10000000)
		{
			PORTC = 0;
			}else{
			if (PORTC == 0)
			{
				PORTC = 0b10000000;
			}
		}
			
			
			//USART_Transmit('\n');

			//pdate_PIDs(); //compute new motor speed values with PID controllers
			//update_motors(); //update motor speeds
		}

	}
	
	
}
	
	void my_delay_ms(int n) {
		while(n--) {
			_delay_ms(1);
		}
	}