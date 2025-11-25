/*
 * timer_1s_delay.c
 *
 * Created: 25-11-2025 22:05:25
 * Author : hp
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
void delay_1s()
{
	TCNT0=0;//count from 0-255,256 counts
	
	TCCR0A&=~((1<<WGM00)|(1<<WGM01));//set normal mode
	TCCR0B&=~((1<<CS01)|(1<<CS02));//set no pre scaling
	TCCR0B=(1<<CS00);//set clk frequency without pre scaling
	
	for(unsigned int i=0;i<62500;i++)//run loop 62500 times to create 1 second loop
	{                                //use unsigned int for positive values
		while(!(TIFR0&(1<<TOV0)));//count until flag sets .000016 second delay
		TIFR0=(1<<TOV0);//set flag to clear
	}
	

}

int main(void)
{
	DDRB|=(1<<DDB5);//set pb5 as output
	while (1)
	{
		PORTB^=(1<<PORTB5);//toggle constantly after every 1 second 
		delay_1s();
	}
}


