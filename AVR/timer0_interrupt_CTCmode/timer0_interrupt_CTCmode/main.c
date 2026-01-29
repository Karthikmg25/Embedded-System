/*
 * timer0_interrupt_CTCmode.c
 *
 * Created: 05-12-2025 12:12:47
 * Author : hp
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include<avr/interrupt.h>

volatile uint16_t count=0;

ISR(TIMER0_COMPA_vect)// triggers every 1 millisecond
{
	count++;// increase on each ms
}


int main(void)
{
	TCCR0A|=(1<<WGM01);
	TCCR0A&=~(1<<WGM00);         // set up CTC mode
	
	TCCR0B|=(1<<CS00)|(1<<CS01);// set up pre scalar 64, 1 count = 64/16000000= 4 micro seconds
	// 250 counts make 1 millisecond delay
	
	TCNT0=0;                    // count from 0
	
	OCR0A=249;                  // count up to 249, making 250 counts
	
	TIMSK0|=(1<<OCIE0A);       // enable timer0 interrupt
	
	sei();                     // enable global interrupt
	
	DDRB|=(1<<DDB5);           // set up PB5 as output
	
	
	while (1)
	{
		if(count==500)         // after 1000 counts, 1 second
		{
			PORTB^=(1<<PORTB5); // toggle
			count=0;            // clear count
		}
	}
}
