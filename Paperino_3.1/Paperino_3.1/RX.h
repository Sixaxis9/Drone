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


void Pin_Change_En(uint8_t reg);

void Pin_Change_Disen();

extern volatile uint8_t throttle;
extern volatile uint8_t pitch;
extern volatile uint8_t roll;
extern volatile uint8_t yaw;
extern volatile uint8_t aux1;
extern volatile uint8_t aux2;
extern volatile uint8_t aux3;

extern volatile float int_period;
extern volatile float int_period_1;

extern volatile float period;

extern volatile uint8_t flag_rx;

extern volatile uint8_t is_started;

#endif /* RX_H_ */