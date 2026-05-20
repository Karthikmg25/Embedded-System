/*
 * I2C.c
 *
 * Created: 20-05-2026 15:25:17
 *  Author: hp
 */ 


#include "I2C.h"

// Initialize I2C Hardware
// - Set bit rate - 100KHz
// -- f_SCL = F_CPU/ 16 + (2 * TWBR * Prescalar)
// - Enable hardware
void Init_I2C()
{
	// set prescaler 1 : TWPS1:0 =00
	TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
	// set TWBR
	// TWBR = ((F_CPU/100KHz)-16)/2 = 72
	TWBR = 72;
	// enable I2C
	TWCR |= (1<< TWEN);
}
uint8_t I2C_Generate_START()
{
	// generate START condition
	// clear TWINT flag, wait till flag goes HIGH
	// return status code ( 0x08 or 0x10 ): Upper 5bits of TWSR
	
	TWCR = (1<<TWINT )|(1<< TWSTA)|(1<< TWEN);
	while(!(TWCR &(1<<TWINT)));
	return (TWSR & 0xF8);
}
uint8_t I2C_Send_SlaveAddress_Write(uint8_t slv_addr)
{
	
	// write 7bit address + write bit(0) to Data register
	// clear TWINT flag : clearing TWINT starts the operation 
	// wait till TWINT goes HIGH: transmission complete
	// return status code (0x18)
	
	TWDR = (slv_addr<<1)|0;
	TWCR = (1<< TWINT)|(1<< TWEN);
	while(!(TWCR &(1<<TWINT)));
	return (TWSR & 0xF8);
}
uint8_t I2C_Send_SlaveAddress_Read(uint8_t slv_addr)
{
	
	// write 7bit address + read bit(1) to Data register
	// clear TWINT flag : clearing TWINT starts the operation
	// wait till TWINT goes HIGH: transmission complete
	// return status code (0x40)
	
	TWDR = (slv_addr<<1)|1;
	TWCR = (1<< TWINT)|(1<< TWEN);
	while(!(TWCR &(1<<TWINT)));
	return (TWSR & 0xF8);
}
uint8_t I2C_Send_byte(uint8_t byte)
{
	// write data to Data register
	// clear TWINT flag : clearing TWINT starts the operation
	// wait till TWINT goes HIGH: transmission complete
	// return status code (0x28)
	
	TWDR = byte;
	TWCR = (1<< TWINT)|(1<< TWEN);
	while(!(TWCR &(1<<TWINT)));
	return (TWSR & 0xF8);
}
uint8_t I2C_Read_byte_ACK()
{
	// clear TWINT flag
	// wait till TWINT goes HIGH: reception complete
	// read byte from data register
	// send acknowledgment: byte received
	// return receive byte
	TWCR = (1<< TWINT)|(1<<TWEA)|(1<< TWEN);
	while(!(TWCR &(1<<TWINT)));
	return TWDR;
}
uint8_t I2C_Read_byte_NACK()
{
	// clear TWINT flag
	// wait till TWINT goes HIGH: reception complete
	// read byte from data register
	// send acknowledgment: last byte received
	// return receive byte
	TWCR = (1<< TWINT)|(1<< TWEN);
	while(!(TWCR &(1<<TWINT)));
	return TWDR;
}
void I2C_Generate_STOP()
{
	// generate STOP condition
	TWCR = (1<<TWINT )|(1<< TWSTO)|(1<< TWEN);
}