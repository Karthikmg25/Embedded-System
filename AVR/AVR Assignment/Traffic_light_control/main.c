/*
 * Traffic_light_control.c
 *
 * Created: 15-05-2026 18:02:37
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>

// Use Macros for readability:
#define GREEN_LED  PORTB0
#define ORANGE_LED PORTB1
#define RED_LED    PORTB2

// Create a custom delay function using TIMER0
// - Use CTC mode
// - Timer frequency = F_CPU/ ((Top+1) * prescale factor)
// - Set a Timer frequency of 1KHz, creating compare match every millisecond
// - Set 250 as top+1 value and select 64 as pre-scalar
// - Use output compare flag to create custom delay function

void Init_timer()
{
	// Set CTC Mode: WGM01:0 - 1 0
	TCCR0A |= (1<< WGM01);
	TCCR0A &=~(1<<WGM00);
	
	// Start counting from 0
	TCNT0 = 0;
	
	// Set Top value in OCR0A register
	OCR0A = 249;
	
	// Set pre-scalar as 64: CS02:0- 0 1 1
	TCCR0B |= (1<<CS01)|(1<<CS00);
	TCCR0B &= ~(1<<CS02);
}
void delay_s(uint8_t sec)
{	
	// Create delay in seconds 
	for(uint16_t i=0;i<(sec*1000);i++)
	{
		// Wait for compare match event : 1ms delay
		while(!(TIFR0 & (1<< OCF0A)));
		
		// Clear flag by writing 1
		TIFR0 |= (1<< OCF0A);
	}
}
void Init_pins()
{
	// Set pins as output for LEDs
	DDRB |= (1<< DDB0)|(1<< DDB1)|(1<< DDB2);
	
	// Set LEDs as LOW initially
	PORTB &= ~((1<< PORTB0)|(1<<PORTB1)|(1<<PORTB2));
}
void ON_GREEN_LED()
{
	// Turn ON green LED and turn OFF others:
	PORTB|= (1<<GREEN_LED);
	PORTB &= ~((1<<ORANGE_LED)|(1<< RED_LED));
}
void ON_ORANGE_LED()
{
	// Turn ON orange LED and turn OFF others:
	PORTB |=  (1<<ORANGE_LED);
	PORTB &= ~((1<<GREEN_LED)|(1<< RED_LED));
}
void ON_RED_LED()
{
	// Turn ON red LED and turn OFF others:
	PORTB |=  (1<< RED_LED);
	PORTB &= ~((1<<ORANGE_LED)|(1<<GREEN_LED));
}

int main(void)
{
	// Initialize TIMER0 for delay:
	Init_timer();
	
	// Initialize GPIO pins:
	Init_pins();
	
    while (1) 
    {
		// Turn ON green LED for 10 seconds:
		ON_GREEN_LED();
		delay_s(10);
		
		// Turn ON orange LED for 3 seconds:
		ON_ORANGE_LED();
		delay_s(3);
		
		// Turn ON red LED for 10 seconds:
		ON_RED_LED();
		delay_s(10);
    }
}

