/*
 * Blink.c
 *
 *  Created on: Jun 1, 2026
 *      Author: hp
 */


#include <stdint.h>
#include "STM32F401RE.h"
#include "stm32f401re_gpio_driver.h"


int blink(void)
{
	GPIO_Handle_t led;
	led.pGPIOx = GPIOA;
	led.GPIO_PinConfig.GPIO_PinNumber = 5;
	led.GPIO_PinConfig.GPIO_PinMode   = GPIO_MODE_OUT;
	led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PUSHPULL;

	GPIO_Init(&led);

	// Blinking on-board LED PA5
	while(1)
	{
		GPIO_Toggle_OutputPin(GPIOA, 5);

		// Crude delay
		for(volatile uint32_t i=0;i<200000;i++);
	}

}
