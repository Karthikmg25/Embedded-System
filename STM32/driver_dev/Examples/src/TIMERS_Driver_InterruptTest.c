/*
 * TIMERS_Driver_InterruptTest.c
 *
 *  Created on: Oct 1, 2026
 *      Author: hp
 */

#include <stdio.h>
#include "stm32f401re_timer_driver.h"



volatile uint32_t ovflow_count_tim2 = 0;

void delay_ms(uint32_t ms)
{
	uint32_t start = ovflow_count_tim2;

	while(ovflow_count_tim2 - start < ms);
}



void TIM2_Handling()
{
	ovflow_count_tim2++;

}

volatile uint32_t ovflow_count_tim3 = 0;




void TIM3_Handling()
{
	ovflow_count_tim3++;

}


int main(void)
{

	// program general purpose timers //


	// program TIM2 for basic time-base settings

	// declare a handle for timer 2 base configurations
	TIM_Handle_t htim2={0};
	    htim2.pTIMx                      = TIM2;
	    htim2.TIM_Base.Prescaler         = 15;// t_frequency = 1MHz, 1 count = 1us
	    htim2.TIM_Base.Period            = 999;// update_frequency = 1KHz, 1 overflow = 1ms
		htim2.TIM_Base.CounterMode       = TIM_COUNTERMODE_UPCOUNTING;   // counts up to ARR
		htim2.TIM_Base.AutoReloadPreload = TIM_AUTORELOADPRELOAD_DISABLE;// ARR not buffered

		TIM_Status_t status = TIM_BaseInit(&htim2);

		if(status == TIM_ERROR)
		{
			printf("Timer init failed !!");
			return 1;

		}
		// register callback before starting timer
		TIM_RegisterCallback(&htim2, TIM2_Handling);

		// Enable IRQ before counter starts
		TIM_IRQEnable(&htim2);

		TIM_BaseStart_IT(&htim2);

		// program TIM3 for basic time-base settings

		TIM_Handle_t htim3={0};
		htim3.pTIMx                      = TIM3;
		htim3.TIM_Base.Prescaler         = 15;// t_frequency = 1MHz, 1 count = 1us
		htim3.TIM_Base.Period            = 1999;//  1 overflow = 2ms
		htim3.TIM_Base.CounterMode       = TIM_COUNTERMODE_UPCOUNTING;   // counts up to ARR
		htim3.TIM_Base.AutoReloadPreload = TIM_AUTORELOADPRELOAD_DISABLE;// ARR not buffered

		status = TIM_BaseInit(&htim3);

				if(status == TIM_ERROR)
				{
					printf("Timer init failed !!");
					return 1;

				}
				// register callback before starting timer
				TIM_RegisterCallback(&htim3, TIM3_Handling);

				// Enable IRQ before counter starts
				TIM_IRQEnable(&htim3);

				TIM_BaseStart_IT(&htim3);




	while(1)
	{
		delay_ms(1000);

		printf("\n\nOne second passed");
		printf("\n tim2 overflow : %d", (int)ovflow_count_tim2);
		printf("\n tim3 overflow : %d", (int)ovflow_count_tim3);
	}
}






