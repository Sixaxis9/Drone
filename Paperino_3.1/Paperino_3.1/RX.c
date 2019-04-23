/*
* RX.c
*
*  Author: mdotg
*/

#include <avr/interrupt.h>
#include "Timer.h"
#include "USART.h"


volatile int8_t throttle = 0;
volatile int8_t pitch = 0;
volatile int8_t roll = 0;
volatile int8_t yaw = 0;
volatile int8_t aux1 = 0;
volatile int8_t aux2 = 0;
volatile int8_t aux3 = 0;

volatile uint16_t ch_1_rising = 0;
volatile uint16_t ch_1_rising_mod = 0;
volatile uint16_t ch_1_falling = 0;
volatile uint16_t ch_1_falling_mod = 0;
volatile uint32_t period = 0;
volatile uint16_t period1 = 0;

volatile uint8_t is_started = 0;

volatile uint8_t portbhistory = 0;

volatile uint8_t flag_rx = 0;

void Int_0_En(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EIFR |= (1 << INTF0);
	EIMSK |= (1 <<	INT0);
	
	SREG = sreg;
}

void Int_1_En(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EIFR |= (1 << INTF1);
	EIMSK |= (1 <<	INT1);
	
	SREG = sreg;
}

void Int_2_En(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EIFR |= (1 << INTF2);
	EIMSK |= (1 <<	INT2);
	
	SREG = sreg;
}

void Int_3_En(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EIFR |= (1 << INTF3);
	EIMSK |= (1 <<	INT3);
	
	SREG = sreg;
}

void Int_6_En(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EIFR |= (1 << INTF6);
	EIMSK |= (1 <<	INT6);
	
	SREG = sreg;
}

void Interrupt_Init_Rising_INT0(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EICRA |= (1 << ISC01) | (1 << ISC00);
	
	SREG = sreg;
}

void Interrupt_Init_Rising_INT1(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EICRA |= (1 << ISC11) | (1 << ISC10);
	
	SREG = sreg;
}
void Interrupt_Init_Rising_INT2(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EICRA |= (1 << ISC21) | (1 << ISC20);
	
	SREG = sreg;
}

void Interrupt_Init_Rising_INT3(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EICRA |= (1 << ISC31) | (1 << ISC30);
	
	SREG = sreg;
}

void Interrupt_Init_Rising_INT6(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EIFR |= (0 << INTF3);

	EICRB |= (1 << ISC61) | (1 << ISC60);
	Int_6_En();
	SREG = sreg;
}

void Interrupt_Init_Falling_INT0(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EIFR |= (1 << INTF0);
	EICRA &= ~(1 << ISC00);

	Int_0_En();
	SREG = sreg;
}

void Interrupt_Init_Falling_INT1(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EIFR |= (1 << INTF1);
	EICRA &= ~(1 << ISC10);

	Int_1_En();
	SREG = sreg;
}

void Interrupt_Init_Falling_INT2(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EIFR |= (1 << INTF2);
	EICRA &= ~(1 << ISC20);

	Int_2_En();
	SREG = sreg;
}

void Interrupt_Init_Falling_INT3(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	
	EIFR |= (1 << INTF3);
	EICRA &= ~(1 << ISC30);

	Int_3_En();
	
	SREG = sreg;
}

void Interrupt_Init_Falling_INT6(){
	uint8_t sreg;
	sreg = SREG;
	cli();
	EIFR |= (1 << INTF6);

	EICRB &= ~(1 << ISC60);
	Int_6_En();
	SREG = sreg;
}

void Pin_Change_En(uint8_t reg){//Parameter: bit activated
	PCICR = (1 << PCIE0);
	PCMSK0 = reg;
	PCIFR = 1;
}

void Pin_Change_Disen(){
	PCICR = (0 << PCIE0);
}




ISR(INT6_vect){
	if (flag_rx  == 0)
	{		
		period = time_precision(ch_1_rising, ch_1_rising_mod);
		
		period1 = 2*249;
		
		ch_1_rising = system_tick_MG_p + (uint16_t)TCNT0;
		ch_1_rising_mod = system_tick_MG_p_mod;
		
		flag_rx = 1;
		
		Interrupt_Init_Falling_INT6();
		
		}else{ 
		
		throttle = (int8_t) (((float)(time_precision(ch_1_rising, ch_1_rising_mod)-249)/(float)period1)*200);
		
		if (throttle<0)
		{
			throttle = 0;
		}else{
			if (throttle > 100)
			{
				throttle = 100;
			}
		}
		
		flag_rx = 0;

		Interrupt_Init_Rising_INT6();

	}

}

ISR(PCINT0_vect){
	//check the ports, store before value, confront with actual
	//store new value
	
	uint8_t changedbits;
	uint8_t intreading = PINB;
	changedbits = intreading ^ portbhistory;
	portbhistory = intreading;
	if (flag_rx == 1)
	{

	}
	if (changedbits == 0b10000000 ||  changedbits == 0b10000000)
	{
		
	}
	

	switch(changedbits){

		case 0: //nothing changed
		break;

		case 1: //pcint0 changed
		break;

		case 2: //pcint1 changed
		//SCK - Yaw
		if (flag_rx == 1)
		{
			yaw = ((float)(time_precision(ch_1_rising, ch_1_rising_mod)-249))/period;
		}
		break;

		case 4: //pcint2 changed
		//MOSI - Roll
		if (flag_rx == 1)
		{
			roll = ((float)(time_precision(ch_1_rising, ch_1_rising_mod)-249))/period;
		}
		break;

		case 8:  //pcint3 changed
		//MISO - Pitch
		if (flag_rx == 1)
		{
			pitch = ((float)(time_precision(ch_1_rising, ch_1_rising_mod)-249))/period;
		}
		break;

		case 16: //pcint4 changed
		//PB4: AUX1
		if (flag_rx == 1)
		{
			aux1 = ((float)(time_precision(ch_1_rising, ch_1_rising_mod)-249))/period;
		}
		break;

		case 32: //pcint5 changed
		break;

		case 64: //pcint6 changed
		break;
		
		case 128: //pcint7 changed
		break;
	}
}

void interrupt_init(){
	Interrupt_Init_Rising_INT6();
	
	//Pin_Change_En(0b10000000);
}
