/*
 * BMP280.c
 *
 *  Created on: Aug 15, 2026
 *      Author: hp
 */


#include "BMP280.h"

//define static-helper functions for Slave select and Deselect
static void BMP_Select(BMP280_t *bmp)
{

	bmp->interface.ss_port->ODR &= ~(1<< bmp->interface.ss_pin);

}
static void BMP_Deselect(BMP280_t *bmp)
{

	bmp->interface.ss_port->ODR |= (1<< bmp->interface.ss_pin);

}
//define static-helper functions for register read and write
static uint8_t BMP280_ReadRegister(BMP280_t *bmp, uint8_t Reg_address)
{
	BMP_Select(bmp);
	SPI_Transmit_Byte(bmp->interface.spi, Reg_address | BMP280_SPI_READ);
	uint8_t byte = SPI_Receive_Byte(bmp->interface.spi);
    BMP_Deselect(bmp);

    return byte;

}
static void BMP280_WriteRegister(BMP280_t *bmp, uint8_t Reg_address, uint8_t data)
{
	BMP_Select(bmp);
	SPI_Transmit_Byte(bmp->interface.spi, Reg_address & BMP280_SPI_WRITE);
	SPI_Transmit_Byte(bmp->interface.spi, data);
    BMP_Deselect(bmp);
}
static void BMP280_ReadRegisters(BMP280_t *bmp, uint8_t Reg_address, uint8_t *buffer, uint8_t len)
{
	BMP_Select(bmp);
	SPI_Transmit_Byte(bmp->interface.spi, Reg_address | BMP280_SPI_READ);
	SPI_Receive_Buffer(bmp->interface.spi, buffer, len);
    BMP_Deselect(bmp);

}

//define higher-level APIs

/*
 * fn     : BMP280_Init-
 *          Reads and store the chip ID
 *	        Reads and store calibration values
 *	        Sets default configurations on control meas register(0x1 oversampling and normal mode)
 *
 * param  : BMP280_t *bmp - pointer to BMP280_t structure
 *
 * return : none
 */
void BMP280_Init(BMP280_t *bmp)
{
	//1. Chip ID
	//*********************************************************************************************************
	bmp->ID = BMP280_ReadRegister(bmp, BMP280_REG_ID);

	//2. Calibration values
	//*********************************************************************************************************

	//temperature calibration:

	uint8_t tempCalibValues[6] = {};
	BMP280_ReadRegisters(bmp, BMP280_REG_TEMP_CALIB, tempCalibValues, 6);

	bmp->calib_values.dig_t1 = ((uint16_t)tempCalibValues[1]<< 8)| (((uint16_t)tempCalibValues[0]));
	bmp->calib_values.dig_t2 = ((int16_t)tempCalibValues[3]<< 8)| (((int16_t)tempCalibValues[2]));
	bmp->calib_values.dig_t3 = ((int16_t)tempCalibValues[5]<< 8)| (((int16_t)tempCalibValues[4]));

	//pressure calibration:

	uint8_t pressureCalibValues[18] = {};
	BMP280_ReadRegisters(bmp, BMP280_REG_PRESSURE_CALIB, pressureCalibValues, 18);

	bmp->calib_values.dig_P1 = ((uint16_t)pressureCalibValues[1]<< 8) | ((uint16_t)pressureCalibValues[0]);
	bmp->calib_values.dig_P2 = ((int16_t)pressureCalibValues[3]<< 8) | ((int16_t)pressureCalibValues[2]);
	bmp->calib_values.dig_P3 = ((int16_t)pressureCalibValues[5]<< 8) | ((int16_t)pressureCalibValues[4]);
	bmp->calib_values.dig_P4 = ((int16_t)pressureCalibValues[7]<< 8) | ((int16_t)pressureCalibValues[6]);
	bmp->calib_values.dig_P5 = ((int16_t)pressureCalibValues[9]<< 8) | ((int16_t)pressureCalibValues[8]);
	bmp->calib_values.dig_P6 = ((int16_t)pressureCalibValues[11]<< 8) | ((int16_t)pressureCalibValues[10]);
	bmp->calib_values.dig_P7 = ((int16_t)pressureCalibValues[13]<< 8) | ((int16_t)pressureCalibValues[12]);
	bmp->calib_values.dig_P8 = ((int16_t)pressureCalibValues[15]<< 8) | ((int16_t)pressureCalibValues[14]);
	bmp->calib_values.dig_P9 = ((int16_t)pressureCalibValues[17]<< 8) | ((int16_t)pressureCalibValues[16]);


	//3. Set default configurations
	//*********************************************************************************************************

	bmp->configurations.BMP_MeasurementMode         = BMP280_MODE_NORMAL;
	bmp->configurations.BMP_PressureOversampling    = BMP280_OVERSAMPLING_X01;
	bmp->configurations.BMP_TemperatureOversampling = BMP280_OVERSAMPLING_X01;

	//Combine the configurables into one single byte and write it to ctrl_meas register
	//Bit:  7 6 5  |  4 3 2  | 1 0
	//      ───────  ───────  ─────
	//      osrs_t    osrs_p   mode
		uint8_t ctrl_meas;
	ctrl_meas = bmp->configurations.BMP_TemperatureOversampling << 5 |
			    bmp->configurations.BMP_PressureOversampling    << 2 |
				bmp->configurations.BMP_MeasurementMode;

    BMP280_WriteRegister(bmp, BMP280_REG_CTRL_MEAS, ctrl_meas);


}
/*
 * fn: BMP280_Configure - For writing custom configurations into control meas register
 *
 * param  : BMP280_t *bmp - pointer to BMP280_t structure
 *
 * return : none
 */
void BMP280_Configure(BMP280_t *bmp)
{
	uint8_t ctrl_meas;

		//Set configurations:

		ctrl_meas = bmp->configurations.BMP_TemperatureOversampling << 5 |
				    bmp->configurations.BMP_PressureOversampling    << 2 |
					bmp->configurations.BMP_MeasurementMode;

	    BMP280_WriteRegister(bmp, BMP280_REG_CTRL_MEAS, ctrl_meas);

}
/**
 * @brief: Read and calculate the current temperature from the BMP280 sensor.
 *
 *         Reads the 20-bit raw temperature ADC value from the BMP280 temperature
 *         registers and converts it into a compensated temperature using the
 *         calibration coefficients stored in the BMP280_t structure.
 *
 *         The calculated temperature is stored in the `temperature` member of
 *         the BMP280_t structure in degrees Celsius. The calculated `t_fine`
 *         value is also stored in the structure for use by the pressure
 *         compensation algorithm.
 *
 * @param:  bmp Pointer to the BMP280 sensor structure.
 *
 * @return: None.
 */
void BMP280_ReadTemperature(BMP280_t *bmp)
{
	//read 20-bit temperature ADC value

	uint8_t tempData[3];

	BMP280_ReadRegisters(bmp, BMP280_REG_TEMP_MSB, tempData, 3);
	uint32_t adc_T= ((uint32_t)tempData[0]<<12)|((uint32_t)tempData[1]<<4)|((uint32_t)tempData[2]>>4);

	//calculate temperature using ADC value and calibration constants
	int32_t var1 = (((((int32_t) adc_T>> 3) - ((int32_t)bmp->calib_values.dig_t1 << 1))) * ((int32_t)bmp->calib_values.dig_t2)) >> 11;
	int32_t var2 = ((((((int32_t) adc_T >> 4) - (int32_t)bmp->calib_values.dig_t1) * (((int32_t) adc_T >> 4) - (int32_t)bmp->calib_values.dig_t1)) >> 12) *(int32_t)bmp->calib_values.dig_t3) >> 14;
	int32_t t_fine = var1 + var2;
	bmp->t_fine = t_fine;
  	int32_t T = (t_fine * 5 + 128) >> 8;

  	bmp->temperature = T/100.0;

}
/**
 * @brief:  Read and calculate the current pressure from the BMP280 sensor.
 *
 *          Reads the 20-bit raw pressure ADC value from the BMP280 pressure
 *          registers and converts it into a compensated pressure using the
 *          calibration coefficients and the `t_fine` value calculated during
 *          temperature compensation.
 *
 *          The calculated pressure is stored in the `pressure` member of the
 *          BMP280_t structure in Pascal (Pa).
 *
 * @param:  bmp Pointer to the BMP280 sensor structure.
 *
 * @return: None.
 *
 * @note:   BMP280_ReadTemperature() must be called before this function
 *          because the pressure compensation algorithm requires the
 *          temperature-derived `t_fine` value.
 */
void BMP280_ReadPressure(BMP280_t *bmp)
{
	//read 20-bit pressure ADC value

	uint8_t pressureData[3];
	BMP280_ReadRegisters(bmp, BMP280_REG_PRESSURE_MSB, pressureData, 3);
	uint32_t adc_P= ((uint32_t)pressureData[0]<<12)|((uint32_t)pressureData[1]<<4)|((uint32_t)pressureData[2]>>4);

	int64_t var1;
	int64_t var2;
	int64_t p;

	var1 = ((int64_t)bmp->t_fine) - 128000;

	var2 = var1 * var1 * (int64_t)bmp->calib_values.dig_P6;
	var2 = var2 + ((var1 * (int64_t)bmp->calib_values.dig_P5) << 17);
	var2 = var2 + (((int64_t)bmp->calib_values.dig_P4) << 35);

	var1 = ((var1 * var1 * (int64_t)bmp->calib_values.dig_P3) >> 8)
	     + ((var1 * (int64_t)bmp->calib_values.dig_P2) << 12);

	var1 = (((((int64_t)1) << 47) + var1) *
	        (int64_t)bmp->calib_values.dig_P1) >> 33;
	if (var1 == 0)
	{
	    bmp->pressure = 0.0f;
	    return;
	}

	p = 1048576 - adc_P;

	p = (((p << 31) - var2) * 3125) / var1;

	var1 = ((int64_t)bmp->calib_values.dig_P9 *
	        (p >> 13) * (p >> 13)) >> 25;

	var2 = ((int64_t)bmp->calib_values.dig_P8 * p) >> 19;

	p = ((p + var1 + var2) >> 8) +
	    ((int64_t)bmp->calib_values.dig_P7 << 4);

	bmp->pressure = p / 256.0f;

}













