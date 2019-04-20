/*
* Timer.c
*
*  Author: mdotg
*/

#include "Timer.h"

#include <avr/interrupt.h>


#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

volatile uint16_t system_tick_MG, system_tick_MG_p = 0;
volatile uint16_t system_tick_MG_mod, system_tick_MG_p_mod = 0;

void timer_8bit_PWM_init(uint8_t duty){
	DDRB |= 0b10000000;
	TCCR0A = (1 << COM0A1) | (1 << WGM00);
	TCCR0B = (1 << CS00) | (0 << CS01) | (0<<CS02) | (0 << FOC0A) | (0 << FOC0B);
	OCR0A = duty;
}

void timer_8bit_PWM_setFreq(uint8_t freq){
	OCR0A = freq;
}

void timer_8bit_CTC_init(uint8_t freq, uint16_t pres){
	TCCR0A = (1 << WGM01);
	timer_8bit_setCLK(pres);
	OCR0A = freq;
}




void timer1_16bit_PWM_A(uint8_t duty, uint16_t fduty, uint16_t pres){
	uint16_t freq = F_CPU/(2*timer_1_setCLK(pres)*fduty);
	uint16_t dutyc = freq/100*duty;
	uint8_t sreg;
	sreg = SREG;
	cli();
	//ICR1H = freq >> 8; //SET FREQUENCY
	//ICR1L = freq;
	ICR1 = freq;
	OCR1A = dutyc;
	//OCR1AH = dutyc >> 8; //SET DUTY
	//OCR1AL = dutyc;
	SREG = sreg;
}

void timer1_16bit_PWM_B(uint8_t duty, uint16_t fduty, uint16_t pres){
	uint16_t freq = F_CPU/(2*timer_1_setCLK(pres)*fduty);
	uint16_t dutyc = freq/100*duty;
	uint8_t sreg;
	sreg = SREG;
	cli(); //Disabling interrupts as stated in datasheet. Otherwise data will be corrupted
	ICR1 = freq;
	OCR1A = dutyc;
	SREG = sreg;

}

void timer1_16bit_PWM_C(uint8_t duty, uint16_t fduty, uint16_t pres){
	uint16_t freq = F_CPU/(2*timer_1_setCLK(pres)*fduty);
	uint16_t dutyc = freq/100*duty;
	uint8_t sreg;
	sreg = SREG;
	cli(); //Disabling interrupts as stated in datasheet. Otherwise data will be corrupted
	ICR1 = freq;
	OCR1C = dutyc;
	SREG = sreg;

}

void timer3_16bit_PWM_A(uint8_t duty, uint16_t fduty, uint16_t pres){
	uint16_t freq = F_CPU/(2*timer_3_setCLK(pres)*fduty);
	uint16_t dutyc = freq/100*duty;
	uint8_t sreg;
	sreg = SREG;
	cli();
	ICR3 = freq;
	OCR3A = dutyc;
	SREG = sreg;
}


void timer1_16bit_PWM_ABC(uint8_t duty, uint8_t duty1, uint8_t duty2, uint16_t fduty, uint16_t pres){
	uint16_t freq =  F_CPU/(2*timer_1_setCLK(pres)*fduty);
	uint16_t dutyc = freq/100*duty;
	uint16_t dutyc1 = freq/100*duty1;
	uint16_t dutyc2 = freq/100*duty2;
	uint8_t sreg;
	sreg = SREG;
	cli();
	//OCR1AH = dutyc >> 8; //SET DUTY
	//OCR1AL = dutyc; //SET DUTY
	//OCR1CH = dutyc1 >> 8; //SET DUTY
	//OCR1CL = dutyc1; //SET DUTY
	//ICR1H = freq >> 8; //SET FREQUENCY
	//ICR1L = freq;
	
	ICR1 = freq;
	OCR1A = dutyc;
	OCR1B = dutyc1;
	OCR1C = dutyc2;
	SREG = sreg;
	
}

void timer1_16bit_PWM_initA(){
	DDRB |= 0b00100000;

	TCCR1A |= (0 << WGM11) | (0 << WGM10) | (1 << COM1A1);
	TCCR1B |= (1 << WGM13);
}

void timer3_16bit_PWM_initA(){
	DDRC = 0b01000000;

	TCCR3A |= (0 << WGM31) | (0 << WGM30) | (1 << COM3A1);
	TCCR3B |= (1 << WGM33);
}

void timer1_16bit_PWM_initB(){
	DDRB |= 0b01000000;
	TCCR1A = (1 << COM1B1) | (0 << WGM11) | (0 << WGM10);
	TCCR1B = (1 << WGM13);
}

void timer1_16bit_PWM_initC(){
	DDRB |= 0b10000000;
	TCCR1A = (1 << COM1C1) | (0 << WGM11) | (0 << WGM10);
	TCCR1B = (1 << WGM13);
}

void timer1_16bit_PWM_initABC(){
	DDRB |= 0b11100000;

	TCCR1A |= (0 << WGM11) | (0 << WGM10) | (1 << COM1A1) | (1 << COM1B1) | (1 << COM1C1);
	TCCR1B = (1 << WGM13);
}


uint16_t timer_1_setCLK(uint16_t pres){
	switch (pres)
	{
		case 0:
		TCCR1B |= (0 << CS12) | (0 << CS11) |(1 << CS10);
		pres = 1;
		break;
		case 8:
		TCCR1B |= (0 << CS12) | (1 << CS11) |(0 << CS10);
		break;
		case 64:
		TCCR1B |= (0 << CS12) | (1 << CS11) |(1 << CS10);
		break;
		case 256:
		TCCR1B |= (1 << CS12) | (0 << CS11) |(0 << CS10);
		break;
		case 1024:
		TCCR1B |= (1 << CS12) | (1 << CS11) |(0 << CS10);
		break;
		default:
		TCCR1B |= (0 << CS12) | (0 << CS11) |(1 << CS10);
		pres = 1;
		break;
	}
	return pres;
}

uint16_t timer_3_setCLK(uint16_t pres){
	switch (pres)
	{
		case 0:
		TCCR3B |= (0 << CS12) | (0 << CS11) |(1 << CS10);
		pres = 1;
		break;
		case 8:
		TCCR3B |= (0 << CS12) | (1 << CS11) |(0 << CS10);
		break;
		case 64:
		TCCR3B |= (0 << CS12) | (1 << CS11) |(1 << CS10);
		break;
		case 256:
		TCCR3B |= (1 << CS12) | (0 << CS11) |(0 << CS10);
		break;
		case 1024:
		TCCR3B |= (1 << CS12) | (1 << CS11) |(0 << CS10);
		break;
		default:
		TCCR3B |= (0 << CS12) | (0 << CS11) |(1 << CS10);
		pres = 1;
		break;
	}
	return pres;
}

uint16_t timer_8bit_setCLK(uint16_t pres){
	switch (pres)
	{
		case 0:
		TCCR0B |= (0 << CS12) | (0 << CS11) |(1 << CS10);
		pres = 1;
		break;
		case 8:
		TCCR0B |= (0 << CS12) | (1 << CS11) |(0 << CS10);
		break;
		case 64:
		TCCR0B |= (0 << CS12) | (1 << CS11) |(1 << CS10);
		break;
		case 256:
		TCCR0B |= (1 << CS12) | (0 << CS11) |(0 << CS10);
		break;
		case 1024:
		TCCR0B |= (1 << CS12) | (1 << CS11) |(0 << CS10);
		break;
		default:
		TCCR0B |= (0 << CS12) | (0 << CS11) |(1 << CS10);
		pres = 1;
		break;
	}
	return pres;
}

void timer_16bit_INT_init_1_TOP(){
	TIMSK1 |= (1 << ICIE1);
}

void timer_16bit_INT_init_1_BOTTOM(){
	TIMSK1 |= (1 << TOIE1);
}

void timer_16bit_INT_init_3_TOP(){
	TIMSK1 |= (1 << ICIE3);
}

void timer_16bit_INT_init_3_BOTTOM(){
	TIMSK3 |= (1 << TOIE3);
}

void timer_8bit_INT_init_COMPA(){
	TIMSK0 |= (1 << OCIE0A);
}

void timer_8bit_INT_init_OVF(){
	TIMSK0 |= (1 << TOIE0);
}

ISR(TIMER1_OVF_vect){
	
}

ISR(TIMER3_OVF_vect){
	
}

ISR(TIMER1_CAPT_vect){
	//system_tick_MG++;
}

ISR(TIMER3_CAPT_vect){
	//system_tick_MG++;
}

ISR(TIMER0_COMPA_vect){ //scatta 1 volta al milli secondo
	system_tick_MG = system_tick_MG + 1; //every ms
	system_tick_MG_p = system_tick_MG_p + 248; //adds 248 every ms for more precision
	
	if (system_tick_MG_p >= 65287) //if more than 65287 int ovfl -> 0
	{
		system_tick_MG_p_mod += 1; //counter will ovfl normally
	}
	
	if (system_tick_MG == 65535)
	{
		system_tick_MG_mod += 1; //counter will ovfl normally
	}
	
		 //if (PORTC == 0xff)
	 	//{
	 		//PORTC = 0;
	 		//}else{
	 		//if (PORTC == 0)
	 		//{
	 			//PORTC = 0xff;
	 		//}
	 	//}
}

uint16_t time_precision(uint16_t precision, uint16_t precision_module){
	uint16_t carry = system_tick_MG_p_mod-precision_module;
	return (system_tick_MG_p + TCNT0) + (65535*carry - precision);
}

uint16_t time_in_millisecondsseconds(uint16_t precision, uint16_t precision_module){
	uint16_t carry = system_tick_MG_mod-precision_module;
	return system_tick_MG + (65535*carry - precision);
}

uint16_t time_in_seconds(uint16_t precision, uint16_t precision_module){
	uint16_t carry = system_tick_MG_mod-precision_module;
	return (system_tick_MG + (65535*carry - precision))/1000;

}

void timer_init(){
	timer1_16bit_PWM_initABC();
	timer3_16bit_PWM_initA();
	
	timer_8bit_CTC_init(248, 64); //1kHz -> 124, divide by two because triggers on both edges
	timer_8bit_INT_init_COMPA();
}