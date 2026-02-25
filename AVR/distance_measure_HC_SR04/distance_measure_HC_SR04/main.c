/*
 * distance_measure_HC_SR04.c
 *
 * Created: 12-01-2026 13:13:11
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "HC_SR04.h"
#include "GPIO.h"

int main(void)
{  
	                                      // Give required Pin configurations
						  
	   HC_SR04_PinConfig.pGPIOx =        GPIOD; // using GPIOD

	   HC_SR04_PinConfig.trigger_pin=    2;     // using PD2

	   HC_SR04_PinConfig.echo_pin=       3;     // using PD3

	   HC_SR04_PinConfig.output_pin=     4;     // using PD4		
	   			  
	   HC_SR04_init();

           HC_SR04_pin_init(); // trigger_pin, Output_pin as Output  and echo_pin as Input 
	   
           uart_init();

   float distance;  
    while (1) 
    {		 
		 distance= HC_SR04_distance();
			
		if(distance<15)
		{
                    set_buzzer();
		}
			else
			{
                             no_buzzer();
			 }
		
		TX_string("distance : ");
		TX_float(distance);
		
		TX_char('\n');
		_delay_ms(250);
		
    }
}

