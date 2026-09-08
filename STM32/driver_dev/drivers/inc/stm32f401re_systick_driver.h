/*
 * stm32f401re_systick_driver.h
 *
 *  Created on: Sep 7, 2026
 *      Author: hp
 */

#ifndef INC_STM32F401RE_SYSTICK_DRIVER_H_
#define INC_STM32F401RE_SYSTICK_DRIVER_H_

#include "STM32F401RE.h"

//include RCC driver for reading processor frequency
#include "stm32f401re_rcc_driver.h"

//define an enum to set the systick clock source
typedef enum
{
	SYSTICK_CLK_AHB,
	SYSTICK_CLK_AHB_DIV8

}SysTick_ClkSrc_t;

//define an enum to return status
typedef enum
{
	SYSTICK_OK,
	SYSTICK_ERROR_INVALID_FREQUENCY,
	SYSTICK_ERROR_INVALID_CLKSRC

}SysTick_Status_t;

// Declare APIs

//initialization
SysTick_Status_t SysTick_Init(uint32_t tick_frequency, SysTick_ClkSrc_t clk_src);

//time-base
uint32_t SysTick_GetTick();

// blocking delay
void SysTick_Delay_ms(uint32_t ms);

// Non-blocking elapsed-time check
bool SysTick_HasElapsedTicks(uint32_t start, uint32_t timeout);















#endif /* INC_STM32F401RE_SYSTICK_DRIVER_H_ */
