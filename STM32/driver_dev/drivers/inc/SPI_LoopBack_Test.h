/*
 * SPI_LoopBack_Test.h
 *
 *  Created on: Feb 27, 2026
 *      Author: hp
 */

#ifndef INC_SPI_LOOPBACK_TEST_H_
#define INC_SPI_LOOPBACK_TEST_H_

#include "stm32f401re_gpio_driver.h"
#include "stm32f401re_spi_driver.h"
#include "GPIO_Assignment.h"

void SPI1_GPIO_Init();
void SPI1_Init();
void SPI_LoopBack_Test();

#endif /* INC_SPI_LOOPBACK_TEST_H_ */
