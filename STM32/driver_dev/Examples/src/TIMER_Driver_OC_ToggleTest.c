/*
 * TIMER_Driver_OC_ToggleTest.c
 *
 *  Created on: Oct 2, 2026
 *      Author: hp
 */


#include <stdio.h>
#include "stm32f401re_timer_driver.h"

void GPIO_Config_TIM2_CH1();

int main(void)
{

	// program general purpose timers //


	// Set basic time-base settings of TIM2
	/******************************************************************************************/

	// declare a handle for timer 2 base configurations
	TIM_Handle_t htim2={0};
	    htim2.pTIMx                      = TIM2;
	    htim2.TIM_Base.Prescaler         = 15999;// t_frequency = 1KHz, 1 count = 1ms
	    htim2.TIM_Base.Period            = 999;  // update_frequency = 1 Hz, 1 overflow = 1s
		htim2.TIM_Base.CounterMode       = TIM_COUNTERMODE_UPCOUNTING;   // counts up to ARR
		htim2.TIM_Base.AutoReloadPreload = TIM_AUTORELOADPRELOAD_DISABLE;// ARR not buffered

		TIM_Status_t status = TIM_BaseInit(&htim2);

		if(status == TIM_ERROR)
		{
			printf("Timer init failed !!");
			return 1;

		}

		// Configure Channel 1 of TIM2 for Output compare: Toggle mode
		/**********************************************************************************/

		TIM_OC_Config_t OC_ch1 = {0};

		OC_ch1.Channel = TIM_CHANNEL_1;
		OC_ch1.CompareValue = 100;
		OC_ch1.OutputMode   = TIM_OCMODE_TOGGLE;
		OC_ch1.OutputPolarity = TIM_OCPOLARITY_HIGH;

		TIM_OC_Init(&htim2, &OC_ch1);

		// Settings : Counter counts till ARR- 999. On CNT = 500, O/P Pin will Toggle

		//  this generates a signal where frequency = update_frequency / 2 ( Here, update freq = 1Hz. so, signal freq = 0.5Hz )



		// Before starting channel, configure GPIO pin as TIM2_CH1
		GPIO_Config_TIM2_CH1();

		// Start Channel
		TIM_OC_Start(&htim2, &OC_ch1);


	while(1)
	{
	}
}

void GPIO_Config_TIM2_CH1()
{
	// Pin is PA0 with alternate function- AF1

	GPIOA_CLK_EN();

	// set AF mode
	GPIOA->MODER &=~ (0x3 << (0*2));
	GPIOA->MODER |=  (0x2 << (0*2));

	// set AF1
	GPIOA->AFR[0] &=~ (0xF << (0*4));
	GPIOA->AFR[0] |=  (1 << (0*4));
}




