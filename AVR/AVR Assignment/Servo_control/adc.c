/*
 * adc.c
 *
 * Created: 17-05-2026 11:43:28
 *  Author: hp
 */ 

#include <avr/io.h>

// Initialize ADC to read potentiometer
// - Set reference voltage as Avcc
// - Select channel 0
// - Set ADC frequency as 125KHz
// - Use single-shot conversion
void Init_ADC()
{
	// Reference voltage : REFS1:0 = 01
	ADMUX &=~(1<< REFS1);
	ADMUX |=(1<< REFS0);
	
	// Select channel0: MUX2:0 = 000
	ADMUX &= ~((1<<MUX0) | (1<<MUX1) |
	           (1<<MUX2) | (1<<MUX3));
	
	// Set prescale factor 128: ADPS 2:0= 111
	// F_ADC = F_CPU / 128 = 125KHz
	ADCSRA |= (1<< ADPS2)	|(1<< ADPS1)|(1<< ADPS0);   
	
	// Enable ADC
	ADCSRA |= (1<< ADEN);
	
}

uint16_t Get_ADC_value()
{
	// Start conversion
	ADCSRA |= (1<< ADSC);
	
	// Wait till conversion finishes
	while(ADCSRA & (1<< ADSC));
	
	//Read value
	return ADC;
}

// Map ADC value to pulse width
// Required pulse width -> 1000us-2000us
// ADC value range      -> 0-1023
// pulse width = 1000 + (ADC value/1023) * 1000
uint16_t Map_ADC_To_PulseWidth(uint16_t ADC_value)
{
	// Prevent overflow : type cast to uint32_t
	uint16_t pulse_width = 1000 + ((uint32_t)ADC_value*1000)/1023;
	
	return pulse_width;
}