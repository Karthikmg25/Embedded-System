/*
 * multiple_channel_reading_ADC.c
 *
 * Created: 16-12-2025 13:34:50
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t channel=0;
volatile uint16_t value0, value1;

void ADC_setup()
{
	ADMUX|=(1<<REFS0);// select reference voltage VCC
	
	ADMUX=(ADMUX & 0xF0)|0;// start with channel 0- ADC0
	
	ADCSRA|=(1<<ADEN);// enable ADC
	
	ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);// set pre scalar 128 for frequency 125kHz
	
	ADCSRA|=(1<<ADIE);// enable interrupts
	
	sei();// enable global interrupt
	
	ADCSRA|=(1<<ADSC);// start first conversion
	
}

ISR(ADC_vect)
{
	if(channel==0)
	  value0=ADC;       // atomic reading of ADC is safer - disable interrupt, read ADC(value0=ADC),enable interrupt
	else 
	if(channel==1)
	  value1=ADC;
	  
	  channel^=1;// switch channel
	  
	  ADMUX=(ADMUX& 0xF0) | channel;
	  
	  ADCSRA|=(1<<ADSC);// start nest conversion
	
}

void PWM_setup()
{
	TCCR0A|=(1<<WGM00)|(1<<WGM01);// fast pwm mode
	
	TCCR0A|=(1<<COM0B1);
	TCCR0A|=(1<<COM0A1);// set non inverting PWM on OC0B(PD5) and OC0A(PD6)
	
	TCCR0B|=(1<<CS00);// no prescalar
	
	DDRD|=(1<<DDD5);
	DDRD|=(1<<DDD6);// set PD5-OC0B and PD6-OC0A as ouputs
}


int main(void)
{
	ADC_setup();
	
	PWM_setup();
    
    while (1) 
    {
		OCR0B=(value1>>2);
		OCR0A=(value0>>2); // set duty cycle
		                   // value1 and value0 are 10 bit, so reduce it to 8 bit range(0-255);
    }
}

