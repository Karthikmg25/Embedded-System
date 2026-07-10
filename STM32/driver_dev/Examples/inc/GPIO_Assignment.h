/*
 * GPIO_Assignment.h
 *
 *  Created on: Feb 27, 2026
 *      Author: hp
 */

#ifndef INC_GPIO_ASSIGNMENT_H_
#define INC_GPIO_ASSIGNMENT_H_

#include <stdint.h>
#include "stm32f401re_gpio_driver.h"


void GPIO_init();
void GPIO_Toggle_ButtonRead();
void led_ON();
void led_OFF();

#endif /* INC_GPIO_ASSIGNMENT_H_ */
