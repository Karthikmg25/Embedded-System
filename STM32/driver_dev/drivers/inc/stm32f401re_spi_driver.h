/*
 * stm32f401re_spi_driver.h
 *
 *  Created on: Feb 23, 2026
 *      Author: hp
 */

#ifndef INC_STM32F401RE_SPI_DRIVER_H_
#define INC_STM32F401RE_SPI_DRIVER_H_

#include "STM32F401RE.h"
/*
 * Configuration structure for SPI
 *
 */
typedef struct
{
	uint8_t SPI_CPOL;          /* SELECT SPI MODE*/
	uint8_t SPI_CPHA;
	uint8_t SPI_Device_Mode;   /* MASTER OR SLAVE*/
	uint8_t SPI_CLK_Speed;     /* SET PRESCALAR*/
	uint8_t SPI_Bus_Config;    /* FULL DUPLEX OR HALF DUPLEX OR SIMPLEX*/
	uint8_t SPI_DataFrame;     /* 8 BIT OR 16 BIT DATA*/
	uint8_t SPI_SSM;           /* SOFTWARE SLAVE MANAGEMENT */


}SPI_Config_t;

/*
 * Handle structure for SPI
 *
 */
typedef struct
{
	SPI_Reg_t *pSPIx;              /* SELECT SPI1/SP12/SP3/SPI4 */

	SPI_Config_t SPI_Config;       /* STORE CONFIGURATIONS */

	uint8_t *pRxBuffer;            /* STORE RECEIVED BUFER */
	uint8_t  Rx_len;               /* STORE RECEIVE LENGTH */

	uint8_t Tx_State;              /* TRANSMISSION STATE  */
	uint8_t Rx_State;              /* RECEIVING STATE     */

}SPI_Handle_t;

/*
 *  SPI Macros
 */

#define SPI_DEVICE_MODE_MASTER   1
#define SPI_DEVICE_MODE_SLAVE    0

#define SPI_CPOL_HIGH            1
#define SPI_CPOL_LOW             0
#define SPI_CPHA_HIGH            1
#define SPI_CPHA_LOW             0

#define SPI_BUS_CONFIG_FD          0
#define SPI_BUS_CONFIG_HD          1
#define SPI_BUS_CONFIG_SIMPLEX_RX  2


#define SPI_MSB_FIRST  0
#define SPI_LSB_FIRST  1

#define SPI_SSM_DI  0        /* HARDWARE NSS*/
#define SPI_SSM_EN  1        /* SOFTWARE NSS*/

#define SPI_DFF_8BIT   0
#define SPI_DFF_16BIT  1

#define SPI_STATE_BUSY    0
#define SPI_STATE_READY   1

#define SPI_PRESCALAR_2     0
#define SPI_PRESCALAR_4     1
#define SPI_PRESCALAR_8     2
#define SPI_PRESCALAR_16    3
#define SPI_PRESCALAR_32    4
#define SPI_PRESCALAR_64    5
#define SPI_PRESCALAR_128   6
#define SPI_PRESCALAR_256   7


/*
 * APIs for SPI
 *
 */

void SPI_PeriClockControl(SPI_Reg_t *pSPIx, uint8_t EnorDi);
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_Reg_t *pSPIx);

void SPI_Transmit_Buffer(SPI_Reg_t *pSPIx, uint8_t *pTxBuffer, uint8_t Tx_len);
void SPI_Recieve_Buffer(SPI_Reg_t *pSPIx, uint8_t *pRxBuffer, uint8_t Rx_len);

void SPI_PeripheralControl(SPI_Reg_t *pSPIx, uint8_t EnorDi);
uint8_t SPI_Get_FlagStatus(SPI_Reg_t *pSPIx, uint8_t Flag);
void SPI_ClearOVRFlag(SPI_Reg_t *pSPIx);
/*
 * Important Bits in SPI
 */
//@ Control bits
#define SPI_CR1_CPHA              0   /* CLOCK PHASE                                  : SETS SPI CLOCK MODE                                         */
#define SPI_CR1_CPOL              1   /* CLOCK POLARITY                               : SETS SPI CLOCK MODE                                         */
#define SPI_CR1_MSTR              2   /* MASTER MODE                                  : SETS DEVICE MODE                                            */
#define SPI_CR1_BR                3   /* PRESCLAR BITS[2:0]                           : SETS SPI SPEED                                              */
#define SPI_CR1_SPE               6   /* ENABLE SPI                                                                                                 */
#define SPI_CR1_LSBFIRST          7   /* DATA ORDER                                   : MSB OR LSB FIRST                                            */
#define SPI_CR1_SSI               8   /* INTERNAL SLAVE SELECT                        : SET AS HIGH IN SSM + MASTER MODE                            */
#define SPI_CR1_SSM               9   /* SOFTWARE SLAVE MANAGEMENT                    : SLAVE SELECTION IN SOFTWARE                                 */
#define SPI_CR1_RXONLY            10  /* RECEIVE ONLY                                 : MISO ACTIVE, MOSI IGNORED                                   */
#define SPI_CR1_DFF               11  /* DATA FRAME FORMAT                            : 8 BIT OR 16 BIT                                             */
#define SPI_CR1_BIDIMODE          15  /* BIDIRECTIONAL MODE                           : SETS HALF DUPLEX MODE                                       */
#define SPI_CR1_BIDIOE            14  /* OUTPUT ENABLE IN HALF DUPLEX IMODE           : SELECT TRANSMIT OR RECEIVE                                  */
//@Flag bits
#define SPI_SR_BSY                7   /* BUSY FLAG                                    : GOES LOW AFTER COMMUNICATION, CAN DISABLE SPI               */
#define SPI_SR_RXNE               0   /* RECEIVE BUFFER NOT EMPTY                     : GOES HIGH WHEN DATA RECEIVED, CAN READ DATA                 */
#define SPI_SR_TXE                1   /* TRANSMIT BUFFER EMPTY                        : GOES HIGH WHEN TRANSMIT BUFFER IS EMPTY, CAN WRITE NEW DATA */
#define SPI_SR_OVR                6   /* OVERRUN FLAG                                 : GOES 1 WHEN NEW DATA ARRIVES BEFORE CLEARING RXNE           */
#define SPI_SR_MODF               5   /* MODE FAULT                                   : GOES 1 WHEN NSS=0 IN HARDWARE MODE OR SSI=0 IN SOFTWARE MODE*/


#endif /* INC_STM32F401RE_SPI_DRIVER_H_ */
