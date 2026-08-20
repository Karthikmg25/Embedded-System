/*
 * BMP280_DriverInterface.c
 *
 *  Created on: Aug 18, 2026
 *      Author: hp
 */


#include <stdint.h>
#include <stdio.h>
#include "BMP280.h"

//Apllication is using SPI1 for interfacing the sensor


static void SPI1_Configurations();

static void SPI1_GPIO_Configurations();

static void force_delay();



int App1_main(void)
{
	//Configure GPIO pins for SPI1
	SPI1_GPIO_Configurations();

	//Configure SPI1 for communication
	SPI1_Configurations();

	//declare a BMP280 instance

	BMP280_t bmp;
	//Using SPI1 for Interface and PB6 as chip select pin:
	bmp.interface.spi = SPI1;
	bmp.interface.ss_port = GPIOB;
	bmp.interface.ss_pin  = 6;

	BMP280_Init(&bmp);

	while(1)
	{
		BMP280_ReadTemperature(&bmp);
		BMP280_ReadPressure(&bmp);

		printf("\nBMP_Interfacing\n");
		printf("_____________________\n\n");
		//For production/reusable driver code,use PRIu16, PRId16 as format specifiers(<inttype.h>)
		printf("Chip ID : 0x%0X\n"          , (int)bmp.ID);
		printf("Temperature : %.2f C\n"     , bmp.temperature);
		printf("Pressure    : %.2f Pascal\n", bmp.pressure);

		force_delay();
	}

}



static void SPI1_Configurations()
{
	//Configure SPI1
		SPI_Handle_t spi1;

		spi1.pSPIx = SPI1;
		spi1.SPI_Config.SPI_Bus_Config  = SPI_BUS_CONFIG_FD;
		spi1.SPI_Config.SPI_CLK_Speed   = SPI_PRESCALAR_16;
		spi1.SPI_Config.SPI_Device_Mode = SPI_DEVICE_MODE_MASTER;
		spi1.SPI_Config.SPI_SSM         = SPI_SSM_EN;
		spi1.SPI_Config.SPI_DataFrame   = SPI_DFF_8BIT;

		SPI_Init(&spi1);

		SPI_PeripheralControl(SPI1, ENABLE);
}

//Configure GPIO pins for SPI1 and BMP280 chip select
		// - SPI1_MOSI -> PA7 (D11)
		// - SPI1_MISO -> PA6 (D12)
		// - SPI1_SCK  -> PA5 (D13)
		// - SPI1_SS   -> PB6 (D10)

static void SPI1_GPIO_Configurations()
{

			// - use alternate function AF5
			// - set SS pin as GPIO output for software toggle
			// - set SS as default HIGH (inactive)
			// - set HIGH speed (SPI is fast)

	GPIOA_CLK_EN();
	GPIOB_CLK_EN();
	// set alternate function mode ater clearing bits
	GPIOA->MODER   &= ~((0x3<< 7*2)| (0x3<< 6*2)|(0x3<< 5*2));
	GPIOA->MODER   |= (0x2<< 7*2)| (0x2<< 6*2)|(0x2<< 5*2);
	// set alternate function for PA7,PA6,PA5
	GPIOA->AFR[0]  &= ~((0xF<< 7*4)|(0xF<< 6*4)|(0xF<< 5*4));
	GPIOA->AFR[0]  |= (0x5<< 7*4)|(0x5<< 6*4)|(0x5<< 5*4);


	// set PB6 as output pin for slave control
	GPIOB->MODER   &= ~(0x3<< 6*2);
	GPIOB->MODER   |= (1<< 6*2);
	// set PB6 as default high
	GPIOB->ODR     |= (1<< 6);

	// set HIGH speed for SPI pins
	GPIOA->OSPEEDR &= ~((0x3<< 7*2)| (0x3<< 6*2)|(0x3<< 5*2));
	GPIOA->OSPEEDR |= (0x3<< 7*2)| (0x3<< 6*2)|(0x3<< 5*2);

	// no pullup or pulldown
	GPIOA->PUPDR &=~((0x3<< 5*2)|(0x3<< 6*2)|(0x3<< 7*2));
}


static void force_delay()
{
	for(int i=0; i<200000;i++);
}

