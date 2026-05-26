/*
 * TIMERs_Baremetal.h
 *
 *  Created on: May 23, 2026
 *      Author: hp
 */

#ifndef INC_TIMERS_BAREMETAL_H_
#define INC_TIMERS_BAREMETAL_H_

#include <stdint.h>
#include "STM32F401RE.h"



void Delay_Creation();
void Timer_Interrupt_Generation();
void Non_blocking_delay_Generation();
void PWM_Generation();
void Set_Brightness(uint16_t value );

uint16_t MapADC_To_PWM(uint16_t ADC_value);

#endif /* INC_TIMERS_BAREMETAL_H_ */
