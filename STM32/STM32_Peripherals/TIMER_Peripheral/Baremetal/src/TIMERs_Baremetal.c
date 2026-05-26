/*
 * TIMERs_Baremetal.c
 *
 *  Created on: May 23, 2026
 *      Author: hp
 */


#include "TIMERs_Baremetal.h"

void Delay_Creation()
{
	// Delay creation using general Purpose Timers
	    // - Use TIMER 2 - 32 bit timer
		// - set TOP+1 value as 1000 in ARR register
		// - Set Prescaler+1 as 16 in PSC register (16bit)
		// - Update event frequency = F_CPU / 16*1000 =1000Hz
		// - Update event(Overflow) occurs every 1ms
	    // - Use UIF flag to detect the event and use it for delay creation
		// - Clear flag by writing 0 after the event

		// enable clock for timer 2
		TIM2_CLK_EN();

		// reset count register
		TIM2->CNT = 0;
		// set top value
		TIM2->ARR = 999;
		// set prescaler
		TIM2->PSC = 15;
		// enable timer
		TIM2->CR1 |= (1<< TIMx_CR1_CEN );

		// enable clock for GPIO
		GPIOA_CLK_EN();

		// Set PA0 as output
		GPIOA->MODER &= ~(0x3<< 0*2);
		GPIOA->MODER |= (1<< 0*2);

		// Function for generating delay
		void delay_ms(uint32_t time)
		{
			for(uint32_t i=0;i<time;i++)
			{
				// 1 millisecond delay:
				while(!(TIM2->SR & (1<< TIMx_SR_UIF)));
				// clear flag
				TIM2->SR &=~(1<< TIMx_SR_UIF);
			}
		}

		// Toggle LED in a loop with custom delay
		while(1)
		{
			GPIOA->ODR ^=(1<< 0);
			delay_ms(100);

		}
}

void Timer_Interrupt_Generation()
{
	// Interrupt generation Using TIMER 5
		// - Generate Interrupt at every 100ms
		// - Enable clock for the timer
		// - Set TOP+1 as 10000 in ARR register
		// - Set prescaler+1 as 160 in PSC register
		// - Event frequency = 16000000/ 10000 * 160 = 10Hz (T=100ms)
		// - Enable Interrupt in Timer and NVIC
		// - Clear Interrupt flag(UIF) by writing 0 in ISR

		TIM5_CLK_EN();

		TIM5->CNT =0;
		TIM5->ARR = 9999;
		TIM5->PSC = 159;

		// enable interrupt
		TIM5->DIER |= (1<< TIMx_DIER_UIE);
		// enable in NVIC :
		// - IRQ number of TIMER5 Interrupt is 50
		// - Interrupts from all timer5 events are mapped to IRQ_no 50
		// - Uses bit-50 % 32 in ISER[1] register
		NVIC->ISER[1] |= (1<< (50%32));

		// enable timer
		TIM5->CR1 |= (1<< TIMx_CR1_CEN);

		// Enable clock for GPIO
		// - set PA0 as output
		GPIOA_CLK_EN();
		GPIOA->MODER &= ~(0x3<<0*2);
		GPIOA->MODER |= (1<<0*2);

		while(1)
		{

		}
}

// Timer5 Interrupt for Handling update Overflow:
void TIM5_IRQHandler()
{
	// Check which event triggered interrupt
	if(TIM5->SR & (1<< TIMx_SR_UIF))
	{
		// clear interrupt flag (UIF)
			TIM5->SR &=~(1<<TIMx_SR_UIF);

		// Toggle LED
			GPIOA->ODR ^=(1<<0);
	}
}


    /*
	 *
	 * Importance of Non Blocking delays:
	 ***************************************
	 * - CPU is free to do other tasks
     * - timing is more stable
     * - scalable for multitasking
     * - basis of RTOS tick systems
     *
	 */
volatile uint32_t count=0;

void Non_blocking_delay_Generation()
{

	// Creating non-blocking delay using Timer Interrupts
	// - Use TIMER 3 (16 bit)
	// - Set Overflow frequency as 1ms: TOP+1 = 1000, PSC+1 = 16
	// - This creates interrupts every 1ms
	// - Increment a count variable inside the ISR
	// - Use count in a main loop to create non blocking delay

	// enable clock for TIMER 3
	TIM3_CLK_EN();

	// Timer configurations:
	TIM3->CNT = 0;
	TIM3->ARR = 999;
	TIM3->PSC = 15;

	// Enable interrupt on timer3 and NVIC
	TIM3->DIER |= (1<< TIMx_DIER_UIE);
	// IRQ number for TIMER3 global interrupt: 29
	NVIC->ISER[0] |= (1<< 29);
	// Set highest Preemption priority level: 0x0000 in IP[29]7:4
	NVIC->IP[29] &=~(0xF<< 4);

	// Enable Timer 3
	TIM3->CR1 |= (1<< TIMx_CR1_CEN);



	// Enable clock for GPIO
	// - set PA0 as output
	GPIOA_CLK_EN();
	GPIOA->MODER &= ~(0x3<<0*2);
	GPIOA->MODER |= (1<<0*2);


	// Variable for delay creation:
	uint32_t current_count= count;

	while(1)
	{
        // non-blocking delay for 500ms
		// - This condition becomes true only after 500ms
		// - Perform the task and reset the current_count as count
        if((count-current_count) > 500)
        {
        	// Toggle:
        	GPIOA->ODR ^= (1<< 0);

        	// Reset count
        	current_count = count;
        }

	}
}
// Increment count inside ISR
void TIM3_IRQHandler()
{
	// clear flag
	TIM3->SR &=~(1<< TIMx_SR_UIF);

	count++;
}



// PWM Generation using Timers
// - Use TIMER 4 (16 bit), Channel 1:
// - 1) Set PWM mode using CCMRx Register
// ---- 4 Independent channels available for each timers
// ---- CCMR1 is used for channel 1 and channel 2
// ---- Set bits OC1M2:0 as 110 for Non inverting PWM
// - 2) Set PWM frequency:
// ---- fPWM = F_CPU/ (TOP+1)(PSC+1)
// ---- Set TOP value in ARR register
// ---- Set prescaler in PSC register
// - 3) Set Duty cycle:
// ---- duty = (CCR1/ TOP+1 )*100
// ---- Set Compare value in CCR1
// - 4) Enable Channel output using CCER register
// - 5) Enable timer using CR1 register
// - 6) Configure GPIO pin(PB6) as alternate function output
// ---- Select TIM4_CH1 pin
// ---- Configure Alternate Function mode in MODER
// ---- Configure AF value as AF2 in AFR register

void PWM_Generation()
{
	// Enable clock for timer4
		TIM4_CLK_EN();

		// Set Non inverting PWM mode
		// clear bits
		TIM4->CCMR1 &=~(0x7<< TIMx_CCMR1_OC1M);
		TIM4->CCMR1 |= (0x6<< TIMx_CCMR1_OC1M);

		// Set PWM frequency as 1KHz
		TIM4->ARR = 3999;
		TIM4->PSC = 3;

		// Set Duty cycle 40%: compare value = (40/100) * TOP+1 = 400
		TIM4->CCR1 = 400;

		// Enable channel output
		TIM4->CCER |= (1<< TIMx_CCER_CC1E);

		// Alternate function configuration
		// enable clock
		GPIOB_CLK_EN();
		// alternate function mode
		GPIOB->MODER &=~(0x3<< 6*2);
		GPIOB->MODER |= (0x2<< 6*2);
		// Set AF2(0010) for PB6 as TIM4_CHANNEL1
		// - Use register AFR[0] for pins 0-7
		// - Each pin requires 4 bits to store AF code: Use bit position - 6*4
		GPIOB->AFR[0] &=~(0xF<< 6 * 4);
		GPIOB->AFR[0] |= (0x2<< 6 * 4);

		// Enable timer4
		TIM4->CR1 |= (1<< TIMx_CR1_CEN);


}

void Set_Brightness(uint16_t value)
{
	TIM4->CCR1 = value;
}
uint16_t MapADC_To_PWM(uint16_t ADC_value)
{
	// 2200, 3500 are threshold ADC values
	// 4000 is Top value set for PWM
	uint16_t duty ;
		if(ADC_value<2200)
		{
			return 0;
		}
		if(ADC_value>4000)
		{
			return 3999;
		}

		duty = 3* ((ADC_value - 2200) * 4000) / (3500 - 2200);

		return duty;

}





