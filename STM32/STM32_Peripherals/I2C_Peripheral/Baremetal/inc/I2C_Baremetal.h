/*
 * I2C_Baremetal.h
 *
 *  Created on: May 31, 2026
 *      Author: hp
 */

#ifndef INC_I2C_BAREMETAL_H_
#define INC_I2C_BAREMETAL_H_

#include <stdint.h>
#include "STM32F401RE.h"
#include "STM32F401RE_USART_Driver.h"
#include "USART_Baremetal.h"
#define DEBUG_LED  (GPIOA->ODR |= (1<<0))


void I2C_Initialization();

void GPIO_Configurations_SCL_SDA();

void generate_start(I2C_Reg_t* I2Cx);

void send_address_write(I2C_Reg_t* I2Cx, uint8_t slv_address);

void clear_ADDR_flag(I2C_Reg_t* I2Cx);

void send_data(I2C_Reg_t* I2Cx, uint8_t data);

uint8_t check_status_flag(I2C_Reg_t* I2Cx, uint32_t flag);

void send_address_read(I2C_Reg_t* I2Cx, uint8_t slv_address);

void set_ACK(I2C_Reg_t* I2Cx);

void set_NACK(I2C_Reg_t* I2Cx);

void generate_stop(I2C_Reg_t* I2Cx);

uint8_t read_data(I2C_Reg_t* I2Cx);

void wait_till_transfer(I2C_Reg_t* I2Cx);

void set_POS(I2C_Reg_t* I2Cx);

void clear_POS(I2C_Reg_t* I2Cx);


void I2C_Application(void);





#endif /* INC_I2C_BAREMETAL_H_ */
