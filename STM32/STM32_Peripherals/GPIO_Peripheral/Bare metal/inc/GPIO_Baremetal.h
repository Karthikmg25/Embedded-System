/*
 * GPIO_Baremetal.h
 *
 *  Created on: May 22, 2026
 *      Author: hp
 */

#ifndef GPIO_BAREMETAL_H_
#define GPIO_BAREMETAL_H_

#include <stdint.h>
#include "STM32F401RE.h"

#define CRUDE_DELAY(x) do{\
                         for(volatile uint32_t i=0;i< (x);i++);\
                         }while(0)
void GPIO_Blink();
void GPIO_ButtonPress();
void External_Interrupt();

#endif /* GPIO_BAREMETAL_H_ */
