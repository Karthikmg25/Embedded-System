/*
 * creating_melodies_PWM.c
 *
 * Created: 15-12-2025 12:49:28
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#define SA    954  // top values for each notes
#define RI1   896
#define RI2   850
#define GA1   796
#define GA2   765
#define MA    716
#define PA    638
#define DA1   597
#define DA2   573
#define NI1   531
#define NI2   509
#define SAH   478   // upper Sa
#define mute  10


void timer0_setup()
{
	TCCR0A|=(1<WGM01);// set up CTC mode in timer0
	
	TCNT0=0;
	OCR0A=249;// counting from 0-249, making 250 counts
	
	TCCR0B|=(1<<CS00)|(1<<CS01);// set prescalar 64 to make 1 count= 4 microseconds
	                            // thus, 250 counts as 1 millisecond
}
void delay_ms(uint16_t time)
{
	while(time--)
	{
		while(!(TIFR0&(1<<OCF0A)));// wait for 1ms
		TIFR0|=(1<<OCF0A);// manually clear flag
	}
	
}

void PWM_setup()
{
	TCCR1A|=(1<<WGM10)|(1<<WGM11);
	TCCR1B|=(1<<WGM12)|(1<<WGM13);// set mode15 in timer1, fast PWM and to set set TOP value in OCR1A
	
	TCCR1A|=(1<<COM1B1);// set non inverting PWM on pin OC1B- PB2
	
	TCCR1B|=(1<<CS11)|(1<<CS10);// set prescalar 64
	
	DDRB|=(1<<DDB2);// set PB2 as output
}

int main(void)
{
	uint16_t melody_1[] = {SA,mute,PA,mute,PA,MA,GA2,mute,SA,mute,PA,mute,PA,DA2,mute,MA,GA2,mute   ,SA ,mute, RI2,mute, SA,mute, SA,mute, RI2,mute, SA,mute , SA,mute, RI2,mute, SA,mute,SA,956,958};
		                   
	uint16_t duration_1[] = {300,30,400,10,400,400,700,80,400,30,400,10,150,150,20,400,700,50     ,400,20,400,20,400, 30 ,400,20,400,20,400, 30 ,400,20,400,20,300,75,700,200,100};
		                    
	

    timer0_setup();// set up the timer for delay
	
	PWM_setup();// set up PWM for signal generation
	
	
    while (1) 
    {
		
		for(int i=0;i<39;i++)
		{
			OCR1A=(melody_1[i]/2);// set top value
			OCR1B= OCR1A/2;// set 50% duty cycle
			
			delay_ms((uint16_t)(duration_1[i]*1.1));// set duration for each notes
			
		}
		OCR1B= 0;// mute for 2 seconds after each loop
		delay_ms(2000);
		
    }
}

