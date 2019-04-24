/*
* RX.h
*
*  Author: mdotg
*/


#ifndef RX_H_
#define RX_H_

void interrupt_init();

void Int_0_En();

void Int_1_En();

void Int_2_En();

void Int_3_En();

void Int_6_En();

void Interrupt_Init_Rising_INT0();

void Interrupt_Init_Rising_INT1();

void Interrupt_Init_Rising_INT2();

void Interrupt_Init_Rising_INT3();

void Interrupt_Init_Rising_INT6();


void Interrupt_Init_Falling_INT0();

void Interrupt_Init_Falling_INT1();

void Interrupt_Init_Falling_INT2();

void Interrupt_Init_Falling_INT3();

void Interrupt_Init_Falling_INT6();

int8_t limiter(int8_t channel);

void Pin_Change_En(uint8_t reg);

void Pin_Change_Disen();

extern volatile int8_t throttle;
extern volatile int8_t pitch;
extern volatile int8_t roll;
extern volatile int8_t yaw;
extern volatile int8_t aux1;
extern volatile int8_t aux2;
extern volatile int8_t aux3;

volatile uint16_t ch_1_rising, ch_1_rising_mod;
volatile uint16_t ch_1_falling, ch_1_falling_mod;

extern volatile uint32_t period;

extern volatile uint8_t flag_rx;

extern volatile uint8_t is_started;

#endif /* RX_H_ */