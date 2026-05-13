/*
 * LED_blink.c
 *
 * Created: 13-05-2026 11:49:11
 * Author : hp
 */ 

// Define clock frequency
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void LED_ON()
{
	PORTD |= (1<<PORTD5);
}
void LED_OFF()
{
	PORTD &= ~(1<<PORTD5);
}
int main(void)
{
	// Set PD5 as Output pin for LED Control:
	DDRD |= (1<<DDD5);
	
    
    while (1) 
    {
		// Turn ON LED for 3 seconds using blocking delay:
		LED_ON();
		_delay_ms(3000);
		
		// Turn OFF LED for 5 seconds:
		LED_OFF();
		_delay_ms(5000);
    }
}

