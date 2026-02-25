/*
 * I2c_real_time_clock.c
 *
 * Created: 15-01-2026 19:26:50
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "I2c.h"
#include "RTC.h"
#define RTC_ADDR 0x68


int main(void)
{
	I2c_init();// initialize I2c
	
    uart_init();// initialize UART
	             
				                 // initialize  real time clock			 				  
	if(RTC_read_byte(0x00)&0x80) // write only when clock is not running, don't over-write every time we run code
	{                            // MSB of sec register = 1 means clock is not ticking
		
		RTC_write_time(8,29,10); // writing hours, minutes ,seconds
	}			  
	
																			
	uint8_t time[3];// for storing seconds,minutes and hours
																	
    while(1) 
    {
		                                                        
		RTC_read_time(time);// read seconds,minutes and hours registers
		
		TX_string("Time - ");
			
		TX_num((time[2]));// hours
		TX_string(" : ");
		TX_num((time[1]));// minutes
		TX_string(" : ");		
		TX_num(time[0]);// seconds
		
		
		_delay_ms(500);
		TX_string("\n\n");
    }
}

