/*
 * RTC_I2C_Interface.c
 *
 *  Created on: May 31, 2026
 *      Author: hp
 */


#include <stdint.h>
#include "STM32F401RE_I2C_Driver.h"


#define RTC_ADDR 0x68


int app_main(void)
{
	// Initialize USART for debugging
	 USART_Configuration();


	 // Reading time from a Real-Time Clock
	 // - Slave address of RTC - 0x68
	 // - RTC stores time(sec,min,hr,..) in BCD format
	 // - Address of seconds register - 0x00
	 // - minute is stored in 0x01, hour in 0x02, data in 0x03,...
	 // ---- MSB of byte in 0x00 is kept 0 for RTC ticking (Use mask 0x7F for read/write)
	 // ---- Bit_6 of hours register is used for setting 12/24 hour mode (0 - 24hr, 1- 12hr)
	 // ---- Bit_5 is used in 12-hour mode to indicate AM/PM
	 // ---- Bit_5:0 is used for storing time in 24hour mode (Use mask 0x3F for read/write)

	 // - initialize RTC with current time
	 // - read time from registers


	 // Set GPIO pins as SDA and SCL
	 GPIO_Configurations_SCL_SDA();

	 // Initialize I2C peripheral
	 I2C_Handle_t handle;
	 handle.I2Cx = I2C1;
	 handle.I2C_Config.I2C_Speed_mode = Standard_Mode;
	 handle.I2C_Config.I2C_SCL_Speed  = 100000;
	 I2C_Init(&handle);


	 // Setup data buffer to transmit time
	 // - stores seconds, minutes, hours
	 // - stored in BCD format
	 // - MSB of seconds must be 0
	 // - Bit_6 of hours must be 0

	 uint8_t buffer[3];

	 // seconds
	 uint8_t seconds= 00;
	 buffer[0] = ((((seconds/10)<< 4)| (seconds%10) )& 0x7F);

	 // minutes
	 uint8_t minutes = 10;
	 buffer[1] = ( ((minutes/10)<< 4)|(minutes%10) );

	 // hours
	 uint8_t hours = 23;
	 buffer[2] = ( (((hours/10)<< 4)|(hours%10)) & 0x3F );

	 // Initialize RTC time by transmitting buffer
	 I2C_Transmit_Buffer(I2C1, 0x00, buffer, 3, RTC_ADDR);



	 while(1)
	 {
		 // Receive RTC data into a buffer
		 I2C_Receive_Buffer(I2C1, 0x00, buffer , 3, RTC_ADDR);

		 // convert data to decimal format
		 seconds = (((buffer[0]>>4)*10)  + (buffer[0]&0x0F));
                 minutes = ((( buffer[1]>>4)*10) + ( buffer[1]&0x0F));
		 hours  =  (((buffer[2] >>4)*10) + (buffer[2] &0x0F));

		 // Send to USART terminal

	         USART_Send_String("\n\nCurrent Time: ");
	         USART_Send_Number(hours);
	         USART_Send_String(": ");
		 USART_Send_Number(minutes);
		 USART_Send_String(": ");
		 USART_Send_Number(seconds);


		 for(volatile uint32_t i=0;i<500000;i++);

	 }



}


