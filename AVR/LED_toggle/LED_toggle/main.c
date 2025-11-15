/*
 * LED_toggle.c
 *
 * Created: 15-11-2025 21:53:11
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
		PORTB^=(1<<PORTB5); // toggle PB5
		_delay_ms(300);
    }
}

