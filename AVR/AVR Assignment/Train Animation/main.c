/*
 * Train Animation.c
 *
 * Created: 14-05-2026 11:16:37
 * Author : hp
 *
 * Hardware Constraint: PB6 and PB7 are not used because they are connected to
 *                      the external crystal oscillator on Arduino-Uno.
 *
 * Implementation: Uses PB0–PB5 and PD6–PD7 to achieve 8 LED sequence.
 *
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void Init_pins()
{
	//  Configure PB0-PB5 and PD6-PD7 as output:
    DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB2);
    DDRB |= (1<<DDB3)|(1<<DDB4)|(1<<DDB5);
    DDRD |= (1<<DDD6)|(1<<DDD7);
	
	// Initialize all LED pins to LOW
	PORTB &= ~((1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|
	          (1<<PORTB3)|(1<<PORTB4)|(1<<PORTB5));
	PORTD &= ~((1<<PORTD6)|(1<<PORTD7));
}
void Train_animation()
{
	// Loop through PORTD and PORTB pins:
	// Turn ON current LED and apply delay
	// Turn OFF current LED before moving to next one

	uint8_t i;
	
	for(i=0;i<6;i++)// PB0-PB5
	{
		PORTB|= (1<< i);
		_delay_ms(50);
			
		PORTB &= ~(1<< i);
	}
	for(i=6;i<=7;i++)// PD6-PD7
	{
		PORTD|= (1<< i);
		_delay_ms(50);
		
		PORTD &= ~(1<< i);
	}
}

int main(void)
{
	
	// Initialize pins:
    Init_pins();
	
    while (1) 
    {
		// Perform continuous train animation:
		Train_animation();
    }
}

