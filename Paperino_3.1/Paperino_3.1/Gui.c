/*
* Gui.c
*
*  Author: mdotg
*/

#define ACCX 97 //a
#define ACCY 98 //b
#define ACCZ 99 //c
#define GYROX 100 //d
#define GYROY 101 //e
#define GYROZ 102 //f

#define TEMP 103 //g
#define WHOIAM 104 //h

#define ANGLEACCX 105 //i
#define ANGLEACCY 106 //j
#define ANGLEGYROX 107 //k
#define ANGLEGYROY 108 //l

#define ANGLEX 109 //m
#define ANGLEY 110 //n
#define TIME   111 //o

#define THROTTLE 112 //p
#define	PITCH 113 //q
#define	ROLL 114 //r
#define YAW 115 //s
#define AUX1 116 //t
#define AUX2 117 //u

#include <stdint.h>

#include "Gui.h"
#include "IMU.h"
#include "USART.h"
#include "Timer.h"

void selected_value(uint8_t choise){
	switch(choise){
		case ACCX:
		computed_acceleration(0);
		USART_Transmit(accx);
		break;
		case ACCY:
		computed_acceleration(0);
		USART_Transmit(accy);
		break;
		case ACCZ:
		computed_acceleration(0);
		USART_Transmit(accz);
		break;
		case GYROX:
		computed_gyroscope(0);
		USART_Transmit(gyrox);
		break;
		case GYROY:
		computed_gyroscope(0);
		USART_Transmit(gyroy);
		break;
		case GYROZ:
		computed_gyroscope(0);
		USART_Transmit(gyroz);
		break;
		case TEMP:
		USART_Transmit(temperature(0));
		break;
		case WHOIAM:
		USART_Transmit(IMU_who_i_am(0));
		break;
		case ANGLEACCX:
		USART_Transmit(Angle_X_Acc);
		break;
		case ANGLEACCY:
		USART_Transmit(Angle_Y_Acc);
		break;
		case ANGLEGYROX:
		USART_Transmit(Angle_X_Gyro);
		break;
		case ANGLEGYROY:
		USART_Transmit(Angle_Y_Gyro);
		break;
		case ANGLEX:
		USART_Transmit(AngleX);
		break;
		case ANGLEY:
		USART_Transmit(AngleY);
		break;
		case TIME:
		USART_Transmit((uint8_t)system_tick_MG);
		break;
		case THROTTLE:
		USART_Transmit(throttle);
		break;
		case PITCH:
		USART_Transmit(pitch);
		break;
		case ROLL:
		USART_Transmit(roll);
		break;
		case YAW:
		USART_Transmit(yaw);
		break;
		case AUX1:
		USART_Transmit(aux1);
		break;
		case AUX2:
		break;
		case 255:
		USART_Transmit('p');
		break;
		case 0:
		USART_Transmit('q');
		break;
		default:
		USART_Transmit('e');
		break;
	}
}
