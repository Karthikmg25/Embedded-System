/*
 * HCSR04_Driver.c
 *
 *  Created on: Mar 22, 2026
 *      Author: hp
 */

#include "HCSR04_Driver.h"


void SysTick_Delay_init()
{
	/* Set SysTick for creating delay */

	SYSTICK->LOAD = 15;  // Load top as 15, frequency=16MHz/15+1=1MHz, overflow on each microseconds

	SYSTICK->VAL = 0; // clear current value

	SYSTICK->CTRL |= (1<<SYSTICK_CTRL_ENABLE)|(1<<SYSTICK_CTRL_CLKSOURCE); // start counting, use CPU clock

}

void Delay_us(uint32_t time)
{
	for(uint32_t i=0; i<time;i++)
	{
		while(!(SYSTICK->CTRL&(1<<SYSTICK_CTRL_COUNTFLAG)));// 1us delay
			// Reading CTRL clears Flag
	}
}
void GPIO_setup()
{
	GPIO_Handle_t output, trigger, echo;

	output.pGPIOx = GPIOA;
	output.GPIO_PinConfig.GPIO_PinNumber= 0;                       /* OUTPUT PIN*/
	output.GPIO_PinConfig.GPIO_PinMode=  GPIO_MODE_OUT;
	output.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PUSHPULL;
	output.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
	output.GPIO_PinConfig.GPIO_PinPUPD=GPIO_NO_PUPD;

	GPIO_Init(&output);

	trigger.pGPIOx = GPIOA;
	trigger.GPIO_PinConfig.GPIO_PinNumber= 1;                      /* TRIGGER PIN*/
	trigger.GPIO_PinConfig.GPIO_PinMode=  GPIO_MODE_OUT;
	trigger.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PUSHPULL;
	trigger.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
	trigger.GPIO_PinConfig.GPIO_PinPUPD=GPIO_NO_PUPD;

	GPIO_Init(&trigger);
	GPIO_Write_OutputPin(GPIOA, 1, 0);// set as default LOW

	echo.pGPIOx = GPIOA;
	echo.GPIO_PinConfig.GPIO_PinNumber= 4;                          /* ECHO PIN*/
	echo.GPIO_PinConfig.GPIO_PinMode=  GPIO_MODE_IN;
	echo.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PUSHPULL;
	echo.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
	echo.GPIO_PinConfig.GPIO_PinPUPD=GPIO_PIN_PULLDOWN ;

	GPIO_Init(&echo);
}



void Send_Trigger()
{                                            /* SEND TRIGGER PULSE*/
	Delay_us(2);
	GPIOA->ODR |= (1<<TRIGGER_PIN);          // Set Trigger Pin as HIGH for 10us
	Delay_us(10);
	GPIOA->ODR &=~(1<<TRIGGER_PIN);          // Reset Trigger Pin
}

void Timer2_Init()
{
	TIM2_CLK_EN();                            /* TIMER2 SETUP FOR TIME MEASUREMENT */

	TIM2->ARR = 65565;                        // Time period as 65.5ms, maximum echo time is only up to 23ms

	TIM2->PSC = 15;                           // Timer frequency as 16MHz/(15+1)= 1MHz, 1 count will be 1 microseconds

	TIM2->CR1 |= (1<<TIMx_CR1_CEN);           // Start Count
}

uint32_t HCSR04_GetDistance()                 /* GET DISTANCE FROM ECHO TIME */
{
	Send_Trigger();                           // Send trigger pulse

	while(!(GPIOA->IDR & (1<< ECHO_PIN)));    // Wait till echo pin goes HIGH

	TIM2->CNT = 0;                            // Reset Timer : start count from 0

	while((GPIOA->IDR & (1<< ECHO_PIN)));     // wait till echo pin goes LOW

    uint32_t echo_time = TIM2->CNT;           // Read count value for echo time (in us) : Time for reaching Object and Return

	return (echo_time)/58;                    // distance= (echo time * speed of sound)/2
                                              //         = (echo time * 0.034cm/us)/2    = echo time/58
}






void Buzzer_ON(){
	GPIOA->ODR |= (1<<0);
}
void Buzzer_OFF(){
	GPIOA->ODR &=~(1<<0);
}







