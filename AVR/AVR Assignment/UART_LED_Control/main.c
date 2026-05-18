/*
 * UART_LED_Control.c
 *
 * Created: 18-05-2026 12:15:45
 * Author : hp
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"
#include "PWM.h"

int main(void)
{
	// Initialize UART 
	Init_UART();
	
	Init_PWM();
	
	// Buffer for receiving user-command:
	char command[50]="";
	

    while (1) 
    {
	    // Receive command from user:
	    UART_Receive_string(command, 50);
	    
	    // Update LED status based on the command:
	    Update_LED_status(command);
	    
	    
	    _delay_ms(100);
    }
}

