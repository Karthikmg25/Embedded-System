/*
 * delay_ms.c
 *
 * Created: 03-12-2025 14:34:33
 *  Author: hp
 */ 
#include <avr/io.h>
#include "delay_ms.h"

void pin_setup()
{
	DDRB|=(1<<DDB6);// set up PB6 as OUTPUT
}
void timer0_setup()
{
	TCNT0=0;// count from 0
	
	TCCR0A|=(1<<WGM01);
	TCCR0A&=~(1<<WGM00);// set up CTC mode
	TCCR0B &= ~(1 << WGM02);

	
	OCR0A=249;// count up to 249, making up 250 counts before overflow
	
	TCCR0B|=(1<<CS00)|(1<<CS01);// set up pre scaling as 64, means 4 microseconds for 1 count
}
void delay_ms(uint16_t ms)
{
	for(int i=0;i<ms;i++)           //creates delay of T milliseconds
	{
		while(!(TIFR0&(1<<OCF0A)));// counts 250 times, creates delay of 250* 4 = 1 millisecond
		
		TIFR0|=(1<<OCF0A);// clear the flag 
		
	}
	
}
void toggle_LED()
{
	PORTB^=(1<<PORTB6);// toggle 
}