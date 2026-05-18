/*
 * ADC_Motor_Speed_Control.c
 *
 * Created: 18-05-2026 16:55:54
 * Author : hp
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "PWM.h"
#include "adc.h"


int main(void)
{
	// Initialize PWM on pin PD6
	Init_PWM();
	
	// initialize ADC for reading potentiometer
	Init_ADC();
	
    while (1) 
    {
		// Read ADC value to control motor speed
		 Map_ADC_To_CompareValue();
		 
		 // add small delay
		 _delay_ms(10);
    }
}

