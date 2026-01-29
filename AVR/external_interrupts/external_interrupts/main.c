/*
 * external_interrupts.c
 *
 * Created: 06-12-2025 20:26:09
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect)// when interrupt occurs
{
	PORTB|=(1<<PORTB5); // set LED high for 3 second
	PORTB&=~(1<<PORTB4); // set LED on PB4 OFF
	_delay_ms(3000);
}


int main(void)
{
    DDRB|=(1<<DDB5)|(1<<PORTB4);//set PB5 and PB4 as output
	DDRD&=~(1<<DDD2);// set INT0 ,PD2 as INPUT
	
	EICRA|=(1<<ISC00)|(1<<ISC01);// set mode for rising edge
	
	EIMSK|=(1<<INT0);// enable INT0 external interrupt
	
	sei();// enable global interrupt
    while (1) 
    {                       // default condition
		
		PORTB&=~(1<<PORTB5);// LED on PB5 is normally OFF
		
		PORTB^=(1<<PORTB4);// toggle LED on PB4 on each half seconds
		_delay_ms(500);
    }
}

