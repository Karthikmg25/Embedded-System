/*
 * GPIO_Assignment.c
 *
 *  Created on: Feb 27, 2026
 *      Author: hp
 */

#include "GPIO_Assignment.h"
void GPIO_init()
{

	GPIO_Handle_t GPIOled;// declare structure variable for handle structure
	GPIO_Handle_t GPIOread;// for reading button press

	GPIOled.pGPIOx=GPIOA;
	GPIOled.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_5;   // set required configurations for PA5
	GPIOled.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_OUT;
	GPIOled.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
	GPIOled.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PUSHPULL;
	GPIOled.GPIO_PinConfig.GPIO_PinPUPD=GPIO_NO_PUPD;

	GPIO_Init(&GPIOled);// initialize PA5

	GPIOread.pGPIOx=GPIOC;                                  // set configurations for PC13
	GPIOread.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_13;  // button is active low
	GPIOread.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_IN ;     // configure pin PC13, on-board button
	GPIOread.GPIO_PinConfig.GPIO_PinPUPD = GPIO_PIN_PULLUP; // internal pull-up


	GPIO_Init(&GPIOread);// initialize PC13

	GPIO_Handle_t led1, led2;// external LED blink

	led1.pGPIOx= GPIOA;
	led2.pGPIOx= GPIOA;
	led1.GPIO_PinConfig.GPIO_PinNumber= GPIO_PIN_NO_0;
	led2.GPIO_PinConfig.GPIO_PinNumber= GPIO_PIN_NO_1;
    led1.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_OUT;
    led2.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_OUT;
    led1.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
    led2.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
    led1.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PUSHPULL;
    led2.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PUSHPULL;// set configurations for led1 and led2

    GPIO_Init(&led1);
    GPIO_Init(&led2);// initialize PA0 and PA1

    GPIO_Write_OutputPin(GPIOA, GPIO_PIN_NO_0, 0);
    GPIO_Write_OutputPin(GPIOA, GPIO_PIN_NO_1, 0);// set PA0 as HIGH and PA1 as LOW at first


}
static uint8_t current_state=1, prev_state=1; // to detect falling edge
void GPIO_Toggle_ButtonRead()
{

	static uint32_t count=0, time=0;              // to set up a software counter

    current_state=GPIO_Read_InputPin(GPIOC, GPIO_PIN_NO_13);

	if(prev_state==1 && current_state==0)// button press creates falling edge
	{
		GPIO_Toggle_OutputPin(GPIOA ,GPIO_PIN_NO_5);// toggle LED
	}
	   	prev_state= current_state;// update previous state

	   	if(count-time > 200000)// non-blocking delay
	   	{
	   		GPIO_Toggle_OutputPin(GPIOA, GPIO_PIN_NO_0 );
	   		GPIO_Toggle_OutputPin(GPIOA, GPIO_PIN_NO_1 );
	 		time=count;// update time
	   	}
	      count++;
}
void led_ON()
{
			 GPIO_Write_OutputPin(GPIOA, 0, 1);
}
void led_OFF()
{
			 GPIO_Write_OutputPin(GPIOA, 0, 0);
}
