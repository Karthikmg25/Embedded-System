/*
 * stm32f401re_gpio_driver.h
 *
 *  Created on: Jan 29, 2026
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

	GPIO_PinConfig_t GPIO_PinConfig;// for GPIO configuration

}GPIO_Handle_t;


/*
 * API for GPIO pin
 *
 */

void GPIO_PeriClockControl(GPIO_Reg_t *pGPIOx, uint8_t EnorDi);
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_Reg_t *pGPIOx);

uint8_t GPIO_Read_InputPin(GPIO_Reg_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_Read_InputPort(GPIO_Reg_t *pGPIOx);

void GPIO_Write_OutputPin(GPIO_Reg_t *pGPIOx, uint8_t PinNumber, uint8_t value);
void GPIO_Write_OutputPort(GPIO_Reg_t *pGPIOx, uint16_t value);
void GPIO_Toggle_OutputPin(GPIO_Reg_t *pGPIOx, uint8_t PinNumber);

/*
 *  GPIO PINS
 *
 */
#define GPIO_PIN_NO_0            0
#define GPIO_PIN_NO_1            1
#define GPIO_PIN_NO_2            2
#define GPIO_PIN_NO_3            3
#define GPIO_PIN_NO_4            4
#define GPIO_PIN_NO_5            5
#define GPIO_PIN_NO_6            6
#define GPIO_PIN_NO_7            7
#define GPIO_PIN_NO_8            8
#define GPIO_PIN_NO_9            9
#define GPIO_PIN_NO_10           10
#define GPIO_PIN_NO_11           11
#define GPIO_PIN_NO_12           12
#define GPIO_PIN_NO_13           13
#define GPIO_PIN_NO_14           14
#define GPIO_PIN_NO_15           15

/*
 *  GPIO PIN MODES
 *
 */
#define GPIO_MODE_IN          0
#define GPIO_MODE_OUT         1
#define GPIO_MODE_ALTFN       2
#define GPIO_MODE_ANALOG      3

/*
 *  GPIO PIN SPEED
 *
 */
#define GPIO_SPEED_LOW            0
#define GPIO_SPEED_MEDIUM         1
#define GPIO_SPEED_FAST           2
#define GPIO_SPEED_HIGH           3

/*
 *  GPIO PIN OUTPUT TYPE
 *
 */
#define GPIO_OP_TYPE_PUSHPULL      0
#define GPIO_OP_TYPE_OPENDRAIN     1

/*
 *  GPIO PIN PULL-UP PULL-DOWN
 *
 */
#define GPIO_NO_PUPD           0
#define GPIO_PIN_PULLUP        1
#define GPIO_PIN_PULLDOWN      2
/*
 *  GPIO PIN SET / RESET
 *
 */
#define GPIO_PIN_SET      1
#define GPIO_PIN_CLEAR    0
#endif /* STM32F401RE_GPIO_DRIVER_H_ */
