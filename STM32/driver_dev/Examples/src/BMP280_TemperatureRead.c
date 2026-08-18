/*
 * BMP280_TemperatureRead.c
 *
 *  Created on: Aug 15, 2026
 *      Author: hp
 */



#include <stdint.h>
#include <stdio.h>
#include "stm32f401re_spi_driver.h"
#include "stm32f401re_gpio_driver.h"

//define functions for slave select and deselcet
static void BMP_Select();
static void BMP_Deselect();
//define function for SPI pins configurations
static void SPI_GPIO_Configurations();
static void force_delay();



int App_main(void)
{
	//Configure GPIO pins for SPI1
	// GPIO Configuration
		// - SPI1_MOSI -> PA7 (D11)
		// - SPI1_MISO -> PA6 (D12)
		// - SPI1_SCK  -> PA5 (D13)
		// - SPI1_SS   -> PB6 (D10)

	SPI_GPIO_Configurations();

	//Configure SPI
	SPI_Handle_t spi1;

	spi1.pSPIx = SPI1;
	spi1.SPI_Config.SPI_Bus_Config = SPI_BUS_CONFIG_FD;
	spi1.SPI_Config.SPI_CLK_Speed = SPI_PRESCALAR_16;
	spi1.SPI_Config.SPI_Device_Mode = SPI_DEVICE_MODE_MASTER;
	spi1.SPI_Config.SPI_SSM = SPI_SSM_EN;
	spi1.SPI_Config.SPI_DataFrame = SPI_DFF_8BIT;

	SPI_Init(&spi1);

	SPI_PeripheralControl(SPI1, ENABLE);


    // 1) Reading Sensor ID
    //****************************************************************************************************
	// Reading sensor id of BMP280 : 0x58
	// Stored at register address  : 0xD0

	BMP_Select();
	// send register address
	SPI_Transmit_Byte(SPI1, 0xD0);
	// receive sensor id
	uint8_t sensor_id = SPI_Receive_Byte(SPI1);
	BMP_Deselect();



	// 2) Read 3 calibration values for temperature calculation
	//************************************************************************************************
	//    - These are 16 bit values (dig_t1,dig_t2,dig_t3)  at  register address: 0x88-0x8D (6 bytes)

		// Allocate a buffer for reading calibration constants
		uint8_t calib_values[6]={};

		BMP_Select();
		// send register address
		SPI_Transmit_Byte(SPI1, 0x88);
		SPI_Receive_Buffer(SPI1, calib_values, 6);
		BMP_Deselect();

		// Convert bytes to 16 bit values
		//    - Stored in little endian format
		//    - digt2 and digt3 are signed values
		uint16_t dig_t1 = ((uint16_t)calib_values[1]<< 8)| (((uint16_t)calib_values[0]));
		int16_t dig_t2 =  ((int16_t)calib_values[3]<< 8)| (((int16_t)calib_values[2]));
		int16_t dig_t3 =  ((int16_t)calib_values[5]<< 8)| (((int16_t)calib_values[4]));


		// 3) Configure control meas register
		//*********************************************************************************************************************
		// register address: 0xF4
		// Write 0x27 for setting configuration
		// - sets 0x01 temperature oversampling
		// - sets 0x16 pressure oversampling
		// - normal mode for continuous measurement

		BMP_Select();
		// send register address
		SPI_Transmit_Byte(SPI1, 0xF4);
		//send data to write
		SPI_Transmit_Byte(SPI1, 0x27);
		BMP_Deselect();



	while(1)
	{

		// 4) Read 20-bit raw ADC values continuously in loop
		//********************************************************************************************************************
		// register address: 0xFA-0xFC
		// data is stored in big-endian format (MSB first)
		// combine MSB,LSB,XLSB to form 20 bit value
		// ADC value= (MSB<<12)|(LSB<<8)|(XLSB>>4)

		uint8_t ADC_data[3];
		BMP_Select();
		// send register address
		SPI_Transmit_Byte(SPI1, 0xFA);
		SPI_Receive_Buffer(SPI1, ADC_data, 3);
		BMP_Deselect();
		uint32_t ADC_value= ((uint32_t)ADC_data[0]<<12)|((uint32_t)ADC_data[1]<<4)|((uint32_t)ADC_data[2]>>4);

		 // 5) calculate temperature:
		 //********************************************************************************************************************
		int32_t var1 = (((((int32_t)ADC_value>> 3) - ((int32_t)dig_t1 << 1))) * ((int32_t)dig_t2)) >> 11;
		int32_t var2 = ((((((int32_t)ADC_value >> 4) - (int32_t)dig_t1) * (((int32_t)ADC_value >> 4) - (int32_t)dig_t1)) >> 12) *(int32_t)dig_t3) >> 14;
		int32_t t_fine = var1 + var2;
	  	int32_t T = (t_fine * 5 + 128) >> 8;
        float temperature = T/100.0;


		printf("\nBMP Sensor Interface ");
		printf("\n_____________________\n\n");

		printf("Sensor ID            : 0x%0X\n", (unsigned int)sensor_id);
		printf("Calibration values   : \n");
		printf("   digt1             : %u\n", (unsigned int)dig_t1);
		printf("   digt2             : %d\n", (int)dig_t2);
		printf("   digt3             : %d\n", (int)dig_t3);
		printf("20-bit ADC Value     : %u\n", (unsigned int)ADC_value);
		printf("Temperature          : %.2f\n", temperature);

		force_delay();
	}

}


//production/reusable driver code,use PRIu16, PRId16 as format specifiers(<inttype.h>)
static void force_delay()
{
	for(int i=0; i<200000;i++);
}

static void SPI_GPIO_Configurations()
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

	// set PB6 as output pin for slave control
	GPIOB->MODER   &= ~(0x3<< 6*2);
	GPIOB->MODER   |= (1<< 6*2);
	// set PB6 as default high
	GPIOB->ODR     |= (1<< 6);

	// set alternate function for PA7,PA6,PA5
	GPIOA->AFR[0]  &= ~((0xF<< 7*4)|(0xF<< 6*4)|(0xF<< 5*4));
	GPIOA->AFR[0]  |= (0x5<< 7*4)|(0x5<< 6*4)|(0x5<< 5*4);

	// set HIGH speed for SPI pins
	GPIOA->OSPEEDR &= ~((0x3<< 7*2)| (0x3<< 6*2)|(0x3<< 5*2));
	GPIOA->OSPEEDR |= (0x3<< 7*2)| (0x3<< 6*2)|(0x3<< 5*2);

	// no pullup or pulldown
	GPIOA->PUPDR &=~((0x3<< 5*2)|(0x3<< 6*2)|(0x3<< 7*2));
}

static void BMP_Select()
{
	// reset PB6 for slave select
	GPIOB->ODR &=~ (1<< 6);
}
static void BMP_Deselect()
{
	GPIOB->ODR |= (1<< 6);
}


