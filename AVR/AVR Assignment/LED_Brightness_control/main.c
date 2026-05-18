/*
 * LED_Brightness_control.c
 *
 * Created: 16-05-2026 18:14:36
 * Author : hp
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void Init_FastPWM()
{
	// Set Fast PWM mode (WGM01 = 1, WGM00 = 1)

	TCCR0A |= (1<< WGM01) | (1<< WGM00);
	
	// Set non-inverting output on OC0A(PD6): COM0A 1:0= 10
	TCCR0A |= (1<< COM0A1);
	TCCR0A &= ~(1<< COM0A0);
	
	// Set duty cycle 25%
	// compare value = duty cycle*(top)/ 100
	//               = 25 * 255 / 100
	//               = 63
	OCR0A = 63;
	
	// Set prescaler 64: CS0 2:0= 011
	TCCR0B &= ~(1<< CS02);
	TCCR0B |= (1<< CS01)|(1<< CS00);
}

// Use an array to store compare values
// Duty cycles : 25%, 50%, 75%, 100%, OFF
uint8_t pwm_level[]= {63, 127, 191, 255, 0}, i=0;
	
// Update duty cycle when button press occurs
// Continue same pattern after full cycle
void Update_brightness()
{
    i++;
	
	if(i>=5)
	{
		i =0;
	}
	OCR0A = pwm_level[i];
}
void Init_pins()
{
	// Set PD6 as output pin for PWM
	DDRD |= (1<< DDD6);
	
	// Set PD2 as input for push button and enable pull-up resistor
	DDRD &= ~(1<< DDD2);
	PORTD |= (1<< PORTD2); 
}


int main(void)
{
    Init_FastPWM();
	
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
		// Read button state on PD2 (0 = pressed due to pull-up)
		current_state = (PIND >>2)&1;
		
		// If Edge detected
		if(prev_state==1 && current_state==0)
		{
			// wait for bounce to settle
			_delay_ms(20);
			
			// re-check button state
			if(!(PIND&(1<<PIND2)))
			{
				Update_brightness();
			}
			
		}
		// Reset state
		prev_state= current_state;
    }
}

