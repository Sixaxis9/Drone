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
	DDRE &= 0b10111111;

	timer_init();
	
	TWI_init();
	USART_Init(57600);
	
	USART_Transmit(IMU_Init());
	
	interrupt_init();
	
	sei(); //cli(); countermeasure

	while (1)
	{
		//aux1 = 50; //for development purpose only
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

			angle_filtered(0);
			
			update_PIDs();
			update_motors();
		}

	}
}
