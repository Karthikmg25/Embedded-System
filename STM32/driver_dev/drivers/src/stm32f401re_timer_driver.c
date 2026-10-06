/*
 * stm32f401re_timer_driver.c
 *
 *  Created on: Sep 28, 2026
 *      Author: hp
 */

#include "stm32f401re_timer_driver.h"


/**
 * @fn      - TIM_PeriClockControl
 * @brief   - Enable or disable the peripheral clock of a timer.
 * @details - Controls the RCC clock gate for the selected timer instance.
 *            The timer registers can only be accessed while its clock is enabled.
 *
 * @param[in] - pTIMx   Timer register block (e.g. TIM2, TIM3, TIM4, TIM5).
 *                      Must not be NULL. Other values are ignored.
 * @param[in] - EnorDi  ENABLE to turn the clock on, DISABLE to turn it off
 *                      (use ENABLE/DISABLE macros).
 *
 * @pre      - Called inside TIM_Base_Init, so users normally don't call it directly.
 * @note     - Writes to a timer's registers while its clock is off have no effect.
 * @note     - Disabling the clock stops the timer, so counting, outputs and
 *             capture stop with it.
 *
 */
void TIM_PeriClockControl(TIM_Reg_t *pTIMx, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		    if(pTIMx  == TIM2)
			{
				TIM2_CLK_EN();
			}
			else if(pTIMx == TIM3)
			{
				TIM3_CLK_EN();
			}
			else if(pTIMx  == TIM4)
			{
				TIM4_CLK_EN();
			}
			else if(pTIMx == TIM5)
			{
				TIM5_CLK_EN();
			}

	}
	else{
		    if(pTIMx  == TIM2)
		    {
				TIM2_CLK_DI();
			}
			else if(pTIMx == TIM3)
			{
				TIM3_CLK_DI();
			}
			else if(pTIMx  == TIM4)
			{
				TIM4_CLK_DI();
			}
			else if(pTIMx == TIM5)
			{
				TIM5_CLK_DI();
			}

	}

}

/**
 * Application handle pointers
 *
 * @brief   - One per timer, used for callback dispatch.
 * @details - Set by TIM_BaseInit. The timer's IRQ handler uses the matching pointer
 *            to reach the handle and call the registered callbacks.
 * @note    - Zero-initialized (NULL) until TIM_BaseInit is called for that timer.
 * @note    - Private to the driver (static). Applications must not access them.
 *
 */
static TIM_Handle_t *pTIM2Handle;
static TIM_Handle_t *pTIM3Handle;
static TIM_Handle_t *pTIM4Handle;
static TIM_Handle_t *pTIM5Handle;

/**
 * @brief   - Initialize the timebase of a timer and register its handle.
 *
 * @details - Validates the configuration, enables the timer clock, then programs
 *            counter mode, auto-reload preload, one-pulse mode, PSC and ARR from
 *            pTIMHandle->TIM_Base. An update event is generated so the new PSC/ARR
 *            values take effect immediately (the update flag is cleared afterwards).
 *            All callbacks in the handle are reset to NULL, and the handle pointer is
 *            stored for the timer's IRQ handler, which uses it to dispatch callbacks.
 *
 * @param[in] pTIMHandle - Timer handle. Must not be NULL. pTIMx and TIM_Base
 *                             must be filled in by the caller before this call.
 *
 * @return    TIM_OK    - Timebase initialized.
 * @return    TIM_ERROR - Invalid configuration. No register is modified in this case.
 *                        Causes: NULL handle, pTIMx not TIM2/TIM3/TIM4/TIM5, invalid
 *                        CounterMode, AutoReloadPreload or OnePulseMode value, or
 *                        Period above 0xFFFF on TIM3/TIM4.
 *
 * @note    - Call this before starting the timer. Re-initializing a running timer
 *            restarts its count (update event), resets all callbacks to NULL, and
 *            switching between edge-aligned and centre-aligned modes is not
 *            allowed while the counter is enabled.
 * @note    - After init the timer is configured but NOT started, and no interrupt
 *            is enabled
 * @note    - The timer clock is enabled internally via TIM_PeriClockControl.
 * @note    - GPIO clock and pin configuration are not handled by the driver.
 * @note    - The driver keeps a pointer to the handle, so it must stay valid for as
 *            long as the timer is in use. Do not pass a local (stack) variable.
 * @note    - Only one handle per timer instance is tracked. A second init on the
 *            same instance replaces the stored handle.
 *
 */
TIM_Status_t TIM_BaseInit(TIM_Handle_t* pTIMHandle)
{

	// < Check the parameters > //
	/*****************************************************************************************************/

	if(pTIMHandle == NULL)
	{
		return TIM_ERROR;
	}
	if(     pTIMHandle->pTIMx != TIM2 &&
			pTIMHandle->pTIMx != TIM3 &&
			pTIMHandle->pTIMx != TIM4 &&
			pTIMHandle->pTIMx != TIM5)
	{
			return TIM_ERROR;
	}

	if(     pTIMHandle->TIM_Base.CounterMode != TIM_COUNTERMODE_UPCOUNTING       &&
			pTIMHandle->TIM_Base.CounterMode != TIM_COUNTERMODE_DOWNCOUNTING     &&
			pTIMHandle->TIM_Base.CounterMode != TIM_COUNTERMODE_CENTRE_ALIGNED_1 &&
			pTIMHandle->TIM_Base.CounterMode != TIM_COUNTERMODE_CENTRE_ALIGNED_2 &&
			pTIMHandle->TIM_Base.CounterMode != TIM_COUNTERMODE_CENTRE_ALIGNED_3 )
	{
		return TIM_ERROR;
	}
	if(     pTIMHandle->TIM_Base.OnePulseMode != TIM_ONEPULSEMODE_DISABLE &&
			pTIMHandle->TIM_Base.OnePulseMode != TIM_ONEPULSEMODE_ENABLE)
	{
		return TIM_ERROR;
	}
	if(     pTIMHandle->TIM_Base.AutoReloadPreload != TIM_AUTORELOADPRELOAD_DISABLE &&
			pTIMHandle->TIM_Base.AutoReloadPreload != TIM_AUTORELOADPRELOAD_ENABLE)
	{
		return TIM_ERROR;
	}

	if(		(pTIMHandle->pTIMx == TIM3 || pTIMHandle->pTIMx == TIM4) &&
			(pTIMHandle->TIM_Base.Period > 65535))
	{
		// ARR is 16bit for TIM3 and TIM4
		return TIM_ERROR;
	}


	// enable timer clock
	/************************************************************************************************************/

	TIM_PeriClockControl(pTIMHandle->pTIMx, ENABLE);

	// Set the Time base configurations //
	/***********************************************************************************************************/

	// Edge-aligned Counting modes

	if(pTIMHandle->TIM_Base.CounterMode == TIM_COUNTERMODE_UPCOUNTING)
	{
		pTIMHandle->pTIMx->CR1 &=~(1<< TIMx_CR1_DIR);
		pTIMHandle->pTIMx->CR1 &=~(0x3 << TIMx_CR1_CMS); // clear CMS bits

	}
	else if(pTIMHandle->TIM_Base.CounterMode == TIM_COUNTERMODE_DOWNCOUNTING)
	{
		pTIMHandle->pTIMx->CR1 |= (1<< TIMx_CR1_DIR);
		pTIMHandle->pTIMx->CR1 &=~(0x3 << TIMx_CR1_CMS); // clear CMS bits
	}

	// Centre aligned counting modes

	else if(pTIMHandle->TIM_Base.CounterMode == TIM_COUNTERMODE_CENTRE_ALIGNED_1 )
	{
		pTIMHandle->pTIMx->CR1 &=~(0x3 << TIMx_CR1_CMS);
		pTIMHandle->pTIMx->CR1 |= (0x1 << TIMx_CR1_CMS);
	}

	else if(pTIMHandle->TIM_Base.CounterMode == TIM_COUNTERMODE_CENTRE_ALIGNED_2 )
	{
		pTIMHandle->pTIMx->CR1 &=~(0x3 << TIMx_CR1_CMS);
		pTIMHandle->pTIMx->CR1 |= (0x2 << TIMx_CR1_CMS);
	}

	else if(pTIMHandle->TIM_Base.CounterMode == TIM_COUNTERMODE_CENTRE_ALIGNED_3 )
	{
		pTIMHandle->pTIMx->CR1 &=~(0x3 << TIMx_CR1_CMS);
		pTIMHandle->pTIMx->CR1 |= (0x3 << TIMx_CR1_CMS);
	}

	// auto reload preload settings
	pTIMHandle->pTIMx->CR1 &=~(1<< TIMx_CR1_APRE);
	pTIMHandle->pTIMx->CR1 |= (pTIMHandle->TIM_Base.AutoReloadPreload << TIMx_CR1_APRE);

	// one-pulse mode
	pTIMHandle->pTIMx->CR1 &=~(1<< TIMx_CR1_OPM);
	pTIMHandle->pTIMx->CR1 |= (pTIMHandle->TIM_Base.OnePulseMode << TIMx_CR1_OPM);

	// set prescaler, divides timer source clock
	pTIMHandle->pTIMx->PSC = pTIMHandle->TIM_Base.Prescaler;

	// set top count value, sets period/update frequency
	pTIMHandle->pTIMx->ARR = pTIMHandle->TIM_Base.Period;



	// !! generate an update event to avoid buffering and start using configured-new values in PSC and ARR registers !! //
	/********************************************************************************************************************/

	pTIMHandle->pTIMx->EGR |= (1<< TIMx_EGR_UG);
	// clear flag after update generation
	pTIMHandle->pTIMx->SR &=~ (1<< TIMx_SR_UIF);


	// Handle Callback mechanism
	/********************************************************************************************************************/

	// Initialize callback functions as NULL
	pTIMHandle->UpdateCallback = NULL;

	pTIMHandle->IC_Callback[0] = NULL;
	pTIMHandle->IC_Callback[1] = NULL;
	pTIMHandle->IC_Callback[2] = NULL;
	pTIMHandle->IC_Callback[3] = NULL;

	// Store pointer to application handle for callback handling in IRQHandler

	if(pTIMHandle->pTIMx == TIM2)
	{
		// Application is using TIM2
		pTIM2Handle = pTIMHandle;
	}
	else if(pTIMHandle->pTIMx == TIM3)
	{
		// Application using TIM3
		pTIM3Handle = pTIMHandle;
	}
	else if(pTIMHandle->pTIMx == TIM4)
	{
		// Application using TIM4
		pTIM4Handle = pTIMHandle;
	}
	else if(pTIMHandle->pTIMx == TIM5)
	{
		// Application using TIM5
		pTIM5Handle = pTIMHandle;
	}

	return TIM_OK;

}
/**
 * @fn      - TIM_BaseStart
 *
 * @brief   - Start the timer counter.
 *
 * @details - Sets the counter enable bit (CEN) in CR1, so the counter starts
 *            running from its current CNT value. Update events, output compare
 *            and input capture operate once the counter is running.
 *
 * @param[in] pTIMHandle - Timer handle. Must not be NULL.
 *
 * @note    - TIM_BaseInit must be called first, since it enables the timer clock
 *            and loads the configuration.
 * @note    - Interrupts are not enabled by this function. Enable them separately
 *            if update or capture callbacks are needed.
 * @note    - In one-pulse mode the counter stops itself at the next update event,
 *            so call this function again to run another pulse.
 * @note    - No parameter check is done. Passing NULL is undefined behavior.
 *
 */
void TIM_BaseStart(TIM_Handle_t* pTIMHandle)
{
	pTIMHandle->pTIMx->CR1 |= (1<< TIMx_CR1_CEN);
}
/**
 *
 * @fn      - TIM_GetFlagStatus
 *
 * @brief   - Read the state of a status flag in the timer's SR register.
 *
 * @details - Returns the value of one bit of SR, selected by its bit position.
 *            Reading does not clear the flag, so clear it separately once it has
 *            been handled.
 *
 * @param[in] pTIMHandle - Timer handle. Must not be NULL.
 * @param[in] TIM_Flag   - Bit position of the flag in SR (not a bit mask).
 *                         Use the driver's SR flag macros
 *                         (e.g. update flag, CC1 to CC4 capture/compare flags).
 *
 * @return 1 - The flag is set.
 * @return 0 - The flag is cleared.
 *
 * @note    - TIM_Flag is a bit position, not a mask. Passing a mask
 *            instead of the position gives wrong results.
 * @note    - No parameter check is done. A NULL handle or a TIM_Flag above 31 is
 *            undefined behavior.
 * @note    - Useful for polling, e.g. waiting for the update flag when no
 *            interrupt is used.
 *
 * @code
 * TIM_BaseStart(&htim2);
 * while (!TIM_GetFlagStatus(&htim2, TIMx_SR_UIF)) { }   // wait for update event
 * @endcode
 *
 */
uint8_t TIM_GetFlagStatus(TIM_Handle_t *pTIMHandle, uint8_t TIM_Flag)
{
	return ((pTIMHandle->pTIMx->SR >> TIM_Flag)&1);
}
/**
 *
 * @fn      - TIM_ClearFlag
 *
 * @brief   - Clear a status flag in the timer's SR register.
 *
 * @details - Clears one flag, selected by its bit position, by writing 0 to it.
 *            Call it once the event has been handled, so the next event can be
 *            detected.
 *
 * @param[in] pTIMHandle - Timer handle. Must not be NULL.
 * @param[in] TIM_Flag   - Bit position of the flag in SR (not a bit mask).
 *                         Use the driver's SR flag macros
 *                         (e.g. update flag, CC1 to CC4 capture/compare flags).
 *
 * @note    - TIM_Flag is a bit position, not a mask. Passing a mask such as 0x01
 *            instead of the position clears the wrong flag.
 * @note    - Flags in SR are cleared by writing 0. Writing 1 leaves them unchanged.
 * @note    - A capture flag (CCxIF) is also cleared by reading the matching CCRx
 *            register, so it may already be cleared when you reach this call.
 * @note    - No parameter check is done. A NULL handle or a TIM_Flag above 31 is
 *            undefined behavior.
 * @see     - TIM_GetStatus
 *
 * @code
 * while (!TIM_GetStatus(&htim2, TIMx_SR_UIF)) { }   // wait for update event
 * TIM_ClearFlag(&htim2, TIMx_SR_UIF);               // re-arm for the next one
 * @endcode
 */
void TIM_ClearFlag(TIM_Handle_t *pTIMHandle, uint8_t TIM_Flag)
{
	pTIMHandle->pTIMx->SR &=~(1<< TIM_Flag);
}
/**
 *
 * @fn      - TIM_BaseStop
 *
 * @brief   - Stop the timer counter.
 *
 * @details - Clears the counter enable bit (CEN) in CR1, which freezes the counter
 *            at its current value. Configuration and registered callbacks are kept,
 *            so the timer can be resumed with TIM_BaseStart.
 *
 * @param[in] pTIMHandle - Timer handle. Must not be NULL.
 *
 * @note    - This function only clears CEN. It does not reset CNT, disable the timer
 *            clock, change interrupt enables or clear status flags.
 * @note    - TIM_BaseStart continues counting from the value where the counter stopped.
 * @note    - While the counter is stopped, no update events or captures occur, and
 *            output channels hold their last level (no further compare events).
 * @note    - In one-pulse mode the counter stops by itself at the next update event,
 *            so calling this function is not required.
 * @note    - No parameter check is done. Passing NULL is undefined behavior.
 *
 */
void TIM_BaseStop(TIM_Handle_t* pTIMHandle)
{
	pTIMHandle->pTIMx->CR1 &=~ (1<< TIMx_CR1_CEN);
}

/**
 *
 * @fn      - TIM_BaseStart_IT
 *
 * @brief   - Enable the update interrupt and start the timer counter.
 *
 * @details - Sets the update interrupt enable bit (UIE) in DIER, then sets the
 *            counter enable bit (CEN) in CR1. An interrupt is raised on every
 *            update event, and the registered update callback is called from
 *            the timer's IRQ handler.
 *
 * @param[in] pTIMHandle - Timer handle. If NULL, the call does nothing.
 *
 * @note    - This function only sets UIE and CEN. It does not enable the timer's
 *            interrupt line in the NVIC, so that must be done separately using TIM_IRQEnable.
 * @note    - The update callback should be registered before calling this function,
 *            since the first update event can occur as soon as the counter starts.
 * @note    - Capture and compare interrupts are not enabled by this function.
 * @note    - If the update flag was already set before this call, an interrupt is
 *            raised immediately. TIM_BaseInit clears the flag, but a flag left set
 *            by an earlier run is not cleared here.
 * @note    - The interrupt is enabled before the counter starts, so no update event
 *            is missed.
 * @note    - In one-pulse mode the counter stops after the first update event, and
 *            this function must be called again for the next pulse.
 *
 */
void TIM_BaseStart_IT(TIM_Handle_t* pTIMHandle)
{
	if(pTIMHandle == NULL) return;

	//Enable interrupt on update events
	pTIMHandle->pTIMx->DIER |= (1<< TIMx_DIER_UIE);

	//Start timer
	pTIMHandle->pTIMx->CR1  |= (1<< TIMx_CR1_CEN);
}
/**
 *
 * @fn      - TIM_BaseStop_IT
 *
 * @brief   - Disable the update interrupt and stop the timer counter.
 *
 * @details - Clears the update interrupt enable bit (UIE) in DIER, then clears the
 *            counter enable bit (CEN) in CR1. The counter freezes at its current
 *            value, and no further update callbacks are called until
 *            TIM_BaseStart_IT is used again.
 *
 * @param[in] pTIMHandle - Timer handle. If NULL, call does nothing.
 *
 * @note    - Use this function to stop a timer that was started with TIM_BaseStart_IT.
 * @note    - This function does not disable the timer's interrupt line in the NVIC,
 *            reset CNT, clear status flags or remove the registered callbacks.
 * @note    - Capture and compare interrupt enables are not changed by this function.
 * @note    - An interrupt that is already pending can still enter the IRQ handler once,
 *            but the callback is not called because the interrupt enable bit is cleared.
 * @note    - TIM_BaseStart_IT continues counting from the value where the counter stopped.
 * @note    - In one-pulse mode the counter stops by itself at the next update event,
 *            but UIE stays set until this function is called.
 * @note    - No parameter check is done. Passing NULL is undefined behavior.
 *
 */
void TIM_BaseStop_IT(TIM_Handle_t* pTIMHandle)
{
	if(pTIMHandle == NULL) return;

	//Disable interrupt on update events
	pTIMHandle->pTIMx->DIER &=~ (1<< TIMx_DIER_UIE);

	//Stop timer
	pTIMHandle->pTIMx->CR1  &=~ (1<< TIMx_CR1_CEN);
}
/**
 *
 * @fn      - TIM_RegisterUpdateCallback
 *
 * @brief   - Register the callback for the timer's update event.
 *
 * @details - Stores the function pointer in the handle's UpdateCallback member.
 *            The callback is called from the timer's IRQ handler on every update
 *            event while the update interrupt is enabled.
 *
 * @param[in] pTIMHandle - Timer handle. Must not be NULL.
 * @param[in] Callback   - Function called on an update event. It receives the
 *                         handle of the timer that raised the event as argument.
 *                         Pass NULL as Callback to remove a callback.
 *
 * @note    - This function only stores the pointer. It does not enable the update
 *            interrupt, so use TIM_BaseStart_IT or enable it separately.
 * @note    - Call this after TIM_BaseInit, because TIM_BaseInit resets all callbacks to NULL.
 * @note    - The callback is called from ISR context. Keep it short and do not block.
 *
 */
void TIM_RegisterUpdateCallback(TIM_Handle_t* pTIMHandle, TIM_Callback_t Callback)
{
	pTIMHandle->UpdateCallback = Callback;
}

/**
 *
 * @fn      - TIM_OC_Init
 *
 * @brief   - Configure one output compare channel of a timer.
 *
 * @details - Validates the channel configuration, then programs the channel as an
 *            output: output mode (OCxM), output polarity (CCxP) and compare value
 *            (CCRx). The channel is selected by the Channel member of
 *            TIM_OC_Config_t, so each of the four channels can be configured
 *            separately on the same timer.
 *
 * @param[in] pTIMHandle - Timer handle. Must not be NULL.
 * @param[in] pOCHandle  - Output compare channel configuration. Must not be NULL.
 *
 * @return TIM_OK    - Channel configured.
 * @return TIM_ERROR - Invalid configuration. No register is modified in this case.
 *                     Causes: NULL pOCHandle, CompareValue above the timer Period,
 *                     invalid Channel, OutputMode or OutputPolarity value.
 *
 * @note    - Output compare has no interrupt or callback support in this driver. The
 *            compare match drives the output pin in hardware, so no ISR is needed.
 * @note    - Call TIM_BaseInit first. CompareValue is checked against the Period stored
 *            in pTIMHandle->TIM_Base, so Period must already be set in the handle.
 * @note    - CompareValue must not be greater than Period. A duty of 100% in PWM mode
 *            cannot be set with this function, because CompareValue equal to Period
 *            gives CompareValue / (Period + 1) in edge-aligned up-counting PWM1.
 * @note    - This function does not enable the channel output (CCxE). The pin is not
 *            driven until the output is enabled separately.Use TIM_OC_Start
 * @note    - The CCRx preload (OCxPE) is not enabled, so a new CompareValue takes
 *            effect immediately.
 * @note    - The channel output must be disabled while this function runs, because the
 *            channel direction bits (CCxS) can only be written while the channel is off.
 * @note    - This function does not start the timer or
 *            configure the GPIO pin.
 * @note    - All channels of a timer share the same timebase (PSC, ARR and counter mode).
 *
 */
TIM_Status_t TIM_OC_Init(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *pOCHandle)
{

	// < Check parameters > //
	/******************************************************************************/

	if(pOCHandle == NULL) return TIM_ERROR;
	if(pTIMHandle == NULL) return TIM_ERROR;

	if((pTIMHandle->pTIMx == TIM3 || pTIMHandle->pTIMx == TIM4)&&
		pOCHandle->CompareValue > 65535)
	{
		// TIM3 and TIM4 are 16bit
		return TIM_ERROR;
	}
	if( pOCHandle->CompareValue > pTIMHandle->TIM_Base.Period )
	{
		return TIM_ERROR;
	}
	if(     pOCHandle->Channel != TIM_CHANNEL_1   &&
			pOCHandle->Channel != TIM_CHANNEL_2   &&
			pOCHandle->Channel != TIM_CHANNEL_3   &&
			pOCHandle->Channel != TIM_CHANNEL_4)
	{
		return TIM_ERROR;
	}
	if(     pOCHandle->OutputMode != TIM_OCMODE_FROZEN   &&
			pOCHandle->OutputMode != TIM_OCMODE_ACTIVE   &&
			pOCHandle->OutputMode != TIM_OCMODE_INACTIVE &&
			pOCHandle->OutputMode != TIM_OCMODE_TOGGLE   &&
			pOCHandle->OutputMode != TIM_OCMODE_PWM1     &&
			pOCHandle->OutputMode != TIM_OCMODE_PWM2)
	{
		return TIM_ERROR;
	}
	if(     pOCHandle->OutputPolarity != TIM_OCPOLARITY_HIGH   &&
			pOCHandle->OutputPolarity != TIM_OCPOLARITY_LOW)
	{
		return TIM_ERROR;
	}


	// Set Channel configurations //
	/**************************************************************************************************/

	switch(pOCHandle->Channel)
	{
		case TIM_CHANNEL_1 :
								// Output selection (~ Clearing CC1S bits ~)
								pTIMHandle->pTIMx->CCMR1 &=~(0x3 << TIMx_CCMR1_CC1S);

								// Output Compare Mode Selection (~ Configuring OC1M bits ~)
								pTIMHandle->pTIMx->CCMR1 &=~(0x7 << TIMx_CCMR1_OC1M);
								pTIMHandle->pTIMx->CCMR1 |= (pOCHandle->OutputMode << TIMx_CCMR1_OC1M);

								// Output Polarity selection (~ Configuring CC1P bit ~)
								pTIMHandle->pTIMx->CCER &=~ (1<< TIMx_CCER_CC1P);
								pTIMHandle->pTIMx->CCER |= (pOCHandle->OutputPolarity << TIMx_CCER_CC1P);

								// Compare value setting (~ CCR1 Register ~)
								pTIMHandle->pTIMx->CCR1 = pOCHandle->CompareValue;


			                          break;
		case TIM_CHANNEL_2 :
								// Output selection (~ Clearing CC2S bits ~)
								pTIMHandle->pTIMx->CCMR1 &=~(0x3 << TIMx_CCMR1_CC2S);

								// Output Compare Mode Selection (~ Configuring OC2M bits ~)
								pTIMHandle->pTIMx->CCMR1 &=~(0x7 << TIMx_CCMR1_OC2M);
								pTIMHandle->pTIMx->CCMR1 |= (pOCHandle->OutputMode << TIMx_CCMR1_OC2M);

								// Output Polarity selection (~ Configuring CC1P bit ~)
								pTIMHandle->pTIMx->CCER &=~ (1<< TIMx_CCER_CC2P);
								pTIMHandle->pTIMx->CCER |= (pOCHandle->OutputPolarity << TIMx_CCER_CC2P);

								// Compare value setting (~ CCR2 Register ~)
								pTIMHandle->pTIMx->CCR2 = pOCHandle->CompareValue;

					                  break;
		case TIM_CHANNEL_3 :
								// Output selection (~ Clearing CC3S bits ~)
								pTIMHandle->pTIMx->CCMR2 &=~(0x3 << TIMx_CCMR2_CC3S);

								// Output Compare Mode Selection (~ Configuring OC3M bits ~)
								pTIMHandle->pTIMx->CCMR2 &=~(0x7 << TIMx_CCMR2_OC3M);
								pTIMHandle->pTIMx->CCMR2 |= (pOCHandle->OutputMode << TIMx_CCMR2_OC3M);

								// Output Polarity selection (~ Configuring CC1P bit ~)
								pTIMHandle->pTIMx->CCER &=~ (1<< TIMx_CCER_CC3P);
								pTIMHandle->pTIMx->CCER |= (pOCHandle->OutputPolarity << TIMx_CCER_CC3P);

								// Compare value setting (~ CCR3 Register ~)
								pTIMHandle->pTIMx->CCR3 = pOCHandle->CompareValue;
					                  break;
		case TIM_CHANNEL_4 :
								// Output selection (~ Clearing CC4S bits ~)
								pTIMHandle->pTIMx->CCMR2 &=~(0x3 << TIMx_CCMR2_CC4S);

								// Output Compare Mode Selection (~ Configuring OC4M bits ~)
								pTIMHandle->pTIMx->CCMR2 &=~(0x7 << TIMx_CCMR2_OC4M);
								pTIMHandle->pTIMx->CCMR2 |= (pOCHandle->OutputMode << TIMx_CCMR2_OC4M);

								// Output Polarity selection (~ Configuring CC1P bit ~)
								pTIMHandle->pTIMx->CCER &=~ (1<< TIMx_CCER_CC4P);
								pTIMHandle->pTIMx->CCER |= (pOCHandle->OutputPolarity << TIMx_CCER_CC4P);

								// Compare value setting (~ CCR4 Register ~)
								pTIMHandle->pTIMx->CCR4 = pOCHandle->CompareValue;

					                  break;
	}


	return TIM_OK;
}
/**
 *
 * @fn      - TIM_OC_SetCompareValue
 *
 * @brief   - Change the compare value of an output compare channel.
 *
 * @details - Writes CompareValue to the CCRx register of the channel given in
 *            TIM_OC_Config_t. It is used to change the compare point, for example
 *            the PWM duty cycle, after the channel has been configured with
 *            TIM_OC_Init.
 *
 * @param[in] pTIMHandle   - Timer handle. If NULL, the call does nothing
 * @param[in] pOCHandle    - Output compare channel configuration. Only the Channel
 *                           member is used. If NULL, the call does nothing
 * @param[in] CompareValue - New compare value, in timer ticks. Must not be greater
 *                           than the Period of the timer (and not above 0xFFFF on
 *                           TIM3 and TIM4).
 *
 * @note    - If a pointer is NULL, CompareValue is out of range or Channel is invalid, nothing is written
 *            and no error is reported.
 * @note    - The CompareValue member of pOCHandle is updated.
 * @note    - It can be called while the timer and the channel are running.
 * @note    - CCRx preload is not enabled, so the new value takes effect immediately.
 *            Changing it in the middle of a period can give a glitched PWM period.
 * @note    - A duty of 100% in PWM mode cannot be set, because CompareValue equal to
 *            Period gives CompareValue / (Period + 1) in edge-aligned up-counting PWM1.
 * @note    - This function does not enable the channel output. Use TIM_OC_Start.
 *
 */
void TIM_OC_SetCompareValue(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *pOCHandle, uint32_t CompareValue)
{
	// check structure pointers //
	if(pTIMHandle == NULL || pOCHandle == NULL) return;

	// check compare value //
	if((pTIMHandle->pTIMx == TIM3 || pTIMHandle->pTIMx == TIM4)&& CompareValue > 0xFFFF)
	{
		// TIM3 and TIM4 are 16bit
		return;
	}
	if(CompareValue > pTIMHandle->TIM_Base.Period)
	{
		return;
	}

	// set compare value

	switch(pOCHandle->Channel)
		{
			case TIM_CHANNEL_1 :
									// Compare value setting (~ CCR1 Register ~)
									pTIMHandle->pTIMx->CCR1 = CompareValue;
									// Update the CompareValue member of OC_Config_t
									pOCHandle->CompareValue = CompareValue;

				                          break;
			case TIM_CHANNEL_2 :
									// Compare value setting (~ CCR2 Register ~)
									pTIMHandle->pTIMx->CCR2 = CompareValue;
									// Update the CompareValue member of OC_Config_t
									pOCHandle->CompareValue = CompareValue;

						                  break;
			case TIM_CHANNEL_3 :
									// Compare value setting (~ CCR3 Register ~)
									pTIMHandle->pTIMx->CCR3 = CompareValue;
									// Update the CompareValue member of OC_Config_t
									pOCHandle->CompareValue = CompareValue;
						                  break;
			case TIM_CHANNEL_4 :
				                    // Compare value setting (~ CCR4 Register ~)
									pTIMHandle->pTIMx->CCR4 = CompareValue;
									// Update the CompareValue member of OC_Config_t
									pOCHandle->CompareValue = CompareValue;

						                  break;

			default            : return;

		}

}
/**
 *
 * @fn      - TIM_OC_Start
 *
 * @brief   - Enable the output of an output compare channel.
 *
 * @details - Sets the capture/compare enable bit (CCxE) in CCER for the channel
 *            given in TIM_OC_Config_t. Once enabled, the compare match drives the
 *            channel's output pin according to the output mode and polarity set by
 *            TIM_OC_Init.
 *
 * @param[in] pTIMHandle - Timer handle. If NULL, call does nothing.
 * @param[in] pOCHandle  - Output compare channel configuration. Only the Channel
 *                         member is used.If NULL, call does nothing.
 *
 * @note    - Call TIM_OC_Init for the channel first.
 * @note    - This function does not start the counter. Use TIM_BaseStart, which can
 *            be called before or after this function.
 * @note    - Use TIM_OC_Stop to disable the channel output.
 * @note    - The GPIO pin must be configured separately (alternate function) for the
 *            signal to appear on the pin.
 * @note    - No error is reported for an invalid Channel value. The call does nothing.
 * @note    - No parameter check is done. Passing NULL as either pointer is undefined behavior.
 *
 */
void TIM_OC_Start(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *pOCHandle)
{
	if(pTIMHandle == NULL) return;
	if(pOCHandle == NULL) return;


	switch(pOCHandle->Channel)
		{
			case TIM_CHANNEL_1 :
									// Enable channel l (~ Setting CC1E bit ~)
									pTIMHandle->pTIMx->CCER |= (1 << TIMx_CCER_CC1E);

				                    break;
			case TIM_CHANNEL_2 :
									// Enable channel 2 (~ Setting CC2E bit ~)
									pTIMHandle->pTIMx->CCER |= (1 << TIMx_CCER_CC2E);

								    break;
			case TIM_CHANNEL_3 :
									// Enable channel 3 (~ Setting CC3E bit ~)
									pTIMHandle->pTIMx->CCER |= (1 << TIMx_CCER_CC3E);

									break;
			case TIM_CHANNEL_4 :
									// Enable channel 4(~ Setting CC4E bit ~)
									pTIMHandle->pTIMx->CCER |= (1 << TIMx_CCER_CC4E);

								    break;
		}


}

/**
 *
 * @fn      - TIM_OC_Stop
 *
 * @brief   - Disable the output of an output compare channel.
 *
 * @details - Clears the capture/compare enable bit (CCxE) in CCER for the channel
 *            given in TIM_OC_Config_t. The timer stops driving the channel's output
 *            pin. The channel configuration (mode, polarity and compare value) is
 *            kept, so the output can be enabled again with TIM_OC_Start.
 *
 * @param[in] pTIMHandle - Timer handle. If NULL, call does nothing.
 * @param[in] pOCHandle  - Output compare channel configuration. Only the Channel
 *                         member is used. If NULL, call does nothing.
 *
 * @note    - This function does not stop the counter. Other channels and the
 *            update event are not affected. Use TIM_BaseStop to stop the counter.
 * @note    - The level of the pin while the channel is disabled is not set by this
 *            function. It depends on the output polarity and the GPIO configuration.
 * @note    - Call this function before TIM_OC_Init if the channel has to be
 *            reconfigured, since the channel direction bits (CCxS) can only be
 *            written while the channel is disabled.
 *
 */
void TIM_OC_Stop(TIM_Handle_t *pTIMHandle, TIM_OC_Config_t *pOCHandle)
{
     	if(pTIMHandle == NULL) return;
		if(pOCHandle == NULL) return;


	switch(pOCHandle->Channel)
			{
				case TIM_CHANNEL_1 :
										// Disable channel (~ Clearing CC1E bit ~)
										pTIMHandle->pTIMx->CCER &=~(1 << TIMx_CCER_CC1E);

					                    break;
				case TIM_CHANNEL_2 :
										// Disable channe2 (~ Clearing CC2E bit ~)
										pTIMHandle->pTIMx->CCER &=~(1 << TIMx_CCER_CC2E);

									    break;
				case TIM_CHANNEL_3 :
										// Disable channe3 (~ Clearing CC3E bit ~)
										pTIMHandle->pTIMx->CCER &=~ (1 << TIMx_CCER_CC3E);

										break;
				case TIM_CHANNEL_4 :
										// Disable channel (~ Clearing CC4E bit ~)
										pTIMHandle->pTIMx->CCER &=~(1 << TIMx_CCER_CC4E);

									    break;
			}

}
/**
 *
 * @fn      - TIM_IC_Init
 *
 * @brief   - Configure one input capture channel of a timer.
 *
 * @details - Validates the channel configuration, then programs the channel as an
 *            input: input mapping (CCxS), input filter (ICxF), capture prescaler
 *            (ICxPSC) and active edge (CCxP and CCxNP). The channel is selected by
 *            the Channel member of TIM_IC_Config_t, so each of the four channels
 *            can be configured separately on the same timer.
 *
 * @param[in] pTIMHandle - Timer handle. Must not be NULL.
 * @param[in] pICConfig  - Input capture channel configuration. Must not be NULL.
 *
 * @return TIM_OK    - Channel configured.
 * @return TIM_ERROR - Invalid configuration. No register is modified in this case.
 *                     Causes: NULL pICConfig, invalid Channel, InputMode,
 *                     InputPolarity or Prescaler value, or Filtering above 15.
 *
 * @note    - Call TIM_BaseInit first, because it enables the timer clock.
 * @note    - This function does not enable the channel, enable a capture interrupt,
 *            register a callback or start the timer.
 * @note    - The GPIO pin must be configured separately (alternate function) for
 *            the signal to reach the timer input.
 * @note    - The channel must be disabled while this function runs, because the
 *            input mapping bits (CCxS) can only be written while the channel is off.
 *            Use TIM_IC_Stop before calling TIM_IC_Init again.
 * @note    - In indirect mode, a channel captures from the input of its paired channel
 *            (CH1 and CH2 are a pair, CH3 and CH4 are a pair). The paired channel
 *            is not configured by this function.
 * @note    - TRC mode works only if an internal trigger input is selected through the
 *            TS bits, which this function does not configure.
 * @note    - The capture prescaler (Prescaler member) divides the input events and is
 *            separate from the timebase prescaler.
 * @note    - All channels of a timer share the same timebase (PSC, ARR and counter mode).
 *
 */
TIM_Status_t TIM_IC_Init(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig)
{
	// < Check Parameters > //
	/******************************************************************************/

	if(pICConfig == NULL) return TIM_ERROR;
	if(pTIMHandle == NULL) return TIM_ERROR;

	if(     pICConfig->Channel != TIM_CHANNEL_1   &&
			pICConfig->Channel != TIM_CHANNEL_2   &&
			pICConfig->Channel != TIM_CHANNEL_3   &&
			pICConfig->Channel != TIM_CHANNEL_4)
	{
		    return TIM_ERROR;
	}
	if( pICConfig->InputMode != TIM_ICMODE_DIRECT &&
		pICConfig->InputMode != TIM_ICMODE_INDIRECT &&
		pICConfig->InputMode != TIM_ICMODE_TRC )
		{
			return TIM_ERROR;
		}
	if( pICConfig->InputPolarity != TIM_ICPOLARITY_RISING &&
		pICConfig->InputPolarity != TIM_ICPOLARITY_FALLING &&
		pICConfig->InputPolarity != TIM_ICPOLARITY_BOTHEDGE)
		{
			return TIM_ERROR;
		}
	if( pICConfig->Prescaler != TIM_ICPSC_DIV1 &&
		pICConfig->Prescaler != TIM_ICPSC_DIV2 &&
		pICConfig->Prescaler != TIM_ICPSC_DIV4 &&
		pICConfig->Prescaler != TIM_ICPSC_DIV8)
		{
			return TIM_ERROR;
		}
	if(pICConfig->Filtering > TIM_ICFILTER_15) {return TIM_ERROR;}



	// Set Channel configurations //
	/**************************************************************************************************/

	switch(pICConfig->Channel)
	{
		case TIM_CHANNEL_1 :
								// Input Mode selection (~ Configuring CC1S bits ~)
								pTIMHandle->pTIMx->CCMR1 &=~(0x3 << TIMx_CCMR1_CC1S);
								pTIMHandle->pTIMx->CCMR1 |= (pICConfig->InputMode << TIMx_CCMR1_CC1S);

								// Input Filtering (~ Configuring IC1F bits ~)
								pTIMHandle->pTIMx->CCMR1 &=~ (0xF<< TIMx_CCMR1_IC1F);
								pTIMHandle->pTIMx->CCMR1 |= (pICConfig->Filtering << TIMx_CCMR1_IC1F);

								// Input Capture prescaler (~ Configuring IC1PSC bits ~)
								pTIMHandle->pTIMx->CCMR1 &=~ (0x3 << TIMx_CCMR1_IC1PSC);
								pTIMHandle->pTIMx->CCMR1 |= (pICConfig->Prescaler << TIMx_CCMR1_IC1PSC);

								// Set Polarity, Edge selection (~ Configuring CC1P and CCN1P bits~)
								if(pICConfig->InputPolarity == TIM_ICPOLARITY_RISING)
								{
									// CCN1P:CC1P -> 00
									pTIMHandle->pTIMx->CCER &=~ ((1<< TIMx_CCER_CCN1P)|(1<< TIMx_CCER_CC1P));
								}
								else if(pICConfig->InputPolarity == TIM_ICPOLARITY_FALLING)
								{
									// CCN1P:CC1P -> 01
									pTIMHandle->pTIMx->CCER &=~ ((1<< TIMx_CCER_CCN1P));
									pTIMHandle->pTIMx->CCER |=  (1<< TIMx_CCER_CC1P);
								}
								else if(pICConfig->InputPolarity == TIM_ICPOLARITY_BOTHEDGE)
								{
									// CCN1P:CC1P -> 11
								    pTIMHandle->pTIMx->CCER |= ((1<< TIMx_CCER_CCN1P)|(1<< TIMx_CCER_CC1P));
								}

			                          break;

		case TIM_CHANNEL_2 :
								    // Input Mode selection (~ Configuring CC2S bits ~)
									pTIMHandle->pTIMx->CCMR1 &=~(0x3 << TIMx_CCMR1_CC2S);
									pTIMHandle->pTIMx->CCMR1 |= (pICConfig->InputMode << TIMx_CCMR1_CC2S);

									// Input Filtering (~ Configuring IC2F bits ~)
									pTIMHandle->pTIMx->CCMR1 &=~ (0xF<< TIMx_CCMR1_IC2F);
									pTIMHandle->pTIMx->CCMR1 |= (pICConfig->Filtering << TIMx_CCMR1_IC2F);

									// Input Capture prescaler (~ Configuring IC2PSC bits ~)
									pTIMHandle->pTIMx->CCMR1 &=~ (0x3 << TIMx_CCMR1_IC2PSC);
									pTIMHandle->pTIMx->CCMR1 |= (pICConfig->Prescaler << TIMx_CCMR1_IC2PSC);

									// Set Polarity, Edge selection (~ Configuring CC2P and CCN2P bits~)
									if(pICConfig->InputPolarity == TIM_ICPOLARITY_RISING)
									{
											// CCN2P:CC2P -> 00
											pTIMHandle->pTIMx->CCER &=~ ((1<< TIMx_CCER_CCN2P)|(1<< TIMx_CCER_CC2P));
									}
									else if(pICConfig->InputPolarity == TIM_ICPOLARITY_FALLING)
									{
										// CCN2P:CC2P -> 01
										pTIMHandle->pTIMx->CCER &=~ ((1<< TIMx_CCER_CCN2P));
										pTIMHandle->pTIMx->CCER |=  (1<< TIMx_CCER_CC2P);
									}
									else if(pICConfig->InputPolarity == TIM_ICPOLARITY_BOTHEDGE)
									{
										// CCN2P:CC2P -> 11
									    pTIMHandle->pTIMx->CCER |= ((1<< TIMx_CCER_CCN2P)|(1<< TIMx_CCER_CC2P));
									}



					                  break;


		case TIM_CHANNEL_3 :
			                		// Input Mode selection (~ Configuring CC3S bits ~)
									pTIMHandle->pTIMx->CCMR2 &=~(0x3 << TIMx_CCMR2_CC3S);
									pTIMHandle->pTIMx->CCMR2 |= (pICConfig->InputMode << TIMx_CCMR2_CC3S);

									// Input Filtering (~ Configuring IC3F bits ~)
									pTIMHandle->pTIMx->CCMR2 &=~ (0xF<< TIMx_CCMR2_IC3F);
									pTIMHandle->pTIMx->CCMR2 |= (pICConfig->Filtering << TIMx_CCMR2_IC3F);

									// Input Capture prescaler (~ Configuring IC3PSC bits ~)
									pTIMHandle->pTIMx->CCMR2 &=~ (0x3 << TIMx_CCMR2_IC3PSC);
									pTIMHandle->pTIMx->CCMR2 |= (pICConfig->Prescaler << TIMx_CCMR2_IC3PSC);

									// Set Polarity, Edge selection (~ Configuring CC3P and CCN3P bits~)
									if(pICConfig->InputPolarity == TIM_ICPOLARITY_RISING)
									{
												// CCN3P:CC3P -> 00
												pTIMHandle->pTIMx->CCER &=~ ((1<< TIMx_CCER_CCN3P)|(1<< TIMx_CCER_CC3P));
									}
									else if(pICConfig->InputPolarity == TIM_ICPOLARITY_FALLING)
									{
													// CCN3P:CC3P -> 01
										pTIMHandle->pTIMx->CCER &=~ ((1<< TIMx_CCER_CCN3P));
										pTIMHandle->pTIMx->CCER |=  (1<< TIMx_CCER_CC3P);
									}
									else if(pICConfig->InputPolarity == TIM_ICPOLARITY_BOTHEDGE)
									{
												// CCN3P:CC3P -> 11
										pTIMHandle->pTIMx->CCER |= ((1<< TIMx_CCER_CCN3P)|(1<< TIMx_CCER_CC3P));
									}

					                break;
		case TIM_CHANNEL_4 :
			                        // Input Mode selection (~ Configuring CC4S bits ~)
									pTIMHandle->pTIMx->CCMR2 &=~(0x3 << TIMx_CCMR2_CC4S);
									pTIMHandle->pTIMx->CCMR2 |= (pICConfig->InputMode << TIMx_CCMR2_CC4S);

									// Input Filtering (~ Configuring IC4F bits ~)
									pTIMHandle->pTIMx->CCMR2 &=~ (0xF<< TIMx_CCMR2_IC4F);
									pTIMHandle->pTIMx->CCMR2 |= (pICConfig->Filtering << TIMx_CCMR2_IC4F);

									// Input Capture prescaler (~ Configuring IC4PSC bits ~)
									pTIMHandle->pTIMx->CCMR2 &=~ (0x3 << TIMx_CCMR2_IC4PSC);
									pTIMHandle->pTIMx->CCMR2 |= (pICConfig->Prescaler << TIMx_CCMR2_IC4PSC);

									// Set Polarity, Edge selection (~ Configuring CC4P and CCN4P bits~)
									if(pICConfig->InputPolarity == TIM_ICPOLARITY_RISING)
									{
										// CCN4P:CC4P -> 00
										pTIMHandle->pTIMx->CCER &=~ ((1<< TIMx_CCER_CCN4P)|(1<< TIMx_CCER_CC4P));
									}
									else if(pICConfig->InputPolarity == TIM_ICPOLARITY_FALLING)
									{
										// CCN4P:CC4P -> 01
										pTIMHandle->pTIMx->CCER &=~ ((1<< TIMx_CCER_CCN4P));
										pTIMHandle->pTIMx->CCER |=  (1<< TIMx_CCER_CC4P);
									}
									else if(pICConfig->InputPolarity == TIM_ICPOLARITY_BOTHEDGE)
									{
										// CCN4P:CC4P -> 11
										pTIMHandle->pTIMx->CCER |= ((1<< TIMx_CCER_CCN4P)|(1<< TIMx_CCER_CC4P));
									}


					                break;
	}

	return TIM_OK;

}

/**
 *
 * @fn      - TIM_IC_GetCaptureValue
 *
 * @brief   - Read the captured value of an input capture channel.
 *
 * @details - Returns the CCRx register of the channel given in TIM_IC_Config_t.
 *            After a capture event, CCRx holds the counter value at the moment
 *            the active edge was detected.
 *
 * @param[in] pTIMHandle - Timer handle. If NULL, the function returns 0.
 * @param[in] pICConfig  - Input capture channel configuration. Only the Channel
 *                         member is used. If NULL, the function returns 0.
 *
 * @return  - The captured value in timer ticks, or 0 if a pointer is NULL or
 *            Channel is invalid. 0 is also a valid captured value, so it cannot
 *            be used to detect an error.
 *
 * @note    - The value is 32-bit on TIM2 and TIM5, and 16-bit on TIM3 and TIM4.
 * @note    - Reading CCRx clears the capture flag (CCxIF) of the channel.
 * @note    - The value is the raw counter value at the capture. To get a period or
 *            a pulse width, subtract two captured values, and handle a counter
 *            overflow between the two captures in the application.
 * @note    - It can be called from the capture callback, which runs in ISR context.
 * @note    - This function does not check whether the channel is configured as an
 *            input. Reading an output compare channel returns its compare value.
 *
 */
uint32_t TIM_IC_GetCaptureValue(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig)
{
	// check structure pointers //
	if(pTIMHandle == NULL || pICConfig == NULL) return 0;

	// Get compare value

	switch(pICConfig->Channel)
	{
		case TIM_CHANNEL_1 :
										// Compare value (~ CCR1 Register ~)
										return pTIMHandle->pTIMx->CCR1;
		case TIM_CHANNEL_2 :
			                            // Compare value (~ CCR2 Register ~)
										return pTIMHandle->pTIMx->CCR2;
		case TIM_CHANNEL_3 :
			                            // Compare value (~ CCR3 Register ~)
										return pTIMHandle->pTIMx->CCR3;
		case TIM_CHANNEL_4 :
			                            // Compare value (~ CCR4 Register ~)
										return pTIMHandle->pTIMx->CCR4;
		default            :            return 0;


	}
}

/**
 *
 * @fn      - TIM_IC_Start
 *
 * @brief   - Enable the input capture of a channel.
 *
 * @details - Sets the capture/compare enable bit (CCxE) in CCER for the channel
 *            given in TIM_IC_Config_t. Once enabled, each active edge on the input
 *            copies the counter value into CCRx and sets the capture flag (CCxIF).
 *
 * @param[in] pTIMHandle - Timer handle. If NULL, call does nothing
 * @param[in] pICConfig  - Input capture channel configuration. Only the Channel
 *                         member is used. If NULL, call does nothing
 *
 * @note    - Call TIM_IC_Init for the channel first.
 * @note    - This function does not start the counter. Use TIM_BaseStart, which can
 *            be called before or after this function.
 * @note    - This function does not enable the capture interrupt, so no callback is
 *            called. Captures are detected by polling the capture flag with
 *            TIM_GetStatus, and the value is read with TIM_IC_GetCaptureValue
 *            Use TIM_IC_Start_IT to capture with an interrupt.
 * @note    - The GPIO pin must be configured separately (alternate function) for the
 *            signal to reach the timer input.
 * @note    - If the capture flag was already set before this call, it stays set until
 *            it is cleared with TIM_ClearFlag or by reading CCRx.
 *
 */
void TIM_IC_Start(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig)
{
	// check pointers
	if(pTIMHandle == NULL || pICConfig == NULL) return;

	switch(pICConfig->Channel)
			{
				case TIM_CHANNEL_1 :
										// Enable channell (~ Setting CC1E bit ~)
										pTIMHandle->pTIMx->CCER |= (1 << TIMx_CCER_CC1E);

					                    break;
				case TIM_CHANNEL_2 :
										// Enable channel2 (~ Setting CC2E bit ~)
										pTIMHandle->pTIMx->CCER |= (1 << TIMx_CCER_CC2E);

									    break;
				case TIM_CHANNEL_3 :
										// Enable channel3 (~ Setting CC3E bit ~)
										pTIMHandle->pTIMx->CCER |= (1 << TIMx_CCER_CC3E);

										break;
				case TIM_CHANNEL_4 :
										// Enable channel4 (~ Setting CC4E bit ~)
										pTIMHandle->pTIMx->CCER |= (1 << TIMx_CCER_CC4E);

									    break;
			}

}
/**
 *
 * @fn      - TIM_IC_Stop
 *
 * @brief   - Disable the input capture of a channel.
 *
 * @details - Clears the capture/compare enable bit (CCxE) in CCER for the channel
 *            given in TIM_IC_Config_t. No further captures happen on the channel.
 *            The channel configuration (input mapping, filter, prescaler and edge)
 *            is kept, so the capture can be enabled again with TIM_IC_Start.
 *
 * @param[in] pTIMHandle - Timer handle. If NULL, the call does nothing.
 * @param[in] pICConfig  - Input capture channel configuration. Only the Channel
 *                         member is used. If NULL, the call does nothing.
 *
 * @note    - This function does not stop the counter. Other channels and the
 *            update event are not affected. Use TIM_BaseStop to stop the counter.
 * @note    - The capture interrupt enable bit is not changed by this function. Use
 *            TIM_IC_Stop_IT if the interrupt was enabled.
 * @note    - CCRx keeps the last captured value, which can still be read with
 *            TIM_IC_GetCatureValue.
 * @note    - A capture flag that is already set stays set until it is cleared with
 *            TIM_ClearFlag or by reading CCRx.
 * @note    - Call this function before TIM_IC_Init if the channel has to be
 *            reconfigured, since the input mapping bits (CCxS) can only be
 *            written while the channel is disabled.
 * @note    - No error is reported for an invalid Channel value. The call does nothing.
 *
 */
void TIM_IC_Stop(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig)
{
	if(pTIMHandle == NULL || pICConfig == NULL) return;

	switch(pICConfig->Channel)
				{
					case TIM_CHANNEL_1 :
											// Disable channel (~ Clearing CC1E bit ~)
											pTIMHandle->pTIMx->CCER &=~(1 << TIMx_CCER_CC1E);

						                    break;
					case TIM_CHANNEL_2 :
											// Disable channe2 (~ Clearing CC2E bit ~)
											pTIMHandle->pTIMx->CCER &=~(1 << TIMx_CCER_CC2E);

										    break;
					case TIM_CHANNEL_3 :
											// Disable channe3 (~ Clearing CC3E bit ~)
											pTIMHandle->pTIMx->CCER &=~ (1 << TIMx_CCER_CC3E);

											break;
					case TIM_CHANNEL_4 :
											// Disable channel (~ Clearing CC4E bit ~)
											pTIMHandle->pTIMx->CCER &=~(1 << TIMx_CCER_CC4E);

										    break;
				}


}

/**
 *
 * @fn      - TIM_IC_Start_IT
 *
 * @brief   - Enable the input capture of a channel with its capture interrupt.
 *
 * @details - Sets the capture/compare enable bit (CCxE) in CCER and the capture
 *            interrupt enable bit (CCxIE) in DIER for the channel given in
 *            TIM_IC_Config_t. Each active edge on the input copies the counter value
 *            into CCRx and raises an interrupt, and the callback registered for the
 *            channel is called from the timer's IRQ handler.
 *
 * @param[in] pTIMHandle - Timer handle. If NULL, the call does nothing.
 * @param[in] pICConfig  - Input capture channel configuration. Only the Channel
 *                         member is used. If NULL, the call does nothing.
 *
 * @note    - Call TIM_IC_Init for the channel first.
 * @note    - Register the channel's capture callback before calling this function,
 *            since the first capture can occur as soon as the counter runs.
 * @note    - This function does not start the counter. Use TIM_BaseStart, which can
 *            be called before or after this function.
 * @note    - This function does not enable the timer's interrupt line in the NVIC,
 *            so that must be done separately using TIM_IRQEnable().
 * @note    - Only the interrupt of the selected channel is enabled. Other channels and
 *            the update interrupt are not changed.
 * @note    - The callback is called from ISR context. Keep it short and do not block.
 * @note    - If the capture flag was already set before this call, an interrupt is
 *            raised immediately.
 * @note    - The GPIO pin must be configured separately (alternate function) for the
 *            signal to reach the timer input.
 *
 */

void TIM_IC_Start_IT(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig)
{

	if(pTIMHandle == NULL || pICConfig == NULL) return;

	switch(pICConfig->Channel)
				{
					case TIM_CHANNEL_1 :
											// Enable channel (~ Setting CC1E bit ~)
											pTIMHandle->pTIMx->CCER |= (1 << TIMx_CCER_CC1E);

											// Enable CC1 interrupt
											pTIMHandle->pTIMx->DIER |= ( 1<< TIMx_DIER_CC1IE);


						                    break;
					case TIM_CHANNEL_2 :
											// Enable channe2 (~ Setting CC2E bit ~)
											pTIMHandle->pTIMx->CCER |= (1 << TIMx_CCER_CC2E);

											// Enable CC2 interrupt
											pTIMHandle->pTIMx->DIER |= ( 1<< TIMx_DIER_CC2IE);

										    break;
					case TIM_CHANNEL_3 :
											// Enable channe3 (~ Setting CC3E bit ~)
											pTIMHandle->pTIMx->CCER |= (1 << TIMx_CCER_CC3E);

											// Enable CC3 interrupt
											pTIMHandle->pTIMx->DIER |= ( 1<< TIMx_DIER_CC3IE);

											break;
					case TIM_CHANNEL_4 :
											// Enable channel (~ Setting CC4E bit ~)
											pTIMHandle->pTIMx->CCER |= (1 << TIMx_CCER_CC4E);

											// Enable CC4 interrupt
											pTIMHandle->pTIMx->DIER |= ( 1<< TIMx_DIER_CC4IE);
										    break;
				}


}

/**
 *
 * @fn      - TIM_IC_Stop_IT
 *
 * @brief   - Disable the input capture of a channel together with its capture interrupt.
 *
 * @details - Clears the capture/compare enable bit (CCxE) in CCER and the capture
 *            interrupt enable bit (CCxIE) in DIER for the channel given in
 *            TIM_IC_Config_t. No further captures happen on the channel, and its
 *            callback is no longer called. The channel configuration is kept, so the
 *            capture can be enabled again with TIM_IC_Start_IT.
 *
 * @param[in] pTIMHandle - Timer handle. If NULL, the call does nothing.
 * @param[in] pICConfig  - Input capture channel configuration. Only the Channel
 *                         member is used. If NULL, the call does nothing.
 *
 * @note    - Use this function to stop a channel started with TIM_IC_Start_IT.
 *            TIM_IC_Stop leaves CCxIE set, so the interrupt would still be raised
 *            if the channel is started again.
 * @note    - This function does not stop the counter. Other channels and the
 *            update interrupt are not affected. Use TIM_BaseStop to stop the counter.
 * @note    - This function does not disable the timer's interrupt line in the NVIC,
 *            remove the registered callback or clear status flags.
 * @note    - CCRx keeps the last captured value, which can still be read with
 *            TIM_IC_GetCaptureeValue.
 * @note    - An interrupt that is already pending can still enter the IRQ handler once,
 *            but the callback is not called because the interrupt enable bit is cleared.
 * @note    - Call this function before TIM_IC_Init if the channel has to be
 *            reconfigured, since the input mapping bits (CCxS) can only be
 *            written while the channel is disabled.
 *
 */
void TIM_IC_Stop_IT(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig)
{
	if(pICConfig == NULL || pTIMHandle == NULL) return;

	switch(pICConfig->Channel)
					{
						case TIM_CHANNEL_1 :
												// Disable channel (~ Clearing CC1E bit ~)
												pTIMHandle->pTIMx->CCER &=~(1 << TIMx_CCER_CC1E);

												// Disable CC1 interrupt
												pTIMHandle->pTIMx->DIER &=~ ( 1<< TIMx_DIER_CC1IE);

							                    break;
						case TIM_CHANNEL_2 :
												// Disable channe2 (~ Clearing CC2E bit ~)
												pTIMHandle->pTIMx->CCER &=~(1 << TIMx_CCER_CC2E);

												// Disable CC2 interrupt
												pTIMHandle->pTIMx->DIER &=~ ( 1<< TIMx_DIER_CC2IE);

											    break;
						case TIM_CHANNEL_3 :
												// Disable channe3 (~ Clearing CC3E bit ~)
												pTIMHandle->pTIMx->CCER &=~ (1 << TIMx_CCER_CC3E);

												// Disable CC3 interrupt
												pTIMHandle->pTIMx->DIER &=~ ( 1<< TIMx_DIER_CC3IE);

												break;
						case TIM_CHANNEL_4 :
												// Disable channel (~ Clearing CC4E bit ~)
												pTIMHandle->pTIMx->CCER &=~(1 << TIMx_CCER_CC4E);

												// Disable CC4 interrupt
												pTIMHandle->pTIMx->DIER &=~ ( 1<< TIMx_DIER_CC4IE);

											    break;
					}

}

/**
 *
 * @fn      - TIM_IC_RegisterCallback
 *
 * @brief   - Register the callback for the capture event of an input capture channel.
 *
 * @details - Stores the function pointer in the handle's IC_Callback entry of the
 *            channel given in TIM_IC_Config_t (CH1 to IC_Callback[0], up to CH4 to
 *            IC_Callback[3]). The callback is called from the timer's IRQ handler on
 *            every capture event of that channel while its capture interrupt is
 *            enabled. Each channel has its own callback, so all four channels of a
 *            timer can be used at the same time.
 *
 * @param[in] pTIMHandle - Timer handle. If NULL, the call does nothing.
 * @param[in] pICConfig  - Input capture channel configuration. Only the Channel
 *                         member is used. If NULL, the call does nothing.
 * @param[in] Callback   - Function called on a capture event of the channel. It
 *                         receives the handle of the timer that raised the event as argument.
 *                         Pass NULL as Callback to remove a callback.
 *
 * @note    - This function only stores the pointer. It does not enable the capture
 *            interrupt, so use TIM_IC_Start_IT.
 * @note    - Call this after TIM_BaseInit, because TIM_BaseInit resets all callbacks
 *            to NULL, and before TIM_IC_Start_IT, because the first capture can occur
 *            as soon as the counter runs.
 * @note    - The callback does not receive the channel number. To tell channels apart,
 *            register a separate function for each channel.
 * @note    - Read the captured value inside the callback with TIM_IC_GetCaptureValue.
 * @note    - The callback is called from ISR context. Keep it short and do not block.
 * @note    - Registering again replaces the previous callback of that channel.
 * @note    - The update event callback is registered with TIM_RegisterUpdateCallback.
 *
 */
void TIM_IC_RegisterCallback(TIM_Handle_t *pTIMHandle, TIM_IC_Config_t *pICConfig, TIM_Callback_t Callback)
{
	if(pICConfig == NULL || pTIMHandle == NULL) return;

	switch(pICConfig->Channel)
	{
		case TIM_CHANNEL_1 : pTIMHandle->IC_Callback[0] = Callback;
								break;
		case TIM_CHANNEL_2 : pTIMHandle->IC_Callback[1] = Callback;
							    break;
		case TIM_CHANNEL_3 : pTIMHandle->IC_Callback[2] = Callback;
							    break;
		case TIM_CHANNEL_4 : pTIMHandle->IC_Callback[3] = Callback;
							    break;
	}
}

/**
 *
 * @fn      - TIM_IRQEnable
 *
 * @brief   - Enable the timer's interrupt line in the NVIC.
 *
 * @details - Sets the NVIC interrupt set-enable bit of the timer given in the handle
 *            (TIM2, TIM3, TIM4 or TIM5). Each timer has one interrupt line shared by
 *            its update and capture events, so enabling it is done per timer and not
 *            per event.
 *
 * @param[in] pTIMHandle - Timer handle. If NULL, the call does nothing.
 *
 * @return TIM_OK    - Interrupt line enabled.
 * @return TIM_ERROR - pTIMx is not TIM2, TIM3, TIM4 or TIM5. No register is modified.
 *
 * @note    - Call TIM_BaseInit first, because the IRQ handler uses the handle pointer
 *            stored by TIM_BaseInit to call the callbacks.
 * @note    - Register the callbacks before calling this function.
 * @note    - This function only enables the line in the NVIC. The update interrupt and
 *            the capture interrupts are enabled in the timer by TIM_BaseStart_IT and
 *            TIM_IC_Start_IT, and no interrupt occurs until one of them is enabled.
 * @note    - The interrupt priority is not changed by this function. Use TIM_IRQPriorityConfig to set it
 * @note    - If a timer interrupt is enabled and its flag is already set, the
 *            interrupt runs as soon as the line is enabled.
 *
 */
TIM_Status_t TIM_IRQEnable(TIM_Handle_t* pTIMHandle)
{
	if(pTIMHandle == NULL) return TIM_ERROR;

	if(pTIMHandle->pTIMx == TIM2 )
	{
		NVIC->ISER[TIM2_IRQ_ISER_INDEX] |=  TIM2_IRQ_ENABLE;
	}
	else if(pTIMHandle->pTIMx == TIM3 )
	{
		NVIC->ISER[TIM3_IRQ_ISER_INDEX] |=  TIM3_IRQ_ENABLE;
	}
	else if(pTIMHandle->pTIMx == TIM4 )
	{
		NVIC->ISER[TIM4_IRQ_ISER_INDEX] |=  TIM4_IRQ_ENABLE;
	}
	else if(pTIMHandle->pTIMx == TIM5 )
	{
		NVIC->ISER[TIM5_IRQ_ISER_INDEX] |=  TIM5_IRQ_ENABLE;
	}
	else{
		return TIM_ERROR;
	}
	return TIM_OK;
}

/**
 *
 * @fn      - TIM_IRQPriorityConfig
 *
 * @brief   - Set the NVIC interrupt priority of a timer.
 *
 * @details - Writes the priority into the NVIC priority register of the timer given in
 *            the handle (TIM2, TIM3, TIM4 or TIM5). Each timer has one interrupt line
 *            shared by its update and capture events, so the priority applies to all
 *            of them.
 *
 * @param[in] pTIMHandle - Timer handle. If NULL, the function returns TIM_ERROR.
 * @param[in] Priority   - Priority level, 0 to 15. 0 is the highest priority. The
 *                         STM32F401 implements 4 priority bits.
 *
 * @return TIM_OK    - Priority set.
 * @return TIM_ERROR - NULL pTIMHandle, Priority above 15, or pTIMx is not TIM2, TIM3,
 *                     TIM4 or TIM5. No register is modified.
 *
 * @note    - A lower number means a higher priority. An interrupt can preempt a running
 *            interrupt handler only if its priority number is lower.
 * @note    - If this function is not called, the timer keeps the NVIC reset priority of 0
 *            (the highest).
 * @note    - The priority grouping is not changed by this function. How the 4 bits are
 *            split into preemption priority and sub-priority depends on the grouping
 *            set by the application. With the reset setting, all 4 bits are preemption bits.
 * @note    - It can be called before or after TIM_IRQEnable.
 * @note    - This function does not enable the interrupt line. Use TIM_IRQEnable.
 * @note    - It is not possible to give the update and capture events of one timer
 *            different priorities, since they share the interrupt line.
 *
 */
TIM_Status_t TIM_IRQPriorityConfig(TIM_Handle_t *pTIMHandle, uint8_t Priority)
{
	if(pTIMHandle == NULL) return TIM_ERROR;

	if(Priority > 15) return TIM_ERROR;// priority is 4bit

	if(pTIMHandle->pTIMx == TIM2 )
	{
		NVIC->IP[TIM2_IRQ_NUMBER] = (Priority << 4) ;
	}
	else if(pTIMHandle->pTIMx == TIM3 )
	{
		NVIC->IP[TIM3_IRQ_NUMBER] = (Priority << 4) ;
	}
	else if(pTIMHandle->pTIMx == TIM4 )
	{
		NVIC->IP[TIM4_IRQ_NUMBER] = (Priority << 4) ;
	}
	else if(pTIMHandle->pTIMx == TIM5 )
	{
		NVIC->IP[TIM5_IRQ_NUMBER] = (Priority << 4) ;
	}
	else{
		return TIM_ERROR;
	}
	return TIM_OK;
}

/************************************************************************************************************************************
 *
 *             Define TIMER IRQ handlers
 *
 ************************************************************************************************************************************/

/**
 *
 * @brief   - Timer IRQ handlers for TIM2-TIM5.
 *
 * @details - Each timer has its own IRQ handler. The handler checks the
 *            interrupt flags for update and input-capture events, clears
 *            the corresponding flag, and invokes the registered callback.
 *
 *            Update events use the timer's UpdateCallback, while capture
 *            events use the callback registered for the corresponding
 *            channel.
 *
 *            The handler gets the timer handle from the pointer stored by
 *            TIM_BaseInit for that timer.
 *
 * @note    - An event is handled only if its flag is set and its interrupt enable bit
 *            is set. The flag is cleared, then the callback is called if one is registered
 * @note    - The handlers are called by the hardware and must not be called by
 *            the application. Their names must match the vector table entries of
 *            the startup file, so do not define other handlers with these names.
 * @note    - If TIM_BaseInit has not been called for the timer, the flags are cleared
 *            and no callback is called.
 * @note    - Each timer has a single IRQ line shared by its update and
 *            capture events.
 * @note    - The NVIC interrupt line must be enabled using TIM_IRQEnable().
 * @note    - The individual timer interrupt sources are enabled separately
 *            using TIM_BaseStart_IT() or TIM_IC_Start_IT().
 * @note    - The priority of each interrupt line can be set using TIM_IRQPriorityConfig..
 * @note    - Callbacks execute in ISR context and should be kept short
 *            and non-blocking.
 * @note    - The overcapture flags (CCxOF) are not handled.
 *
 */

void TIM2_IRQHandler()
{
	//check the handle pointer
	if(pTIM2Handle == NULL ) {TIM2->SR = 0; return;}


	if(TIM2->SR & (1<< TIMx_SR_UIF) && (TIM2->DIER & (1<< TIMx_DIER_UIE)))          // update event occured
	{
		// clear flag
		TIM2->SR = ~ (1<< TIMx_SR_UIF);

		// invoke callback if registered
		if(pTIM2Handle->UpdateCallback != NULL)
		{
			pTIM2Handle->UpdateCallback(pTIM2Handle);
		}
	}
	if(TIM2->SR & (1<<TIMx_SR_CC1IF) && (TIM2->DIER & (1<< TIMx_DIER_CC1IE)))         // capture event in channel 1
	{
		// clear flag
		pTIM2Handle->pTIMx->SR =~ (1<< TIMx_SR_CC1IF);

		// invoke callback if registered
		if(pTIM2Handle->IC_Callback[0] != NULL)
		{
			pTIM2Handle->IC_Callback[0](pTIM2Handle);
		}
	}
	if(TIM2->SR & (1<<TIMx_SR_CC2IF)&& (TIM2->DIER & (1<< TIMx_DIER_CC2IE)))         // capture event in channel 2
	{
		// clear flag
		pTIM2Handle->pTIMx->SR =~ (1<< TIMx_SR_CC2IF);

		// invoke callback if registered
		if(pTIM2Handle->IC_Callback[1] != NULL)
		{
			pTIM2Handle->IC_Callback[1](pTIM2Handle);
		}
	}
	if(TIM2->SR & (1<<TIMx_SR_CC3IF)&& (TIM2->DIER & (1<< TIMx_DIER_CC3IE)))          // capture event in channel 3
	{
		// clear flag
		pTIM2Handle->pTIMx->SR =~ (1<< TIMx_SR_CC3IF);

		// invoke callback if registered
		if(pTIM2Handle->IC_Callback[2] != NULL)
		{
			pTIM2Handle->IC_Callback[2](pTIM2Handle);
		}
	}
	if(TIM2->SR & (1<<TIMx_SR_CC4IF)&& (TIM2->DIER & (1<< TIMx_DIER_CC4IE)))           // capture event in channel 4
	{
		// clear flag
		pTIM2Handle->pTIMx->SR =~ (1<< TIMx_SR_CC4IF);

		// invoke callback if registered
		if(pTIM2Handle->IC_Callback[3] != NULL)
		{
			pTIM2Handle->IC_Callback[3](pTIM2Handle);
		}
	}

}
void TIM3_IRQHandler()
{
	//check the handle pointer
		if(pTIM3Handle == NULL ) {TIM3->SR = 0; return;}

	if(TIM3->SR & (1<< TIMx_SR_UIF) && (TIM3->DIER & (1<< TIMx_DIER_UIE)))// update event occured
	{
		// clear flag
		TIM3->SR = ~ (1<< TIMx_SR_UIF);

		if(pTIM3Handle->UpdateCallback != NULL)
		{
			// invoke callback
			pTIM3Handle->UpdateCallback(pTIM3Handle);
		}
	}
	if(TIM3->SR & (1<<TIMx_SR_CC1IF)&& (TIM3->DIER & (1<< TIMx_DIER_CC1IE))) // capture event in channel 1
	{
		// clear flag
		pTIM3Handle->pTIMx->SR =~ (1<< TIMx_SR_CC1IF);

		// invoke callback if registered
		if(pTIM3Handle->IC_Callback[0] != NULL)
		{
			pTIM3Handle->IC_Callback[0](pTIM3Handle);
		}
	}
	if(TIM3->SR & (1<<TIMx_SR_CC2IF)&& (TIM3->DIER & (1<< TIMx_DIER_CC2IE))) // capture event in channel 2
	{
		// clear flag
		pTIM3Handle->pTIMx->SR =~ (1<< TIMx_SR_CC2IF);

		// invoke callback if registered
		if(pTIM3Handle->IC_Callback[1] != NULL)
		{
			pTIM3Handle->IC_Callback[1](pTIM3Handle);
		}
	}
	if(TIM3->SR & (1<<TIMx_SR_CC3IF)&& (TIM3->DIER & (1<< TIMx_DIER_CC3IE))) // capture event in channel 3
	{
		// clear flag
		pTIM3Handle->pTIMx->SR =~ (1<< TIMx_SR_CC3IF);

		// invoke callback if registered
		if(pTIM3Handle->IC_Callback[2] != NULL)
		{
			pTIM3Handle->IC_Callback[2](pTIM3Handle);
		}
	}
	if(TIM3->SR & (1<<TIMx_SR_CC4IF)&& (TIM3->DIER & (1<< TIMx_DIER_CC4IE))) // capture event in channel 4
	{
		// clear flag
		pTIM3Handle->pTIMx->SR =~ (1<< TIMx_SR_CC4IF);

		// invoke callback if registered
		if(pTIM3Handle->IC_Callback[3] != NULL)
		{
			pTIM3Handle->IC_Callback[3](pTIM3Handle);
		}
	}


}
void TIM4_IRQHandler()
{

	//check the handle pointer
		if(pTIM4Handle == NULL ) {TIM4->SR = 0; return;}

	if(TIM4->SR & (1<< TIMx_SR_UIF) && (TIM4->DIER & (1<< TIMx_DIER_UIE)))// update event occured
	{
		// clear flag
		TIM4->SR = ~ (1<< TIMx_SR_UIF);

		if(pTIM4Handle->UpdateCallback != NULL)
		{
			// invoke callback
			pTIM4Handle->UpdateCallback(pTIM4Handle);
		}
	}
	if(TIM4->SR & (1<<TIMx_SR_CC1IF)&& (TIM4->DIER & (1<< TIMx_DIER_CC1IE))) // capture event in channel 1
	{
		// clear flag
		pTIM4Handle->pTIMx->SR =~ (1<< TIMx_SR_CC1IF);

		// invoke callback if registered
		if(pTIM4Handle->IC_Callback[0] != NULL)
		{
			pTIM4Handle->IC_Callback[0](pTIM4Handle);
		}
	}
	if(TIM4->SR & (1<<TIMx_SR_CC2IF)&& (TIM4->DIER & (1<< TIMx_DIER_CC2IE))) // capture event in channel 2
	{
		// clear flag
		pTIM4Handle->pTIMx->SR =~ (1<< TIMx_SR_CC2IF);

		// invoke callback if registered
		if(pTIM4Handle->IC_Callback[1] != NULL)
		{
			pTIM4Handle->IC_Callback[1](pTIM4Handle);
		}
	}
	if(TIM4->SR & (1<<TIMx_SR_CC3IF)&& (TIM4->DIER & (1<< TIMx_DIER_CC3IE))) // capture event in channel 3
	{
		// clear flag
		pTIM4Handle->pTIMx->SR =~ (1<< TIMx_SR_CC3IF);

		// invoke callback if registered
		if(pTIM4Handle->IC_Callback[2] != NULL)
		{
			pTIM4Handle->IC_Callback[2](pTIM4Handle);
		}
	}
	if(TIM4->SR & (1<<TIMx_SR_CC4IF)&& (TIM4->DIER & (1<< TIMx_DIER_CC4IE))) // capture event in channel 4
	{
		// clear flag
		pTIM4Handle->pTIMx->SR =~ (1<< TIMx_SR_CC4IF);

		// invoke callback if registered
		if(pTIM4Handle->IC_Callback[3] != NULL)
		{
			pTIM4Handle->IC_Callback[3](pTIM4Handle);
		}
	}


}

void TIM5_IRQHandler()
{

	//check the handle pointer
		if(pTIM5Handle == NULL ) {TIM5->SR = 0; return;}

	if(TIM5->SR & (1<< TIMx_SR_UIF) && (TIM5->DIER & (1<< TIMx_DIER_UIE)))// update event occured
	{
		// clear flag
		TIM5->SR = ~ (1<< TIMx_SR_UIF);

		if(pTIM5Handle->UpdateCallback != NULL)
		{
			// invoke callback
			pTIM5Handle->UpdateCallback(pTIM5Handle);
		}
	}
	if(TIM5->SR & (1<<TIMx_SR_CC1IF)&& (TIM5->DIER & (1<< TIMx_DIER_CC1IE))) // capture event in channel 1
	{
		// clear flag
		pTIM5Handle->pTIMx->SR =~ (1<< TIMx_SR_CC1IF);

		// invoke callback if registered
		if(pTIM5Handle->IC_Callback[0] != NULL)
		{
			pTIM5Handle->IC_Callback[0](pTIM5Handle);
		}
	}
	if(TIM5->SR & (1<<TIMx_SR_CC2IF)&& (TIM5->DIER & (1<< TIMx_DIER_CC2IE))) // capture event in channel 2
	{
		// clear flag
		pTIM5Handle->pTIMx->SR =~ (1<< TIMx_SR_CC2IF);

		// invoke callback if registered
		if(pTIM5Handle->IC_Callback[1] != NULL)
		{
			pTIM5Handle->IC_Callback[1](pTIM5Handle);
		}
	}
	if(TIM5->SR & (1<<TIMx_SR_CC3IF)&& (TIM5->DIER & (1<< TIMx_DIER_CC3IE))) // capture event in channel 3
	{
		// clear flag
		pTIM5Handle->pTIMx->SR =~ (1<< TIMx_SR_CC3IF);

		// invoke callback if registered
		if(pTIM5Handle->IC_Callback[2] != NULL)
		{
			pTIM5Handle->IC_Callback[2](pTIM5Handle);
		}
	}
	if(TIM5->SR & (1<<TIMx_SR_CC4IF)&& (TIM5->DIER & (1<< TIMx_DIER_CC4IE))) // capture event in channel 4
	{
		// clear flag
		pTIM5Handle->pTIMx->SR =~ (1<< TIMx_SR_CC4IF);

		// invoke callback if registered
		if(pTIM5Handle->IC_Callback[3] != NULL)
		{
			pTIM5Handle->IC_Callback[3](pTIM5Handle);
		}
	}


}







