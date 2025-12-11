/*
 * trafficLight_interrupts.c
 *
 * Created: 11-12-2025 20:20:09
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t flag=0;

void interrupt_setup()
{
	DDRD&=~(1<<DDD2);// set PD2-INT0 as input
	
	EICRA|=(1<<ISC01)|(1<<ISC00);// set mode as rising edge
	
	EIMSK|=(1<<INT0);// enable INT0-PD2
	
	sei();// enable global interrupt

}


ISR(INT0_vect)
{
	 
	flag=1;// set flag when interrupt occurs
}

void green()
{
	PORTD|=(1<<PORTD1);// set PD1 HIGH, others LOW
	PORTD&=~((1<<PORTD4)|(1<<PORTD3));
}
void orange()
{
	PORTD|=(1<<PORTD4);// set PD4 HIGH, others LOW
	PORTD&=~((1<<PORTD1)|(1<<PORTD3));
}
void red()
{
	PORTD|=(1<<PORTD3);// set PD3 HIGH, others LOW
	PORTD&=~((1<<PORTD4)|(1<<PORTD1));
}
void clkwise()
{
	PORTD|=(1<<PORTD5);// set PD5 HIGH, other LOW
	PORTD&=~((1<<PORTD6));
}
void anti_clkwise()
{
	PORTD|=(1<<PORTD6);// set PD6 HIGH, other LOW
	PORTD&=~((1<<PORTD5));
}
void stop()
{
	PORTD&=~(1<<PORTD6);// set PD6 and PD5 as LOW
	PORTD&=~((1<<PORTD5));
}

void interruption()
{
	red();// red LED on
	clkwise();// motor clock wise for 10 seconds
	_delay_ms(10000);
	stop();
	
	_delay_ms(20000);// wait for 20 seconds
	
	anti_clkwise();// motor ant  clock wise for 10 seconds
	_delay_ms(10000);
	stop();
	
	green();// turn red back to green
}


int main(void)
{
    DDRD|=(1<<DDD1)|(1<<DDD4)|(1<<DDD3)|(1<<DDD5)|(1<<DDD6);// set them as outputs
	
	interrupt_setup(); 
	
    while (1) 
    {
		green();// green for 3 seconds
		for(int i=0;i<3000;i++)
		{
			_delay_ms(1);
			if(flag==1)// if interrupt happens, break
			break;
		}
		
		orange();// orange for 3 seconds
		for(int i=0;i<3000;i++)
		{
			_delay_ms(1);
			if(flag==1)// if interrupt happens, break
			break;
		}
		
		red();// red for 3 seconds
		for(int i=0;i<3000;i++)
		{
			_delay_ms(1);
			if(flag==1)// if interrupt happens, break
			break;
		}
		
		if(flag==1)
		{
			interruption();
			
			flag=0;
		}
			
    }
}

