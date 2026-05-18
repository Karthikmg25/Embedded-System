/*
 * PWM.c
 *
 * Created: 18-05-2026 12:23:33
 *  Author: hp
 */ 
#include <avr/io.h>


// Set Fast PWM on OC0A/PD6
void Init_PWM()
{
	// Set Fast PWM on TIMER0 : WGM0 2:0 = 011
	TCCR0B &= ~(1<< WGM02);
	TCCR0A |= (1<< WGM01)|(1<< WGM00);
	
	// set non-inverting output on PD6: COM01:0 = 10
	TCCR0A |=(1<<COM0A1);
	TCCR0A &=~(1<< COM0A0);
	
	// set PD6 as output pin
	DDRD |= (1<< DDD6);
	
	// set prescaler 64 : CS02:0=011
	TCCR0B &=~(1<< CS02);
	TCCR0B |= (1<< CS01)|(1<< CS00);
	
}
