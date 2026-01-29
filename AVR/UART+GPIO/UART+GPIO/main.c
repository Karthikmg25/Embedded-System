/*
 * UART+GPIO.c
 *
 * Created: 28-12-2025 20:57:52
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
void white_led()
{
	PORTC|=(1<<PORTC1);// ON white led
	PORTC&=~((1<<PORTC2)|(1<<PORTC3));// others off
}
void blue_led()
{
	PORTC|=(1<<PORTC3);// ON blue led
	PORTC&=~((1<<PORTC1)|(1<<PORTC2));// others off
}
void yellow_led()
{
	PORTC|=(1<<PORTC2);// yellow on
	
	PORTC&=~((1<<PORTC1)|(1<<PORTC3));// others off	
}

int main(void)
{
	DDRC|=(1<<DDC1)|(1<<DDC2)|(1<<DDC3);// set PC1,PC2,PC3 as outputs
	
	uart_init();
	
	static char str[20];
	
   
   TX_string("Select led (white,blue,yellow)");
   
    while (1) 
    {
		RX_string(str,sizeof(str));// receive string
		
		switch(*str)
		{
			case 'w' : white_led();
			           break;
	        case 'b' : blue_led();
			           break;
			case 'y' : yellow_led();
					   break;
		    default  : PORTC&=~((1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3));// OFF everything			   				   
		}
		
		TX_string("Current state : ");
		TX_string(str);
		TX_char('\n');
		_delay_ms(200);
    }
}

