/*
 * HCSR04_Driver.h
 *
 *  Created on: Mar 22, 2026
 *      Author: hp
 */

#ifndef INC_HCSR04_DRIVER_H_
#define INC_HCSR04_DRIVER_H_

#include "STM32F401RE.h"
#include "stm32f401re_gpio_driver.h"

#define OUTPUT_PIN   0
#define TRIGGER_PIN  1
#define ECHO_PIN     4

void SysTick_Delay_init();

void Delay_us(uint32_t time);

void Timer2_Init();

void GPIO_setup();

//@helpers
void Send_Trigger();
void Buzzer_ON();
void Buzzer_OFF();
#endif /* INC_HCSR04_DRIVER_H_ */
