/*
 * TIMER_Driver_IC_FrequencyCalculationTest.c
 *
 *  Created on: Oct 3, 2026
 *      Author: hp
 */

#include <stdio.h>
#include "stm32f401re_timer_driver.h"
#include "stm32f401re_gpio_driver.h"

void GPIO_Configure_TIM4_CH1()
{
	// pin is PB6 with AF2
	GPIOB_CLK_EN();

	// set AF mode
	GPIOB->MODER &=~ (0x3 << (6*2));
	GPIOB->MODER |= (0x2 << (6*2));

	// set AF2
	GPIOB->AFR[0] &=~ (0xF << (6*4));
	GPIOB->AFR[0] |= (0x2 << (6*4));
}
void EXTI1_IRQHandler(void)
{
	if(EXTI->PR & (1<< 1))// if interrupt occured
	{
		// clear interrupt bit
		EXTI->PR = (1<< 1);
		printf("\n\nOne second elapsed");

	}
}

int main(void)
{
	// program timer4 for producing a signal with period 1 seconds. ie, 1Hz frequency.

	TIM_Handle_t htim4={0};

	htim4.pTIMx = TIM4;
	htim4.TIM_Base.Prescaler = 15999;// means one count is 1ms
	htim4.TIM_Base.Period    = 499;// means 500 counts till overfolw, ie 500ms
	htim4.TIM_Base.CounterMode = TIM_COUNTERMODE_UPCOUNTING;
	htim4.TIM_Base.AutoReloadPreload = TIM_AUTORELOADPRELOAD_DISABLE;// no buffering , ARR value takes effect immediately.

	TIM_BaseInit(&htim4);

	// program timer4 channel 1 in output toggle mode for signal generation.

	TIM_OC_Config_t OC_ch1={0};
	OC_ch1.Channel = TIM_CHANNEL_1;
	OC_ch1.CompareValue = 100;// doesnt affect signal frequency in this mode
	OC_ch1.OutputMode   = TIM_OCMODE_TOGGLE;// toggles at compare match
	OC_ch1.OutputPolarity = TIM_OCPOLARITY_HIGH;

	// generated signal period = update period of timer * 2
	//                         = 500ms * 2 = 1s, ie signal frequency = 1Hz.

	TIM_OC_Init(&htim4, &OC_ch1);

	// configure gpio pin as TIM4 CH1
	GPIO_Configure_TIM4_CH1();

	// start the channel
	TIM_OC_Start(&htim4, &OC_ch1);

	//start the timer
	TIM_BaseStart(&htim4);


	// configure GPIO pin as EXTI line and enable EXTI interrupt to detect signal on pin- PA1

	GPIO_Handle_t hpa1={0};
	hpa1.pGPIOx = GPIOA;
	hpa1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;// captures all rising edges. every 1 second (signal frequency is 1Hz)
	hpa1.GPIO_PinConfig.GPIO_PinNumber = 1;
	GPIO_Init(&hpa1);

	// enable IRQ line for EXTI1
	GPIO_IRQInterruptControl(IRQ_NO_EXTI1, ENABLE);

	//<< Connect a wire between PA1 and PB6 to read signal edges >>//



	while(1)
	{
		for(int i=0;i<2;i++)
		{
			while(TIM_GetFlagStatus(&htim4, TIMx_SR_UIF) != 1);
			//clear flag
			TIM_ClearFlag(&htim4, TIMx_SR_UIF);
		}
		//printf("\n\nOne second passed");




	}
}


