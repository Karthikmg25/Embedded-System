/*
 * blinking.c
 *
 * Created: 15-11-2025 20:28:24
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include<util/delay.h>

int main(void)
{
    DDRB|=(1<<DDB5);// set PB5 as OUTPUT
    while (1) 
    {
		PORTB|=(1<<PORTB5);// set PB5 as HIGH
		_delay_ms(500);
		PORTB&=~(1<<PORTB5);// set PB5 as LOW
		_delay_ms(500);
    }
}

