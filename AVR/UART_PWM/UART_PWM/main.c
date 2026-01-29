/*
 * UART_PWM.c
 *
 * Created: 28-12-2025 18:32:59
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "uart.h"

void PWM_setup()
{
	TCCR0A|=(1<<WGM01)|(1<<WGM00);// set fast PWM mode
	
	TCCR0A|=(1<<COM0B1);// non inverting PWM on OC0B
	
	TCCR0B|=(1<<CS01);// set prescalar 8, PWM frequency=16000000/(256*8)= 7.8KHz
	
	DDRD|=(1<<DDD5);// set PD5 as output 
	
}


int main(void)
{
   uart_init();// initialize UART
   
   PWM_setup();
   
   uint16_t dutycycle;
   char buf[30];// array to receive number string
    
	TX_string("Enter the duty cycle (0-255) ");
	
    while (1) 
    {
		RX_string(buf,sizeof(buf));// receive string
		
		dutycycle=RX_num(buf);// convert string to a 16 bit number
		
		if(dutycycle>255)// values always less than 255 
		dutycycle=255;
		
		OCR0B=(dutycycle );
		
		TX_string("\nCurrent duty cycle : ");
		
		TX_num(dutycycle);
		
    }
}

