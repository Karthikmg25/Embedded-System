/*
 * ADC_Baremetal.h
 *
 *  Created on: May 25, 2026
 *      Author: hp
 */

#ifndef INC_ADC_BAREMETAL_H_
#define INC_ADC_BAREMETAL_H_

#include<stdint.h>

void ADC_Configuration();
void ADC_Start_Conversion();
uint16_t ADC_GetResult();
void Toggle_LED_BasedOn_ADC(uint16_t result);
void GPIO_Configuration_Channel_0();
void GPIO_Configuration_LED();

#endif /* INC_ADC_BAREMETAL_H_ */
