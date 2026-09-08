/*
 * stm32f401re_systick_driver.c
 *
 *  Created on: Sep 7, 2026
 *      Author: hp
 */


#include "stm32f401re_systick_driver.h"

/*
 * Private tick counter.
 * Incremented by the SysTick interrupt handler.
 *
 */
static volatile uint32_t tick_count =0;

/*
 * Configured SysTick frequency in ticks per second.
 * Used for converting milliseconds to SysTick ticks.
 */
static uint32_t tick_f;

/********************************************************************************************************************/
/* Public APIs*/
/********************************************************************************************************************/



/*
 * fn: SysTick_Init()
 *
 * breif:    Configure and start the Cortex-M SysTick timer.
 *
 * param[]:  tick_frequency : Number of SysTick ticks to generate per second.
 * param[]:  clk_src        : SysTick clock source (AHB or AHB/8).
 *
 * Returns:
 *   SYSTICK_OK                       - Initialization successful
 *   SYSTICK_ERROR_INVALID_FREQUENCY  - Invalid/unachievable frequency
 *   SYSTICK_ERROR_INVALID_CLKSRC     - Invalid clock source
 */
SysTick_Status_t SysTick_Init(uint32_t tick_frequency, SysTick_ClkSrc_t clk_src)
{

	 /* Store configured tick frequency for time conversion APIs */
	tick_f = tick_frequency;

	// validate user inputs first

	if(tick_frequency == 0)
	{
		return SYSTICK_ERROR_INVALID_FREQUENCY;
	}

	// Get HCLK frequency from RCC driver
	uint32_t hclk_f = RCC_GetHCLKFreq();


	/* Find the clock supplied to SysTick */

	uint32_t systick_clk;

	if(clk_src == SYSTICK_CLK_AHB)
	{
		systick_clk = hclk_f;
	}

	else if(clk_src == SYSTICK_CLK_AHB_DIV8)
	{
		systick_clk = hclk_f / 8;
	}
	else
	{
		return SYSTICK_ERROR_INVALID_CLKSRC;
	}

	// Calculate the reload value
	uint32_t reload_value = ( systick_clk / tick_frequency) - 1;

	 /* SysTick LOAD register is limited to 24 bits */
	if(reload_value >= 0xFFFFFF)
	{
		return SYSTICK_ERROR_INVALID_FREQUENCY;
	}


	// Disable Systick before programming registers
	SYSTICK->CTRL = 0;

	// program the reload value to LOAD register
	SYSTICK->LOAD = reload_value;

	//clear current value
	SYSTICK->VAL = 0;

	/* Select AHB as the SysTick clock source.
	     * CLKSOURCE = 0 selects AHB/8.
	     */
	if(clk_src == SYSTICK_CLK_AHB)
	{
		SYSTICK->CTRL |=  (1<< SYSTICK_CTRL_CLKSOURCE);
	}

	// Enable the counter, enable interrupt
	SYSTICK->CTRL |= (1<< SYSTICK_CTRL_ENABLE)
			      |  (1<< SYSTICK_CTRL_TICKINT);

	return SYSTICK_OK;
}

/*
 * SysTick_Handler()
 *
 * SysTick interrupt service routine.
 * Increments the software tick counter every SysTick interrupt.
 */
void SysTick_Handler()
{
	tick_count++;
}

/*
 * SysTick_GetTick()
 *
 * param[]: Return the number of SysTick interrupts that have occurred
 *          since the tick counter started.
 *
 * Returns:
 *   Current SysTick count in ticks.
 *
 * Note:
 *   The returned value represents ticks, not milliseconds.
 */
uint32_t SysTick_GetTick()
{
	return tick_count;
}



/*
 * SysTick_Delay_ms()
 *
 * Create a blocking delay for approximately the requested
 * number of milliseconds.
 *
 * The requested time is converted to SysTick ticks using
 * the frequency configured during SysTick_Init().
 *
 * param[]: ms: requested delay time
 *
 * Note:
 *   Delay resolution depends on the configured SysTick frequency.
 */
void SysTick_Delay_ms(uint32_t ms)
{
	uint32_t ticks = ((uint64_t)ms * tick_f ) / 1000;// ms*tick_f may overflow 32 bits

	uint32_t start = SysTick_GetTick();

	while((SysTick_GetTick() - start ) < ticks);

}


/*
 * SysTick_HasElapsedTicks()
 *
 * Check whether the specified number of SysTick ticks has
 * elapsed since the supplied starting tick.
 *
 * param[]: start   : Tick count recorded when timing started.
 * param[]: timeout : Number of ticks to wait.
 *
 * Returns:
 *   true  - Timeout has elapsed.
 *   false - Timeout has not yet elapsed.
 *
 * Note: This function is non-blocking and does not modify 'start'.
 * The application must update its start value when the event
 * has been handled.
 */
bool SysTick_HasElapsedTicks(uint32_t start, uint32_t timeout)
{

	// send true if timeout elapsed
	if(SysTick_GetTick() - start >= timeout)
	{
		return true;
	}
	else
	{
		return false;
	}

}










