/*
 * SPI_LoopBack_Test.c
 *
 *  Created on: Feb 27, 2026
 *      Author: hp
 */
#include "SPI_LoopBack_Test.h"
/*
 * SPI1 Pins:
 * NSS/CS  - PA4
 * SCK     - PA5
 * MISO    - PA6
 * MOSI    - PA7
 * Alternate Function: AF5
 */

void SPI1_GPIO_Init()
{
	GPIO_Handle_t MOSI, MISO, SCK;                              /* SET ALTERNATE FUNCTION SETUP*/
	// MOSI - PA7
	MOSI.pGPIOx                           = GPIOA;
	MOSI.GPIO_PinConfig.GPIO_PinNumber    = GPIO_PIN_NO_7;
	MOSI.GPIO_PinConfig.GPIO_PinMode      = GPIO_MODE_ALTFN;
	MOSI.GPIO_PinConfig.GPIO_PINAlternate = AF5;
	MOSI.GPIO_PinConfig.GPIO_PinSpeed     = GPIO_SPEED_HIGH ;
	MOSI.GPIO_PinConfig.GPIO_PinOPType    = GPIO_OP_TYPE_PUSHPULL;
	MOSI.GPIO_PinConfig.GPIO_PinPUPD      = GPIO_NO_PUPD;
	// MISO - PA6
	MISO.pGPIOx                           = GPIOA;
	MISO.GPIO_PinConfig.GPIO_PinNumber    = GPIO_PIN_NO_6;
	MISO.GPIO_PinConfig.GPIO_PinMode      = GPIO_MODE_ALTFN;
	MISO.GPIO_PinConfig.GPIO_PINAlternate = AF5;
	MISO.GPIO_PinConfig.GPIO_PinSpeed     = GPIO_SPEED_HIGH ;
	MISO.GPIO_PinConfig.GPIO_PinOPType    = GPIO_OP_TYPE_PUSHPULL;
	MISO.GPIO_PinConfig.GPIO_PinPUPD      = GPIO_NO_PUPD;
	// SCK - PA5
	SCK.pGPIOx                           = GPIOA;
	SCK.GPIO_PinConfig.GPIO_PinNumber    = GPIO_PIN_NO_5;
	SCK.GPIO_PinConfig.GPIO_PinMode      = GPIO_MODE_ALTFN;
	SCK.GPIO_PinConfig.GPIO_PINAlternate = AF5;
	SCK.GPIO_PinConfig.GPIO_PinSpeed     = GPIO_SPEED_HIGH ;
	SCK.GPIO_PinConfig.GPIO_PinOPType    = GPIO_OP_TYPE_PUSHPULL;
	SCK.GPIO_PinConfig.GPIO_PinPUPD      = GPIO_NO_PUPD;
	                                                                 /* INITIALISE ALL PINS*/
	GPIO_Init(&MOSI);
	GPIO_Init(&MISO);
	GPIO_Init(&SCK);

}
void SPI1_Init()
{
	SPI_Handle_t SPI_1;                         /* CONFIGURE SPI 1 */

	SPI_1.pSPIx                       =  SPI1;
	SPI_1.SPI_Config.SPI_Device_Mode  =  SPI_DEVICE_MODE_MASTER;
	SPI_1.SPI_Config.SPI_CLK_Speed    =  SPI_PRESCALAR_16;
	SPI_1.SPI_Config.SPI_Bus_Config   =  SPI_BUS_CONFIG_FD ;
	SPI_1.SPI_Config.SPI_DataFrame    =  SPI_DFF_8BIT;
	SPI_1.SPI_Config.SPI_CPOL         =  SPI_CPOL_LOW;
	SPI_1.SPI_Config.SPI_CPHA         =  SPI_CPHA_LOW;
	SPI_1.SPI_Config.SPI_SSM          =  SPI_SSM_EN;

	SPI_Init(&SPI_1);                                   /* INITIALISE CONFIGURATIONS */

}
void SPI_LoopBack_Test()
{
	     SPI1_GPIO_Init();                           /* SET GPIO PINS AS SPI PINS*/

		 SPI1_Init();                                /* INITIALISE CONFIGURATIONS*/

		 SPI_PeripheralControl(SPI1, ENABLE);        /* ENABLE SPI 1 */

		 uint8_t tx_buff = 0x56;
		 uint8_t rx_buff;

		 SPI_Transmit(SPI1, &tx_buff, 1);           /* TRANSMIT BYTE 0X56 */

		 SPI_Recieve(SPI1, &rx_buff, 1);            /* RECEIVE BYTE INTO rx_buff*/

		 GPIO_init();

		 if(rx_buff == 0x56)                        /* CHECK SEND BYTE == RECEIVED BYTE */
		 {
			 led_ON();
		 }
		 else
		 {
			 led_OFF();
		 }
}
