/*
 * USART_Baremetal.h
 *
 *  Created on: May 28, 2026
 *      Author: hp
 */

#ifndef INC_USART_BAREMETAL_H_
#define INC_USART_BAREMETAL_H_

#include <stdint.h>
#include <stdlib.h>
#include "STM32F401RE.h"

void USART_Configuration(void);
void USART_Transmission(uint8_t);
uint8_t USART_Reception();
void USART_Send_String(char* string);
void USART_Send_Number(uint8_t number);
#endif /* INC_USART_BAREMETAL_H_ */
