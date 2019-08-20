/*
* I2C.c
*
*  Author: mdotg
*/

#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

#include "I2C.h"

#include "USART.h"

#define I2C_DEBUG 1 //1 on, 0 off

volatile unsigned long F_SCL = 400000;
volatile char address = 0x68;

char error[8] = {'E', 'r', 'r', 'o', 'r', ' ', ':', ' '};
char start[7] = {'s', 't', 'a', 'r', 't', ':', ' '};
char send[6] = {'s', 'e', 'n', 'd', ':', ' '};
char receive[9] = {'r', 'e', 'c', 'e', 'i', 'v', 'e', ':', ' '};
char cap = '\n';

void TWI_init(char address_temp, unsinged long speed_temp){
	address = address_temp;
	F_SCL = speed_temp;
	TWBR = ((((F_CPU / F_SCL)) - 16 ) / 2);//set the frequency divider - formula on datasheet
	TWSR = 0; //set prescaler to 0

}

uint8_t TWI_start(){
	//TWCR = 0;
	TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT); //Start condition for TWI
	
	while (!(TWCR &(1<<TWINT)));
	
	if (((TWSR & 0xF8) == TW_START) || ((TWSR & 0xF8) == 0x10))
	{
		return 0;
	}
	else{
		if (I2C_DEBUG)
		{
			USART_String(error, 8);
			USART_String(start, 7);
			USART_Transmit(TWSR & 0xF8);
			USART_Transmit(cap);
		}
		return TWSR & 0xF8;
	}
}


uint8_t TWI_address(uint8_t mode){ //mode R_1/W_0
	
	TWDR = address<<1 | mode;//ADDRESS | mode; //Loading address

	TWCR = (1<<TWINT) |	(1<<TWEN); //Initializing communication
	
	while (!(TWCR &	(1<<TWINT))){
	}; //waiting for address to be sent
	
	if (((TWSR & 0xF8) == TW_MR_SLA_ACK) || ((TWSR & 0xF8) == TW_MT_SLA_ACK))
	{
		return 0;
	}
	else
	{
		if (I2C_DEBUG)
		{
			USART_String(error, 8);
			USART_String(send, 6);
			USART_Transmit(TWSR & 0xF8);
			USART_Transmit(cap);
		}
		
		return TWSR & 0xF8;
	}
}


uint8_t TWI_send(uint8_t data_to_be_sent){
	TWDR = data_to_be_sent;
	TWCR = (1<<TWINT | 1<<TWEN);
	
	while (!(TWCR &	(1<<TWINT))){
	}; //waiting for data to be sent
	
	if ((TWSR & 0xF8) == TW_MT_DATA_ACK)
	{
		return 0;
	}
	else
	{
		if (I2C_DEBUG)
		{
			USART_String(error, 8);
			USART_String(send, 6);
			USART_Transmit(TWSR & 0xF8);
			USART_Transmit(cap);
		}
		
		return TWSR & 0xF8;
	}
	
}

uint8_t TWI_receive_ack(){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); //ACK signal to be sent after read complete
	while (!(TWCR &	(1<<TWINT))){
	}; //waiting all bits to be received
	if ((TWSR & 0xF8) == TW_MR_DATA_ACK)
	{
		return TWDR;
	}
	else
	{
		if (I2C_DEBUG)
		{
			USART_String(error, 8);
			USART_String(receive, 9);
			USART_Transmit(TWSR & 0xF8);
			USART_Transmit(cap);
		}
		
		return TWSR & 0xF8;
	}
}

uint8_t TWI_receive_nack(){
	TWCR = (1<<TWINT) | (1<<TWEN); //NACK signal to be sent after read complete
	while (!(TWCR &	(1<<TWINT))){
	}; //waiting all bits to be received

	if ((TWSR & 0xF8) == TW_MR_DATA_NACK)
	{
		return TWDR;
	}
	else
	{
		if (I2C_DEBUG)
		{
			USART_String(error, 8);
			USART_String(receive, 9);
			USART_Transmit(TWSR & 0xF8);
			USART_Transmit(cap);
		}
		
		return TWSR & 0xF8;
	}
}

void TWI_stop(){
	TWCR =	(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}


uint8_t TWI_send_8bit(uint8_t data){

	if (TWI_start()!= 0){
		return TWSR & 0xF8;
	}
	
	if (TWI_address(0) != 0){
		return TWSR & 0xF8;
	}
	if (TWI_send(data) != 0){
		return TWSR & 0xF8;
	}
	TWI_stop();
	return 0;
}

uint8_t TWI_send_16bit(uint8_t data, uint8_t data1){

	if (TWI_start()!= 0){
	}
	
	if (TWI_address(0) != 0){
		return TWSR & 0xF8;
	}
	if (TWI_send(data) != 0){
		return TWSR & 0xF8;
	}
	if (TWI_send(data1) != 0){
		return TWSR & 0xF8;
	}

	TWI_stop();
	return 0;
}

uint8_t TWI_send_several_bit(uint8_t *data, uint8_t lenght){ //pass a declared array and its lenght

	if (TWI_start()!= 0){ //masking for prescaler bits|
		return TWSR & 0xF8;
	}
	
	if (TWI_address(0) != 0){
		return TWSR & 0xF8;
	}
	for (int i=0; i<lenght; i++)
	{
		if (TWI_send(data[i]) != 0){
			return TWSR & 0xF8;
		}
		
	}
	
	TWI_stop();
	return 0;
}


uint8_t TWI_receive_8bit(uint8_t registro){
	if (TWI_start()!= 0){ //masking for prescaler bits|
		return TWSR & 0xF8;
	}
	
	if (TWI_address(0) != 0){
		return TWSR & 0xF8;
	}
	
	if (TWI_send(registro) != 0){
		return TWSR & 0xF8;
	}
	
	
	if (TWI_start()!= 0){
		return TWSR & 0xF8;
	}
	
	if (TWI_address(1) != 0){
		return TWSR & 0xF8;
	}

	uint8_t data = 0;
	data = TWI_receive_nack();
	TWI_stop();
	return data;
}

uint16_t TWI_receive_16bit(uint8_t registro){
	if (TWI_start()!= 0){
		return TWSR & 0xF8;
	}
	
	if (TWI_address(0) != 0){
		return TWSR & 0xF8;
	}

	if (TWI_send(registro) != 0){
		return TWSR & 0xF8;
	}

	if (TWI_start()!= 0){
		return TWSR & 0xF8;
	}
	
	if (TWI_address(1) != 0){
		return TWSR & 0xF8;
	}


	uint16_t data = 0;
	data = TWI_receive_ack()<<8;
	data = data | TWI_receive_nack();
	TWI_stop();
	return data;
}
