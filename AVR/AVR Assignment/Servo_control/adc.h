/*
 * adc.h
 *
 * Created: 17-05-2026 11:43:13
 *  Author: hp
 */ 


#ifndef ADC_H_
#define ADC_H_

void Init_ADC();
uint16_t Get_ADC_value();
uint16_t Map_ADC_To_PulseWidth(uint16_t ADC_value);


#endif /* ADC_H_ */