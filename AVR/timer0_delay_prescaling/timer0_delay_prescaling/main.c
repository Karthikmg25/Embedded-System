/*
 * timer0_delay_prescaling.c
 *
 * Created: 27-11-2025 20:27:18
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
void delay_1s()
{
	
	TCNT0=0;//start count from 0 to 255
	TCCR0A&=~((1<<WGM00)|(1<<WGM01));//set normal mode
	TCCR0B|=(1<<CS01)|(1<<CS00);//set pre scalar 64, frequency= CLK frequency/64 =250000
	
	for(int i=0;i<976;i++)// run loop for 976 times to create a delay of 976 * 0.001024= 1second
	{
		while(!(TIFR0&(1<<TOV0)));// creates delay of 256*0.000004= 0.001024 seconds
		TIFR0|=(1<<TOV0);//set flag to clear
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

