/*
 * servo_motor_PWM.c
 *
 * Created: 13-12-2025 13:33:05
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void PWM_setup()
{
	TCCR1A|=(1<<WGM10)|(1<<WGM11);
	TCCR1B|=(1<<WGM12)|(1<<WGM13);// set mode 15 in timer1- fast PWM, TOP in OCR1A
	
	OCR1A=39999;// set top value for frequency 50 Hz
	
	TCCR1A|=(1<<COM1B1);// set non inverting PWM on OC1B- PB2
	
	TCCR1B|=(1<<CS11);// set pre scalar 8 , PWM frequency= 16Mh/(40000*8)
	
	DDRB|=(1<<DDB2);// set PB2 as OUTPUT
}
int main(void)
{
    PWM_setup();
	
	
    while (1) 
    {
		OCR1B= 5000; // 2.5 ms pulse ? servo max angle - duty cycle 12.5%
		_delay_ms(1000);
		
		OCR1B= 3000; // 1.5 ms pulse ? servo center angle - duty cycle 7.5%
		_delay_ms(1000);
		
		OCR1B= 1000; // 0.5 ms pulse ? servo min angle - duty cycle 2.5%
		_delay_ms(1000);
		
		OCR1B= 3000; // 1.5 ms pulse ? servo center angle - duty cycle 7.5%
		_delay_ms(1000);
		
    }
}

