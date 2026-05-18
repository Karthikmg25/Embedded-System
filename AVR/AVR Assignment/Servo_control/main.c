/*
 * Servo_control.c
 *
 * Created: 16-05-2026 21:10:05
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"
#include "adc.h"

int main(void)
{
	// Initialize timer1 PWM for servo control
    Init_PWM();
	
	// Initialize ADC for reading potentiometer
	Init_ADC();
	
	uint16_t ADC_value, pulse_width;
	
    while (1) 
    {
		// Read potentiometer value
		ADC_value= Get_ADC_value();
		
		// Convert ADC value to servo pulse width
		pulse_width = Map_ADC_To_PulseWidth(ADC_value);
		
		// Update pulse width to control servo angle
		Set_pulse_width(pulse_width);
		
		_delay_ms(5);
    }
}
