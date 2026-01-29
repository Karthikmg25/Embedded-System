/*
 * LEDblink.c
 *
 * Created: 14-11-2025 12:42:34
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include<util/delay.h>

int main(void)
{
    DDRB|=(1<<DDB5);// setting PORTB5 as output
    while (1) 
    {
		PORTB|=(1<<PORTB5);// setting PORTB5 as 1
		_delay_ms(3000);   // creating a delay of 3 seconds
		PORTB &=~(1<<PORTB5);// setting PORTB5 as 0
		_delay_ms(3000);   // creating a delay of 3 seconds
		
    }
}

