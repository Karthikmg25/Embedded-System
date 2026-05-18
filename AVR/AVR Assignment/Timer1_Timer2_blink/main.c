/*
 * Timer1_Timer2_blink.c
 *
 * Created: 16-05-2026 11:44:58
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <avr/io.h>

// Using TIMER1 to create LED blink every 2 seconds (Status LED)
// - Set CTC mode and enable compare match interrupt
// - Set compare value as 49999 and prescale factor as 64
// - Time period = 64 * (49999+1) / F_CPU
// - Creating an interrupt every 0.2 seconds  
// - Increment count inside ISR and blink when count=10
void Timer1_init()
{
	// Set CTC mode: WGM1 3:0 = 0100
	TCCR1B &= ~(1<< WGM13);
	TCCR1B |= (1<< WGM12);
	TCCR1A &= ~((1<< WGM11)|(1<< WGM10));
	
	// Reset count 
	TCNT1= 0;
	
	// set compare value
	OCR1A = 49999;
	
	// Set pre-scalar 64 : CS1 2:0 = 011
	TCCR1B &= ~(1<< CS12);
	TCCR1B |= (1<< CS11) | (1<< CS10);
	
	// Enable compare match interrupt 
	TIMSK1 |= (1<< OCIE1A);
	
}

// Using TIMER2 to create LED blink every 500 milliseconds (Heartbeat LED)
// - Set CTC mode and enable compare match interrupt
// - Set compare value as 249 and prescale factor as 256
// - Time period = 256 * (249+1) / F_CPU
// - Creating an interrupt every 4 milliseconds
// - Increment count inside ISR and blink when count= 125
void Timer2_init()
{
	// Set CTC mode : WGM2 2:0= 010
	TCCR2A |= (1<< WGM21);
	TCCR2A &= ~(1<< WGM20);
	TCCR2B &= ~(1<< WGM22);
	
	// Reset count 
	TCNT2 =0;
	
	// Set compare value
	OCR2A = 249;
	
	// Set pre scalar 256 : CS2 2:0 = 110
	TCCR2B |= (1<< CS22)|(1<< CS21) ;
	TCCR2B &= ~(1<< CS20);
	
	// Enable compare match interrupt 
	TIMSK2 |= (1<< OCIE2A);
	
}
// Define ISR for timer1 and timer2

volatile uint16_t count_1=0, count_2=0;
ISR(TIMER1_COMPA_vect)
{
	count_1++;
	
	if(count_1==10)
	{
		// toggle LED and reset count
		PORTB ^= (1<< PORTB1);
		
		count_1=0;
	}
}
ISR(TIMER2_COMPA_vect)
{
	count_2++;
	
	if(count_2== 125)
	{
		// toggle LED and reset count
		PORTB ^= (1<< PORTB2);
		
		count_2=0;
	}
}
void Init_pins()
{
	DDRB |= (1<< DDB1);
	DDRB |= (1<< DDB2);
}

int main(void)
{
	// initialize pins for LEDs
    Init_pins();
	// initialize timers
	Timer1_init();
	Timer2_init();
	// Enable global interrupt
	sei();
	
    while (1) 
    {
    }
}

