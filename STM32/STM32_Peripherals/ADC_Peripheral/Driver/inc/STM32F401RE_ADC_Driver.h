/*
 * STM32F401RE_ADC_Driver.h
 *
 *  Created on: May 25, 2026
 *      Author: hp
 */

#ifndef INC_STM32F401RE_ADC_DRIVER_H_
#define INC_STM32F401RE_ADC_DRIVER_H_

#include <stdint.h>
#include <stddef.h>
#include "STM32F401RE.h"
#include "ADC_Baremetal.h"

// Define enums for configurables

typedef enum
{
   channel_0,Channel_1, channel_2, channel_3,
   channel_4,channel_5, channel_6, channel_7,
   channel_8,channel_9, channel_10, channel_11,
   channel_12,channel_13, channel_14, channel_15
}Channel_t;

typedef enum
{
	resolution_12bit, resolution_10bit,
	resolution_8bit, resolution_6bit

}Resolution_t;

typedef enum
{
	prescaler_2, prescaler_4,
	prescaler_6, prescaler_8

}Prescaler_t;

typedef enum
{
	sampletime_3cycle, sampletime_15cycle,
	sampletimer_28cycle, sampletime_56cycle,
	sampletime_84cycle, sampletime_112cycle,
	sampletime_144cycle, sampletime_480cycle

}Sample_time_t;

// Define Configuration structure
typedef struct
{
	Resolution_t ADC_Resolution;
	Prescaler_t ADC_Prescaler;
	Sample_time_t ADC_Sample_time;

}ADC_Config_t;

// Define Handle structure
// - Stores ADC hardware instance information
// - Stores latest ADC result
// - Stores Conversion Complete status
// - Stores configurations
// - Stores Function pointer for call back support
typedef struct
{
	volatile uint16_t ADC_Result;

	volatile uint8_t ADC_Conversion_Complete;

	ADC_Config_t ADC_config;

	void (*function_ptr)(uint16_t result);

}ADC_Handle_t;

/*
 *  Driver APIs
 *
 */
void ADC_Init(ADC_Handle_t* pADC_handle);
void ADC_StartConversion(ADC_Handle_t* pADC_Handle, Channel_t channel);
void ADC_RegisterCallback(ADC_Handle_t* pADC_Handle, void(*fn_ptr)(uint16_t result));


#endif /* INC_STM32F401RE_ADC_DRIVER_H_ */
































