

#include <stdint.h>
#include "STM32F401RE.h"
#include "STM32F401RE_ADC_Driver.h"
#include "TIMERs_Baremetal.h"


// User-defined callback functions:
// - This function is invoked by the ISR after each conversion
// - Defines application behavior0
// - Receives latest ADC result as argument
// - Logic: Change state of  LED based on ADC readings

void Toggle_LED_BasedOn_ADC_Result(uint16_t result)
{
	// Turn LED ON if ADC value is larger than threshold
	if(result> 2500)
	{
		GPIOA->ODR|= (1<< 1);
	}
	else
	{
		GPIOA->ODR &=~ (1<< 1);
	}

}
void UpdateBrightness_From_ADC(uint16_t result)
{
	// Convert ADC value to set PWM duty cycle
	uint16_t duty = MapADC_To_PWM(result);

	Set_Brightness(duty);
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
	ADC_RegisterCallback( &handle,  UpdateBrightness_From_ADC );

	// GPIO Configuration:
	// ADC1 Channel 0 maps to GPIO pin PA0
	GPIO_Configuration_Channel_0();

	// Set PA1 as Output for LED
	GPIO_Configuration_LED();

	// Set Timer  for PWM generation
	PWM_Generation();

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






















