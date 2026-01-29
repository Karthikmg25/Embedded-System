/*
 * stm32f401re_gpio_driver.h
 *
 *  Created on: Jan 27, 2026
 *      Author: hp
 */

#ifndef STM32F401RE_GPIO_DRIVER_H_
#define STM32F401RE_GPIO_DRIVER_H_

#include "STM32F401RE.h"

/*
 * Configuration structure for GPIO pin
 *
 */
typedef struct
{
	uint8_t GPIO_PinNumber; // pin number(0-15)
	uint8_t GPIO_PinMode;   // set pin mode
	uint8_t GPIO_PinSpeed;  // set pin speed
	uint8_t GPIO_PinOPType; // set output type
	uint8_t GPIO_PinPUPD;   // Pull UP and Pull down
	uint8_t GPIO_PINAlternate;// set alternate function

}GPIO_PinConfig_t;

/*
 * Handle structure for GPIO pin
 *
 */
typedef struct
{
	GPIO_Reg_t *pGPIOx;     // for GPIO register access

	GPIO_PinConfig_t *pGPIO_PinConfig;// for GPIO configuration

}GPIO_Handle_t;

/*
 * API for GPIO pin
 *
 */

#endif /* STM32F401RE_GPIO_DRIVER_H_ */
