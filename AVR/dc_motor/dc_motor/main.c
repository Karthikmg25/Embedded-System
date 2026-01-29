/*
 * dc_motor.c
 *
 * Created: 18-11-2025 20:45:59
 * Author : hp
 */ 

#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>

void cw()
{
	PORTD|=(1<<PORTD1); // setting PD1 as HIGH and PD2 as LOW
	PORTD&=~(1<<PORTD2);
	_delay_ms(5000);
}
void ccw()
{
	PORTD&=~(1<<PORTD1); // setting PD1 as LOW and PD2 as HIGH
	PORTD|=(1<<PORTD2);
	_delay_ms(5000);
}
void stp()
{
	PORTD&=~(1<<PORTD1); // setting PD1 and PD2 as LOW
	PORTD&=~(1<<PORTD2);
	_delay_ms(3000);
}
int main(void)
{
	DDRD|=(1<<DDD1)|(1<<DDD2); // setting PD1 and PD2 as OUTPUTs
	while (1)
	{
		cw();                 // PD1=HIGH , PD2=LOW for 5 seconds
		
		
		stp();                // both LOW for 3 seconds
		
		
		ccw();                // PD1=LOW , PD2=HIGH for 5 seconds
		
		
		stp();                // both LOW for 3 seconds
		
		
	}
}

