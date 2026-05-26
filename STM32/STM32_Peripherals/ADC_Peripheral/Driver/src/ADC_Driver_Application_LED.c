
#include <stdint.h>
#include "STM32F401RE.h"
#include "STM32F401RE_ADC_Driver.h"


// Define the call back function:
// - This function is invoked by the ISR after conversion
// - decides what happens after ADC conversion
// - Receives latest ADC result as argument
// - Logic: Change state of  LED based on ADC readings

void Toggle_LED_BasedOn_ADC_Result(uint16_t result)
{
	// Turn LED ON if ADC value is larger than 4000
	if(result> 4000)
	{
		GPIOA->ODR|= (1<< 1);
	}
	else
	{
		GPIOA->ODR &=~ (1<< 1);
	}

}

int main(void)
{

	// Enable ADC clock
	ADC1_CLK_EN();

	// Declare an ADC handle & Set configurations

	ADC_Handle_t handle;
	handle.ADC_config.ADC_Prescaler   = prescaler_8;
	handle.ADC_config.ADC_Resolution  = resolution_12bit;
	handle.ADC_config.ADC_Sample_time = sampletime_144cycle;

	ADC_Init(&handle);

	// Register call back function
	ADC_RegisterCallback( &handle, Toggle_LED_BasedOn_ADC_Result );

	// GPIO Configuration:
	// ADC1 Channel 0 maps to GPIO pin PA0
	GPIO_Configuration_Channel_0();

	// Set PA1 as Output for LED
	GPIO_Configuration_LED();

	// Start first conversion
	ADC_StartConversion(&handle, channel_0);

	while(1)
	{
		// Start next conversion if previous one is over
		if(handle.ADC_Conversion_Complete)
		{

			ADC_StartConversion(&handle, channel_0);
			// After each conversions, ISR invokes the call back function to perform the logic
		}
	}


}














