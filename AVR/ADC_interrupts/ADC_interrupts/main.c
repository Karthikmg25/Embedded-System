/*
 * ADC_interrupts.c
 *
 * Created: 09-12-2025 13:43:00
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t value;

ISR(ADC_vect)// happens after ADC conversion
{
	value=ADC;// read the resulting value
	
	ADCSRA|=(1<<ADSC);// start the next conversion
}

void ADC_setup()
{
	ADMUX|=(1<<REFS0);// select reference voltage as 5v
	
	ADMUX=(ADMUX & 0xF0) | 0;// select ADC0- safe to clear lower 4 bits of register before setting the bit
	
	ADCSRA|=(1<<ADEN);// enable ADC
	
	ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);// set prescalar 128, for 125KHz frequency
	                                         // ADC needs a frequency in the range 50-200 KHz	
											
   	ADCSRA|=(1<<ADSC);// start first conversion . After that, interrupt will get triggered 
	                  // and in the ISR, we start the next conversion 											 
											 
    ADCSRA|=(1<<ADIE);// enable ADC interrupt
	
	sei();// enable global interrupt											 										 
}

int main(void)
{
    DDRB|=(1<<DDB5)|(1<<DDB4);// set PB5 and PB4 as output
	
	ADC_setup();
    while (1) 
    {
		
		if(value>921)// ON RED led after 4.5v
		{
			PORTB|=(1<<PORTB5);
		}
		else
		if(value>102)// ON GREEN led after 0.5v
		{
			PORTB|=(1<<PORTB4);
		}
		else// both OFF when voltage below 0.5
		 {PORTB&=~((1<<PORTB5)|(1<<PORTB4));}
		
		
    }
}

