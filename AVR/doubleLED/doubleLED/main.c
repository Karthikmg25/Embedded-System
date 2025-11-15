/*
 * doubleLED.c
 *
 * Created: 15-11-2025 20:58:10
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include<util/delay.h>

int main(void)
{
	DDRD|=(1<<DDD1)|(1<<DDD2);//set PD1 and PD2 as OUTPUTs
    
    while (1) 
    {
		PORTD|=(1<<PORTD1);// set PD1 as HIGH and PD2 as LOW for 5 seconds
		PORTD&=~(1<<PORTD2);
		_delay_ms(5000);
		PORTD&=~(1<<PORTD1);  // set PD1 and PD2 as LOW for 3 seconds
		_delay_ms(3000);
		PORTD|=(1<<PORTD2);   // set PD2 as HIGH and PD1 as LOW for 5 seconds
		_delay_ms(5000);
		PORTD&=~(1<<PORTD2);  // set PD1 and PD2 as LOW for 3 seconds
		_delay_ms(3000);
    }
}

