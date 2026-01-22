/*
 * spi_BMP280_pressure.c
 *
 * Created: 11-01-2026 17:53:42
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
	uint32_t t_fine,value;// for 20 bit value
	
	uint8_t pre[3];// array to store bytes
	uint32_t p_value;// for 20 bit value
	
	uint16_t dig_t1;
	int16_t dig_t2,dig_t3;//temperature calibration values
	uint8_t calib[6];//temperature calibration bytes, 6 bytes for three 16bit values
	float var1,var2;//temperature compensation formula variables
	
	uint16_t dig_p1;
	int16_t dig_p2,dig_p3,dig_p4,dig_p5,dig_p6,dig_p7,dig_p8,dig_p9;//pressure calibration values
	uint8_t p_calib[18];//pressure calibration bytes, 18 bytes for nine 16bit values
	double p_var1,p_var2,pressure;//pressure compensation formula variables
	
	uart_init();// UART for debugging
	spi_init(); // initialize SPI
	
	sen_id = BMP280_read(0xD0 | 0x80); // send command to read sensor id register
	
	TX_hex_num(sen_id);// output expected is 0x58
	TX_string("\nReceived the sensor id\n");
	
	
	//read calibration bytes
	
	SS_LOW();// select slave
	
	spi_transfer(0x88 | 0x80);                  // send command to read temperature calibration bytes
	
	for(uint8_t i=0;i<6;i++)
	{
		calib[i]=spi_transfer(0xAA);// send dummy bytes to receive calibration bytes
	}
	
	SS_HIGH();// de select slave
	
	dig_t1=((uint16_t)calib[1]<<8)|(uint16_t)calib[0];// calib[1] as MSB and calib[0] as LSB, do type cast to form values
	dig_t2=((int16_t)calib[3]<<8)|(int16_t)calib[2];// calib[3] as MSB and calib[2] as LSB, do type cast to form values
	dig_t3=((int16_t)calib[5]<<8)|(int16_t)calib[4];// calib[5] as MSB and calib[4] as LSB, do type cast to form values
	
	SS_LOW();// select slave for reading pressure calibration bytes
	
		spi_transfer(0x8E | 0x80);                  // send command to read pressure calibration bytes, starts from register 0x8E
		
		for(uint8_t i=0;i<18;i++)
		{
			p_calib[i]=spi_transfer(0xAA);// send dummy bytes to receive calibration bytes
		}	
	SS_HIGH();// de select after reading
	
	dig_p1=((uint16_t)p_calib[1]<<8|p_calib[0]);// p_calib[1] as MSB and p_calib[0] as LSB, do type cast to form values
	dig_p2=((int16_t)p_calib[3]<<8)|p_calib[2];// p_calib[3] as MSB and p_calib[2] as LSB, do type cast to form values
	dig_p3=((int16_t)p_calib[5]<<8)|p_calib[4];// p_calib[5] as MSB and p_calib[4] as LSB, do type cast to form values	
	dig_p4=((uint16_t)p_calib[7]<<8 | p_calib[6]);// p_calib[7] as MSB and p_calib[6] as LSB, do type cast to form values
	dig_p5=((int16_t)p_calib[9]<<8)|p_calib[8];// p_calib[9] as MSB and p_calib[8] as LSB, do type cast to form values
	dig_p6=((int16_t)p_calib[11]<<8)|p_calib[10];// p_calib[11] as MSB and p_calib[10] as LSB, do type cast to form values
	dig_p7=((int16_t)p_calib[13]<<8)|p_calib[12];// p_calib[13] as MSB and p_calib[12] as LSB, do type cast to form values
	dig_p8=((int16_t)p_calib[15]<<8)|p_calib[14];// p_calib[15] as MSB and p_calib[14] as LSB, do type cast to form values
	dig_p9=((int16_t)p_calib[17]<<8)|p_calib[16];// p_calib[17] as MSB and p_calib[16] as LSB, do type cast to form values
	
	
	BMP_write(0xF4 & 0x7F, 0x27);// configure control meas register(0xF4), reading temperature, continuous mode
	// & 0x7F indicates write operation, MSB as 0
	
	while (1)
	{
		SS_LOW();// select slave for reading temperature data
		
		spi_transfer(0xFA|0x80);// send read command to data register 0xFA
		// send dummy bytes to receive data
		b[0]=spi_transfer(0xAA);// upper 8 bits , MSB
		b[1]=spi_transfer(0xAA);// middle 8 bits, LSB
		b[2]=spi_transfer(0xAA);// lower 4 bits , XLSB
		
		SS_HIGH();// de select slave
		
		
		value=((uint32_t)b[0]<<12)|((uint32_t)b[1]<<4)|((uint32_t)b[2]>>4);// arrange bytes in correct order to form 20 bit value
		
		var1=((value/16384.0f)-(dig_t1/1024.0f))*dig_t2;// compensation formula for temperature
		var2=((value/131072.0f)-(dig_t2/8192.0f))*dig_t3;
		t_fine=var1+var2;
		
		SS_LOW();// select slave for reading pressure data
		
		spi_transfer(0xF7|0x80);// send read command to data register 0xF7, pressure data
		// send dummy bytes to receive data
		pre[0]=spi_transfer(0xAA);// upper 8 bits , MSB
		pre[1]=spi_transfer(0xAA);// middle 8 bits, LSB
		pre[2]=spi_transfer(0xAA);// lower 4 bits , XLSB
		
		SS_HIGH();// de select slave
		
		p_value=((uint32_t)pre[0]<<12)|((uint32_t)pre[1]<<4)|((uint32_t)pre[2]>>4);// arrange bytes in correct order to form 20 bit value for pressure
		
		                                                                       // compensation formula for pressure
		p_var1=((double)t_fine/2.0f)-64000.0;
		p_var2=p_var1*p_var1*((double)dig_p6/32768.0f);
		p_var2=(p_var2/4.0f)+p_var1*((double)dig_p5*2.0f);
		p_var2=(p_var2)+((double)dig_p4*65536.0f);	
	    p_var1=(double)dig_p3*p_var1*(p_var1/524288.0f)+((double)dig_p2*(p_var1/524288.0f));
		p_var1=(1.0f+p_var1/32768.0f)*(double)dig_p1;
		
		pressure=1048576.0f-(double)p_value;
		pressure=(pressure-(p_var2/4096.0f))*(6250.0f/p_var1);
		
		p_var1=(double)dig_p9*pressure*(pressure/2147483648.0f);
		p_var2=pressure*(double)dig_p8/32768.0f;
		
		pressure=pressure+(p_var1+p_var2+(double)dig_p7)/16.0f;// pressure in pascal
																		   
		
		
		TX_string("\nPressure : ");
		TX_Float_send(pressure);
		TX_char('\n');
		TX_num(dig_p4);
		TX_char('\n');
		TX_hex_num(p_calib[6]); TX_string(" ");
		TX_hex_num(p_calib[7]);

		
		_delay_ms(500);
	}
}


