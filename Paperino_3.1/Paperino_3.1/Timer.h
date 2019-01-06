/*
* Timer.h
*
*  Author: mdotg
*/


#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <avr/io.h>

void timer_init();

void timer_8bit_PWM_init(uint8_t duty);
void timer_8bit_PWM_setFreq(uint8_t freq);

void timer1_16bit_PWM_A(uint8_t duty, uint16_t fduty, uint16_t pres);
void timer1_16bit_PWM_B(uint8_t duty, uint16_t fduty, uint16_t pres);
void timer1_16bit_PWM_C(uint8_t duty, uint16_t fduty, uint16_t pres);
void timer1_16bit_PWM_ABC(uint8_t duty, uint8_t duty1, uint8_t duty2, uint16_t fduty, uint16_t pres);

void timer3_16bit_PWM_A(uint8_t duty, uint16_t fduty, uint16_t pres);

void timer1_16bit_PWM_initA();
void timer1_16bit_PWM_initA();
void timer1_16bit_PWM_initC();
void timer1_16bit_PWM_initABC();

void timer3_16bit_PWM_initA();

uint16_t timer_1_setCLK(uint16_t pres);
uint16_t timer_3_setCLK(uint16_t pres);
uint16_t timer_8bit_setCLK(uint16_t pres);

void timer_16bit_INT_init_1_TOP();
void timer_16bit_INT_init_1_BOTTOM();
void timer_16bit_INT_init_3_TOP();
void timer_16bit_INT_init_3_BOTTOM();

void timer_8bit_INT_init_COMPA();

void timer_8bit_INT_init_OVF();

void timer_8bit_CTC_init(uint8_t freq, uint16_t pres);

extern volatile float system_tick_MG;


#endif /* TIMER_H_ */