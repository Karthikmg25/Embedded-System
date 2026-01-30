/*
 * stm32f401re_gpio_driver.c
 *
 *  Created on: Jan 29, 2026
 *      Author: hp
 */
#include <stdint.h>
#include "stm32f401re_gpio_driver.h"

/*************************************************************************
 * @fn                - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - pGPIOx: base address of the GPIO peripheral(GPIOA, GPIOB, .....GPIOH)
 * @param[in]         - EnorDi: ENABLE or DISABLE macros
 *
 * @return            - none
 *
 * @Note              - Refer macros for GPIO configurations
 */
void GPIO_PeriClockControl(GPIO_Reg_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi==ENABLE)
	{
	        if(pGPIOx== GPIOA)
              {
        	    GPIOA_CLK_EN();
              }
	  else  if(pGPIOx== GPIOB)
	          {
	          	GPIOB_CLK_EN();
	          }

	  else  if(pGPIOx== GPIOC)
	          {
	          	GPIOC_CLK_EN();
	          }
	  else  if(pGPIOx== GPIOD)
	          {
	          	GPIOD_CLK_EN();
	          }
	  else  if(pGPIOx== GPIOE)
	          {
	          	GPIOE_CLK_EN();
	          }
	  else  if(pGPIOx== GPIOH)
	          {
	          	GPIOH_CLK_EN();
	          }
	}
	else
	{
		            if(pGPIOx== GPIOA)
		              {
		        	    GPIOA_CLK_DI();
		              }
			  else  if(pGPIOx== GPIOB)
			          {
			          	GPIOB_CLK_DI();
			          }

			  else  if(pGPIOx== GPIOC)
			          {
			          	GPIOC_CLK_DI();
			          }
			  else  if(pGPIOx== GPIOD)
			          {
			          	GPIOD_CLK_DI();
			          }
			  else  if(pGPIOx== GPIOE)
			          {
			          	GPIOE_CLK_DI();
			          }
			  else  if(pGPIOx== GPIOH)
			          {
			          	GPIOH_CLK_DI();
			          }

	}
}
