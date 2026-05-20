/*
 * I2C.h
 *
 * Created: 20-05-2026 15:25:38
 *  Author: hp
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>

void Init_I2C();

uint8_t I2C_Generate_START();

uint8_t I2C_Send_SlaveAddress_Write(uint8_t slv_addr);

uint8_t I2C_Send_SlaveAddress_Read(uint8_t slv_addr);

uint8_t I2C_Send_byte(uint8_t byte);

uint8_t I2C_Read_byte_ACK();

uint8_t I2C_Read_byte_NACK();

void I2C_Generate_STOP();



#endif /* I2C_H_ */