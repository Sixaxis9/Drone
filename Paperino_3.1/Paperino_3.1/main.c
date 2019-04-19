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
	
	TWI_init();
	USART_Init(57600);
	
	USART_Transmit(IMU_Init());
	
	interrupt_init();
	
	sei(); //cli(); countermeasure
	float end_this, end_that, end_m;
	while (1)
	{
		end_this = system_tick_MG_p;
		end_m = system_tick_MG_p_mod;
		PORTB = 0b00010000;
		_delay_ms(20);
		PORTB = 0b00000000;
		_delay_ms(20);
		end_that = time_in_seconds(end_this, end_m, system_tick_MG_p, system_tick_MG_p_mod);
		
		USART_Transmit(end_that);
		USART_Transmit('\n');
	}
		

}



	/*while (1)
	{
	PORTC = 255;
		aux1 = 50; //for development purpose only
		if (aux1 > 25) //taking some noise into account
		{
			is_started = 1;
			}else{
			is_started = 0;
		}
		
		if (is_started == 1)
		{
			read_acceleration(1); //refresh acc reading
			read_gyroscope(); //refresh gyro reading
			
			computed_acceleration(0);
			computed_gyroscope(0);
			
			compute_angle_acc(0);
			compute_angle_gyro(0);

			angle_filtered(1);
			
			update_PIDs();
			update_motors();
		}

	}*/