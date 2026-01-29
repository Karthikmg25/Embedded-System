/*
 * traffic_interruption.c
 *
 * Created: 23-11-2025 12:59:52
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include<util/delay.h>
void green()
{
	PORTD|=(1<<PORTD1);//set PD1 as HIGH
	PORTD&=~((1<<PORTD2)|(1<<PORTD3));//set PD2 and PD3 as LOW
}
void orange()
{
	PORTD|=(1<<PORTD2);//set PD2 as HIGH
	PORTD&=~((1<<PORTD1)|(1<<PORTD3));//set PD1 and PD3 as LOW
}
void red()
{
	PORTD|=(1<<PORTD3);//set PD3 as HIGH
	PORTD&=~((1<<PORTD2)|(1<<PORTD1));//set PD1 and PD2 as LOW
}
void clockwise()
{
	PORTD|=(1<<PORTD5);//set PD5 as HIGH
	PORTD&=~(1<<PORTD6);//set PD6 as LOW
	
}
void anti_clockwise()
{
	PORTD|=(1<<PORTD6);//set PD6 as HIGH
	PORTD&=~(1<<PORTD5);//set PD5 as LOW
	
}
void stop()
{
	PORTD&=~((1<<PORTD5)|(1<<PORTD6));//set both PD6 and PD5 as LOW
	
}
void interruption()
{
	red();           //light red LED
	clockwise();     //rotate the motor clockwise.
	_delay_ms(10000);//for 10 seconds
	stop();          //stop motor
	
	_delay_ms(20000);//wait for 20 seconds
	
	anti_clockwise();//rotate the motor anti clockwise.
	_delay_ms(10000);//for 10 seconds
	stop();          //stop motor
	
}
int main(void)
{
	DDRD|=(1<<DDD1)|(1<<DDD2)|(1<<DDD3)|(1<<DDD5)|(1<<DDD6);//set PD1,PD2,PD3,PD5,PD6 as OUTPUTs
    DDRB&=~(1<<DDB5);//set PB5 as INPUT
    PORTB|=(1<<PORTB5);//enable pull up on PB5
    while (1) 
    {

			green();               // light green LED for 4 seconds
			
			                       // instead of using continuous delays,break it using loops
			for(int i=0;i<4000;i++)// use loops so that interrupt checking can be done during every milliseconds,
			{                      // else button press will be ignored and won't be checked because of constant delay
				_delay_ms(1);
				if(!(PINB&(1<<PINB5)))// if PB5(button) pressed,
				{
					interruption();   // interruption happens
					break;           // break from loop after interruption to avoid further delays
				}
			}
				
			orange();// light orange LED for 4 seconds
			
			for(int i=0;i<4000;i++)
			{                      
				_delay_ms(1);
				if(!(PINB&(1<<PINB5)))// if PB5(button) pressed,
				{
					interruption();
					break;
				}
			}
			red();// light red LED for 4 seconds
			
			for(int i=0;i<4000;i++)
			{
				_delay_ms(1);
				if(!(PINB&(1<<PINB5)))// if PB5(button) pressed,
				{
					interruption();
				    break;
				}
			}
				
				
		
		
		
		
		
    }
}

