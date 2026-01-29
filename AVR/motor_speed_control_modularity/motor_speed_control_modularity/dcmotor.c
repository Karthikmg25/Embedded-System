/*
 * dcmotor.c
 *
 * Created: 03-12-2025 17:16:55
 *  Author: hp
 */ 
#include <avr/io.h>
#include "dcmotor.h"

void motor_init()
{
	TCCR0A|=(1<<WGM00)|(1<<WGM01);// set up fast PWM mode
	
	TCCR0A|=(1<<COM0B1);
	TCCR0A&=~(1<<COM0B0);// set up non inverting PWM on pin OC0B ,PD5
	
	TCCR0B|=(1<<CS01);
	TCCR0B&=~((1<<CS00)|(1<<CS02));// set up pre scalar 8, PWM frequency= 16MHz/8*256 = 7.8 KHz
	                               //  desired range of frequency for a DC motor- 7-15 KHz
	DDRD|=(1<<DDD5);// set up PD5 as output							   
								   	
}
void speed_control(uint8_t speed)
{
	OCR0B=speed;// set up duty cycle
}
