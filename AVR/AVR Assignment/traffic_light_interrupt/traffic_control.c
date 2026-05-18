/*
 * traffic_control.c
 *
 * Created: 15-05-2026 20:29:14
 *  Author: hp
 */ 

#include <avr/io.h>
#include "traffic_control.h"

// Create a custom delay function using TIMER0
// - Use CTC mode
// - Timer frequency = F_CPU/ ((Top+1) * prescale factor)
// - Set a Timer frequency of 1KHz, creating compare match every millisecond
// - Set 250 as top+1 value and select 64 as pre-scalar
// - Use output compare flag to create custom delay function

volatile uint8_t flag=0;

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

// Initialize GPIO pins:
void Init_pins()
{
	// Set INT0-PD2 as input pin for external interrupt:
	DDRD &= ~(1<< DDD2);
	// Enable pull up to detect falling edge
	PORTD |= (1<< PORTD2);
	
	// Set pins as output for LEDs
	DDRB |= (1<< DDB0)|(1<< DDB1)|(1<< DDB2);
	
	// Set LEDs as LOW initially
	PORTB &= ~((1<< PORTB0)|(1<<PORTB1)|(1<<PORTB2));
}

// functions for LED control

void ON_GREEN_LED()
{
	// Turn ON green LED 
	// Turn OFF others:
	PORTB|= (1<<GREEN_LED);
	PORTB &= ~((1<<ORANGE_LED)|(1<< RED_LED));
	
}
void ON_ORANGE_LED()
{
	// Turn ON orange LED 
	// Turn OFF others:
	PORTB |=  (1<<ORANGE_LED);
	PORTB &= ~((1<<GREEN_LED)|(1<< RED_LED));
	
}
void ON_RED_LED()
{
	// Turn ON red LED for 
	// Turn OFF others:
	PORTB |=  (1<< RED_LED);
	PORTB &= ~((1<<ORANGE_LED)|(1<<GREEN_LED));
	
}

// Set External interrupt : EXTI0
void Init_interrupt()
{
	// Trigger during falling edge:
	EICRA |= (1<< ISC01);
	EICRA &=~(1<< ISC00);
	
	// enable interrupt:
	EIMSK |= (1<< INT0);
	
	// Enable global interrupt:
	sei();	
}

//Define ISR
ISR(INT0_vect)
{
	// Set flag inside ISR
	flag=1;
	

}
void check_flag()
{
	// If flag is set:
	if(flag== 1)
	{
		// ON green LED for 5 seconds:
		ON_GREEN_LED();
		delay_s(5);
		
		// Clear flag:
		flag=0;
	}
	
}