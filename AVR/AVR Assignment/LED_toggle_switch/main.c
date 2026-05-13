/*
 * LED_toggle_switch.c
 *
 * Created: 13-05-2026 13:32:40
 * Author : hp
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Use Macros for readability:
#define  GREEN_LED       PORTD5
#define  RED_LED         PORTD6

void Toggle_LEDs()
{
	// Toggle Green LED:
	PORTD ^= (1<< GREEN_LED);
	// Toggle Red LED:
	PORTD ^= (1<< RED_LED);
}

void LED_Init()
{
	// Initialize both LEDs to different states:
	
	PORTD |= (1<< GREEN_LED);// ON
	PORTD &= ~(1<< RED_LED);// OFF
}

int main(void)
{
	// Set PD4 as Input for push button:
	DDRD &= ~(1<< DDD4);
	
	// Enable pull-up resistor to avoid floating condition:
	// Default button state : HIGH
	// When button pressed  : LOW
	PORTD |= (1<< PORTD4);
	
    // Set PD5 as Output for Green LED:
	DDRD |= (1<< DDD5);
	
	// Set PD6 as Output for Red LED:
	DDRD |= (1<< DDD6);
	
	// Initialize LED states:
	LED_Init();
	
	// De-bouncing is needed, else single button press triggers multiple toggles:
	// Use Edge detection + delay:
	
	// Variables to detect edge:
	uint8_t previous_state=1, current_state;
	
    while (1) 
    {
		// Read current state of button inside the loop:
		current_state= (PIND >> 4) & 1;
		
		// If Button press happens: State changes-
		if(previous_state==1 && current_state==0)
		{
		    // Wait for button state to stabilize:
		    _delay_ms(20);
		
		    // Toggle LEDs
		    Toggle_LEDs();
		
		}
		
		// Update previous state:
		previous_state= current_state;
	
    }
}

