/*
 * 4_bit_binary_count.c
 *
 * Created: 15-05-2026 10:40:02
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Set PD2 as input for button : Enable pull up resistor
// Set PB0,PB1,PB2,PB3 as output for LEDs
// Set LEDs as LOW
void Init_pins()
{
	DDRD &= ~(1<< DDD2);
	PORTD |=(1<< PORTD2);
	
	DDRB |= (1<< DDB0)|(1<< DDB1)|(1<< DDB2)|(1<< DDB3);
	PORTB&= ~((1<< PORTB0)|(1<< PORTB1)|(1<< PORTB2)|(1<< PORTB3));
}


// Update the count when button press occurs
// Set LEDs based on count: LEDs are connected to lower nibble of PORTB(0-3)
// Reset the count when count reaches 15
uint8_t count=0;
void Update_count()
{
	
	count++;
	if(count > 15)
	{
		count = 0;
	}
	PORTB = count & 0x0F;
}

int main(void)
{
	// Initialize pins
	Init_pins();
    
	// Detect button press inside loop
	// Use de-bouncing logic to prevent multiple triggering
	// Logic:
	// - Falling edge detection
	// - Delay for stabilization
	// - Confirm button is still pressed for safety
	
	// Variables for edge detection 
	uint8_t prev_state=1, current_state;
	
    while (1) 
    {
			current_state = (PIND >>2)&1;
			// If Edge detected
			if(prev_state==1 && current_state==0)
			{
			   // wait for bounce to settle
			   _delay_ms(20);
			
			  // re-check button state
			  if(!(PIND&(1<<PIND2)))
			  {
				Update_count();
			  }
			
			}
			// Reset state
			prev_state= current_state;
			
		
    }
}


