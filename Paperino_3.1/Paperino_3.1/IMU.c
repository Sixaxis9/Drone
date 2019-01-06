/*
* IMU.c
*
*  Author: mdotg
*/

#include "IMU.h"
#include "USART.h"
#include "Timer.h"

#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <util/delay.h>

#include <math.h>

//#include "I2C.h"
//#include <stdint.h>


/*typdef struct Acceleration{
int16_t acceleration_x, acceleration_y, acceleration_z;
} Acceleration;

extern Acceleration acceleration;


typdef struct Gyroscope {
int16_t gyroscope_x, gyroscope_y, gyroscope_z;
} Gyroscope;

extern Gyroscope gyroscope;*/

volatile float accx;
volatile float accy;
volatile float accz;

volatile float gyrox;
volatile float gyroy;
volatile float gyroz;

volatile float Angle_X_Acc;
volatile float Angle_Y_Acc;

volatile float Angle_X_Gyro = 0;
volatile float Angle_Y_Gyro = 0;

volatile int16_t acceleration_x, acceleration_y, acceleration_z;
volatile int16_t gyroscope_x, gyroscope_y, gyroscope_z;

volatile float last_sample_gyro;

int16_t x_offset_gyro = 0;
int16_t y_offset_gyro = 0;
int16_t z_offset_gyro = 0;

int16_t x_offset_acc = 0;
int16_t y_offset_acc = 0;
int16_t z_offset_acc = 0;

int8_t IMU_Init(){
	uint8_t register_value;
	
	register_value = 0b00000010;//config DLPF
	if (TWI_send_16bit(0x1a, register_value) != 0)
	{
		return -1;
	}
	register_value = (unsigned char) 0b00001000; //Full scale range gyro, 1000rad/s
	if (TWI_send_16bit(0x1b, register_value) != 0)
	{
		return -1;
	}
	register_value = (unsigned char) 0b00001000; //Full scale range gyro, 4g
	if (TWI_send_16bit(0x1c, register_value) != 0)
	{
		return -1;
	}
	register_value = 0b00000000; //Disable FIFO
	if (TWI_send_16bit(0x23, register_value) != 0)
	{
		return -1;
	}
	register_value = 0b00000000; //Disable multi master
	if (TWI_send_16bit(0x24, register_value) != 0)
	{
		return -1;
	}
	register_value = 0b00000000; //Disable interrupts
	if (TWI_send_16bit(0x37, register_value) != 0)
	{
		return -1;
	}
	register_value = 0b00000000; //Disable interrupts
	if (TWI_send_16bit(0x38, register_value) != 0)
	{
		return -1;
	}
	register_value = 0b00000000; //User CTRL
	if (TWI_send_16bit(0x6a, register_value) != 0)
	{
		return -1;
	}
	register_value = 0b00000001; //Setting gyro clock source
	if (TWI_send_16bit(0x6b, register_value) != 0)
	{
		return -1;
	}
	
	register_value = TWI_receive_8bit(0x75); //who I am confirmation
	
	if (register_value != 0x68)
	{
		return -2;
	}
	
	
	read_acceleration(0);
	x_offset_acc = acceleration_x;
	y_offset_acc = acceleration_y;
	z_offset_acc = acceleration_z;
	
	read_gyroscope();
	x_offset_gyro = gyroscope_x;
	y_offset_gyro = gyroscope_y;
	z_offset_gyro = gyroscope_z;
	
	// 	read_acceleration(1);
	//
	// 	Angle_X_Gyro = Angle_Y_Acc;
	// 	Angle_Y_Gyro = Angle_X_Acc;
	
	return register_value;
}


int8_t read_acceleration(uint8_t calibrate_acc){
	
	if (TWI_start()!= 0){ //masking for prescaler bits|
		return -1;
	}
	
	if (TWI_address(0) != 0){
		return -1;
	}

	if (TWI_send(0x3B) != 0){
		return -1;
	}
	
	if (TWI_start()!= 0){ //masking for prescaler bits|
		return -1;
	}
	
	if (TWI_address(1) != 0){
		return -1;
	}

	/*	acceleration.acceleration_x = TWI_receive_ack() << 8;
	acceleration.acceleration_x = acceleration.acceleration_x | TWI_receive_ack();
	acceleration.acceleration_y = TWI_receive_ack() << 8;
	acceleration.acceleration_y = acceleration.acceleration_y | TWI_receive_ack();
	acceleration.acceleration_z = TWI_receive_ack() << 8;
	acceleration.acceleration_z = acceleration.acceleration_z | TWI_receive_nack();*/

	acceleration_x = TWI_receive_ack() << 8;
	acceleration_x = acceleration_x | TWI_receive_ack();
	acceleration_y = TWI_receive_ack() << 8;
	acceleration_y = acceleration_y | TWI_receive_ack();
	acceleration_z = TWI_receive_ack() << 8;
	acceleration_z = acceleration_z | TWI_receive_nack();
	TWI_stop();
	
	if (calibrate_acc == 1)
	{

		acceleration_x = acceleration_x - x_offset_acc;
		
		acceleration_y = acceleration_y - y_offset_acc;

		
		if (acceleration_z > z_offset_acc)
		{
			acceleration_z = acceleration_z - 8192 + z_offset_acc;
			}else{
			acceleration_z = acceleration_z + 8192 - z_offset_acc;
		}
		
	}
	
	return 0;
}

int8_t read_gyroscope(){
	
	if (TWI_start()!= 0){ //masking for prescaler bits|
		return -1;
	}
	
	if (TWI_address(0) != 0){
		return -1;
	}

	if (TWI_send(0x43) != 0){
		return -1;
	}
	
	if (TWI_start()!= 0){ //masking for prescaler bits|
		return -1;
	}
	
	if (TWI_address(1) != 0){
		return -1;
	}

	/*	gyroscope.gyroscope_x = TWI_receive_ack() << 8;
	gyroscope.gyroscope_x = gyroscope.gyroscope_x | TWI_receive_ack();
	gyroscope.gyroscope_y = TWI_receive_ack() << 8;
	gyroscope.gyroscope_y = gyroscope.gyroscope_y | TWI_receive_ack();
	gyroscope.gyroscope_z = TWI_receive_ack() << 8;
	gyroscope.gyroscope_z = gyroscope.gyroscope_z | TWI_receive_nack();*/

	gyroscope_x = TWI_receive_ack() << 8;
	gyroscope_x = gyroscope_x | TWI_receive_ack();
	gyroscope_y = TWI_receive_ack() << 8;
	gyroscope_y = gyroscope_y | TWI_receive_ack();
	gyroscope_z = TWI_receive_ack() << 8;
	gyroscope_z = gyroscope_z | TWI_receive_nack();
	
	gyroscope_x = gyroscope_x - x_offset_gyro;
	gyroscope_y = gyroscope_y - y_offset_gyro;
	gyroscope_z = gyroscope_z - z_offset_gyro;
	
	TWI_stop();
	
	return 0;
}

uint8_t IMU_who_i_am(uint8_t print){
	uint8_t whoiam = TWI_receive_8bit(0x75);
	
	if (print == 1)
	{
		USART_Transmit(whoiam);
		USART_Transmit('\n');
	}
	
	return whoiam;
}

int8_t temperature(uint8_t print){
	int8_t temperature_read = (((float) TWI_receive_16bit(0x3b))/340 + 36.53);
	
	if (print == 1)
	{
		USART_Transmit(temperature_read);
		USART_Transmit('\n');
	}
	
	return temperature_read;
}

void computed_acceleration(uint8_t print){
	accx = ((float) acceleration_x / 8192) * 98.1;
	accy = ((float) acceleration_y / 8192) * 98.1;
	accz = ((float) acceleration_z / 8192) * 98.1;
	
	if (print == 1)
	{
		USART_Transmit(accx);
		USART_Transmit(accy);
		USART_Transmit(accz);
		USART_Transmit('\n');
	}
}

void computed_gyroscope(uint8_t print){
	gyrox = ((float) gyroscope_x / 65);
	gyroy = ((float) gyroscope_y / 65);
	gyroz = ((float) gyroscope_z / 65);
	
	if (print == 1)
	{
		USART_Transmit(gyrox);
		USART_Transmit(gyroy);
		USART_Transmit(gyroz);
		USART_Transmit('\n');
	}
}

void compute_angle_acc(uint8_t print){
	
	Angle_X_Acc = (int8_t) (atan(accx/sqrt(accy*accy+accz*accz))*180/3.1415); //transform from radiants to degree
	Angle_Y_Acc = (int8_t) (atan(accy/sqrt(accx*accx+accz*accz))*180/3.1415); //transform from radiants to degree
	
	if (print == 1)
	{
		USART_Transmit(Angle_X_Acc);
		USART_Transmit(Angle_Y_Acc);
		USART_Transmit('\n');
	}
}

void compute_angle_gyro(uint8_t print){
	
	gyrox = ((float) gyroscope_x / 65);
	gyroy = ((float) gyroscope_y / 65);
	gyroz = ((float) gyroscope_z / 65);
	
	float delta_time = system_tick_MG + 0.0000041*TCNT0 - last_sample_gyro;
	
	Angle_X_Gyro = Angle_X_Gyro + gyrox*delta_time; //transform from radiants to degree
	Angle_Y_Gyro = Angle_Y_Gyro + gyroy*delta_time;  //transform from radiants to degree

	last_sample_gyro = system_tick_MG + 0.0000041*TCNT0;
	
	if (print ==1)
	{
		USART_Transmit(Angle_X_Gyro);
		USART_Transmit(-Angle_Y_Gyro);
		USART_Transmit('\n');
	}
}






//-----------------------------------------

//FILTERING

//-----------------------------------------


volatile int8_t AngleX;
volatile int8_t AngleY;

float x_acc_k, y_acc_k, z_acc_k;
float x_gyro_k, y_gyro_k, z_gyro_k;


void angle_filtered(uint8_t print){
	AngleX = 0.05*Angle_X_Acc - 0.95*Angle_Y_Gyro; //Right math, wrong variable names - to correct in future version
	AngleY = 0.05*Angle_Y_Acc + 0.95*Angle_X_Gyro; //Right math, wrong variable names - to correct in future version

	if (print == 1)
	{
		USART_Transmit(AngleX);
		USART_Transmit(AngleY);
	}
	
}
