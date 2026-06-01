/*
 * BMP280_I2Cc.c
 *
 * Created: 20-05-2026 17:23:03
 *  Author: hp
 */ 

#include "BMP280_I2C.h"
#include <avr/io.h>

// Function for reading a single byte
uint8_t BMP_Read_Byte(uint8_t reg_addr)
{
	// start communication
	I2C_Generate_START();
	
	// call for write operation
	I2C_Send_SlaveAddress_Write(BMP280_ADDR);
	
	// send register address to read byte
	I2C_Send_byte(reg_addr);
	
	// Restart condition
	I2C_Generate_START();
	
	// call for read operation
	I2C_Send_SlaveAddress_Read(BMP280_ADDR);
	
	// read byte with NACK
	uint8_t byte=  I2C_Read_byte_NACK();
	
	// Stop communication
	I2C_Generate_STOP();
	
	return byte;
	
}

// Function for reading n bytes
// Received bytes are stored into a buffer
void BMP_Read_Multi_Byte(uint8_t reg_addr, uint8_t *buff, uint8_t n)
{
	// start communication
	I2C_Generate_START();
	
	// call for write operation
	I2C_Send_SlaveAddress_Write(BMP280_ADDR);
	
	// send register address to read byte
	I2C_Send_byte(reg_addr);
	
	// Restart condition
	I2C_Generate_START();
	
	// call for read operation
	I2C_Send_SlaveAddress_Read(BMP280_ADDR);
	
	// read n-1 bytes with ACK
	for(uint8_t i=0;i<n-1;i++)
	{
		*buff=  I2C_Read_byte_ACK();
		buff++;
	}
	// read last byte with NACK
	*buff = I2C_Read_byte_NACK();
	
	// Stop communication
	I2C_Generate_STOP();
	
	
}
// Function to write byte to a slave register
void BMP_Write_Byte(uint8_t reg_addr, uint8_t byte)
{
	
	// start communication
	I2C_Generate_START();
	
	// call for write operation
	I2C_Send_SlaveAddress_Write(BMP280_ADDR);
	
	// send register address 
	I2C_Send_byte(reg_addr);
	
	// send byte
	I2C_Send_byte(byte);
	
	// Stop communication
	I2C_Generate_STOP();
	
}