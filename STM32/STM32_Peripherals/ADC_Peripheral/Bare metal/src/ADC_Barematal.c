/*
 * ADC_Barematal.c
 *
 *  Created on: May 25, 2026
 *      Author: hp
 */

#include "ADC_Baremetal.h"
#include "STM32F401RE.h"

void ADC_Configuration()
{
	// ADC Configuration for single channel conversion
		// - Set ADC resolution: CR1 Register
		// - Select channel    : SQRx Registers
		// - Set ADC frequency : CCR Register
		// -- We can set sampling time for each channels     : SMPRx Registers
	    // -- Use SCAN mode for multiple channel conversions : CR1 Register
		// -- Use CONT mode for continuous conversions       : CR2 Register
		// - GPIO Configuration: Set GPIO pin as ADC channel
		// - Enable ADC
	    // - Inside the main loop:
		// -- Start conversion and wait till it's over
		// -- Read ADC result from Data Register

		// enable clock for ADC
		ADC1_CLK_EN();

		// resolution as 12 bit : RES[1:0] = 00
		ADC1->CR1 &=~(0x3<< ADC1_CR1_RES);

		// set ADC frequency as 2MHz: ADCPRE[1:0] = 11 (prescaler 8)
		ADC_COMMON->CCR &= ~(0x3<< ADC_CCR_ADCPRE);
		ADC_COMMON->CCR |= (0x3<< ADC_CCR_ADCPRE);

		// select channel 0 for first conversion: SQR3[4:0] = 00000
		ADC1->SQR3 &=~(0x1F<< 0 * 5);

		// set sample time of 144 cycles for channel 0
		// - longer sampling time for accurate readings:
		// - SMP0[2:0] = 110 : 144 cycles
		ADC1->SMPR2 &=~(0x7<< 0 * 3);
		ADC1->SMPR2 |= (0x6<< 0 * 3);

		// EOC is set at end of single conversion: EOCS bit = 1
		ADC1->CR2 |= (1<< ADC1_CR2_EOCS);

		// Disable SCAN mode
		ADC1->CR1 &=~(1<< ADC1_CR1_SCAN);

		// Disable continuous mode
		ADC1->CR2 &=~(1<< ADC1_CR2_CONT);

		// GPIO Configuration:
		// ADC1 Channel 0 maps to GPIO pin PA0
		// Set analog mode
		GPIOA_CLK_EN();
		GPIOA->MODER |= (0x3<< 0*2);

		// enable ADC
		ADC1->CR2|=(1<< ADC1_CR2_ADON);

        // Detect ADC Value change using an LED:
		// set PA1 as Output for LED
		GPIOA->MODER &= ~(3<< 1*2);
		GPIOA->MODER |= (1<< 1*2);

}
void GPIO_Configuration_Channel_0()
{
	// GPIO Configuration:
	// ADC1 Channel 0 maps to GPIO pin PA0
	// Set analog mode
	GPIOA_CLK_EN();
	GPIOA->MODER |= (0x3<< 0*2);

}
void GPIO_Configuration_LED()
{
	// Detect ADC Value change using an LED:
	// set PA1 as Output for LED
	GPIOA->MODER &= ~(3<< 1*2);
	GPIOA->MODER |= (1<< 1*2);
}
void ADC_Start_Conversion()
{
	//start conversion:
	ADC1->CR2 |= (1<< ADC1_CR2_SWSTART);

	// wait till conversion is over
	while(!(ADC1->SR &(1<< ADC1_SR_EOC)));
}
uint16_t ADC_GetResult()
{
	// read result & Return
	uint16_t result = ADC1->DR;

	return result;
}
void Toggle_LED_BasedOn_ADC(uint16_t result)
{
	// toggle LED based on ADC readings
	if(result> 2000)
	{
		GPIOA->ODR|= (1<< 1);
	}
	else
	{
		GPIOA->ODR &=~ (1<< 1);
	}

}
