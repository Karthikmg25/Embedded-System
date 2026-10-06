/*
 * TIMER_Driver_IC_FrequencyCalculationTest.c
 *
 *  Created on: Oct 3, 2026
 *      Author: hp
 */

#include <stdio.h>
#include "stm32f401re_timer_driver.h"

volatile uint32_t ovflow_cnt = 0;
void TIM3_UpdateCallback()
{
	ovflow_cnt++;
}

// declare variables for frequency calculation
volatile uint32_t CaptureStage = 0;
volatile uint32_t totalTicksT1 = 0;
volatile uint32_t totalTicksT2 = 0;


volatile uint32_t capture=0;
void TIM3_CH4_CaptureCallback(TIM_Handle_t *pTIMHandle)
{
	if(CaptureStage == 0) // first edge capture
	{
		uint32_t captureValue = pTIMHandle->pTIMx->CCR4;

		totalTicksT1 = captureValue + (ovflow_cnt * (pTIMHandle->pTIMx->ARR + 1));

		CaptureStage = 1;
	}
	else if(CaptureStage == 1)
	{
		uint32_t captureValue = pTIMHandle->pTIMx->CCR4;

		totalTicksT2 = captureValue + (ovflow_cnt * (pTIMHandle->pTIMx->ARR + 1));

		CaptureStage = 2;
	}

	capture++;
}
float CalculateFrequency(TIM_Handle_t *pTIMHandle)
{
	// start a new capture
	CaptureStage = 0;

	// wait till both captures
	while(CaptureStage != 2);

	// calculate elapsed ticks
	uint32_t elapsedTicks = totalTicksT2 - totalTicksT1;

	// calculate time period
	float timePeriod = elapsedTicks * ((pTIMHandle->pTIMx->PSC + 1) / 16000000.0f);


	float frequency = 1/timePeriod;

	return frequency;

}
// define variables for PWM calculation
volatile uint32_t totalTicksF1 = 0;
void TIM3_CH3_CaptureCallback(TIM_Handle_t *pTIMHandle)
{
	// calculate total ticks happened on first falling edge
	if(CaptureStage == 1)
	{
		uint32_t captureValue = pTIMHandle->pTIMx->CCR3;

		totalTicksF1 = captureValue + (ovflow_cnt * (pTIMHandle->pTIMx->ARR + 1));
	}
}
float CalculateDutyCycle(TIM_Handle_t *pTIMHandle)
{
	// start a new capture
	CaptureStage = 0;

	// wait till both captures
	while(CaptureStage != 2);

	// calculate high ticks
	uint32_t highTicks = totalTicksF1 - totalTicksT1;

	// calculate period ticks
	uint32_t periodTicks = totalTicksT2 - totalTicksT1;

	// calculate dutycycle
	float duty = 100*((float)highTicks/periodTicks);

	return duty;
}
void GPIO_Config_TIM3_CH1()
{
	// Pin is PB4 with AF 2
	GPIOB_CLK_EN();

	// set AF mode
	GPIOB->MODER &=~ (0x3 << (4*2));
	GPIOB->MODER |= (0x2 << (4*2));

	// set AF2
	GPIOB->AFR[0] &=~ (0xF << (4*4));
	GPIOB->AFR[0] |= (2 << (4*4));

}
void GPIO_Config_TIM3_CH3()
{
	// Pin is PB0 with AF 2
	GPIOB_CLK_EN();

	// set AF mode
	GPIOB->MODER &=~ (0x3 << (0*2));
	GPIOB->MODER |= (0x2 << (0*2));

	// set AF2
	GPIOB->AFR[0] &=~ (0xF << (0*4));
	GPIOB->AFR[0] |= (2 << (0*4));

}
void GPIO_Config_TIM3_CH4()
{
	// Pin is PB1 with AF2
	GPIOB_CLK_EN();

	// set AF mode
	GPIOB->MODER &=~ (0x3 << (1*2));
	GPIOB->MODER |= (0x2 << (1*2));

	// set AF2
	GPIOB->AFR[0] &=~ (0xF << (1*4));
	GPIOB->AFR[0] |= (2 << (1*4));
}
int main(void)
{

	// program general purpose timers //


	// Set basic time-base settings of TIM3
	/******************************************************************************************/

	TIM_Handle_t htim3={0};
	htim3.pTIMx                = TIM3;
	htim3.TIM_Base.CounterMode = TIM_COUNTERMODE_UPCOUNTING;
	htim3.TIM_Base.AutoReloadPreload = TIM_AUTORELOADPRELOAD_DISABLE;
	htim3.TIM_Base.Prescaler = 15; // timer_f      = 16000000 / 15+1 = 1MHz
	htim3.TIM_Base.Period    = 999;// update_f     = 1MHz / 999+1 = 1Khz
	//htim3.UpdateCallback     = TIM3_UpdateCallback;

	TIM_BaseInit(&htim3);

	TIM_RegisterCallback(&htim3, TIM3_UpdateCallback);

	TIM_IRQEnable(&htim3);


	// Configure Channel 1 of TIM3 for Output compare: Toggle mode
	/**********************************************************************************/

		TIM_OC_Config_t OC_ch1 = {0};

		OC_ch1.Channel        = TIM_CHANNEL_1;
		OC_ch1.OutputMode     = TIM_OCMODE_PWM1;
		OC_ch1.OutputPolarity = TIM_OCPOLARITY_HIGH;
		OC_ch1.CompareValue   = 100;

		TIM_OC_Init(&htim3, &OC_ch1);

		// configure GPIO pin as TIM3_CH1
		GPIO_Config_TIM3_CH1();

		// start the channel
		TIM_OC_Start(&htim3, &OC_ch1);



	// Configure Channel 4 of TIM3 for Input capture: frequency calculation
	/**********************************************************************************/

	TIM_IC_Config_t IC_ch4={0};

	IC_ch4.Channel = TIM_CHANNEL_4;
	IC_ch4.Filtering = TIM_ICFILTER_0;
	IC_ch4.Prescaler = TIM_ICPSC_DIV1;
	IC_ch4.InputMode = TIM_ICMODE_DIRECT;        // direct mapping ~ CH4 -> TI4
	IC_ch4.InputPolarity = TIM_ICPOLARITY_RISING;// capture rising edges

	TIM_IC_Init(&htim3, &IC_ch4);

	// configure GPIO pin as TIM3 CH4
	GPIO_Config_TIM3_CH4();

	// register callback function
	TIM_IC_RegisterCallback(&htim3, &IC_ch4, TIM3_CH4_CaptureCallback);

	//start channel
	TIM_IC_Start_IT(&htim3, &IC_ch4);


	// Configure Channel 3 of TIM3 for Input capture: Duty cycle calculation
	/**********************************************************************************/

	TIM_IC_Config_t IC_ch3={0};

	IC_ch3.Channel = TIM_CHANNEL_3;
	IC_ch3.Filtering = TIM_ICFILTER_0;
	IC_ch3.Prescaler = TIM_ICPSC_DIV1;
	IC_ch3.InputMode = TIM_ICMODE_INDIRECT;        // indirect mapping ~ CH3 -> TI4
	IC_ch3.InputPolarity = TIM_ICPOLARITY_FALLING;  // capture falling edges

	TIM_IC_Init(&htim3, &IC_ch3);

	// register callback function
	TIM_IC_RegisterCallback(&htim3, &IC_ch3, TIM3_CH3_CaptureCallback);

	//start channel
	TIM_IC_Start_IT(&htim3, &IC_ch3);




	// Start timer
	TIM_BaseStart_IT(&htim3);

	while(1)
	{

		printf("\n\nOverflow count of timer 3 : %d", (int)ovflow_cnt);

		float f = CalculateFrequency(&htim3);

		float duty = CalculateDutyCycle(&htim3);

		printf("\n\nFrequency of signal : %f", f);

		//printf("\n\nsignal - %d", (int) (GPIOA->IDR >> (6*2))&1);

		printf("\n\nDuty Cycle of signal : %f ", duty);
	}
}


