/*
 * stm32f401re_adc_driver.h
 *
 *  Created on: Apr 10, 2026
 *      Author: hp
 */

#ifndef INC_STM32F401RE_ADC_DRIVER_H_
#define INC_STM32F401RE_ADC_DRIVER_H_

#include "STM32F401RE.h"
/*
 *  ADC Configuration Structure
 */
typedef struct
{
	uint8_t Resolution;           /* 12_BIT/ 10_BIT/ 8_BIT/ 6_BIT */
	uint8_t Prescalar;            /* DIV_2/ DIV_4/ DIV_6/ DIV_8  */
	uint8_t Num_of_Channels;      /* 16 REGULAR CHANNELS*/
	uint8_t Channels[16];         /* 0-15*/
	uint8_t Sampling_Time[16];    /* FOR EACH CHANNEL*/
	uint8_t Conversion_Mode;      /* SINGLE/ CONTINUOUS*/

}ADC_Config_t;
/*
 *  ADC Handle Structure
 */
typedef struct ADC_Handle_t
{
	ADC_Config_t ADC_Config;      /* ADC CONFIGURATIONS*/
	volatile uint32_t Data[16];   /* Buffer Storing Results per channel*/
	volatile uint8_t Index;       /* Index of Data buffer : Part of ADC state*/
	volatile uint8_t  State;      /* Software State  : For ISR-main synchronization*/

	                              // SINGLE mode     : Indicates conversion status : BUSY->conversion ongoing/ READY->data available
	                              // CONTINUOUS mode : ADC runs continuously. So, State Indicates buffer/sequence completion (no conversion status)
	// Call Back function
	// Invoked when sequence/buffer is completed
	void(*pCallback)( struct ADC_Handle_t *pHandle);

}ADC_Handle_t;

/*
 *  ADC General APIs
 */
void ADC_Peripheral_Control(ADC_Handle_t* pADCHandle, uint8_t EnorDi);
void ADC_Init(ADC_Handle_t* pADCHandle);
void ADC_Peripheral_Reset();
void ADC_StartConversion(ADC_Handle_t* pADCHandle);
uint8_t ADC_Get_status(uint8_t Flag);

/*
 *  ADC APIs for Interrupt Mode
 */

// For Callback registration
void ADC_Register_CallBack(ADC_Handle_t* pADC_Handle, void (*pCallback)(ADC_Handle_t *pADC_Handle));
// For registering global pointer
void ADC_Register_GlobalPointer(ADC_Handle_t* pADC_Handle);


/*
 *  ADC APIs for Polling Mode
 */
// For single channel read : Returns the ADC value
uint32_t ADC_Read_Data(ADC_Handle_t* pADCHandle);
// For Multi-Channel read : Fills the Handle Buffer
void ADC_Fill_Buffer(ADC_Handle_t* pADCHandle);


uint8_t driver_testing(ADC_Handle_t* pADCHandle);
/*
 *  ADC Macros
 */

//@ ADC Pre-scalar
#define ADC_PRESCALAR_2             0
#define ADC_PRESCALAR_4             1
#define ADC_PRESCALAR_6             2
#define ADC_PRESCALAR_8             3

//@ ADC Resolution
#define ADC_RESOLUTION_12_BIT        0
#define ADC_RESOLUTION_10_BIT        1
#define ADC_RESOLUTION_8_BIT         2
#define ADC_RESOLUTION_6_BIT         3

//@ ADC Sampling Time
#define ADC_CYCLES_3                 0
#define ADC_CYCLES_15                1
#define ADC_CYCLES_28                2
#define ADC_CYCLES_56                3
#define ADC_CYCLES_84                4
#define ADC_CYCLES_112               5
#define ADC_CYCLES_144               6
#define ADC_CYCLES_480               7

/*
 * ADC Modes
 */

//@ ADC Conversion Modes
#define ADC_SINGLE_CONVERSION        0
#define ADC_CONTINUOUS_CONVERSION    1

//@ ADC States
#define ADC_BUSY                    0
#define ADC_READY                   1
#define ADC_IDLE                    2

//@ ADC Regular Channels
#define CHANNEL_0              0
#define CHANNEL_1              1
#define CHANNEL_2              2
#define CHANNEL_3              3
#define CHANNEL_4              4
#define CHANNEL_5              5
#define CHANNEL_6              6
#define CHANNEL_7              7
#define CHANNEL_8              8
#define CHANNEL_9              9
#define CHANNEL_10             10
#define CHANNEL_11             11
#define CHANNEL_12             12
#define CHANNEL_13             13
#define CHANNEL_14             14
#define CHANNEL_15             15

/*
 * Important bits in ADC
 */
#define ADC1_CR1_RES               24      /* RESOLUTION BITS_25:24 */
#define ADC1_CR1_SCAN              8       /* SCAN MODE */
#define ADC1_CR1_EOCIE             5       /* INTERRUPT ENABLE AFTER CONVERSION */
#define ADC1_CR2_EOCS              10      /* END OF CONVERSION SELECTION  */
#define ADC1_CR2_ADON              0       /* ENABLE ADC */
#define ADC1_CR2_CONT              1       /* CONTINUOUS CONVERSION MODE */
#define ADC1_CR2_SWSTART           30      /* START CONVERSION */
#define ADC1_SQR1_L                20      /* SEQUENCE LENGTH_23:20*/
#define ADC_CCR_ADCPRE             16      /* PRESCALAR BITS 17:16*/
/*
 * ADC Flags
 */
#define ADC1_SR_EOC                (1<<1)       /* END OF CONVERSION*/
#define ADC1_SR_STRT               (1<<4)       /* START OF CONVERSION*/
#define ADC1_SR_OVR                (1<<5)       /* OVERRUN*/



#endif /* INC_STM32F401RE_ADC_DRIVER_H_ */
