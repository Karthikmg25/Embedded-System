/*
 * GPIO_input_LED.c
 *
 * Created: 20-11-2025 20:32:02
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>


int main(void)
{
   DDRB|=(1<<DDB5);    //set PB5 as OUTPUT
   DDRD&=~(1<<DDD2);  //set PD2 as INPUT
    while (1) 
    {
		if(PIND&(1<<PIND2))// if PD2 is HIGH,
		   {
			PORTB|=(1<<PORTB5);// set PB5 as HIGH
			}
			else
			{
				PORTB&=~(1<<PORTB5);// set PB5 as LOW
			}
    }
}

