/*
 * timer0_pwmfast_varying_brightness.c
 *
 * Created: 29-11-2025 21:32:46
 * Author : hp
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include<util/delay.h>
void fast_pwm()
{
	TCCR0A|=(1<<WGM00)|(1<<WGM01);// set fast PWM mode
	
	
	
	TCCR0A|=(1<<COM0B1);
	TCCR0A&=~(1<<COM0B0);// set non inverting PWM on output pin OC0b- PD5
	
	TCCR0B&=~(1<<CS02);// normal clock frequency
	TCCR0B|=((1<<CS01)|(1<<CS00));//no pre scaling, PWM frequency= 16Mhz/256 =62.5Khz
	
	
}


int main(void)
{
	DDRD|=(1<<DDD5);//set PD5 as output
	fast_pwm();
	while (1)
	{
		for(int i=0;i<256;i++)
		{
			OCR0B=i;//vary duty cycle from 0 to 255
			_delay_ms(70);//delay for visibility
		}
		for(unsigned int i=255;i>=0;i--)
		{
			OCR0B=i;//vary duty cycle from 255 to 0
			_delay_ms(70);
			
			
		}
	}
}



