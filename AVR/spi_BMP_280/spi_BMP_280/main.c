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
	uint8_t b[3];// array to store bytes
	uint32_t value;// for 20 bit value
	
	uint16_t dig_t1;
	int16_t dig_t2,dig_t3;// calibration values
	uint8_t calib[6];//calibration bytes, 6 bytes for three 16bit values
	
	float var1,var2,t_fine,temp;// compensation formula variables
	
   uart_init();// UART for debugging
   spi_init(); // initialize SPI
    	
	  sen_id = BMP280_read(0xD0 | 0x80); // send command to read sensor id register  
	    
	TX_hex_num(sen_id);// output expected is 0x58 
	TX_string("\nReceived the sensor id\n");
	
	
	
	
	BMP_write(0xF4 & 0x7F, 0x27);// configure control meas register(0xF4), reading temperature, continuous mode
	                             // & 0x7F indicates write operation, MSB as 0
								 
								 //read calibration bytes 
	SS_LOW();// select slave
	
	spi_transfer(0x88 | 0x80);                  // send command to read calibration bytes
	
	for(uint8_t i=0;i<6;i++)
	{
		calib[i]=spi_transfer(0xAA);// send dummy bytes to receive calibration bytes
	}
	
	SS_HIGH();// de select slave
		
	dig_t1=((uint16_t)calib[1]<<8)|(uint16_t)calib[0];// calib[1] as MSB and calib[0] as LSB, do type cast to form values
	dig_t2=((int16_t)calib[3]<<8)|(int16_t)calib[2];// calib[3] as MSB and calib[2] as LSB, do type cast to form values
	dig_t3=((int16_t)calib[5]<<8)|(int16_t)calib[4];// calib[5] as MSB and calib[4] as LSB, do type cast to form values
		
						 
    while (1) 
    {
		SS_LOW();// select slave
		
		spi_transfer(0xFA|0x80);// send read command to data register 0xFA
		                        // send dummy bytes to receive data
		b[0]=spi_transfer(0xAA);// upper 8 bits , MSB
		b[1]=spi_transfer(0xAA);// middle 8 bits, LSB
		b[2]=spi_transfer(0xAA);// lower 4 bits , XLSB
		
		SS_HIGH();// de select slave
		
		value=((uint32_t)b[0]<<12)|((uint32_t)b[1]<<4)|((uint32_t)b[2]>>4);// arrange bytes in correct order to form 20 bit value
		
		TX_string("\n\nRaw value : ");
		TX_32bit_num(value);
		TX_char('\n');
		
		TX_string("Calibration values : \n");
		TX_num(dig_t1);
		TX_char('\n');
		
		TX_num(dig_t2);
		TX_char('\n');
		
		TX_num(dig_t3);
		TX_char('\n');
		
		var1=((value/16384.0f)-(dig_t1/1024.0f))*dig_t2;
		var2=((value/131072.0f)-(dig_t2/8192.0f))*dig_t3;
		t_fine=var1+var2;
		
		temp=t_fine/5120.0f;// actual temperature
		
		TX_string("Temperature : ");
		TX_Float_send(temp);
		
		_delay_ms(250);
    }
}

