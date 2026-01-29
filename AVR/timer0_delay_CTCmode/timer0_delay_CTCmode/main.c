/*
 * timer0_delay_CTCmode.c
 *
 * Created: 27-11-2025 21:00:50
 * Author : hp
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
void delay_1s()
{
	
	TCNT0=0;//start count from 0
	
	TCCR0A|=((1<<WGM01));//set CTC mode
	TCCR0A&=~((1<<WGM00));//set CTC mode
	
	OCR0A=199;//count up to 199, making 200 counts
	 
	TCCR0B&=~((1<<CS01)|(1<<CS02));//set no pre scalar
	TCCR0B|=(1<<CS00);//set frequency as same as clock 
	
	for(long int i=0;i<80000;i++)// run loop for 80000 times to create a delay of 80000 * 12.5 micoseconds= 1second
	{
		while(!(TIFR0&(1<<OCF0A)));// creates delay of 200*(1/16000000)= 12.5 microseconds
		TIFR0|=(1<<OCF0A);//set flag to clear
	}
}

int main(void)
{
	DDRD|=(1<<DDD5);//set PD5 as output
	while (1)
	{
		PORTD^=(1<<PORTD5);//toggle
		delay_1s();//delay for 1 second
	}
}


