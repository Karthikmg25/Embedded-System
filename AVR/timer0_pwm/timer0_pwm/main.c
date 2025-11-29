/*
 * timer0_pwm.c
 *
 * Created: 29-11-2025 21:10:47
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
void fast_pwm()
{
	TCCR0A|=(1<<WGM00)|(1<<WGM01);// set fast PWM mode
	
	OCR0B=102;//set duty cycle of 40%, 40% of 256 is 102
	
	TCCR0A|=(1<<COM0B1);
	TCCR0A&=~(1<<COM0B0);// set non inverting PWM on output pin OC0b- PD5
	
	TCCR0B|=(1<<CS00);// normal clock frequency
	TCCR0B&=~((1<<CS01)|(1<<CS02));//no pre scaling, PWM frequency= 16Mhz/256 =62.5Khz
	
	
}


int main(void)
{
    DDRD|=(1<<DDD5);//set PD5 as output
	fast_pwm();
    while (1) 
    {
    }
}

