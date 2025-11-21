/*
 * dcmotor_INPUT.c
 *
 * Created: 20-11-2025 21:14:38
 * Author : hp
 */ 
#define F_CPU16000000 UL
#include <avr/io.h>


int main(void)
{
   DDRD|=(1<<DDD1)|(1<<DDD2); //set PD1 and PD2 as output
   DDRB&=~((1<<DDB5)|(1<<DDB4));// set PB4 and PB5 as inputs
   
    while (1) 
    {
		if(PINB&(1<<PINB5)&&!(PINB&(1<<PINB4)))//if PB5 is HIGH and PB4 is LOW
		{
			PORTD|=(1<<PORTD1);//PD1 becomes HIGH and PD2 becomes LOW
			PORTD&=~(1<<PORTD2);
		}
		else
		if(PINB&(1<<PINB4)&&!(PINB&(1<<PINB5)))//if PB4 is HIGH and PB5 is LOW
		{
			PORTD|=(1<<PORTD2);//PD1 becomes LOW and PD2 becomes HIGH
			PORTD&=~(1<<PORTD1);
		}
		else
		if(!(PINB&(1<<PINB4))&&!(PINB&(1<<PINB5)))//if PB5 is LOW and PB4 is LOW
		{
			PORTD&=~(1<<PORTD2);//PD1 becomes LOW and PD2 also becomes LOW
			PORTD&=~(1<<PORTD1);
		}
			
    }
}

