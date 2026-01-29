/*
 * ADC_PWM_brightness_potentiometer.c
 *
 * Created: 09-12-2025 18:00:09
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t value;



 void ADC_setup()
 {
	 ADMUX|=(1<<REFS0);// set reference voltage 5
	 
	 ADMUX= (ADMUX & 0xF0)| 0;// select ADC0-PC0
	 
	 ADCSRA|=(1<<ADEN);// enable ADC
	 
	 ADCSRA|=(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);// set prescalar 128 for 125KHz
	 
	 ADCSRA|=(1<<ADSC);// start the first conversion
	 
 }
 void ADC_interrupt()// enable interrupts
 {
	 ADCSRA|=(1<<ADIE);
	 
	 sei();
 }
 ISR(ADC_vect)// after conversion
 {
	 value= ADC;// read value
	 
	 ADCSRA|=(1<<ADSC);// start next conversion
 }
 
 void PWM_setup()
 {
	 TCCR0A|=(1<<WGM00);// set phase correct PWM
	 
	 
	 TCCR0A|=(1<<COM0B1);
	 TCCR0A&=~(1<<COM0B0);// set non inverting PWM on OC0B-PD5
	 
	 TCCR0B|=(1<<CS00);// no prescalar, PWM frequency= 16MHz/(2*256) = 31.25KHz	 
	 
	 DDRD|=(1<<DDD5);// set PD5 as output
	 
	 
 }

int main(void)
{
    ADC_setup();// set up ADC0
	
	ADC_interrupt();// enable ADC interrupts
	
	PWM_setup();// set up phase correct PWM
	
	DDRC&=~(1<<DDC0);// set PC0/ADC0 as input
	
    while (1) 
    {
		uint8_t duty_cycle= value>>2;// ADC value is 10 bit, make it 8 bit and use it as duty cycle
		
		OCR0B= duty_cycle;// set duty cycle
    }
}

