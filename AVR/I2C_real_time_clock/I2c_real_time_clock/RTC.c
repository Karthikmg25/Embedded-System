/*
 * RTC.c
 *
 * Created: 22-01-2026 17:40:33
 *  Author: hp
 *  Real Time Clock functions
 *
 */                      
#include <avr/io.h>
#include "I2c.h"
#define RTC_ADDR 0x68

void RTC_write_time(uint8_t hr,uint8_t min,uint8_t sec)
{                                                     
	I2c_start();
	
	I2c_slave_add_write(RTC_ADDR);// 0x68 is the slave address of DS1307
	
	I2c_send_byte(0x00);// 0x00 is the seconds register address
	
	I2c_send_byte((dec_to_bcd(sec)& 0x7F));// send data, write 30 as seconds, MSB of seconds register must be alway zero, else clock won't tick
	
	I2c_send_byte(dec_to_bcd(min));// write minutes, register address increments to 0x01 (minutes register)
	
	I2c_send_byte(dec_to_bcd(hr));// write hours, register address increments to 0x02 (hours register)
	
	I2c_stop();
}
void RTC_read_time(uint8_t time[3])
{
	uint8_t sec,min,hr;
	
	I2c_start();
	
	I2c_slave_add_write(RTC_ADDR);
	
	I2c_send_byte(0x00);//send register address to start reading
	
	I2c_start();// repeated start
	
	I2c_slave_add_read(RTC_ADDR);
	
	sec=I2c_read_byte_ack();// read seconds
	
	min=I2c_read_byte_ack();// read minutes
	
	hr=I2c_read_byte_nack();// read hours
	
	I2c_stop();
	                       // convert from BCD to decimal
	time[0]=bcd_to_dec(sec&0x7F);
	time[1]=bcd_to_dec(min);
	time[2]=bcd_to_dec(hr);					   
	
}

uint8_t RTC_read_byte(uint8_t add)
{
	uint8_t data;
	
	I2c_start();
	I2c_slave_add_write(0x68);// slave address + W
	I2c_send_byte(add);// send register address
	I2c_start();// repeated start
	I2c_slave_add_read(0x68);// slave address + R
	data=I2c_read_byte_nack();// read single byte
	I2c_stop();
	
	return data;
}
void RTC_write_byte(uint8_t add, uint8_t data)
{
	I2c_start();
	I2c_slave_add_write(0x68);// RTC address + w
	I2c_send_byte(add);// send register address
	I2c_send_byte(data);// send data
	I2c_stop();
	
}
uint8_t dec_to_bcd(uint8_t value)
{
	uint8_t BCD= ((value/10)<<4)|((value%10)&0x0F);// if value=56, make 5 as upper nibble and 6 as lower nibble
	
	return BCD;
}
uint8_t bcd_to_dec(uint8_t bcd)
{
	uint8_t dec= ((bcd>>4)*10)+(bcd&0x0F);// if bcd=0x56, multiply upper nibble with 10 and add lower nibble
	
	return dec;
}