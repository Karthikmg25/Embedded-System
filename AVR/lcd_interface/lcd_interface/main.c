/*
 * lcd_interface.c
 *
 * Created: 17-12-2025 18:53:41
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include <util/delay.h>

volatile uint16_t value;// global volatile variable for ADC value

void ADC_init()
{
	ADMUX =(1<<REFS0);// set VCC as reference voltage
	
	ADMUX=(ADMUX&0xF0) | 0;// channel 0 -ADC0
	
	ADCSRA|=(1<<ADEN);// enable ADC
	
	ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);// set prescalar 128 for 125KHz frequency; ADC frequency should be 50-200 KHz
	
	ADCSRA|=(1<<ADIE);// enable ADC interrupt
	
	 DIDR0 |= (1<<ADC0D); // disable digital input
	
	sei();// enable global interrupt
	
	ADCSRA|=(1<<ADSC);// start first conversion
	
}
ISR(ADC_vect)
{
	value=ADC;// store value
	
	ADCSRA|=(1<<ADSC);// start next conversion
	
}

int main()
{
	DDRD=0xF0;// set PD7-PD4 as outputs
	DDRB|=(1<<EN)|(1<<RS);
	DDRC&=(1<<DDC0);// set PC0 as input
	
	lcd_init();
	
	lcd_cmnd(0x01);// clear display
	
	_delay_ms(2);
	
	uint16_t count=0;
	while(1)
	{
		lcd_goto(0,0);// initial position of first line
		
		lcd_num(count);// display ADC values for 500 ms
		
		_delay_ms(500);
		
		count++;
	}
}
