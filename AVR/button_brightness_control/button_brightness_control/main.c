/*
 * button_brightness_control.c
 *
 * Created: 10-12-2025 12:12:25
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t duty_cycle;

void PWM_setup()
{
	TCCR0A|=(1<<WGM01)|(1<<WGM00);// set up fast PWm
	
	TCCR0A|=(1<<COM0B1)|(1<<COM0B0);// set  inverting PWM on OC0B-PD5
	
	TCCR0B|=(1<<CS01);// set prescalar 8 for PWM frequency 7.8KHz
	
	DDRD|=(1<<DDD5);// set PD5 as output
	
	
}

void ext_interrupt_setup()
{
	EICRA|=(1<<ISC00)|(1<<ISC01);// set mode as rising edge
	
	EIMSK|=(1<<INT0);// enable INT0
	
	sei();// enable global interrupts
	
	DDRD&=~(1<<DDD2);// set PD2-INT0 as input
	
}

ISR(INT0_vect)
{
	duty_cycle+=50;// increment by 50 on each interrupt
	
}
int main(void)
{
    PWM_setup();
	
	ext_interrupt_setup();
	
	duty_cycle=0;// initialize as zero
	
    while (1) 
    {
	      if(duty_cycle>255)
		  {
			 duty_cycle=0;// revert back to 0 
		  }
	      
			
			OCR0B=duty_cycle;// vary duty cycle based on interrupt
    }
}

