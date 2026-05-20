/*
 * BMP280.c
 *
 * Created: 19-05-2026 18:02:21
 *  Author: hp
 */ 

#include <avr/io.h>
#include "BMP280.h"
#include "SPI.h"

uint8_t BMP280_Read_Byte(uint8_t reg_addr)
{
	uint8_t byte;
	
	// Initiate communication with sensor
	// - select BMP280 by pulling SS_PIN LOW
	// - send register address with MSB=1 for read
	// - send dummy bytes to read data
	// - end communication : SS_PIN = HIGH
	
	Select_slave();
	SPI_transfer( reg_addr| 0x80);
	byte = SPI_transfer(0xFF);
	Deselect_slave();
	
	return byte;
}
// Function to read multiple bytes
void BMP280_Read_Multi_Byte(uint8_t reg_addr, uint8_t *buff, uint8_t n)
{
	// Initiate communication with sensor
	// - select BMP280 by pulling SS_PIN LOW
	// - send register address with MSB=1 for read
	// - send dummy bytes to read n bytes of data
	// - store received bytes to a buffer
	// - end communication : SS_PIN = HIGH
	
	Select_slave();
	SPI_transfer( reg_addr| 0x80);
	// Read n bytes
	for(uint8_t i=0;i <n; i++)
	{
		*buff = SPI_transfer(0xFF);
		buff++;
	}
	Deselect_slave();
	
}

// Function to write a single byte to a register
void BMP280_Write_Byte(uint8_t reg_addr, uint8_t data)
{
	// Initiate communication with sensor
	// - select BMP280 by pulling SS_PIN LOW
	// - send register address with MSB=0 for write
	// - send data 
	// - end communication : SS_PIN = HIGH
	
	Select_slave();
	SPI_transfer(reg_addr & 0x7F);
	SPI_transfer(data);
	Deselect_slave();
}