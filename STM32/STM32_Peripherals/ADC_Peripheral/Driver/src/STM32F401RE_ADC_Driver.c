/*
 * STM32F401RE_ADC_Driver.c
 *
 *  Created on: May 25, 2026
 *      Author: hp
 */
#include "STM32F401RE_ADC_Driver.h"



/*
 * Design an Interrupt driven ADC Driver for single shot conversion
 **************************************************************************************
 * - Focusing more on simplicity and reliability
 * - Driver features:
 * --- Interrupt driven
 * --- Single conversion per channel
 * --- User decides channel and sampling time at start of conversion
 * --- Software sequencing is used for multi-channel,continuous conversion in Application layer
 * --- Stores latest ADC result is in handle structure
 * --- stores conversion flag in handle structure: synchronizes ISR with main loop
 *       : main loop triggers next conversion only when previous one is complete
 * --- Call back function support
 * - Hardware features not available: SCAN mode
 *                                    Continuous mode
 *                                    Discontinuous mode
 *                                    Injected channels
 *                                    Analog Watchdog
 *                                    DMA
 *
 */

// Define a global-static pointer for ISR Linkage
// - this pointer links the ISR with the handle structure declared in Application layer
// - allows ISR to modify the handle elements during run time
static ADC_Handle_t* handle_ptr;

/*
 * ADC Initialization:
 * - sets resolution
 * - sets ADC frequency
 * - enables interrupt
 * - enable ADC peripheral
 * - initialize pointer for ISR linkage
 * - initialize function pointer for call back as NULL
 *
 */

void ADC_Init(ADC_Handle_t* pADC_Handle)
{
	// resolution: CR1_RES[1:0]
	ADC1->CR1 &=~(0x3<< ADC1_CR1_RES);
	ADC1->CR1 |= (pADC_Handle->ADC_config.ADC_Resolution<< ADC1_CR1_RES);

	// ADC frequency: CCR_ADCPRE[1:0]
	ADC_COMMON->CCR &=~(0x3<< ADC_CCR_ADCPRE);
	ADC_COMMON->CCR |= (pADC_Handle->ADC_config.ADC_Prescaler<< ADC_CCR_ADCPRE);


	/* Optional for single channel conversion */
	// 1) End of conversion selection:
	// ---- EOC set at the end of single conversion
	ADC1->CR2 |= (1<< ADC1_CR2_EOCS);
	// 2) Disable SCAN mode for single channel conversion:
	ADC1->CR1 &=~(1<< ADC1_CR1_SCAN);
	// 3) Disable CONT mode
	ADC1->CR2 &=~(1<< ADC1_CR2_CONT);



	// Enable ADC Interrupt
	ADC1->CR1 |= (1<< ADC1_CR1_EOCIE);
	// Enable NVIC: IRQ Number is 18 for ADC global interrupt, uses ISER[0] Register
	NVIC->ISER[0] |= (1<< IRQ_NO_ADC1);

	// Enable ADC Peripheral:
	ADC1->CR2 |= (1<< ADC1_CR2_ADON);


	// Initialize the global pointer for ISR linkage
	handle_ptr = pADC_Handle;

	// Initialize function pointer for call back as NULL
	pADC_Handle->function_ptr = NULL;

}
/*
 * ADC Start conversion
 * - selects the channel
 * - sets the sample time
 * - sets conversion flag as LOW: Synchronization b/w main & ISR
 * - starts ADC conversion
 */
void ADC_StartConversion(ADC_Handle_t* pADC_Handle, Channel_t channel)
{
	// Channel selection: SQR3_SQ1[4:0]
	ADC1->SQR3 &=~(0x1F<< 0);
	ADC1->SQR3 |= (channel<< 0);

	// Setting Sample time:
	// - channel number decides register and bit position
	// - SPMR2 for channel0-channel9, SMPR1 for other channels
	// - number of bits used: 3
	uint8_t bit_pos = channel%10;
	if(channel <= 9)
	{
		ADC1->SMPR2 &=~(0x7<< bit_pos * 3);
		ADC1->SMPR2 &=~(pADC_Handle->ADC_config.ADC_Sample_time<< bit_pos * 3);
	}
	else
	{
		ADC1->SMPR1 &=~(0x7<< bit_pos * 3);
		ADC1->SMPR1 &=~(pADC_Handle->ADC_config.ADC_Sample_time<< bit_pos * 3);
	}
	// Set conversion state as unfinished
	pADC_Handle->ADC_Conversion_Complete = 0;

	// Start conversion
	ADC1->CR2 |= (1<< ADC1_CR2_SWSTART);

}

/*
 * Design ISR
 * - check for the event which triggered ISR
 * - reads ADC value
 * - sets conversion state as finished: Synchronization b/w main & ISR
 * - reading ADC result clears EOC flag
 *
 */
void ADC_IRQHandler()
{
	if(ADC1->SR & (1<< ADC1_SR_EOC))
	{

		// store result to handle
		handle_ptr->ADC_Result = ADC1->DR;

		// conversion finished
		handle_ptr->ADC_Conversion_Complete = 1;


		// Invoke call back function only if registered:
		// - Latest ADC result is passed as argument
		if(handle_ptr->function_ptr != NULL)
		{
			handle_ptr->function_ptr(handle_ptr->ADC_Result);
		}

	}
}

/*
 * Register call back function
 * - This function initializes the function pointer in handle structure
 * - address of user defined call back function is passed as argument
 *
 */

void ADC_RegisterCallback(ADC_Handle_t* pADC_Handle, void(*fn_ptr)(uint16_t result))
{
	pADC_Handle->function_ptr = fn_ptr;
}












