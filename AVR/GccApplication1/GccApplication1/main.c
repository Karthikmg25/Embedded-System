/*
 * GccApplication1.c
 *
 * Created: 13-11-2025 21:02:14
 * Author : hp
 */ 

#define F_CPU 16000000UL // frequency setting to 16 Mhz
#include <avr/io.h>
#include<util/delay.h>

int main(void)
{
   DDRD=0b00000100; // setting DDD2(portD 2) as output
    while (1) 
    {
		PORTD=0b00000100; // set PD2 as 1
		_delay_ms(1000);
		PORTD=0b00000000; // set PD2 as 0
		_delay_ms(1000);
    }
}

