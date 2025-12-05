/*
 * timer0_interrupt.c
 *
 * Created: 04-12-2025 20:51:42
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include<avr/interrupt.h>

volatile uint16_t count=0;

ISR(TIMER0_OVF_vect)// happens every 1.02 milliseconds
{
	if(count==977) // 977 counts of 1.02 milliseconds creates a delay of 1 second
	{
		PORTB^=(1<<PORTB5);// blinks LED every 1 second
		
		count=0;// clear the count value for further counts
	}
	else
	{ count++;}
}
	


int main(void)
{
	DDRB|=(1<<DDB5);// set PB5 as OUTPUT
	
	sei();//enable global interrupt
	
	TCCR0A&=~((1<<WGM01)|(1<<WGM00));//set normal mode
	
    TCNT0=0;//count from 0 to 255
	
	TCCR0B|=(1<<CS01)|(1<<CS00);// set pre scalar 64, CLK frequency =16Mhz/64= 250 KHz 
	                           // CLK time period, T= 4 microseconds, 256 counts= 256 * 4 microsecond= 1.02 ms
							   
	                            // interrupt happens each 1.02 milliseconds
	
	TIMSK0|=(1<<TOIE0);// enable timer0 interrupt
	
    while (1) 
    {
		
	}
}


