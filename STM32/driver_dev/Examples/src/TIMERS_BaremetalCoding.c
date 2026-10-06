/*
 * TIMERS_BaremetalCoding.c
 *
 *  Created on: Sep 29, 2026
 *      Author: hp
 */

#include <stdio.h>
#include <STM32F401RE.h>


// basic Time base setting
///////////////////////////
void TIM2_BasicInit()
{
	//enable clock for timer 2
	TIM2_CLK_EN();

	// set prescaler as 16 for frequency of 1MHz: APB1 clock is 16MHz
	TIM2->PSC = 15;// 0-15 : 16 counts

	// set Top value in Auto reload register: set top = 1000: update event on each milliseconds
	TIM2->ARR = 999; // 0-999: 1000 counts

	// enable counter
	TIM2->CR1 |= (1<<TIMx_CR1_CEN);



}

// basic Interrupt settings
////////////////////////////

void TIM2_InterrruptInit()
{
	// enable clock for timer2
	TIM2_CLK_EN();

	// set prescaler value
	TIM2->PSC = 15;// division by 16 gives 1 MHz frequency

	TIM2->ARR = 999;// generate 1000 counts for update event

	//eneble interrupt on each update event
	TIM2->DIER |= (1<< TIMx_DIER_UIE);

	// Enable IRQ in NVIC : TIM2 IRQ no is 28: controlled by ISER[0] (controls IRQ0 - IRQ31)
	NVIC->ISER[0] |= TIM2_IRQ_ENABLE;

	//enable counter
	TIM2->CR1 |= (1<< TIMx_CR1_CEN);
}

volatile int count =0;

// define interrupt handler: gets invoked at each milliseconds
void TIM2_IRQHandler()
{

	//clear UIF inside handler
	if(TIM2->SR & (1<< TIMx_SR_UIF))
	{
		count++;

		TIM2->SR &= ~(1<< TIMx_SR_UIF);
	}
}
void delay_ms(uint32_t ms)
{
	int start = count;

	while(count-start < ms);
}

// Timer settings for toggle on compare match : square wave generation
//////////////////////////////////////////////////////////////////////
void TIM5_Toggle_CompareModeConfiguration()
{
	// enable timer clock
	/*****************************************************************************************/
	TIM5_CLK_EN();

	// basic timebase settings of timer
	/*****************************************************************************************/

	// set prescaler 15999: division by 16-> one pulse -> 1 ms : timer time period  = 1ms
	TIM5->PSC = 15;

	// set Top value 999: 1000 counts for overflow -> 1s       : update time period = 1s
	TIM5->ARR = 499;
	//                                                         : signal time perod  = 2s

	// Channel configuration
	/*****************************************************************************************/

	// set output compare mode for channel 1 of TIM5
	// ccmr1 -> channel 1 & 2
	// ccmr2 -> channel 3 & 4
	TIM5->CCMR1 &= ~(0x3 << TIMx_CCMR1_CC1S);// 00 : output compare mode

	// set compare mode : Toggle on compare match
	// 011 -> toggle on compare match
	TIM5->CCMR1 &= ~(0x7 << TIMx_CCMR1_OC1M);// clear bits
	TIM5->CCMR1 |= (0x3 << TIMx_CCMR1_OC1M);// 0x3 -> 011

	// set compare value to be compared against TIMx_CNT value
	// CCR1 for channel 1

	TIM5->CCR1 = 499; // compare event happens when TCNT = 499

	// Frequency of resulting output signal:

	// f_signal = f_timer / ( 2 * (ARR + 1 ))  OR  f_signal = f_update /  2

	// f_signal will be half of timer-overfolw frequency:     One full time period of signal = Two timer overflows

	// ! CCRx value doesnt affect the signal frequency !! //

	// In this configuration, where f_update is 1 Hz, f_signal = .5 hz ( OR time period =  2 seconds)



	// Enable channel ouptput pin (optioal - set polarity)
	/*****************************************************************************************/
	TIM5->CCER |= (1<< TIMx_CCER_CC1E);

	// enable counter
	/*****************************************************************************************/
	TIM5->CR1 |= (1<< TIMx_CR1_CEN);


	//sigal be like : |_______1 sec_________|-------1sec----------|_____________

}
// GPIO settings for  Timer5 channel 1
void GPIO_TIM5_Channel1Config()
{

	// configure PA0 as TIM5 channel1 pin
	GPIOA_CLK_EN();

	GPIOA->MODER &=~(0x3 << (0*2));
	GPIOA->MODER |= (0x2 << (0*2));//set AF mode for PA0

	GPIOA->AFR[0] &=~ (0xF << (0*4));// clear bits
	GPIOA->AFR[0] |= (0x2 << (0*4)); // AF2 - TIM5 CH1

}
// congifure TIM3 channel 1 for PWM wave generation
////////////////////////////////////////////////////

// counting mode decides FAST PWM OR PHASE CORRECT PWM:

//  : edge aligned mode   : fast PWM
//  : centre aligned mode : phase correct PWM

#define EDGE_ALIGNED_MODE             0
#define CENTRE_ALIGNED_MODE           1

void TIM3_PWM_Configuration(uint8_t count_mode)
{
	// enable clock for timer3
	/*****************************************************************************/

	TIM3_CLK_EN();

	// configure basic Time-base settings of timer
	/*****************************************************************************/

	// set prescaler 15: 16 counts - timer frequency = 1 MHz
	TIM3->PSC = 15;

	// set top value 999: 1000 counts - FAST PWM frequency          = 1KHz
	//                                - PHASE-CORRECT PWM frequency = 1KHz / 2
	TIM3->ARR = 999;

	// set count mode
	if(count_mode == EDGE_ALIGNED_MODE)
	{
		TIM3->CR1 &= ~(0x3 << TIMx_CR1_CMS);
	}
	else if(count_mode == CENTRE_ALIGNED_MODE)
	{
		// set MODE 3 : compare event on both up and down counting
		TIM3->CR1 &= ~(0x3 << TIMx_CR1_CMS);
		TIM3->CR1 |=  (0x3 << TIMx_CR1_CMS);
	}

	// Channel configuration : CH 1
	/*****************************************************************************/

	// set output compare mode
	TIM3->CCMR1 &= ~(0x3 << TIMx_CCMR1_CC1S);

	// set Output compare mode: PWM mode 1
	// OCM bits : 110
	TIM3->CCMR1 &= ~(0x7 << TIMx_CCMR1_OC1M);
	TIM3->CCMR1 |= (0x6 << TIMx_CCMR1_OC1M);

	// set compare value : decides duty cycle

	// Duty cycle = ( CCRx  / (ARR + 1 )) * 100
	// this eqn is same for both edge aligned and centre aligned modes (only event timing changes)
	TIM3->CCR1 = 800;// 50% duty cycle

	// enable output
	/***********************************************************************************/
	TIM3->CCER |= (1<< TIMx_CCER_CC1E);

	// enable timer
	/************************************************************************************/
	TIM3->CR1 |= (1<< TIMx_CR1_CEN);
}

// set PA6 as TIM3 channel 1
void GPIO_TIM3_Channel1Config()
{
	GPIOA_CLK_EN();

	// set AF mode for PA6
	GPIOA->MODER &= ~(0x3 << (6*2));
	GPIOA->MODER |= (0x2 << (6*2));

	// set Alternate function - AF2
	GPIOA->AFR[0] &=~(0xF << (6*4));
	GPIOA->AFR[0] |= (0x2 << (6*4));
}

// Configure TIM4 for Inout capture
////////////////////////////////////
void TIM4_InputCaptureConfiguration()
{

	// enable peripheral clock
	/******************************************************************************/
	TIM4_CLK_EN();



	// configure basic Time-base settings of timer
	/*****************************************************************************/

	TIM4->PSC = 15; // t_frequency = 16/15+1 = 1MHz

	TIM4->ARR = 999;// update_frequency as random value so that TIM4 ARR doesnt divide input signal frequency.



	// configure channel for input capture
	/*****************************************************************************/


	// set input capture mode : CC1S bits : 01 - use TIM4_CH1 as input pin (TI1) - PB6 ~ DIRECT MAPPING
	//                                      10 - use TIM4_CH2 as input pin (TI2) - PB7 ~ INDIRECT MAPPING
	TIM4->CCMR1 &=~(0x3 << TIMx_CCMR1_CC1S);
	TIM4->CCMR1 |= (0x1 << TIMx_CCMR1_CC1S);

	// set filtering: very low filtering here needed

	// IC1F bits : 0001 - sampling at timer clk frequency, 2 valid samples needed
	TIM4->CCMR1 &=~ (0xF << TIMx_CCMR1_IC1F);
	TIM4->CCMR1 |= (0x01 << TIMx_CCMR1_IC1F);

	// set input capture frequency

	// IC1PSC bits : 00 - capture done when an edge is detected
	TIM4->CCMR1 &=~(0x3 << TIMx_CCMR1_IC1PSC);

	// select which edges to capture

	//   CCN1P:CC1P - 00 - RISING EDGE CAPTURE
	//   CCN1P:CC1P - 01 - FALLING EDGE CAPTURE
	//   CCN1P:CC1P - 11 - BOTH EDGE CAPTURE

	// clearing both bits → 00 → RISING EDGE selected
	TIM4->CCER &=~((1<< TIMx_CCER_CC1P)|(1<< TIMx_CCER_CCN1P));

	// enable channel
	/*****************************************************************************/

	TIM4->CCER |= (1<< TIMx_CCER_CC1E);


	// Interrupt enable (if needed)
	/****************************************************************************/
	// Add this for frequency calculation:

	// Set up update interrupt to count number of overflows occured b/w time-stamps
	TIM4->DIER |= (1<< TIMx_DIER_UIE);
	// enable capture event interrupt
	TIM4->DIER |= (1<< TIMx_DIER_CC1IE);
	// Enable IRQ in NVIC: IRQ 30 for TIM4
	NVIC->ISER[TIM4_IRQ_ISER_INDEX] |= TIM4_IRQ_ENABLE;

	// enable timer
	/*****************************************************************************/
	TIM4->CR1 |= (1<< TIMx_CR1_CEN);


}
// declare a global variable for overflow count
volatile uint32_t ovflow_count=0;

// declare variables for frequency calculation
// total elapsed ticks on each capture events
volatile uint32_t total_ticks_t1=0, total_ticks_t2=0;
// need two captures for calculation
volatile uint8_t capture_stage =0;

// declare variables for duty cycle and pulsewidth calculation
volatile uint32_t total_ticks_fall =0;

void TIM4_IRQHandler()
{
	if(TIM4->SR & (1<< TIMx_SR_UIF))// Update event - timer overflow
	{
		// increment on each overflow interrupt
		ovflow_count++;
		//clear flag by writing 0
		TIM4->SR &=~(1<< TIMx_SR_UIF);
	}
	if(TIM4->SR & (1<< TIMx_SR_CC1IF))// capture event on channel 1 - PB6
	{
		// read CCR1 to clear CC1IF flag
		// reading CCR! clears CC1IF flag
		uint32_t captured_value = TIM4->CCR1;

		if(capture_stage == 0) // First capture (Risisng edge)
		{
			total_ticks_t1 = (ovflow_count * (TIM4->ARR+1)) + captured_value;

			capture_stage = 1;// one capture occured
		}
		else if(capture_stage == 1)// Second capture (Risisng edge)
		{
			total_ticks_t2 = (ovflow_count * (TIM4->ARR+1)) + captured_value;

			capture_stage = 2;// both captures occured - variables are now valid for frequency calculation
		}
	}
	if(TIM4->SR & (1<< TIMx_SR_CC2IF))
	{
		// total ticks on first falling edge
		uint32_t capture_value = TIM4->CCR2;

		//capture falling edge ticks only after 1st rising edge - t1
		if(capture_stage == 1)
		{
			 total_ticks_fall = (ovflow_count*(TIM4->ARR+1))+ capture_value;
		}
	}

}
float TIM4_FrequencyCalculation_Interrupt()
{
	// reset capture_stage for first capture
	capture_stage = 0;

	// wait till both captures occured
	while( capture_stage != 2);

	//calculate elapsed ticks between the two captures
	uint32_t elapsed_ticks = total_ticks_t2 - total_ticks_t1;

	// calculate time period of signal
	float time_period = elapsed_ticks * ((TIM4->PSC + 1)/16000000.0);

	// calculate frequency of signal
	float frequency = 1.0 / time_period;

	return frequency;

}
float TIM4_FrequencyCalculation_Polling()// error prone !
{
	float frequency=0;

	// wait for first Input capture event : detects first Risisng edge of signal
	//////////////////////////////////////////////////////////////////////////////////

	while(!(TIM4->SR & (1<< TIMx_SR_CC1IF)));

	// read CCR1 and overflow_count to  calculate number of ticks on first timestamp
	//////////////////////////////////////////////////////////////////////////////////

	//read time-stamp1 and overflow count

	uint32_t ovflow_cnt1 = ovflow_count;
	uint32_t t1          = TIM4->CCR1;// reading CCR1 clears the CC1IF flag


	// wait for next Input capture event : detects next Risisng edge of signal
	//////////////////////////////////////////////////////////////////////////////////

	while(!(TIM4->SR & (1<< TIMx_SR_CC1IF)));

	// read CCR1 and overflow_count to calculate number of ticks on second time-stamp
	//////////////////////////////////////////////////////////////////////////////////

	//read time-stamp2 and overflow count

	uint32_t ovflow_cnt2 = ovflow_count;
	uint32_t t2          = TIM4->CCR1;// reading CCR1 clears the CC1IF flag

	// calculate total elapsed ticks between two time-stamps
	//////////////////////////////////////////////////////////////////////////////////

	uint32_t elapsed_ticks = ((ovflow_cnt2 - ovflow_cnt1)*(TIM4->ARR + 1)) + (t2-t1);

	// calculate time period of signal based on elapsed ticks
	//////////////////////////////////////////////////////////////////////////////////

	float time_period = elapsed_ticks * (float) (TIM4->PSC+1) / 16000000.0 ;

	// calculate frequency
	//////////////////////////////////////////////////////////////////////////////////

	frequency = 1.0 / time_period;

	return frequency;
}
// Set PB6 as TIM 4 Channel 1 - TI1
void GPIO_TIM4_Channel1Config()
{
	//enable GPIO clock
	GPIOB_CLK_EN();

	// set AF mode
	GPIOB->MODER &=~(0x3 << (6*2));
	GPIOB->MODER |= (0x2 << (6*2));

	// set AF2
	GPIOB->AFR[0] &=~(0xF << (6*4));
	GPIOB->AFR[0] |= (0x2 << (6*4));
}

// Configure TIM4 Channel 2 to read falling edge of PWM signal on TI1 (PB6)
// calculate pulse width and duty cycle of PWM
void TIM4_CH2_Configuration()
{
	// clock enable       - done
	// time base settings - done
	// timer enable       - done

	// channel 2 configuration
	//////////////////////////////////////////////

	// set input capture mode: channel for capturing TI1 PB6- indirect mapping
	TIM4->CCMR1 &=~(0x3 << TIMx_CCMR1_CC2S);
	TIM4->CCMR1 |= (0x2 << TIMx_CCMR1_CC2S);

	// set low filtering
	TIM4->CCMR1 &=~(0xF << TIMx_CCMR1_IC2F);
	TIM4->CCMR1 |= (0x1 << TIMx_CCMR1_IC2F);

	// capture an edge when detected - no prescaler
	TIM4->CCMR1 &=~(0x3 << TIMx_CCMR1_IC2PSC);

	// select edge to capture
	// - falling edge - CCN2P:CC2P - 01
	TIM4->CCER &=~(1<< TIMx_CCER_CCN2P);
	TIM4->CCER |= (1<< TIMx_CCER_CC2P);

	// enable channel
	TIM4->CCER |= (1<< TIMx_CCER_CC2E);

	// enable interrupt on capture
	TIM4->DIER |= (1<< TIMx_DIER_CC2IE);
}

void TIM4_PWMCalculation()
{
	// reset capture stage to read first rising edge
	capture_stage = 0;

	// wait till 2nd rising edge
	while(capture_stage != 2);

	// calculate high ticks and low ticks

	uint32_t high_ticks = total_ticks_fall - total_ticks_t1; // ticks b/w first rise edge and fall edge
	uint32_t period_ticks =  total_ticks_t2 -  total_ticks_t1;// ticks b/w first and second rise edges
	//uint32_t low_ticks = period_ticks - high_ticks;

	// calculate duty cycle

	float duty_cycle = ((float)high_ticks/(float)period_ticks) * 100;

	// calculate pulse width

	float pulse_width = high_ticks * ((TIM4->PSC + 1)/ 16000000.0);
	char ch = '%';
	printf("\n\n PWM Duty cycle : %.02f", duty_cycle); printf(" %c", ch);

	printf("\n PWM pulse width : %.02f ms", pulse_width*1000);

}

int main(void)
{

	// program general purpose timers //

	// program TIM2 for interrupt generation
	TIM2_InterrruptInit();

	// program TIM5 for output compare mode
	TIM5_Toggle_CompareModeConfiguration();

	// set PA0 as TIM5 channel 1
	GPIO_TIM5_Channel1Config();

	// program TIM3 for PWM generation
	TIM3_PWM_Configuration(EDGE_ALIGNED_MODE);

	//set PA6 as TIM3 channel 1
	GPIO_TIM3_Channel1Config();

	// Program TIM4 for Input capture
	TIM4_InputCaptureConfiguration();

	//set PB6 as TIM4 Channel 1
	GPIO_TIM4_Channel1Config();

	// read signal from PA0 (TIM5 CH1) and calculate frequency of that signal
	//float frequency = TIM4_FrequencyCalculation_Polling(); - causes error based TIM4-> ARR value

	// read signal from PA6 (TIM6 CH1) and calculate duty cycle and frequency

	// program TIM4 CH2 for PWM signal
	TIM4_CH2_Configuration();



	while(1)
	{
		delay_ms(1000);// delay 1000 milliseconds

		//uint32_t PA0_State = GPIOA->IDR & (1<< 0);

		//printf("\nState of TIM5 channel 1 pin: %d", (int)PA0_State);


		// read signal from PA0 (TIM5 CH1) and calculate frequency of that signal
		float frequency = TIM4_FrequencyCalculation_Interrupt();


		printf("\nfrequency of signal on TIM4 channel 1 pin: %f Hz", frequency);
		TIM4_PWMCalculation();


	}
}









