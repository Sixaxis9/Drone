/*
* IMU.h
*
*  Author: mdotg
*/


#ifndef IMU_H_
#define IMU_H_

#include "I2C.h"

extern volatile int16_t acceleration_x, acceleration_y, acceleration_z;
extern volatile int16_t gyroscope_x, gyroscope_y, gyroscope_z;

int8_t IMU_Init();
uint8_t IMU_who_i_am(uint8_t print);
int8_t temperature(uint8_t print);

int8_t read_acceleration(uint8_t calibrate_acc);
int8_t read_gyroscope(uint8_t calibrate_gyro);

void computed_acceleration(uint8_t print);
void computed_gyroscope(uint8_t print);
void compute_angle_acc(uint8_t print);
void compute_angle_gyro(uint8_t print);

void complementary_filter(uint8_t print);

extern volatile float accx;
extern volatile float accy;
extern volatile float accz;

extern volatile float gyrox;
extern volatile float gyroy;
extern volatile float gyroz;

extern volatile float Angle_X_Acc;
extern volatile float Angle_Y_Acc;

extern volatile float Angle_X_Gyro;
extern volatile float Angle_Y_Gyro;

volatile uint16_t last_sample_gyro, last_sample_gyro_mod;


//-----------------------------------------

//FILTERING

//-----------------------------------------


void angle_filtered(uint8_t print);


extern volatile int8_t AngleX;
extern volatile int8_t AngleY;


#endif /* IMU_H_ */