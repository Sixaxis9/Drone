/*
* RX.c
*
*  Author: mdotg
*/

#include <avr/interrupt.h>
#include "Timer.h"


volatile uint8_t throttle = 0;
volatile uint8_t pitch = 0;
volatile uint8_t roll = 0;
volatile uint8_t yaw = 0;
volatile uint8_t aux1 = 0;
volatile uint8_t aux2 = 0;
volatile uint8_t aux3 = 0;

volatile float int_period = 0;
volatile float int_period_1 = 0;

volatile float period = 0;

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
		
		period = (system_tick_MG + 0.000004*TCNT0 - int_period);

		int_period = system_tick_MG + 0.000004*TCNT0; //To count the period of the wave in ms
		
		Interrupt_Init_Falling_INT6();

		flag_rx = 1;

		}else{
		
		throttle = ((system_tick_MG  - int_period + 0.000004*TCNT0)*100)/period;
		
		Interrupt_Init_Rising_INT6();

		flag_rx = 0;
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
			yaw = ((system_tick_MG  - int_period + 0.000004*TCNT0)*100)/period;
		}
		break;

		case 4: //pcint2 changed
		//MOSI - Roll
		if (flag_rx == 1)
		{
			roll = ((system_tick_MG  - int_period + 0.000004*TCNT0)*100)/period;
		}
		break;

		case 8:  //pcint3 changed
		//MISO - Pitch
		if (flag_rx == 1)
		{
			pitch = ((system_tick_MG  - int_period + 0.000004*TCNT0)*100)/period;
		}
		break;

		case 16: //pcint4 changed
		//PB4: AUX1
		if (flag_rx == 1)
		{
			aux1 = ((system_tick_MG  - int_period + 0.000004*TCNT0)*100)/period;
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
	Int_6_En();
	
	Pin_Change_En(0b10000000);
}
