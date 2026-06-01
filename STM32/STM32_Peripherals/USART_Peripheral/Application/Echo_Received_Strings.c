/*
 * Echo_Received_Strings.c
 *
 *  Created on: Jun 1, 2026
 *      Author: hp
 */



#include <stdint.h>
#include "STM32F401RE.h"
#include "STM32F401RE_USART_Driver.h"
#include "USART_Baremetal.h"

int App_main(void)
{
	// Set PA3 and PA2 as RX ang TX
	USART2_GPIO_Configuration();

	// Set USART Configurations
	USART_Handle_t handle;
	handle.pUSARTx = USART2;
	handle.USART_Config.USART_Baud = 9600;
	handle.USART_Config.USART_WordLength =  USART_WORDLEN_8BITS;
	handle.USART_Config.USART_OverSampling = USART_OVERSAMPLING_16;
	handle.USART_Config.USART_Mode = USART_MODE_RXTX ;

	USART_Init(&handle);

	// Enable USART
	USART_PeripheralControl(USART2, ENABLE);

	// Set up a buffer to transmit
	uint8_t buffer[] = "\n\rUSART Driver Application";

	// transmit a buffer
    USART_Transmit_Buffer(USART2, buffer, 26);

    char string[40];
	 while(1)
	 {
		// Transmit back received Strings in loop

		 USART_Receive_String(USART2, string, 40);

		 USART_Transmit_String(USART2, string);

		 USART_Transmit(USART2, '\n');


		 for(volatile uint32_t i=0;i< 200000;i++);
	 }



}



