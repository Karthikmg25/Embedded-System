/*
 * stm32f401re_i2c_driver.h
 *
 *  Created on: Mar 14, 2026
 *      Author: hp
 */

#ifndef INC_STM32F401RE_I2C_DRIVER_H_
#define INC_STM32F401RE_I2C_DRIVER_H_

#include "STM32F401RE.h"

/*
 * Configuration structure for I2C instance
 *
 */
typedef struct
{
	uint32_t I2C_SCL_Speed;  /* STANDARD OR FAST MODE*/
	uint8_t  I2C_Device_Address; /* IN SLAVE MODE */
	uint8_t  I2C_SCL_DutyCycle;  /* IN FAST MODE*/

}I2C_Config_t;
/*
 * Handle structure for I2C Instance
 *
 */
typedef struct
{
	I2C_Reg_t *pI2Cx;

	I2C_Config_t I2C_Config;

}I2C_Handle_t;

/*
 * APIs for I2C
 *
 */

void I2C_PeriClockControl(I2C_Reg_t *pI2Cx, uint8_t EnorDi);                                                            /* ENABLE CLOCK */
void I2C_PeripheralControl(I2C_Reg_t *pI2Cx, uint8_t EnorDi);                                                           /* ENBLE OR DISABLE PERIPHERAL*/

void I2C_Init(I2C_Handle_t *pI2C_Handle);                                                                               /* INITIALIZE CONFIGURATIONS */
void I2C_Transmit_Byte(I2C_Reg_t *pI2Cx, uint8_t slv_addr,uint8_t Reg_addr, uint8_t byte);                              /* TRANSMITS A BYTE */
uint8_t I2C_Receive_Byte(I2C_Reg_t *pI2Cx, uint8_t slv_addr,uint8_t Reg_addr);                                          /* RECEIVES A BYTE*/
void I2C_Transmit_Buffer(I2C_Reg_t *pI2Cx, uint8_t slv_addr,uint8_t Reg_addr, uint8_t *Tx_Buffer, uint8_t Tx_Len);      /* TRANSMITS A BUFFER*/
void I2C_Receive_Buffer(I2C_Reg_t *pI2Cx, uint8_t slv_addr,uint8_t Reg_addr, uint8_t *Rx_Buffer, uint8_t Rx_Len);       /* RECEIVES A BUFFER*/
/*
 * Other Peripheral control APIs
 */
void Get_FlagStatus(I2C_Reg_t *pI2Cx, uint32_t Flag);

/*
 * Peripheral Clock Frequency
 */
#define F_PCLK             16000000U     /*16MHz*/

/*
 * I2C SCL Speeds
 */
#define I2C_SCL_SPEED_SM    100000  /*STANDARD MODE :100KHz*/
#define I2C_SCL_SPEED_FM    400000  /*FAST MODE     : 400KHz*/
/*
 * I2C ACKControl
 */
#define I2C_ACK_ENABLE   0
#define I2C_ACK_DISABLE  1
/*
 * I2C Duty cycles in Fast mode
 */
#define I2C_FM_DUTY_2       0  /* LOW_TIME : HIGH_TIME = 2:1*/
#define I2C_FM_DUTY_16_9    1  /* LOW_TIME : HIGH_TIME = 16:9*/

/*
 * I2C Control Bits
 */
#define I2C_CR1_PE          0   /* PERIPHERAL ENABLE */
#define I2C_CR1_ENGC        6   /* GENERAL CALL ENABLE */
#define I2C_CR1_START       8   /* GENERATE START */
#define I2C_CR1_STOP        9   /* GENERATE STOP */
#define I2C_CR1_ACK         10   /* ACKNOWLEDGMENT ENABLE */
#define I2C_CR1_POS         11   /* ACK CONTROL FOR 2 BYTE RECEPTION */
#define I2C_CR2_FREQ        0   /* PERIPHERAL CLOCK FREQUENCY : 5:0 */
#define I2C_OAR1_ADD        1  /* 7 BIT ADDRESS 7:1 */
#define I2C_CCR_FS          15  /* SCL SPPED*/
#define I2C_CCR_DUTY        14  /* SCL DUTY CYCLE*/
/*
 * I2C Flags
 */
#define I2C_SR1_SB         (1<<0)   /* START GENERATED*/
#define I2C_SR1_ADDR       (1<<1)   /* ADDRESS MATCHED*/
#define I2C_SR1_BTF        (1<<2)   /* END OF TRANSMISSION*/
#define I2C_SR1_STOPF      (1<<4)   /* STOP GENERATED*/
#define I2C_SR1_RxNE       (1<<6)   /* DATA RECEIVED IN DATA REGISTER*/
#define I2C_SR1_TxE        (1<<7)   /* DATA REGISTER EMPTY*/
#define I2C_SR1_AF         (1<<10)  /* ACKNOWLEDGMENT FAULT*/
#define I2C_SR1_OVR        (1<<11)  /* OVERRUN ERROR*/


#endif /* INC_STM32F401RE_I2C_DRIVER_H_ */
