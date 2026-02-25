/*
 * stm32f401re_spi_driver.h
 *
 *  Created on: Feb 23, 2026
 *      Author: hp
 */

#ifndef INC_STM32F401RE_SPI_DRIVER_H_
#define INC_STM32F401RE_SPI_DRIVER_H_

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
	uint8_t SPI_Bus_Config;    /* FULL DUPLEX OR HALF DUPLEX*/
	uint8_t SPI_DataOrder;     /* MSB OR LSB FIRST*/
	uint8_t SPI_DataFrame;     /* 8 BIT OR 16 BIT DATA */
	uint8_t SPI_SSM;           /* SOFTWARE OR HARDWARE SLAVE MANAGEMENT */


}SPI_Config_t;

/*
 * Handle structure for SPI
 *
 */
typedef struct
{
	SPI_Reg_t *pSPIx;     // for SPI register access

	SPI_Config_t SPI_Config;// for SPI configuration

	uint32_t rx_data;

	uint8_t tx_state, rx_state;

}SPI_Handle_t;
/*
 *  SPI Macros
 */
#define SPI_DEVICE_MODE_MASTER 1
#define SPI_DEVICE_MODE_SLAVE 0

#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW  0
#define SPI_CPHA_HIGH 1
#define SPI_CPHA_LOW  0

#define SPI_DFF_8BIT   0
#define SPI_DFF_16BIT  1

/*
 * API for SPI
 *
 */

void SPI_PeriClockControl(SPI_Reg_t *pSPIx, uint8_t EnorDi);
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_Reg_t *pSPIx);

void SPI_Read_InputPin(SPI_Reg_t *pSPIx, uint8_t PinNumber);
void SPI_Read_InputPort(GPIO_Reg_t *pGPIOx);


#endif /* INC_STM32F401RE_SPI_DRIVER_H_ */
