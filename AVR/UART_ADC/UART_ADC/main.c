/*
 * UART_ADC.c
 *
 * Created: 28-12-2025 19:54:34
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "uart.h"

volatile uint16_t ADC_value;

void ADC_setup()
{
	ADMUX|=(1<<REFS0);// set reference voltage as VCC
	
	ADMUX=(ADMUX&0xF0) | 0;// select channel 0
	
    ADCSRA|=(1<<ADEN);// enable ADC
	
	ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);// set prescalar 128, ADC frequency = 125KHz 
	
	ADCSRA|=(1<<ADIE);// enable ADC interrupt
	
	sei();// set global interrupt
	
	ADCSRA|=(1<<ADSC);// start first conversion
	
}
ISR(ADC_vect)
{
	ADC_value=ADC;// read value 
	
	ADCSRA|=(1<<ADSC);// start next conversion
}


int main(void)
{
    ADC_setup();
	
	uart_init();
	
	float voltage;
	
	
    while (1) 
    {		
			TX_string("ADC value : ");
			TX_num(ADC_value);
			
			TX_string("\nVoltage  : ");
			voltage=(ADC_value*5.0)/1023;// convert to voltage
			TX_float(voltage);
			
			TX_string("\n\n");
			
			_delay_ms(200);
			
    }
}

