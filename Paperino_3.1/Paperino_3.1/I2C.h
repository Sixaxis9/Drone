/*
* I2C.h
*
*  Author: mdotg
*/


#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <util/twi.h>


void TWI_init(char address_temp, unsinged long speed_temp);
uint8_t TWI_start();
uint8_t TWI_address(uint8_t mode);
uint8_t TWI_send(uint8_t data_to_be_sent);
uint8_t TWI_receive_ack();
uint8_t TWI_receive_nack();
void TWI_stop();
uint8_t TWI_send_8bit(uint8_t data);
uint8_t TWI_send_16bit(uint8_t data, uint8_t data1);
uint8_t TWI_send_several_bit(uint8_t *data, uint8_t lenght);
uint8_t TWI_receive_8bit(uint8_t registro);
uint16_t TWI_receive_16bit(uint8_t registro);


#endif /* I2C_H_ */