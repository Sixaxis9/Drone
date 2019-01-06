/*
* PID.h
*
*  Author: mdotg
*/


#ifndef PID_H_
#define PID_H_

#include <stdint.h>

/*
* PID.c
*
* Created: 27/12/2018 22:50:31
*  Author: mdotg
*/

#include "PID.h"
#include "Timer.h"
#include "IMU.h"
#include "RX.h"

uint8_t k_pitch[3]; //[P,I,D]
uint8_t k_roll[3]; //[P,I,D]
uint8_t k_yaw[3]; //[P,I,D]

uint8_t pid_p, pid_i, pid_d;

uint8_t error_pitch, error_pitch_previous;
uint8_t error_roll, error_roll_previous;
uint8_t error_yaw, error_yaw_previous;

float last_sample_pitch;
float last_sample_roll;
float last_sample_yaw;
float delta_time_PID;

extern volatile uint8_t motor1;
extern volatile uint8_t motor2;
extern volatile uint8_t motor3;
extern volatile uint8_t motor4;

uint8_t PID[3];

void PID_Pitch();
void PID_Roll();
void PID_Yaw();

void update_PIDs();
void update_motors();

int8_t limit_bound(int8_t value,int8_t min,int8_t max);
int8_t map_values(int8_t in_var, int8_t in_min, int8_t in_max, int8_t out_min, int8_t out_max);

#endif /* PID_H_ */