/*
 * stm32f401re_adc_driver.c
 *
 *  Created on: Apr 10, 2026
 *      Author: hp
 */


#include <stdint.h>
#include <stddef.h>
#include "stm32f401re_gpio_driver.h"
#include "stm32f401re_adc_driver.h"
#include "HCSR04_Driver.h"

static ADC_Handle_t* gpADCHandle=NULL;     /* USE A GLOBAL-STATIC POINTER FOR ISR TO ACCESS THE HANDLE STRUCTURE*/
                                      /* HARDWARE EVENT DIRECLY MODIFIES STRUCTURE ELEMENTS USING THIS POINTER */
                                      /* ENSURES ISR-MAIN SYNCHRONIZATION */

/**************************************************************************************************************************************************************************
 * @fn                - ADC_Init
 *
 * @brief             - This function initializes ADC with required configurations
 *
 * @param[in]         - pADCHandle
 *
 * @return            - none
 *
 * @Note              - none
 */
void ADC_Init(ADC_Handle_t* pADCHandle)
{


    // Initialize Call back function to NULL
    pADCHandle->pCallback= NULL;

    // Discard any previous data in DR
    // Clear EOC flag to avoid immediate false trigger
    // Ensure ADC starts in a clean state:
    (void)ADC1->DR;
    (void)ADC1->SR;

	// Variables for selecting Register and Bit positions of each channel
	uint32_t temp1;
    uint32_t temp2;
                                                                        /* INITIALISATION STEPS*/

	ADC1_CLK_EN();                                                                                      /* ENABLE CLOCK FOR ADC*/

	ADC1->CR1 &=~(3<<ADC1_CR1_RES);// clear bits 25:24                                                  /* SET RESOLUTION*/
	ADC1->CR1 |=(pADCHandle->ADC_Config.Resolution<<ADC1_CR1_RES);
	                                                                                                    /* SET PRESCALAR*/
	ADC_COMMON->CCR &=~(3 << ADC_CCR_ADCPRE);// Clear bits
	ADC_COMMON->CCR |=(pADCHandle->ADC_Config.Prescalar << ADC_CCR_ADCPRE);
                                                                                                        /* SET SEQUENCE LENGTH (INDICATES NUMBER OF CHANNELS)*/
    ADC1->SQR1 &=~ (0xF<< ADC1_SQR1_L);// clear bits
    ADC1->SQR1 |=  ((pADCHandle->ADC_Config.Num_of_Channels-1)<< ADC1_SQR1_L);

                                                                                                        /* ENEBLE SCAN MODE IF MULTIPLE CHANNELS*/
    if(pADCHandle->ADC_Config.Num_of_Channels > 1)
    {
      	// SCAN Mode Enabled
        ADC1->CR1|=(1<<ADC1_CR1_SCAN );                                                                 /* SET EOCS HIGH, THUS EOC INDICATES END OF CONVERSION */
        // EOC is End of Conversion
        ADC1->CR2|=(1<<ADC1_CR2_EOCS );
    }
    else
    {
    	// SCAN Mode Disabled
    	ADC1->CR1&=~(1<<ADC1_CR1_SCAN );
    	// EOC is End of Sequence
        ADC1->CR2&=~(1<<ADC1_CR2_EOCS );
    }


                                                                                                         /* SELECT CHANNELS */
    // Set all selected channels in the array in SQR Registers
    // SQR Registers are set based on Channel order,
    // not based on channel number (eg: channel_8 can be set as first in the sequence)

    for(uint8_t i=0; i<pADCHandle->ADC_Config.Num_of_Channels;i++)
    {
    	temp1= i / 6;  // 1 SQR register for 6 channels
    	temp2= i % 6;

    	if(temp1==0)// if number of channels = 1-6, use SQR3
    	{
    		ADC1->SQR3 &=~(0x1F << temp2*5);// clear 5 bits
    		ADC1->SQR3 |=(pADCHandle->ADC_Config.Channels[i]  << temp2*5);
    	}
    	else if(temp1==1)// if number of channels = 7-12, use SQR2
    	{
    		ADC1->SQR2 &=~(0x1F << temp2*5);// clear 5 bits
    		ADC1->SQR2 |=(pADCHandle->ADC_Config.Channels[i]  << temp2*5);
    	}
    	else if(temp1==2)// if number of channels = 13-16, use SQR1
    	{
    	    ADC1->SQR1 &=~(0x1F << temp2*5);// clear 5 bits
    	    ADC1->SQR1 |=(pADCHandle->ADC_Config.Channels[i]  << temp2*5);
    	}
    }
	                                                                                                    /* SET SAMPLING TIME*/
    // Set in SMPR registers based on channel number(0-15)

	for(uint8_t i=0; i<pADCHandle->ADC_Config.Num_of_Channels;i++)
	{
		temp1= pADCHandle->ADC_Config.Channels[i] /10;  // 1 SMPR register for 10 channels
		temp2= pADCHandle->ADC_Config.Channels[i] %10;

		if(temp1==0)                                    // Use SMPR2 for channels 0-9
		{
			ADC1->SMPR2 &=~(7<<(temp2*3));// clear
			ADC1->SMPR2 |= ((pADCHandle->ADC_Config.Sampling_Time[i])<<(temp2*3));
		}
		else                                            // Use SMPR1 for channels 10-15
		{
			ADC1->SMPR1 &=~(7<<(temp2*3));// clear
	        ADC1->SMPR1 |= ((pADCHandle->ADC_Config.Sampling_Time[i])<<(temp2*3));
		}
	}
                                                                                                          /* SET INTERRUPT */

		// Enable ADC interrupt :
		// Triggers after each conversions
		// Enable NVIC

		NVIC->ISER[0]|=(1<<18 );
		ADC1->CR1 |= (1<<ADC1_CR1_EOCIE);

                                                                                                            /* SELECT CONVERSION MODE*/
        // single Or Continuous conversion

        ADC1->CR2&=~(1<<ADC1_CR2_CONT);// clear bit
        ADC1->CR2|=(pADCHandle->ADC_Config.Conversion_Mode<<ADC1_CR2_CONT);



}
/******************************************************************************************************************************************************************
 * @fn                - ADC_Peripheral_Control
 *
 * @brief             - This function is used to Enable or Disable the peripheral
 *
 * @param[in]         - EnorDi: Enable Or Disable
 *
 * @return            - none
 *
 * @Note              - This API Should be called after ADC initialization for enabling ADC peripheral
 */
void ADC_Peripheral_Control(ADC_Handle_t* pADCHandle, uint8_t EnorDi)
{
	// Enable Or Disable based on argument
	if(EnorDi == ENABLE)
	{
		ADC1->CR2 |=(1<<ADC1_CR2_ADON);

		// After ADC Enable, the Peripheral needs a short stabilization time,
        // This ensure the next conversion is valid
	    // Stabilization delay
		Delay_us(10);
	}
	else
	{
		ADC1->CR2 &=~(1<<ADC1_CR2_ADON);
	}

}
void ADC_Register_GlobalPointer(ADC_Handle_t* pADC_Handle)
{
	gpADCHandle= pADC_Handle;
}

/******************************************************************************************************************************************************************
 * @fn                - ADC_Peripheral_Reset
 *
 * @brief             - This function is used to Reset the peripheral
 *
 * @param[in]         - none
 *
 * @return            - none
 *
 * @Note              - This API Should be called before ADC initialization for peripheral reset
 */
void ADC_Peripheral_Reset()
{
	RCC->APB2RSTR |= (1 << 8);// peripheral Reset
	RCC->APB2RSTR &= ~(1 << 8); // release Reset
}
uint8_t ADC_Get_status(uint8_t Flag)
{
	uint8_t flag_state= (ADC1->SR & Flag);
	if( flag_state)
	{return 1;}
	else
	{return 0;}
}
void ADC_StartConversion(ADC_Handle_t* pADCHandle)
{

    // Initialize buffer index to 0
	pADCHandle->Index=0;

	pADCHandle->State= ADC_BUSY;  // Initialize State

	ADC1->CR2 |=(1<<ADC1_CR2_SWSTART);// start conversion
}



/******************************************************************************************************************************************************************
 * @fn                - ADC_IRQHandler : Interrupt Service Routine
 *
 * @brief             - This ISR is triggered after end of each conversions when Interrupt is enabled
 *
 * @Note              - ISR fills the ADC data buffer the handle sequentially after each conversion
 *                      When sequence finishes, it resets the index and updates the state to indicate the buffer is ready.
 */

void ADC_IRQHandler(void)
{
	if(ADC1->SR& ADC1_SR_EOC )//conversion complete
	{                                                                                         /* ISR UPDATES HE HANDLE DIRECTLY VIA POINTER */

		if(gpADCHandle == NULL) {return;}// return if global pointer is not initialized

		if(gpADCHandle->Index < gpADCHandle->ADC_Config.Num_of_Channels)
		{
			gpADCHandle->Data[gpADCHandle->Index]= ADC1->DR;// read value
			gpADCHandle->Index++;                           // Update Index
		}
		else
		{
			gpADCHandle->Index = 0;// reset index in case of overflow
		}


		// After sequence finish
		if(gpADCHandle->Index >= gpADCHandle->ADC_Config.Num_of_Channels)                      /* STORE DATA INSIDE A DATA BUFFER */
		{
			gpADCHandle->Index=0;// Reset Index back to 0
			gpADCHandle->State= ADC_READY;//SEQUENCE COMPLETE : BUFFER READY TO READ

			// Call the Callback function after sequence complete
			// Check if user has a call back registered
			if(gpADCHandle->pCallback != NULL)
			{
				gpADCHandle->pCallback(gpADCHandle);
			}
		}

	}
}
/******************************************************************************************************************************************************************
 * @fn                - ADC_Register_CallBack
 *
 * @brief             - This function is used to register the callback function if user have any
 *
 * @param[in]         - pADC_Handle: Pointer to ADC Handle
 * @param[in]         - pCallback  : Function Pointer to Callback function
 *
 * @return            - none
 *
 * @Note              - This API Should be called after ADC initialization because the Initialization resets pointer to NULL
 */

void ADC_Register_CallBack(ADC_Handle_t* pADC_Handle, void (*pCallback)(ADC_Handle_t *pADC_Handle))
{
	pADC_Handle->pCallback = pCallback;
}


/*
 *
 * POLLING MODE APIS : FOR LATER IMPLIMENTATION
 *
 */
/******************************************************************************************************************************************************************
 * @fn                - ADC_Read_Data
 *
 * @brief             - This function returns ADC value after a single conversion
 *
 * @param[in]         - none
 *
 * @return            - ADC value
 *
 * @Note              - This API is used in Polling Mode for Single channel conversion
 */
uint32_t ADC_Read_Data(ADC_Handle_t* pADCHandle)
{
	// Wait till End of Conversion and Read Data
	// Switch state to Ready after Conversion
	// Store data to handle buffer
	while(!(ADC1->SR & ADC1_SR_EOC));

	pADCHandle->State= ADC_READY;

	uint32_t result= ADC1->DR;

	//pADCHandle->Data[0]= result;

	return result;
}

/******************************************************************************************************************************************************************
 * @fn                - ADC_Fill_Buffer
 *
 * @brief             - This API is used in Polling Mode for Multi-channel conversion
 *
 * @param[in]         - pADCHandle : Pointer to Handle structure
 *
 * @return            - none
 *
 * @Note              - This function fills the ADC data buffer the handle sequentially after each conversion
 *                      When sequence finishes, it resets the index and updates the state to indicate the buffer is ready.
 */
void ADC_Fill_Buffer(ADC_Handle_t* pADCHandle)
{
	// Wait till End of Conversion of each channel
    while(!(ADC1->SR & ADC1_SR_EOC));

    // store results in Handle Buffer
    pADCHandle->Data[pADCHandle->Index++]= ADC1->DR;

    // Reset index if Sequence finishes
    if(pADCHandle->Index >= pADCHandle->ADC_Config.Num_of_Channels)
    {
    	pADCHandle->Index=0;
    	pADCHandle->State= ADC_READY;//SEQUENCE COMPLETE : BUFFER READY TO READ
    }

}

uint8_t driver_testing(ADC_Handle_t* pADCHandle)
{
	if(gpADCHandle==pADCHandle)
	{
		return 1;
	}
	else
		return 0;
}
