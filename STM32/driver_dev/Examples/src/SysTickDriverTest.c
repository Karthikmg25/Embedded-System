/*
 * SysTickDriverTest.c
 *
 *  Created on: Sep 18, 2026
 *      Author: hp
 */


#include <stdio.h>
#include "stm32f401re_systick_driver.h"

int Sys_main(void)
{

	SysTick_Status_t status = SysTick_Init(1000, SYSTICK_CLK_AHB);// generate 1000 ticks per second (1 tick = 1ms), use core frequency as clock source


	uint32_t seconds = 0;

	// record starting time
	uint32_t start_1s = SysTick_GetTick();
	uint32_t start_2s = SysTick_GetTick();  // Multiple independant timers formsame timebase

	while(1)
	{
		// non blocking delay
			if(SysTick_HasElapsedTicks(start_1s, 1000))// print only if 1000 ticks has passed (1 second)
			{
				printf("\n1 second elapsed %d ", (int)seconds++);

				//Update start after timeout
				start_1s = SysTick_GetTick();
			}

			// non blocking delay
				if(SysTick_HasElapsedTicks(start_2s, 2000))// print only if 2000 ticks has passed (2 seconds)
				{
					printf("\n2 seconds elapsed %d ", (int)seconds++);

					//Update start after timeout
					start_2s = SysTick_GetTick();
				}

	}
}












