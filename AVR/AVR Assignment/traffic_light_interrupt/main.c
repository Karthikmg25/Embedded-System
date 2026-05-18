/*
 * traffic_light_interrupt.c
 *
 * Created: 15-05-2026 20:28:32
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include "traffic_control.h"

int main(void)
{	
	// Initialize TIMER0 for custom delay:
	Init_timer();
	
	// Initialize GPIO pins:
	Init_pins();
	
	// Initialize external interrupt
	Init_interrupt();
    
    while (1) 
    {
		// Turn ON green LED for 10 seconds:
		ON_GREEN_LED();
		delay_s(10);
		
		// Check flag  after each states
		check_flag();
		
		// Turn ON orange LED for 3 seconds:
		ON_ORANGE_LED();
		delay_s(3);;
		
		// Check flag after each states
		check_flag();
		
		// Turn ON red LED for 10 seconds:
		ON_RED_LED();
		delay_s(10);
		
		// Check flag after each states
		check_flag();
		
    }
}

