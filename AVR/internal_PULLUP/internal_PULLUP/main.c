/*
 * internal_PULLUP.c
 *
 * Created: 22-11-2025 20:57:34
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>


int main(void)
{
   DDRB&=~(1<<DDB6);//set PB6 as INPUT
   DDRC|=(1<<DDC3);//set PC3 as OUTPUT
   PORTB|=(1<<PORTB6);// pull up PB6 internally 
    while (1) 
	
    {
		if(!(PINB&(1<<PINB6)))//if PB6 is LOW,
		PORTC|=(1<<PORTC3);//set the LED(PC3) as HIGH
		else
		PORTC&=~(1<<PORTC3);//set the LED(PC3) as LOW
		
    }
}

