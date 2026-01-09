/*
 * spi_BMP_280.c
 *
 * Created: 06-01-2026 20:44:11
 * Author : hp
 */ 
#define F_CPU 16000000UL
#define MISO PORTB4
#define MOSI PORTB3
#define SCK PORTB5
#define SS PORTB2


#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

 void spi_init()
 {
	 DDRB|=(1<<MOSI)|(1<<SS)|(1<<SCK);
	 DDRB&=~(1<<MISO);// set MISO as input others as output
	 
	 SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR0);// enable spi, set master mode, set prescalar 16
	 SPCR&=~((1<<CPOL)|(CPHA)|(1<<SPR1));
 }
 uint8_t spi_transfer(uint8_t data)
 {
	 uint8_t d;
	 SPDR=data;// writing data enables data transfer
	 
	 while(!(SPSR&(1<<SPIF)));// wait till data transfer is over
	 
	 d=SPDR;// read data after transfer, reading clears SPIF
	 
	 return d;
 }

void SS_LOW()
{
	PORTB&=~(1<<SS); 
}
void SS_HIGH()
{
	 PORTB|=(1<<SS);// de select slave
}

int8_t BMP280_read(uint8_t cmd)
{
	uint8_t id;
	
	
	SS_LOW();// select slave
	
	spi_transfer(cmd);// send read command
	id=spi_transfer(0xAA);// send dummy byte to receive data
	
	SS_HIGH();//de select slave
	
	return id;
}

void BMP_write(uint8_t cmd, uint8_t data)
{
	SS_LOW();// select slave
	
	spi_transfer(cmd);// send write command
	
	spi_transfer(data);// send data to write
	
	SS_HIGH();// de select slave
	
}
int main(void)
{
	uint8_t sen_id;
	uint8_t b[3];// array to store value
	uint32_t value;// for 20 bit value
	
   uart_init();// UART for debugging
   spi_init();// initialize SPI
    	
	  sen_id = BMP280_read(0xD0); // send command 0xD0 to read sensor id 
	    
	TX_hex_num(sen_id);// output expected is 0x58 
	TX_string("\nReceived the sensor id\n");
	
	
	
	
	BMP_write(0xF4 & 0x7F, 0x27);// configure control meas register(0xF4), 
	
    while (1) 
    {
		SS_LOW();// select slave
		
		spi_transfer(0xFA|0x80);// send read command to data register 0xFA
		
		for(uint8_t i=0;i<3;i++)
		{
			b[i]=spi_transfer(0xAA);// send dummy bytes to receive data
		}
		
		SS_HIGH();// de select slave
		
		value=((uint32_t)b[0]<<12)|((uint32_t)b[1]<<4)|((uint32_t)b[2]>>4);// b[0] is the upper 8 bits, b[1] as middle 8 bits and b[2] as lowest 4bits
		
		TX_32bit_num(value);
		TX_char('\n');
		
		_delay_ms(250);
    }
}

