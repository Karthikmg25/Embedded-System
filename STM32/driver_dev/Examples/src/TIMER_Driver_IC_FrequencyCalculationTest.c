/*
 * TIMER_Driver_IC_FrequencyCalculationTest.c
 *
 *  Created on: Oct 3, 2026
 *      Author: hp
 */

#include <stdio.h>
#include "stm32f401re_timer_driver.h"

void GPIO_Config_TIM2_CH1();
void GPIO_Config_TIM2_CH2();

volatile uint32_t ovflow_count=0;

// define callback function for finding overflow count
void TIM2_Callback()
{
	ovflow_count++;
}

// declare variables to calculate signal frequency

volatile uint32_t CaptureStage = 0;  // need two captures to calculate frequency
volatile uint32_t TotalTicksT1 = 0;  // total counts happened till first capture
volatile uint32_t TotalTicksT2 = 0;  // total counts happened till second capture

void TIM2_CH2_CaptureCallback(TIM_Handle_t *pTIMhandle)
{
	// Calculate total ticks till first capture
	if(CaptureStage == 0)
	{
		// read captured value
		uint32_t captureValue = pTIMhandle->pTIMx->CCR2;

		TotalTicksT1 = captureValue + ( ovflow_count * (pTIMhandle->pTIMx->ARR + 1));

		CaptureStage = 1; // one capture finished
	}
	// Calculate total ticks till second capture
	else if(CaptureStage == 1)
	{
		// read captured value
		uint32_t captureValue = pTIMhandle->pTIMx->CCR2;

		TotalTicksT2 = captureValue + (ovflow_count * (pTIMhandle->pTIMx->ARR + 1));

		CaptureStage = 2; // both captures finished
	}
}
float CalculateFrequency(TIM_Handle_t* pTIMHandle)
{
	// start a new capture
	CaptureStage = 0;

	// wait till both captures happened
	while(CaptureStage != 2);

	// calculate time between both captures ~ time period of signal

	uint32_t elapsedTicks = TotalTicksT2 - TotalTicksT1;
	float timePeriod   = elapsedTicks * ((pTIMHandle->pTIMx->PSC + 1)/16000000.0f);

	float frequency = 1.0f/timePeriod;

	return frequency;
}
int main(void)
{

	// program general purpose timers //


	// Set basic time-base settings of TIM2
	/******************************************************************************************/

	// declare a handle for timer 2 base configurations
	    TIM_Handle_t htim2 = {0};
	    htim2.pTIMx                      = TIM2;
	    htim2.TIM_Base.Prescaler         = 15;   // t_frequency = 1MHz, 1 count = 1us
	    htim2.TIM_Base.Period            = 999;  // update_frequency = 1 KHz, 1 overflow = 1ms
		htim2.TIM_Base.CounterMode       = TIM_COUNTERMODE_UPCOUNTING;    // counts up to ARR
		htim2.TIM_Base.AutoReloadPreload = TIM_AUTORELOADPRELOAD_DISABLE; // ARR not buffered

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

		// Settings : toggle mode : Counter counts till ARR- 999.
		//  this generates a signal where,
		//                  frequency = update_frequency / 2 ( Here, update freq = 1Hz. so, signal freq = 500 Hz )

		// Before starting channel, configure GPIO pin as TIM2_CH1
		GPIO_Config_TIM2_CH1();

		// Start Channel
		TIM_OC_Start(&htim2, &OC_ch1);


		// Configure Channel 2 of TIM2 for Input capture: frequency calculation
		/**********************************************************************************/

		TIM_IC_Config_t IC_ch2={0};

		IC_ch2.Channel       = TIM_CHANNEL_2;
		IC_ch2.Filtering     = TIM_ICFILTER_0;// no filtering
		IC_ch2.InputMode     = TIM_ICMODE_DIRECT; // direct mapping: input captured on TI2 - PA1 ( AF1  )
		IC_ch2.Prescaler     = TIM_ICPSC_DIV1; // no prescaler, capture all signals
		IC_ch2.InputPolarity = TIM_ICPOLARITY_RISING;// capture rising edges

		TIM_IC_Init(&htim2, &IC_ch2);

		// Configure GPIO pin as TIM2_CH2
		GPIO_Config_TIM2_CH2();

		// Start channel
		TIM_IC_Start_IT(&htim2, &IC_ch2);

		// register callback function | capture event |
		TIM_IC_RegisterCallback(&htim2, &IC_ch2, TIM2_CH2_CaptureCallback);

		// register callback function |  finding overflow count |
		TIM_RegisterCallback(&htim2, TIM2_Callback);


		// Enable IRQ for TIM2
		TIM_IRQEnable(&htim2);

		// Start the timer
		TIM_BaseStart_IT(&htim2);



		// Now PA0 is producing a 500Hz signal

		// PA1 is reading the signal frequency


	while(1)
	{
		printf("\n\n Overflow count : %d", (int)ovflow_count);

		float frequency = CalculateFrequency(&htim2);

		printf("\n Frequency = %f", frequency);


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
void GPIO_Config_TIM2_CH2()
{
	// Pin is PA1 with alternate function- AF1

		// set AF mode
		GPIOA->MODER &=~ (0x3 << (1*2));
		GPIOA->MODER |=  (0x2 << (1*2));

		// set AF1
		GPIOA->AFR[0] &=~ (0xF << (1*4));
		GPIOA->AFR[0] |=  (1 << (1*4));
}




