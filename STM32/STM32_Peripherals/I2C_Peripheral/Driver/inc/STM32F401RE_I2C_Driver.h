/*
 * STM32F401RE_I2C_Driver.h
 *
 *  Created on: May 31, 2026
 *      Author: hp
 */

#ifndef INC_STM32F401RE_I2C_DRIVER_H_
#define INC_STM32F401RE_I2C_DRIVER_H_

#include "I2C_Baremetal.h"

// define enums for configurations
typedef enum
{
	FastMode_duty_2,
	FastMode_duty_16_9
}FM_Duty_t;
typedef enum
{
	Standard_Mode,
	Fast_Mode
}SCL_Mode_t;

// Structure for configurations
typedef struct
{
	uint32_t I2C_SCL_Speed;
	SCL_Mode_t I2C_Speed_mode;
	FM_Duty_t  I2C_SCL_DutyCycle;

}I2C_Config_t;
// Define handle structure
typedef struct
{
	I2C_Reg_t* I2Cx;
	I2C_Config_t I2C_Config;
}I2C_Handle_t;
void I2C_Init(I2C_Handle_t* pI2C_Handle);
void I2C_Transmit_Buffer(I2C_Reg_t* I2Cx, uint8_t reg_address, uint8_t* Tx_Buff, uint8_t len, uint8_t slv_address);
void I2C_Receive_Buffer(I2C_Reg_t* I2Cx, uint8_t reg_address, uint8_t* Rx_Buff, uint8_t len, uint8_t slv_address);

#endif /* INC_STM32F401RE_I2C_DRIVER_H_ */


















