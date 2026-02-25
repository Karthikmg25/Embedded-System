/*
 * I2c.c
 *
 * Created: 15-01-2026 19:33:03
 *  Author: hp
 */ 
#include <avr/io.h>
void I2c_init()
{
	TWSR=0x00;// clear status register, setting pre scalar 1
	TWBR= 72;// for frequency of 100KHz, I2c frequency= F_CPU/16+ 2*twbr*prescalar
	
	TWCR=(1<<TWEN);// enable I2c hardware
}
uint8_t I2c_start()
{
	TWCR=(1<<TWSTA)|(1<<TWINT)|(1<<TWEN);// START, clear TWINT
	while(!(TWCR&(1<<TWINT)));// wait till TWINT=1, finish
	
	uint8_t status=TWSR&0xF8;// upper 5 bits of TWSR makes status code
	
	return((status==0x08)|(status==0x10));// return 1 if START(0x08) or RESTART(0x10), else return 0
	
}
uint8_t I2c_slave_add_write(uint8_t add)// 7 bit slave address
{
	TWDR=(add<<1)| 0;// address + write bit
	
	TWCR=(1<<TWINT)|(1<<TWEN);// clear TWINT. if we don't set TWEN , it means TWEN=0 and I2c won't work
	
	while(!(TWCR&(1<<TWINT)));// wait till TWINT=1, till data transfer
	
	uint8_t status=TWSR&0xF8;// check status
	
	return(status==0x18);// return 1 if ACK received- 0x18
}
uint8_t I2c_send_byte(uint8_t byte)// for sending register address or data
{
	TWDR=byte;// write for transfer
	
	TWCR=(1<<TWINT)|(1<<TWEN);// clear TWINT
	
	while(!(TWCR&(1<<TWINT)));// wait till TWINT=1, till data transfer
	
	uint8_t status=TWSR&0xF8;// check status
	
	return(status==0x28);// return 1 if ACK received- 0x28
}
uint8_t I2c_slave_add_read(uint8_t add)// 7 bit slave address
{
	TWDR=(add<<1)| 1;// address + read bit
	
	TWCR=(1<<TWINT)|(1<<TWEN);// clear TWINT
	
	while(!(TWCR&(1<<TWINT)));// wait till TWINT=1, till data transfer
	
	uint8_t status=TWSR&0xF8;// check status
	
	return(status==0x40);// return 1 if ACK received- 0x40
}
uint8_t I2c_read_byte_ack()
{
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);// TWEA=1 means sending ACK
	
	while(!(TWCR&(1<<TWINT)));// wait till TWINT=1, till data receive
	
	return TWDR;// read data
}
uint8_t I2c_read_byte_nack()
{
	TWCR=(1<<TWINT)|(1<<TWEN);// TWEA=0 means sending NACK, receiving last byte
	
	while(!(TWCR&(1<<TWINT)));// wait till TWINT=1, till data receive
	
	return TWDR;// read data
}
void I2c_stop()
{
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);
}
