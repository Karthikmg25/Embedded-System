/*
 * traffic_LED.c
 *
 * Created: 18-11-2025 21:42:25
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include<util/delay.h>
void red()
{
	PORTD|=(1<<PORTD1);// set PD1 as HIGH
	PORTD&=~(1<<PORTD2);// set PD2 as LOW
	PORTD&=~(1<<PORTD3);// set PD3 as LOW
}
void orange()
{
	PORTD&=~(1<<PORTD1);// set PD1 as LOW
	PORTD|=(1<<PORTD2);// set PD2 as HIGH
	PORTD&=~(1<<PORTD3);// set PD3 as LOW
	
}
void green()
{
	PORTD&=~(1<<PORTD1);// set PD1 as LOW
    PORTD&=~(1<<PORTD2);// set PD2 as LOW
    PORTD|=(1<<PORTD3);// set PD3 as HIGH
}
int main(void)
{
    DDRD|=(1<<DDD1)|(1<<DDD2)|(1<<DDD3);// setting PD1,PD2,PD3 as OUTPUT
    while (1) 
    {
		red(); // PD1 HIGH and others LOW for 3 seconds
		_delay_ms(3000);
		orange();//PD2 HIGH and others LOW for 3 seconds
		_delay_ms(3000);
		green();//PD3 HIGH and others LOW for 3 seconds
		_delay_ms(3000);
		
    }
}

