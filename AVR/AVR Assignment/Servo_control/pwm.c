/*
 * pwm.c
 *
 * Created: 17-05-2026 11:15:42
 *  Author: hp
 */ 
#include <avr/io.h>

// Use Timer1 to generate PWM signal of frequency 50Hz (20000us time period)
// Set top+1= 20000, prescaler as 8
// -T = (2 * N * (TOP+1)) / F_CPU = 2 * 8 * 20000 /F_CPU = 20000us
// Update the pulse width of the signal to control servo angle
// - For 0 degree angle   -> set 1000us as pulse width
// - For 90 degree angle  -> set 1500us as pulse width
// - For 180 degree angle -> set 2000us as pulse width

void Init_PWM()
{
	// Set mode10 (Phase correct PWM with top= ICR1):
	TCCR1B |= (1<< WGM13);
	TCCR1B &=~(1<< WGM12);
	TCCR1A |= (1<< WGM11);
	TCCR1A &=~(1<< WGM10);
	
	// Set non-inverting output on OC1A (PB1):
	TCCR1A |= (1<< COM1A1);
	TCCR1A &= ~(1<< COM1A0);
	
	// Set PB1 as output pin:
	DDRB|= (1<< DDB1);
	
	// Set top value: 19999 (top+1= 20000)
	ICR1 = 19999;
	
	// Set pre scaler 8: CS2:0 = 010
	TCCR1B &=~((1<< CS12)| (1<< CS10));
	TCCR1B |= (1<< CS11);
	
}

// Set compare value on OCR1A to generate required pulse width
// Total timer counts in one PWM cycle = 2 * TOP = 40000
// Each timer tick duration = 1 / (F_CPU / prescaler) = 1 / (16MHz / 8) = 0.5us
// OCR1A (compare value)    = pulse width (in us) / tick duration (0.5 us) 
//                          = pulse width * 2
void Set_pulse_width(uint16_t pulse_width)
{

	uint16_t comp_value= (pulse_width*2);
	OCR1A = comp_value;
}