/*
 * LDR_project_ADC.c
 *
 * Created: 11-12-2025 19:02:13
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint16_t value;

void ADC_setup()
{
	ADMUX|=(1<<REFS0);// set reference voltage 5
	
	ADMUX=(ADMUX& 0xF0)|0;// select channel 0
	
	ADCSRA|=(1<<ADEN);// enable ADC
	
	ADCSRA|=(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);// set prescalar 128
	
	ADCSRA|=(1<<ADIE);// enable interrupt
	
	sei();// enable global interrupt
	
	ADCSRA|=(1<<ADSC);// start first conversion
}

ISR(ADC_vect)
{
	value=ADC;// store result
	
	ADCSRA|=(1<<ADSC);// start next conversion
	
}
int main(void)
{
    DDRB|=(1<<DDB4);// set PB4 as output
	
	ADC_setup();
    
     while (1)
     {                 // setting the limit as 0.07v (1023*0.07/5) 
	     if(value<15)  // when it gets darker, the LDR resistance goes up and voltage will reduce below 0.07
		 {
		     PORTB|=(1<<PORTB4);// then ON the LED if its dark
	     }
	     else     // means there is enough light to give voltage above 0.07
	     {
		     PORTB&=~(1<<PORTB4);//  OFF the LED
	     }
	                    // this effectively turns on LED when it gets too dark
     }
}