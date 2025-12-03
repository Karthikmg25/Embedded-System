/*
 * delay_using_modularity.c
 *
 * Created: 03-12-2025 14:31:17
 * Author : hp
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include "delay_ms.h"


int main(void)
{
    pin_setup();// set PB6 as output
	
	timer0_setup();// set up timer 0 for creating delay
	
	while(1)
    {
		toggle_LED();// toggle repeatedly
		
		delay_ms(1000);// delay for 1000 milliseconds
    }
}

