/*
* PID.c
*
*  Author: mdotg
*/

#include "PID.h"
#include "Timer.h"
#include "IMU.h"
#include "RX.h"
#include "USART.h"

uint8_t k_pitch[3] = {4,10,5}; //[P,I,D]
uint8_t k_roll[3] = {4,10,5}; //[P,I,D]
uint8_t k_yaw[3] = {0,0,0}; //[P,I,D]

uint8_t pid_p, pid_i, pid_d;

uint8_t error_pitch, error_pitch_previous;
uint8_t error_roll, error_roll_previous;
uint8_t error_yaw, error_yaw_previous;

float last_sample_pitch = 0;
float last_sample_roll = 0;
float last_sample_yaw = 0;
float delta_time_PID = 0;

volatile uint8_t motor1 = 0;
volatile uint8_t motor2 = 0;
volatile uint8_t motor3 = 0;
volatile uint8_t motor4 = 0;

uint8_t PID[3];

void PID_Pitch(){

	error_pitch = map_values(AngleX, 0, 90, 0, 100) - pitch;

	/*Next the proportional value of the PID is just a proportional constant
	*multiplied by the error*/

	pid_p = k_pitch[0]*error_pitch;
	
	delta_time_PID = system_tick_MG + 0.0000041*TCNT0 - last_sample_pitch;


	pid_i = pid_i+(k_pitch[1]*error_pitch)*delta_time_PID;


	pid_d = k_pitch[2]*((error_pitch - error_pitch_previous)/delta_time_PID);

	/*The final PID values is the sum of each of this 3 parts*/
	PID[0] = pid_p + pid_i + pid_d;
	
	last_sample_pitch = system_tick_MG + 0.0000041*TCNT0;

}

void PID_Roll(){

	error_roll = map_values(AngleY, 0, 90, 0, 100) - roll;

	/*Next the proportional value of the PID is just a proportional constant
	*multiplied by the error*/

	pid_p = k_roll[0]*error_roll;
	
	delta_time_PID = system_tick_MG + 0.0000041*TCNT0 - last_sample_roll;


	pid_i = pid_i+(k_roll[1]*error_roll)*delta_time_PID;


	pid_d = k_roll[2]*((error_roll - error_roll_previous)/delta_time_PID);

	/*The final PID values is the sum of each of this 3 parts*/
	PID[1] = pid_p + pid_i + pid_d;
	
	last_sample_roll = system_tick_MG + 0.0000041*TCNT0;

}

void PID_Yaw(){

	error_yaw = map_values(AngleX, 0, 90, 0, 100) - yaw;

	/*Next the proportional value of the PID is just a proportional constant
	*multiplied by the error*/

	pid_p = k_yaw[0]*error_yaw;
	
	delta_time_PID = system_tick_MG + 0.0000041*TCNT0 - last_sample_yaw;


	pid_i = pid_i+(k_yaw[1]*error_yaw)*delta_time_PID;


	pid_d = k_yaw[2]*((error_yaw - error_yaw_previous)/delta_time_PID);

	/*The final PID values is the sum of each of this 3 parts*/
	PID[2] = pid_p + pid_i + pid_d;
	
	last_sample_yaw = system_tick_MG + 0.0000041*TCNT0;

}

void update_PIDs(){
	if (throttle > 5)
	{
		PID_Pitch();
		PID_Roll();
		PID_Yaw();
		
		motor1 = throttle + PID[0] - PID[1] + PID[2];
		motor2 = throttle + PID[0] + PID[1] - PID[2];
		motor3 = throttle - PID[0] + PID[1] + PID[2];
		motor4 = throttle - PID[0] - PID[1] - PID[2];
		}else{
		motor1 = 5;
		motor2 = 5;
		motor3 = 5;
		motor4 = 5;
	}
}

void update_motors(){
	
	timer1_16bit_PWM_ABC(limit_bound(motor2, 5, 100), limit_bound(motor1, 5, 100), limit_bound(motor3, 5, 100), 500, 1);
	timer3_16bit_PWM_A(limit_bound(motor4, 5, 100), 500, 1);
}

int8_t limit_bound(int8_t value,int8_t min,int8_t max){
	if (value > max)
	{
		return max;
		}else{
		if(value < min){
			return min;
		}
	}
	
	return value;
	
}

int8_t map_values(int8_t in_var, int8_t in_min, int8_t in_max, int8_t out_min, int8_t out_max){
	return (in_var - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}